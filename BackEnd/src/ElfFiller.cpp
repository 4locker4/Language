#include "../../inc/Language.h"
#include "../inc/AdditionalFuncs.hpp"
#include "../inc/emitters.hpp"
#include "../inc/Elf64Gen.hpp"
#include "../inc/EmittersDSL.hpp"

static int IF_COUNTER    = 0;
static int WHILE_COUNTER = 0;
static size_t N_GLOBAL_VARS = 2;
static size_t lable_counter = 0;
static NODE_WITH_STR nodes_with_strings = {};

static char STRINGS_NAME[32] = {0};

static const char * shstrtab_data = "\0.text\0.data\0.shstrtab\0.symtab\0.strtab\0.rela.text";

// Сюда с легкостью можно добавить разные виды relocation`ов. Только ввести их и делать | в if
// TD сделать номера секций

void ApplyRelocation (CodeBuffer_t * cb, ADDR_T * for_rel_t) 
{
    size_t max_relc = for_rel_t->rel_t.free_box;
    size_t max_symb = for_rel_t->sm_t.l_free_box;

    Elf64_Sym * target = NULL;

    for (int i = 1; max_relc > i; i++) 
    {
        for (int j = 1; max_symb > j; j++)
        {
            printf ("%s\n", for_rel_t->str_t.strtab + for_rel_t->sm_t.labels[j].st_name);

            if (! strcmp(for_rel_t->str_t.strtab + for_rel_t->sm_t.labels[j].st_name, for_rel_t->rel_t.target_names[i])) 
            {
                target = &for_rel_t->sm_t.labels[j];
                
                for_rel_t->rel_t.relocation[i].r_info |= ((j + 1) << 32);

                break;
            }
        }
        
        if (!target) 
        {
            printf ("Error: undefined symbol '%s'\n", for_rel_t->rel_t.target_names[i]);
            exit(1);
        }

        if (target->st_shndx != SHN_UNDEF)
        {
            for_rel_t->rel_t.relocation[i].r_addend = target->st_value - (for_rel_t->rel_t.relocation[i].r_offset + 4);
            memcpy (cb->buffer + for_rel_t->rel_t.relocation[i].r_offset, &for_rel_t->rel_t.relocation[i].r_addend, sizeof (uint64_t));
        }
        else
            for_rel_t->rel_t.relocation[i].r_addend = 0;
    }
}

void AddLabel (ADDR_T * for_rel_t, const char * label_name, int64_t address, char st_info, int64_t st_shndx)
{
    if (for_rel_t->sm_t.l_capacity == for_rel_t->sm_t.l_free_box)
    {
        for_rel_t->sm_t.l_capacity += START_TAB_CAPACITY;
        for_rel_t->sm_t.labels    = (Elf64_Sym *) realloc (for_rel_t->sm_t.labels, for_rel_t->sm_t.l_capacity * sizeof (Elf64_Sym));
    }

    for_rel_t->sm_t.labels[for_rel_t->sm_t.l_free_box].st_info  = st_info;
    for_rel_t->sm_t.labels[for_rel_t->sm_t.l_free_box].st_shndx = st_shndx;
    for_rel_t->sm_t.labels[for_rel_t->sm_t.l_free_box].st_name = for_rel_t->str_t.srt_id;
    for_rel_t->sm_t.labels[for_rel_t->sm_t.l_free_box].st_other = 0;
    for_rel_t->sm_t.labels[for_rel_t->sm_t.l_free_box].st_size = 0;
    for_rel_t->sm_t.labels[for_rel_t->sm_t.l_free_box++].st_value = address;

    size_t string_l = strlen (label_name) + 1;
    while (for_rel_t->str_t.str_capacity <= for_rel_t->str_t.srt_id + string_l)
    {
        for_rel_t->str_t.str_capacity *= 2;
        for_rel_t->str_t.strtab    = (char *) realloc (for_rel_t->str_t.strtab, for_rel_t->str_t.str_capacity * sizeof (char));
    }
    
    memcpy (for_rel_t->str_t.strtab + for_rel_t->str_t.srt_id, label_name, string_l);
    for_rel_t->str_t.srt_id += string_l;

    return;
}

