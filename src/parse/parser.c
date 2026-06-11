#include "compiler_c/parse/parser.h"
#include "compiler_c/analyse/sema.h"
#include "compiler_c/compiler.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/core/util.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/tokenize/tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser new_parser() { return (Parser){0}; }

void init_parser(Parser *p, Array *src, const int size) {
    p->size = size;
    p->src = src;
    p->index = 0;
    p->expect_semi = true;
    array_init(&p->scopes_array, 4, sizeof(Array));
    arena_init(&p->symbols_arena, 64, sizeof(Symbol));
    p_append_symbol_table(p);
}

void free_parser(Parser *p) {
    for (int i = 0; i < p->scopes_array.count; i++) {
        array_free(get_symbol_table(p, i));
    }
    array_free(&p->scopes_array);
    arena_free(&p->symbols_arena);
}

void p_append_symbol_table(Parser *p) {
    Array symbol_table;
    array_init(&symbol_table, 4, sizeof(Symbol *));
    append(&p->scopes_array, &symbol_table);
}

int current_scope_depth;

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
    if (n == 0) return NULL;
    if (p->index + n > p->src->count) {
        printf("P_consume_n %d Reached the end of the token list %d/%d\n", n, p->index, p->src->count);
        return NULL;
    }
    Token *token = get_token(p->src, p->index);
    p->index += n;
    if (has_flag(CF_DEBUG_PARSER)) print_token(token);
    return token;
}

Token *p_consume(Parser *p) { return p_consume_n(p, 1); }

void p_skip_n(Parser *p, const int n) { p_consume_n(p, n); }
void p_skip(Parser *p) { p_consume_n(p, 1); }

