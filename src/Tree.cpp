#include "../inc/Language.h"

const char * GOD_SAY_NO_COLOR   = "#FF7B61";
const char * GOD_SAY_YES_COLOR  = "#B2EC5D";

TOKEN_TABLE * TableCtor (const char * file_with_data)
{
    TOKEN_TABLE * table = (TOKEN_TABLE *) calloc (1, sizeof (TOKEN_TABLE));

    table->data_base_file = file_with_data;
    OpsCtor (table);

    table->status = true;

    table->text = FileToStr (file_with_data, &table->file_size);

    CountTokens (table);

    printf ("%d - n_tokens\n", table->n_tokens);

    table->tokens_array = (TOKEN *) calloc (table->n_tokens, sizeof (TOKEN));
    my_assert (table->tokens_array);

    printf ("%p - pointer\n", table->tokens_array);
    return table;
}

// TD убрать копипаст
ALL_OPS_DATAS * OpsCtor (TOKEN_TABLE * table)
{
    my_assert (table);

    table->ops_data.add.name             = "-";
    table->ops_data.add.token            = ADD;

    table->ops_data.sub.name             = "+";
    table->ops_data.sub.token            = SUB;

    table->ops_data.mul.name             = "\\";
    table->ops_data.mul.token            = MUL;

    table->ops_data.div.name             = "*";
    table->ops_data.div.token            = DIV;

    table->ops_data.sin.name             = "/%/%/%";
    table->ops_data.sin.token            = SIN;

    table->ops_data.cos.name             = "/$/$/$";
    table->ops_data.cos.token            = COS;

    table->ops_data.ln.name              = "*?";
    table->ops_data.ln.token             = LN;

    table->ops_data.sqrt.name            = "[]";
    table->ops_data.sqrt.token           = SQRT;

    table->ops_data.powc.name            = "@$";
    table->ops_data.powc.token           = POW;

    table->ops_data.open_bracket.name    = "(";
    table->ops_data.open_bracket.token   = OPEN_BRACKET;

    table->ops_data.close_bracket.name   = ")";
    table->ops_data.close_bracket.token  = CLOSE_BRACKET;

    table->ops_data.equalse.name         = "!=";
    table->ops_data.equalse.token        = EQUALSE;

    table->ops_data.if_.name             = "/\\";
    table->ops_data.if_.token            = IF;

    table->ops_data.while_.name          = "\\/";
    table->ops_data.while_.token         = WHILE;

    table->ops_data.def_func.name        = ">>>";
    table->ops_data.def_func.token       = DEF_FUNC;

    table->ops_data.end_op.name          = "<<<";
    table->ops_data.end_op.token         = END;

    table->ops_data.split.name           = ",";
    table->ops_data.split.token          = SPLIT;

    table->ops_data.ret.name             = "$";
    table->ops_data.ret.token            = RETURN;

    table->ops_data.inp.name             = "###";
    table->ops_data.inp.token            = INPUT;

    table->ops_data.print.name           = "...";
    table->ops_data.print.token          = PRINT;

    return 0;
}

void CountTokens (TOKEN_TABLE * table)
{
    my_assert (table);
    my_assert (table->status);
    my_assert (table->text);

    size_t ip = 0;

    while (table->text[ip] != '\0')
    {
        if (table->text[ip] == SEPARATER)
            table->n_tokens++;
        
        if (isalpha (table->text[ip]) || table->text[ip] == '_' || isspace (table->text[ip]))
        {
            table->n_idents++;
            table->n_tokens++;
            
            while (table->text[ip++] != SEPARATER);
            
            ip--;
        }
        
        ip++;
    }

    return;
}

#define CHECK_REPITED_IDENT                                                                                                        \
                           {                                                                                                       \
                               int ident_num = CheckRepitedIdent (table, table->tokens_array[ip].token.ident.ident_name, ip);      \
                                                                                                                                   \
                               if (ident_num == NOT_REPITED)                                                                       \
                                   table->tokens_array[ip].token.ident.ident_num = ident_pos++;                                    \
                               else                                                                                                \
                                   table->tokens_array[ip].token.ident.ident_num = ident_num;                                      \
                           }