void AddRelocation (RELOC_TABLE * rel_t, const char * target, Elf64_Addr offset)
{
    if (rel_t->capacity == rel_t->free_box)
    {
        rel_t->capacity += START_TAB_CAPACITY;
        rel_t->relocation = (Elf64_Rela *) realloc (rel_t->relocation, rel_t->capacity * sizeof (Elf64_Rela));
        rel_t->target_names = (char **) realloc (rel_t->target_names, rel_t->capacity * sizeof (char *));
    }

    rel_t->target_names[rel_t->free_box] = strdup (target);

    rel_t->relocation[rel_t->free_box].r_offset = offset;
    rel_t->relocation[rel_t->free_box++].r_info = R_X86_64_PC32 & 0xFFFFFFFF;
}

// Нужно знать количество глобальных функций
size_t MakeSymArray (SYMBOL_TABLE * sm_t)
{
    Elf64_Sym buf_sym = {};

    size_t n_globals = 0;
    size_t left      = 1;
    size_t right     = sm_t->l_free_box - 1;

    while (left <= right)
    {
        if (ELF64_ST_BIND(sm_t->labels[left].st_info) == STB_GLOBAL) {
            while (left <= right && ELF64_ST_BIND(sm_t->labels[right].st_info) == STB_GLOBAL)
                right--;

            if (left <= right) 
            {
                buf_sym             = sm_t->labels[left];
                sm_t->labels[left]  = sm_t->labels[right];
                sm_t->labels[right] = buf_sym;

                n_globals++;
                right--;
            }
        }

        left++;
    }

    return n_globals;
}

#define ELF64_ST_INFO(bind, type) (((bind) << 4) + ((type) & 0x0F))

void symbol_tab_init (ADDR_T * for_rel_t)
{
    for_rel_t->sm_t.l_capacity = START_TAB_CAPACITY;
    for_rel_t->sm_t.l_free_box = 1;
    for_rel_t->sm_t.labels   = (Elf64_Sym *) calloc (START_TAB_CAPACITY, sizeof (Elf64_Sym));

    for_rel_t->sm_t.labels[0] = {};                                               // Нулевая структура

    for_rel_t->rel_t.capacity       = START_TAB_CAPACITY;
    for_rel_t->rel_t.free_box       = 1;
    for_rel_t->rel_t.target_names   = (char **) calloc (START_TAB_CAPACITY, sizeof (char *));
    for_rel_t->rel_t.relocation     = (Elf64_Rela *) calloc (START_TAB_CAPACITY, sizeof (Elf64_Rela));

    for_rel_t->rel_t.relocation[0] = {};                                          // Нулевая структура

    for_rel_t->str_t.str_capacity = START_TAB_CAPACITY * 32 * sizeof (char);
    for_rel_t->str_t.strtab   = (char *) calloc (START_TAB_CAPACITY * 32, sizeof (char));

    for_rel_t->str_t.strtab[0] = '\0';                                             // Нулевой байт
    for_rel_t->str_t.srt_id = 1;
    
    return;
}

