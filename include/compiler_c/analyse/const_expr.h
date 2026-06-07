#ifndef COMPILER_C_CONST_EXPR_H
#define COMPILER_C_CONST_EXPR_H

#include "compiler_c/core/type.h"

/* Evaluate parsed const expression and evaluate it into a ConstLiteral */
ConstLiteral evaluate_const_expression(const Node *node);
/* Convert N_LITERAL to ConstLiteral */
ConstLiteral evaluate_const_literal(const Node *node);

void free_const_literal(ConstLiteral *l);

void print_const_literal(const ConstLiteral *node);

#endif
