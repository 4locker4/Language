#include "../inc/Language.h"

const char * GOD_SAY_NO_COLOR   = "#FF7B61";
const char * GOD_SAY_YES_COLOR  = "#B2EC5D";

/*
При парсинге сделать везде один пробел
*/
static size_t n_funcs = 0;

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

void TableDtor (TOKEN_TABLE * table)
{
    my_assert (table);

    free (table->text);

    free (table->tokens_array);

    if (table->tree) TreeDestr (table->tree);

    return;
}

int TreeDestr (NODE * node)
{
    if (node->left)  TreeDestr (node->left);
    if (node->right) TreeDestr (node->right);

    free (node);

    return 0;
}

ALL_OPS_DATAS * OpsCtor (TOKEN_TABLE * table)
{
    my_assert (table);

    table->ops_data.add.name             = "+";
    table->ops_data.add.token            = ADD;
    table->ops_data.add.length           = sizeof ("+") - 1;

    table->ops_data.sub.name             = "-";
    table->ops_data.sub.token            = SUB;
    table->ops_data.sub.length           = sizeof ("-") - 1;

    table->ops_data.mul.name             = "*";
    table->ops_data.mul.token            = MUL;
    table->ops_data.mul.length           = sizeof ("*") - 1;

    table->ops_data.div.name             = "/";
    table->ops_data.div.token            = DIV;
    table->ops_data.div.length           = sizeof ("/") - 1;

    table->ops_data.sin.name             = "sidr";
    table->ops_data.sin.token            = SIN;
    table->ops_data.sin.length           = sizeof ("sidr") - 1;

    table->ops_data.cos.name             = "koniak";
    table->ops_data.cos.token            = COS;
    table->ops_data.cos.length           = sizeof ("koniak") - 1;

    table->ops_data.ln.name              = "liker";
    table->ops_data.ln.token             = LN;
    table->ops_data.ln.length            = sizeof ("liker") - 1;

    table->ops_data.sqrt.name            = "sake";
    table->ops_data.sqrt.token           = SQRT;
    table->ops_data.sqrt.length          = sizeof ("sake") - 1;

    table->ops_data.powc.name            = "^";
    table->ops_data.powc.token           = POW;
    table->ops_data.powc.length          = sizeof ("^") - 1;

    table->ops_data.open_bracket.name    = "(";
    table->ops_data.open_bracket.token   = OPEN_BRACKET;
    table->ops_data.open_bracket.length  = sizeof ("(") - 1;

    table->ops_data.close_bracket.name   = ")";
    table->ops_data.close_bracket.token  = CLOSE_BRACKET;
    table->ops_data.close_bracket.length = sizeof (")") - 1;

    table->ops_data.equals.name          = "piva_z";
    table->ops_data.equals.token         = EQUALS;
    table->ops_data.equals.length        = sizeof ("piva_z") - 1;

    table->ops_data.if_.name             = "esli_krepkost_alko";
    table->ops_data.if_.token            = IF;
    table->ops_data.if_.length           = sizeof ("esli_krepkost_alko") - 1;

    table->ops_data.while_.name          = "poka_krepkost_alko";
    table->ops_data.while_.token         = WHILE;
    table->ops_data.while_.length        = sizeof ("poka_krepkost_alko") - 1;

    table->ops_data.def_func.name        = "shema_buhicha";
    table->ops_data.def_func.token       = DEF_FUNC;
    table->ops_data.def_func.length      = sizeof ("shema_buhicha") - 1;

    table->ops_data.end_op.name          = "vodka";
    table->ops_data.end_op.token         = END;
    table->ops_data.end_op.length        = sizeof ("vodka") - 1;

    table->ops_data.split.name           = ",";
    table->ops_data.split.token          = SPLIT;
    table->ops_data.split.length         = sizeof (",") - 1;

    table->ops_data.ret.name             = "rvota";
    table->ops_data.ret.token            = RETURN;
    table->ops_data.ret.length           = sizeof ("rvota") - 1;

    table->ops_data.inp.name             = "trebuemoe_kolichestvo_alko";
    table->ops_data.inp.token            = INPUT;
    table->ops_data.inp.length           = sizeof ("trebuemoe_kolichestvo_alko") - 1;

    table->ops_data.print.name           = "na_utro_posle_NK";
    table->ops_data.print.token          = PRINT;
    table->ops_data.print.length         = sizeof ("na_utro_posle_NK") - 1;

    table->ops_data.less.name            = "<";
    table->ops_data.less.token           = LESS;
    table->ops_data.less.length          = sizeof ("<") - 1;

    table->ops_data.more.name            = ">";
    table->ops_data.more.token           = MORE;
    table->ops_data.more.length          = sizeof (">") - 1;

    table->ops_data.less_or_eq.name      = "<=";
    table->ops_data.less_or_eq.token     = LESS_OR_EQ;
    table->ops_data.less_or_eq.length    = sizeof ("<=") - 1;

    table->ops_data.grater_or_eq.name    = ">=";
    table->ops_data.grater_or_eq.token   = GREATER_OR_EQ;
    table->ops_data.grater_or_eq.length  = sizeof (">=") - 1;

    table->ops_data.eq_equals.name       = "ebat_takaia_je";
    table->ops_data.eq_equals.token      = EQ_EQUALS;
    table->ops_data.eq_equals.length     = sizeof ("ebat_takaia_je") - 1;

    table->ops_data.or_.name             = "blay_ili";
    table->ops_data.or_.token            = OR;
    table->ops_data.or_.length           = sizeof ("blay_ili") - 1;

    table->ops_data.and_.name            = "cyka_i_eche";
    table->ops_data.and_.token           = AND;
    table->ops_data.and_.length          = sizeof ("cyka_i_eche") - 1;

    table->ops_data.start_func.name      = "{";
    table->ops_data.start_func.token     = START_FUNC;
    table->ops_data.start_func.length    = sizeof ("{") - 1;

    table->ops_data.end_func.name        = "}";
    table->ops_data.end_func.token       = END_FUNC;
    table->ops_data.end_func.length      = sizeof ("}") - 1;

    table->ops_data.inp.name             = "cha_cha_skaju";
    table->ops_data.inp.token            = INPUT;
    table->ops_data.inp.length           = sizeof ("cha_cha_skaju") - 1;

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
        else if (table->text[ip] == START_TEXT)
        {
            table->n_tokens++;

            ip++;

            while (table->text[ip] != START_TEXT)
                ip++;
            
            ip++;
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

#define CHECK_IF_OP(get_nm)                                                                                                                 \
                                if (!strncmp (text, table->ops_data.get_nm.name, table->ops_data.get_nm.length))                            \
                                {                                                                                                           \
                                    table->tokens_array[ip].token.op  =  table->ops_data.get_nm.token;                                      \
                                    table->tokens_array[ip].token_type = OP;                                                                \
                                                                                                                                            \
                                    text += table->ops_data.get_nm.length - 1;                                                              \
                                                                                                                                            \
                                    continue;                                                                                               \
                                }

#define SKIP_SPACES()           while (isspace (*text))                                          \
                                    text++;                                                              

#define SKIP_UNTIL_(stop_symb)  while (*text != stop_symb)                                       \
                                    text++;
                
#define SKIP_IDENT()            while (isalpha (*text) || isalnum (*text) || *text == '_')       \
                                    text++;

TOKEN * FillTokenTypes (TOKEN_TABLE * table)
{

// TD сделать оператор глобализирования переменной. Те переменные, что задаются с этим оператором заносятся в таблицу переменных, которая создается еще в FillTokenTypes.
//    Потом просто закидывать их объявления в сегмент R+W (.data или .bss, если не инитнуты)

    my_assert (table);
    my_assert (table->status);

    char * text = table->text;

    printf ("n_tokens: %d\n", table->n_tokens);

// ================================= START FILLING TOKENS TABLE ==================================

    for (size_t ip = 0; table->n_tokens > ip; ip++)
    {
        SKIP_SPACES ();

        text = DefFunc (table, text, &ip);

        n_funcs++;

        printf ("IP = %d\n", ip);
    }
        
// ============================================= END =============================================

    return table->tokens_array;
}

char * DefFunc (TOKEN_TABLE * table, char * text, size_t * ip)
{
    COLOR_PRINT (MANGETA, "DEFENDED\n");

// ===================================== FUNCTION DEFENITION =====================================

    size_t len_counter = 0;

    IDENTIFICATORS_TABLE ids_table = {};

    ids_table.existing_ids = (size_t *) calloc (IDS_TABLE_SIZE_DELTA, sizeof (size_t));
    my_assert (ids_table.existing_ids);

    ids_table.func_ip = *ip + 1;

    table->tokens_array[*ip].token_type = OP;
    table->tokens_array[*ip].token.op   = DEF_FUNC;

    text += table->ops_data.def_func.length;

    *ip += 1;

    table->tokens_array[*ip].token_type = FUNC_IDENT;

    SKIP_SPACES()

    char * pointer_to_ident_start = text;

    SKIP_IDENT ();

    len_counter = text - pointer_to_ident_start;
    table->tokens_array[*ip].token_size = len_counter;

    memcpy (table->tokens_array[*ip].token.ident.ident_name, pointer_to_ident_start, len_counter);
    table->tokens_array[*ip].token.ident.ident_name[len_counter + 1] = '\0';

    *ip += 1;

// ============================================= END =============================================

// ==================================== PROCESS OPEN BRACKETS ====================================

    SKIP_UNTIL_(OPEN_BRACKET);
    
    table->tokens_array[*ip].token_type = OP;
    table->tokens_array[*ip].token.op   = OPEN_BRACKET;

    text++;

    *ip += 1;

// ============================================= END =============================================

// ========================================= FILL PARAMS =========================================

    text = FillParameters (table, &ids_table, text, ip, ids_table.func_ip);

// ============================================= END =============================================
            
// ===================================== PROCESS START FUNC ======================================

    SKIP_UNTIL_(START_FUNC);

    table->tokens_array[*ip].token_type = OP;
    table->tokens_array[*ip].token.op   = START_FUNC;

    text++;

    *ip += 1;

// ============================================= END =============================================

// ======================================== READ FUNC BODY =======================================

    text = ReadBody (table, &ids_table, text, ip);

    SKIP_SPACES ();

    table->tokens_array[ids_table.func_ip].token.ident.ident_num = ids_table.free_box;

    free (ids_table.existing_ids);

    return text;
}

char * ReadBody (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ids_table, char * text, size_t * ip)
{
    while (true)
    {
        COLOR_PRINT (BLUE, "IP = %u\n", *ip);
        SKIP_SPACES ();

        if (isalpha (*text) || *text == '_')
            text = ProcessAlphas (table, ids_table, text, ip);
        else if (*text == '-' && *(text + 1) == ' ')
        {
            table->tokens_array[*ip].token_type = OP;

            text++;

            table->tokens_array[*ip].token.op  =  table->ops_data.sub.token;
        }
        else if (*text == '-' || isalnum (*text))
            text = ProcessNums (table, text, ip);
        else
        {
            table->tokens_array[*ip].token_type = OP;

            switch (*text++)
            {
                case START_TEXT:
                {                    
                    text = ProcessCharIdent (table, ids_table, text, ip);

                    break;
                }
                case LESS:
                {
                    if (*text == '=')
                    {
                        table->tokens_array[*ip].token.op  =  table->ops_data.less_or_eq.token;
                        
                        text++;
                    }
                    else
                    table->tokens_array[*ip].token.op  =  table->ops_data.less.token;
                    
                    break;
                }
                case MORE:
                {
                    if (*text == '=')
                    {
                        table->tokens_array[*ip].token.op  =  table->ops_data.grater_or_eq.token;

                        text++;
                    }
                    else
                        table->tokens_array[*ip].token.op  =  table->ops_data.more.token;

                    break;
                }
                case ADD:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.add.token;

                    break;
                }
                case SUB:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.sub.token;

                    break;
                }
                case MUL:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.mul.token;

                    break;
                }
                case DIV:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.div.token;

                    break;
                }
                case POW:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.powc.token;

                    break;
                }
                case SPLIT:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.split.token;

                    break;
                }
                case CLOSE_BRACKET:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.close_bracket.token;

                    break;
                }
                case OPEN_BRACKET:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.open_bracket.token;

                    break;
                }
                case START_FUNC:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.start_func.token;

                    break;
                }
                case END_FUNC:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.end_op.token;

                    return text;
                }
                default:
                {
                    COLOR_PRINT (RED, "Error! Wrong operator!\n");

                    printf ("%s\n", text);

                    TableDtor (table);

                    exit (1);
                }
            }
        }

        *ip += 1;
    }
}

