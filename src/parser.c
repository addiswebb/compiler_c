
#include "compiler_c/tokenizer.h"
#include <compiler_c/node.h>
#include <compiler_c/parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STATEMENTS_PER_BLOCK 8

Parser new_parser() {
    Parser parser;
    parser.size = 0;
    parser.index = 0;
    parser.src = NULL;
    parser.func_def_capacity = 0;
    parser.func_def_count = 0;
    parser.func_defs = NULL;
    parser.var_decl_count = 0;
    parser.var_decl_capacity = 0;
    parser.var_decls = NULL;
    return parser;
}

void init_parser(Parser *p, TokenArray *src, const int size) {
    init_types();
    p->size = size;
    p->src = src;
    p->index = 0;
    p->expect_semi = true;
    p->func_def_capacity = 4;
    p->func_def_count = 0;
    p->func_defs = malloc(sizeof(P_Func_Def) * p->func_def_capacity);
    if (!p->func_defs) {
        printf("Failed to allocate for func_defs\n");
        exit(1);
    }
    p->var_decl_capacity = 4;
    p->var_decl_count = 0;
    p->var_decls = malloc(sizeof(P_Var_Decl) * p->var_decl_capacity);
    if (!p->var_decls) {
        printf("Failed to allocate for var_decls\n");
        exit(1);
    }
}

/*
Is End of token array?
*/
bool p_is_last_token(const Parser *p) { return p->index >= p->size; }