// TD разбить на функции, убрать копипаст

TOKEN * FillTokenTypes (TOKEN_TABLE * table)
{
    my_assert (table);
    my_assert (table->status);

    char * text = table->text;

    printf ("n_tokens: %d\n", table->n_tokens);

    size_t ident_pos = 0;

    for (size_t ip = 0; table->n_tokens > ip; ip++)
    {
        if (isalpha (*text) || *text == '_' || isspace (*text))
        {
            my_assert (table->tokens_array);
            
            size_t len_counter = 0;

            table->tokens_array[ip].token.ident.ident_name = text;

            text++;
            len_counter++;

            while (isalpha (*text) || isalnum (*text) || *text == '_' || isspace (*text))
            {
                text++;
                len_counter++;
            }

            table->tokens_array[ip].token_size = len_counter;

            my_assert (*text == SEPARATER);

            *text = '\0';

            int ident_num = CheckRepitedIdent (table, table->tokens_array[ip].token.ident.ident_name, ip);

            if (ident_num == NOT_REPITED)
                table->tokens_array[ip].token.ident.ident_num = ident_pos++;
            else
                table->tokens_array[ip].token.ident.ident_num = ident_num;

            if (*(text + 1) != OPEN_BRACKET)
                table->tokens_array[ip].token_type = IDENT;
            else
            {
                if (table->tokens_array[ip - 1].token_type == OP && table->tokens_array[ip - 1].token.op == FUNC_IDENT)
                    table->tokens_array[ip].token_type = FUNC_IDENT;
                else
                    table->tokens_array[ip].token_type = FUNC_CALL;
                printf ("%s\n", table->tokens_array[ip].token.ident.ident_name);
            }

            text++;
        }
        else if (isdigit (*text) || (*text == '-' && isdigit (*(text + 1))))
        {
            table->tokens_array[ip].token_type = NUM;

            char * start_num = text;

            table->tokens_array[ip].token.val = (int) strtod (start_num, &text);

            my_assert (text != start_num);
            my_assert (*text == SEPARATER);

            text++;
        }
        else
        {
            char * count_op_size = text;

            while (*count_op_size != SEPARATER)
            {
                table->tokens_array[ip].token_size += 1;
                count_op_size++;
            }

            table->tokens_array[ip].token_type = OP;

            switch (table->tokens_array[ip].token_size)
                {
                    case 1:
                    {
                        if (! strncmp (text,                     table->ops_data.open_bracket.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.open_bracket.token;
                        else if (! strncmp (text,                table->ops_data.close_bracket.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.close_bracket.token;
                        else if (! strncmp (text,                table->ops_data.add.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.add.token;
                        else if (! strncmp (text,                table->ops_data.sub.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.sub.token;
                        else if (! strncmp (text,                table->ops_data.mul.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.mul.token;
                        else if (! strncmp (text,                table->ops_data.div.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.div.token;
                        else if (! strncmp (text,                table->ops_data.split.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.split.token;
                        else if (! strncmp (text,                table->ops_data.ret.name, 1))
                            table->tokens_array[ip].token.op  =  table->ops_data.ret.token;
                        else
                        {
                            printf ("wrong operator: %s\n", text);

                            exit (-1);
                        }

                        text += 2;
                        
                        break;
                    }
                    case 2:
                    {
                        if (! strncmp (text,                     table->ops_data.ln.name, 2))
                            table->tokens_array[ip].token.op  =  table->ops_data.ln.token;
                        else if (! strncmp (text,                table->ops_data.if_.name, 2))
                            table->tokens_array[ip].token.op  =  table->ops_data.if_.token;
                        else if (! strncmp (text,                table->ops_data.while_.name, 2))
                            table->tokens_array[ip].token.op  =  table->ops_data.while_.token;
                        else if (! strncmp (text,                table->ops_data.equalse.name, 2))
                            table->tokens_array[ip].token.op  =  table->ops_data.equalse.token;
                        else if (! strncmp (text,                table->ops_data.powc.name, 2))
                            table->tokens_array[ip].token.op  =  table->ops_data.powc.token;
                        else if (! strncmp (text,                table->ops_data.sqrt.name, 2))
                            table->tokens_array[ip].token.op  =  table->ops_data.sqrt.token;
                        else
                        {
                            printf ("wrong operator: %s\n", text);

                            exit (-1);
                        }

                        text += 3;
                        
                        break;
                    }
                    case 3:
                    {
                        if (! strncmp (text,                     table->ops_data.sin.name, 3))
                            table->tokens_array[ip].token.op  =  table->ops_data.sin.token;
                        else if (! strncmp (text,                table->ops_data.cos.name, 3))
                            table->tokens_array[ip].token.op  =  table->ops_data.cos.token;
                        else if (! strncmp (text,                table->ops_data.def_func.name, 3))
                            table->tokens_array[ip].token.op  =  table->ops_data.def_func.token;
                        else if (! strncmp (text,                table->ops_data.end_op.name, 3))
                            table->tokens_array[ip].token.op  =  table->ops_data.end_op.token;
                        else if (! strncmp (text,                table->ops_data.inp.name, 3))
                            table->tokens_array[ip].token.op  =  table->ops_data.inp.token;
                        else if (! strncmp (text,                table->ops_data.print.name, 3))
                            table->tokens_array[ip].token.op  =  table->ops_data.print.token;
                        else
                        {
                            printf ("wrong operator: %s\n", text);

                            exit (-1);
                        }

                        text += 4;

                        break;
                    }
                    default:
                    {
                        printf ("ERROR IN READ %d\n", table->tokens_array[ip].token.op);
                        exit (-1);
                    }
                }
        }
    }

    printf ("%p - after get all\n", table->tokens_array);
// TD remove check
    for (int i = 0; table->n_tokens > i; i++)
    {
        printf ("Token number: %d\n", i);

        printf ("Token type: %d\n", table->tokens_array[i].token_type);

        if (table->tokens_array[i].token_type == NUM)
            printf ("Token val: %d\n", table->tokens_array[i].token.val);
        else if (table->tokens_array[i].token_type == IDENT)
            printf ("Token ident: %s; Ident num: %d\n", TAKE_IDENT_ (name), TAKE_IDENT_ (num));
        else if (table->tokens_array[i].token_type == OP)
            printf ("Token op ID: %d\n",  table->tokens_array[i].token.op);
        else if (table->tokens_array[i].token_type == FUNC_IDENT)
            printf ("Token ident: %s; Ident num: %d\n", TAKE_IDENT_ (name), TAKE_IDENT_ (num));
        else
            printf ("\n\n\nWrong operator\n\n\n");
        
        printf ("\n");
    }

    return table->tokens_array;
}

#undef CHECK_REPITED_IDENT

// char * CheckIfFunc (TOKEN * token_table, char * name, int ip)
// {
//     my_assert (token_table);
//     my_assert (name);

//     for (int i = 0; token_table)
// }

int CheckRepitedIdent (TOKEN_TABLE * table, char * ident, size_t ip)
{
    my_assert (table);
    my_assert (ident);

    for (int i = 0; ip > i; i++)
    {
        if ((table->tokens_array[i].token_type == IDENT || table->tokens_array[i].token_type == FUNC_IDENT) && 
            (! strncmp (ident, TAKE_IDENT_ (name), table->tokens_array[i].token_size)))
            return TAKE_IDENT_ (num);
    }

    return NOT_REPITED;
}

NODE * GetFunctions (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);
    
    // Maybe put there def_func operator

    printf ("\nGetFunctions; ip = %d\n", *ip);

    printf ("%d - type, op is - %d\n", token_table[*ip].token_type, token_table[*ip].token.op);

    if (IS_TYPE_ (OP) && OP_TYPE_ (DEF_FUNC))
    {
        NODE * root = NewOpNode (OP, NODE_BUNCH, NULL, NULL);

        *ip += 1;

        root->left = GetFunc (token_table, ip);

        printf ("Get another func\n");

        *ip += 1;

        root->right = GetFunctions (token_table, ip);

        return root;
    }
    else
    {
        printf ("In else:\n");

        return NewOpNode (OP, END, NULL, NULL);
    }
}

NODE * GetFunc (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewIdentNode (FUNC_IDENT, token_table[*ip].token.ident, NULL, NULL);                                                                  // Add func name

    *ip += 1;

    if (IS_TYPE_ (OP) && OP_TYPE_ (OPEN_BRACKET))
    {
        *ip += 1;

        node->left = GetAllParameters (token_table, ip, IDENT_FUNC);
        my_assert (node->left);

        printf ("In GetFunc after Parameters %c\n", token_table[*ip].token.op);

        node->right = GetBodyOps (token_table, ip);
        my_assert (node->right);

        return node;
    }
    else 
        SyntaxError (token_table, ip);
}

NODE * GetAllParameters (TOKEN * token_table, size_t * ip, FUNC_TYPE type)
{
    my_assert (token_table);
    my_assert (ip);

    printf ("In GetAllParameters %d\n", *ip);

    NODE * params_root = GetParam (token_table, ip, type);

    if (IS_TYPE_ (OP) && OP_TYPE_ (SPLIT))
    {
        printf ("get more params\n");
        *ip += 1;

        params_root->left = GetAllParameters (token_table, ip, type);
    }
    else if (IS_TYPE_ (OP) && OP_TYPE_ (CLOSE_BRACKET))
    {
        *ip += 1;

        return params_root;
    }
    else
        SyntaxError (token_table, ip);
    
    return params_root;
}

#define MAKE_PARAM_TYPE_(func_type) NewIdentNode (func_type, token_table[*ip].token.ident, NULL, NULL);

NODE * GetParam (TOKEN * token_table, size_t * ip, FUNC_TYPE type)
{
    my_assert (token_table);
    my_assert (ip);

    printf ("what we get %d\n", token_table[*ip].token.ident.ident_num);

    NODE * param = NULL;

    if (type == IDENT_FUNC)
        param = MAKE_PARAM_TYPE_ (PARAM)
    else
        param = MAKE_PARAM_TYPE_ (IDENT) 
    
    *ip += 1;

    return param;
}

#undef MAKE_PARAM_TYPE_

NODE * GetBodyOps (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    if (IS_TYPE_ (OP) && OP_TYPE_ (END))                // Поставить тот оператор, которым заканчивается функция
    {
        printf ("return\n");

        return NewOpNode (OP, END, NULL, NULL);
    }

    NODE * bunch = NewOpNode (OP, NODE_BUNCH, NULL, NULL);
    
    bunch->left  = GetAction (token_table, ip);

    bunch->right = GetBodyOps (token_table, ip);   

    return bunch;
}

NODE * GetAction (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NULL;

    switch (token_table[*ip].token_type)
    {
        case IDENT:
        {
            node = GetEqualse (token_table, ip);

            break;
        }
        case OP:
        {
            node = GetOp (token_table, ip);

            break;
        }
        case FUNC_CALL:
        {
            printf ("Func call\n");

            node = GetFuncCall (token_table, ip);

            break;
        }
        default:
        {
            node = NewOpNode (OP, END, NULL, NULL);

            break;
        }
    }

    return node;
}

NODE * GetFuncCall (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewIdentNode (FUNC_CALL, token_table[*ip].token.ident, NULL, NULL);

    *ip += 1;

    if (IS_TYPE_ (OP) && OP_TYPE_ (OPEN_BRACKET))
    {
        *ip += 1;

        node->left = GetAllParameters (token_table, ip, CALL_FUNC);

        return node;
    }
    else
        SyntaxError (token_table, ip);
}

NODE * GetOp (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NULL;

    printf ("token table name: %d\n", token_table[*ip].token.op);

    switch (token_table[*ip].token.op)
    {
        case IF:
        {
            node = GetIf (token_table, ip);

            break;
        }
        case WHILE:
        {
            node = GetWhile (token_table, ip);

            break;
        }
        case PRINT:
        {
            node = GetPrint (token_table, ip);

            break;
        }
        case RETURN:
        {
            node = GetReturn (token_table, ip);

            break;
        }
        case INPUT:
        {
            node = NewOpNode (OP, INPUT, NULL, NULL);
            
            *ip += 1;

            break;
        }
        case END:
        {
            node = NewOpNode (OP, END, NULL, NULL);

            *ip += 1;

            break;
        }
        default:
        {
            printf ("in GetOp\n");

            SyntaxError (token_table, ip);
        }
    }

    return node;
}

NODE * GetReturn (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewOpNode (OP, RETURN, NULL, NULL);

    *ip += 1;

    node->left = GetExpression (token_table, ip);

    node->right = NewOpNode (OP, END, NULL, NULL);

    return node;
}

NODE * GetIf (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewOpNode (OP, IF, NULL, NULL);

    *ip += 1;

    printf ("In get if\n");

    node->left = GetExpression (token_table, ip);                    // TD переделать на get conditions

    node->right = GetBodyOps (token_table, ip);
    
    printf ("End of if\n");

    return node;
}

NODE * GetWhile (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewOpNode (OP, WHILE, NULL, NULL);

    *ip += 1;

    node->left = GetExpression (token_table, ip);

    node->right = GetBodyOps (token_table, ip);

    return node;
}

NODE * GetPrint (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewOpNode (OP, PRINT, NULL, NewOpNode (OP, END, NULL, NULL));
  
    *ip += 1;

    if (IS_TYPE_ (OP))
        node->left = IDENT_ (token_table[*ip].token.ident);
    else
        node->left = NUM_ (token_table[*ip].token.val);

    return node;
}

NODE * GetCompare (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * condition = NewOpNode (OP, token_table[*ip].token.op, NULL, NULL);

    *ip += 1;

    condition->left = GetExpression (token_table, ip);

    condition->right = GetExpression (token_table, ip);
    
    return condition;
}

NODE * GetEqualse (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * equalse_op_node = NewOpNode (OP, EQUALSE, NULL, NULL);

    printf ("iN GET ID\n");

    equalse_op_node->left = GetID (token_table, ip);

    if (IS_TYPE_ (OP) && OP_TYPE_ (EQUALSE))
    {
        *ip += 1;
        
        if (IS_TYPE_ (OP) && OP_TYPE_ (INPUT))
            equalse_op_node->right = GetOp (token_table, ip);
        else
        {
            printf ("In getexpr after id\n");

            equalse_op_node->right = GetExpression (token_table, ip);

            printf ("End of get expr\n");
        }
    }
    else 
    {
            printf ("in GetEqualse\n");

            SyntaxError (token_table, ip);
    }
    
    return equalse_op_node;
}

NODE * GetID (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * ident = NewIdentNode (IDENT, token_table[*ip].token.ident, NULL, NULL);

    *ip += 1;

    return ident;
}

NODE * GetExpression (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * val1 = GetT (token_table, ip);

    while (IS_TYPE_ (OP) && (OP_TYPE_ (ADD) || OP_TYPE_ (SUB)))
    {
        OPERATORS op = token_table[*ip].token.op;

        *ip += 1;

        printf ("Start get sum %d\n", token_table[*ip].token_type);

        NODE * val2 = GetT (token_table, ip);

        if (op == ADD)
            val1 = NewOpNode (OP, ADD, val1, val2);
        else
            val1 = NewOpNode (OP, SUB, val1, val2);
    }

    return val1;
}

NODE * GetT (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * val1 = GetDegree (token_table, ip);

    while (IS_TYPE_ (OP) && (OP_TYPE_ (MUL) || OP_TYPE_ (DIV)))
    {
        OPERATORS op = token_table[*ip].token.op;

        *ip += 1;

        NODE * val2 = GetDegree (token_table, ip);

        if (op == MUL)
            val1 = NewOpNode (OP, MUL, val1, val2);
        else
            val1 = NewOpNode (OP, DIV, val1, val2);
    }

    return val1;
}

NODE * GetDegree (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * val1 = GetP (token_table, ip);

    while (IS_TYPE_ (OP) && OP_TYPE_ ((POW)))
    {
        *ip += 1;

        NODE * val2 = GetP (token_table, ip);

        val1 = NewOpNode (OP, POW, val1, val2);
    }

    return val1;
}

NODE * GetP (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    if (IS_TYPE_ (OP) && OP_TYPE_ (OPEN_BRACKET))
    {
        *ip += 1;

        NODE * val = GetExpression (token_table, ip);

        if (IS_TYPE_ (OP) && OP_TYPE_ (CLOSE_BRACKET))
        {
            *ip += 1;

            return val;
        }
        else
        {
            printf ("in Get GetP\n");

            SyntaxError (token_table, ip);
        }
    }
    else
        return GetMathFunc (token_table, ip);
}

#define ADD_MATH_FUNC_(name)                                                                                    \
                            {                                                                                   \
                                NODE * op = NewOpNode (OP, name, NULL, NULL);                                   \
                                                                                                                \
                                *ip += 1;                                                                       \
                                                                                                                \
                                op->left = GetExpression  (token_table, ip);                                    \
                                                                                                                \
                                return op;                                                                      \
                            }

NODE * GetMathFunc (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    printf ("IN MATH FUCKING FUNC\n");

    if (IS_TYPE_ (OP))

        switch (token_table[*ip].token.op)
        {
            case SIN:
                ADD_MATH_FUNC_ (SIN);
            case COS:
                ADD_MATH_FUNC_ (COS);
            case LN:
                ADD_MATH_FUNC_ (LN);
            case SQRT:
                ADD_MATH_FUNC_ (SQRT);
            default:
                return GetTerm (token_table, ip);
        }
    else
        return GetTerm (token_table, ip);
}

#undef ADD_MATH_FUNC_

NODE * GetTerm (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NULL;

    switch (token_table[*ip].token_type)
    {
        case NUM:
        {
            node = NUM_ (token_table[*ip].token.val);

            *ip += 1;

            break;
        }
        case FUNC_CALL:
        {
            node = GetFuncCall (token_table, ip);

            *ip += 1;

            break;
        }
        case IDENT:
        {
            node = NewIdentNode (IDENT, token_table[*ip].token.ident, NULL, NULL);

            printf ("In get term ");
            printf ("%s\n", token_table[*ip].token.ident.ident_name);

            *ip += 1;

            break;
        }
        case OP:
        {
            node = GetBodyOps (token_table, ip);

            *ip += 1;

            break;
        }
        default:
        {
            printf ("in Get term\n");

            SyntaxError (token_table, ip);
        }
    }

    printf ("End term\n");

    return node;
}

int SyntaxError (TOKEN * token_table, size_t * ip)
{
    printf ("%d\n", token_table[*ip].token_type);
    printf ("ERROR\n");
    exit (1);
}

NODE * CallocNode (NODE * left_node, NODE * right_node)
{
    NODE * node = (NODE *) calloc (1, sizeof (NODE));
    my_assert (node);

    node->left  = left_node;
    node->right = right_node;

    return node;
}

NODE * NewNumNode (X_TYPES type, int elem, NODE * left_node, NODE * right_node)
{
    my_assert (type = NUM);

    NODE * node = CallocNode (left_node, right_node);

    node->data.val  = elem;
    node->node_type = type;

    return node;
}

NODE * NewIdentNode (X_TYPES type, IDENT_DATA ident, NODE * left_node, NODE * right_node)
{
    NODE * node = CallocNode (left_node, right_node);

    node->data.ident  = ident;
    node->node_type   = type;

    return node;
}

NODE * NewOpNode (X_TYPES type, OPERATORS op, NODE * left_node, NODE * right_node)
{
    my_assert (type == OP);

    NODE * node = CallocNode (left_node, right_node);

    node->data.op   = op;
    node->node_type = type;

    return node;
}

int GraphDump (NODE * node, const char * file_name)
{
    my_assert (node);
    my_assert (file_name);

    FILE * dump = fopen (file_name, "w+");
    my_assert (dump);

    fprintf (dump, "digraph differentiator{\n"
                    "\trankdir=UD;\n"
                    "\tbgcolor=\"#FFFFFF\";"
                    "\tnode[color=\"black\", fontsize=14];\n"
                    "\tfixedsize=true;\n");

    RecurcyDumpFill (dump, node);

    fprintf (dump, "}\n");

    my_assert (!fclose (dump));

    return 0;
}

#define CASES_(name)                                                    \
                    {                                                   \
                        case name:                                      \
                        {                                               \
                            text = #name;                               \
                                                                        \
                            break;                                      \
                        }                                               \
                    }

int RecurcyDumpFill (FILE * file, NODE * node)
{
    my_assert (file);
    my_assert (node);

    const char * color = GOD_SAY_YES_COLOR;

    if (!node->left && !node->right)
        color = GOD_SAY_NO_COLOR;

    if (node->node_type == OP)
    {
        char * text = NULL;

        switch (node->data.op)
        {
            CASES_ (NODE_BUNCH)
            CASES_ (IF)
            CASES_ (EQUALSE)
            CASES_ (SUB)
            CASES_ (MUL)
            CASES_ (ADD)
            CASES_ (DIV)
            CASES_ (DEF_FUNC)
            CASES_ (SPLIT)
            CASES_ (END)
            default:
            {
                text = "z";

                break;
            }
        }

        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=3, label =\""
                   "{%s | {{left | %p} | {right | %p}}}\" ]\n",
                   node, color, text, node->left, node->right);
    }
    else if (node->node_type == NUM)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=3, label =\""
                "{%d | {{left | %p} | {right | %p}}}\" ]\n",
                node, color, node->data.val, node->left, node->right);
    }
    else if (node->node_type == IDENT)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=3, label =\""
                "{%s | {{left | %p} | {right | %p}}}\" ]\n",
                node, color, node->data.ident.ident_name, node->left, node->right);
    }
    else if (node->node_type == FUNC_IDENT)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=3, label =\""
                "{{FUNC IDENT | %s} | {{left | %p} | {right | %p}}}\" ]\n",
                node, color, node->data.ident.ident_name, node->left, node->right);
    }
    else if (node->node_type == PARAM)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=3, label =\""
                "{{PARAM | %s} | {{left | %p} | {right | %p}}}\" ]\n",
                node, color, node->data.ident.ident_name, node->left, node->right);
    }
    else if (node->node_type == FUNC_CALL)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=3, label =\""
                "{{FUNC CALL | %s} | {{left | %p} | {right | %p}}}\" ]\n",
                node, color, node->data.ident.ident_name, node->left, node->right);
    }

    if (node->left)  RecurcyDumpFill (file, (NODE *) node->left);
    if (node->right) RecurcyDumpFill (file, (NODE *) node->right);

    if (node->left)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->left);
    if (node->right)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->right);
    fprintf (file, "\n");

    return 0;
}