void p_expect(const Parser *p, const TokenType expected_type) {
    if (!p_is_last_token(p)) {
        const Token *token = get_token(p->src, p->index);
        ASSERT(token->type == expected_type, "[%d:%d]: Expected %tk got %tk\n", token->line_n, token->char_n, expected_type, token->type);
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
Node *new_init_translation_unit_node(NodeManager *nm) {
    Node *node = new_node(nm, N_TRANSLATION_UNIT);
    array_init(&node->translation_unit.declarations_array, 8, sizeof(Node **));
    return node;
}

Node *new_compound_node(NodeManager *nm) {
    Node *node = new_node(nm, N_COMPOUND);
    array_init(&node->compound.items_array, 8, sizeof(Node **));
    return node;
}

Node *p_parse_builtin(Parser *p, NodeManager *nm, const BuiltinKind kind) {
    Node *b = new_node(nm, N_BUILTIN);
    b->_builtin.kind = kind;
    p_consume_a(p, TK_OPEN_PAREN);
    switch (kind) {
    case BUILTIN_MEMCPY:
        array_init(&b->_builtin.params, 3, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        break;
    case BUILTIN_VA_START:
        array_init(&b->_builtin.params, 2, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        break;
    case BUILTIN_VA_ARG:
        array_init(&b->_builtin.params, 2, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        p_consume_a(p, TK_COMMA);
        Node *va_arg_type = new_node(nm, N_TYPE);
        va_arg_type->type = p_parse_abstract_type(p, nm);
        append(&b->_builtin.params, &va_arg_type);
        break;
    case BUILTIN_VA_END:
        array_init(&b->_builtin.params, 1, sizeof(Node *));
        append(&b->_builtin.params, &(Node *){p_parse_expression(p, nm, 0)});
        break;
    case BUILTIN_NONE:
    default:
        PANIC("builtin reached but not builtin\n");
    }

    p_consume_a(p, TK_CLOSE_PAREN);
    return b;
}
Node *p_parse_postfix_expression(Parser *p, NodeManager *nm) {
    Node *expr = p_parse_primary_expression(p, nm);
    for (;;) {
        switch (p_peek(p)->type) {
        case TK_OPEN_PAREN:
            if (expr->kind == N_IDENTIFIER) {
                const BuiltinKind kind = get_builtin_kind(expr->identifier.name);
                if (kind != BUILTIN_NONE) {
                    expr = p_parse_builtin(p, nm, kind);
                    break;
                }
            }
            p_consume(p); // '('
            Node *func_call = new_function_call_node(nm, expr);

            while (p_peek(p)->type != TK_CLOSE_PAREN) {
                p_append_call_param(func_call, p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE));
                if (p_peek(p)->type == TK_COMMA) p_consume(p);
                else break;
            }
            p_consume_a(p, TK_CLOSE_PAREN);
            expr = func_call;
            break;
        case TK_OPEN_SQUARE:
            p_consume(p); // '['
            ASSERT(is_lvalue(expr), "%nt is not an lvalue which is needed for indexing\n", expr->kind);
            Node *node = new_node(nm, N_INDEX);
            node->index.index = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
            node->index.identifier = expr;
            expr = node;
            p_consume_a(p, TK_CLOSE_SQUARE);
            break;
        case TK_DOT:
        case TK_ARROW:
            const TokenType op = p_consume(p)->type;
            const Token *t = p_consume_a(p, TK_IDENTIFIER);
            Node *member = new_node(nm, N_IDENTIFIER);
            member->identifier.name = t->value;
            member->identifier.len = t->size;

            Node *access = new_node(nm, N_MEMBER_ACCESS);
            access->member_access.op = op;
            access->member_access.identifier = expr;
            access->member_access.member = member;
            expr = access;
            break;
        case TK_INCR:
        case TK_DECR:
            Node *unary = new_node(nm, N_UNARY);
            unary->unary.op = p_consume(p)->type;
            unary->unary.associativity = LEFT_ASSOCIATIVITY;
            unary->unary.expr = expr;
            expr = unary;
            break;
        default:
            return expr;
        }
    }
}
Node *p_parse_prefix(Parser *p, NodeManager *nm) {
    if (is_unary_operator(p_peek(p)->type)) {
        Node *node = new_node(nm, N_UNARY);
        node->unary.op = p_consume(p)->type;
        node->unary.associativity = RIGHT_ASSOCIATIVITY;
        if (node->unary.op == TK_SIZEOF && p_peek(p)->type == TK_OPEN_PAREN && is_start_of_type(p, p_peek_next(p))) {
            node->unary.expr = new_node(nm, N_TYPE);
            p_consume(p); // '('
            node->unary.expr->type = p_parse_abstract_type(p, nm);
            p_consume_a(p, TK_CLOSE_PAREN); // ')'
        } else node->unary.expr = p_parse_cast(p, nm);
        return node;
    }
    return p_parse_postfix_expression(p, nm);
}

Node *p_parse_primary_expression(Parser *p, NodeManager *nm) {
    Node *primary = NULL;
    Token *tk;

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
        if (primary->literal.kind == L_STRING) {
            Array cat_str;
            array_init(&cat_str, primary->literal.len + tk->size + 1, sizeof(char));
            memcpy(cat_str.data, primary->literal.raw_rata, primary->literal.len);
            cat_str.count += primary->literal.len;
            while (p_peek(p)->type == TK_STRING_LITERAL) {
                tk = p_consume(p);
                array_str_catn(&cat_str, tk->value, tk->size);
            }
            primary->literal.raw_rata = cat_str.data;
            primary->literal.len = cat_str.count;
        }
        return primary;
    case TK_IDENTIFIER:
        primary = new_node(nm, N_IDENTIFIER);
        tk = p_consume(p);
        primary->identifier.name = tk->value;
        primary->identifier.len = tk->size;
        break;
    case TK_OPEN_PAREN:
        p_consume_a(p, TK_OPEN_PAREN);
        primary = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        p_consume_a(p, TK_CLOSE_PAREN);
        return primary;
    case TK_OPEN_CURLY:
        return p_parse_init_list(p, nm);
    default:
        tk = p_consume(p);
        PANIC("[%d:%d]: Expected primary expression got %tk\n", tk->line_n, tk->char_n, tk->type);
    }

    return primary;
}
Node *p_parse_init_list(Parser *p, NodeManager *nm) {
    Node *node = new_init_list_node(nm);
    p_consume(p); // '{'
    while (p_peek(p)->type != TK_CLOSE_CURLY) {
        if (p_peek(p)->type == TK_DOT) {
            p_consume(p);
            const Token *identifier = p_consume_a(p, TK_IDENTIFIER);
            p_consume_a(p, TK_EQ);

            Node *member_assign = new_node(nm, N_DESIGNATOR);
            member_assign->designator.kind = T_STRUCT; // Possibly also T_UNION
            member_assign->designator._struct.name = identifier->value;
            member_assign->designator.value = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);

            p_append_element(node, member_assign);
        } else if (p_peek(p)->type == TK_OPEN_SQUARE) {
            p_consume(p);

            Node *index_expr = p_parse_expression(p, nm, 0);
            p_consume_a(p, TK_CLOSE_SQUARE);
            p_consume_a(p, TK_EQ);
            Node *element_assign = new_node(nm, N_DESIGNATOR);
            // TODO investigate why this was commented, and uncomment if good
            // element_assign->designated_init.kind = T_ARRAY;
            element_assign->designator._array.const_expr = index_expr;
            element_assign->designator.value = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
            p_append_element(node, element_assign);
        } else p_append_element(node, p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE));
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_CURLY); // '}'
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

Node *p_parse_cast(Parser *p, NodeManager *nm) {
    // Try parse a cast (Type) or compound literal (Type){}
    if (p_peek(p)->type == TK_OPEN_PAREN && is_start_of_type(p, p_peek_next(p))) {
        p_consume_a(p, TK_OPEN_PAREN);
        Type *type = p_parse_abstract_type(p, nm);
        p_consume_a(p, TK_CLOSE_PAREN);
        if (p_peek(p)->type == TK_OPEN_CURLY) {
            Node *comp_node = new_node(nm, N_COMPOUND_LITERAL);
            comp_node->type = type;
            comp_node->compound_literal.value = p_parse_init_list(p, nm);
            return comp_node;
        }
        Node *cast_node = new_node(nm, N_CAST);
        cast_node->type = type;
        cast_node->cast.to = type;
        cast_node->cast.expr = p_parse_cast(p, nm);
        return cast_node;
    }
    return p_parse_prefix(p, nm);
}
Node *p_parse_binary(Parser *p, NodeManager *nm, Node *lhs) {
    Node *b = new_node(nm, N_BINARY);
    b->binary.op = p_consume(p)->type;
    b->binary.rhs = p_parse_expression(p, nm, op_precedence(b->binary.op) + op_associativity(b->binary.op));
    b->binary.lhs = lhs;
    return b;
}
Node *p_parse_ternary(Parser *p, NodeManager *nm, Node *cond) {
    p_consume(p);
    Node *t = new_node(nm, N_TERNARY);
    t->ternary.cond = cond;
    t->ternary.if_true = p_parse_expression(p, nm, 0);
    p_consume_a(p, TK_COLON);
    t->ternary.if_false = p_parse_expression(p, nm, op_precedence(TK_TERNARY));
    return t;
}
Node *p_parse_expression(Parser *p, NodeManager *nm, const int min_prec) {
    if (p_peek(p)->type == TK_SEMI) {
        if (p->expect_semi) p_consume(p);
        return new_node(nm, N_NULL);
    }
    Node *primary = p_parse_cast(p, nm);
    // Post-decrement/increment
    while (is_postfix_operator(p_peek(p)->type)) primary = p_parse_postfix_expression(p, nm);

    for (;;) {
        if (is_binary_operator(p_peek(p)->type) && op_precedence(p_peek(p)->type) >= min_prec) {
            primary = p_parse_binary(p, nm, primary);
        } else if (p_peek(p)->type == TK_TERNARY && op_precedence(TK_TERNARY) >= min_prec) {
            primary = p_parse_ternary(p, nm, primary);
        } else break;
    }

    return primary;
}

Node *p_parse_block_item(Parser *p, NodeManager *nm) {
    if (is_start_of_type(p, p_peek(p))) return p_parse_block_declaration(p, nm);
    return p_parse_statement(p, nm);
}

Type *p_parse_abstract_type(Parser *p, NodeManager *nm) {
    const char *name = NULL;
    Type *type = p_parse_type(p, nm, &name);
    ASSERT(name == NULL, "Unexpected identifier in parsing abstract type.\n");
    return type;
}

void free_declarator(Declarator *decl) {
    for (int i = 0; i < decl->modifiers.count; i++) {
        Modifier *mod = get(&decl->modifiers, i);
        if (mod->kind == MOD_FUNCTION) array_free(&mod->function.params);
    }
    array_free(&decl->modifiers);
}
Type *p_parse_type(Parser *p, NodeManager *nm, const char **name) {
    Type *type;
    unsigned int qualifiers = QUAL_NONE;
    bool is_signed = SIGNED;
    for (;;) {
        if (p_peek(p)->type == TK_CONST) qualifiers |= QUAL_CONST;
        else if (p_peek(p)->type == TK_VOLATILE) qualifiers |= QUAL_VOLATILE;
        else if (p_peek(p)->type == TK_UNSIGNED) is_signed = UNSIGNED;
        else if (p_peek(p)->type == TK_SIGNED) is_signed = SIGNED;
        else break;
        p_consume(p);
    }
    if (p_peek(p)->type == TK_STRUCT) type = p_parse_struct(p, nm);
    else if (p_peek(p)->type == TK_ENUM) type = p_parse_enum(p, nm);
    else if (p_peek(p)->type == TK_UNION) type = p_parse_union(p, nm);
    else type = token_to_type(p, p_consume(p));

    ASSERT(type != type_invalid, "Got type_invalid in p_parse_base_type\n");

    // Find unsigned type if needed for T_INT only
    if (type->kind == T_INT && !is_signed && is_signed != type->is_signed) type = get_unsigned_type(type);

    Declarator decl = p_parse_declarator(p, nm);
    *name = decl.name;
    type = get_modified_type(type, &decl);
    if (qualifiers != QUAL_NONE) type = get_qualified_type(type, qualifiers);
    free_declarator(&decl);
    return type;
}

Declarator p_parse_declarator(Parser *p, NodeManager *nm) {
    Declarator d = {.name = NULL};
    array_init(&d.modifiers, 4, sizeof(Modifier));
    int ptrs = 0;
    while (p_peek(p)->type == TK_MULTIPLY) {
        p_consume(p);
        ptrs++;
    }
    if (p_peek(p)->type == TK_OPEN_PAREN) {
        p_consume(p);
        free_declarator(&d);
        d = p_parse_declarator(p, nm);
        p_consume_a(p, TK_CLOSE_PAREN);
    } else if (p_peek(p)->type == TK_IDENTIFIER) d.name = p_consume_a(p, TK_IDENTIFIER)->value;

    for (;;) {
        if (p_peek(p)->type == TK_OPEN_SQUARE) {
            p_consume(p);      // [
            Node *expr = NULL; // NULL for inferred size

            if (p_peek(p)->type != TK_CLOSE_SQUARE) expr = p_parse_expression(p, nm, 0);

            p_consume_a(p, TK_CLOSE_SQUARE);
            append(&d.modifiers, &(Modifier){.kind = MOD_ARRAY, .array_bounds = expr});
        } else if (p_peek(p)->type == TK_OPEN_PAREN) {
            Modifier func_modifier = p_parse_parameter_list(p, nm);
            append(&d.modifiers, &func_modifier);
        } else break;
    }
    for (int i = 0; i < ptrs; i++) {
        append(&d.modifiers, &(Modifier){.kind = MOD_POINTER});
    }
    return d;
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
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            EnumField f = {};
            f.name = p_consume_a(p, TK_IDENTIFIER)->value;
            if (p_peek(p)->type == TK_EQ) {
                p_consume(p);
                f.const_expr = p_parse_expression(p, nm, 0);
            }
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
                PANIC("Redefinition of enum %s\n", enum_t._enum.name);
            } else *s = enum_t;
        }
        return s;
    } else {
        // Add it to the type pool, at &t
        Type *t = new_type();
        for (int i = 0; i < enum_t._enum.fields_array.count; i++) {
            EnumField *f = get_enum_field(&enum_t, i);
            // Update the fields to have reference to parent enum type
            f->_enum_t = t;
        }
        *t = enum_t;
        t->is_resolved = false;
        return t;
    }
}
Type *p_parse_union(Parser *p, NodeManager *nm) {
    Type union_t = union_type();
    p_consume_a(p, TK_UNION);
    if (p_peek(p)->type == TK_IDENTIFIER) {
        union_t._union.name = p_consume(p)->value;
    }

    if (p_peek(p)->type == TK_OPEN_CURLY) {
        array_init(&union_t._union.members_array, 4, sizeof(UnionMember));
        p_consume(p); // {
        while (p_peek(p)->type != TK_CLOSE_CURLY) {
            UnionMember m;
            m.name = NULL;
            Type *t = p_parse_type(p, nm, &m.name);
            if (!(t->kind == T_STRUCT || t->kind == T_UNION)) ASSERT(m.name, "Scalar Union member must be named\n");
            m.type = t;
            m.offset = 0;
            append_union_member(&union_t, &m);
            p_consume_semi(p);
        }
        p_consume(p); // }
        union_t._union.complete = true;
    }

    Type *u = get_union_type(union_t._union.name);
    if (u) {
        if (union_t._union.complete) {
            if (u->_union.complete) {
                // If the union is already defined elsewhere,
                PANIC("Redefinition of union %s\n", union_t._union.name);
            } else *u = union_t;
        }
        return u;
    } else {
        Type *t = new_type();
        *t = union_t;
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
            f.name = NULL;
            Type *t = p_parse_type(p, nm, &f.name);
            if (!(t->kind == T_STRUCT || t->kind == T_UNION)) ASSERT(f.name, "Scalar Struct member must be named\n");
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
            ASSERT(!s->_struct.complete, "Redefinition of struct %s\n", struct_t._struct.name);
            *s = struct_t;
        }
        return s;
    }
    Type *t = new_type();
    *t = struct_t;
    return t;
}

