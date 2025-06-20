#include "../inc/Language.h"

const char * GOD_SAY_NO_COLOR   = "#FF7B61";
const char * GOD_SAY_YES_COLOR  = "#B2EC5D";

/*
    В FillTokenTypes: 

    1) Сделать маску IsFuncIdent, потому что после shema_buhicha идет всегда название функции
*/

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

    return table;
}

ALL_OPS_DATAS * OpsCtor (TOKEN_TABLE * table)
{
    my_assert (table);

    table->ops_data.add.name             = "+";
    table->ops_data.add.token            = ADD;

    table->ops_data.sub.name             = "-";
    table->ops_data.sub.token            = SUB;

    table->ops_data.mul.name             = "*";
    table->ops_data.mul.token            = MUL;

    table->ops_data.div.name             = "/";
    table->ops_data.div.token            = DIV;

    table->ops_data.sin.name             = "sidr";
    table->ops_data.sin.token            = SIN;

    table->ops_data.cos.name             = "koniak";
    table->ops_data.cos.token            = COS;

    table->ops_data.ln.name              = "liker";
    table->ops_data.ln.token             = LN;

    table->ops_data.sqrt.name            = "sake";
    table->ops_data.sqrt.token           = SQRT;

    table->ops_data.powc.name            = "'";
    table->ops_data.powc.token           = POW;

    table->ops_data.open_bracket.name    = "(";
    table->ops_data.open_bracket.token   = OPEN_BRACKET;

    table->ops_data.close_bracket.name   = ")";
    table->ops_data.close_bracket.token  = CLOSE_BRACKET;

    table->ops_data.equalse.name         = "piva_z";
    table->ops_data.equalse.token        = EQUALSE;

    table->ops_data.if_.name             = "esli_krepkost_alko";
    table->ops_data.if_.token            = IF;

    table->ops_data.while_.name          = "poka_krepkost_alko";
    table->ops_data.while_.token         = WHILE;

    table->ops_data.def_func.name        = "shema_buhicha";
    table->ops_data.def_func.token       = DEF_FUNC;

    table->ops_data.end_op.name          = "vodka";
    table->ops_data.end_op.token         = END;

    table->ops_data.split.name           = ",";
    table->ops_data.split.token          = SPLIT;

    table->ops_data.ret.name             = "rvota";
    table->ops_data.ret.token            = RETURN;

    table->ops_data.inp.name             = "trebuemoe_kolichestvo_alko";
    table->ops_data.inp.token            = INPUT;

    table->ops_data.print.name           = "na_utro_posle_NK";
    table->ops_data.print.token          = PRINT;

    table->ops_data.less.name            = "<";
    table->ops_data.less.token           = LESS;

    table->ops_data.more.name            = ">";
    table->ops_data.more.token           = MORE;

    table->ops_data.less_or_eq.name      = "<=";
    table->ops_data.less_or_eq.token     = LESS_OR_EQ;

    table->ops_data.grater_or_eq.name    = ">=";
    table->ops_data.grater_or_eq.token   = GREATER_OR_EQ;

    table->ops_data.or_.name             = "blay_ili";
    table->ops_data.or_.token            = OR;

    table->ops_data.and_.name            = "cyka_i_eche";
    table->ops_data.and_.token           = AND;

    table->ops_data.start_func.name      = "{";
    table->ops_data.start_func.token     = START_FUNC;
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
        if (isalpha (table->text[ip]) || isalnum (table->text[ip]) || table->text[ip] == '_')
        {
            table->n_tokens++;
            ip++;
            
            while (isalpha (table->text[ip]) || isalnum (table->text[ip]) || table->text[ip] == '_')
                ip++;

            ip--;
        }
        else if (!isspace (table->text[ip]))
        {
            table->n_tokens++;

            if (table->text[ip + 1] == '=')
                ip++;
        }

        if (table->text[ip] != '\0') ip++;
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

#define CHECK_IF_OP(get_nm)                                                                                                        \
                                if (!strncasecmp (text, table->ops_data.get_nm.name, strlen (table->ops_data.get_nm.name) * sizeof (char)))        \
                                {                                                                                                  \
                                    table->tokens_array[*ip].token.op  =  table->ops_data.get_nm.token;                             \
                                    table->tokens_array[*ip].token_type = OP;                                                       \
                                    *text += sizeof (char) * strlen (table->ops_data.get_nm.name);                                  \
                                                                                                                                   \
                                    return;                                                                                      \
                                }

TOKEN * FillTokenTypes (TOKEN_TABLE * table)
{
    my_assert (table);
    my_assert (table->status);

    char * text = table->text;

    printf ("n_tokens: %d\n", table->n_tokens);

    size_t ident_pos = 0;
    size_t len_counter = 0;

    char * pointer_to_ident_start = NULL;

    size_t ident_num = 0;
    size_t rep_num   = 0;

    for (size_t ip = 0; table->n_tokens > ip; ip++)
    {
        while (isspace (*text))
            text++;

        if (isalpha (*text) || *text == '_')
        {
            ProcessAlpha (text, table, &len_counter, &ip, &ident_num);
// ========================================== END ==========================================
        }
        else if (isdigit (*text) || (*text == '-' && isdigit (*(text + 1))))
        {
            table->tokens_array[ip].token_type = NUM;

            char * start_num = text;

            table->tokens_array[ip].token.val = (int) strtod (start_num, &text);

            my_assert (text != start_num);
        }
#define FILL_TABLE_TOKEN(token_name) table->tokens_array[ip].token.op  =  table->ops_data.##token_name.token;
        else
        {
            table->tokens_array[ip].token_type = OP;

            switch (*text++)
            {
                case LESS:
                {
                    if (*text == '=')
                    {
                        FILL_TABLE_TOKEN (less_or_eq)
                        
                        text++;
                    }
                    else
                        FILL_TABLE_TOKEN (less)
                    
                    break;
                }
                case MORE:
                {
                    if (*text == '=')
                    {
                        FILL_TABLE_TOKEN (grater_or_eq)

                        text++;
                    }
                    else
                        FILL_TABLE_TOKEN (more)

                    break;
                }
                case ADD:
                {
                    FILL_TABLE_TOKEN (add)

                    break;
                }
                case SUB:
                {
                    FILL_TABLE_TOKEN (sub)

                    break;
                }
                case MUL:
                {
                    FILL_TABLE_TOKEN (mul)

                    break;
                }
                case DIV:
                {
                    FILL_TABLE_TOKEN (div)

                    break;
                }
                case POW:
                {
                    FILL_TABLE_TOKEN (powc)

                    break;
                }
                case SPLIT:
                {
                    FILL_TABLE_TOKEN (split)

                    break;
                }
                case CLOSE_BRACKET:
                {
                    FILL_TABLE_TOKEN (close_bracket)

                    break;
                }
                case OPEN_BRACKET:
                {
                    FILL_TABLE_TOKEN (open_bracket)

                    break;
                }
                case START_FUNC:
                {
                    FILL_TABLE_TOKEN (start_func)

                    break;
                }
                case END_FUNC:
                {
                    FILL_TABLE_TOKEN (end_op)

                    break;
                }
                default:
                {
                    COLOR_PRINT (RED, "Error! Wrong operator!\n");

                    printf ("%s\n", text);

                    exit (1);
                }
            }
        }
    }

    return table->tokens_array;
}

#undef FILL_TABLE_TOKEN

inline void ProcessAlpha (char * text, TOKEN_TABLE * table, size_t * len_counter, size_t * ip, size_t * ident_num)
{
    my_assert (table->tokens_array);
            
    *len_counter = 0;

    CHECK_IF_OP (sin);
    CHECK_IF_OP (cos);
    CHECK_IF_OP (ln);
    CHECK_IF_OP (sqrt);
    CHECK_IF_OP (equalse);
    CHECK_IF_OP (if_);
    CHECK_IF_OP (while_);
    CHECK_IF_OP (end_op);
    CHECK_IF_OP (ret);
    CHECK_IF_OP (inp);
    CHECK_IF_OP (print);
    CHECK_IF_OP (or_);
    CHECK_IF_OP (and_);

    if (!strncasecmp (text, table->ops_data.def_func.name, strlen (table->ops_data.def_func.name) * sizeof (char)))
    {
        table->tokens_array[*ip].token.op  =  table->ops_data.def_func.token;
        text += sizeof (char) * strlen (table->ops_data.def_func.name);  
        
        table->tokens_array[*ip].token_type = OP;

        printf ("%s\n", text);
        
        return;
    }

// ================================ TAKE IDENT OR FUNC NAME ================================
    char * pointer_to_ident_start = text;

    text++;
    len_counter++;

    while (isalpha (*text) || isalnum (*text) || *text == '_')
    {
        text++;
        len_counter++;
    }

    table->tokens_array[*ip].token_size = *len_counter;

    memcpy (table->tokens_array[*ip].token.ident.ident_name, pointer_to_ident_start, *len_counter);
    table->tokens_array[*ip].token.ident.ident_name[*len_counter + 1] = '\0';

// ========================================== END ==========================================

// ============================== LOOK ELEMENT IN TOKEN TABLE ==============================

    size_t rep_num = CheckRepitedIdent (table, table->tokens_array[*ip].token.ident.ident_name, *ip);

    if (rep_num == NOT_REPITED)
    {
        table->tokens_array[*ip].token.ident.ident_num = *ident_num;                // If element not into token table, put it in it

        *ident_num += 1;
        
        table->n_idents++;
    }
    else
        table->tokens_array[*ip].token.ident.ident_num = rep_num;                   // Else, give it it`s ident number

// ========================================== END ==========================================

// ================= SWITCH TO IDENT TYPE (IDENT, FUNC_IDENT OR FUNC_CALL) =================

    if (*text++ != OPEN_BRACKET && *text++ != OPEN_BRACKET)                         // If on next or +2 pos there is no bracket, it is
    {                                                                               // ident
        table->tokens_array[*ip].token_type = IDENT;
        table->tokens_array[*ip].token.ident.ident_num = *ident_num;

        *ident_num += 1;

        table->n_idents++;

        text -= 2;
    }
    else                                                                            // Else - switch to FUNC_IDENT or FUNC_CALL
    {
        if (table->tokens_array[*ip - 1].token_type == OP && table->tokens_array[*ip - 1].token.op == DEF_FUNC)
        {
            table->tokens_array[*ip].token_type = FUNC_IDENT;

            ident_num = 0;
        }
        else
            table->tokens_array[*ip].token_type = FUNC_CALL;
        
        text--;
    }
}

#undef CHECK_REPITED_IDENT

int CheckRepitedIdent (TOKEN_TABLE * table, char * ident, size_t ip)
{
    my_assert (table);
    my_assert (ident);

    for (size_t i = 0; ip > i; i++)
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
    
    if (IS_TYPE_ (OP) && OP_TYPE_ (DEF_FUNC))
    {
        NODE * root = NewOpNode (OP, NODE_BUNCH, NULL, NULL);

        *ip += 1;

        root->left = GetFunc (token_table, ip);

        root->left->parent = root;

        *ip += 1;

        root->right = GetFunctions (token_table, ip);

        root->left->parent = root;

        return root;
    }
    else
        return NewOpNode (OP, END, NULL, NULL);
}

NODE * GetFunc (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewIdentNode (FUNC_IDENT, token_table[*ip].token.ident, NULL, NULL);

    node->data.val = token_table[*ip].token.val;

    *ip += 1;

    if (IS_TYPE_ (OP) && OP_TYPE_ (OPEN_BRACKET))
    {
        *ip += 1;

        size_t n_params = 0;

        node->left = GetAllParameters (token_table, ip, IDENT_FUNC, &n_params);
        node->left->parent = node;
        my_assert (node->left);

        node->data.n_params = n_params;

        if (IS_TYPE_ (OP) && OP_TYPE_ (START_FUNC)) *ip += 1;
        else                                        SyntaxError (token_table, ip, "GetFunc, is`s not START_FUNC\n");

        node->right = GetBodyOps (token_table, ip);
        my_assert (node->right);
        node->right->parent = node;

        return node;
    }
    else 
        SyntaxError (token_table, ip, "GetFunc\n");
}

NODE * GetAllParameters (TOKEN * token_table, size_t * ip, FUNC_TYPE type, size_t * n_params)
{
    my_assert (token_table);
    my_assert (ip);
    
    NODE * params_root = GetParam (token_table, ip, type, n_params);

    if (IS_TYPE_ (OP) && OP_TYPE_ (SPLIT))
    {
        *ip += 1;

        params_root->left = GetAllParameters (token_table, ip, type, n_params);
    }
    else if (IS_TYPE_ (OP) && OP_TYPE_ (CLOSE_BRACKET))
    {
        *ip += 1;

        params_root->right = NULL;

        return params_root;
    }
    else
        SyntaxError (token_table, ip, "GetAllParameters\n");
    
    return params_root;
}

#define MAKE_PARAM_TYPE_(func_type) NewIdentNode (func_type, token_table[*ip].token.ident, NULL, NULL);

NODE * GetParam (TOKEN * token_table, size_t * ip, FUNC_TYPE type, size_t * n_params)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * param = NULL;

    if (type == IDENT_FUNC)
    {
        param = MAKE_PARAM_TYPE_ (PARAM)

        *n_params += 1;
    }
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

    if (IS_TYPE_ (OP) && OP_TYPE_ (END))
        return NewOpNode (OP, END, NULL, NULL);

    NODE * bunch = NewOpNode (OP, NODE_BUNCH, NULL, NULL);
    
    bunch->left  = GetAction (token_table, ip);

    bunch->left->parent = bunch;

    if (IS_TYPE_ (OP) && OP_TYPE_ (END))
        *ip += 1;
    else
        SyntaxError (token_table, ip, "GetBodyOps\n");
    
    bunch->right = GetBodyOps (token_table, ip);   

    bunch->right->parent = bunch;

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
            COLOR_PRINT (YELLOW, "Equalse\n");
            node = GetEqualse (token_table, ip);

            return node;
        }
        case OP:
        {
            node = GetOp (token_table, ip);

            return node;
        }
        case FUNC_CALL:
        {
            node = GetFuncCall (token_table, ip);

            return node;
        }
        default:
        {
            node = NewOpNode (OP, END, NULL, NULL);

            return node;
        }
    }
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

        node->left = GetAllParameters (token_table, ip, CALL_FUNC, NULL);

        node->left->parent = node;

        return node;
    }
    else
        SyntaxError (token_table, ip, "GetFuncCall");
}

