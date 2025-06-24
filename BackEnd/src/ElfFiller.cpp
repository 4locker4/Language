#include "../inc/Elf64Gen.hpp"

static int IF_COUNTER = 0;
static int WHILE_COUNTER = 0;

static size_t lable_counter = 0;

void ReadTree (TOKEN_TABLE * token_table)
{
    my_assert (token_table);

    FILE * elf_file = fopen ("LanguaegeList/ElfFile.txt", "w+");
    my_assert (elf_file);

    NODE * root = token_table->tree;

//========================================= START CODEGEN =========================================

    Elf64_Ehdr ehdr  = {0};
    Elf64_Phdr phdr  = {0};

    memcpy(ehdr.e_ident, "\x7F""ELF", 4);
    ehdr.e_ident[4]  = 2;                               // 64-bit
    ehdr.e_ident[5]  = 1;                               // little-endian
    ehdr.e_type      = 2;                               // Исполняемый
    ehdr.e_machine   = 0x3E;                            // x86_64
    ehdr.e_version   = 1;
    ehdr.e_entry     = 0x400000 + sizeof(ehdr) + sizeof(phdr);  // Точка входа
    ehdr.e_phoff     = sizeof(ehdr);
    ehdr.e_ehsize    = sizeof(ehdr);
    ehdr.e_phentsize = sizeof(phdr);                    // Размер одного заголовка
    ehdr.e_phnum     = 1;                               // Количество заголовков

    phdr.p_type      = 1;                               // Загружаемый
    phdr.p_flags     = PF_X;             
    phdr.p_offset    = 0;
    phdr.p_vaddr     = 0x400000;
    phdr.p_filesz    = phdr.p_memsz = sizeof(ehdr) + sizeof(phdr); // + sizeof(code);
    phdr.p_align     = 0x1000;

    /* Короче, сначала смещение ставим на конец всех заголовков (или если align больше, то на значение align) для сегмента данных.
    Потом смещение сегмента кода на значение align+размер сегмента данны*/


    while (root->data.op != END)                        // Start func generation
    {
        InitAsmFunc (root->left, elf_file);

        root = root->right;
    }

    fprintf (elf_file, "section .data\n\n"              // Add section data with all vars
                       "\tmem: \n\n \t\t times %d dq 0", (token_table->n_idents + 1) * 8);
    
    fclose (elf_file);
    
    return;
}

void FillIdentsTable (NODE * node, IDENT_INFO * ident_table, size_t * free_elem)
{
    my_assert (node && ident_table);

    size_t n_elems = *free_elem;

    if (node->left  != NULL) FillIdentsTable (node->left,  ident_table, free_elem);
    if (node->right != NULL) FillIdentsTable (node->right, ident_table, free_elem);

    if (node->node_type == IDENT || node->node_type == PARAM)
    {
        for (size_t i = 0; n_elems > i; i++)
            if (!strcasecmp (ident_table[i].ident_name, ident_table[*free_elem].ident_name))
                return;
        
        ident_table[*free_elem].ident_name = node->data.ident.ident_name;
        ident_table[*free_elem].ident_num  = *free_elem;

        *free_elem += 1;
    }

    return;
}

NODE * InitAsmFunc (NODE * node, FILE * file)
{
    my_assert (node && file);

    size_t params_counter = 0;

    CountParams (node, &params_counter);

    fprintf (file,  ".%s:\n\n"
                    "\t\tpop  r14\n"             // Адрес возврата
                    "\t\tpush rsp\n"
                    "\t\tpop  rbx\n"             // Адрес, где хранятся аргументы
                    "\t\tpush rsp\n"
                    "\t\tpop  r13\n"
                    "\t\tadd  r13, %d\n"         // Место под все переменные и параметры
                    "\t\tpush r13\n"
                    "\t\tpop  rsp\n", node->data.ident.ident_name, params_counter * 8);

    RecursyTreeRead (node->left, file);
    RecursyTreeRead (node->right, file);
    COLOR_PRINT (RED, "/(. Y .)\\\n");

    return node->right;
}