void CreateElf64File (TOKEN_TABLE * token_table, const char * file_name)
{
    CodeBuffer_t cb = {};
    CodeBuffer_t cb_data = {};

    code_buffer_init (&cb);
    code_buffer_init (&cb_data);

    ADDR_T for_rel_t = {};

    symbol_tab_init (&for_rel_t);

    ReadTreeBin (token_table, &cb, &cb_data, &for_rel_t);

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
            // Остальные байты  0
        },
    
        .e_type      = ET_REL,                  // исполняемый файл
        .e_machine   = EM_X86_64,               // архитектура x86-64
        .e_version   = EV_CURRENT,              // версия
        .e_entry     = 0,                       // адрес_start
        .e_phoff     = 0,                       // адрес phdr
        .e_shoff     = sizeof(Elf64_Shdr),      // таблица секций
        .e_flags     = 0,                       // не используется
        .e_ehsize    = sizeof(Elf64_Shdr),      // размер Elf64_Ehdr
        .e_phentsize = 0,                       // размер Elf64_Phdr
        .e_phnum     = 0,                       // количество программных заголовков
        .e_shentsize = sizeof(Elf64_Shdr),      // размер Elf64_Shdr
        .e_shnum     = 7,                       // 7 секций
        .e_shstrndx  = 3,                       // .shstrtab - нет
    };

    #define ELF64_ALIGN(current_offset, align) ((current_offset + align - 1) & ~(align - 1))

    Elf64_Shdr null_shdr = {0};

    Elf64_Shdr text_shdr = 
    {
        .sh_name = 1,
        .sh_type = SHT_PROGBITS,
        .sh_flags = SHF_ALLOC | SHF_EXECINSTR,                                            // Нечему исполняться, будет ошибка
        .sh_addr = 0,   
        .sh_offset = ELF64_ALIGN (sizeof (Elf64_Ehdr) + sizeof (Elf64_Shdr) * 7, 16),   // Потому что у нас ehdr, text_shdr, data_shdr
                                                                                        // shstrtab_shdr, rela_text_shdr
        .sh_size = cb.offset,
        .sh_link = 0,
        .sh_info = 0,
        .sh_addralign = 16,
        .sh_entsize = 0
    };
    
    Elf64_Shdr data_shdr = 
    {
        .sh_name = 1 + sizeof (".text"),
        .sh_type = SHT_PROGBITS,
        .sh_flags = SHF_ALLOC | SHF_WRITE,
        .sh_addr = 0, 
        .sh_offset = ELF64_ALIGN (text_shdr.sh_offset + text_shdr.sh_size, 4) + 4,
        .sh_size = cb_data.offset,
        .sh_link = 0,
        .sh_info = 0,                           // Для rela не забыть поменять
        .sh_addralign = 4,                      // Линкер ставит 4
        .sh_entsize = 0                         // Для rela тоже можно поменять
    };

    Elf64_Shdr shstrtab_shdr = 
    {
        .sh_name = 1 + sizeof (".text") + sizeof (".data"),
        .sh_type = SHT_STRTAB,
        .sh_flags = 0,
        .sh_addr  = 0,
        .sh_offset = data_shdr.sh_offset + data_shdr.sh_size + 1,
        .sh_size = 1 + sizeof (".text")      + sizeof (".data")   + sizeof (".shstrtab") + 
                       sizeof (".rela.text") + sizeof (".strtab") + sizeof (".symtab"),
        .sh_link = 0,
        .sh_info = 0,
        .sh_addralign = 1,
        .sh_entsize = 0                         
    };
        
    Elf64_Shdr symtab_shdr =
    {
        .sh_name = shstrtab_shdr.sh_name + sizeof (".shstrtab"),
        .sh_type = SHT_SYMTAB,
        .sh_flags = 0,
        .sh_addr  = 0,
        .sh_offset = ELF64_ALIGN (shstrtab_shdr.sh_offset + shstrtab_shdr.sh_size, 8) + 8,
        .sh_size = (for_rel_t.sm_t.l_free_box) * sizeof (Elf64_Sym),
        .sh_link = 5,                                                                         // Индекс секции symtab
        .sh_info = 0,                                                                         // Заполняю позже
        .sh_addralign = 8,
        .sh_entsize = sizeof (Elf64_Sym) 
    };

    Elf64_Shdr strtab_shdr = 
    {
        .sh_name = symtab_shdr.sh_name + sizeof (".symtab"),
        .sh_type = SHT_STRTAB,
        .sh_flags = 0,
        .sh_addr  = 0,
        .sh_offset = symtab_shdr.sh_offset + symtab_shdr.sh_size + 1,
        .sh_size = for_rel_t.str_t.srt_id,                              // 1й нулевой байт
        .sh_link = 0,
        .sh_info = 0,
        .sh_addralign = 1,
        .sh_entsize = 0
    };

    Elf64_Shdr rela_text_shdr =
    {
        .sh_name = strtab_shdr.sh_name + sizeof (".strtab"),
        .sh_type = SHT_RELA,
        .sh_flags = 0,
        .sh_addr  = 0,
        .sh_offset = ELF64_ALIGN (strtab_shdr.sh_offset + strtab_shdr.sh_size, 8) + 8,
        .sh_size = (for_rel_t.rel_t.free_box) * sizeof (Elf64_Rela),
        .sh_link = 4,                                                                   // Индекс секции symtab
        .sh_info = 1,                                                                   // Индекс секции text
        .sh_addralign = 8,
        .sh_entsize = sizeof (Elf64_Rela)                 
    };

