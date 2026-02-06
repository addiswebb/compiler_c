#include "compiler_c/parse/parser.h"
#include "compiler_c/node.h"
#include "compiler_c/parse/parse_util.h"
#include "compiler_c/sema.h"
#include "compiler_c/tokenizer.h"
#include "compiler_c/type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STATEMENTS_PER_BLOCK 8

Parser new_parser() { return (Parser){0}; }

void init_parser(Parser *p, TokenArray *src, const int size) {
    p->size = size;
    p->src = src;
    p->index = 0;
    p->expect_semi = true;
    p->st.capacity = 4;
    p->st.count = 0;
    p->st.symbols = malloc(sizeof(Symbol) * p->st.capacity);
    if (!p->st.symbols) {
        printf("Failed to allocate for symbol table");
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
    if (DEBUG_CONSUME) {
        print_token(token);
    }
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
    node->translation_unit.declarations = malloc(sizeof(Node **) * DEFAULT_STATEMENTS_PER_BLOCK);
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
    node->compound.items = malloc(sizeof(Node **) * DEFAULT_STATEMENTS_PER_BLOCK);
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
    if (p_peek(p)->type == TK_OPEN_CURLY) return p_parse_init_list(p, nm);
    Node *primary = NULL;
    Token *tk;
    if (is_unary_operator(p_peek(p)->type)) {
        primary = new_node(nm, N_UNARY);
        primary->unary.op = p_consume(p)->type;
        primary->unary.associativity = RIGHT_ASSOCIATIVITY;
        primary->unary.expr = p_parse_primary_expression(p, nm);
        return primary;
    }

    switch (p_peek(p)->type) {
    case TK_INT_LITERAL:
    case TK_FLT_LITERAL:
    case TK_CHAR_LITERAL:
    case TK_STRING_LITERAL:
        primary = new_node(nm, N_LITERAL);
        tk = p_consume(p);
        primary->literal.kind = literal_kind(tk->type);
        primary->literal.raw_rata = tk->value;
        primary->literal.len = tk->size;
        return primary;
    case TK_IDENTIFIER:
        primary = new_node(nm, N_IDENTIFIER);
        tk = p_consume(p);
        primary->identifier.name = tk->value;
        primary->identifier.len = tk->size;
        if (p_peek(p)->type == TK_DOT || p_peek(p)->type == TK_ARROW) {
            Node *member_access = new_node(nm, N_MEMBER_ACCESS);
            member_access->member_access.op = p_consume(p)->type;
            member_access->member_access.identifier = primary;
            member_access->member_access.member = p_parse_primary_expression(p, nm);
            if (!(member_access->member_access.member->kind == N_IDENTIFIER ||
                  member_access->member_access.member->kind == N_MEMBER_ACCESS)) {
                printf("Expected member identifier got ");
                print_node(member_access->member_access.member, 0);
                printf("\n");
                exit(1);
            }
            primary = member_access;
        }
        break;
    case TK_OPEN_PAREN:
        p_consume_a(p, TK_OPEN_PAREN);
        if (is_type_token(p_peek(p)->type)) {
            Node *type_node = new_node(nm, N_TYPE);
            type_node->type = p_parse_type(p, nm);
            p_consume_a(p, TK_CLOSE_PAREN);
            if (is_unary_operator(p_peek(p)->type) || is_binary_operator(p_peek(p)->type) || p_peek(p)->type == TK_SEMI ||
                p_peek(p)->type == TK_CLOSE_PAREN) {
                return type_node;
            }
            primary = p_parse_primary_expression(p, nm);
            primary = cast_node_unchecked(nm, primary, type_node->type);
        } else {
            primary = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
            p_consume_a(p, TK_CLOSE_PAREN);
        }
        return primary;
    default:
        printf("Expected term got ");
        print_token_type(p_peek(p)->type);
        printf("\n");
        exit(1);
    }

    if (p_peek(p)->type == TK_OPEN_SQUARE) {
        p_consume(p); // '['
        if (!is_lvalue(primary)) {
            print_node_type(primary->kind);
            printf(" is not a an ltype, needed for indexing\n");
            exit(1);
        }
        Node *node = new_node(nm, N_INDEX);
        node->index.index = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        node->index.identifier = primary;
        primary = node;
        p_consume_a(p, TK_CLOSE_SQUARE);
    } else if (p_peek(p)->type == TK_OPEN_PAREN) {
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
    return primary;
}
Node *p_parse_init_list(Parser *p, NodeManager *nm) {
    Node *node = new_init_list_node(nm);
    p_consume(p); // '{'
    while (p_peek(p)->type != TK_CLOSE_CURLY) {
        p_append_element(node, p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE));
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_CURLY); // '}'
    return node;
}
Node *new_init_list_node(NodeManager *nm) {
    Node *node = new_node(nm, N_INIT_LIST);
    node->init_list.count = 0;
    node->init_list.capacity = 4;
    node->init_list.elements = malloc(sizeof(Node) * node->init_list.capacity);
    if (!node->init_list.elements) {
        printf("Failed to create new init list node\n");
        exit(1);
    }
    return node;
}

Node *new_function_node(NodeManager *nm) {
    Node *node = new_node(nm, N_FUNCTION);
    node->func.body = NULL;
    node->func.param_count = 0;
    node->func.param_capacity = 4;
    node->func.params = malloc(sizeof(Node) * node->func.param_capacity);
    node->func.type = NULL;
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
    if (is_type_token(p_peek(p)->type)) return p_parse_block_declaration(p, nm);
    else return p_parse_statement(p, nm);
}
Node *p_parse_block_declaration(Parser *p, NodeManager *nm);
/*
    Give the var decl node, if the var name/identifier is needed, otherwise NULL
*/
Type *p_parse_type(Parser *p, NodeManager *nm) {
    Type *type = type_invalid;
    if (p_peek(p)->type == TK_STRUCT) {
        type = p_parse_struct(p, nm);
    } else if (p_peek(p)->type == TK_ENUM) {
        type = p_parse_enum(p, nm);
    } else {
        Token *t = p_peek(p);
        type = token_to_type(p_consume(p)->type);
    }
    if (type == type_invalid) {
        printf("Tried to parse an unknown type\n");
        exit(1);
    }
    int ptrs = 0;
    while (p_peek(p)->type == TK_MULTIPLY) {
        ptrs++;
        p_consume(p);
    }
    for (int i = 0; i < ptrs; i++)
        type = get_pointer_type(type);

    return type;
}

Type *p_parse_enum(Parser *p, NodeManager *nm) {
    Type enum_t = enum_type();
    p_consume_a(p, TK_ENUM);
    if (p_peek(p)->type == TK_IDENTIFIER) {
        // TODO: add a null terminator plz
        enum_t._enum.name = p_consume(p)->value;
    }
    if (p_peek(p)->type == TK_OPEN_CURLY) {
        enum_t._enum.capacity = 4;
        enum_t._enum.fields = malloc(sizeof(EnumField) * enum_t._enum.capacity);
        if (!enum_t._enum.fields) {
            printf("Failed to allocate for enum fields\n");
            exit(1);
        }
        p_consume(p); // {
        int val = 0;
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            EnumField f;
            f.name = p_consume_a(p, TK_IDENTIFIER)->value;
            if (p_peek(p)->type == TK_EQ) {
                p_consume(p);
                Token *t = p_consume_a(p, TK_INT_LITERAL);
                int new_val = parse_int(t->value, t->size);
                val = new_val;
            }
            f.value = val++;
            f._enum_t = NULL;
            append_enum_field(&enum_t, &f);
            if (p_peek(p)->type == TK_COMMA) p_consume(p);
            else break;
        }
        p_consume(p); // }
        enum_t._enum.complete = true;
    }
    Type *s = get_enum_type(enum_t._enum.name);
    if (s) {
        if (enum_t._enum.complete) {
            if (s->_enum.complete) {
                // If the enum is already defined elsewhere,
                printf("Redefinition of enum %s\n", enum_t._enum.name);
                exit(1);
            }
            *s = enum_t;
        }
        return s;
    } else {
        Type *t = new_type();
        *t = enum_t;
        for (int i = 0; i < enum_t._enum.count; i++) {
            enum_t._enum.fields[i]._enum_t = t;
            p_append_enum_const(p, &enum_t._enum.fields[i]);
        }
        return t;
    }
}
Type *p_parse_struct(Parser *p, NodeManager *nm) {
    Type struct_t = struct_type();
    p_consume_a(p, TK_STRUCT);
    if (p_peek(p)->type == TK_IDENTIFIER) {
        // TODO: add a null terminator plz
        struct_t._struct.name = p_consume(p)->value;
    }
    if (p_peek(p)->type == TK_OPEN_CURLY) {
        struct_t._struct.capacity = 4;
        struct_t._struct.fields = malloc(sizeof(StructField) * struct_t._struct.capacity);
        if (!struct_t._struct.fields) {
            printf("Failed to allocate for struct fields\n");
            exit(1);
        }
        p_consume(p); // {
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            StructField f;
            Type *t = p_parse_type(p, nm);
            f.name = p_consume_a(p, TK_IDENTIFIER)->value;
            f.type = t;
            append_struct_field(&struct_t, &f);
            p_consume_semi(p);
        }
        p_consume(p); // }
        struct_t.size = align(struct_t.size, struct_t.align);
        struct_t._struct.complete = true;
    }
    Type *s = get_struct_type(struct_t._struct.name);
    if (s) {
        if (struct_t._struct.complete) {
            if (s->_struct.complete) {
                // If the struct is already defined elsewhere,
                printf("Redefinition of struct %s\n", struct_t._struct.name);
                exit(1);
            }
            *s = struct_t;
        }
        return s;
    } else {
        Type *t = new_type();
        *t = struct_t;
        return t;
    }
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