char * ProcessCharIdent (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text, size_t * ip)
{
    size_t string_len = 0;
    char * pointer_to_ident_start = text;
    
    table->tokens_array[*ip].token_type = STRING;
    table->tokens_array[*ip].token.ident.ident_data_type = CHAR;

    SKIP_UNTIL_(START_TEXT);
    
    string_len = text - pointer_to_ident_start + 1;

    if (string_len > MAX_STRING_LENGHT)
    {
        *text = '\0';
        
        COLOR_PRINT (RED, "ERROR! String <%s> is more than appear (%d - max lenght)\n", pointer_to_ident_start, MAX_STRING_LENGHT);

        TableDtor (table);

        exit (1);
    }

    memcpy (table->tokens_array[*ip].token.ident.ident_name, pointer_to_ident_start, string_len - 1);
    table->tokens_array[*ip].token.ident.ident_name[string_len] = '\0';

    table->tokens_array[*ip].token_size = string_len;

    COLOR_PRINT (GREEN, "%s\n", table->tokens_array[*ip].token.ident.ident_name)

    text++;     // skip < " >

    return text;
}

#define ADD_ID_INTO_TABLE()     table->tokens_array[*ip].token.ident.ident_num = ident_table->free_box;                                                             \
                                ident_table->existing_ids[ident_table->free_box++] = *ip;                                                                           \
                                                                                                                                                                    \
                                if (ident_table->table_size <= ident_table->free_box)                                                                               \
                                {                                                                                                                                   \
                                    ident_table->table_size  += IDS_TABLE_SIZE_DELTA;                                                                               \
                                    ident_table->existing_ids = (size_t *) realloc (ident_table->existing_ids, ident_table->table_size * sizeof (u_int64_t));                            \
                                }