//====================================== WRITE INTO FILE ======================================

    FILE * file = fopen (file_name, "wb");

    size_t n_globals = MakeSymArray (&for_rel_t.sm_t);

    symtab_shdr.sh_info = for_rel_t.sm_t.l_free_box - n_globals;

    COLOR_PRINT (RED, "START APPLYING\n");

    ApplyRelocation (&cb, &for_rel_t);

    COLOR_PRINT (YELLOW, "PROCESSING...\n");
    ApplyRelocation (&cb_data, &for_rel_t);

    COLOR_PRINT (GREEN, "ITS OK\n");

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=
    fwrite (&ehdr,           sizeof (Elf64_Ehdr), 1, file);             //
    fwrite (&null_shdr,      sizeof (Elf64_Shdr), 1, file);             //
    fwrite (&text_shdr,      sizeof (Elf64_Shdr), 1, file);             //
    fwrite (&data_shdr,      sizeof (Elf64_Shdr), 1, file);             //      Put headers and segments
    fwrite (&shstrtab_shdr,  sizeof (Elf64_Shdr), 1, file);             //
    fwrite (&symtab_shdr,    sizeof (Elf64_Shdr), 1, file);             //
    fwrite (&strtab_shdr,    sizeof (Elf64_Shdr), 1, file);             //
    fwrite (&rela_text_shdr, sizeof (Elf64_Shdr), 1, file);             //
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

    fseek (file, text_shdr.sh_offset, SEEK_SET);      // Put .text
    fwrite (cb.buffer, cb.offset, 1, file);

    fseek (file, data_shdr.sh_offset, SEEK_SET);      // Put .data
    fwrite (cb_data.buffer, cb_data.offset, 1, file);

    fseek (file, shstrtab_shdr.sh_offset, SEEK_SET);
    fwrite (shstrtab_data, shstrtab_shdr.sh_size, 1, file);

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=
    fseek (file, symtab_shdr.sh_offset, SEEK_SET);
    fwrite (for_rel_t.sm_t.labels, symtab_shdr.sh_size, 1, file);
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=
    fseek (file, strtab_shdr.sh_offset, SEEK_SET);
    fwrite (for_rel_t.str_t.strtab, strtab_shdr.sh_size, 1, file);
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=
    fseek (file, rela_text_shdr.sh_offset, SEEK_SET);
    fwrite (for_rel_t.rel_t.relocation, rela_text_shdr.sh_size, 1, file);
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

    //Link2Files (&cb, "./BackEnd/src/Printf.o");

    return;
}

void ReadTreeBin (TOKEN_TABLE * token_table, CodeBuffer_t * cb, CodeBuffer_t * cb_data, ADDR_T * for_rel_t)
{
    NODE * root = token_table->tree;

    nodes_with_strings.nodes_with_text = (NODE **) calloc (DELTA_NODE_WITH_STR, sizeof (NODE *));

//====================================== START CODEGEN ======================================

    AddLabel (for_rel_t, "MyPrintf", cb->offset, ELF64_ST_INFO (STB_GLOBAL, STT_FUNC), SHN_UNDEF);
    AddLabel (for_rel_t, "_start", cb->offset, ELF64_ST_INFO (STB_GLOBAL, STT_FUNC), TEXT_SEG);

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+
    emit_call_func (cb, root->left->data.ident.ident_name, &for_rel_t->rel_t);      //
    emit_mov_reg_imm (cb, RAX, 60);                                                 // Call entry func and end prog.
    emit_mov_reg_imm (cb, RDI, 0);                                                  //
    emit_syscall (cb);                                                              //
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+

    while (root->data.op != END)                        // Start func generation
    {
        InitAsmFuncBin (root->left, cb, for_rel_t);

        root = root->right;
    }

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+
    SqrtByNewton (cb, for_rel_t);      // Sqrt func                                // Additional funcs
    InputFunc    (cb, for_rel_t);      // Input num func                           //
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+

    size_t end_of_text_seg = cb->offset;

// =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+= START SECTION .DATA =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

#define MAKE_LABEL(str, num)    STRINGS_NAME[32] = {0};                                        \
                                snprintf(STRINGS_NAME, sizeof(STRINGS_NAME), str, num);

// =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+ END DEFINES +=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

    size_t n_strings = nodes_with_strings.free_box;
    size_t lenght = 0;
    
    for (size_t i = 0; n_strings > i; i++)
    {
        MAKE_LABEL ("STRI_NUM_%d", nodes_with_strings.nodes_with_text[i]->data.ident.ident_num)
        AddLabel (for_rel_t, STRINGS_NAME, cb->offset + cb_data->offset, ELF64_ST_INFO (STB_LOCAL, STT_OBJECT), DATA_SEG);

        lenght = strlen (nodes_with_strings.nodes_with_text[i]->data.ident.ident_name) + 1;

        code_buffer_write (cb_data, (uint8_t *) nodes_with_strings.nodes_with_text[i]->data.ident.ident_name, lenght);
        code_buffer_write (cb_data, (uint8_t *) "\0", 1);
    }

    AddLabel (for_rel_t, "INPUT_BUFFER", cb->offset + cb_data->offset, ELF64_ST_INFO (STB_LOCAL, STT_OBJECT), DATA_SEG);           // For input func

    uint8_t bites[SIZE_OF_INPUT_BUFFER] = {0};
    code_buffer_write (cb_data, bites, SIZE_OF_INPUT_BUFFER);

    free (nodes_with_strings.nodes_with_text);

    return;
}

