#ifndef COMPILER_C_NODE_H
#define COMPILER_C_NODE_H

#include "compiler_c/core/type.h"
#include "compiler_c/tokenize/tokenizer.h"

#include <stdint.h>
#include <string.h>

typedef enum {
    N_TRANSLATION_UNIT,
    N_FUNCTION,
    N_COMPOUND,
    N_VAR_DECL,
    N_IF,
    N_WHILE,
    N_FOR,
    N_SWITCH,
    N_CASE,
    N_RETURN,
    N_UNARY,
    N_BINARY,
    N_LITERAL,
    N_IDENTIFIER,
    N_FUNCTION_CALL,
    N_CAST,
    N_INDEX,
    N_TYPE,
    N_TYPEDEF,
    N_CONTINUE,
    N_BREAK,
    N_INIT_LIST,
    N_MEMBER_ACCESS,
} NodeKind;

typedef enum{
    L_INT,
    L_FLOAT,
    L_CHAR,
    L_STRING,
} LiteralKind;

typedef enum{
    NONE,
    EXTERN,
    STATIC
}StorageClass;

typedef struct Node Node;
typedef struct Symbol Symbol;

struct Node {
    NodeKind kind;
    Type* type;
    // The type of the values associated with this node
    union {
        struct {
            Array declarations_array;
        } translation_unit;
        // type name(params) [body]
        struct {
            Node *type;
            const char *name;
            Array params_array;
            Node *body;
            StorageClass storage_class;
            bool has_initializer;
            bool is_global;
            bool is_defined;
            Symbol *symbol;
            bool is_variadic;
        } func;
        // { [block_item]+ }
        struct {
            Array items_array;
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
            LiteralKind kind;
            const char *raw_rata;
            int len;
            union{
                int64_t i;
                double f;
                char c;
                struct {
                    const char *data;
                    int len;
                } s;
                void *ptr;
            };
        } literal;
        struct {
            const char *name;
            int len;
        } identifier;
        // type name = expr;
        struct {
            Node *type;
            Node *identifier;
            Node *expr;
            StorageClass storage_class;
            bool has_initializer;
            bool is_global;
            bool is_defined;
            Symbol *symbol;
        } var_decl;
        // identifier(params*)
        struct {
            Node *identifier;
            Array params_array;
        } func_call;
        // (int)0.5
        struct {
            Node *expr;
            Type *from;
            Type *to;
        } cast;
        // identifier[index]
        struct {
            Node *identifier;
            Node *index;
        } index;
        // break;
        struct{
            Node *loop;
        }_break;
        // continue;
        struct{
            Node *loop;
        }_continue;
        // { [element]+ }
        struct{
            Array elements_array;
        }init_list;
        // a->b, a.b
        struct{
            TokenType op;
            Node *identifier;
            Node *member;
            int offset;
        }member_access;
        // switch(cond) {}
        struct{
            Node *test;
            Node *block;
            Array cases_array;
        }_switch;
        // case test:
        struct{
            Node *test;
            int i;
        }_case;
        // typedef Type Symbol
        struct{
            Node *symbol;
        }_typedef;
    };
};

typedef struct {
    int count;
    int capacity;
    Node *nodes;
} NodeManager;

#define NODE_ARENA_SIZE 1024

void init_types();

Type *init_global_type(TypeKind type, int size);
NodeManager new_node_manager();
void free_node_manager(const NodeManager *nm);

/*
    Handles creating a Node, pushing it to the global node array
*/
Node *new_node(NodeManager *nm, NodeKind kind);

Node *init_translation_unit(NodeManager *nm);
Node *new_compound_node(NodeManager *nm);
Node *new_init_list_node(NodeManager *nm);
Node *new_function_node(NodeManager *nm);
Node *new_function_call_node(NodeManager *nm, Node *identifier);

Node *cast_node_unchecked(NodeManager *nm, Node *node, Type *type);
Node *cast_node(NodeManager *nm, Node *node, Type *type);
bool is_valid_cast(const Type *from, const Type *to);


LiteralKind literal_kind(TokenType type);
void print_node_type(NodeKind type);

void print_indent(int depth);

void print_node(const Node *node,int depth);

/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm);

static inline Node *get_node(const Array *node_array, int index) { return *(Node **)get(node_array, index); }
static inline void set_node(const Array *node_array, Node** node, int index) {
    memcpy((char*)node_array->data + index * node_array->element_size, node, sizeof(Node *));
}
#endif // COMPILER_C_NODE_H
