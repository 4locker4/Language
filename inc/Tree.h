#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "Utils.h"
#include "Dsl.h"

typedef int nodeElem;

const int NOT_REPITED = -1;             // TD переделать нахуй

enum X_TYPES
{
    NOT_INITED = 0,
    IDENT,
    NUM,
    OP,
    FUNC_IDENT,
    FUNC_CALL,
    PARAM,
};

enum FUNC_TYPE
{
    CALL_FUNC  = 0,
    IDENT_FUNC = 1,
};

enum OPERATORS
{
    NOT_OPERATOR = 0,
    NODE_BUNCH   = 1,
    SIN  = 's',
    COS  = 'c',
    LN   = 'l',
    SQRT = 'S',
    POW  = '^',
    ADD  = '+',
    SUB  = '-',
    MUL  = '*',
    DIV  = '/',
    END  = ';',
    EQUALSE       = '=',
    OPEN_BRACKET  = '(',
    CLOSE_BRACKET = ')',
    SPLIT         = ',',
    IF            = 'f',
    WHILE         = 'w',
    PRINT         = 'p',
    DEF_FUNC      = 'd',

    EQUALITY      = 'e',
    MORE          = '>',
    LESS          = '<',
    GREATER_OR_EQ = 'g',
    LESS_OR_EQ    = 'l',
    INPUT         = 'i',

    RETURN        = 'r',
};

typedef struct
{
    int    ident_val  = 0;
    char * ident_name = NULL;
    int    ident_num  = -1;
} IDENT_DATA;

typedef union
{
    IDENT_DATA ident;
    nodeElem   val;
    OPERATORS  op;
} DATA;

typedef struct NODE
{
    struct NODE * left;
    struct NODE * right;

    struct NODE * parent;

    X_TYPES node_type = NOT_INITED;
    DATA data  = {};
} NODE;

typedef struct
{
    char *    name    = NULL;
    OPERATORS token   = NOT_OPERATOR;
} OP_DATA;

typedef struct
{
    OP_DATA add             = {};
    OP_DATA sub             = {};
    OP_DATA mul             = {};
    OP_DATA div             = {};
    OP_DATA sin             = {};
    OP_DATA cos             = {};
    OP_DATA ln              = {};
    OP_DATA pow             = {};
    OP_DATA sqrt            = {};
    OP_DATA open_bracket    = {};
    OP_DATA close_bracket   = {};
    OP_DATA end_op          = {};
    OP_DATA if_             = {};
    OP_DATA while_          = {};
    OP_DATA def_func        = {};
    OP_DATA equalse         = {};
    OP_DATA split           = {};
    OP_DATA ret             = {};
    OP_DATA inp             = {};
    OP_DATA powc            = {};
    OP_DATA print           = {};

} ALL_OPS_DATAS;

typedef struct 
{
    X_TYPES token_type  = NOT_INITED;
    DATA    token       = {};
    size_t  token_size  = 0;
} TOKEN;

typedef struct
{
    const char *        data_base_file      = NULL;
    char  *             text                = NULL;

    TOKEN *             tokens_array        = NULL;

    NODE  *             tree                = NULL;

    ALL_OPS_DATAS       ops_data            = {};

    size_t n_idents         = 0;
    size_t n_tokens         = 0;
    size_t file_size        = 0;

    bool   status = 0;
} TOKEN_TABLE;

NODE *  GetFunctions                (TOKEN * token_table, size_t * ip);
NODE *  GetFunc                     (TOKEN * token_table, size_t * ip);

NODE *  GetAllParameters            (TOKEN * token_table, size_t * ip, FUNC_TYPE type);
NODE *  GetParam                    (TOKEN * token_table, size_t * ip, FUNC_TYPE type);

NODE *  GetBodyOps                  (TOKEN * token_table, size_t * ip);
NODE *  GetAction                   (TOKEN * token_table, size_t * ip);

NODE *  GetFuncCall                 (TOKEN * token_table, size_t * ip);

NODE *  GetOp                       (TOKEN * token_table, size_t * ip);
NODE *  GetReturn                   (TOKEN * token_table, size_t * ip);
NODE *  GetIf                       (TOKEN * token_table, size_t * ip);
NODE *  GetWhile                    (TOKEN * token_table, size_t * ip);
NODE *  GetPrint                    (TOKEN * token_table, size_t * ip);

NODE *  GetCompare                  (TOKEN * token_table, size_t * ip);

NODE *  GetEqualse                  (TOKEN * token_table, size_t * ip);

NODE *  GetID                       (TOKEN * token_table, size_t * ip);
NODE *  GetExpression               (TOKEN * token_table, size_t * ip);
NODE *  GetT                        (TOKEN * token_table, size_t * ip);
NODE *  GetDegree                   (TOKEN * token_table, size_t * ip);
NODE *  GetMathFunc                 (TOKEN * token_table, size_t * ip);
NODE *  GetP                        (TOKEN * token_table, size_t * ip);
NODE *  GetTerm                     (TOKEN * token_table, size_t * ip);
int     SyntaxError                 (TOKEN * token_table, size_t * ip);

NODE *  CallocNode                  (NODE * left_node, NODE * right_node);
NODE *  NewNumNode                  (X_TYPES type, int elem, NODE * left_node, NODE * right_node);
NODE *  NewIdentNode                (X_TYPES type, IDENT_DATA ident, NODE * left_node, NODE * right_node);
NODE *  NewOpNode                   (X_TYPES type, OPERATORS op, NODE * left_node, NODE * right_node);

int     GraphDump                   (NODE * node, const char * file_name);
int     RecurcyDumpFill             (FILE * file, NODE * node);

#endif