#include "compiler_c/analyse/sema.h"
#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/const_expr.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/tokenize/tokenizer.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_lvalue(const Node *n) { return n->kind == N_IDENTIFIER || n->kind == N_INDEX || n->kind == N_MEMBER_ACCESS || is_deref(n); }
bool is_deref(const Node *n) { return n->kind == N_UNARY && n->unary.op == TK_MULTIPLY; }

Type *check_unary_op(NodeManager *nm, Node *unary_op) {
    const Node *expr = unary_op->unary.expr;
    const TypeKind kind = expr->type->kind;
    switch (unary_op->unary.op) {
    // [Int, Float] => [Int, Float]
    case TK_PLUS:
    case TK_MINUS:
        if (kind == T_INT || kind == T_FLOAT) return expr->type;
        break;
    // [Int, Float] => Int
    case TK_L_NOT:
        if (kind == T_INT || kind == T_FLOAT) return type_i32;
        break;
    // Int => Int
    case TK_BW_NOT:
        if (kind == T_INT) return expr->type;
        break;
    case TK_AND:
        if (is_lvalue(expr)) {
            return get_pointer_type(expr->type);
        }
        PANIC("Tried to reference a non assignable term\n");
    case TK_MULTIPLY:
        if (expr->type->kind == T_ARRAY) {
            unary_op->unary.expr = cast_node(nm, unary_op->unary.expr, get_pointer_type(expr->type->base));
        }
        if (expr->type->base && expr->type->base != type_invalid) return expr->type->base;
        PANIC("Tried to dereference some nonexistent term\n");
    case TK_SIZEOF:
        if (expr->type != type_invalid && expr->type->size) return type_i32;
        PANIC("Tried to get the sizeof something without a size\n");
    case TK_INCR:
    case TK_DECR:
        return expr->type;
    default:
        break;
    }
    printf("Invalid operand type ");
    print_type(expr->type);
    printf("for the given unary operator ");
    print_token_type(unary_op->unary.op);
    printf("\n");

    return type_invalid;
}

bool is_valid_binary_op(TokenType op, const Node *lhs, const Node *rhs) {
    if (op == TK_EQ) return true;
    if (is_assignment_op(op)) op = get_underlying_op(op);
    bool is_lhs_ptr = lhs->type->kind == T_POINTER || lhs->type->kind == T_ARRAY;
    bool is_rhs_ptr = rhs->type->kind == T_POINTER || rhs->type->kind == T_ARRAY;
    if ((is_lhs_ptr || is_rhs_ptr) && is_bitwise_op(op)) return false;
    if (is_lhs_ptr && !is_rhs_ptr) return op == TK_PLUS || op == TK_MINUS || op == TK_EQ_EQ || op == TK_NEQ;
    if (!is_lhs_ptr && is_rhs_ptr) return op == TK_PLUS || op == TK_EQ_EQ || op == TK_NEQ;
    if (is_lhs_ptr && is_rhs_ptr) return op == TK_MINUS || is_comparison_op(op);
    return true;
}

Type *check_binary_op(NodeManager *nm, const TokenType op, Node *binop) {
    if (binop->binary.lhs->type == type_invalid || binop->binary.rhs->type == type_invalid) {
        PANIC("Binary op was given expression with an invalid type\n");
    }
    const Node *lhs = binop->binary.lhs;
    Node *rhs = binop->binary.rhs;
    // Only checks pointer arithmatic
    if (!is_valid_binary_op(op, lhs, rhs)) {
        PANIC("Invalid arithmetic operands\n");
    }
    if (is_assignment_op(op)) {
        if (!is_lvalue(lhs)) {
            PANIC("Analysis: Binary op lhs is not assignable\n");
        }
        const TokenType underlying = get_underlying_op(op);
        if (is_arithmetic_op(underlying) || is_bitwise_op(underlying)) {
            promote_binary_operands(nm, binop);
        }

        if (lhs->type->kind != rhs->type->kind) {
            binop->binary.rhs = cast_node(nm, rhs, lhs->type);
        }
        return lhs->type;
    }

    Type *common = promote_binary_operands(nm, binop);
    if (!common || common == type_invalid) {
        PANIC("Invalid arithmetic operands\n");
    }

    if (is_arithmetic_op(op)) return common;
    // Review this to ensure correctness
    if (is_comparison_op(op)) return common;

    // Review this to ensure correctness
    if (is_bitwise_op(op)) {
        if (lhs->type->kind != T_INT || rhs->type->kind != T_INT) {
            PANIC("Bitwise operation requires integers\n");
        }
        return type_i32;
    }

    if (is_logical_op(op)) return type_i32;

    printf("Unknown binary operator\n");
    return type_invalid;
}

