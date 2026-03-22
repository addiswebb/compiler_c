#ifndef COMPILER_C_PARSER_H
#define COMPILER_C_PARSER_H

#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <stdbool.h>

#define DEBUG_CONSUME 0
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

typedef struct {
    int index;
    int size;
    Array *src;
    bool expect_semi;
    Array scopes_array;
    Arena symbols_arena;
} Parser;

Parser new_parser();
void init_parser(Parser *p, Array* src, int size);
void free_parser(Parser *p);

/* ===== Parsing Utility Functions ===== */
/*
    Is End of token array?
*/
bool p_is_last_token(const Parser *p);

Token *p_peek_n(const Parser *p, int n);
Token *p_peek(const Parser *p);
Token *p_peek_next(const Parser *p);

Token *p_consume_n(Parser *p, int n);
Token *p_consume(Parser *p);

void p_skip_n(Parser *p, int n);
void p_skip(Parser *p);

/*
    Errors on type mismatch with current token.
*/
void p_expect(const Parser *p, TokenType expected_type);

/*
    Consumes the current token expecting the given type.
    Errors on type mismatch.
*/
Token *p_consume_a(Parser *p,TokenType type);
/*
    Consumes a `;` and errors on any other token.
*/
Token *p_consume_semi(Parser *p);
/*
    Checks against C standard types, and typedef table.
*/
bool is_type_token(const Parser *p, const Token *t);
bool is_storage_classifier(const TokenType type);
bool is_qualifier_token(const TokenType type);
bool is_start_of_type(const Parser *p, const Token *t);
/*
    Converts token using C standard types, and typedef table.
*/
Type *token_to_type(Parser *p, const Token *t);


/*
    Pushes a new symbol table onto the stack, where local variables declarations will be recorded.
    When "popped", the variables will go "out of scope".
*/
void p_push_scope(Parser *p);
void p_pop_scope(Parser *p);

/* ===== Parsing Functions ===== */

/*
    Consumes any,
    `literal`
    `identifier`
    `[expr]`
*/
Node *p_parse_primary_expression(Parser *p, NodeManager *nm);

/*
    Consumes any
    `primary expr[expr]`
    `primary expr(param-list)`
    `primary [./->] (identifier)`
    `primary [++/--] `
*/
Node *p_parse_postfix_expression(Parser *p, NodeManager *nm);

/*
    Consumes any compiler builtin including appropriate paramters
*/
Node *p_parse_builtin(Parser *p, NodeManager *nm, BuiltinKind kind);
/*
    Tries to consume
    `prefix (expr)`
    Falls back to
    `(expr) postfix`
*/
Node *p_parse_prefix(Parser *p, NodeManager *nm);
/*
    Consumes
    `{ [expr]* }`.
    Where each `expr` is seperated by a `,`.
*/
Node *p_parse_init_list(Parser *p, NodeManager *nm);
/*
    Tries to consume, in order
    Cast: `(type)`
    Prefix: `op [expr]`
    Primary: `literal`, `identifier`, `(expr)` or `{init list}`
    Postfix: `[expr] op`
    Where `op` is any prefixed or postfixed unary op respectively.
*/
Node *p_parse_expression(Parser *p,NodeManager *nm,int min_prec);

/*
    Tries to consume
    `(type)`
    Otherwise parses `postfix expr`
*/
Node *p_parse_cast(Parser *p, NodeManager *nm);

/*
    Consumes
    `goto [label];`
    Where `label` is any valid unique identifier.
*/
Node *p_parse_goto_statement(Parser *p,NodeManager *nm);

/*
    Consumes
    `[label]:`
    Where `label` is any valid unique identifier.
*/
Node *p_parse_label(Parser *p,NodeManager *nm);

/*
    Consumes either,
    `var decl`.
    or
    `[statement]`.
*/
Node *p_parse_block_item(Parser *p, NodeManager *nm);
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
/*
    Consumes
    `do {[compound]} while ([cond])`
*/
Node *p_parse_do_while_loop(Parser *p, NodeManager *nm);


/*
    Consumes
    `switch ([cond]) {[[case]/[statement]]*}
*/
Node *p_parse_switch_statement(Parser *p, NodeManager *nm);

/*
    Consumes
    case [int literal]:
*/
Node *p_parse_case(Parser *p, NodeManager *nm);

/*
    Consumes
    `for ([init];[cond];[end]) {[compound]}`
*/
Node *p_parse_for_loop(Parser *p, NodeManager *nm);
/*
    Consumes
    `return [expr]?;
    Where [expr] is optional.
*/
Node *p_parse_return(Parser*p, NodeManager *nm);
/*
    Consumes
    `continue`
*/
Node *p_parse_continue(Parser*p, NodeManager *nm);

/*
    Consumes
    `break`
*/
Node *p_parse_break(Parser*p, NodeManager *nm);


/*
    Consumes any of,
    `(type) identifier = [= expr]?;`
    `[if statement]`
    `return [expr]?`
    `[expr];`

    Never consumes `;`, other functions must consume it. (Expect for N_IDENTIFIER... ;_;)
*/
Node *p_parse_statement(Parser *p, NodeManager *nm);

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
Node *p_parse_function(Parser *p, NodeManager *nm, Type *type,const char *name, const StorageClass storage_class, bool is_inline);