/*
    Appends a statement to the given compound node,
    Resizes its statement array if necessary.
*/
void p_append_block_item(Node *root, Node *item) {
    if (item != NULL) append(&root->compound.items_array, &item);
    else WARN("p_append_block_item: Skipping empty node\n");
}

void p_append_param(const Node *func, Node *param) {
    if (param != NULL) append(&func->type->_func.params, &param);
    else PANIC("Received a NULL param node to append\n");
}

void p_append_call_param(Node *func_call, Node *param) { append(&func_call->func_call.params_array, &param); }

Symbol *p_append_symbol(Array *st, const Symbol *s) { return *(Symbol **)append(st, &s); }

Symbol *p_get_symbol(const Parser *p, const char *name, const SymbolKind kind, const bool same_depth) {
    for (int i = p->scopes_array.count - 1; i >= 0; i--) {
        const Array *st = get_symbol_table(p, i);
        for (int j = 0; j < st->count; j++) {
            Symbol *symbol = get_symbol(st, j);
            if (same_depth && symbol->scope_depth != p->current_scope_depth) continue;
            if (symbol->scope_depth <= p->current_scope_depth && (kind == ANY || symbol->kind == kind) && strcmp(symbol->name, name) == 0) {
                return symbol;
            }
        }
    }
    return NULL;
}