Type *promote_binary_operands(NodeManager *nm, Node *binop) {
    Type *common = type_invalid;
    Node **lhs = &binop->binary.lhs;
    Node **rhs = &binop->binary.rhs;
    if ((*lhs)->type->kind == T_ENUM) {
        *lhs = cast_node(nm, (*lhs), type_i32);
    }
    if ((*rhs)->type->kind == T_ENUM) {
        *rhs = cast_node(nm, (*rhs), type_i32);
    }
    // Decay array -> pointer
    if ((*lhs)->type->kind == T_ARRAY) {
        *lhs = cast_node(nm, (*lhs), get_pointer_type((*lhs)->type->base));
    }
    if ((*rhs)->type->kind == T_ARRAY) {
        *rhs = cast_node(nm, (*rhs), get_pointer_type((*rhs)->type->base));
    }

    // Integer promotion
    if (is_arithmetic_op(binop->binary.op) || is_comparison_op(binop->binary.op)) {
        if ((*lhs)->type->kind == T_INT && (*lhs)->type->size < type_i32->size)
            *lhs = cast_node(nm, (*lhs), (*lhs)->type->is_signed ? type_i32 : type_u32);
        if ((*rhs)->type->kind == T_INT && (*rhs)->type->size < type_i32->size)
            *rhs = cast_node(nm, (*rhs), (*rhs)->type->is_signed ? type_i32 : type_u32);
    }

    if ((*lhs)->type->kind == T_FLOAT || (*rhs)->type->kind == T_FLOAT) {
        common = (*lhs)->type->size > (*rhs)->type->size ? (*lhs)->type : (*rhs)->type;
    } else if ((*lhs)->type->kind == T_POINTER && (*rhs)->type->kind == T_INT) {
        if ((*rhs)->type != type_i64) *rhs = cast_node(nm, (*rhs), type_i64);
        return (*lhs)->type;
    } else if ((*lhs)->type->kind == T_INT && (*rhs)->type->kind == T_POINTER) {
        if ((*lhs)->type != type_i64) *lhs = cast_node(nm, (*lhs), type_i64);
        return (*rhs)->type;
    } else if ((*lhs)->type->kind == T_INT && (*rhs)->type->kind == T_INT) {
        common = (*lhs)->type->size >= (*rhs)->type->size ? (*lhs)->type : (*rhs)->type;
    } else if ((*lhs)->type->kind == T_POINTER && (*rhs)->type->kind == T_POINTER) {
        return type_i64;
    } else {
        PANIC("UNSURE HOW TO HANDLE COMMON CASE;\n");
    }

    if ((*lhs)->type != common) *lhs = cast_node(nm, (*lhs), common);
    if ((*rhs)->type != common) *rhs = cast_node(nm, (*rhs), common);
    return common;
}

void lower_compound_literal(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Node *node) {
    Node *ident = new_node(nm, N_IDENTIFIER);
    // TODO track compound literals and name accordingly.
    ident->identifier.name = "__tmp_cl";
    ident->identifier.len = 9;
    Node *d = new_node(nm, N_VAR_DECL);
    d->var_decl.identifier = ident;
    d->type = node->type;

    d->var_decl.expr = node->compound_literal.value;
    d->var_decl.is_defined = true;
    d->var_decl.storage_class = STATIC;
    d->var_decl.is_global = false;
    d->var_decl.symbol = NULL;

    node->kind = N_IDENTIFIER;
    node->identifier.name = ident->identifier.name;
    node->identifier.len = ident->identifier.len;

    insert_node(&sema_ctx->compound->compound.items_array, &d, *get_i(sema_ctx));
    // Insert shifted all nodes over by one, so increment tracker too.
    (*get_i(sema_ctx))++;
}

void handle_builtin_call(BuiltinKind kind, Node *node) {
    Node builtin;
    builtin.kind = N_BUILTIN;
    builtin._builtin.kind = kind;
    builtin._builtin.params = node->func_call.params_array;
    memcpy(node, &builtin, sizeof(Node));
}

