#ifndef COMPILER_C_NODE_H
#define COMPILER_C_NODE_H

#include "tokenizer.h"

typedef enum {
    N_TRANSLATION_UNIT,
    N_FUNCTION,
    N_COMPOUND,
    N_VAR_DECL,
    N_IF,
    N_WHILE,
    N_RETURN,
    N_BINARY,
    N_LITERAL,
    N_IDENTIFIER,
} NodeType;

typedef struct Node Node;

struct Node {
    NodeType type;
    union {
        struct {
            Node **declarations;
            int capacity;
            int count;
        } translation_unit;
        struct {
            const char *name;
            int param_count;
            Node **params;
            TokenType return_type;
            Node *body;
        } function;
        struct {
            Node **statements;
            int capacity;
            int count;
        } compound;
        struct {
            Node *lhs;
            Node *rhs;
            TokenType op;
        } binary;
        struct {
            Node *expr;
        } _return;
        struct {
            Node *cond;
            Node *if_true;
            Node *if_false;
        } _if;
        struct {
            Node *cond;
            Node *block;
        } _while;
        struct {
            TokenType type;
            union {
                int i;
                float f;
            };
        } literal;
        struct {
            char *name;
        } identifier;
        struct {
            char *name;
            TokenType type;
            Node *expr;
        } var_decl;
    };
};

typedef struct {
    int count;
    int capacity;
    Node *nodes;
} NodeManager;

#define NODE_ARENA_SIZE 1024

NodeManager new_node_manager();
void free_node_manager(const NodeManager *nm);

/*
    Handles creating a Node, pushing it to the global node array
*/
Node *new_node(NodeManager *nm,NodeType type);

void print_node_type(NodeType type);
// Prints a single node
void print_node_flat(const Node *node);

void print_indent(int depth);

void print_node(const Node *node,int depth);
void print_nodes(NodeManager *nm);

/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm);

#endif // COMPILER_C_NODE_H