Typedef *p_get_typedef(const Parser *p, const char *name) {
    Symbol *s = p_get_symbol(p, name, TYPEDEF, false);
    if (s) return &s->_typedef;
    PANIC("Tried to get the typedef of %s, which does not exist\n", name);
}

Symbol *p_new_symbol(Parser *p, const Symbol *s) { return arena_append(&p->symbols_arena, s); }

void p_append_typedef(Parser *p, const Typedef *t) {
    const Symbol s = (Symbol){.name = t->new_def,
                              .kind = TYPEDEF,
                              .linkage = LINK_NONE,
                              .storage = STORAGE_NONE,
                              ._typedef = *t,
                              .type = t->type,
                              .scope_depth = p->current_scope_depth};
    p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &s));
}
Symbol *p_append_func_def(Parser *p, Node *f) {
    // TODO double check max scope depth for a function declaration
    ASSERT(p->scopes_array.count <= 2, "Function declaration within function definition is unsupported.\n");
    const Linkage linkage = f->func.storage_class == STATIC ? LINK_INTERNAL : LINK_EXTERNAL;
    // if defined -> text, otherwise none
    const Storage storage = STORAGE_TEXT;
    return p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &(Symbol){.name = f->func.name,
                                                                                  .kind = FUNC,
                                                                                  .linkage = linkage,
                                                                                  .storage = storage,
                                                                                  .func_def = f,
                                                                                  .type = f->type,
                                                                                  .scope_depth = p->current_scope_depth}));
}

