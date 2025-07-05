#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <stdlib.h>

#include "Utils.h"
#include "Asserts.h"
#include "Tree.h"

const char SEPARATER = ' ';

typedef struct
{
    char * ident_name = NULL;
    size_t ident_num  = 0;
} IDENT_INFO;

TOKEN *         FillTokenTypes              (TOKEN_TABLE * table);
void            CountTokens                 (TOKEN_TABLE * table);
ALL_OPS_DATAS * OpsCtor                     (TOKEN_TABLE * table);
TOKEN_TABLE *   TableCtor                   (const char * file_with_data);
int             TreeDestr                   (NODE * node);
void            TableDtor                   (TOKEN_TABLE * table);

void            ReverseReadFuncArgs         (NODE * node, FILE * file, size_t * n_args);
void            ReadTree                    (TOKEN_TABLE * token_table, const char * file_name);
NODE *          RecursyTreeRead             (NODE * node, FILE * file);
void            GenOpCode                   (NODE * node, FILE * file);
void            IfCodeGen                   (NODE * node, FILE * file);
void            WhileCodeGen                (NODE * node, FILE * file);
NODE *          InitAsmFunc                 (NODE * node, FILE * file);
void            ReadFuncArgs                (NODE * node, FILE * file);

void            AsmOpsCompare               (NODE * node, FILE * file, OPERATORS mode, int label, ENUM_IDENT_DATA_TYPE data_type);
void            AsmConditional              (NODE * node, FILE * file, OPERATORS mode, int label);

#endif