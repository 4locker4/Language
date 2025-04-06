#ifndef DSL_H
#define DSL_H

#define IS_TYPE_(tok_type) token_table[*ip].token_type == tok_type
#define OP_TYPE_(op_type)  token_table[*ip].token.op == op_type
#define TAKE_IDENT_(what)  table->tokens_array[i].token.ident.ident_##what

#define NUM_(num)     NewNumNode (NUM, num, NULL, NULL)
#define IDENT_(ident) NewIdentNode (IDENT, ident, NULL, NULL)
#define REP_          NewConnectingNode (REPLACEMENT, node->left);

#define ADD_(node_1, node_2)  NewOpNode (OP, ADD, node_1, node_2)
#define SUB_(node_1, node_2)  NewOpNode (OP, SUB, node_1, node_2)
#define MUL_(node_1, node_2)  NewOpNode (OP, MUL, node_1, node_2)
#define DIV_(node_1, node_2)  NewOpNode (OP, DIV, node_1, node_2)
#define POW_(node_1, node_2)  NewOpNode (OP, POW, node_1, node_2)
#define SIN_(node)            NewOpNode (OP, SIN, node, NULL)
#define COS_(node)            NewOpNode (OP, COS, node, NULL)
#define SQRT_(node)           NewOpNode (OP, SQRT, node, NULL)
#define LN_(node)             NewOpNode (OP, LN, node, NULL)

#define DIF_(dif)             Diffirentiation (dif, ident)

#define COPY_(root)           CopyTree (root)

#define R_DTOR(node)                                            \
                    {                                           \
                        NODE * new_node = node->left;           \
                                                                \
                        TreeDtor (node, WITH_RIGHT_BRANCH);     \
                                                                \
                        return new_node;                        \
                    }

#define L_DTOR(node)                                            \
                    {                                           \
                        NODE * new_node = node->right;          \
                                                                \
                        TreeDtor (node, WITH_LEFT_BRACH);       \
                                                                \
                        return new_node;                        \
                    }

#endif