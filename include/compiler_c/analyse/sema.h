#ifndef COMPILER_C_SEMA_H
#define COMPILER_C_SEMA_H
#include "compiler_c/core/node.h"
#include "compiler_c/parse/parser.h"

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
void semantic_analysis(Parser *p, NodeManager *nm, Node *node, Node *loop);

/*
    Converts all AST enum field nodes to integer literals. Must be called after semantic analysis has computed types.
*/
void lower_enums(const NodeManager *nm);
#endif // COMPILER_C_SEMA_H
