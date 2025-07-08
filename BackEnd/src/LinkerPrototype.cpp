#include "../inc/emitters.hpp"

const char * shstrtab_alphabet = ".text\0.data";

void Link2Files (const char * likable_file)
{
    CodeBuffer_t cb      = {};
    CodeBuffer_t cb_data = {};

    code_buffer_init (&cb);
    code_buffer_init (&cb_data);

// TD закинуть их заполнение в отдельную функцию
    Elf64_Ehdr ehdr = 
    {
        // e_ident (первые 16 байт)
        .e_ident = {
            [EI_MAG0]       = ELFMAG0,
            [EI_MAG1]       = ELFMAG1,
            [EI_MAG2]       = ELFMAG2,
            [EI_MAG3]       = ELFMAG3,
            [EI_CLASS]      = ELFCLASS64,       // 64-битный файл
            [EI_DATA]       = ELFDATA2LSB,      // Little-endian 
            [EI_VERSION]    = EV_CURRENT,       // Версия ELF
            [EI_OSABI]      = ELFOSABI_LINUX,   // ОС: Linux
            [EI_ABIVERSION] = 0,                // ABI-версия
            // Остальные байты  0
        },
    
        .e_type      = ET_EXEC,                 // исполняемый файл
        .e_machine   = EM_X86_64,               // архитектура x86-64
        .e_version   = EV_CURRENT,              // версия
        .e_entry     = 0x4001000,               // адрес_start
        .e_phoff     = sizeof(Elf64_Ehdr),      // адрес phdr
        .e_shoff     = 0,                       // таблица секций
        .e_flags     = 0,                       // не используется
        .e_ehsize    = sizeof(Elf64_Ehdr),      // размер Elf64_Ehdr
        .e_phentsize = sizeof(Elf64_Phdr),      // размер Elf64_Phdr
        .e_phnum     = 2,                       // количество программных заголовков
        .e_shentsize = sizeof(Elf64_Shdr),      // размер Elf64_Shdr
        .e_shnum     = 0,                       // 2 секции (.text, .data)
        .e_shstrndx  = 0,                       // .shstrtab - нет
    };

    Elf64_Phdr text_segment =
    {
        .p_type     = PT_LOAD,
        .p_flags    = PF_R | PF_X,
        .p_offset   = 0x1000,
        .p_vaddr    = 0x4001000,
        .p_paddr    = 0x4001000,
        .p_filesz   = cb.offset,
        .p_memsz    = cb.offset,
        .p_align    = 0x1000
    };

    Elf64_Phdr data_segment =
    {
        .p_type     = PT_LOAD,
        .p_flags    = PF_R | PF_W,
        .p_offset   = 0x2000,
        .p_vaddr    = 0x4002000,
        .p_paddr    = 0x4002000,
        .p_filesz   = cb_data.offset,
        .p_memsz    = cb_data.offset,
        .p_align    = 0x1000
    };

    FILE * likbl_f = fopen (likable_file, "r");
    my_assert (likbl_f);

    Elf64_Ehdr ehdr_o           = {};
    Elf64_Shdr shstrtab_shdr_o  = {};
    Elf64_Shdr data_shdr_o      = {};
    Elf64_Shdr text_shdr_o      = {};
    Elf64_Shdr rela_text_shdr_o = {};

    fread (&ehdr_o, sizeof (Elf64_Ehdr), 1, likbl_f);

    fseek (likbl_f, ehdr_o.e_shoff + sizeof (Elf64_Shdr) * ehdr_o.e_shstrndx, SEEK_SET);
    fread (&shstrtab_shdr_o, sizeof (Elf64_Shdr), 1, likbl_f);

    char   seg_type[16] = {};
    size_t offset       = 0;

    for (size_t i = 0; ehdr_o.e_shnum > i; i++)
    {
        fseek (likbl_f, shstrtab_shdr_o.sh_offset + 1 + offset, SEEK_SET);
        fscanf (likbl_f, "%s", &seg_type);

        if (! strcmp (seg_type, ".data"))
        {
            fseek (likbl_f, ehdr_o.e_shoff + sizeof (Elf64_Shdr) * i, SEEK_SET);
            fread (&data_shdr_o, sizeof (Elf64_Shdr), 1, likbl_f);
            
            offset += sizeof (".data");
        }
        else if (! strcmp (seg_type, ".text"))
        {
            fseek (likbl_f, ehdr_o.e_shoff + sizeof (Elf64_Shdr) * i, SEEK_SET);
            fread (&text_shdr_o, sizeof (Elf64_Shdr), 1, likbl_f);
            
            offset += sizeof (".text");
        }
        else if (! strcmp (seg_type, ".rela.text"))
        {
            fseek (likbl_f, ehdr_o.e_shoff + sizeof (Elf64_Shdr) * i, SEEK_SET);
            fread (&text_shdr_o, sizeof (Elf64_Shdr), 1, likbl_f);
            
            offset += sizeof (".rela.text");
        }
        else if (! strcmp (seg_type, ".shstrtab"))
        {
            offset += sizeof (".shstrtab");
        }
        else if (! strcmp (seg_type, ".symtab"))
        {
            offset += sizeof (".symtab");

            COLOR_PRINT (YELLOW, "Warning! I can`t read .symtab section\n");
        }
        else if (! strcmp (seg_type, ".strtab"))
        {
            offset += sizeof (".strtab");

            COLOR_PRINT (YELLOW, "Warning! I can`t read .strtab section\n");
        }
        else
        {
            COLOR_PRINT (RED, "\nSorry, but now I can`t read this section today :((\n\n")

            exit (1);
        }
    }

    size_t start_offset = cb.offset;

    printf ("%c - segtype, offset = %d\n", seg_type, ehdr_o.e_shoff);

    return;
}

int CodeBufferWriteFromFile (CodeBuffer_t * cb, size_t len, FILE * file)
{
    while (cb->offset + len > cb->buffer_size)
    {
        cb->buffer_size *= 2;
        cb->buffer = (uint8_t*) realloc (cb->buffer, cb->buffer_size);
        if (cb->buffer == NULL)
        {
            fprintf (stderr, "Error: failed to realloc code buffer\n");
            exit (1);
        }
    }

    fread (cb->buffer + cb->offset, sizeof (uint8_t), len, file);

    cb->offset += len;

    return 0;
}

void ProcessLinkSegmet (FILE * lnkbl_f, CodeBuffer_t * cb, Elf64_Shdr * shdr_s, )
{

    Elf64_Rela rela_s = {};


}