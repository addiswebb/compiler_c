#include "compiler_c/parse/parser.h"
#include "compiler_c/analyse/sema.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/util.h"
#include "compiler_c/tokenize/tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_STATEMENTS_PER_BLOCK 8

Parser new_parser() { return (Parser){0}; }

void init_parser(Parser *p, Array *src, const int size) {
    p->size = size;
    p->src = src;
    p->index = 0;
    p->expect_semi = true;
    array_init(&p->scopes_array, 4, sizeof(Array));
    p_append_symbol_table(p);
}

void p_append_symbol_table(Parser *p) {
    Array st;
    array_init(&st, 4, sizeof(Symbol));
    append(&p->scopes_array, &st);
}

/*
Is End of token array?
*/
bool p_is_last_token(const Parser *p) { return p->index >= p->size; }

Token *p_peek_n(const Parser *p, const int n) {
    if (p->index + n > p->src->count) {
        printf("P_peek_n Tried peeking past eof\n");
        return NULL;
    }
    return get_token(p->src, p->index + n);
}
Token *p_peek(const Parser *p) { return p_peek_n(p, 0); }
Token *p_peek_next(const Parser *p) { return p_peek_n(p, 1); }
Token *p_consume_n(Parser *p, const int n) {
    if (p->index + n > p->src->count) {
        printf("P_consume_n %d Reached the end of the token list %d/%d\n", n, p->index, p->src->count);
        return NULL;
    }
    Token *token = get_token(p->src, p->index);
    p->index += n;
    if (DEBUG_CONSUME) print_token(token);
    return token;
}

Token *p_consume(Parser *p) { return p_consume_n(p, 1); }

void p_skip_n(Parser *p, const int n) { p_consume_n(p, n); }
void p_skip(Parser *p) { p_consume_n(p, 1); }

