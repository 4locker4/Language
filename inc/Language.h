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

TOKEN *             FillTokenTypes            (TOKEN_TABLE * table);
void                CountTokens               (TOKEN_TABLE * table);
ALL_OPS_DATAS *     OpsCtor                   (TOKEN_TABLE * table);
TOKEN_TABLE *       TableCtor                 (const char * file_with_data);
int                 CheckRepitedIdent         (TOKEN_TABLE * table, char * ident, size_t ip);

void    ReadTree            (TOKEN_TABLE * token_table);
NODE *  RecursyTreeRead     (NODE * node, FILE * file);
void    GenOpCode           (NODE * node, FILE * file);
void    IfCodeGen           (NODE * node, FILE * file);
void    WhileCodeGen        (NODE * node, FILE * file);
NODE *  InitAsmFunc         (NODE * node, FILE * file);

void AsmOpsCompare (NODE * node, FILE * file, OPERATORS mode, int label);
void AsmConditional (NODE * node, FILE * file, OPERATORS mode, int label);
void CountParams (NODE * node, size_t * n_params);

#endif