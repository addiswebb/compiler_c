#ifndef COMPILER_C_SEMA_H
#define COMPILER_C_SEMA_H
#include "compiler_c/node.h"
#include "compiler_c/parser.h"
#include "compiler_c/type.h"
bool is_lvalue(Node *n);

Type *token_to_type(TokenType t);
Type *check_unary_op(Node *unaryop);
Type *check_binary_op(NodeManager *nm, TokenType op, Node *binop);
Type *promote_binary_operands(NodeManager *nm, Node *binop);
void semantic_analysis(Parser *p, NodeManager *nm, Node *node);
#endif
