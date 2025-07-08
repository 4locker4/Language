#include "../../inc/Language.h"
#include "../inc/AdditionalFuncs.hpp"
#include "../inc/emitters.hpp"
#include "../inc/Elf64Gen.hpp"
#include "../inc/EmittersDSL.hpp"

static int IF_COUNTER    = 0;
static int WHILE_COUNTER = 0;

static size_t lable_counter = 0;
static NODE_WITH_STR nodes_with_strings = {};
static char STRINGS_NAME[32] = {0};

static const char * shstrtab_data = "\0.text\0.data\0.shstrtab\0.symtab\0.strtab\0.rela.text";

// Сюда с легкостью можно добавить разные виды relocation`ов. Только ввести их и делать | в if

void ApplyRelocations (CodeBuffer_t * cb, Elf64_Rela * rela_table) 
{
    size_t max_relc = cb->relc_tab.free_box;
    size_t max_symb = cb->symb_tab.free_box;

    RELOCATION * reloc = NULL;
        
    SYMBOL * target = NULL;

    for (int i = 0; max_relc > i; i++) 
    {
        reloc = &cb->relc_tab.relocation[i];
        target = NULL;

        rela_table[i].r_offset = reloc->patch_offset;

        for (int j = 0; max_symb > j; j++) {
            if (! strcmp(cb->symb_tab.labels[j].label_name, reloc->target)) 
            {
                target = &cb->symb_tab.labels[j];
                rela_table[i].r_info = (j >> 32) | R_X86_64_PC32;

                break;
            }
        }
        
        if (!target) 
        {
            printf ("Error: undefined symbol '%s'\n", reloc->target);
            exit(1);
        }

        rela_table[i].r_addend = target->address - (reloc->patch_offset);
    }
}

void AddLabel (SYMBOL_TABLE  * symbol_tab, const char * label_name, int64_t address)
{
    if (symbol_tab->capacity == symbol_tab->free_box)
    {
        symbol_tab->capacity += START_TAB_CAPACITY;
        symbol_tab->labels    = (SYMBOL *) realloc (symbol_tab->labels, symbol_tab->capacity * sizeof (SYMBOL));
    }
    
    symbol_tab->labels_len += strlen (label_name) + 1;

    symbol_tab->labels[symbol_tab->free_box].label_name = strdup (label_name);
    COLOR_PRINT (MANGETA, "%s - label\n", symbol_tab->labels[symbol_tab->free_box].label_name);
    symbol_tab->labels[symbol_tab->free_box++].address  = address;
}

void AddRelocation (RELOC_TABLE * reloc_table, const char * target, int64_t patch_pos)
{
    if (reloc_table->capacity == reloc_table->free_box)
    {
        reloc_table->capacity += START_TAB_CAPACITY;
        reloc_table->relocation = (RELOCATION *) realloc (reloc_table->relocation, reloc_table->capacity * sizeof (RELOCATION));
    }

    reloc_table->relocation[reloc_table->free_box].patch_offset = patch_pos;
    reloc_table->relocation[reloc_table->free_box++].target = strdup (target);
}

// TD Важно! Проверить еще раз, что lenght достаточно для копирования всех меток.
char * MakeAllLabelsString (SYMBOL_TABLE * symtab, size_t lenght)
{
    char * all_labels_string = (char *) calloc (sizeof (char), lenght);
    char * pointer = all_labels_string;

    for (size_t i = 0; symtab->free_box > i; i++)
    {
        strcpy (pointer, symtab->labels[i].label_name);

        pointer += strlen (symtab->labels[i].label_name) + 1;
    }
    
    return all_labels_string;
}

