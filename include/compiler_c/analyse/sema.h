#ifndef COMPILER_C_SEMA_H
#define COMPILER_C_SEMA_H
#include "compiler_c/core/array.h"
#include "compiler_c/core/node.h"
#include "compiler_c/parse/parser.h"
#include <complex.h>

typedef struct{
    Node *loop;
    Node *func;
    Node *compound;
    Array i_array;
}SemanticContext;

/* Is a node which is assignable */
bool is_lvalue(const Node *n);
/* Is a unary node with the `*`, unary operator. */
bool is_deref(const Node *n);

/*
    Checks the given unary operation. Ensures valid operator and operand combination.
    Returns the correct result type if safe, otherwise throws an error.
*/
Type *check_unary_op(NodeManager *nm, Node *unary_op);

/*
    Checks the given binary operation. Ensures valid operator and operands. Promotes where necessary.
    Returns the correct result type if safe, otherwise throws an error.
*/
Type *check_binary_op(NodeManager *nm, TokenType op, Node *binop);
/*
    Promotes and decays nodes to allow safe and correct binary operations.
    Ensures the types are the same and returns their common type.
    Exception: `Pointer` `Integer` arithmetic is allowed and also handled, returns the pointer type in this case.
*/
Type *promote_binary_operands(NodeManager *nm, Node *binop);

/*
    Recursively traverses the AST, performing semantic correctness checks on every node.
    Ensures that the resulting AST is safe and correct to lower to IR.
    Casts types, lowers enums, handles literals, assigns correct types, lowers `a->` to `*(a).b`.
*/
void semantic_analysis(SemanticContext *sema_ctx,Parser *p, NodeManager *nm, Node *node);

/*
    Lowers enums and compound literals to their literal counterparts.
*/
void lower_nodes(NodeManager *nm);

/*
    Convert compount literal into a variable declaration+initialization
    `foo(&(Type){1,2,3});`
    into
    ```c
    static Type _tmp = {1,2,3};
    foo(&_tmp);
    ```
*/
void lower_compound_literal(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Node *node);
// TODO Consider having the actual conversion after sema, but add the symbols during sema.

/*
    Handles pushing a new symbol table and compound + index tracker onto the stack.
*/
void push_sema_scope(SemanticContext *sema_ctx, Parser *p, Node *n);

/*
    Handles poping the symbol table and compound + index tracker off the top of the stack.
*/
void pop_sema_scope(SemanticContext *sema_ctx, Parser *p);

static inline int* get_i(SemanticContext *sema_ctx){
    return (int *)get(&sema_ctx->i_array, sema_ctx->i_array.count-1);
}

#endif // COMPILER_C_SEMA_H
