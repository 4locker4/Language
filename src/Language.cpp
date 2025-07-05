#include "../inc/Language.h"

const char * FACTORIAL_CODE  = "./FactorialTranslation/FactorialCode.txt";
const char * FACTORIAL_NASM  = "./FactorialTranslation/FactorialNasm.txt";
const char * FACTORIAL_DUMP  = "./FactorialTranslation/FactorialDump.dot";

const char * SQ_SOLVER_CODE  = "SquareSolverTranslation/SquareSolverCode.txt";
const char * SQ_SOLVER_NASM  = "SquareSolverTranslation/SquareSolverNasm.txt";
const char * SQ_SOLVER_DUMP  = "SquareSolverTranslation/SquareSolverDump.dot";

int main (size_t argc, char * argv[])
{
    if (argc)
    {
        TOKEN_TABLE * table = NULL;

        if (! strcasecmp (argv[1], "Factorial"))
        {
            table = TableCtor (FACTORIAL_CODE);

            FillTokenTypes (table);
        
            size_t ip = 0;
        
            table->tree = GetFunctions (table->tokens_array, &ip);
        
            GraphDump (table->tree, FACTORIAL_DUMP);
        
            ReadTree (table, FACTORIAL_NASM);
        }
        else if (! strcasecmp (argv[1], "SquareSolver"))
        {
            table = TableCtor (SQ_SOLVER_CODE);

            FillTokenTypes (table);
        
            size_t ip = 0;
        
            table->tree = GetFunctions (table->tokens_array, &ip);
        
            GraphDump (table->tree, SQ_SOLVER_DUMP);
        
            ReadTree (table, SQ_SOLVER_NASM);
        }
        else
        {
            COLOR_PRINT (RED, "Tell me, what programm we compile. I can`t do %s\n", argv[1]);

            return 1;
        }

        TableDtor (table);

        return 0;
    }
}