void CountParams (NODE * node, size_t * n_params)
{
    my_assert (node && n_params);

    if (node->right != NULL) CountParams (node->right, n_params);
    
    *n_params += 1;

    return;
}

NODE * RecursyTreeRead (NODE * node, FILE * file)
{
    my_assert (node);
    my_assert (file);

    if (node->node_type == IDENT)
        printf ("ident: %s\n", node->data.ident.ident_name);
    if (node->node_type == OP)
        printf ("op: %c\n", node->data.op);
    if (node->node_type == FUNC_IDENT)
        printf ("func ident: %s & %d\n", node->data.ident.ident_name, node->data.ident.ident_val);
    

    switch (node->node_type)
    {
        case NUM:
        {
            fprintf (file, "\t\tpush %d\n", node->data.val);

            return NULL;
        }
        case FUNC_CALL:
        {
            NODE * root_param = node->right;

            if (node->left) RecursyTreeRead (node->left, file);

            fprintf (file, "call .%s\n", node->data.ident.ident_name);

            return NULL;
        }
        case PARAM:
        case IDENT:
        {
            fprintf (file, "\t\tpush qword [r15 + %d]\n", node->data.ident.ident_num * 8);

            return NULL;
        }
        case OP:
        {
            printf ("OP\n");

            GenOpCode (node, file);

            return NULL;
        }
        default:
        {
            printf ("ERROR in code gen\n");

            exit (-1);
        }
    }
}

#define EXPR_ASM(str)                                      \
    RecursyTreeRead (node->left, file);              \
    RecursyTreeRead (node->right, file);             \
                                                           \
    fprintf (file,  "\t\tpop r11\n"                        \
                    "\t\tpop r12\n"                        \
                    "\t\t" str " r12, r11\n"               \
                    "\t\tpush r12\n");

void GenOpCode (NODE * node, FILE * file)
{
    my_assert (node);
    my_assert (file);

    switch (node->data.op)
    {
        case NODE_BUNCH:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            break;
        }
        case SIN:
        {
            RecursyTreeRead (node->left, file);

            fprintf (file, "sin\n");

            break;
        }
        case COS:
        {
            RecursyTreeRead (node->left, file);

            fprintf (file, "cos\n");

            break;
        }
        case POW:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file,  "\t\tpop rcx\n"
                            "\t\tpop rax\n"
                            "\t.loop_%d:\n"
                            "\t\tmul rax\n"
                            "\t\tloop\n"
                            "\t\t", lable_counter++);

            break;
        }
        case SQRT:
        {
            RecursyTreeRead (node->left, file);

            fprintf (file, "sqrt\n");

            break;
        }
        case LN:
        {
            RecursyTreeRead (node->left, file);

            fprintf (file, "ln\n");

            break;
        }
        case ADD:
        {
            EXPR_ASM("add")

            break;
        }
        case SUB:
        {
            EXPR_ASM("sub")

            break;
        }
        case MUL:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file,  "\t\tpop r11\n"
                            "\t\tpop rax\n"
                            "\t\tmul r11\n"
                            "\t\tpush rax\n");

            break;
        }
        case DIV:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file,  "\t\tpop r11\n"
                            "\t\tpop rax\n"
                            "\t\tidiv r11\n"
                            "\t\tpush rax\n");

            break;
        }
        case EQUALSE:
        {
            int ident_num = node->left->data.ident.ident_val;
            
            RecursyTreeRead (node->right, file);

            fprintf (file, "\t\tpop qword [r15 - %d]\n", node->left->data.ident.ident_num * 8);

            break;
        }
        case IF:
        {
            IfCodeGen (node, file);

            break;
        }
        case WHILE:
        {
            WhileCodeGen (node, file);

            break;
        }
        case PRINT:
        {
            fprintf (file, "outp\n");
            
            break;
        }
        case END:
        {
            fprintf (file, "\n");

            break;
        }
        case INPUT:
        {
            fprintf (file, "inp\n");

            break;
        }
        case RETURN:
        {
            fprintf (file, "\t\tpush r15\n"
                           "\t\tret\n");

            break;
        }
        default:
        {
            printf ("I cant do this\n");

            exit (-1);
        }
    }
}

