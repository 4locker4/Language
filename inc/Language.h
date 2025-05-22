#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <stdlib.h>

#include "Utils.h"
#include "Asserts.h"
#include "Tree.h"

const char SEPARATER = ' ';

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

#endif