NODE * GetOp (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NULL;
    printf ("token now: %d\n", token_table[*ip].token.op);

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
            COLOR_PRINT (GREEN, "IN RETURN\n");
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
            SyntaxError (token_table, ip, "GetOp\n");
    }

    return node;
}

NODE * GetReturn (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    COLOR_PRINT (RED, "I AM RETURNING!\n");
    NODE * node = NewOpNode (OP, RETURN, NULL, NULL);

    *ip += 1;

    node->left = GetExpression (token_table, ip);

    node->right = NewOpNode (OP, END, NULL, NULL);

    node->left->parent  = node;
    node->right->parent = node;

    return node;
}

NODE * GetIf (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewOpNode (OP, IF, NULL, NULL);

    *ip += 1;

    if (IS_TYPE_ (OP) && OP_TYPE_ (OPEN_BRACKET))   *ip += 1;
    else                                            SyntaxError (token_table, ip, "GetIf, it`s not op in open bracket\n");

    node->left = GetCondition (token_table, ip);

    if (IS_TYPE_ (OP) && OP_TYPE_ (CLOSE_BRACKET))  *ip += 1;
    else                                            SyntaxError (token_table, ip, "GetIf, it`s not close bracket\n");
    if (IS_TYPE_ (OP) && OP_TYPE_ (START_FUNC))     *ip += 1;
    else                                            SyntaxError (token_table, ip, "GetIf, it`s not start_func\n");

    node->right = GetBodyOps (token_table, ip);

    node->left->parent  = node;
    node->right->parent = node;
    
    return node;
}

