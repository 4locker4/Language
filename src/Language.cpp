#include "../inc/Language.h"

const char * MY_TRY     = "./Code.txt";
const char * SQ_SOLVER  = "./SquareSolver.txt";

int main ()
{
    TOKEN_TABLE * table = TableCtor (SQ_SOLVER);

    printf ("%p - fafter ctor\n", table->tokens_array);

    FillTokenTypes (table);

    printf ("%p - fafter fill\n", table->tokens_array);

    size_t ip = 0;

    printf ("end of token filling\n");
    
    table->tree = GetFunctions (table->tokens_array, &ip);

    GraphDump (table->tree, "./GraphDump.dot");
    system ("dot -Tsvg GraphDump.dot -o GraphDump.svg");

    ReadTree (table);
}



