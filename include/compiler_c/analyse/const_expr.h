#ifndef COMPILER_C_CONST_EXPR_H
#define COMPILER_C_CONST_EXPR_H

#include "compiler_c/core/node.h"
#include <stdint.h>

typedef enum {
    CONST_INTEGER,
    CONST_FLOAT,
    CONST_INIT_LIST,
} ConstExprType;

typedef struct ConstExpr ConstExpr;

struct ConstExpr {
    ConstExprType kind;
    union {
        double f;
        int64_t i;
        Array init_list;
    };
};

ConstExpr evaluate_const_expression(Node *const_expr);
void print_const_expr(const ConstExpr *expr);

#endif