void p_append_symbol(Parser *p, Symbol *s) {
    if (p->st.count >= p->st.capacity) {
        p->st.capacity *= 2;
        Symbol *new_symbols = realloc(p->st.symbols, sizeof(Symbol) * p->st.capacity);
        if (!new_symbols) {
            printf("Failed to realloc for symbol table size: %d\n", p->st.capacity);
            exit(1);
        }
        p->st.symbols = new_symbols;
    }
    p->st.symbols[p->st.count++] = *s;
}
Symbol *p_get_symbol(Parser *p, const char *name, SymbolKind kind) {
    for (int i = 0; i < p->st.count; i++) {
        if ((kind == ANY || p->st.symbols[i].kind == kind) && strcmp(p->st.symbols[i].name, name) == 0) {
            return &p->st.symbols[i];
        }
    }
    return NULL;
}

Node *p_get_func_def(Parser *p, const char *name) {
    Symbol *s = p_get_symbol(p, name, FUNC);
    if (s) return s->func_def;
    printf("Tried to call %s which does not exist\n", name);
    exit(1);
}

void p_append_func_def(Parser *p, Node *func) { p_append_symbol(p, &(Symbol){func->func.name, FUNC, .func_def = func}); }
void p_append_var_decl(Parser *p, Node *var) { p_append_symbol(p, &(Symbol){var->var_decl.name, VAR, .var_decl = var}); }
void p_append_enum_const(Parser *p, EnumField *e) { 
    p_append_symbol(p, &(Symbol){e->name, ENUM, .enum_field = *e}); 
}