void IfCodeGen (NODE * node, FILE * file)
{
    my_assert (node);
    my_assert (file);

    int end_if = IF_COUNTER++;

    int end_if_label_body = 0;

    while (node)
    {
        printf ("node type: %c\n", node->data.op);

        end_if_label_body = IF_COUNTER++;
        COLOR_PRINT (MANGETA, "/(. Y .)\\\n");

        AsmConditional (node->left, file, IF, end_if_label_body);

        RecursyTreeRead (node->right, file);

        fprintf (file, "\t\tjmp .end_if_%d\n", end_if);
        fprintf (file, "\t.if_%d:\n", end_if_label_body);

        node = node->right;

        if (node->node_type == OP && node->data.op == END)
            break;
    }
    COLOR_PRINT (MANGETA, "o===3\\\n");

    fprintf (file, "\t.end_if_%d:\n", end_if);

    return;
}

void AsmConditional (NODE * node, FILE * file, OPERATORS mode, int label)
{
    my_assert (node);
    my_assert (file);

    int num_of_label = IF_COUNTER;
    COLOR_PRINT (CYAN, "/(. Y .)\\\n");

    if (node->node_type == OP && (node->data.op == OR || node->data.op == AND))
    {
        if (node->left) AsmConditional (node->left, file, mode, label);

        if      (node->data.op == AND)   AsmOpsCompare (node->left, file, mode, label);
        else if (node->data.op == OR)    
        {
            AsmOpsCompare (node->left, file, mode, IF_COUNTER);

            num_of_label = IF_COUNTER++;
        }

        if (node->right) AsmConditional (node->right, file, mode, label);

        if (node->right) AsmOpsCompare (node->right, file, mode, label);

        if (node->data.op == OR)
        {
            fprintf (file, "\t.if_%d:\n", num_of_label);
        }
    }
    else
    {

        RecursyTreeRead (node->left, file);
        COLOR_PRINT (CYAN, "/(. Y .)\\\n");

        if (node->right) RecursyTreeRead (node->right, file);
        COLOR_PRINT (GREEN, "/(. Y .)\\\n");

        if (node->parent->data.op == NOT_OPERATOR || node->parent->data.op == WHILE)
            AsmOpsCompare (node, file, mode, label);
        COLOR_PRINT (RED, "/(. Y .)\\\n");
    }

    return;
}

#define PRINT_LABEL(jmp)                                      \
    fprintf (file, "\t\tcmp r12, r11\n");                     \
                                                              \
    if (mode == WHILE)                                        \
        fprintf (file, "\t\t" jmp " .while_%d\n\n", label);   \
    else                                                      \
        fprintf (file, "\t\t" jmp " .if_%d\n\n", label);      \

void AsmOpsCompare (NODE * node, FILE * file, OPERATORS mode, int label)
{
    my_assert (file && node);
    COLOR_PRINT (RED, "/(. Y .)\\\n");

    fprintf (file, "\t\tpop r11\n"
                   "\t\tpop r12\n");
    
    switch (node->data.op)
    {
        case MORE:
        {
            PRINT_LABEL ("ja")

            break;
        }
        case LESS:
        {
            PRINT_LABEL ("jb");

            break;
        }
        case GREATER_OR_EQ:
        {
            PRINT_LABEL ("jae")

            break;
        }
        case LESS_OR_EQ:
        {
            PRINT_LABEL ("jbe");

            break;
        }
        default:
        {
            break;
        }
    }
}

#undef PRINT_LABEL

void WhileCodeGen (NODE * node, FILE * file)
{
    my_assert (node);
    my_assert (file);

    int start_label_body = WHILE_COUNTER++;
    int end_label_body   = WHILE_COUNTER;

    AsmConditional (node->left, file, WHILE, end_label_body);

    WHILE_COUNTER++;

    RecursyTreeRead (node->left, file);


    fprintf (file, "\t\tjmp .while_%d\n", start_label_body);

    fprintf (file, "\t.while_%d:\n", end_label_body);

    return;
}