#define CHECK_IF_OP(get_nm)                                                                                                                                         \
                                else if (! strncmp (text, table->ops_data.get_nm.name, table->ops_data.get_nm.length))                                              \
                                {                                                                                                                                   \
                                    table->tokens_array[*ip].token_type = OP;                                                                                       \
                                    table->tokens_array[*ip].token.op   = table->ops_data.get_nm.token;                                                             \
                                                                                                                                                                    \
                                    text += table->ops_data.get_nm.length;                                                                                          \
                                    printf ("END ", table->ops_data.get_nm.name);                                                                               \
                                                                                                                                                                    \
                                    printf ("%s\n", table->ops_data.get_nm.name);                                                                               \
                                }

char * ProcessAlphas (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table,  char * text, size_t * ip)
{
    char * pointer_to_ident_start = text;
    size_t len_counter            = 0;

//  AFTER TYPE INT STRNCMP NEED A SPACE!!!  //

    SKIP_SPACES ();

    if (! strncmp (text, "size_t ", sizeof ("size_t")))
    {
        printf ("In size_t\t");

        table->n_tokens--;

        text = ProcessInitIdent (table, ident_table, text, ip, UNSIGNED_INT, sizeof ("size_t"));

        printf ("end size_t\n");

        ADD_ID_INTO_TABLE ();
        COLOR_PRINT (YELLOW, "ident type = %d\n", table->tokens_array[*ip].token.ident.ident_data_type);

        printf ("Added into var table\n\n");
    }
    else if (! strncmp (text, "int ", sizeof ("int")))
    {
        printf ("In int\t");

        table->n_tokens--;

        text = ProcessInitIdent (table, ident_table, text, ip, SIGNED_INT, sizeof ("int"));

        printf ("end int\n");

        ADD_ID_INTO_TABLE ();
        COLOR_PRINT (YELLOW, "ident type = %d\n", table->tokens_array[*ip].token.ident.ident_data_type);

        printf ("Added into var table\n\n");
    }
    else if (! strncmp (text, "double ", sizeof ("double")))
    {
        table->n_tokens--;

        text = ProcessInitIdent (table, ident_table, text, ip, DOUBLE, sizeof ("double"));
        COLOR_PRINT (YELLOW, "ident type = %d\n", table->tokens_array[*ip].token.ident.ident_data_type);

        ADD_ID_INTO_TABLE ();
    }
    else if (! strncmp (text, "char * ", sizeof ("char *")))
    {
        table->n_tokens -= 2;

        text = ProcessInitIdent (table, ident_table, text, ip, CHAR, sizeof ("char *"));
        COLOR_PRINT (YELLOW, "ident type = %d\n", table->tokens_array[*ip].token.ident.ident_data_type);

        ADD_ID_INTO_TABLE ();
    }
// ====================================== PROCESS MATH FUNCS =====================================

    CHECK_IF_OP (sin)
    CHECK_IF_OP (cos)
    CHECK_IF_OP (ln)
    CHECK_IF_OP (sqrt)
    CHECK_IF_OP (equals)
    CHECK_IF_OP (end_op)
    CHECK_IF_OP (ret)
    CHECK_IF_OP (inp)

// ============================================= END =============================================
    else if (! strncmp (text, table->ops_data.if_.name, table->ops_data.if_.length))
    {        
        text = ProcessCycles (table, ident_table, IF, text, ip);

        printf ("End IF process cycles\n");
    }
    else if (! strncmp (text, table->ops_data.while_.name, table->ops_data.while_.length))
    {        
        text = ProcessCycles (table, ident_table, WHILE, text, ip);

        printf ("End WHILE process cycles\n");
    }
    else
    {
        pointer_to_ident_start = text;

        SKIP_IDENT ();
        
        len_counter = text - pointer_to_ident_start;

        SKIP_SPACES ();

        if (*text == OPEN_BRACKET)
        {
            printf ("start process func call ---> [");

            table->tokens_array[*ip].token_type = FUNC_CALL;

/* This strncmp is not necessary. I just want to save origin func name for func, which I did in my other projects */
            if (! strncmp (pointer_to_ident_start, table->ops_data.print.name, table->ops_data.print.length))
            {
                len_counter = sizeof ("MyPrintf");

                memcpy (table->tokens_array[*ip].token.ident.ident_name, "MyPrintf", len_counter);
            }
            else
            {
                memcpy (table->tokens_array[*ip].token.ident.ident_name, pointer_to_ident_start, len_counter);
                table->tokens_array[*ip].token.ident.ident_name[len_counter + 1] = '\0';
            }

            table->tokens_array[*ip].token_size = len_counter;

            *ip += 1;

            table->tokens_array[*ip].token_type = OP;
            table->tokens_array[*ip].token.op   = OPEN_BRACKET;

            text++;

            *ip += 1;
            
            text = ProcessGettingParameters (table, ident_table, text, ip);

            printf ("] end\n\n");
        }
        else
        {
            printf ("start process read ident ---> ");

            table->tokens_array[*ip] = table->tokens_array[FindRepitedIds (table, ident_table, pointer_to_ident_start)];
            COLOR_PRINT (MANGETA, "ident type = %d\n", table->tokens_array[*ip].token.ident.ident_data_type);

            printf ("%s ---> end\n\n", table->tokens_array[*ip].token.ident.ident_name);
        }
    }

    return text;
}

