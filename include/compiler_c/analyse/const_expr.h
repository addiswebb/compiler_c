#ifndef COMPILER_C_CONST_EXPR_H
#define COMPILER_C_CONST_EXPR_H

#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include <stdint.h>

ConstLiteral evaluate_const_expression(const Node *node);
ConstLiteral evaluate_const_literal(const Node *node);
void print_const_literal(const ConstLiteral *node);

void free_const_literal(ConstLiteral *l);

#endif