void CreateElf64File (TOKEN_TABLE * token_table, const char * file_name)
{
    CodeBuffer_t cb = {};
    CodeBuffer_t cb_data = {};

    code_buffer_init (&cb);
    code_buffer_init (&cb_data);

    ReadTreeBin (token_table, &cb, &cb_data);

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
        .e_version   = 0,                       // версия
        .e_entry     = 0,                       // адрес_start
        .e_phoff     = 0,                       // адрес phdr
        .e_shoff     = sizeof(Elf64_Shdr),      // таблица секций
        .e_flags     = 0,                       // не используется
        .e_ehsize    = sizeof(Elf64_Shdr),      // размер Elf64_Ehdr
        .e_phentsize = 0,                       // размер Elf64_Phdr
        .e_phnum     = 0,                       // количество программных заголовков
        .e_shentsize = sizeof(Elf64_Shdr),      // размер Elf64_Shdr
        .e_shnum     = 6,                       // 6 секций
        .e_shstrndx  = 2,                       // .shstrtab - нет
    };

    Elf64_Shdr text_shdr = 
    {
        .sh_name = 1,
        .sh_type = SHT_PROGBITS,
        .sh_flags = SHF_ALLOC | SHF_EXECINSTR,
        .sh_addr = ALIGN,
        .sh_offset = sizeof (Elf64_Ehdr) + sizeof (Elf64_Shdr) * 6, // Потому что у нас ehdr, text_shdr, data_shdr
                                                                    // shstrtab_shdr, rela_text_shdr
        .sh_size = cb.offset,
        .sh_link = 0,
        .sh_info = 0,                           // Для rela не забыть поменять
        .sh_addralign = 16,
        .sh_entsize = 0
    };

    Elf64_Shdr data_shdr = 
    {
        .sh_name = 1 + sizeof (".text"),
        .sh_type = SHT_PROGBITS,
        .sh_flags = SHF_ALLOC | SHF_WRITE,
        .sh_addr = ALIGN + ((cb.offset + ALIGN - 1) & ~(ALIGN - 1)), 
        .sh_offset = text_shdr.sh_offset + text_shdr.sh_size,
        .sh_size = cb_data.offset,
        .sh_link = 0,
        .sh_info = 0,                           // Для rela не забыть поменять
        .sh_addralign = 8,
        .sh_entsize = 0                         // Для rela тоже можно поменять
    };

    Elf64_Shdr shstrtab_shdr = 
    {
        .sh_name = 1 + sizeof (".text") + sizeof (".data"),
        .sh_type = SHT_STRTAB,
        .sh_flags = 0,
        .sh_addr  = 0,
        .sh_offset = data_shdr.sh_offset + data_shdr.sh_size,
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
        .sh_offset = shstrtab_shdr.sh_offset + shstrtab_shdr.sh_size,
        .sh_size = cb.symb_tab.free_box * sizeof (Elf64_Sym),
        .sh_link = 4,                                                                   // Индекс секции symtab
        .sh_info = 0,                                                                   // Индекс первого нелокального символа (не делал локальных меток), а под 0 будет 0
        .sh_addralign = 8,
        .sh_entsize = sizeof (Elf64_Sym) 
    };

    Elf64_Shdr strtab_shdr = 
    {
        .sh_name = symtab_shdr.sh_name + sizeof (".symtab"),
        .sh_type = SHT_STRTAB,
        .sh_flags = 0,
        .sh_addr  = 0,
        .sh_offset = symtab_shdr.sh_offset + symtab_shdr.sh_size,
        .sh_size = cb.symb_tab.labels_len,
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
        .sh_offset = strtab_shdr.sh_offset + strtab_shdr.sh_size,
        .sh_size = cb.relc_tab.free_box * sizeof (Elf64_Rela),
        .sh_link = 3,                                                                   // Индекс секции symtab
        .sh_info = 1,                                                                   // Индекс секции text
        .sh_addralign = 8,
        .sh_entsize = sizeof (Elf64_Rela)                 
    };

//====================================== WRITE INTO FILE ======================================

    FILE * file = fopen (file_name, "w+");

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=
    fwrite (&ehdr,           sizeof (Elf64_Ehdr), 1, file);             //
    fwrite (&text_shdr,      sizeof (Elf64_Shdr), 1, file);             //
    fwrite (&data_shdr,      sizeof (Elf64_Shdr), 1, file);             //
    fwrite (&shstrtab_shdr,  sizeof (Elf64_Shdr), 1, file);             //      Put headers and segments
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

    Elf64_Sym * sym_table = (Elf64_Sym *) calloc (cb.symb_tab.free_box, sizeof (Elf64_Sym));
    
    fseek (file, symtab_shdr.sh_offset, SEEK_SET);
    fwrite (cb.symb_tab.labels, symtab_shdr.sh_size, 1, file);

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=
    char * all_labels_string = MakeAllLabelsString (&cb.symb_tab, cb.symb_tab.labels_len);

    fseek (file, strtab_shdr.sh_offset, SEEK_SET);
    fwrite (all_labels_string, strtab_shdr.sh_size, 1, file);

    free (all_labels_string);
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=
    Elf64_Rela * rela_table = (Elf64_Rela *) calloc (cb.relc_tab.free_box, sizeof (Elf64_Rela));
    ApplyRelocations (&cb, rela_table);

    fseek (file, rela_text_shdr.sh_offset, SEEK_SET);
    fwrite (rela_table, rela_text_shdr.sh_size, 1, file);

    free (rela_table);
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+=

    //Link2Files (&cb, "./BackEnd/src/Printf.o");

    return;
}

