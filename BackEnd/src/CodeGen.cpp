#include "../../inc/Language.h"

static int IF_COUNTER    = 0;
static int WHILE_COUNTER = 0;

static size_t lable_counter    = 0;

void ReadTree (TOKEN_TABLE * token_table, const char * file_name)
{
    my_assert (token_table);

    FILE * asm_file = fopen (file_name, "w+");
    my_assert (asm_file);

    NODE * root = token_table->tree;

//====================================== START CODEGEN ======================================

    fprintf (asm_file, "global main\n\n"                // Init asm prog
                       "section .note.GNU-stack noexec\n"
                       "section .text\n\n", root->left->data.ident.ident_name);

    fprintf (asm_file, "main:\n"
                       "\t\tcall .%s\n"                // Call entry func
                       "\t\tjmp endprog\n", root->left->data.ident.ident_name);


    while (root->data.op != END)                        // Start func generation
    {
        InitAsmFunc (root->left, asm_file);

        root = root->right;
    }

    fprintf (asm_file, "endprog:\n"
                       "\t\tmov rax, 60\n"
                       "\t\tmov rdi, 0\n"
                        "\t\tsyscall\n\n"
                       "section .data\n\n"              // Add section data with all vars
                       "\tmem: \n\n \t\t times %d dq 0", (token_table->n_idents + 1) * 8);
    
    fclose (asm_file);

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

    fprintf (file,  ".%s:\n\n"                                                              // Имя функции
                    "\t\tpop  r14               ; r14 - сохраняем адрес возврата\n"         // Адрес возврата
                    "\t\tpush rsp               ;----\n"
                    "\t\tpop  r15               ;    |--> создаем сегмент переменных\n"     // Адрес, где хранятся аргументы
                    "\t\tadd  r15, %d            ;----\n"                                   // Смещение на начало переменных
                    "\t\tmov r13, r15           ;------\n"            
                    "\t\tsub  r13, %d           ;      |--> новый адрес стека\n"            // Выделено место под все переменные и параметры
                    "\t\tpush r13               ;      |\n"
                    "\t\tpop  rsp               ;------\n\n"
                    "; ========================== START FUNC ==========================\n\n",
                    node->data.ident.ident_name, (node->data.ident.ident_val.unsigned_type - 1) * 8, node->data.ident.ident_num * 8);

    RecursyTreeRead (node->right, file);

    return node->right;
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
    
// TD добавить обработку char *
    switch (node->node_type)
    {
        case NUM:
        {
            fprintf (file, "\t\tpush %d\n", node->data.val);

            return NULL;
        }
        case FUNC_CALL:
        {
            fprintf (file,  "\t\tpush r15\n"
                            "\t\tpush r14\n"
                            "\t\tpush rsp\n");

            if (node->left) RecursyTreeRead (node->left, file);

            fprintf (file, "\n\t\tcall .%s\n\n", node->data.ident.ident_name);

            fprintf (file,  "\t\tpop rax\n"
                            "\t\tpush qword [r15 + 8]\n"
                            "\t\tpop rsp\n"
                            "\t\tpop r14\n"
                            "\t\tpop r15\n"
                            "\t\tpush rax\n");
    
            return NULL;
        }
        case PARAM:
        case IDENT:
        {
            fprintf (file, "\t\tpush qword");

            fprintf (file, " [r15 - %d]           ; переменная: %s\n", node->data.ident.ident_num * 8, node->data.ident.ident_name);

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
    RecursyTreeRead (node->left, file);                    \
    RecursyTreeRead (node->right, file);                   \
                                                           \
    fprintf (file,  "\t\txor r11, r11\n"                    \
                    "\t\txor r12, r12\n"                   \
                    "\t\tpop r11\n"                        \
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

            fprintf (file, "; end SIN\n\n");

            break;
        }
        case COS:
        {
            RecursyTreeRead (node->left, file);

            fprintf (file, "cos\n");

            fprintf (file, "; end COS\n\n");

            break;
        }
        case POW:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file,  "; exponentiation\n\n"
                            "\t\tpop rcx\n"
                            "\t\tpop rax\n"
                            "\t.loop_%d:\n"
                            "\t\tmul rax\n"
                            "\t\tloop\n"
                            "\t\t", lable_counter++);

            fprintf (file, "; end POW\n\n");
            
            break;
        }
        case SQRT:
        {
            fprintf (file, "\n; Start taking the square root\n\n");

            RecursyTreeRead (node->left, file);

            fprintf (file, "sqrt\n\n");

            fprintf (file, "; end SQRT\n\n");

            break;
        }
        case LN:
        {
            fprintf (file, "\n; Start taking the naturel logarithm\n\n");

            RecursyTreeRead (node->left, file);

            fprintf (file, "ln\n\n");

            fprintf (file, "; end LN\n\n");

            break;
        }
        case ADD:
        {
            fprintf (file, "\n; Make: ADD\n\n");

            EXPR_ASM("add")

            fprintf (file, "; end ADD\n\n");

            break;
        }
        case SUB:
        {
            fprintf (file, "\n; Make: SUB\n\n");

            EXPR_ASM("sub")

            fprintf (file, "; end SUB\n\n");

            break;
        }
        case MUL:
        {
            fprintf (file, "\n; Make: MUL\n\n");

            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file,  "\t\tpop r11\n"
                            "\t\tpop rax\n"
                            "\t\tmul r11\n"
                            "\t\tpush rax\n\n");

            fprintf (file, "; end MUL\n\n");

            break;
        }
        case DIV:
        {
            fprintf (file, "\n; Make: DIV\n\n");
            
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file,  "\t\tpop r11\n"
                            "\t\tpop rax\n"
                            "\t\tidiv r11\n"
                            "\t\tpush rax\n\n");

            fprintf (file, "; end DIV\n\n");

            break;
        }
        case EQUALS:
        {
            int ident_num = node->left->data.ident.ident_num;
            
            fprintf (file, "\n; EQUALSE\n\n");

            RecursyTreeRead (node->right, file);

            fprintf (file, "\t\tpop qword [r15 - %d]\n", node->left->data.ident.ident_num * 8);

            fprintf (file, "; end EQUALS\n");

            break;
        }
        case IF:
        {
            fprintf (file, "\n; Start IF code\n\n");

            IfCodeGen (node, file);

            fprintf (file, "; end IF\n\n");

            break;
        }
        case WHILE:
        {
            fprintf (file, "\n; Start WHILE code\n\n");

            WhileCodeGen (node, file);

            fprintf (file, "; end WHILE\n\n");

            break;
        }
        case PRINT:
        {
            fprintf (file, "; Start PRINTF\n\n");

            fprintf (file, "outp\n");

            fprintf (file, "; end OUTP\n\n");
            
            break;
        }
        case END:
        {
            fprintf (file, "\n");

            break;
        }
        case INPUT:
        {
            fprintf (file, "; Start INPUT\n\n");

            fprintf (file, "inp\n");

            fprintf (file, "; end\n\n");

            break;
        }
        case RETURN:
        {
            fprintf (file, "\n; RETURN\n\n");

            fprintf (file, "\t\tpush qword [r15 - %d]\n"
                           "\t\tpush r14\n"
                           "\t\tret\n", node->left->data.ident.ident_num * 8);

            fprintf (file, "; end RETURN\n\n");
            
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

void IfCodeGen (NODE * node, FILE * file)
{
    my_assert (node);
    my_assert (file);

    int end_if = IF_COUNTER++;

    int end_if_label_body = 0;

    end_if_label_body = IF_COUNTER++;
    COLOR_PRINT (MANGETA, "/(. Y .)\\\n");

    fprintf (file, "; ========================= START IF CONDITIONAL =========================\n");

    AsmConditional (node->left, file, IF, end_if_label_body);

    fprintf (file, "; ========================= END IF CONDITIONAL =========================\n");

    RecursyTreeRead (node->right, file);

    fprintf (file, "\t.if_%d:\n", end_if_label_body);

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
        else fprintf (file, "\t\tpush 0\n");
        
        COLOR_PRINT (GREEN, "/(. Y .)\\\n");

        AsmOpsCompare (node, file, mode, label);
        COLOR_PRINT (RED, "/(. Y .)\\\n");
    }

    return;
}