NODE * GetCondition (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = GetCompare (token_table, ip);

    while (IS_TYPE_(OP) && (OP_TYPE_ (OR) || OP_TYPE_ (AND)))
    {
        OPERATORS op = token_table[*ip].token.op;

        *ip += 1;

        NODE * node_r = GetCompare (token_table, ip);
        
        NODE * node_l = node;

        switch (op)
        {
            case AND:
            {
                node = NewOpNode (OP, AND, node_l, node_r);

                node->left->parent  = node;
                node->right->parent = node;

                break;
            }
            case OR:
            {
                node = NewOpNode (OP, OR, node_l, node_r);

                node->left->parent  = node;
                node->right->parent = node;

                break;
            }
            default:
                break;
        }
    }

    return node;
}

NODE * GetWhile (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    NODE * node = NewOpNode (OP, WHILE, NULL, NULL);

    *ip += 1;

    node->left = GetCondition (token_table, ip);

    node->right = GetBodyOps (token_table, ip);

    node->left->parent  = node;
    node->right->parent = node;

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

    node->left->parent  = node;

    return node;
}

NODE * GetCompare (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    OPERATORS op = NOT_OPERATOR;

    NODE * node_l = GetExpression (token_table, ip);

    if (IS_TYPE_ (OP)) op = token_table[*ip].token.op;
    else               SyntaxError (token_table, ip, "GetCompare\n");

    *ip += 1;

    NODE * node_r = GetExpression (token_table, ip);

    printf ("Second expr taken\n");

    NODE * condition = NewOpNode (OP, op, node_l, node_r);

    condition->left->parent  = condition;
    condition->right->parent = condition;
    
    return condition;
}

