#ifndef COMPILER_C_PARSER_H
#define COMPILER_C_PARSER_H

#include "compiler_c/node.h"
#include "compiler_c/tokenizer.h"
#include "compiler_c/type.h"
#include <stdbool.h>

#define DEBUG_CONSUME 1
#define DEFAULT_STATEMENTS_PER_BLOCK 8

typedef struct{
    const char *new_def;
    Type *type;
}Typedef;

typedef enum {
    ENUM,
    VAR,
    FUNC,
    TYPEDEF,
    ANY,
} SymbolKind;

typedef enum{
    LINK_NONE,
    LINK_INTERNAL,
    LINK_EXTERNAL
}Linkage;

typedef enum{
    STORAGE_NONE,
    STORAGE_DATA,
    STORAGE_BSS,
    STORAGE_TEXT,
}Storage;

typedef struct Symbol{
    const char *name;
    SymbolKind kind;
    Linkage linkage;
    Storage storage;
    int scope_depth;
    union{
        Node *var_decl;
        Node *func_def;
        EnumField enum_field;
        Typedef _typedef;
    };
}Symbol;

typedef struct{
    int count;
    int capacity;
    Symbol *symbols;
}SymbolTable;

typedef struct {
    int index;
    int size;
    TokenArray *src;
    bool expect_semi;
    int scope_stack_count;
    int scope_stack_capacity;
    SymbolTable *scope_stack;
} Parser;

Parser new_parser();
void init_parser(Parser *p,TokenArray* src, int size);
void p_append_symbol_table(Parser *p);

/*
Is End of token array?
*/
bool p_is_last_token(const Parser *p);

Symbol *p_append_symbol(SymbolTable *st, const Symbol *s);
Token *p_peek_n(const Parser *p, int n);
Token *p_peek(const Parser *p);
Token *p_peek_next(const Parser *p);

Token *p_consume_n(Parser *p, int n);
Token *p_consume(Parser *p);

void p_skip_n(Parser *p, int n);
void p_skip(Parser *p);

/*
    Error on type mismatch
*/
void p_expect(const Parser *p, TokenType expected_type);

Token *p_consume_a(Parser *p,TokenType type);
Token *p_consume_semi(Parser *p);
/*
    Creates the root translation unit node
    And allocates an array for its declarations
*/
Node *init_translation_unit(NodeManager *nm);

/*
    Creates a new compound node
    And allocates an array for its statements
*/
Node *new_compound_node(NodeManager *nm);
Node *new_init_list_node(NodeManager *nm);
Node *new_function_node(NodeManager *nm);
Node *new_function_call_node(NodeManager *nm, Node *identifier);

/*
    Consumes
    `literal`
    `identifier`
    `(expr)`
*/
Node *p_parse_primary_expression(Parser *p, NodeManager *nm);

Node *p_parse_init_list(Parser *p, NodeManager *nm);
/*
    Consumes
    `[term]+`
    Where `term` is any `literal`, `identifier` or `(expr)`
*/
Node *p_parse_expression(Parser *p,NodeManager *nm,int min_prec);

Node *p_parse_block_item(Parser *p, NodeManager *nm);

/*
    Appends a declaration to the given translation unit,
    Resizes its declaration array if necessary.
*/
void p_append_declaration(Node *root, Node *decl);

void p_append_param(Node *func, Node *param);
void p_add_call_param(Node *func, Node *param);

Symbol *p_get_symbol(const Parser *p, const char *name, SymbolKind kind);

SymbolTable *current_symbol_table(Parser *p);
void p_append_typedef(Parser *p, const Typedef *t);
Symbol * p_append_func_def(Parser *p, Node *f);
Node *p_get_func_def(const Parser *p, const char* name);
Typedef *p_get_typedef(const Parser *p, const char *name);

void p_append_element(Node *init_list, Node *element);

Symbol *p_append_var_decl(Parser *p, Node *v);
Node *p_get_var_decl(const Parser *p, const char* name);

EnumField *p_get_enum_const(const Parser *p, const char* name);
void p_append_enum_const(Parser *p, const EnumField *e);
/*
    Appends a block item to the given compound node,
    Resizes its statement array if necessary.
*/
void p_append_block_item(Node *root, Node *item);

/*
    Consumes
    `if ([cond]) {[compound]} [else [if statement]? {[compound]}]? ;
*/
Node *p_parse_if_statement(Parser *p, NodeManager *nm);

/*
    Consumes
    `while ([cond]) {[compound]}`
*/
Node *p_parse_while_loop(Parser *p, NodeManager *nm);

void p_append_case(Node *s, Node *c);

/*
    Consumes
    `switch ([cond]) {[case LABEL:/statement]}
*/
Node *p_parse_switch_statement(Parser *p, NodeManager *nm);

Node *p_parse_case(Parser *p, NodeManager *nm);
/*
    Consumes
    `for ([init];[cond]; [end]) {[compound]}`
*/
Node *p_parse_for_loop(Parser *p, NodeManager *nm);
/*
    Consumes
    `return [expr]?;
    Where [expr] is optional.
*/
Node *p_parse_return(Parser*p, NodeManager *nm);
Node *p_parse_continue(Parser*p, NodeManager *nm);
Node *p_parse_break(Parser*p, NodeManager *nm);

Node *p_parse_var_assign(Parser *p, NodeManager *nm);

/*
    Consumes any of,
    `(type) identifier = [= expr]?;`
    `[if statement]`
    `return [expr]?`
    `[expr];`

    Never consumes `;`, other functions must consume it.
*/
Node *p_parse_statement(Parser *p, NodeManager *nm);
void p_push_scope(Parser *p);
void p_pop_scope(Parser *p);

/*
    Consumes
    `{[statement]*}`
    Where any amount of statements is allowed including zero.
*/
Node *p_parse_compound(Parser *p, NodeManager *nm);

/*
    Consumes
    `(type) identifier ([var decl]*) {[statement]*}`

    () contains any amount of var declarations, including zero,
    and {} contains any amount of statements, including zero.
*/
Node *p_parse_function(Parser *p, NodeManager *nm, Node *type, StorageClass storage_class);

Node *p_parse_external_declaration(Parser *p, NodeManager *nm);
Node *p_parse_block_declaration(Parser *p, NodeManager *nm);
Node *p_parse_decl_identifier(Parser *p, NodeManager *nm);
Node *p_parse_declaration(Parser *p, NodeManager *nm, Node *type_decl, StorageClass storage_class, bool global);
Node *p_parse_typedef(Parser *p, NodeManager *nm);

Node *p_parse_translation_unit(Parser* p, NodeManager *nm);
Type *p_parse_type(Parser *p, NodeManager *nm);
Type *p_parse_enum(Parser *p, NodeManager *nm);
Type *p_parse_struct(Parser *p, NodeManager *nm);

Node *current_func_definition(const Parser *p);

bool is_type_token(const Parser *p, const Token *t);
Type *token_to_type(const Parser *p, const Token *t);
#endif // COMPILER_C_PARSER_H
