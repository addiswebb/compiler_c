#ifndef COMPILER_C_SEMA_H
#define COMPILER_C_SEMA_H
#include "compiler_c/node.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/type.h"
bool is_lvalue(const Node *n);
bool is_deref(const Node *n);

Type *check_unary_op(NodeManager *nm, Node *unary_op);
Type *check_binary_op(NodeManager *nm, TokenType op, Node *binop);
Type *promote_binary_operands(NodeManager *nm, Node *binop);
void semantic_analysis(Parser *p, NodeManager *nm, Node *node, Node *loop);
void lower_enums(const NodeManager *nm);
#endif
