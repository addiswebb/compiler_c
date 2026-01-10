#include "parser.h"

#include "node.h"

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_STATEMENTS_PER_BLOCK 8

Parser new_parser() {
    Parser parser;
    parser.size = 0;
    parser.index = 0;
    parser.src = NULL;
    return parser;
}

void init_parser(Parser *p, TokenArray *src, const int size) {
    p->size = size;
    p->src = src;
    p->index = 0;
}

/*
Is End of token array?
*/
bool p_is_last_token(const Parser *p) { return p->index >= p->size; }

Token *p_peek_n(const Parser *p, const int n) {
    if (p->index + n > p->src->size) {
        printf("P_peek_n Tried peeking past eota\n");
        return NULL;
    } else {
        return &p->src->data[p->index + n];
    }
}
Token *p_peek(Parser *p) { return p_peek_n(p, 0); }
Token *p_peek_next(Parser *p) { return p_peek_n(p, 1); }
Token *p_consume_n(Parser *p, const int n) {
    if (p->index + n > p->src->size) {
        printf("P_consume_n %d Reached the end of the token list %d/%d\n", n, p->index, p->src->size);
        return NULL;
    }
    Token *token = &p->src->data[p->index];
    p->index += n;
    return token;
}

Token *p_consume(Parser *p) { return p_consume_n(p, 1); }

void p_skip_n(Parser *p, const int n) { p_consume_n(p, n); }
void p_skip(Parser *p) { p_consume_n(p, 1); }

/*
    Error on type mismatch
*/
void p_expect(Parser *p, const TokenType expected_type) {
    if (!p_is_last_token(p)) {
        const TokenType token_type = p->src->data[p->index].type;
        if (token_type != expected_type) {
            printf("Expected ");
            print_token_type(expected_type);
            printf(" got ");
            print_token_type(token_type);
            printf("\n");
            exit(1);
        }
    }
}

Token *p_consume_a(Parser *p, const TokenType type) {
    p_expect(p, type);
    return p_consume(p);
}
/*
    Creates the root translation unit node
    And allocates an array for its declarations
*/
Node *init_translation_unit(NodeManager *nm) {
    Node *node = new_node(nm, N_TRANSLATION_UNIT);
    node->translation_unit.declarations = malloc(sizeof(*node->translation_unit.declarations) * DEFAULT_STATEMENTS_PER_BLOCK);
    if (node->translation_unit.declarations == NULL) {
        printf("Failed to initialize translation unit");
        exit(1);
    }
    node->translation_unit.capacity = DEFAULT_STATEMENTS_PER_BLOCK;
    node->translation_unit.count = 0;
    return node;
}

/*
    Creates a new compound node
    And allocates an array for its statements
*/
Node *new_compound_node(NodeManager *nm) {
    Node *node = new_node(nm, N_COMPOUND);
    node->compound.statements = malloc(sizeof(*node->compound.statements) * DEFAULT_STATEMENTS_PER_BLOCK);
    if (node->translation_unit.declarations == NULL) {
        printf("Failed to create new compound node");
        exit(1);
    }
    node->compound.capacity = DEFAULT_STATEMENTS_PER_BLOCK;
    node->compound.count = 0;
    return node;
}

/*
    Consumes
    `literal`
    `identifier`
    `(expr)`
*/
Node *p_parse_term(Parser *p, NodeManager *nm) {
    Node *node = NULL;
    switch (p_peek(p)->type) {
    case TK_INT_LITERAL:
        node = new_node(nm, N_LITERAL);
        node->literal.type = p_peek(p)->type;
        node->literal.i = atoi(p_consume(p)->value);
        return node;
    case TK_FLT_LITERAL:
        node = new_node(nm, N_LITERAL);
        node->literal.type = p_peek(p)->type;
        node->literal.f = atof(p_consume(p)->value);
        return node;
    case TK_IDENTIFIER:
        node = new_node(nm, N_IDENTIFIER);
        node->identifier.name = p_consume(p)->value;
        return node;
    case TK_OPEN_PAREN:
        p_consume_a(p, TK_OPEN_PAREN);
        node = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        p_consume_a(p, TK_CLOSE_PAREN);
        return node;
    default:
        printf("Expected expression got ");
        print_token_type(p_peek(p)->type);
        printf("\n");
        exit(1);
    }
}

/*
    Consumes
    `[term]+`
    Where `term` is any `literal`, `identifier` or `(expr)`
*/
Node *p_parse_expression(Parser *p, NodeManager *nm, const int min_prec) {
    Node *lhs = p_parse_term(p, nm);

    while (is_binary_operator(p_peek(p)->type) && !p_is_last_token(p) && precedence(p_peek(p)->type) >= min_prec) {
        const int prec = precedence(p_peek(p)->type);
        const int assoc = associativity(p_peek(p)->type);
        Node *binary = new_node(nm, N_BINARY);
        binary->binary.op = p_consume(p)->type;
        binary->binary.rhs = p_parse_expression(p, nm, prec + assoc);
        binary->binary.lhs = lhs;
        lhs = binary;
    }
    return lhs;
}

/*
    Consumes
    `(type) identifier = [= expr]?;`
    Where [= expr] is optional
*/
Node *p_parse_var_declaration(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_VAR_DECL);
    node->var_decl.type = p_consume(p)->type;
    p_expect(p, TK_IDENTIFIER);
    node->var_decl.name = p_consume(p)->value;
    if (p_peek(p)->type == TK_EQ) {
        p_consume(p);
        node->var_decl.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    } else {
        node->var_decl.expr = NULL;
    }
    p_consume_a(p, TK_SEMI);
    return node;
}