Token *p_peek_n(const Parser *p, const int n) {
    if (p->index + n > p->src->size) {
        printf("P_peek_n Tried peeking past eota\n");
        return NULL;
    }
    return &p->src->data[p->index + n];
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

Token *p_consume_semi(Parser *p) {
    if (p->expect_semi) {
        return p_consume_a(p, TK_SEMI);
    }
    return NULL;
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
    node->compound.items = malloc(sizeof(*node->compound.items) * DEFAULT_STATEMENTS_PER_BLOCK);
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
Node *p_parse_primary_expression(Parser *p, NodeManager *nm) {
    Node *node = NULL;

    if (is_unary_operator(p_peek(p)->type)) {
        node = new_node(nm, N_UNARY);
        node->unary.op = p_consume(p)->type;
        node->unary.associativity = RIGHT_ASSOCIATIVITY;
        node->unary.expr = p_parse_primary_expression(p, nm);
        node->type = type_invalid;
        return node;
    }
    switch (p_peek(p)->type) {
    case TK_INT_LITERAL:
        node = new_node(nm, N_LITERAL);
        node->type = type_int;
        node->literal.i = atoi(p_consume(p)->value);
        return node;
    case TK_FLT_LITERAL:
        node = new_node(nm, N_LITERAL);
        node->type = type_float;
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
        printf("Expected term got ");
        print_token_type(p_peek(p)->type);
        printf("\n");
        exit(1);
    }
}

Node *new_function_node(NodeManager *nm) {
    Node *node = new_node(nm, N_FUNCTION);
    node->func.body = NULL;
    node->func.param_count = 0;
    node->func.param_capacity = 4;
    node->func.params = malloc(sizeof(Node) * node->func.param_capacity);
    if (!node->func.params) {
        printf("Failed to create new function node\n");
        exit(1);
    }
    return node;
}
Node *new_function_call_node(NodeManager *nm, Node *identifier, int param_count) {
    Node *node = new_node(nm, N_FUNCTION_CALL);
    node->func_call.identifier = identifier;
    node->func_call.param_capacity = param_count;
    node->func_call.param_count = 0;
    node->func_call.params = malloc(sizeof(Node) * param_count);
    if (!node->func_call.params) {
        printf("Failed to create new function call node\n");
        exit(1);
    }
    return node;
}

Node *p_parse_expression(Parser *p, NodeManager *nm, const int min_prec) {
    Node *primary = p_parse_primary_expression(p, nm);
    if (p_peek(p)->type == TK_INCR || p_peek(p)->type == TK_DECR) {
        Node *node = new_node(nm, N_UNARY);
        node->unary.op = p_consume(p)->type;
        node->unary.associativity = LEFT_ASSOCIATIVITY;
        node->unary.expr = primary;
        primary = node;
    }
    if (p_peek(p)->type == TK_OPEN_PAREN) {
        p_consume(p); // '('
        if (primary->kind != N_IDENTIFIER) {
            print_node_type(primary->kind);
            printf(" is not a function\n");
            exit(1);
        }
        Node *func_def = p_get_func_def(p, primary->identifier.name);
        Node *func_call = new_function_call_node(nm, primary, func_def->func.param_count);

        for (int i = 0; i < func_def->func.param_count; i++) {
            p_add_call_param(func_call, p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE));
            if (i != func_def->func.param_count - 1) {
                p_consume_a(p, TK_COMMA);
            }
        }
        p_consume_a(p, TK_CLOSE_PAREN);
        primary = func_call;
    }

    while (is_binary_operator(p_peek(p)->type) && !p_is_last_token(p) && precedence(p_peek(p)->type) >= min_prec) {
        const int prec = precedence(p_peek(p)->type);
        const int assoc = associativity(p_peek(p)->type);
        Node *binary = new_node(nm, N_BINARY);
        binary->binary.op = p_consume(p)->type;
        binary->binary.rhs = p_parse_expression(p, nm, prec + assoc);
        binary->binary.lhs = primary;
        primary = binary;
    }
    return primary;
}

/*
    Consumes either,
    var decl
    or
    statement
*/
Node *p_parse_block_item(Parser *p, NodeManager *nm) {
    switch (p_peek(p)->type) {
    case TK_INT:
    case TK_FLOAT:
        return p_parse_var_declaration(p, nm);
        break;
    default:
        return p_parse_statement(p, nm);
        break;
    }
}
/*
    Consumes
    `(type) identifier = [= expr]?;`
    Where [= expr] is optional
*/
Node *p_parse_var_declaration(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_VAR_DECL);
    node->type = token_to_type(p_consume(p)->type);
    node->var_decl.name = p_consume_a(p, TK_IDENTIFIER)->value;
    if (p_peek(p)->type == TK_EQ) {
        p_consume(p);
        node->var_decl.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    } else {
        node->var_decl.expr = NULL;
    }
    p_consume_semi(p);
    p_append_var_decl(p, node);
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
void p_append_block_item(Node *root, Node *item) {
    if (root->compound.count >= root->compound.capacity) {
        root->compound.capacity *= 2;
        root->compound.items = realloc(root->compound.items, sizeof(Node) * root->compound.capacity);
        if (!root->compound.items) {
            printf("Failed to append declaration");
            exit(1);
        }
    }
    if (item != NULL) {
        root->compound.items[root->compound.count++] = item;
    } else {
        printf("Skipping empty node\n");
    }
}

void p_append_param(Node *func, Node *param) {
    if (func->func.param_count >= func->func.param_capacity) {
        func->func.param_capacity *= 2;
        func->func.params = realloc(func->func.params, sizeof(Node *) * func->func.param_capacity);
        if (!func->func.params) {
            printf("Failed to append params");
            exit(1);
        }
    }
    if (param != NULL) {
        func->func.params[func->func.param_count++] = param;
    }
}

void p_add_call_param(Node *func, Node *param) {
    if (func->func_call.param_count >= func->func_call.param_capacity) {
        printf("Tried adding too many call params to a function\n");
        exit(1);
    }
    if (param != NULL) {
        func->func_call.params[func->func_call.param_count++] = param;
    }
}

void p_append_func_def(Parser *p, Node *func) {
    if (p->func_def_count >= p->func_def_capacity) {
        p->func_def_capacity *= 2;
        p->func_defs = realloc(p->func_defs, sizeof(P_Func_Def) * p->func_def_capacity);
        if (!p->func_defs) {
            printf("Failed to realloc for func defs");
            exit(1);
        }
    }
    p->func_defs[p->func_def_count++] = (P_Func_Def){func->func.name, func->type, func};
}

Node *p_get_func_def(Parser *p, const char *name) {
    for (int i = 0; i < p->func_def_count; i++) {
        if (strcmp(p->func_defs[i].name, name) == 0) {
            return p->func_defs[i].def;
        }
    }
    printf("Tried to call %s which does not exist\n", name);
    exit(1);
}

void p_append_var_decl(Parser *p, Node *var) {
    if (p->var_decl_count >= p->var_decl_capacity) {
        p->var_decl_capacity *= 2;
        p->var_decls = realloc(p->var_decls, sizeof(P_Var_Decl) * p->var_decl_capacity);
        if (!p->var_decls) {
            printf("Failed to realloc for var decl\n");
            exit(1);
        }
    }
    p->var_decls[p->var_decl_count++] = (P_Var_Decl){var->identifier.name, var->type, var};
}
Node *p_get_var_decl(Parser *p, const char *name) {
    for (int i = 0; i < p->var_decl_count; i++) {
        if (strcmp(p->var_decls[i].name, name) == 0) {
            return p->var_decls[i].decl;
        }
    }
    printf("Tried to find variable %s which does not exist\n", name);
    exit(1);
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
            node->_if.if_false = p_parse_statement(p, nm);
        }
    } else {
        node->_if.if_false = NULL;
    }
    return node;
}

Node *p_parse_while_loop(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_WHILE);
    p_consume_a(p, TK_WHILE);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_while.cond = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_while.block = p_parse_statement(p, nm);
    return node;
}