char * ProcessGettingParameters (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text, size_t * ip)
{
    printf ("start process getting parameters ---> ");

    while (true)
    {
        SKIP_SPACES ();

        if (isalpha (*text) || *text == '_')
        {
            table->tokens_array[*ip] = table->tokens_array[FindRepitedIds (table, ident_table, text)];
            COLOR_PRINT (MANGETA, "ident type = %d\n", table->tokens_array[*ip].token.ident.ident_data_type);
            
            SKIP_IDENT ();
        }
        else if (isalnum (*text))
        {
            table->tokens_array[*ip].token_type = NUM;

            table->tokens_array[*ip].token.val = strtol (text, &text, 10);
        }
        else if (*text == SPLIT)
        {
            table->tokens_array[*ip].token_type = OP;

            table->tokens_array[*ip].token.op   = SPLIT;

            text++;
        }
        else if (*text == CLOSE_BRACKET)
        {
            table->tokens_array[*ip].token_type = OP;

            table->tokens_array[*ip].token.op   = CLOSE_BRACKET;
            
            text++;

            printf ("end");

            return text;
        }
        else if (*text == START_TEXT)
        {
            text++;
            
            text = ProcessCharIdent (table, ident_table, text, ip);

            table->tokens_array[*ip].token.ident.ident_num = *ip;
        }
        else
        {
            COLOR_PRINT (RED, "ERROR! There is something unknown in func parameters\n");

            TableDtor (table);

            exit (1);
        }

        *ip += 1;
    }
}