void update_linkage_storage(Symbol *s, const Node *v) {
    ASSERT(v->kind == N_VAR_DECL, "Expected Var decl node to update symbol linkage and storage\n");

    Linkage linkage = LINK_NONE;
    Storage storage = STORAGE_NONE;
    if (v->var_decl.is_global) {
        storage = v->var_decl.is_defined ? STORAGE_DATA : STORAGE_BSS;
        linkage = v->var_decl.storage_class == STATIC ? LINK_INTERNAL : LINK_EXTERNAL;
    } else {
        // local variable
        storage = STORAGE_NONE;
        if (v->var_decl.storage_class == NONE) linkage = LINK_NONE;
        if (v->var_decl.storage_class == EXTERN) linkage = LINK_EXTERNAL;
        if (v->var_decl.storage_class == STATIC) linkage = LINK_INTERNAL;
    }
    s->linkage = linkage;
    s->storage = storage;
}
Symbol *p_append_var_decl_symbol(Parser *p, Node *v) {
    Symbol *s = p_new_symbol(p, &(Symbol){.name = v->var_decl.identifier->identifier.name,
                                          .kind = VAR,
                                          .var_decl = v,
                                          .type = v->type,
                                          .scope_depth = p->current_scope_depth});
    update_linkage_storage(s, v);
    return p_append_symbol(get_current_symbol_table(p), s);
}

Symbol *p_append_param_decl_symbol(Parser *p, const ParamDecl *param) {
    ASSERT(param->name, "Function parameter must be named\n");
    return p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &(Symbol){.name = param->name,
                                                                                  .kind = VAR,
                                                                                  .linkage = LINK_NONE,
                                                                                  .storage = STORAGE_NONE,
                                                                                  .var_decl = NULL,
                                                                                  .type = param->type,
                                                                                  .scope_depth = p->current_scope_depth}));
}
void p_append_enum_const(Parser *p, const EnumField *e) {
    p_append_symbol(get_current_symbol_table(p), p_new_symbol(p, &(Symbol){.name = e->name,
                                                                           .kind = ENUM,
                                                                           .linkage = LINK_NONE,
                                                                           .storage = STORAGE_NONE,
                                                                           .enum_field = e,
                                                                           .type = type_i32,
                                                                           .scope_depth = p->current_scope_depth}));
}

void p_append_element(Node *init_list, Node *element) { append(&init_list->init_list.elements_array, &element); }

Node *p_get_var_decl(const Parser *p, const char *name) {
    const Symbol *s = p_get_symbol(p, name, VAR, false);
    if (s) return s->var_decl;
    PANIC("Tried to find variable %s which does not exist\n", name);
}

const EnumField *p_get_enum_const(const Parser *p, const char *name) {
    const Symbol *s = p_get_symbol(p, name, ENUM, false);
    ASSERT(s, "Failed to find enum constant '%s'.\n", name);
    return s->enum_field;
}