NODE * InitAsmFuncBin (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    my_assert (node);
    
    AddLabel (for_rel_t, node->data.ident.ident_name, cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    POP_REG (R14)
    PUSH_REG(RSP)
    POP_REG (R15)
    ADD_REG_NUM (R15, (node->data.ident.ident_val.unsigned_type - 1) * 8)
    MOV_REG_REG (R13, R15)
    SUB_REG_NUM (R13, node->data.ident.ident_num * 8)
    PUSH_REG (R13)
    POP_REG (RSP)

    RecursyTreeReadBin (node->right, cb, for_rel_t);

    return node->right;
}

#define ADD_STRING_INTO_TABLE()     nodes_with_strings.nodes_with_text[nodes_with_strings.free_box++] = node;                                                       \
                                                                                                                                                                    \
                                    if (nodes_with_strings.free_box == nodes_with_strings.size)                                                                     \
                                    {                                                                                                                               \
                                        nodes_with_strings.size += DELTA_NODE_WITH_STR;                                                                             \
                                                                                                                                                                    \
                                        nodes_with_strings.nodes_with_text = (NODE **) realloc (nodes_with_strings.nodes_with_text, nodes_with_strings.size * 8);   \
                                    }                                                                                                                               \

NODE * RecursyTreeReadBin (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    my_assert (node);

    switch (node->node_type)
    {
        case NUM:
        {
            PUSH_NUM (node->data.val)

            return NULL;
        }
        case FUNC_CALL:
        {
            PUSH_REG (R15)
            PUSH_REG (R14)
            PUSH_REG (RSP)

            if (! strncmp (node->data.ident.ident_name, "MyPrintf", sizeof ("MyPrintf")))      
            {
                COLOR_PRINT (RED, "ITS MY PRINTF\n");
                
                size_t n_args = 0;

                ReverseReadFuncArgsBin (node->left, cb, &n_args, for_rel_t);
                
                EMIT_CALL_FUNC ("MyPrintf")

                POP_REG (RAX)
                PUSH_REG (RSP)
                POP_REG (RBX)
                ADD_REG_NUM (RBX, (n_args + 1) * 8)
                PUSH_REG (RBX)
                POP_REG (RSP)
                POP_REG (R14)
                POP_REG (R15)
                PUSH_REG (RAX)
            }
            else
            {
                ReadFuncArgsBin (node->left, cb, for_rel_t);

                EMIT_CALL_FUNC (node->data.ident.ident_name)

                POP_REG (RAX)
                PUSH_MEM (R15, 8)
                POP_REG (RSP)
                POP_REG (R14)
                POP_REG (R15)
                PUSH_REG (RAX)
            }

            return NULL;
        }
        case STRING:
        {
            ADD_STRING_INTO_TABLE ();

            MAKE_LABEL ("STRI_NUM_%d", node->data.ident.ident_num);

            COLOR_PRINT (MANGETA "LEA\t");

            LEA_REG_LABEL (RSI, STRINGS_NAME)
            PUSH_REG (RSI)

            return NULL;
        }
        case PARAM:
        case IDENT:
        {
            PUSH_MEM (R15, -1 * node->data.ident.ident_num * 8)

            return NULL;
        }
        case OP:
        {
            GenOpCodeBin (node, cb, for_rel_t);

            return NULL;
        }
        default:
        {
            printf ("ERROR in code gen\n");

            exit (-1);
        }
    }
}

void ReadFuncArgsBin (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    my_assert (node);

    RecursyTreeReadBin (node, cb, for_rel_t);

    if (node->left) ReadFuncArgsBin (node->left, cb, for_rel_t);

    return;
}

void ReverseReadFuncArgsBin (NODE * node, CodeBuffer_t * cb, size_t * n_args, ADDR_T * for_rel_t)
{
    my_assert (node);

    *n_args += 1;

    if (node->left) ReverseReadFuncArgsBin (node->left, cb, n_args, for_rel_t);

    RecursyTreeReadBin (node, cb, for_rel_t);

    return;
}

#define EXPR_ASM()                                         \
    RecursyTreeReadBin (node->left, cb, for_rel_t);      \
    RecursyTreeReadBin (node->right, cb, for_rel_t);     \
                                                           \
    XOR_REG_REG (R11, R11)                                 \
    XOR_REG_REG (R12, R12)                                 \
    POP_REG (R11)                                          \
    POP_REG (R12)

void GenOpCodeBin (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    my_assert (node);


    switch (node->data.op)
    {
        case NODE_BUNCH:
        {
            RecursyTreeReadBin (node->left, cb, for_rel_t);

            RecursyTreeReadBin (node->right, cb, for_rel_t);

            break;
        }
        case SIN:
        {
            RecursyTreeReadBin (node->left, cb, for_rel_t);

            printf ("sin\n");

            printf ("; end SIN\n\n");

            break;
        }
        case COS:
        {
            RecursyTreeReadBin (node->left, cb, for_rel_t);

            printf ("cos\n");

            printf ("; end COS\n\n");

            break;
        }
        case POW:
        {
            RecursyTreeReadBin (node->left, cb, for_rel_t);

            RecursyTreeReadBin (node->right, cb, for_rel_t);

            POP_REG (RCX)
            POP_REG (RDX)
            MOV_REG_IMM (RAX, 1)
            MAKE_LABEL ("loop_%d", lable_counter)
            AddLabel (for_rel_t, STRINGS_NAME, cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);
            IMUL_REG_REG (RAX, RDX)
            SUB_REG_NUM (RCX, 1)
            CMP_REG_NUM (RCX, 0)

            JG (STRINGS_NAME)

            PUSH_REG (RAX)

            lable_counter++;

            break;
        }
        case SQRT:
        {
            RecursyTreeReadBin (node->left, cb, for_rel_t);

            EMIT_CALL_FUNC ("sqrt_by_Newton")

            break;
        }
        case LN:
        {
            printf ("\n; Start taking the naturel logarithm\n\n");

            RecursyTreeReadBin (node->left, cb, for_rel_t);

            printf ("ln\n\n");

            printf ("; end LN\n\n");

            break;
        }
        case ADD:
        {
            EXPR_ASM()

            ADD_REG_REG (R12, R11)
            PUSH_REG (R12)

            break;
        }
        case SUB:
        {
            EXPR_ASM()

            SUB_REG_REG (R12, R11)
            PUSH_REG (R12)

            break;
        }
        case MUL:
        {
            RecursyTreeReadBin (node->left, cb, for_rel_t);

            RecursyTreeReadBin (node->right, cb, for_rel_t);

            POP_REG (R11)
            POP_REG (RAX)
            IMUL_REG_REG (RAX, R11)
            PUSH_REG (RAX)

            break;
        }
        case DIV:
        {
            RecursyTreeReadBin (node->left, cb, for_rel_t);

            RecursyTreeReadBin (node->right, cb, for_rel_t);

            POP_REG (R11)
            POP_REG (RAX)
            CQO ()
            IDIV_REG (R11)
            PUSH_REG (RAX)

            break;
        }
        case EQUALS:
        {
            int ident_num = node->left->data.ident.ident_num;
            
            RecursyTreeReadBin (node->right, cb, for_rel_t);

            POP_MEM (R15, -1 * node->left->data.ident.ident_num * 8)

            break;
        }
        case IF:
        {
            IfCodeGenBin (node, cb, for_rel_t);

            break;
        }
        case WHILE:
        {
            WhileCodeGenBin (node, cb, for_rel_t);

            break;
        }
        case END:
            break;
        case INPUT:
        {
            PUSH_REG (RBX)
            PUSH_REG (R14)
            PUSH_REG (RSI)
            PUSH_REG (RDI)
            PUSH_REG (RDX)

            EMIT_CALL_FUNC ("input_func")

            POP_REG (RAX)
            POP_REG (RDX)
            POP_REG (RDI)
            POP_REG (RSI)
            POP_REG (R14)
            POP_REG (RBX)
            PUSH_REG (RAX)

            break;
        }
        case RETURN:
        {
            PUSH_MEM (R15, -1 * node->left->data.ident.ident_num * 8)
            PUSH_REG (R14)
            RET ()
            
            node = node->right;

            break;
        }
        default:
        {
            printf ("I cant do this becouse node data op is: %c\n", node->data.op);

            exit (-1);
        }
    }
}

void IfCodeGenBin (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    my_assert (node);

    int end_if = IF_COUNTER++;

    int end_if_label_body = 0;

    end_if_label_body = IF_COUNTER++;

    AsmConditionalBin (node->left, cb, IF, end_if_label_body, for_rel_t);

    RecursyTreeReadBin (node->right, cb, for_rel_t);

    MAKE_LABEL ("if_%d", end_if_label_body);
    AddLabel (for_rel_t, STRINGS_NAME, cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    return;
}

void AsmConditionalBin (NODE * node, CodeBuffer_t * cb, OPERATORS mode, int label, ADDR_T * for_rel_t)
{
    my_assert (node);


    int num_of_label = IF_COUNTER;

    if (node->node_type == OP && (node->data.op == OR || node->data.op == AND))
    {
        if (node->left) AsmConditionalBin (node->left, cb, mode, label, for_rel_t);

        if (node->right) AsmConditionalBin (node->right, cb, mode, label, for_rel_t);

        if (node->data.op == OR)
        {
            MAKE_LABEL ("if_%d", num_of_label)
            AddLabel (for_rel_t, STRINGS_NAME, cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);
        }
    }
    else
    {
        ENUM_IDENT_DATA_TYPE ident_tp = SIGNED_INT;

        if (node->left->node_type == IDENT) ident_tp = node->left->data.ident.ident_data_type;

        RecursyTreeReadBin (node->left, cb, for_rel_t);

        if (node->right)
        {
            if (ident_tp == SIGNED_INT) ;
            else if (node->right->node_type == IDENT) ident_tp = node->left->data.ident.ident_data_type;

            RecursyTreeReadBin (node->right, cb, for_rel_t);
        }
        else PUSH_NUM (0)

        AsmOpsCompareBin (node, cb, mode, label, ident_tp, for_rel_t);
    }

    return;
}

#define PRINT_LABEL(jmp)                                        \
                        {                                       \
                            POP_REG (R11)                       \
                            POP_REG (R12)                       \
                            CMP_REG_REG (R12, R11)              \
                                                                \
                            if (mode == WHILE)                  \
                            {                                   \
                                MAKE_LABEL ("while_%d", label);\
                                jmp (STRINGS_NAME)              \
                            }                                   \
                            else                                \
                            {                                   \
                                MAKE_LABEL ("if_%d", label);   \
                                jmp (STRINGS_NAME)              \
                            }                                   \
                        }

void AsmOpsCompareBin (NODE * node, CodeBuffer_t * cb, OPERATORS mode, int label, ENUM_IDENT_DATA_TYPE data_type, ADDR_T * for_rel_t)
{   
    my_assert (node);

    switch (node->data.op)
    {
        case MORE:
        {
            if (data_type == SIGNED_INT || data_type == DOUBLE)
                PRINT_LABEL (JLE)
            else
                PRINT_LABEL (JBE)

            break;
        }
        case LESS:
        {
            if (data_type == SIGNED_INT || data_type == DOUBLE)
                PRINT_LABEL (JGE)
            else
                PRINT_LABEL (JAE)

            break;
        }
        case GREATER_OR_EQ:
        {
            if (data_type == SIGNED_INT || data_type == DOUBLE)
                PRINT_LABEL (JL)
            else
                PRINT_LABEL (JB)

            break;
        }
        case LESS_OR_EQ:
        {
            if (data_type == SIGNED_INT || data_type == DOUBLE)
                PRINT_LABEL (JG)
            else
                PRINT_LABEL (JA)

            break;
        }
        case EQ_EQUALS:
        {
            PRINT_LABEL (JNE);

            break;
        }
        default:
        {
            break;
        }
    }
}

#undef PRINT_LABEL

void WhileCodeGenBin (NODE * node, CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    my_assert (node);


    int start_label_body = WHILE_COUNTER++;
    int end_label_body   = WHILE_COUNTER;

    AsmConditionalBin (node->left, cb, WHILE, end_label_body, for_rel_t);

    WHILE_COUNTER++;

    RecursyTreeReadBin (node->right, cb, for_rel_t);

    MAKE_LABEL ("while_%d", start_label_body);
    JMP (STRINGS_NAME)

    MAKE_LABEL ("while_%d", end_label_body);
    AddLabel (for_rel_t, STRINGS_NAME, cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);
    return;
}

void InputFunc (CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    AddLabel (for_rel_t, "input_func", cb->offset, ELF64_ST_INFO (STB_GLOBAL, STT_FUNC), TEXT_SEG);

    MOV_REG_IMM (RAX, 0)
    MOV_REG_IMM (RDI, 0)
    LEA_REG_LABEL (RSI, "INPUT_BUFFER")
    MOV_REG_IMM (RDX, 32)
    SYSCALL ()

    XOR_REG_REG (RAX, RAX)
    XOR_REG_REG (RBX, RBX)
    XOR_REG_REG (R8, R8)
    LEA_REG_LABEL (RSI, "INPUT_BUFFER")

    MOV_R8_MEM (RBX, RSI, 0)
    CMP_R8_IMM8 (RBX, '-')
    JNE ("loop_input")
    MOV_REG_IMM (R8, 1)
    INC_REG (RSI)

    AddLabel (for_rel_t, "loop_input", cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    MOV_R8_MEM (RBX, RSI, 0)
    CMP_R8_IMM8 (RBX, 0)
    JE ("success")
    
    CMP_R8_IMM8 (RBX, 10)
    JE ("success")

    SUB_R8_IMM8 (RBX, '0')
    CMP_R8_IMM8 (RBX, 9)
    JA ("error")

    IMUL_REG_NUM (RAX, 10)
    ADD_REG_REG (RAX, RBX)
    INC_REG (RSI)

    JMP ("loop_input")

    AddLabel (for_rel_t, "success", cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    CMP_REG_NUM (R8, 1)
    JNE ("posit")

    NEG_REG (RAX)

    AddLabel (for_rel_t, "posit", cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    POP_REG (RBX)
    PUSH_REG (RAX)
    PUSH_REG (RBX)
    RET ()

    AddLabel (for_rel_t, "error", cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    MOV_REG_IMM (RAX, -1)
    RET ()
}

void SqrtByNewton (CodeBuffer_t * cb, ADDR_T * for_rel_t)
{
    AddLabel (for_rel_t, "sqrt_by_Newton", cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    POP_REG (R9)
    POP_REG (RAX)

    MOV_REG_REG (R8, RAX)
    MOV_REG_REG (RCX, RAX)

    AddLabel (for_rel_t, "newton_loop", cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    MOV_REG_REG (RAX, R8)
    XOR_REG_REG (RDX, RDX)
    IDIV_REG (RCX)
    ADD_REG_REG (RAX, RCX)
    SHR_REG_IMM (RAX, 1)
    CMP_REG_REG (RAX, RCX)
    JAE ("done")

    MOV_REG_REG (RCX, RAX)
    JMP ("newton_loop")

    AddLabel (for_rel_t, "done", cb->offset, ELF64_ST_INFO (STB_LOCAL, STT_FUNC), TEXT_SEG);

    PUSH_REG (RCX)
    PUSH_REG (R9)
    RET ()
}