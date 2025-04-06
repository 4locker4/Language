#include "../../inc/Language.h"

static int IF_COUNTER = 0;
static int WHILE_COUNTER = 0;

void ReadTree (TOKEN_TABLE * token_table)
{
    my_assert (token_table);

    FILE * asm_file = fopen ("AsmCode.txt", "w+");
    my_assert (asm_file);

    NODE * root = token_table->tree;

    fprintf (asm_file, "call main\nhlt\n");

    RecursyTreeRead (root, asm_file);

    return;
}

NODE * RecursyTreeRead (NODE * node, FILE * file)
{
    my_assert (node);
    my_assert (file);

    printf ("In read tree\n");

    switch (node->node_type)
    {
        case NUM:
        {
            fprintf (file, "push %d\n", node->data.val);

            break;
        }
        case FUNC_IDENT:
        {
            fprintf (file, "%s:\n", node->data.ident.ident_name);

            if (node->left) RecursyTreeRead (node->left, file);

            if (node->right) RecursyTreeRead (node->right, file);

            fprintf (file, "ret\n");

            break;
        }
        case FUNC_CALL:
        {
            if (node->left) RecursyTreeRead (node->left, file);

            if (node->right) RecursyTreeRead (node->right, file);

            fprintf (file, "call %s\n", node->data.ident.ident_name);

            break;
        }
        case PARAM:
        {
            if (node->left) RecursyTreeRead (node->left, file);

            fprintf (file, "pop [%d]\n", node->data.ident.ident_num);

            break;
        }
        case IDENT:
        {
            fprintf (file, "push [%d]\n", node->data.ident.ident_num);

            if (node->left) RecursyTreeRead (node->left, file);

            break;
        }
        case OP:
        {
            GenOpCode (node, file);

            break;
        }
        default:
        {
            printf ("ERROR in code gen\n");

            exit (-1);
        }
    }
}

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

            fprintf (file, "pow\n");

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
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file, "add\n");

            break;
        }
        case SUB:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file, "sub\n");

            break;
        }
        case MUL:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file, "mul\n");

            break;
        }
        case DIV:
        {
            RecursyTreeRead (node->left, file);

            RecursyTreeRead (node->right, file);

            fprintf (file, "div\n");

            break;
        }
        case EQUALSE:
        {
            RecursyTreeRead (node->right, file);

            fprintf (file, "pop [%d]\n", node->left->data.ident.ident_num);

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
            fprintf (file, "ret\n");

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

    int if_num = IF_COUNTER++;

    RecursyTreeRead (node->left, file);

    fprintf (file, "push 0\n"
                   "jne end_if_%d\n", if_num);
    
    RecursyTreeRead (node->right, file);

    fprintf (file, "end_if_%d:\n", if_num);

    return;
}

void WhileCodeGen (NODE * node, FILE * file)
{
    my_assert (node);
    my_assert (file);

    int while_num = WHILE_COUNTER++;

    fprintf (file, "while_start_%d:\n", while_num);

    RecursyTreeRead (node->left, file);

    fprintf (file, "push 0\n"
                   "je end_while_%d\n", while_num);

    RecursyTreeRead (node->right, file);

    fprintf (file, "jmp while_start_%d\n", while_num);

    fprintf (file, "end_while_%d:\n", while_num);

    return;
}
