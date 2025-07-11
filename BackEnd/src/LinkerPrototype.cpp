#include "../inc/emitters.hpp"

const char * shstrtab_alphabet = ".text\0.data";

static uint32_t SEG_ALIGN = 0x1000;

#define ELF64_ALIGN(current_offset, align) ((current_offset + align - 1) & ~(align - 1))

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

    Elf64_Ehdr ehdr_o           = {};
    Elf64_Shdr text_shdr_o      = {};
    Elf64_Shdr data_shdr_o      = {};
    Elf64_Shdr shstrtab_shdr_o  = {};
    Elf64_Shdr symtab_shdr_o    = {};
    Elf64_Shdr strtab_shdr_o    = {};
    Elf64_Shdr rela_text_shdr_o = {};

    FILE * likbl_f = fopen (likable_file, "r");
    my_assert (likbl_f);

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
            fseek (likbl_f, ehdr_o.e_shoff + sizeof (Elf64_Shdr) * i, SEEK_SET);
            fread (&symtab_shdr_o, sizeof (Elf64_Shdr), 1, likbl_f);

            offset += sizeof (".symtab");
        }
        else if (! strcmp (seg_type, ".strtab"))
        {
            fseek (likbl_f, ehdr_o.e_shoff + sizeof (Elf64_Shdr) * i, SEEK_SET);
            fread (&strtab_shdr_o, sizeof (Elf64_Shdr), 1, likbl_f);

            offset += sizeof (".strtab");
        }
        else
        {
            COLOR_PRINT (RED, "\nSorry, but now I can`t read this section today :((\n\n")

            exit (1);
        }
    }

    Elf64_Phdr text_segment =
    {
        .p_type     = PT_LOAD,
        .p_flags    = PF_R | PF_X,
        .p_offset   = ALIGN,
        .p_vaddr    = 0x4001000,
        .p_paddr    = 0x4001000,
        .p_filesz   = cb.offset,
        .p_memsz    = cb.offset,
        .p_align    = ALIGN
    };

    Elf64_Phdr data_segment =
    {
        .p_type     = PT_LOAD,
        .p_flags    = PF_R | PF_W,
        .p_offset   = ELF64_ALIGN (text_segment.p_offset + text_segment.p_memsz, ALIGN),
        .p_vaddr    = ELF64_ALIGN (text_segment.p_vaddr + text_segment.p_memsz, ALIGN),
        .p_paddr    = ELF64_ALIGN (text_segment.p_paddr + text_segment.p_memsz, ALIGN),
        .p_filesz   = cb_data.offset,
        .p_memsz    = cb_data.offset,
        .p_align    = ALIGN
    };

    CodeBuffer_t txt_cb = {};
    CodeBuffer_t dt_cb  = {};

    code_buffer_init (&txt_cb);
    code_buffer_init (&dt_cb);
    
    
    printf ("%c - segtype, offset = %d\n", seg_type, ehdr_o.e_shoff);

    return;
}

void CodeBufferWriteFromFile (CodeBuffer_t * cb, FILE * file, size_t len, size_t offset)
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

    fseek (file, offset, SEEK_SET);
    fread (cb->buffer, len, 1, file);

    cb->offset += len;

    return;
}

void ProcessLinkSegmet (FILE * file, Elf64_Shdr * shdr_o_to_fill, size_t offset)
{

    CodeBuffer_t cb = {};
    code_buffer_init (&cb);



    cb->buffer_size 
}