Node *p_parse_for_loop(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_FOR);
    p_consume_a(p, TK_FOR);
    p_consume_a(p, TK_OPEN_PAREN);
    // Manually consume semi colons
    p->expect_semi = false;
    node->_for.init = p_parse_block_item(p, nm);
    p_consume_a(p, TK_SEMI);
    node->_for.cond = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_SEMI);
    node->_for.iter = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p->expect_semi = true;
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_for.block = p_parse_statement(p, nm);
    return node;
}

Node *p_parse_return(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_RETURN);
    p_consume(p); // -> return
    node->_return.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_semi(p);
    return node;
}

Node *p_parse_var_assign(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_BINARY);
    node->binary.lhs = p_parse_primary_expression(p, nm);
    node->binary.op = p_consume_a(p, TK_EQ)->type;
    node->binary.rhs = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_semi(p);
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
    case TK_IF:
        return p_parse_if_statement(p, nm);
    case TK_WHILE:
        return p_parse_while_loop(p, nm);
    case TK_FOR:
        return p_parse_for_loop(p, nm);
    case TK_RETURN:
        return p_parse_return(p, nm);
    case TK_IDENTIFIER:
        Node *n = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        p_consume_semi(p);
        return n;
    case TK_OPEN_CURLY:
        return p_parse_compound(p, nm);
    case TK_SEMI:
        printf("Null statement is currently unsupported ';'\n");
        exit(1);
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
        p_append_block_item(node, p_parse_block_item(p, nm));
    }
    p_consume_a(p, TK_CLOSE_CURLY);
    return node;
}

/*
    Consumes
    `(type) identifier ([var decl],*) {[statement]*}`

    () contains any amount of var declarations, including zero,
    and {} contains any amount of statements, including zero.
*/
Node *p_parse_function(Parser *p, NodeManager *nm) {
    Node *node = new_function_node(nm);
    node->type = token_to_type(p_consume(p)->type);
    node->func.name = p_consume(p)->value;

    p_consume_a(p, TK_OPEN_PAREN);
    while (p_peek(p)->type == TK_INT && p_peek(p)->type != TK_CLOSE_PAREN && !p_is_last_token(p)) {
        Node *param = new_node(nm, N_VAR_DECL);
        node->type = token_to_type(p_consume_a(p, TK_INT)->type);
        param->var_decl.name = p_consume_a(p, TK_IDENTIFIER)->value;
        param->var_decl.expr = NULL;
        p_append_param(node, param);
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_PAREN);
    node->func.body = p_parse_compound(p, nm);
    p_append_func_def(p, node);
    return node;
}

bool is_function_ahead(Parser *p) {
    return (p_peek(p)->type == TK_INT || p_peek(p)->type == TK_FLOAT) && p_peek_n(p, 1)->type == TK_IDENTIFIER &&
           p_peek_n(p, 2)->type == TK_OPEN_PAREN;
}