Node *p_parse_if_statement(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_IF);
    p_consume_a(p, TK_IF); // -> if
    p_consume_a(p, TK_OPEN_PAREN);
    node->_if.cond = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_if.if_true = p_parse_statement(p, nm); //{[compound]} (in the future, can be a function call)
    if (p_peek(p)->type == TK_ELSE) {             // If there is an if, it can be a
        p_consume(p);                             // -> else
        if (p_peek(p)->type == TK_IF) node->_if.if_false = p_parse_if_statement(p, nm);
        else node->_if.if_false = p_parse_statement(p, nm);
    } else node->_if.if_false = NULL;
    return node;
}
Node *p_parse_do_while_loop(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_WHILE);
    node->_while.is_do_while = true;
    p_consume_a(p, TK_DO);
    node->_while.block = p_parse_statement(p, nm);
    p_consume_a(p, TK_WHILE);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_while.cond = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_CLOSE_PAREN);
    p_consume_semi(p);
    return node;
}

Node *p_parse_while_loop(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_WHILE);
    node->_while.is_do_while = false;
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
        node->_case.const_expr = p_parse_expression(p, nm, 0);
    } else {
        p_consume_a(p, TK_DEFAULT);
        node->_case.const_expr = NULL;
    }
    p_consume_a(p, TK_COLON);
    return node;
}

void p_append_case(Node *s, Node *c) {
    c->_case.i = s->_switch.block->compound.items_array.count;
    append(&s->_switch.cases_array, &c);
}

UnionMember *get_union_member_named(Type *union_t, const char *name) {
    for (int j = 0; j < union_t->_union.members_array.count; j++) {
        UnionMember *member = get_union_member(union_t, j);
        if (strcmp(member->name, name) == 0) {
            return member;
        }
    }
    PANIC("No such member '%s' in union %t\n", name, union_t);
}
StructMember *get_struct_member_named(Type *struct_t, const char *name, int *index) {
    for (int j = 0; j < struct_t->_struct.members_array.count; j++) {
        StructMember *member = get_struct_member(struct_t, j);
        if (member->name) {
            if (strcmp(member->name, name) == 0) {
                // Continue from after the named initializer.
                *index = j;
                return member;
            }
        } else {
            StructMember *m = get_member(member->type, name, 0, 0, 0);
            if (m) {
                *index = j;
                return m;
            }
        }
    }
    PANIC("No such member '%s' in struct %t", name, struct_t);
}

Node *p_parse_switch_statement(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_SWITCH);
    array_init(&node->_switch.cases_array, 4, sizeof(Node *));

    p_consume_a(p, TK_SWITCH);
    p_consume_a(p, TK_OPEN_PAREN);
    node->_switch.test = p_parse_expression(p, nm, 0);
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
    // Manually consume semicolons
    p->expect_semi = false;

    if (p_peek(p)->type != TK_SEMI) node->_for.init = p_parse_block_item(p, nm);
    p_consume_a(p, TK_SEMI);
    if (p_peek(p)->type != TK_SEMI) node->_for.cond = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    p_consume_a(p, TK_SEMI);
    if (p_peek(p)->type != TK_CLOSE_PAREN) node->_for.iter = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);

    p->expect_semi = true;
    p_consume_a(p, TK_CLOSE_PAREN);
    node->_for.block = p_parse_statement(p, nm);
    return node;
}

Node *p_parse_return(Parser *p, NodeManager *nm) {
    Node *node = new_node(nm, N_RETURN);
    p_consume(p); // -> return
    if (p_peek(p)->type != TK_SEMI) {
        node->_return.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        node->type = type_invalid;
    }
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
    case TK_DO:
        return p_parse_do_while_loop(p, nm);
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
    case TK_GOTO:
        return p_parse_goto_statement(p, nm);
    default:
        return p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
    }
}

void p_push_scope(Parser *p) {
    p->current_scope_depth++;
    p_append_symbol_table(p);
}
void p_pop_scope(Parser *p) {
    p->current_scope_depth--;
    array_free(get_current_symbol_table(p));
    pop(&p->scopes_array);
}

Node *p_parse_compound(Parser *p, NodeManager *nm) {
    Node *node = new_compound_node(nm);
    p_consume_a(p, TK_OPEN_CURLY);
    while (p_peek(p)->type != TK_CLOSE_CURLY && !p_is_last_token(p)) {
        p_append_block_item(node, p_parse_block_item(p, nm));
    }
    p_consume_a(p, TK_CLOSE_CURLY);
    return node;
}

Type *decay_array_type(Type *t) {
    if (t->kind == T_ARRAY) return get_pointer_type(decay_array_type(t->base));
    return t;
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
        return p_get_symbol(p, t->value, TYPEDEF, false) != NULL;
    default:
        return false;
    }
}

bool is_storage_classifier(const TokenType type) {
    switch (type) {
    case TK_STATIC:
    case TK_EXTERN:
        return true;
    default:
        return false;
    }
}