Type *resolve_type(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Type *t) {
    switch (t->kind) {
    case T_POINTER:
        return get_pointer_type(resolve_type(sema_ctx, p, nm, t->base));
    case T_ARRAY:
        Type *base = resolve_type(sema_ctx, p, nm, t->base);
        if (t->_array.is_complete) return get_array_type(base, t->_array.array_len);
        else {
            t->_array.is_complete = true;
            semantic_analysis(sema_ctx, p, nm, t->_array.const_expr);
            return get_array_type(base, t->_array.const_expr ? evaluate_const_expression(t->_array.const_expr).i : -1);
        }
    case T_STRUCT:
        for (int i = 0; i < t->_struct.members_array.count; i++) {
            StructMember *m = get_struct_member(t, i);
            m->type = resolve_type(sema_ctx, p, nm, m->type);
        }
        return t;
    case T_UNION:
        for (int i = 0; i < t->_union.members_array.count; i++) {
            UnionMember *u = get_union_member(t, i);
            u->type = resolve_type(sema_ctx, p, nm, u->type);
        }
        return t;
    case T_ENUM:
    case T_FUNCTION:
    case T_VOID:
    case T_INT:
    case T_FLOAT:
    case T_INVALID:
        return t;
    }
}

void semantic_analysis(SemanticContext *sema_ctx, Parser *p, NodeManager *nm, Node *node) {
    if (!node) return;
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        for (int i = 0; i < node->translation_unit.declarations_array.count; i++) {
            semantic_analysis(sema_ctx, p, nm, get_node(&node->translation_unit.declarations_array, i));
        }
        node->type = type_void;
        break;
    case N_FUNCTION:
        ASSERT(!(node->func.is_defined && node->func.storage_class == EXTERN), "External Function cannot have a definition\n");
        // Simulate a function params in symbol table
        if (node->func.is_defined) {
            p_push_scope(p);
            sema_ctx->func = node;
            for (int i = 0; i < node->type->_func.params.count; i++) {
                ParamDecl *param = (ParamDecl *)get(&node->type->_func.params, i);
                if (node->func.is_defined) {
                    ASSERT(param->name, "All Defined Function Paramaters must be named\n");
                    Node *param_decl = new_node(nm, N_VAR_DECL);
                    Node *ident = new_node(nm, N_IDENTIFIER);
                    ident->identifier.name = param->name;
                    param_decl->var_decl.identifier = ident;
                    param->symbol = p_append_var_decl_symbol(p, param_decl);
                    param_decl->var_decl.symbol = param->symbol;
                    param_decl->type = param->type;
                }
            }

            semantic_analysis(sema_ctx, p, nm, node->func.body);
            p_pop_scope(p);
        }

        Symbol *func_symbol = p_get_symbol(p, node->func.name, FUNC, false);
        if (func_symbol) {
            if (func_symbol->func_def->func.storage_class == STATIC && node->func.storage_class != STATIC) {
                PANIC("Linkage conflict between function declarations of %s\n", node->func.name);
            }
            // If previous declaration was prototype, and current has {}
            if (!func_symbol->func_def->func.is_defined && node->func.is_defined) {
                // Update func symbol to defined node
                func_symbol->func_def = node;
            } else if (node->func.is_defined && func_symbol->func_def->func.is_defined) {
                // If symbol and current both have {}
                PANIC("Redefinition of function %s\n", node->func.name);
            }
            node->func.symbol = func_symbol;
        } else node->func.symbol = p_append_func_def(p, node);
        break;
    case N_COMPOUND:
        push_sema_scope(sema_ctx, p, node);
        int n_nodes = node->compound.items_array.count;
        for (int i = 0; i < n_nodes; i++, (*get_i(sema_ctx))++) {
            semantic_analysis(sema_ctx, p, nm, get_node(&node->compound.items_array, *get_i(sema_ctx)));
        }
        pop_sema_scope(sema_ctx, p);
        break;
    case N_VAR_DECL:
        // Skip extern nodes
        // Resolve const expr array bounds for array types
        if (node->type->kind == T_ARRAY && !node->type->_array.is_complete) {
            semantic_analysis(sema_ctx, p, nm, node->type->_array.const_expr);
            node->type = resolve_type(sema_ctx, p, nm, node->type);
        }
        if (node->var_decl.storage_class == EXTERN) {
            if (node->var_decl.is_defined) {
                PANIC("External variable cannot be initialized in the same statement\n");
            }
        }
        Symbol *var_symbol = p_get_symbol(p, node->var_decl.identifier->identifier.name, VAR, true);
        // TODO consider if symbol management can happen after symantic analysis
        if (var_symbol) {
            // If we are within a function and var_symbol is a also a local variable
            if (p->scopes_array.count > 1) {
                if (var_symbol->scope_depth == p->current_scope_depth) {
                    PANIC("Redeclaration of local variable %s\n", node->var_decl.identifier->identifier.name);
                }
            } else if (!var_symbol->var_decl->var_decl.is_defined && node->var_decl.is_defined) {
                var_symbol->var_decl = node;
            } else if (var_symbol->var_decl->var_decl.is_defined && node->var_decl.is_defined) {
                PANIC("Redefinition of global variable %s\n", node->var_decl.identifier->identifier.name);
            }
            /*
                If the var decl is not in global scope, this symbol will be freed before it reaches IR,
                where it might be used (not currently as of writing). If I place a symbols in one
            */
        } else var_symbol = p_append_var_decl_symbol(p, node);

        node->var_decl.symbol = var_symbol;
        node->var_decl.identifier->identifier.symbol = var_symbol;

        if (!node->var_decl.expr) break;
        if (node->var_decl.expr->kind == N_INIT_LIST) {
            node->var_decl.expr->type = node->type;
            semantic_analysis(sema_ctx, p, nm, node->var_decl.expr);
            if (node->var_decl.is_global) {
                ConstLiteral init_list = evaluate_const_expression(node->var_decl.expr);
                node->var_decl.const_expr = malloc(sizeof(ConstLiteral));
                ASSERT(node->var_decl.const_expr, "Failed to allocate for const expr");
                *node->var_decl.const_expr = init_list;
            }
            Node *init_list = node->var_decl.expr;
            break;
        }
        semantic_analysis(sema_ctx, p, nm, node->var_decl.expr);
        if (node->var_decl.expr->kind == N_LITERAL && node->var_decl.expr->literal.kind == L_STRING) {
            if (node->var_decl.expr->literal.kind == L_STRING) {
                // TODO allow char* str = ""
                if (node->type->kind != T_ARRAY && node->type->base == type_i8) {
                    log_start(LOG_ERROR);
                    printf("Cannot initialize ");
                    print_type(node->type);
                    printf(" with String Literal\n");
                    exit(1);
                }
                // Infer array length
                if (node->type->kind == T_ARRAY && node->type->_array.array_len == -1) {
                    node->type = node->var_decl.expr->type;
                    node->var_decl.symbol->type = node->type;
                }
            }
        }
        if (node->var_decl.expr->type != node->type) {
            node->var_decl.expr = cast_node(nm, node->var_decl.expr, node->type);
        }

        if (node->var_decl.is_global) {
            ConstLiteral val = evaluate_const_expression(node->var_decl.expr);
            // TODO move this into a assign const_expr function:
            node->var_decl.const_expr = malloc(sizeof(ConstLiteral));
            ASSERT(node->var_decl.const_expr, "Failed to allocate for const expr");
            *node->var_decl.const_expr = val;
        }
        break;
    case N_UNARY:
        semantic_analysis(sema_ctx, p, nm, node->unary.expr);
        node->type = check_unary_op(nm, node);
        break;
    case N_BINARY:
        semantic_analysis(sema_ctx, p, nm, node->binary.lhs);
        semantic_analysis(sema_ctx, p, nm, node->binary.rhs);
        node->binary.common_type = check_binary_op(nm, node->binary.op, node);
        node->type = is_comparison_op(node->binary.op) ? type_i32 : node->binary.common_type;
        break;
    case N_TERNARY:
        semantic_analysis(sema_ctx, p, nm, node->ternary.cond);
        semantic_analysis(sema_ctx, p, nm, node->ternary.if_true);
        semantic_analysis(sema_ctx, p, nm, node->ternary.if_false);
        node->type = node->ternary.if_true->type;
        break;
    case N_CAST:
        semantic_analysis(sema_ctx, p, nm, node->cast.expr);
        if (is_valid_cast(node->cast.expr->type, node->cast.to)) {
            node->cast.from = node->cast.expr->type;
            node->type = node->cast.to;
            break;
        }
        log_start(LOG_ERROR);
        printf("Invalid cast from ");
        print_type(node->cast.expr->type);
        printf(" to ");
        print_type(node->cast.to);
        printf("\n");
        exit(1);
    case N_FUNCTION_CALL:
        const char *fn_name = node->func_call.callee->kind == N_IDENTIFIER ? node->func_call.callee->identifier.name : "";
        BuiltinKind builtin = get_builtin_kind(fn_name);
        if (builtin != BUILTIN_NONE) {
            handle_builtin_call(builtin, node);
            return semantic_analysis(sema_ctx, p, nm, node);
        }

        semantic_analysis(sema_ctx, p, nm, node->func_call.callee);
        Type *callee_type = node->func_call.callee->type;

        // if (callee_type->kind == T_FUNCTION) {
        //     callee_type = get_pointer_type(callee_type);
        //     node->func_call.callee->type = callee_type;
        // }
        ASSERT(callee_type->kind == T_FUNCTION || (callee_type->kind == T_POINTER && callee_type->base->kind == T_FUNCTION),
               "Cannot call non function type\n");

        Type *fn_type = callee_type->kind == T_FUNCTION ? callee_type : callee_type->base;
        if (!fn_type->_func.is_variadic && fn_type->_func.params.count != node->func_call.params_array.count) {
            print_type(fn_type);
            printf("\n");
            PANIC("Argument count mismatch: Function %s expects %d found %d\n", fn_name, fn_type->_func.params.count,
                  node->func_call.params_array.count);
        }
        // TODO handle variadic with no named paramter here instead of parser.
        node->type = fn_type->_func.return_type;
        for (int i = 0; i < node->func_call.params_array.count; i++) {
            Node *fn_call_param = get_node(&node->func_call.params_array, i);
            semantic_analysis(sema_ctx, p, nm, fn_call_param);
            // Only type check named params, skip variadic params.

            // Always downcast arrays to pointers for functions
            if (fn_call_param->type->kind == T_ARRAY) {
                Node *casted_node = cast_node(nm, fn_call_param, get_pointer_type(fn_call_param->type->base));
                set_node(&node->func_call.params_array, &casted_node, i);
            }
            if (i < fn_type->_func.params.count) {
                ParamDecl *fn_param = get(&fn_type->_func.params, i);
                if (fn_param->type != fn_call_param->type) {
                    Node *casted_node = cast_node(nm, fn_call_param, fn_param->type);
                    set_node(&node->func_call.params_array, &casted_node, i);
                }
            } else {
                // Promote Variadic args [T_INT < int -> int] [T_FLOAT < double -> double]
                if (fn_call_param->type->kind == T_INT && fn_call_param->type->size < type_i32->size) {
                    Node *casted_node = cast_node(nm, fn_call_param, fn_call_param->type->is_signed ? type_i32 : type_u32);
                    set_node(&node->func_call.params_array, &casted_node, i);
                } else if (fn_call_param->type->kind == T_FLOAT && fn_call_param->type->size < type_f64->size) {
                    Node *casted_node = cast_node(nm, fn_call_param, type_f64);
                    set_node(&node->func_call.params_array, &casted_node, i);
                }
            }
        }
        break;
    case N_IDENTIFIER:
        Symbol *ident_symbol = p_get_symbol(p, node->identifier.name, ANY, false);
        if (!ident_symbol) {
            PANIC("Failed to find symbol %s\n", node->identifier.name);
        }
        node->identifier.symbol = ident_symbol;
        switch (ident_symbol->kind) {
        case ENUM:
            node->kind = N_LITERAL;
            node->literal.kind = L_INT;
            node->literal.i = (int64_t)ident_symbol->enum_field.value;
            node->type = ident_symbol->enum_field._enum_t;
            break;
        case VAR:
            node->type = ident_symbol->var_decl->type;
            break;
        case TYPEDEF:
            // Maybe reference an N_TYPE node instead
            node->type = ident_symbol->_typedef.type;
            break;
        case FUNC:
            node->type = ident_symbol->func_def->type;
            break;
        case ANY:
            PANIC("Should be unreachable\n");
        }
        break;
    case N_IF:
        p_push_scope(p);
        semantic_analysis(sema_ctx, p, nm, node->_if.cond);
        if (node->_if.cond->type != type_i32) {
            node->_if.cond = cast_node(nm, node->_if.cond, type_i32);
        }
        semantic_analysis(sema_ctx, p, nm, node->_if.if_true);
        semantic_analysis(sema_ctx, p, nm, node->_if.if_false);
        p_pop_scope(p);
        break;
    case N_WHILE:
        p_push_scope(p);
        sema_ctx->loop = node;
        semantic_analysis(sema_ctx, p, nm, node->_while.cond);
        if (node->_while.cond->type != type_i32) {
            node->_while.cond = cast_node(nm, node->_while.cond, type_i32);
        }
        semantic_analysis(sema_ctx, p, nm, node->_while.block);
        p_pop_scope(p);
        break;
    case N_FOR:
        p_push_scope(p);
        sema_ctx->loop = node;
        semantic_analysis(sema_ctx, p, nm, node->_for.init);
        semantic_analysis(sema_ctx, p, nm, node->_for.cond);
        if (node->_for.cond->type != type_i32) {
            node->_for.cond = cast_node(nm, node->_for.cond, type_i32);
        }
        semantic_analysis(sema_ctx, p, nm, node->_for.iter);
        semantic_analysis(sema_ctx, p, nm, node->_for.block);
        p_pop_scope(p);
        break;
    case N_RETURN:
        if (!sema_ctx->func) {
            PANIC("Cannot call return outside of a function\n");
        }
        Type *expected_type = sema_ctx->func->type->_func.return_type;
        // Early exit if return type is void, and node is `return;`
        if (expected_type != type_void)
            ASSERT(node->_return.expr, "Non-void type function '%s' should return a value\n", sema_ctx->func->func.name);

        if (node->_return.expr) {
            semantic_analysis(sema_ctx, p, nm, node->_return.expr);
            Type *return_type = node->_return.expr->type;
            if (node->_return.expr->type != expected_type) {
                node->_return.expr = cast_node(nm, node->_return.expr, expected_type);
            }
        }
        node->type = expected_type;
        break;
    case N_LITERAL:
        char *data = malloc(node->literal.len + 1);
        if (!data) {
            PANIC("Failed to allocate for sema literal analysis\n");
        }
        memcpy(data, node->literal.raw_rata, node->literal.len);
        data[node->literal.len] = '\0';
        switch (node->literal.kind) {
        case L_INT:
            node->type = parse_int_suffix(node->literal.raw_rata, &node->literal.len);
            node->literal.i = parse_int(data, node->literal.len);
            free(data);
            break;
        case L_FLOAT:
            node->type = parse_float_suffix(node->literal.raw_rata, &node->literal.len);
            node->literal.f = parse_float(data, node->literal.len);
            free(data);
            break;
        case L_CHAR:
            node->type = type_u32;
            node->literal.i = parse_multi_character(data, node->literal.len);
            free(data);
            break;
        case L_STRING:
            node->type = get_array_type(type_i8, node->literal.len + 1);
            node->literal.s.data = data;
            node->literal.s.len = node->literal.len + 1;
            break;
        }
        break;
    case N_INDEX:
        semantic_analysis(sema_ctx, p, nm, node->index.index);
        semantic_analysis(sema_ctx, p, nm, node->index.identifier);
        if (node->index.index->type != type_i64) {
            node->index.index = cast_node(nm, node->index.index, type_i64);
        }
        if (node->index.identifier->type->kind != T_POINTER) {
            Type *pointer_type = get_pointer_type(node->index.identifier->type->base);
            node->index.identifier = cast_node(nm, node->index.identifier, pointer_type);
        }
        node->type = node->index.identifier->type->base;
        break;
    case N_CONTINUE:
        if (!sema_ctx->loop) {
            PANIC("Cannot call continue outside of a loop\n");
        }
        node->_continue.loop = sema_ctx->loop;
        break;
    case N_BREAK:
        if (!sema_ctx->loop) {
            PANIC("Cannot call break outside of a loop or switch statement\n");
        }
        node->_break.loop = sema_ctx->loop;
        break;
    case N_INIT_LIST:
        if (node->type == type_invalid) {
            PANIC("Semantic Analysis recieved an untyped initializer list\n");
        }
        switch (node->type->kind) {
        case T_INT:
        case T_FLOAT:
        case T_POINTER:
        case T_UNION:
            if (node->init_list.elements_array.count == 0) break;
            if (node->init_list.elements_array.count > 1) {
                log_start(LOG_ERROR);
                printf("Excess elements in initializer list for ");
                print_type(node->type);
                printf("\n");
                exit(1);
            }

            Node *e = get_node(&node->init_list.elements_array, 0);
            Type *target_type = node->type->kind == T_UNION ? get_union_member(node->type, 0)->type : node->type;
            Node *value = e;
            if (e->kind == N_DESIGNATED_INITIALIZER) {
                if (node->type->kind != T_UNION) {
                    log_start(LOG_ERROR);
                    printf("Cannot use designated initializers for type ");
                    print_type(node->type);
                    printf("\n");
                    exit(1);
                }
                UnionMember *member = get_union_member_named(node->type, e->designated_init._union.name);
                target_type = member->type;
                e->type = target_type;
                e->designated_init._union.member = member;
                value = e->designated_init.value;
            }
            semantic_analysis(sema_ctx, p, nm, value);

            if (value->type != target_type) {
                Node *casted_node = cast_node(nm, value, target_type);
                set_node(&node->init_list.elements_array, &casted_node, 0);
            }
            break;
        case T_ARRAY:
            // Infer the size from the initializer list
            if (node->type->_array.array_len == -1) {
                if (!node || node->init_list.elements_array.count < 1) {
                    PANIC("Inferred array must be initialized, and cannot be empty.\n");
                }
                node->type = infer_array_length(node->type, node->init_list.elements_array.count);
            }
            // Intentional passthrough in switch to handle T_ARRAY and T_STRUCT similarily
        case T_STRUCT:
            int index = 0;
            bool is_array = node->type->kind == T_ARRAY;
            int count = is_array ? node->type->_array.array_len : node->type->_struct.members_array.count;
            for (int i = 0; i < node->init_list.elements_array.count; i++) {
                Node *e = get_node(&node->init_list.elements_array, i);
                bool is_designator = e->kind == N_DESIGNATED_INITIALIZER;
                if (index >= count && !is_designator) {
                    log_start(LOG_ERROR);
                    printf("Too many initializers for ");
                    print_type(node->type);
                    printf("\n");
                    exit(1);
                }

                StructMember *member = NULL;
                if (is_array) {
                    if (is_designator) {
                        if (!e->designated_init._array.is_complete) {
                            semantic_analysis(sema_ctx, p, nm, e->designated_init._array.const_expr);
                            e->designated_init._array.index = evaluate_const_expression(e->designated_init._array.const_expr).i;
                        }
                        index = e->designated_init._array.index;
                    }
                } else
                    member = is_designator ? get_struct_member_named(node->type, e->designated_init._struct.name, &index)
                                           : get_struct_member(node->type, index);

                Type *target_type = is_array ? node->type->base : member->type;
                Node *value = is_designator ? e->designated_init.value : e;

                if (value->kind == N_INIT_LIST) value->type = target_type;

                semantic_analysis(sema_ctx, p, nm, value);
                if (!is_designator) {
                    Node *de = new_node(nm, N_DESIGNATED_INITIALIZER);

                    if (is_array) de->designated_init._array.index = index;
                    else de->designated_init._struct.name = member->name;

                    de->designated_init.value = e;
                    set_node(&node->init_list.elements_array, &de, i);
                    e = de;
                }
                e->designated_init.kind = is_array ? T_ARRAY : T_STRUCT;
                e->type = target_type;

                if (is_array) e->designated_init._array.index = index;
                else e->designated_init._struct.member = member;
                if (value->type != target_type) e->designated_init.value = cast_node(nm, value, target_type);

                index++;
            }
            break;
        default:
            log_start(LOG_ERROR);
            printf("Tried to assign initializer list to unsupported type ");
            print_type(node->type);
            printf("\n");
            exit(1);
        }
        break;
    case N_MEMBER_ACCESS:
        semantic_analysis(sema_ctx, p, nm, node->member_access.identifier);
        Type *lhs_t = node->member_access.identifier->type;
        if (node->member_access.op == TK_ARROW) {
            if (lhs_t->kind != T_POINTER) {
                PANIC("Dereference '->' can only be used on pointers\n");
            }
            lhs_t = lhs_t->base;
            Node *deref = new_node(nm, N_UNARY);
            deref->unary.op = TK_MULTIPLY;
            deref->unary.associativity = RIGHT_ASSOCIATIVITY;
            deref->unary.expr = node->member_access.identifier;
            deref->type = lhs_t;
            node->member_access.identifier = deref;
            node->member_access.op = TK_DOT;
        }
        AggrMember *member_f = get_member(lhs_t, node->member_access.member->identifier.name, true);
        node->member_access.member->type = member_f->type;
        node->member_access.offset = lhs_t->kind == T_UNION ? 0 : member_f->offset;
        node->type = member_f->type;

        break;
    case N_SWITCH:
        p_push_scope(p);
        sema_ctx->loop = node;
        semantic_analysis(sema_ctx, p, nm, node->_switch.test);
        if (node->_switch.test->type != type_i32) node->_switch.test = cast_node(nm, node->_switch.test, type_i32);
        semantic_analysis(sema_ctx, p, nm, node->_switch.block);
        node->type = node->_switch.test->type;
        p_pop_scope(p);
        break;
    case N_CASE:
        semantic_analysis(sema_ctx, p, nm, node->_case.const_expr);
        if (!node->_case.const_expr) break;
        if (!(node->_case.const_expr->type->kind == T_INT || node->_case.const_expr->type->kind == T_ENUM)) {
            PANIC("Not ready to handle non int test cases\n");
        }
        node->_case.test = evaluate_const_expression(node->_case.const_expr).i;
        break;
    case N_COMPOUND_LITERAL:
        node->compound_literal.value->type = node->type;
        semantic_analysis(sema_ctx, p, nm, node->compound_literal.value);
        lower_compound_literal(sema_ctx, p, nm, node);
        break;
    case N_TYPEDEF:
    case N_TYPE:
        // Add enum consts to symbol table
        if (node->type->kind == T_ENUM) {
            int64_t value = 0;
            for (int i = 0; i < node->type->_enum.fields_array.count; i++) {
                EnumField *f = get_enum_field(node->type, i);
                if (f->const_expr) {
                    semantic_analysis(sema_ctx, p, nm, f->const_expr);
                    value = evaluate_const_expression(f->const_expr).i;
                }
                f->value = value++;
                p_append_enum_const(p, f);
            }
        }
        break;
    case N_BUILTIN:
        switch (node->_builtin.kind) {
        case BUILTIN_VA_START:
            ASSERT(node->_builtin.params.count == 2, "%s expects 2 arguments\n", builtin_names[node->_builtin.kind]);
            Node *dst_ap = get_node(&node->_builtin.params, 0);
            Node *last_named_param = get_node(&node->_builtin.params, 1);
            semantic_analysis(sema_ctx, p, nm, dst_ap);
            semantic_analysis(sema_ctx, p, nm, last_named_param);
            ASSERT(last_named_param->kind == N_IDENTIFIER, "Last named param must be an identifier.\n");
            ASSERT(is_va_list_type(dst_ap->type), "%s expects va_list as first arg.\n", builtin_names[node->_builtin.kind]);
            node->type = type_void;
            break;
        case BUILTIN_VA_ARG:
            ASSERT(node->_builtin.params.count == 2, "%s expects 2 arguments\n", builtin_names[node->_builtin.kind]);
            dst_ap = get_node(&node->_builtin.params, 0);
            Node *type_info = get_node(&node->_builtin.params, 1);
            semantic_analysis(sema_ctx, p, nm, dst_ap);
            semantic_analysis(sema_ctx, p, nm, type_info);
            ASSERT(is_va_list_type(dst_ap->type), "%s expects va_list as first arg.\n", builtin_names[node->_builtin.kind]);
            ASSERT(type_info->kind == N_TYPE && type_info->type != type_invalid, "%s expects a type as second arg.",
                   builtin_names[node->_builtin.kind]);
            node->type = type_info->type;
            break;
        case BUILTIN_VA_END:
            ASSERT(node->_builtin.params.count == 1, "%s expects 1 arguments\n", builtin_names[node->_builtin.kind]);
            dst_ap = get_node(&node->_builtin.params, 0);
            semantic_analysis(sema_ctx, p, nm, dst_ap);
            ASSERT(is_va_list_type(dst_ap->type), "%s expects va_list as first arg.\n", builtin_names[node->_builtin.kind]);
            node->type = type_void;
            break;
        case BUILTIN_MEMCPY:
            ASSERT(node->_builtin.params.count == 3, "%s expects 3 arguments\n", builtin_names[node->_builtin.kind]);
            Node *memcpy_dst = get_node(&node->_builtin.params, 0);
            Node *memcpy_src = get_node(&node->_builtin.params, 1);
            Node *memcpy_size = get_node(&node->_builtin.params, 2);
            semantic_analysis(sema_ctx, p, nm, memcpy_dst);
            semantic_analysis(sema_ctx, p, nm, memcpy_src);
            semantic_analysis(sema_ctx, p, nm, memcpy_size);
            ASSERT(memcpy_dst->type->kind == T_POINTER && memcpy_src->type->kind, "%s expects both src and dst to be pointers\n",
                   builtin_names[node->_builtin.kind]);
            ASSERT(memcpy_size->type->kind == T_INT, "%s expects size to be integer or size_t\n");
            node->type = type_u64;
            break;
        case BUILTIN_NONE:
            PANIC("given __builtin_none but BUILTIN?\n");
        }
        break;
    case N_GOTO:
    case N_LABEL:
    case N_DESIGNATED_INITIALIZER:
        PANIC("Unreachable\n");
    }
}
void push_sema_scope(SemanticContext *sema_ctx, Parser *p, Node *n) {
    p_push_scope(p);
    sema_ctx->compound = n;
    int tmp = 0;
    append(&sema_ctx->i_array, &tmp);
}
void pop_sema_scope(SemanticContext *sema_ctx, Parser *p) {
    pop(&sema_ctx->i_array);
    sema_ctx->compound = NULL;
    p_pop_scope(p);
}

void lower_nodes(NodeManager *nm) {
    for (int i = 0; i < nm->count; i++) {
        Node *n = arena_get(nm, i);
        if (n->type->kind == T_ENUM) {
            n->type = type_i32;
        }
        if (n->kind == N_CAST) {
            if (n->cast.from && n->cast.from->kind == T_ENUM) {
                n->cast.from = type_i32;
            }
            if (n->cast.to && n->cast.to->kind == T_ENUM) {
                n->cast.to = type_i32;
                n->type = type_i32;
            }
            // Optimize out no op (cast from=x, to=x)
            if (n->cast.from == n->type) {
                *n = *n->cast.expr;
            }
        }
    }
}