/*
    Appends a declaration to the given translation unit,
    Resizes its declaration array if necessary.
*/
void p_append_declaration(Node *root, Node *decl) {
    if (root->translation_unit.count >= root->translation_unit.capacity) {
        root->translation_unit.capacity *= 2;
        root->translation_unit.declarations = realloc(root->translation_unit.declarations, sizeof(Node) * root->translation_unit.capacity);
        if (root->translation_unit.declarations == NULL) {
            printf("Failed to append declaration");
            exit(1);
        }
    }
    root->translation_unit.declarations[root->translation_unit.count++] = decl;
}

/*
    Appends a statement to the given compound node,
    Resizes its statement array if necessary.
*/
void p_append_statement(Node *root, Node *stmt) {
    if (root->compound.count >= root->compound.capacity) {
        root->compound.capacity *= 2;
        root->compound.statements = realloc(root->compound.statements, sizeof(Node) * root->compound.capacity);
        if (root->compound.statements == NULL) {
            printf("Failed to append declaration");
            exit(1);
        }
    }
    if (stmt != NULL) {
        root->compound.statements[root->compound.count++] = stmt;
    } else {
        printf("Skipping empty node\n");
    }
}

/*
    Consumes
    `if ([cond]) {[compound]} [else [if statement]? {[compound]}]? ;
*/
Node *p_parse_if_statement(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_IF);
    p_consume_a(p, TK_IF); // -> if
    p_consume_a(p, TK_OPEN_PAREN);
    node->_if.cond = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_if.if_true = p_parse_compound(p, nm); //{[compound]} (in the future, can be a function call)
    if (p_peek(p)->type == TK_ELSE) {            // If there is an if, it can be a
        p_consume(p);                            // -> else
        if (p_peek(p)->type == TK_IF) {
            node->_if.if_false = p_parse_if_statement(p, nm);
        } else {
            node->_if.if_false = p_parse_compound(p, nm);
        }
    } else {
        node->_if.if_false = NULL;
    }
    return node;
}

/*
    Consumes
    while ([cond]) {[compound]}
*/
Node *p_parse_while_statement(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_WHILE);
    p_consume_a(p, TK_WHILE);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_while.cond = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_while.block = p_parse_compound(p, nm);
    return node;
}

/*
    Consumes
    `return [expr]?;
    Where [expr] is optional.
*/
Node *p_parse_return(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_RETURN);
    p_consume(p); // -> return
    node->_return.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_SEMI);
    return node;
}

Node *p_parse_var_assign(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_BINARY);
    node->binary.lhs = p_parse_term(p, nm);
    node->binary.op = p_consume_a(p, TK_EQ)->type;
    node->binary.rhs = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_SEMI);
    return node;
}

/*
    Consumes any of,
    `(type) identifier = [= expr]?;`
    `[if statement]`
    `return [expr]?`
    `[expr];`

    Never consumes `;`, other functions must consume it.
*/
Node *p_parse_statement(Parser *p, NodeManager *nm) {
    switch (p_peek(p)->type) {
    case TK_INT:
    case TK_FLOAT:
        return p_parse_var_declaration(p, nm);
    case TK_IF:
        return p_parse_if_statement(p, nm);
    case TK_WHILE:
        return p_parse_while_statement(p, nm);
    case TK_RETURN:
        return p_parse_return(p, nm);
    case TK_IDENTIFIER:
        return p_parse_var_assign(p, nm);
    case TK_OPEN_CURLY:
        return p_parse_compound(p, nm);
    default:
        return p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    }
}

/*
    Consumes
    `{[statement]*}`
    Where any amount of statements is allowed including zero.
*/
Node *p_parse_compound(Parser *p, NodeManager *nm) {
    Node *node = new_compound_node(nm);
    p_consume_a(p, TK_OPEN_CURLY);
    while (p_peek(p)->type != TK_CLOSE_CURLY && !p_is_last_token(p)) {
        p_append_statement(node, p_parse_statement(p, nm));
    }
    p_consume_a(p, TK_CLOSE_CURLY);
    return node;
}
/*
    Consumes
    `(type) identifier ([var decl]*) {[statement]*}`

    () contains any amount of var declarations, including zero,
    and {} contains any amount of statements, including zero.
*/
Node *p_parse_function(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_FUNCTION);
    node->function.return_type = p_consume(p)->type;
    node->function.name = p_consume(p)->value;
    p_consume_a(p, TK_OPEN_PAREN);
    while (p_peek(p)->type != TK_CLOSE_PAREN && !p_is_last_token(p)) {
        // Skip all params for now...
        p_skip(p);
    }
    p_consume_a(p, TK_CLOSE_PAREN);
    node->function.param_count = 0;
    node->function.params = NULL;
    node->function.body = p_parse_compound(p, nm);
    return node;
}

bool is_function_ahead(Parser *p) {
    return ((p_peek(p)->type == TK_INT || p_peek(p)->type == TK_FLOAT) && p_peek_n(p, 1)->type == TK_IDENTIFIER &&
            p_peek_n(p, 2)->type == TK_OPEN_PAREN);
}

Node *p_parse_declaration(Parser *p, NodeManager *nm) {
    if (is_function_ahead(p)) {
        return p_parse_function(p, nm);
    } else {
        return p_parse_var_declaration(p, nm);
    }
}

Node *p_parse_translation_unit(Parser *p, NodeManager *nm) {
    Node *root = init_translation_unit(nm);
    if (p->size == 0) {
        printf("The token array is empty,\n Don't forget to initialize the parser after "
               "tokenization.");
        exit(1);
    }

    while (!p_is_last_token(p)) {
        p_append_declaration(root, p_parse_declaration(p, nm));
    }
    return root;
}