bool is_qualifier_token(const TokenType type) {
    switch (type) {
    case TK_CONST:
    case TK_VOLATILE:
        return true;
    default:
        return false;
    }
}

bool is_start_of_type(const Parser *p, const Token *tk) {
    return tk->type == TK_UNSIGNED || tk->type == TK_SIGNED || is_type_token(p, tk) || is_qualifier_token(tk->type);
}

Type *token_to_type(Parser *p, const Token *t) {
    switch (t->type) {
    case TK_CHAR:
        return type_i8;
    case TK_SHORT:
        return type_i16;
    case TK_INT:
        return type_i32;
    case TK_LONG:
#ifdef _WIN64
        if (p_peek(p)->type == TK_LONG) {
            p_consume(p);
            return type_i64;
        } else return type_i32;
#else
        return type_i64;
#endif
    case TK_FLOAT:
        return type_f32;
    case TK_DOUBLE:
        return type_f64;
    case TK_VOID:
        return type_void;
    case TK_IDENTIFIER:
        return p_get_typedef(p, t->value)->type;
    default:
        return type_invalid;
    }
}
Node *p_parse_function(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, const bool is_inline) {
    Node *node = new_function_node(nm);
    ASSERT(name, "p_parse_function received NULL function name\n");

    node->func.name = name;
    node->func.is_inline = is_inline;
    node->type = type;
    if (p_peek(p)->type == TK_SEMI) {
        p_consume(p);
        node->func.is_defined = false;
    } else node->func.is_defined = true;

    node->func.storage_class = storage_class;
    if (node->func.is_defined) node->func.body = p_parse_compound(p, nm);
    return node;
}

Modifier p_parse_parameter_list(Parser *p, NodeManager *nm) {
    Modifier mod = {.kind = MOD_FUNCTION, .function = {.is_variadic = false}};
#ifdef __COMPILER_C__
    mod.function.is_variadic = 0;
#endif
    array_init(&mod.function.params, 4, sizeof(ParamDecl));
    p_consume_a(p, TK_OPEN_PAREN);
    while (p_peek(p)->type != TK_CLOSE_PAREN && !p_is_last_token(p)) {
        if (p_peek(p)->type == TK_ELLIPSES) {
            p_consume(p);
            mod.function.is_variadic = true;
            break;
        }
        const char *name = NULL;
        append(&mod.function.params, &(ParamDecl){.type = decay_array_type(p_parse_type(p, nm, &name)), .name = name});
        if (p_peek(p)->type == TK_COMMA) p_consume(p);
        else break;
    }
    p_consume_a(p, TK_CLOSE_PAREN);
    return mod;
}

// Either function or type/var declaration
Node *p_parse_external_declaration(Parser *p, NodeManager *nm) {
    if (p_peek(p)->type == TK_TYPEDEF) return p_parse_typedef(p, nm);

    StorageClass storage_class = NONE;
    bool is_inline = false;
    for (;;) {
        if (p_peek(p)->type == TK_STATIC) storage_class = STATIC;
        else if (p_peek(p)->type == TK_EXTERN) storage_class = EXTERN;
        else if (p_peek(p)->type == TK_INLINE) is_inline = true;
        else break;
        p_consume(p);
    }

    const char *name = NULL;
    Type *type = p_parse_type(p, nm, &name);

    if (type->kind == T_FUNCTION) return p_parse_function(p, nm, type, name, storage_class, is_inline);
    else return p_parse_declaration(p, nm, type, name, storage_class, true);
}