Node *p_parse_declaration(Parser *p, NodeManager *nm) {
    if (is_function_ahead(p)) {
        return p_parse_function(p, nm);
    }
    return p_parse_var_declaration(p, nm);
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
// Is this node assignable?
bool is_lvalue(Node *n) { return n->kind == N_IDENTIFIER; }

Type *token_to_type(TokenType t) {
    switch (t) {
    case TK_INT:
        return type_int;
    case TK_FLOAT:
        return type_float;
    default:
        printf("Tried to convert a token which is not a valid value type\n");
        exit(1);
    }
}

Type *check_unary_op(Node *unaryop) {
    Node *expr = unaryop->unary.expr;
    TypeKind kind = expr->type->kind;
    switch (unaryop->unary.op) {
    // [Int, Float] => [Int, Float]
    case TK_PLUS:
    case TK_MINUS:
        if (kind == T_INT || kind == T_FLOAT) return expr->type;
        break;
    // [Int, Float] => Int
    case TK_L_NOT:
        if (kind == T_INT || kind == T_FLOAT) return type_int;
        break;
    // Int => Int
    case TK_AND:
    case TK_SHL:
    case TK_SHR:
    case TK_BW_NOT:
        if (kind == T_INT) return expr->type;
        break;
    default:
        break;
    }
    printf("Invalid operand type for the given unary operator\n");
    return type_invalid;
}

Type *check_binary_op(NodeManager *nm, TokenType op, Node *binop) {
    if (binop->binary.lhs->type == T_INVALID || binop->binary.rhs->type == T_INVALID) {
        printf("Semantic Analysis: Binary op was given expression with an invalid type\n");
        exit(1);
    }
    Node *lhs = binop->binary.lhs;
    Node *rhs = binop->binary.rhs;
    if (is_assignment_op(op)) {
        if (!is_lvalue(lhs)) {
            printf("Semantic Analysis: Binary op lhs is not assignable\n");
            exit(1);
        }
        TokenType underlying = get_underlying_op(op);
        if (is_arithmetic_op(underlying) || is_bitwise_op(underlying)) {
            Type *common = promote_binary_operands(nm, binop);
        }

        if (lhs->type->kind != rhs->type->kind) {
            binop->binary.rhs = cast_node(nm, rhs, lhs->type);
        }
        return lhs->type;
    }

    Type *common = promote_binary_operands(nm, binop);
    if (!common) {
        printf("Invalid arithmetic operands");
        exit(1);
    }

    if (is_arithmetic_op(op)) return common;
    if (is_comparison_op(op)) return type_int;

    if (is_bitwise_op(op)) {
        if (lhs->type != type_int || rhs->type != type_int) {
            printf("Bitwise operation requires integers\n");
            exit(1);
        }
        return type_int;
    }

    if (is_logical_op(op)) return type_int;

    printf("Unknown binary operator\n");
    return type_invalid;
}

Type *promote_binary_operands(NodeManager *nm, Node *binop) {
    Type *common;
    Node *lhs = binop->binary.lhs;
    Node *rhs = binop->binary.rhs;
    if (lhs->type == rhs->type) return lhs->type;
    if (lhs->type->kind == T_FLOAT || rhs->type->kind == T_FLOAT) {
        common = type_float;
    } else {
        common = type_int;
    }

    if (lhs->type != common) binop->binary.lhs = cast_node(nm, lhs, common);
    if (rhs->type != common) binop->binary.rhs = cast_node(nm, rhs, common);
    return common;
}
void semantic_analysis(Parser *p, NodeManager *nm, Node *node) {
    if (!node) return;
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        for (int i = 0; i < node->translation_unit.count; i++) {
            semantic_analysis(p, nm, node->translation_unit.declarations[i]);
        }
        break;
    case N_FUNCTION:
        semantic_analysis(p, nm, node->func.body);
        break;
    case N_COMPOUND:
        for (int i = 0; i < node->compound.count; i++) {
            semantic_analysis(p, nm, node->compound.items[i]);
        }
        break;
    case N_VAR_DECL:
        semantic_analysis(p, nm, node->var_decl.expr);
        if (node->var_decl.expr->type != node->type) {
            cast_node(nm, node, node->type);
        }
        break;
    case N_UNARY:
        semantic_analysis(p, nm, node->unary.expr);
        node->type = check_unary_op(node);
        break;
    case N_BINARY:
        semantic_analysis(p, nm, node->binary.lhs);
        semantic_analysis(p, nm, node->binary.rhs);
        node->type = check_binary_op(nm, node->binary.op, node);
        break;
    case N_CAST:
        semantic_analysis(p, nm, node->cast.expr);
        if (is_valid_cast(node->cast.from, node->cast.to)) {
            node->type = node->cast.to;
            break;
        } else {
            printf("Invalid cast between");
            print_type(node->cast.from);
            printf(" and ");
            print_type(node->cast.to);
            printf("\n");
            exit(1);
        }
    case N_FUNCTION_CALL:
        node->type = p_get_func_def(p, node->func_call.identifier->identifier.name)->type;
        break;
    case N_IDENTIFIER:
        node->type = p_get_var_decl(p, node->identifier.name)->type;
        break;
    case N_IF:
        semantic_analysis(p, nm, node->_if.cond);
        if (node->_if.cond->type != type_int) {
            node->_if.cond = cast_node(nm, node->_if.cond, type_int);
        }
        semantic_analysis(p, nm, node->_if.if_true);
        semantic_analysis(p, nm, node->_if.if_false);
        break;
    case N_WHILE:
        semantic_analysis(p, nm, node->_while.cond);
        if (node->_while.cond->type != type_int) {
            node->_while.cond = cast_node(nm, node->_while.cond, type_int);
        }
        semantic_analysis(p, nm, node->_while.block);
        break;
    case N_FOR:
        semantic_analysis(p, nm, node->_for.init);
        semantic_analysis(p, nm, node->_for.cond);
        if (node->_for.cond->type != type_int) {
            node->_for.cond = cast_node(nm, node->_for.cond, type_int);
        }
        semantic_analysis(p, nm, node->_for.iter);
        semantic_analysis(p, nm, node->_for.block);
        break;
    case N_RETURN:
        semantic_analysis(p, nm, node->_return.expr);
        if (node->_return.expr->type != type_int) {
            node->_return.expr = cast_node(nm, node->_return.expr, type_int);
        }
        break;
    case N_LITERAL:
        // Trust the tokenizer
        break;
    }
}
