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

// ======================================= FOR TREE CREATE =======================================

enum FUNC_TYPE
{
    CALL_FUNC  = 0,
    IDENT_FUNC = 1,
};

// ============================================= END =============================================

// ================================== IDENTIFICATOR DATA STRUCT ==================================

enum ENUM_IDENT_DATA_TYPE
{
    NULL_TYPE    = 0,
    UNSIGNED_INT = 1,                                               // Здесь также хранится количество параметров функции
    SIGNED_INT   = 2,
    DOUBLE       = 3,
    CHAR         = 4
};

const size_t MAX_STRING_LENGHT = 32;
typedef union ID_VAL_TYPE
{
    size_t unsigned_type;
    int    signed_type;
    double double_type;
    char   string_type[MAX_STRING_LENGHT + 1];
};

typedef struct
{
    char ident_name[32] = {};

    ID_VAL_TYPE          ident_val;
    size_t               ident_num       = -1;                      // Здесь хранится количество переменных функции
    ENUM_IDENT_DATA_TYPE ident_data_type = NULL_TYPE;

} IDENT_DATA;

// ============================================= END =============================================

// ============================== IDENTIFICATORS TABLE DATA STRUCT ===============================

const size_t IDS_TABLE_SIZE_DELTA = 5;

typedef struct
{
    size_t   table_size   = IDS_TABLE_SIZE_DELTA;
    size_t   free_box     = 0;

    size_t * existing_ids = NULL;
} IDENTIFICATORS_TABLE;

// ==================================== OPERATORS DATA STRUCTS ===================================

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
    EQUALS        = '=',
    EQ_EQUALS     = 'E',
    OPEN_BRACKET  = '(',
    CLOSE_BRACKET = ')',
    SPLIT         = ',',
    IF            = 'f',
    WHILE         = 'w',
    PRINT         = 'p',
    DEF_FUNC      = 'd',
    START_FUNC    = '{',
    END_FUNC      = '}',

    EQUALITY      = 'e',
    MORE          = '>',
    LESS          = '<',
    GREATER_OR_EQ = 'g',
    LESS_OR_EQ    = 'l',
    INPUT         = 'i',

    OR            = 'o',
    AND           = 'a',

    RETURN        = 'r',

    START_TEXT    = '"'
};

typedef struct
{
    IDENT_DATA ident    = {};
    nodeElem   val      = 0;
    size_t     n_params = 0;
    OPERATORS  op       = NOT_OPERATOR;
} DATA;

// ============================================= END =============================================

// ====================================== NODES DATA STRUCTS =====================================

enum NODE_TYPES
{
    NOT_INITED = 0,
    IDENT,
    NUM,
    STRING,
    OP,
    FUNC_IDENT,
    FUNC_CALL,
    PARAM,
};

typedef struct NODE
{
    struct NODE * left;
    struct NODE * right;

    struct NODE * parent;

    NODE_TYPES node_type = NOT_INITED;
    DATA data  = {};
} NODE;

// ============================================= END =============================================

// ================================== TOKEN TABLE DATA STRUCTS ===================================

typedef struct
{
    const char * name    = NULL;
    OPERATORS    token   = NOT_OPERATOR;
    size_t       length  = 0;
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
    OP_DATA equals          = {};
    OP_DATA split           = {};
    OP_DATA ret             = {};
    OP_DATA inp             = {};
    OP_DATA powc            = {};
    OP_DATA print           = {};
    OP_DATA more            = {};
    OP_DATA less            = {};
    OP_DATA grater_or_eq    = {};
    OP_DATA less_or_eq      = {};
    OP_DATA or_             = {};
    OP_DATA and_            = {};
    OP_DATA start_func      = {};
    OP_DATA end_func        = {};
    OP_DATA eq_equals       = {};
} ALL_OPS_DATAS;

typedef struct 
{
    NODE_TYPES token_type  = NOT_INITED;
    DATA       token       = {};
    size_t     token_size  = 0;
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

// ============================================= END =============================================

char *        DefFunc                   (TOKEN_TABLE * table, char * text, size_t * ip);
char *        ReadBody                  (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ids_table, char * text, size_t * ip);
char *        ProcessCharIdent          (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text, size_t * ip);
char *        ProcessAlphas             (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table,  char * text, size_t * ip);
char *        ProcessGettingParameters  (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text, size_t * ip);
char *        ProcessInitIdent          (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text, size_t * ip, ENUM_IDENT_DATA_TYPE id_type_enum, size_t offset);
inline char * ProcessCycles             (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, OPERATORS op_type, char * text, size_t * ip);
char *        ProcessConditions         (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table,  char * text, size_t * ip);
size_t        FindRepitedIds            (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text);
inline char * ProcessNums               (TOKEN_TABLE * table, char * text, size_t * ip);
char *        FillParameters            (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table,  char * text, size_t * ip, size_t func_ip);


NODE *        GetFunctions              (TOKEN * token_table, size_t * ip);
inline char * DefFunc                   (TOKEN_TABLE * table, char * text, size_t * ip);

NODE *        GetFunc                   (TOKEN * token_table, size_t * ip);

NODE *        GetAllParameters          (TOKEN * token_table, size_t * ip, FUNC_TYPE type, size_t * n_params);
NODE *        GetParam                  (TOKEN * token_table, size_t * ip, FUNC_TYPE type, size_t * n_params);

NODE *        GetBodyOps                (TOKEN * token_table, size_t * ip);
NODE *        GetAction                 (TOKEN * token_table, size_t * ip);

NODE *        GetFuncCall               (TOKEN * token_table, size_t * ip);

NODE *        GetOp                     (TOKEN * token_table, size_t * ip);
NODE *        GetReturn                 (TOKEN * token_table, size_t * ip);
NODE *        GetIf                     (TOKEN * token_table, size_t * ip);
NODE *        GetWhile                  (TOKEN * token_table, size_t * ip);
NODE *        GetPrint                  (TOKEN * token_table, size_t * ip);

NODE *        GetCompare                (TOKEN * token_table, size_t * ip);
NODE *        GetCondition              (TOKEN * token_table, size_t * ip);

NODE *        GetEquals                 (TOKEN * token_table, size_t * ip);

NODE *        GetExpression             (TOKEN * token_table, size_t * ip);
NODE *        GetT                      (TOKEN * token_table, size_t * ip);
NODE *        GetDegree                 (TOKEN * token_table, size_t * ip);
NODE *        GetMathFunc               (TOKEN * token_table, size_t * ip);
NODE *        GetP                      (TOKEN * token_table, size_t * ip);
NODE *        GetTerm                   (TOKEN * token_table, size_t * ip);
int           SyntaxError               (TOKEN * token_table, size_t * ip, const char * func_name);

NODE *        CallocNode                (NODE * left_node, NODE * right_node);
NODE *        NewStringNode             (char * elem, NODE * left_node, NODE * right_node);
NODE *        NewNumNode                (NODE_TYPES type, int elem, NODE * left_node, NODE * right_node);
NODE *        NewIdentNode              (NODE_TYPES type, IDENT_DATA ident, NODE * left_node, NODE * right_node);
NODE *        NewOpNode                 (NODE_TYPES type, OPERATORS op, NODE * left_node, NODE * right_node);

int           GraphDump                 (NODE * node, const char * file_name);
int           RecurcyDumpFill           (FILE * file, NODE * node);

#endif