void p_append_element(Node *init_list, Node *element) {
    if (init_list->init_list.count >= init_list->init_list.capacity) {
        init_list->init_list.capacity *= 2;
        Node **new_elements = realloc(init_list->init_list.elements, sizeof(Node *) * init_list->init_list.capacity);
        if (!new_elements) {
            printf("Failed to append s");
            exit(1);
        }
    }
    init_list->init_list.elements[init_list->init_list.count++] = element;
}
Node *p_get_var_decl(Parser *p, const char *name) {
    Symbol *s = p_get_symbol(p, name, VAR);
    if (s) return s->var_decl;
    printf("Tried to find variable %s which does not exist\n", name);
    exit(1);
}

EnumField *p_get_enum_const(Parser *p, const char *name) {
    Symbol *s = p_get_symbol(p, name, ENUM);
    if (s) return &s->enum_field;
    printf("Tried to find enum constant %s which does not exist\n", name);
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
    node->_if.if_true = p_parse_statement(p, nm); //{[compound]} (in the future, can be a function call)
    if (p_peek(p)->type == TK_ELSE) {             // If there is an if, it can be a
        p_consume(p);                             // -> else
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

Node *current_func_definition(Parser *p) {
    for (int i = p->st.count - 1; i >= 0; i--) {
        if (p->st.symbols[i].kind == FUNC) return p->st.symbols[i].func_def;
    }
    printf("Cannot return outside of a function\n");
    exit(1);
}

Node *p_parse_return(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_RETURN);
    p_consume(p); // -> return
    node->_return.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    node->type = current_func_definition(p)->type;
    p_consume_semi(p);
    return node;
}
Node *p_parse_continue(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_CONTINUE);
    p_consume(p); // -> continue
    p_consume_semi(p);
    return node;
}
Node *p_parse_break(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_BREAK);
    p_consume(p); // -> break
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
    case TK_CONTINUE:
        return p_parse_continue(p, nm);
    case TK_BREAK:
        return p_parse_break(p, nm);
    case TK_RETURN:
        return p_parse_return(p, nm);
    case TK_MULTIPLY:
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
Node *p_parse_function(Parser *p, NodeManager *nm, Node *type) {
    Node *node = new_function_node(nm);
    node->func.name = p_consume_a(p, TK_IDENTIFIER)->value;
    node->func.type = type;
    node->type = type->type;

    p_consume_a(p, TK_OPEN_PAREN);
    while (p_peek(p)->type != TK_CLOSE_PAREN && !p_is_last_token(p)) {
        Node *param = new_node(nm, N_VAR_DECL);
        param->type = p_parse_type(p, nm);
        param->var_decl.name = p_consume_a(p, TK_IDENTIFIER)->value;
        param->var_decl.expr = NULL;
        p_append_param(node, param);
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_PAREN);
    p_append_func_def(p, node);
    node->func.body = p_parse_compound(p, nm);
    return node;
}