NODE * GetEqualse (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);


    NODE * equalse_op_node = NewOpNode (OP, EQUALSE, NULL, NULL);

    equalse_op_node->left = NewIdentNode (IDENT, token_table[*ip].token.ident, NULL, NULL);

    *ip += 1;

    printf ("%c\n", token_table[*ip].token.val + 61);

    if (IS_TYPE_ (OP) && OP_TYPE_ (EQUALSE))
    {
        *ip += 1;
        
        if (IS_TYPE_ (OP) && OP_TYPE_ (INPUT))
            equalse_op_node->right = GetOp (token_table, ip);
        else
            equalse_op_node->right = GetExpression (token_table, ip);

    }
    else 
        SyntaxError (token_table, ip, "GetEqualse\n");
    
    equalse_op_node->left->parent  = equalse_op_node;
    equalse_op_node->right->parent = equalse_op_node;
    
    printf ("END OF EQUALSE\n");
    return equalse_op_node;
}

NODE * GetExpression (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);

    COLOR_PRINT (BLUE, "ITS EXPRESSION\n");
    NODE * val1 = GetT (token_table, ip);

    while (IS_TYPE_ (OP) && (OP_TYPE_ (ADD) || OP_TYPE_ (SUB)))
    {
        OPERATORS op = token_table[*ip].token.op;

        *ip += 1;

        NODE * val2 = GetT (token_table, ip);

        if (op == ADD)
            val1 = NewOpNode (OP, ADD, val1, val2);
        else
            val1 = NewOpNode (OP, SUB, val1, val2);
    }
    printf ("DAAAWN\n");

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
            SyntaxError (token_table, ip, "GetP\n");
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

            *ip += 1;

            break;
        }
        case OP:
        {
            if (OP_TYPE_ (CLOSE_BRACKET))
                return node;
        }
        default:
            SyntaxError (token_table, ip, "GetTerm\n");
    }

    return node;
}

int SyntaxError (TOKEN * token_table, size_t * ip, const char * func_name)
{
    printf ("ERROR\t");
    printf ("type: %d\n", token_table[*ip].token_type);

    if (token_table[*ip].token_type == OP)
    {
        printf ("%c\n", token_table[*ip].token.op);
        printf ("%s\n", func_name);
    }
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
            CASES_ (OR)
            CASES_ (AND)
            CASES_ (MORE)
            CASES_ (LESS)
            CASES_ (GREATER_OR_EQ)
            CASES_ (LESS_OR_EQ)
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