/*
    Error on type mismatch
*/
void p_expect(const Parser *p, const TokenType expected_type) {
    if (!p_is_last_token(p)) {
        const TokenType token_type = get_token(p->src, p->index)->type;
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
    array_init(&node->translation_unit.declarations_array, DEFAULT_STATEMENTS_PER_BLOCK, sizeof(Node **));
    return node;
}

/*
    Creates a new compound node
    And allocates an array for its statements
*/
Node *new_compound_node(NodeManager *nm) {
    Node *node = new_node(nm, N_COMPOUND);
    array_init(&node->compound.items_array, DEFAULT_STATEMENTS_PER_BLOCK, sizeof(Node **));
    return node;
}

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
        break;
    case TK_OPEN_PAREN:
        p_consume_a(p, TK_OPEN_PAREN);
        if (is_type_token(p, p_peek(p))) {
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
            printf(" is not a an lvalue, needed for indexing\n");
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
        Node *func_call = new_function_call_node(nm, primary);

        while (p_peek(p)->type != TK_CLOSE_PAREN) {
            p_append_call_param(func_call, p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE));
            if (p_peek(p)->type == TK_COMMA) p_consume(p);
            else break;
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
    array_init(&node->init_list.elements_array, 4, sizeof(Node *));
    return node;
}

Node *new_function_node(NodeManager *nm) {
    Node *node = new_node(nm, N_FUNCTION);
    array_init(&node->func.params_array, 4, sizeof(Node **));
    node->func.body = NULL;
    node->func.type = NULL;
    return node;
}
Node *new_function_call_node(NodeManager *nm, Node *identifier) {
    Node *node = new_node(nm, N_FUNCTION_CALL);
    node->func_call.identifier = identifier;
    array_init(&node->func_call.params_array, 4, sizeof(Node *));
    return node;
}
Node *p_parse_goto_statement(Parser *p, NodeManager *nm) {
    p_consume(p);
    Node *identifier = new_node(nm, N_IDENTIFIER);
    const Token *t = p_consume_a(p, TK_IDENTIFIER);
    identifier->identifier.name = t->value;
    identifier->identifier.len = t->size;
    Node *node = new_node(nm, N_GOTO);
    node->_goto.identifier = identifier;
    p_consume_semi(p);
    return node;
}

Node *p_parse_label(Parser *p, NodeManager *nm) {
    Node *identifier = new_node(nm, N_IDENTIFIER);
    const Token *t = p_consume_a(p, TK_IDENTIFIER);
    identifier->identifier.name = t->value;
    identifier->identifier.len = t->size;
    Node *node = new_node(nm, N_LABEL);
    node->_goto.identifier = identifier;
    p_consume_a(p, TK_COLON);
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
    while (p_peek(p)->type == TK_DOT || p_peek(p)->type == TK_ARROW) {
        TokenType op = p_consume(p)->type;
        Token *t = p_consume_a(p, TK_IDENTIFIER);
        Node *member = new_node(nm, N_IDENTIFIER);
        member->identifier.name = t->value;
        member->identifier.len = t->size;

        Node *access = new_node(nm, N_MEMBER_ACCESS);
        access->member_access.op = op;
        access->member_access.identifier = primary;
        access->member_access.member = member;
        primary = access;
    }

    while (is_binary_operator(p_peek(p)->type) && !p_is_last_token(p) && op_precedence(p_peek(p)->type) >= min_prec) {
        const int prec = op_precedence(p_peek(p)->type);
        const int assoc = op_associativity(p_peek(p)->type);
        Node *binary = new_node(nm, N_BINARY);
        binary->binary.op = p_consume(p)->type;
        binary->binary.rhs = p_parse_expression(p, nm, prec + assoc);
        binary->binary.lhs = primary;
        primary = binary;
    }

    return primary;
}

Node *p_parse_block_item(Parser *p, NodeManager *nm) {
    if (is_type_token(p, p_peek(p))) return p_parse_block_declaration(p, nm);
    else return p_parse_statement(p, nm);
}
/*
    Give the var decl node, if the var name/identifier is needed, otherwise NULL
*/
Type *p_parse_type(Parser *p, NodeManager *nm) {
    Type *type;
    if (p_peek(p)->type == TK_STRUCT) {
        type = p_parse_struct(p, nm);
    } else if (p_peek(p)->type == TK_ENUM) {
        type = p_parse_enum(p, nm);
    } else {
        type = token_to_type(p, p_consume(p));
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
        array_init(&enum_t._enum.fields_array, 4, sizeof(EnumField));
        p_consume(p); // {
        int val = 0;
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            EnumField f;
            f.name = p_consume_a(p, TK_IDENTIFIER)->value;
            if (p_peek(p)->type == TK_EQ) {
                p_consume(p);
                const Token *t = p_consume_a(p, TK_INT_LITERAL);
                val = (int)parse_int(t->value, t->size);
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
        // Add it to the type pool, at &t
        Type *t = new_type();
        *t = enum_t;
        for (int i = 0; i < enum_t._enum.fields_array.count; i++) {
            get_enum_field(&enum_t, i)->_enum_t = t;
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
        array_init(&struct_t._struct.members_array, 4, sizeof(StructMember));
        p_consume(p); // {
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            StructMember f;
            Type *t = p_parse_type(p, nm);
            f.name = p_consume_a(p, TK_IDENTIFIER)->value;
            f.type = t;
            append_struct_member(&struct_t, &f);
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
    Appends a statement to the given compound node,
    Resizes its statement array if necessary.
*/
void p_append_block_item(Node *root, Node *item) {
    if (item != NULL) append(&root->compound.items_array, &item);
    else printf("Skipping empty node\n");
}

void p_append_param(Node *func, Node *param) {
    if (param != NULL) {
        append(&func->func.params_array, &param);
    } else {
        printf("Recieved a NULL param node to append\n");
        exit(1);
    }
}

void p_append_call_param(Node *func_call, Node *param) { append(&func_call->func_call.params_array, &param); }

Symbol *p_append_symbol(Array *st, const Symbol *s) { return (Symbol *)append(st, s); }

Symbol *p_get_symbol(const Parser *p, const char *name, const SymbolKind kind) {
    for (int i = p->scopes_array.count - 1; i >= 0; i--) {
        Array *st = get_symbol_table(p, i);
        for (int j = 0; j < st->count; j++) {
            Symbol *symbol = get_symbol(st, j);
            if ((kind == ANY || symbol->kind == kind) && strcmp(symbol->name, name) == 0) {
                return symbol;
            }
        }
    }
    return NULL;
}

Typedef *p_get_typedef(const Parser *p, const char *name) {
    Symbol *s = p_get_symbol(p, name, TYPEDEF);
    if (s) return &s->_typedef;
    printf("Tried to get the typedef of %s, which does not exist\n", name);
    exit(1);
}
Node *p_get_func_def(const Parser *p, const char *name) {
    const Symbol *s = p_get_symbol(p, name, FUNC);
    if (s) return s->func_def;
    printf("Tried to call %s which does not exist\n", name);
    exit(1);
}

void p_append_typedef(Parser *p, const Typedef *t) {
    p_append_symbol(get_current_symbol_table(p), &(Symbol){.name = t->new_def,
                                                           .kind = TYPEDEF,
                                                           .linkage = LINK_NONE,
                                                           .storage = STORAGE_NONE,
                                                           ._typedef = *t,
                                                           .scope_depth = p->scopes_array.count - 1});
}
Symbol *p_append_func_def(Parser *p, Node *f) {
    if (p->scopes_array.count > 2) {
        printf("Declaring function inside a function???\n");
        exit(1);
    }
    Linkage linkage = f->func.storage_class == STATIC ? LINK_INTERNAL : LINK_EXTERNAL;
    ;
    // if defined -> text, otherwise none
    Storage storage = STORAGE_TEXT;
    return p_append_symbol(get_current_symbol_table(p), &(Symbol){.name = f->func.name,
                                                                  .kind = FUNC,
                                                                  .linkage = linkage,
                                                                  .storage = storage,
                                                                  .func_def = f,
                                                                  .scope_depth = p->scopes_array.count - 1});
}
Symbol *p_append_var_decl(Parser *p, Node *v) {
    Linkage linkage = LINK_NONE;
    Storage storage = STORAGE_NONE;
    if (v->var_decl.is_global) {
        storage = v->var_decl.has_initializer ? STORAGE_DATA : STORAGE_BSS;
        linkage = v->var_decl.storage_class == STATIC ? LINK_INTERNAL : LINK_EXTERNAL;
    } else {
        // local variable
        storage = STORAGE_NONE;
        if (v->var_decl.storage_class == NONE) linkage = LINK_NONE;
        if (v->var_decl.storage_class == EXTERN) linkage = LINK_EXTERNAL;
        if (v->var_decl.storage_class == STATIC) linkage = LINK_INTERNAL;
    }
    return p_append_symbol(get_current_symbol_table(p), &(Symbol){.name = v->var_decl.identifier->identifier.name,
                                                                  .kind = VAR,
                                                                  .linkage = linkage,
                                                                  .storage = storage,
                                                                  .var_decl = v,
                                                                  .scope_depth = p->scopes_array.count - 1});
}
void p_append_enum_const(Parser *p, const EnumField *e) {
    p_append_symbol(get_current_symbol_table(p), &(Symbol){.name = e->name,
                                                           .kind = ENUM,
                                                           .linkage = LINK_NONE,
                                                           .storage = STORAGE_NONE,
                                                           .enum_field = *e,
                                                           .scope_depth = p->scopes_array.count - 1});
}

void p_append_element(Node *init_list, Node *element) { append(&init_list->init_list.elements_array, &element); }

Node *p_get_var_decl(const Parser *p, const char *name) {
    const Symbol *s = p_get_symbol(p, name, VAR);
    if (s) return s->var_decl;
    printf("Tried to find variable %s which does not exist\n", name);
    exit(1);
}

EnumField *p_get_enum_const(const Parser *p, const char *name) {
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
Node *p_parse_case(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_CASE);
    if (p_peek(p)->type == TK_CASE) {
        p_consume_a(p, TK_CASE);
        node->_case.test = p_parse_primary_expression(p, nm);
    } else {
        p_consume_a(p, TK_DEFAULT);
        node->_case.test = NULL;
    }
    p_consume_a(p, TK_COLON);
    return node;
}

void p_append_case(Node *s, Node *c) {
    c->_case.i = s->_switch.block->compound.items_array.count;
    append(&s->_switch.cases_array, &c);
}

Node *p_parse_switch_statement(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_SWITCH);
    array_init(&node->_switch.cases_array, 4, sizeof(Node *));

    p_consume_a(p, TK_SWITCH);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_switch.test = p_parse_primary_expression(p, nm);
    p_consume_a(p, TK_CLOSE_PAREN);

    node->_switch.block = new_compound_node(nm);

    p_consume_a(p, TK_OPEN_CURLY);
    while (p_peek(p)->type != TK_CLOSE_CURLY) {
        Node *item;
        if (p_peek(p)->type == TK_CASE || p_peek(p)->type == TK_DEFAULT) {
            item = p_parse_case(p, nm);
            p_append_case(node, item);
        } else {
            item = p_parse_block_item(p, nm);
        }
        p_append_block_item(node->_switch.block, item);
    }
    p_consume_a(p, TK_CLOSE_CURLY);

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

Node *p_get_current_func_definition(const Parser *p) {
    for (int i = p->scopes_array.count - 1; i >= 0; i--) {
        Array *st = get_symbol_table(p, i);
        for (int j = 0; j < st->count; j++) {
            Symbol *symbol = get_symbol(st, j);
            if (symbol->kind == FUNC) return symbol->func_def;
        }
    }
    printf("Cannot return outside of a function\n");
    exit(1);
}

Node *p_parse_return(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_RETURN);
    p_consume(p); // -> return
    node->_return.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    node->type = type_invalid;
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

Node *p_parse_statement(Parser *p, NodeManager *nm) {
    switch (p_peek(p)->type) {
    case TK_IF:
        return p_parse_if_statement(p, nm);
    case TK_WHILE:
        return p_parse_while_loop(p, nm);
    case TK_FOR:
        return p_parse_for_loop(p, nm);
    case TK_SWITCH:
        return p_parse_switch_statement(p, nm);
    case TK_CONTINUE:
        return p_parse_continue(p, nm);
    case TK_BREAK:
        return p_parse_break(p, nm);
    case TK_RETURN:
        return p_parse_return(p, nm);
    case TK_MULTIPLY:
    case TK_IDENTIFIER:
        if (p_peek_next(p)->type == TK_COLON) return p_parse_label(p, nm);
        Node *n = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        p_consume_semi(p);
        return n;
    case TK_OPEN_CURLY:
        return p_parse_compound(p, nm);
    case TK_SEMI:
        printf("Null statement is currently unsupported ';'\n");
        exit(1);
    case TK_GOTO:
        return p_parse_goto_statement(p, nm);
    default:
        return p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    }
}

void p_push_scope(Parser *p) { p_append_symbol_table(p); }
void p_pop_scope(Parser *p) {
    array_free(get_current_symbol_table(p));
    pop(&p->scopes_array);
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

int p_parse_parameter_list(Parser *p, NodeManager *nm, Node *func) {
    p_consume_a(p, TK_OPEN_PAREN);
    while (p_peek(p)->type != TK_CLOSE_PAREN && !p_is_last_token(p)) {
        if (p_peek(p)->type == TK_ELLIPSES) {
            if (func->func.params_array.count >= 1) {
                p_consume(p);
                p_consume_a(p, TK_CLOSE_PAREN);
                return 1;
            } else {
                printf("Variadic function declaration must have at least one named parameter.\n");
                exit(1);
            }
        }
        Node *param = new_node(nm, N_VAR_DECL);
        param->type = p_parse_type(p, nm);
        param->var_decl.identifier = p_parse_decl_identifier(p, nm);
        param->var_decl.expr = NULL;
        p_append_param(func, param);
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_PAREN);
    return 0;
}

/*
    Consumes
    `(type) identifier ([var decl],*) {[statement]*}`

    () contains any amount of var declarations, including zero,
    and {} contains any amount of statements, including zero.
*/
Node *p_parse_function(Parser *p, NodeManager *nm, Node *type, const StorageClass storage_class) {
    Node *node = new_function_node(nm);
    node->func.name = p_consume_a(p, TK_IDENTIFIER)->value;
    node->func.type = type;
    node->type = type->type;

    node->func.is_variadic = p_parse_parameter_list(p, nm, node);
    if (p_peek(p)->type == TK_SEMI) {
        p_consume(p);
        node->func.has_initializer = false;
        node->func.is_defined = false;
    } else {
        node->func.has_initializer = true;
        node->func.is_defined = true;
    }
    node->func.storage_class = storage_class;
    if (node->func.has_initializer) node->func.body = p_parse_compound(p, nm);
    return node;
}

Node *p_parse_decl_identifier(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_IDENTIFIER);
    const bool expect_closing_paren = p_peek(p)->type == TK_OPEN_PAREN;
    if (expect_closing_paren) p_consume(p); // (
    const Token *t = p_consume_a(p, TK_IDENTIFIER);
    node->identifier.name = t->value;
    node->identifier.len = t->size;
    if (expect_closing_paren) p_consume_a(p, TK_CLOSE_PAREN);
    return node;
}

Node *p_parse_declaration(Parser *p, NodeManager *nm, Node *type_decl, const StorageClass storage_class, const bool global) {
    if (type_decl->type->kind == T_STRUCT || type_decl->type->kind == T_ENUM) {
        if (p_peek(p)->type != TK_IDENTIFIER) {
            p_consume_semi(p);
            return type_decl;
        }
        // TODO: this assumes .complete is at the same offset for _enum
        if (!type_decl->type->_struct.complete) {
            printf("Cannot instantiate an incomplete type\n");
            exit(1);
        }
    }
    Node *var_decl = new_node(nm, N_VAR_DECL);
    var_decl->var_decl.identifier = p_parse_decl_identifier(p, nm);
    var_decl->var_decl.is_global = global;

    if (p_peek(p)->type == TK_OPEN_SQUARE) {
        p_consume(p); // [
        int len = -1; // -1 for inferred size
        if (p_peek(p)->type != TK_CLOSE_SQUARE) {
            // Only works for a[5], not a[b + 1] (can fix later)
            // Todo; allow for const expressions like [5 + 6] or smt
            const Token *t = p_consume_a(p, TK_INT_LITERAL);
            len = (int)parse_int(t->value, t->size);
        }
        p_consume_a(p, TK_CLOSE_SQUARE);
        type_decl->type = get_array_type(type_decl->type, len);
    }
    var_decl->var_decl.type = type_decl;
    var_decl->type = type_decl->type;

    if (p_peek(p)->type == TK_EQ) {
        p_consume(p);
        var_decl->var_decl.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        var_decl->var_decl.has_initializer = true;
    } else {
        // Forward declaration
        var_decl->var_decl.has_initializer = false;
        var_decl->var_decl.expr = NULL;
    }
    var_decl->var_decl.storage_class = storage_class;
    var_decl->var_decl.is_defined = storage_class != EXTERN;
    p_consume_semi(p);

    return var_decl;
}

// Either function or type/var declaration
Node *p_parse_external_declaration(Parser *p, NodeManager *nm) {
    StorageClass storage_class = NONE;
    switch (p_peek(p)->type) {
    case TK_TYPEDEF:
        return p_parse_typedef(p, nm);
    case TK_EXTERN:
        storage_class = EXTERN;
        p_consume(p);
        break;
    case TK_STATIC:
        storage_class = STATIC;
        p_consume(p);
        break;
    default:
        break;
    }
    Node *type_decl = new_node(nm, N_TYPE);
    type_decl->type = p_parse_type(p, nm);

    if (p_peek(p)->type == TK_IDENTIFIER && p_peek_next(p)->type == TK_OPEN_PAREN) {
        return p_parse_function(p, nm, type_decl, storage_class);
    } else {
        return p_parse_declaration(p, nm, type_decl, storage_class, true);
    }
}
Node *p_parse_typedef(Parser *p, NodeManager *nm) {
    p_consume_a(p, TK_TYPEDEF);
    Node *node = new_node(nm, N_TYPEDEF);
    node->type = p_parse_type(p, nm);
    node->_typedef.symbol = p_parse_decl_identifier(p, nm);
    p_consume_semi(p);
    p_append_typedef(p, &(Typedef){.type = node->type, .new_def = node->_typedef.symbol->identifier.name});
    return node;
}

Node *p_parse_block_declaration(Parser *p, NodeManager *nm) {
    StorageClass storage_class = NONE;
    switch (p_peek(p)->type) {
    case TK_TYPEDEF:
        return p_parse_typedef(p, nm);
    case TK_EXTERN:
        storage_class = EXTERN;
        break;
    case TK_STATIC:
        storage_class = STATIC;
        break;
    default:
        break;
    }
    Node *type_decl = new_node(nm, N_TYPE);
    type_decl->type = p_parse_type(p, nm);

    if (p_peek(p)->type == TK_IDENTIFIER && p_peek_next(p)->type == TK_OPEN_PAREN) {
        // return p_parse_function(p, nm, type_decl);
        printf("Function prototypes within blocks is unsupported\n");
        exit(1);
    } else {
        return p_parse_declaration(p, nm, type_decl, storage_class, false);
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
        Node *decl = p_parse_external_declaration(p, nm);
        append(&root->translation_unit.declarations_array, &decl);
    }
    return root;
}
bool is_type_token(const Parser *p, const Token *t) {
    switch (t->type) {
    case TK_CHAR:
    case TK_SHORT:
    case TK_INT:
    case TK_LONG:
    case TK_FLOAT:
    case TK_DOUBLE:
    case TK_VOID:
    case TK_STRUCT:
    case TK_ENUM:
        return true;
    case TK_IDENTIFIER:
        return p_get_symbol(p, t->value, TYPEDEF) != NULL;
    default:
        return false;
    }
}

Type *token_to_type(const Parser *p, const Token *t) {
    switch (t->type) {
    case TK_CHAR:
        return type_char;
    case TK_SHORT:
        return type_short;
    case TK_INT:
        return type_int;
    case TK_LONG:
        return type_long;
    case TK_FLOAT:
        return type_float;
    case TK_DOUBLE:
        return type_double;
    case TK_VOID:
        return type_void;
    case TK_IDENTIFIER:
        return p_get_typedef(p, t->value)->type;
    default:
        return type_invalid;
    }
}

/* ===== Parse Literals ===== */

int64_t parse_int(const char *raw, int len) {
    if (len > 20) {
        printf("Cannot parse an integer larger than 64 bytes\n");
        exit(1);
    }
    if (raw[0] == '0' && len > 1) {
        switch (raw[1]) {
        case 'x':
        case 'X':
            return parse_hex(raw + 2, len - 2);
        case 'b':
        case 'B':
            return parse_binary(raw + 2, len - 2);
        default:
            return parse_oct(raw + 1, len - 1);
        }
    }
    return parse_dec(raw, len);
}

int64_t parse_dec(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        res = res * 10 + (*raw - '0');
        raw++;
    }
    return res;
}
int64_t parse_binary(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value = (*raw - '0');
        if (value > 1) {
            printf("Parse Binary Failed: digit cannot be larger than 1\n");
            exit(1);
        }
        res = res * 2 + value;
        raw++;
    }
    return res;
}
int64_t parse_oct(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value = (*raw - '0');
        if (value > 7) {
            printf("Parse Octal Failed: digit cannot be larger than 7\n");
            exit(1);
        }
        res = res * 8 + value;
        raw++;
    }
    return res;
}

int64_t parse_hex(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value;
        if (is_num(*raw)) value = *raw - '0';
        char c = *raw | 0x20;
        if (c <= 'f' && c >= 'a') value = c - 'a' + 10;
        res = res * 16 + value;
        raw++;
    }
    return res;
}

double parse_float(const char *raw, int len) {
    double res = 0;
    const char *end = raw + len;
    double m = 0;
    while (raw < end) {
        if (*raw == '.') {
            m = 0.1;
            raw++;
            continue;
        }
        int digit = *raw - '0';
        if (m) {
            res += digit * m;
            m *= 0.1;
        } else {
            res = res * 10.0 + digit;
        }
        raw++;
    }

    return res;
}
