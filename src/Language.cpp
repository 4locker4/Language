#include "../inc/Language.h"

const char * MY_TRY     = "TestCode/Code.txt";
const char * SQ_SOLVER  = "./SquareSolver.txt";

int main ()
{
    printf ("Start\n");
    TOKEN_TABLE * table = TableCtor (MY_TRY);

    printf ("%p - after ctor\n", table->tokens_array);

    FillTokenTypes (table);

    printf ("%p - after fill\n", table->tokens_array);

    size_t ip = 0;

    printf ("end of token filling\n");

    table->tree = GetFunctions (table->tokens_array, &ip);
    COLOR_PRINT (GREEN, "Complite\n");
    GraphDump (table->tree, "./GraphDump.dot");
    system ("dot -Tsvg GraphDump.dot -o GraphDump.svg");

    ReadTree (table);
}