char * ProcessInitIdent (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text, size_t * ip, ENUM_IDENT_DATA_TYPE id_type_enum, size_t offset)
{                                                         
    printf ("start process init ident ---> ");

    table->tokens_array[*ip].token_type = IDENT;                                                  
    table->tokens_array[*ip].token.ident.ident_data_type = id_type_enum;            
    text += offset;
                                
    SKIP_SPACES ();                                                                                
                                
    char * pointer_to_ident_start = text;                                                                

    SKIP_IDENT ();
                                
    size_t len_counter = text - pointer_to_ident_start;                                              
                                
    memcpy (table->tokens_array[*ip].token.ident.ident_name, pointer_to_ident_start, len_counter);
    table->tokens_array[*ip].token.ident.ident_name[len_counter + 1] = '\0';                      
                                
    table->tokens_array[*ip].token_size = len_counter;                                            
                                
    SKIP_SPACES ();
    
    printf ("end\n");

    return text;
}

inline char * ProcessCycles (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, OPERATORS op_type, char * text, size_t * ip)
{
    printf ("start process cycles ---> ");

    table->tokens_array[*ip].token_type = OP;
    table->tokens_array[*ip].token.op   = op_type;

    text += table->ops_data.if_.length;
    *ip += 1;

    SKIP_SPACES ();

    table->tokens_array[*ip].token_type = OP;
    table->tokens_array[*ip].token.op   = OPEN_BRACKET;

    text++;
    *ip += 1;

    SKIP_SPACES ();

    printf ("%s\n\n", text);

    text = ProcessConditions (table, ident_table, text, ip);

    SKIP_SPACES ();

    table->tokens_array[*ip].token_type = OP;
    table->tokens_array[*ip].token.op   = START_FUNC;

    text++;

    *ip += 1;

    printf ("end\n");

    printf ("%s\n", text);

    return ReadBody (table, ident_table, text, ip);
}

