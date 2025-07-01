#include "../inc/Language.h"

const char * FACTORIAL_CODE  = "./FactorialTranslation/FactorialCode.txt";
const char * FACTORIAL_NASM  = "./FactorialTranslation/FactorialNasm.txt";
const char * FACTORIAL_DUMP  = "./FactorialTranslation/FactorialDump.dot";

const char * SQ_SOLVER       = "TestCode/SquareSolver.txt";

int main ()
{
    printf ("Start\n");
    TOKEN_TABLE * table = TableCtor (FACTORIAL_CODE);

    printf ("%p - after ctor\n", table->tokens_array);

    FillTokenTypes (table);

    printf ("%p - after fill\n", table->tokens_array);

    size_t ip = 0;

    printf ("end of token filling\n");

    table->tree = GetFunctions (table->tokens_array, &ip);
    COLOR_PRINT (GREEN, "Complite\n");

    GraphDump (table->tree, FACTORIAL_DUMP);

    ReadTree (table, FACTORIAL_NASM);

}