void ReadTreeBin (TOKEN_TABLE * token_table, CodeBuffer_t * cb, CodeBuffer_t * cb_data)
{
    NODE * root = token_table->tree;

    nodes_with_strings.nodes_with_text = (NODE **) calloc (DELTA_NODE_WITH_STR, sizeof (NODE *));

//====================================== START CODEGEN ======================================

    AddLabel (&cb->symb_tab, "_start", cb->offset);

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+
    emit_call_func (cb, root->left->data.ident.ident_name);             //
    emit_mov_reg_imm (cb, RAX, 60);                                     // Call entry func and end prog.
    emit_mov_reg_imm (cb, RDI, 0);                                      //
    emit_syscall (cb);                                                  //
    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+

    while (root->data.op != END)                        // Start func generation
    {
        InitAsmFuncBin (root->left, cb);

        root = root->right;
    }

    // =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+
    SqrtByNewton (cb);      // Sqrt func                                // Additional funcs
    InputFunc    (cb);      // Input num func                           //
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
        AddLabel (&cb->symb_tab, STRINGS_NAME, cb->offset + cb_data->offset);

        lenght = strlen (nodes_with_strings.nodes_with_text[i]->data.ident.ident_name) + 1;

        code_buffer_write (cb_data, (uint8_t *) nodes_with_strings.nodes_with_text[i]->data.ident.ident_name, lenght);
        code_buffer_write (cb_data, (uint8_t *) "\0", 1);
    }

    AddLabel (&cb->symb_tab, "INPUT_BUFFER", cb->offset + cb_data->offset);           // For input func

    uint8_t bites[SIZE_OF_INPUT_BUFFER] = {0};
    code_buffer_write (cb_data, bites, SIZE_OF_INPUT_BUFFER);

    free (nodes_with_strings.nodes_with_text);

    return;
}

NODE * InitAsmFuncBin (NODE * node, CodeBuffer_t * cb)
{
    my_assert (node);
    
    AddLabel (&cb->symb_tab, node->data.ident.ident_name, cb->offset);

    POP_REG (R14)
    PUSH_REG(RSP)
    POP_REG (R15)
    ADD_REG_NUM (R15, (node->data.ident.ident_val.unsigned_type - 1) * 8)
    MOV_REG_REG (R13, R15)
    SUB_REG_NUM (R13, node->data.ident.ident_num * 8)
    PUSH_REG (R13)
    POP_REG (RSP)

    RecursyTreeReadBin (node->right, cb);

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

NODE * RecursyTreeReadBin (NODE * node, CodeBuffer_t * cb)
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

                ReverseReadFuncArgsBin (node->left, cb, &n_args);
                
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
                ReadFuncArgsBin (node->left, cb);

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
            GenOpCodeBin (node, cb);

            return NULL;
        }
        default:
        {
            printf ("ERROR in code gen\n");

            exit (-1);
        }
    }
}

void ReadFuncArgsBin (NODE * node, CodeBuffer_t * cb)
{
    my_assert (node);

    RecursyTreeReadBin (node, cb);

    if (node->left) ReadFuncArgsBin (node->left, cb);

    return;
}

void ReverseReadFuncArgsBin (NODE * node, CodeBuffer_t * cb, size_t * n_args)
{
    my_assert (node);

    *n_args += 1;

    if (node->left) ReverseReadFuncArgsBin (node->left, cb, n_args);

    RecursyTreeReadBin (node, cb);

    return;
}

#define EXPR_ASM()                                         \
    RecursyTreeReadBin (node->left, cb);             \
    RecursyTreeReadBin (node->right, cb);            \
                                                           \
    XOR_REG_REG (R11, R11)                                 \
    XOR_REG_REG (R12, R12)                                 \
    POP_REG (R11)                                          \
    POP_REG (R12)