char * ProcessConditions (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table,  char * text, size_t * ip)
{
    char * pointer_to_num_start = NULL;

    while (true)
    {
        SKIP_SPACES ();

        if (! strncmp (text, table->ops_data.and_.name, table->ops_data.and_.length))
        {
            table->tokens_array[*ip].token_type = OP;
            table->tokens_array[*ip].token.op   = AND;

            text += table->ops_data.and_.length;
        }
        else if (! strncmp (text, table->ops_data.or_.name, table->ops_data.or_.length))
        {
            table->tokens_array[*ip].token_type = OP;
            table->tokens_array[*ip].token.op   = OR;

            text += table->ops_data.or_.length;
        }
        else if (! strncmp (text, table->ops_data.eq_equals.name, table->ops_data.eq_equals.length))
        {
            table->tokens_array[*ip].token_type = OP;
            table->tokens_array[*ip].token.op   = EQ_EQUALS;

            text += table->ops_data.eq_equals.length;
        }
        else if (*text == CLOSE_BRACKET)
        {
            printf ("start process close bracket conditions ---> ");

            table->tokens_array[*ip].token_type = OP;
            table->tokens_array[*ip].token.op   = CLOSE_BRACKET;

            text++;

            *ip += 1;

            printf ("end\n");

            return text;
        }
        else if (isalpha (*text) || *text == '_')
        {
            printf ("start process alpha conditions ---> ");

            table->tokens_array[*ip] = table->tokens_array[FindRepitedIds (table, ident_table, text)];
            COLOR_PRINT (MANGETA, "ident type = %d\n", table->tokens_array[*ip].token.ident.ident_data_type);

            SKIP_IDENT ();
                
            printf ("end\n");
        }
        else if (isalnum (*text))
        {        
            text = ProcessNums (table, text, ip);
        }
        else
        {
            table->tokens_array[*ip].token_type = OP;

            switch (*text++)
            {
                case LESS:
                {
                    if (*text == '=')
                    {
                        table->tokens_array[*ip].token.op  =  table->ops_data.less_or_eq.token;

                        text++;
                    }
                    else
                        table->tokens_array[*ip].token.op  =  table->ops_data.less.token;

                    break;
                }
                case MORE:
                {
                    if (*text == '=')
                    {
                        table->tokens_array[*ip].token.op  =  table->ops_data.grater_or_eq.token;

                        text++;
                    }
                    else
                        table->tokens_array[*ip].token.op  =  table->ops_data.more.token;

                    break;
                }
                case EQUALS:
                {
                    table->tokens_array[*ip].token.op  =  table->ops_data.eq_equals.token;

                    text += 2;

                    break;
                }
                default:
                {
                    COLOR_PRINT (RED, "ERROR! There is unknown symbol in condition: <%s>\n", text - 1);

                    TableDtor (table);

                    exit (1);
                }
            }
        }

        *ip += 1;
    }
}

