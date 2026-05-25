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
    N_TERNARY,
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
    N_GOTO,
    N_LABEL,
    N_COMPOUND_LITERAL,
    N_DESIGNATOR,
    N_BUILTIN,
    N_NULL,
} NodeKind;

#define BUILTIN_COUNT 4

typedef enum {
    BUILTIN_MEMCPY,
    BUILTIN_VA_START,
    BUILTIN_VA_ARG,
    BUILTIN_VA_END,
    BUILTIN_NONE,
} BuiltinKind;

extern const char *builtin_names[BUILTIN_COUNT];

typedef enum {
    L_INT,
    L_FLOAT,
    L_CHAR,
    L_STRING,
} LiteralKind;

typedef enum { NONE, EXTERN, STATIC } StorageClass;

typedef struct Node Node;
typedef struct Symbol Symbol;

struct Node {
    NodeKind kind;
    Type *type;
    // The type of the values associated with this node
    union {
        struct {
            Array declarations_array;
        } translation_unit;
        // type name(params) [body]
        struct {
            // Node *type;
            const char *name;
            Node *body;
            StorageClass storage_class;
            Symbol *symbol;
            bool is_global;
            bool is_defined;
            bool is_inline;
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
            Type *common_type;
        } binary;
        // cond ? true : false
        struct {
            Node *cond;
            Node *if_true;
            Node *if_false;
        } ternary;
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
            bool is_do_while;
        } _while;
        // for (init; cond; end) {block}
        struct {
            Node *init;
            Node *cond;
            Node *iter;
            Node *block;
        } _for;
        // 'a' "abc" 0.1512 123
        struct {
            LiteralKind kind;
            const char *raw_rata;
            int len;
            union {
                int64_t i;
                double f;
                char c;
                struct {
                    char *data;
                    int len;
                } s;
                void *ptr;
            };
            Symbol *symbol;
        } literal;
        struct {
            Node *value;
        } compound_literal;
        struct {
            const char *name;
            int len;
            Symbol *symbol;
        } identifier;
        // type name = expr;
        struct {
            Node *identifier;
            Node *expr;
            ConstLiteral *const_expr;
            StorageClass storage_class;
            bool is_global;
            bool is_defined;
            Symbol *symbol;
        } var_decl;
        // identifier(params*)
        struct {
            Node *callee;
            Array params_array;
            Symbol *symbol;
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
        struct {
            Node *loop;
        } _break;
        // continue;
        struct {
            Node *loop;
        } _continue;
        // { [element]+ }
        struct {
            Array elements_array;
        } init_list;
        // a->b, a.b
        struct {
            TokenType op;
            Node *identifier;
            Node *member;
            int offset;
        } member_access;
        // switch(cond) {}
        struct {
            Node *test;
            Node *block;
            Array cases_array;
        } _switch;
        // case test:
        struct {
            Node *const_expr;
            int64_t test;
            int i;
        } _case;
        // typedef Type Symbol
        struct {
            const char *name;
        } _typedef;
        struct {
            Node *identifier;
        } _goto;
        struct {
            Node *identifier;
        } label;
        struct {
            TypeKind kind;
            union {
                struct {
                    bool is_complete;
                    union {
                        unsigned int index;
                        Node *const_expr;
                    };
                } _array;
                struct {
                    const char *name;
                    StructMember *member;
                } _struct;
                struct {
                    const char *name;
                    UnionMember *member;
                } _union;
            };
            Node *value;
        } designated_init;
        struct {
            BuiltinKind kind;
            Array params;
        } _builtin;
    };
};

typedef Arena NodeManager;

#define NODE_ARENA_SIZE 1024

NodeManager new_node_manager();
void free_node_manager(NodeManager *nm);

BuiltinKind get_builtin_kind(const char *name);

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

void print_node(const Node *node, int depth);

/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm);

void free_node(Node *node);

static inline Node *get_node(const Array *node_array, int index) { return *(Node **)get(node_array, index); }
static inline void set_node(Array *node_array, Node **node, int index) {
    memcpy((char *)node_array->data + index * node_array->element_size, node, sizeof(Node *));
}
static inline Node *insert_node(Array *node_array, Node **node, int index) { return (Node *)insert(node_array, node, index); }
#endif // COMPILER_C_NODE_H