void GenOpCodeBin (NODE * node, CodeBuffer_t * cb)
{
    my_assert (node);


    switch (node->data.op)
    {
        case NODE_BUNCH:
        {
            RecursyTreeReadBin (node->left, cb);

            RecursyTreeReadBin (node->right, cb);

            break;
        }
        case SIN:
        {
            RecursyTreeReadBin (node->left, cb);

            printf ("sin\n");

            printf ("; end SIN\n\n");

            break;
        }
        case COS:
        {
            RecursyTreeReadBin (node->left, cb);

            printf ("cos\n");

            printf ("; end COS\n\n");

            break;
        }
        case POW:
        {
            RecursyTreeReadBin (node->left, cb);

            RecursyTreeReadBin (node->right, cb);

            POP_REG (RCX)
            POP_REG (RDX)
            MOV_REG_IMM (RAX, 1)
            MAKE_LABEL ("loop_%d", lable_counter)
            AddLabel (&cb->symb_tab, STRINGS_NAME, cb->offset);
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
            RecursyTreeReadBin (node->left, cb);

            EMIT_CALL_FUNC ("sqrt_by_Newton")

            break;
        }
        case LN:
        {
            printf ("\n; Start taking the naturel logarithm\n\n");

            RecursyTreeReadBin (node->left, cb);

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
            RecursyTreeReadBin (node->left, cb);

            RecursyTreeReadBin (node->right, cb);

            POP_REG (R11)
            POP_REG (RAX)
            IMUL_REG_REG (RAX, R11)
            PUSH_REG (RAX)

            break;
        }
        case DIV:
        {
            RecursyTreeReadBin (node->left, cb);

            RecursyTreeReadBin (node->right, cb);

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
            
            RecursyTreeReadBin (node->right, cb);

            POP_MEM (R15, -1 * node->left->data.ident.ident_num * 8)

            break;
        }
        case IF:
        {
            IfCodeGenBin (node, cb);

            break;
        }
        case WHILE:
        {
            WhileCodeGenBin (node, cb);

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

void IfCodeGenBin (NODE * node, CodeBuffer_t * cb)
{
    my_assert (node);

    int end_if = IF_COUNTER++;

    int end_if_label_body = 0;

    end_if_label_body = IF_COUNTER++;

    AsmConditionalBin (node->left, cb, IF, end_if_label_body);

    RecursyTreeReadBin (node->right, cb);

    MAKE_LABEL ("if_%d", end_if_label_body);
    AddLabel (&cb->symb_tab, STRINGS_NAME, cb->offset);

    return;
}

void AsmConditionalBin (NODE * node, CodeBuffer_t * cb, OPERATORS mode, int label)
{
    my_assert (node);


    int num_of_label = IF_COUNTER;

    if (node->node_type == OP && (node->data.op == OR || node->data.op == AND))
    {
        if (node->left) AsmConditionalBin (node->left, cb, mode, label);

        if (node->right) AsmConditionalBin (node->right, cb, mode, label);

        if (node->data.op == OR)
        {
            MAKE_LABEL ("if_%d", num_of_label)
            AddLabel (&cb->symb_tab, STRINGS_NAME, cb->offset);
        }
    }
    else
    {
        ENUM_IDENT_DATA_TYPE ident_tp = SIGNED_INT;

        if (node->left->node_type == IDENT) ident_tp = node->left->data.ident.ident_data_type;

        RecursyTreeReadBin (node->left, cb);

        if (node->right)
        {
            if (ident_tp == SIGNED_INT) ;
            else if (node->right->node_type == IDENT) ident_tp = node->left->data.ident.ident_data_type;

            RecursyTreeReadBin (node->right, cb);
        }
        else PUSH_NUM (0)

        AsmOpsCompareBin (node, cb, mode, label, ident_tp);
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

void AsmOpsCompareBin (NODE * node, CodeBuffer_t * cb, OPERATORS mode, int label, ENUM_IDENT_DATA_TYPE data_type)
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

void WhileCodeGenBin (NODE * node, CodeBuffer_t * cb)
{
    my_assert (node);


    int start_label_body = WHILE_COUNTER++;
    int end_label_body   = WHILE_COUNTER;

    AsmConditionalBin (node->left, cb, WHILE, end_label_body);

    WHILE_COUNTER++;

    RecursyTreeReadBin (node->right, cb);

    MAKE_LABEL ("while_%d", start_label_body);
    JMP (STRINGS_NAME)

    MAKE_LABEL ("while_%d", end_label_body);
    AddLabel (&cb->symb_tab, STRINGS_NAME, cb->offset);
    return;
}

void InputFunc (CodeBuffer_t * cb)
{
    AddLabel (&cb->symb_tab, "input_func", cb->offset);

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

    AddLabel (&cb->symb_tab, "loop_input", cb->offset);

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

    AddLabel (&cb->symb_tab, "success", cb->offset);

    CMP_REG_NUM (R8, 1)
    JNE ("posit")

    NEG_REG (RAX)

    AddLabel (&cb->symb_tab, "posit", cb->offset);

    POP_REG (RBX)
    PUSH_REG (RAX)
    PUSH_REG (RBX)
    RET ()

    AddLabel (&cb->symb_tab, "error", cb->offset);

    MOV_REG_IMM (RAX, -1)
    RET ()
}

void SqrtByNewton (CodeBuffer_t * cb)
{
    AddLabel (&cb->symb_tab, "sqrt_by_Newton", cb->offset);

    POP_REG (R9)
    POP_REG (RAX)

    MOV_REG_REG (R8, RAX)
    MOV_REG_REG (RCX, RAX)

    AddLabel (&cb->symb_tab, "newton_loop", cb->offset);

    MOV_REG_REG (RAX, R8)
    XOR_REG_REG (RDX, RDX)
    IDIV_REG (RCX)
    ADD_REG_REG (RAX, RCX)
    SHR_REG_IMM (RAX, 1)
    CMP_REG_REG (RAX, RCX)
    JAE ("done")

    MOV_REG_REG (RCX, RAX)
    JMP ("newton_loop")

    AddLabel (&cb->symb_tab, "done", cb->offset);

    PUSH_REG (RCX)
    PUSH_REG (R9)
    RET ()
}