size_t FindRepitedIds (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table, char * text)
{
    size_t free_box = ident_table->free_box;

    for (int i = 0; free_box > i; i++)
    {
        // TD Here maybe size without \0. So if idnt smthng like "strncmpCHORTBLYA" and we have got "strncmp" as ident, it will be the same for prog.
        if (! strncmp (text, table->tokens_array[ident_table->existing_ids[i]].token.ident.ident_name, table->tokens_array[ident_table->existing_ids[i]].token_size))
        {
            printf ("Free is: %d and it is: %d\n", ident_table->free_box, i);
            
            return ident_table->existing_ids[i];
        }
    }

    char * bed_text = text;

    SKIP_IDENT ();

    *text = '\0';

    COLOR_PRINT (RED, "ERROR! The variable <%s> was not declared in this scope\n", bed_text);

    TableDtor (table);
    
    exit (1);
}

inline char * ProcessNums (TOKEN_TABLE * table, char * text, size_t * ip)
{
    printf ("In process num ---> ");
    if (*text == '-')
    {
        table->tokens_array[*ip].token.val  = -1;

        text++;
    }
    else table->tokens_array[*ip].token.val =  1;

    table->tokens_array[*ip].token_type = NUM;
    table->tokens_array[*ip].token.val  *= strtol (text, &text, 10);

    printf ("end\n");

    return text;
}

