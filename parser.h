#ifndef COMPILER_C_PARSER_H
#define COMPILER_C_PARSER_H

#include "node.h"
#include "tokenizer.h"

#include <stdbool.h>

#define DEFAULT_STATEMENTS_PER_BLOCK 8

typedef struct {
    int index;
    int size;
    TokenArray *src;
} Parser;

Parser new_parser();
void init_parser(Parser *p,TokenArray* src, int size);

/*
Is End of token array?
*/
bool p_is_last_token(const Parser *p);

Token *p_peek_n(const Parser *p, int n);
Token *p_peek(Parser *p);
Token *p_peek_next(Parser *p);

Token *p_consume_n(Parser *p, int n);
Token *p_consume(Parser *p);

void p_skip_n(Parser *p, int n);
void p_skip(Parser *p);

/*
    Error on type mismatch
*/
void p_expect(Parser *p, TokenType expected_type);

Token *p_consume_a(Parser *p,TokenType type);
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

/*
    Consumes
    `literal`
    `identifier`
    `(expr)`
*/
Node *p_parse_term(Parser *p, NodeManager *nm);

/*
    Consumes
    `[term]+`
    Where `term` is any `literal`, `identifier` or `(expr)`
*/
Node *p_parse_expression(Parser *p,NodeManager *nm,int min_prec);

/*
    Consumes
    `(type) identifier = [= expr]?;`
    Where [= expr] is optional
*/
Node *p_parse_var_declaration(Parser *p, NodeManager *nm);

/*
    Appends a declaration to the given translation unit,
    Resizes its declaration array if necessary.
*/
void p_append_declaration(Node *root, Node *decl);

/*
    Appends a statement to the given compound node,
    Resizes its statement array if necessary.
*/
void p_append_statement(Node *root, Node *stmt);

/*
    Consumes
    `if ([cond]) {[compound]} [else [if statement]? {[compound]}]? ;
*/
Node *p_parse_if_statement(Parser *p, NodeManager *nm);

/*
    Consumes
    while ([cond]) {[compound]}
*/
Node *p_parse_while_statement(Parser *p, NodeManager *nm);

/*
    Consumes
    `return [expr]?;
    Where [expr] is optional.
*/
Node *p_parse_return(Parser*p, NodeManager *nm);

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
Node *p_parse_function(Parser *p, NodeManager *nm);

bool is_function_ahead(Parser *p);

Node *p_parse_declaration(Parser *p, NodeManager *nm);

Node *p_parse_translation_unit(Parser* p, NodeManager *nm);

#endif // COMPILER_C_PARSER_H