Node *p_parse_block_declaration(Parser *p, NodeManager *nm) {
    if (p_peek(p)->type == TK_TYPEDEF) return p_parse_typedef(p, nm);

    const StorageClass storage_class = p_parse_storage_classifier(p, nm);

    const char *name = NULL;
    Type *type = p_parse_type(p, nm, &name);

    ASSERT(type->kind != T_FUNCTION, "Function prototypes within block scope is unsupported\n");
    return p_parse_declaration(p, nm, type, name, storage_class, false);
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
Node *p_parse_declaration(Parser *p, NodeManager *nm, Type *type, const char *name, const StorageClass storage_class, const bool global) {
    if (type->kind == T_STRUCT || type->kind == T_ENUM || type->kind == T_UNION) {
        if (name == NULL) {
            p_consume_semi(p); // trailing semi
            Node *type_decl = new_node(nm, N_TYPE);
            type_decl->type = type;
            return type_decl;
        }
        ASSERT(type->_struct.complete, "Cannot instantiate an incomplete type\n");
    }
    ASSERT(name != NULL, "Expected non null name of variable\n");
    Node *ident = new_node(nm, N_IDENTIFIER);
    ident->identifier.name = name;
    Node *var_decl = new_node(nm, N_VAR_DECL);
    var_decl->var_decl.identifier = ident;
    var_decl->var_decl.is_global = global;

    var_decl->type = type;

    if (p_peek(p)->type == TK_EQ) {
        p_consume(p); // =
        var_decl->var_decl.expr = p_parse_expression(p, nm, MIN_BINARY_OP_PRECEDENCE);
        var_decl->var_decl.is_defined = true;
    } else {
        // Forward declaration
        var_decl->var_decl.is_defined = false;
        var_decl->var_decl.expr = NULL;
    }
    var_decl->var_decl.storage_class = storage_class;
    p_consume_semi(p);

    return var_decl;
}
Node *p_parse_typedef(Parser *p, NodeManager *nm) {
    p_consume_a(p, TK_TYPEDEF);
    Node *node = new_node(nm, N_TYPEDEF);
    node->_typedef.name = NULL;
    node->type = p_parse_type(p, nm, &node->_typedef.name);
    ASSERT(node->_typedef.name, "Missing typedef name\n");
    p_consume_semi(p);
    p_append_typedef(p, &(Typedef){.type = node->type, .new_def = node->_typedef.name});
    return node;
}
Node *p_parse_translation_unit(Parser *p, NodeManager *nm) {
    Node *root = new_init_translation_unit_node(nm);
    ASSERT(p->size > 0, "Tried to generate AST with empty token array.");

    while (!p_is_last_token(p)) {
        Node *decl = p_parse_external_declaration(p, nm);
        append(&root->translation_unit.declarations_array, &decl);
    }
    return root;
}

StorageClass p_parse_storage_classifier(Parser *p, NodeManager *nm) {
    switch (p_peek(p)->type) {
    case TK_EXTERN:
        p_consume(p);
        return EXTERN;
    case TK_STATIC:
        p_consume(p);
        return STATIC;
    default:
        return NONE;
    }
}

/* ===== Parse Literals ===== */

Type *parse_int_suffix(const char *raw, int *len) {
    int i = *len - 1;
    int l_count = 0;
    bool is_unsigned = false;
    for (;;) {
        if (i <= 0) break;
        char c = raw[i];
        if (c == 'u' || c == 'U') is_unsigned = true;
        else if (c == 'l' || c == 'L') l_count++;
        else break;
        i--;
    }
    Type *type = type_i32;
    *len -= *len - i - 1;
    // in the future, l_count == 2 is i64 otherwise i32
    // when long long int is supported properly
    if (l_count > 0) type = type_i64;
    if (is_unsigned) return get_unsigned_type(type);
    return type;
}

Type *parse_float_suffix(const char *raw, int *len) {
    Type *type = type_f64;
    switch (raw[*len - 1]) {
    case 'f':
    case 'F':
        type = type_f32;
    case 'l':
    case 'L':
        *len -= 1;
    default:
        return type;
    }
}

int64_t parse_int(const char *raw, const int len) {
    ASSERT(len <= 20, "Cannot parse an integer larger than 64 bytes, character length of %d\n", len);
    if (raw[0] == '-') return -parse_int(raw + 1, len - 1);
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

int64_t parse_dec(const char *raw, const int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        res = res * 10 + (*raw - '0');
        raw++;
    }
    return res;
}
int64_t parse_binary(const char *raw, const int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        const int value = (*raw - '0');
        ASSERT(value == 0 || value == 1, "Parse Binary Failed: Non binary digit '%c'\n", *raw);
        res = res * 2 + value;
        raw++;
    }
    return res;
}
int64_t parse_oct(const char *raw, const int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        const int value = *raw - '0';
        ASSERT(value >= 0 && value <= 7, "Parse Octal Failed: Non octal digit '%c'\n", *raw);
        res = res * 8 + value;
        raw++;
    }
    return res;
}

int64_t parse_hex(const char *raw, const int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value;
        const char c = (*raw) | 0x20;
        if (is_num(c)) value = *raw - '0';
        else if (c >= 'a' && c <= 'f') value = c - 'a' + 10;
        else PANIC("Parse Hex Failed: Non hex digit '%c'\n", *raw);
        res = res * 16 + value;
        raw++;
    }
    return res;
}

int parse_multi_character(const char *raw, const int len) {
    int c = 0;
    for (int i = 0; i < len && i < 4; i++) {
        c |= (unsigned char)raw[i] << (i * 8);
    }
    return c;
}

double parse_float(const char *raw, const int len) {
    if (raw[0] == '-') return -parse_float(raw + 1, len - 1);
    double res = 0;
    const char *end = raw + len;
    double m = 0;
    while (raw < end) {
        if (*raw == '.') {
            m = 0.1;
            raw++;
            continue;
        }
        const int digit = *raw - '0';
        ASSERT(digit >= 0 && digit <= 9, "Parse Float Failed: Non decimal digit '%c'\n", *raw);
        if (m) {
            res += digit * m;
            m *= 0.1;
        } else res = res * 10.0 + digit;

        raw++;
    }

    return res;
}