char * FillParameters (TOKEN_TABLE * table, IDENTIFICATORS_TABLE * ident_table,  char * text, size_t * ip, size_t func_ip)
{
    size_t string_len = 0;

    char * pointer_to_ident_start = NULL;

    while (true)
    {
        SKIP_SPACES ();

        if (*text == CLOSE_BRACKET)
        {
            table->tokens_array[*ip].token_type = OP;
            table->tokens_array[*ip].token.op   = CLOSE_BRACKET;

            *ip += 1;

            text++;
            
            return text;
        }

        if (! strncmp (text, "size_t ", sizeof ("size_t")))
        {
            table->n_tokens--;
            
            table->tokens_array[*ip].token.ident.ident_data_type = UNSIGNED_INT;

            text += sizeof ("size_t");
        }
        else if (! strncmp (text, "int ", sizeof ("int")))
        {
            table->n_tokens--;
            COLOR_PRINT (GREEN, "+1 id\n");
            table->tokens_array[*ip].token.ident.ident_data_type = SIGNED_INT;

            text += sizeof ("int");
        }
        else if (! strncmp (text, "double ", sizeof ("double")))
        {
            table->n_tokens--;
    
            table->tokens_array[*ip].token.ident.ident_data_type = DOUBLE;

            text += sizeof ("double");
        }
        else if (! strncmp (text, "char * ", sizeof ("char *")))
        {
            table->n_tokens -= 2;
    
            table->tokens_array[*ip].token.ident.ident_data_type = CHAR;

            text += sizeof ("char *");
        }
        else
        {
            COLOR_PRINT (RED, "ERROR! Wrong param type\n");

            TableDtor (table);

            exit (1);
        }

        SKIP_SPACES ();

        table->tokens_array[*ip].token_type = PARAM;

        ADD_ID_INTO_TABLE ()

        pointer_to_ident_start = text;

        do     text++;
        while (*text != SPLIT && *text != CLOSE_BRACKET && ! isspace (*text));

        string_len = text - pointer_to_ident_start;

        table->tokens_array[*ip].token_size = string_len;
        
        memcpy (table->tokens_array[*ip].token.ident.ident_name, pointer_to_ident_start, string_len);
        table->tokens_array[*ip].token.ident.ident_name[string_len + 1] = '\0';

        *ip += 1;

        if (*text == CLOSE_BRACKET)
        {
            table->tokens_array[*ip].token_type = OP;
            table->tokens_array[*ip].token.op   = CLOSE_BRACKET;
            
            table->tokens_array[func_ip].token.ident.ident_val.unsigned_type = ident_table->free_box;

            text++;

            *ip += 1;
        
            return text;
        }

        table->tokens_array[*ip].token_type = OP;
        table->tokens_array[*ip].token.op   = SPLIT;

        *ip += 1;

        text++;
    }
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

    printf ("IN GET FUNC\n");

    NODE * node = NewIdentNode (FUNC_IDENT, token_table[*ip].token.ident, NULL, NULL);

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
        else                                        SyntaxError (token_table, ip + 1, "GetFunc, is`s not START_FUNC\n");

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
    
    NODE * params_root = NULL;

    if (IS_TYPE_ (OP) && OP_TYPE_ (CLOSE_BRACKET))
    {
        *ip += 1;

        params_root = NewOpNode (OP, END, NULL, NULL);

        return params_root;
    }

    params_root = GetParam (token_table, ip, type, n_params);

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
    else if (IS_TYPE_ (NUM))
        param = NewNumNode (NUM, token_table[*ip].token.val, NULL, NULL);
    else if (IS_TYPE_ (STRING))
        param = NewStringNode (STRING, token_table[*ip].token.ident, NULL, NULL);
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
        case PARAM:
        case IDENT:
        {
            node = GetEquals (token_table, ip);

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

    while (IS_TYPE_(OP) && (OP_TYPE_ (OR) || OP_TYPE_ (AND) || OP_TYPE_ (EQ_EQUALS)))
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

    if (IS_TYPE_ (OP) && OP_TYPE_ (OPEN_BRACKET))   *ip += 1;
    else                                            SyntaxError (token_table, ip, "GitWHile, it`s not op in open bracket\n");

    node->left = GetCondition (token_table, ip);

    if (IS_TYPE_ (OP) && OP_TYPE_ (CLOSE_BRACKET))  *ip += 1;
    else                                            SyntaxError (token_table, ip, "GitWHile, it`s not close bracket\n");
    if (IS_TYPE_ (OP) && OP_TYPE_ (START_FUNC))     *ip += 1;
    else                                            SyntaxError (token_table, ip, "GitWHile, it`s not start_func\n");

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

NODE * GetEquals (TOKEN * token_table, size_t * ip)
{
    my_assert (token_table);
    my_assert (ip);


    NODE * equals_op_node = NewOpNode (OP, EQUALS, NULL, NULL);

    equals_op_node->left = NewIdentNode (IDENT, token_table[*ip].token.ident, NULL, NULL);

    *ip += 1;

    if (IS_TYPE_ (OP) && OP_TYPE_ (EQUALS))
    {
        *ip += 1;
        
        if (IS_TYPE_ (OP) && OP_TYPE_ (INPUT))
            equals_op_node->right = GetOp (token_table, ip);
        else if (IS_TYPE_ (STRING))
        {
            COLOR_PRINT (RED, "STIRIRIRIRIRN\n");

            equals_op_node->right = NewStringNode (STRING, token_table[*ip].token.ident, NULL, NULL);

            *ip += 1;
        }
        else
            equals_op_node->right = GetExpression (token_table, ip);

    }
    else 
        SyntaxError (token_table, ip, "GetEquals\n");
    
    equals_op_node->left->parent  = equals_op_node;
    equals_op_node->right->parent = equals_op_node;
    
    printf ("END OF equals\n");
    return equals_op_node;
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

            break;
        }
        case PARAM:
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
    COLOR_PRINT (RED, "ERROR\t");
    printf ("type: %d\n", token_table[*ip].token_type);
    printf ("%s\n", func_name);

    if (token_table[*ip].token_type == OP)
    {
        printf ("%c\n", token_table[*ip].token.op);
        printf ("%c\n", token_table[*ip + 1].token.op);
    }
    
    free (token_table);

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

NODE * NewStringNode (NODE_TYPES type, IDENT_DATA ident, NODE * left_node, NODE * right_node)
{
    NODE * node = CallocNode (left_node, right_node);

    node->data.ident  = ident;
    node->node_type   = STRING;

    return node;
}

NODE * NewNumNode (NODE_TYPES type, int elem, NODE * left_node, NODE * right_node)
{
    my_assert (type = NUM);

    NODE * node = CallocNode (left_node, right_node);

    node->data.val  = elem;
    node->node_type = type;

    return node;
}

NODE * NewIdentNode (NODE_TYPES type, IDENT_DATA ident, NODE * left_node, NODE * right_node)
{
    NODE * node = CallocNode (left_node, right_node);

    node->data.ident  = ident;
    node->node_type   = type;

    COLOR_PRINT (GREEN, "ident type: %d\n", node->data.ident.ident_data_type);

    return node;
}

NODE * NewOpNode (NODE_TYPES type, OPERATORS op, NODE * left_node, NODE * right_node)
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
            CASES_ (EQUALS)
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
            CASES_ (EQ_EQUALS)
            CASES_ (START_FUNC)
            CASES_ (RETURN)
            CASES_ (END_FUNC)
            CASES_ (OPEN_BRACKET)
            CASES_ (CLOSE_BRACKET)
            CASES_ (END)
            default:
            {
                text = "CHORT";
                
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
    else if (node->node_type == STRING)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=3, label =\""
                "{{STRING | %s} | {{left | %p} | {right | %p}}}\" ]\n",
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