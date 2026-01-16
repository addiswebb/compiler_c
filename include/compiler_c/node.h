#ifndef COMPILER_C_NODE_H
#define COMPILER_C_NODE_H

#include "type.h"
#include "tokenizer.h"

typedef enum {
    N_TRANSLATION_UNIT,
    N_FUNCTION,
    N_COMPOUND,
    N_VAR_DECL,
    N_IF,
    N_WHILE,
    N_FOR,
    N_RETURN,
    N_UNARY,
    N_BINARY,
    N_LITERAL,
    N_IDENTIFIER,
    N_FUNCTION_CALL,
    N_CAST,
    N_INDEX,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Type* type;
    // The type of the values associated with this node
    union {
        struct {
            Node **declarations;
            int capacity;
            int count;
        } translation_unit;
        struct {
            const char *name;
            int param_count;
            int param_capacity;
            Node **params;
            Node *body;
        } func;
        struct {
            Node **items;
            int capacity;
            int count;
        } compound;
        // op expr | expr op
        struct {
            Node *expr;
            TokenType op;
            int associativity;
        } unary;
        // lhs op rhs
        struct {
            Node *lhs;
            Node *rhs;
            TokenType op;
        } binary;
        // return expr;
        struct {
            Node *expr;
        } _return;
        // if (cond) {if_true} else {if_else}
        struct {
            Node *cond;
            Node *if_true;
            Node *if_false;
        } _if;
        // while (cond) {block}
        struct {
            Node *cond;
            Node *block;
        } _while;
        // for (init; cond; end) {block}
        struct {
            Node *init;
            Node *cond;
            Node *iter;
            Node *block;
        } _for;
        struct {
            union {
                long long i;
                double f;
                char*s;
            };
        } literal;
        struct {
            char *name;
        } identifier;
        // type name = expr;
        struct {
            char *name;
            Node *expr;
        } var_decl;
        // identifier(params*)
        struct {
            Node *identifier;
            int param_count;
            int param_capacity;
            Node **params;
        } func_call;
        // (int)0.5
        struct {
            Node *expr;
            Type *from;
            Type *to;
        } cast;
        struct {
            Node *identifier;
            Node *index;
        } index;
    };
};

typedef struct {
    int count;
    int capacity;
    Node *nodes;
} NodeManager;

#define NODE_ARENA_SIZE 1024

void init_types();

Type *init_type(TypeKind type, int size);
NodeManager new_node_manager();
void free_node_manager(const NodeManager *nm);

/*
    Handles creating a Node, pushing it to the global node array
*/
Node *new_node(NodeManager *nm, NodeKind type);
Node *cast_node(NodeManager *nm, Node *node, Type *type);
bool is_valid_cast(Type *from, Type *to);

void print_type(Type* type);
void print_node_type(NodeKind type);

void print_indent(int depth);

void print_node(const Node *node,int depth);

/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm);

#endif // COMPILER_C_NODE_H