#define PRINT_LABEL(jmp)                                      \
    fprintf (file, "\t\tpop r11\n"                            \
                   "\t\tpop r12\n"                            \
                   "\t\tcmp r12, r11\n");                     \
                                                              \
    if (mode == WHILE)                                        \
        fprintf (file, "\t\t" jmp " .while_%d\n\n", label);   \
    else                                                      \
        fprintf (file, "\t\t" jmp " .if_%d\n\n", label);      \

void AsmOpsCompare (NODE * node, FILE * file, OPERATORS mode, int label)
{
    my_assert (file && node);
    COLOR_PRINT (RED, "/(. Y .)\\\n");

    switch (node->data.op)
    {
        case MORE:
        {
            PRINT_LABEL ("jbe")

            break;
        }
        case LESS:
        {
            PRINT_LABEL ("jae");

            break;
        }
        case GREATER_OR_EQ:
        {
            PRINT_LABEL ("jb")

            break;
        }
        case LESS_OR_EQ:
        {
            PRINT_LABEL ("ja");

            break;
        }
        case EQ_EQUALS:
        {
            PRINT_LABEL ("jne");

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

    RecursyTreeRead (node->right, file);

    fprintf (file, "\t\tjmp .while_%d\n", start_label_body);

    fprintf (file, "\t.while_%d:\n", end_label_body);

    return;
}