/*
    Consums
    `([var decl]*)`
    Where var decl cannot have a definition, and identifier is optional.
*/
Modifier p_parse_parameter_list(Parser *p);
/*
    Consumes any valid declaration.
    `Function, Struct, Enum, Global Variable`.
    Also consumes storage classifiers, `extern/static`
*/
Node *p_parse_external_declaration(Parser *p, NodeManager *nm);
/*
    Consumes any,
    `Function Prototype, Local Variable`
    Also consumes storage classifiers, `extern/static`
*/
Node *p_parse_block_declaration(Parser *p, NodeManager *nm);
/*
    Consumes
    `([identifier])`
    or
    `[identifier]`
    Where the parenthesis are optional.
*/
Node *p_parse_decl_identifier(Parser *p, NodeManager *nm);
/*
    Consumes either
    `struct or enum delcaration`
    or `{type} [[int literal]?] [= expr]?;
    Where the `{type}` has already been consumed.

    A declaration is any variable or type declaration.
*/
Node *p_parse_declaration(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, const bool global);
/*
    Consumes
    `typedef [type] [identifier];`
*/
Node *p_parse_typedef(Parser *p, NodeManager *nm);

/*
    Consumes the whole translation unit.
*/
Node *p_parse_translation_unit(Parser* p, NodeManager *nm);
/*
    Consumes either
    `struct or enum` or a primitive `[type]`.
*/
Type *p_parse_abstract_type(Parser *p);
Type *p_parse_type(Parser *p, const char **name);
Declarator p_parse_declarator(Parser *p);
/*
    Consumes
    `enum [Tag]? [{[Field Identifier]*}]?`
    Where `Field Identifier` is any `[Identifier]`
*/
Type *p_parse_enum(Parser *p);

/*
    Consumes
    `struct [Tag]? [{[Member Declaration]*}]?`
    Where `Member Declaration` is any `[var decl]`
*/
Type *p_parse_struct(Parser *p);

/*
    Consumes
    `union [Tag]? [{[Member Declaration]*}]?`
    Where `Member Declaration` is any `[var decl]`
*/
Type *p_parse_union(Parser *p);

/*
    Consumes either,
    `extern`
    `static`
    or
    Nothing,
    Returns the corresponding storageClass;
*/
StorageClass p_parse_storage_classifier(Parser *p, NodeManager *nm);

/* ===== Parse Number Literals ===== */

Type *parse_int_suffix(const char *raw, int *len);
Type *parse_float_suffix(const char *raw, int *len);
int64_t parse_int(const char *raw, int len);
int64_t parse_binary(const char *raw, int len);
int64_t parse_oct(const char *raw, int len);
int64_t parse_dec(const char *raw, int len);
int64_t parse_hex(const char *raw, int len);
int parse_multi_character(const char *raw, int len);
double parse_float(const char *raw, int len);

/* ===== Symbol Table Functions ===== */

Symbol *p_get_symbol(const Parser *p, const char *name, SymbolKind kind);
Node *p_get_func_def(const Parser *p, const char* name);
Typedef *p_get_typedef(const Parser *p, const char *name);
Node *p_get_var_decl(const Parser *p, const char* name);
EnumField *p_get_enum_const(const Parser *p, const char* name);

/* ===== Array Helper Functions ===== */

void p_append_call_param(Node *func_call, Node *param);
void p_append_param(Node *func, Node *param);
void p_append_enum_const(Parser *p, const EnumField *e);
Symbol *p_append_var_decl_symbol(Parser *p, Node *v);

Symbol *p_append_param_decl_symbol(Parser *p, ParamDecl *param);
void p_append_element(Node *init_list, Node *element);
void p_append_symbol_table(Parser *p);
Symbol *p_append_symbol(Array *st, const Symbol *s);
void p_append_typedef(Parser *p, const Typedef *t);
Symbol * p_append_func_def(Parser *p, Node *f);
void p_append_block_item(Node *root, Node *item);
void p_append_case(Node *s, Node *c);

static inline Array* get_symbol_table(const Parser *p, int index){ return (Array *) get(&p->scopes_array, index); }
static inline Array* get_current_symbol_table(Parser *p) { return get_symbol_table(p, p->scopes_array.count-1); }
static inline Symbol *get_symbol(Array *symbol_table, int index) { return *(Symbol **)get(symbol_table, index); }

static inline EnumField *get_enum_field(const Type *enum_t, int index) { return (EnumField*)get(&enum_t->_enum.fields_array, index); }
static inline StructMember *get_struct_member(const Type *struct_t, int index) { return (StructMember *)get(&struct_t->_struct.members_array, index); }
static inline UnionMember *get_union_member(const Type *union_t, int index) { return (UnionMember*)get(&union_t->_union.members_array, index); }

UnionMember *get_union_member_named(const Type *union_t, const char *name);
StructMember *get_struct_member_named(const Type *struct_t, const char *name, int *index);

#endif // COMPILER_C_PARSER_H