Node *p_parse_declaration(Parser *p, NodeManager *nm, Node *type_decl) {
    if (type_decl->type->kind == T_STRUCT || type_decl->type->kind == T_ENUM) {
        if (p_peek(p)->type != TK_IDENTIFIER) {
            p_consume_semi(p);
            return type_decl;
        }
        if (!type_decl->type->_struct.complete) {
            printf("Cannot instantiate incomplete type\n");
            exit(1);
        }
    }
    Node *var_decl = new_node(nm, N_VAR_DECL);
    var_decl->var_decl.name = p_consume_a(p, TK_IDENTIFIER)->value;

    if (p_peek(p)->type == TK_OPEN_SQUARE) {
        p_consume(p); // [
        int len = -1; // -1 for infered size
        if (p_peek(p)->type != TK_CLOSE_SQUARE) {
            // Only works for a[5], not a[b + 1] (can fix later)
            // Todo; allow for const expressions like [5 + 6] or smt
            Token *t = p_consume_a(p, TK_INT_LITERAL);
            len = parse_int(t->value, t->size);
        }
        p_consume_a(p, TK_CLOSE_SQUARE);
        type_decl->type = get_array_type(type_decl->type, len);
    }
    var_decl->var_decl.type = type_decl;
    var_decl->type = type_decl->type;

    if (p_peek(p)->type == TK_EQ) {
        p_consume(p);
        var_decl->var_decl.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    } else {
        // Forward declaration
        var_decl->var_decl.expr = NULL;
    }
    p_consume_semi(p);
    p_append_var_decl(p, var_decl);
    return var_decl;
}

// Either function or type/var declaration
Node *p_parse_external_declaration(Parser *p, NodeManager *nm) {
    Node *type_decl = new_node(nm, N_TYPE);
    type_decl->type = p_parse_type(p, nm);

    if (p_peek(p)->type == TK_IDENTIFIER && p_peek_next(p)->type == TK_OPEN_PAREN) {
        return p_parse_function(p, nm, type_decl);
    } else {
        return p_parse_declaration(p, nm, type_decl);
    }
}

Node *p_parse_block_declaration(Parser *p, NodeManager *nm) {
    Node *type_decl = new_node(nm, N_TYPE);
    type_decl->type = p_parse_type(p, nm);

    if (p_peek(p)->type == TK_IDENTIFIER && p_peek_next(p)->type == TK_OPEN_PAREN) {
        // return p_parse_function(p, nm, type_decl);
        printf("Function prototypes within blocks is unsupported\n");
        exit(1);
    } else {
        return p_parse_declaration(p, nm, type_decl);
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
        p_append_declaration(root, p_parse_external_declaration(p, nm));
    }
    return root;
}
