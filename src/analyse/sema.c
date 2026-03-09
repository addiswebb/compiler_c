#include "compiler_c/analyse/sema.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/parse/parser.h"

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
        printf("Tried to reference a non assignable term\n");
        exit(1);
    case TK_MULTIPLY:
        if (expr->type->kind == T_ARRAY) {
            unary_op->unary.expr = cast_node(nm, unary_op->unary.expr, get_pointer_type(expr->type->base));
        }
        if (expr->type->base && expr->type->base != type_invalid) return expr->type->base;
        printf("Tried to dereference some nonexistent term\n");
        exit(1);
    case TK_SIZEOF:
        if (expr->type != type_invalid && expr->type->size) return type_i32;
        printf("Tried to get the sizeof something without a size\n");
        exit(1);
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

Type *check_binary_op(NodeManager *nm, const TokenType op, Node *binop) {
    if (binop->binary.lhs->type == type_invalid || binop->binary.rhs->type == type_invalid) {
        printf("Semantic Analysis: Binary op was given expression with an invalid type\n");
        exit(1);
    }
    const Node *lhs = binop->binary.lhs;
    Node *rhs = binop->binary.rhs;
    if (is_assignment_op(op)) {
        if (!is_lvalue(lhs)) {
            printf("Semantic Analysis: Binary op lhs is not assignable\n");
            exit(1);
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
        printf("Invalid arithmetic operands");
        exit(1);
    }

    if (is_arithmetic_op(op)) return common;
    if (is_comparison_op(op)) return type_i32;

    if (is_bitwise_op(op)) {
        if (lhs->type->kind != T_INT || rhs->type->kind != T_INT) {
            printf("Bitwise operation requires integers\n");
            exit(1);
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
    if (is_arithmetic_op(binop->binary.op)) {
        if ((*lhs)->type->kind == T_INT && (*lhs)->type->size < type_i32->size) *lhs = cast_node(nm, (*lhs), type_i32);
        if ((*rhs)->type->kind == T_INT && (*rhs)->type->size < type_i32->size) *rhs = cast_node(nm, (*rhs), type_i32);
    }
    // Check for pointer - pointer, only allowed binop with two pointers
    if ((*lhs)->type == (*rhs)->type) return (*lhs)->type;

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
    } else {
        printf("UNSURE HOW TO HANDLE COMMON CASE;\n");
        exit(1);
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
    Node *d_type = new_node(nm, N_TYPE);
    d_type->type = node->type;
    Node *d = new_node(nm, N_VAR_DECL);
    d->var_decl.type = d_type;
    d->var_decl.identifier = ident;
    d->type = node->type;

    d->var_decl.expr = node->compound_literal.value;
    d->var_decl.has_initializer = true;
    d->var_decl.is_defined = true;
    d->var_decl.storage_class = STATIC;
    d->var_decl.is_global = false;
    // d->var_decl.symbol = p_append_var_decl(p, d);
    d->var_decl.symbol = NULL;

    node->kind = N_IDENTIFIER;
    node->identifier.name = ident->identifier.name;
    node->identifier.len = ident->identifier.len;

    insert_node(&sema_ctx->compound->compound.items_array, &d, *get_i(sema_ctx));
    // Insert shifted all nodes over by one, so increment tracker too.
    (*get_i(sema_ctx))++;
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
        p_push_scope(p);
        sema_ctx->func = node;
        for (int i = 0; i < node->func.params_array.count; i++) {
            p_append_var_decl(p, get_node(&node->func.params_array, i));
        }
        semantic_analysis(sema_ctx, p, nm, node->func.body);
        p_pop_scope(p);
        if (node->func.storage_class == EXTERN) {
            if (node->func.is_defined) {
                printf("External Function cannot have a definition\n");
                exit(1);
            }
        }
        Symbol *func_symbol = p_get_symbol(p, node->func.name, FUNC);
        if (func_symbol) {
            if (func_symbol->func_def->func.storage_class == STATIC && node->func.storage_class != STATIC) {
                printf("Linkage conflict between function declarations of %s\n", node->func.name);
                exit(1);
            }
            // If previous declaration was prototype, and current has {}
            if (!func_symbol->func_def->func.is_defined && node->func.is_defined) {
                // Update func symbol to defined node
                func_symbol->func_def = node;
            } else if (node->func.is_defined && func_symbol->func_def->func.is_defined) {
                // If symbol and current both have {}
                printf("Redefinition of function %s\n", node->func.name);
                exit(1);
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
        if (node->var_decl.storage_class == EXTERN) {
            if (node->var_decl.has_initializer) {
                printf("External variable cannot be initialized in the same statement\n");
                exit(1);
            }
        }
        Symbol *var_symbol = p_get_symbol(p, node->var_decl.identifier->identifier.name, VAR);
        // TODO consider if symbol management can happen after symantic analysis
        if (var_symbol) {
            // If we are within a function and var_symbol is a also a local variable
            if (p->scopes_array.count > 1) {
                if (var_symbol->scope_depth == p->scopes_array.count - 1) {
                    printf("Redeclaration of local variable %s\n", node->var_decl.identifier->identifier.name);
                    exit(1);
                }
            } else if (!var_symbol->var_decl->var_decl.is_defined && node->var_decl.is_defined) {
                var_symbol->var_decl = node;
            } else if (var_symbol->var_decl->var_decl.is_defined && node->var_decl.is_defined) {
                printf("Redefinition of global variable %s\n", node->var_decl.identifier->identifier.name);
                exit(1);
            }
            node->var_decl.symbol = var_symbol;
        } else node->var_decl.symbol = p_append_var_decl(p, node);

        if (!node->var_decl.expr) break;
        if (node->var_decl.expr->kind == N_INIT_LIST) {
            node->var_decl.expr->type = node->type;
            semantic_analysis(sema_ctx, p, nm, node->var_decl.expr);
            Node *init_list = node->var_decl.expr;
            break;
        }
        semantic_analysis(sema_ctx, p, nm, node->var_decl.expr);
        if (node->var_decl.expr->kind == N_LITERAL && node->var_decl.expr->literal.kind == L_STRING) {
            if (node->var_decl.expr->literal.kind == L_STRING) {
                if (node->type->kind != T_ARRAY && node->type->base == type_i8) {
                    printf("Cannot initialize ");
                    print_type(node->type);
                    printf(" with String Literal\n");
                    exit(1);
                }
                // Infer array length
                if (node->type->_array.array_len == -1) node->type = node->var_decl.expr->type;
            }
        }
        if (node->var_decl.expr->type != node->type) {
            node->var_decl.expr = cast_node(nm, node->var_decl.expr, node->type);
        }

        break;
    case N_UNARY:
        semantic_analysis(sema_ctx, p, nm, node->unary.expr);
        node->type = check_unary_op(nm, node);
        break;
    case N_BINARY:
        semantic_analysis(sema_ctx, p, nm, node->binary.lhs);
        semantic_analysis(sema_ctx, p, nm, node->binary.rhs);
        node->type = check_binary_op(nm, node->binary.op, node);
        break;
    case N_CAST:
        semantic_analysis(sema_ctx, p, nm, node->cast.expr);
        if (is_valid_cast(node->cast.expr->type, node->cast.to)) {
            node->cast.from = node->cast.expr->type;
            node->type = node->cast.to;
            break;
        }
        printf("Invalid cast from ");
        print_type(node->cast.expr->type);
        printf(" to ");
        print_type(node->cast.to);
        printf("\n");
        exit(1);
    case N_FUNCTION_CALL:
        const Node *func_def = p_get_func_def(p, node->func_call.identifier->identifier.name);
        if (!func_def->func.is_variadic && func_def->func.params_array.count != node->func_call.params_array.count) {
            printf("Argument count mismatch: %s expects %d found %d\n", func_def->func.name, func_def->func.params_array.count,
                   node->func_call.params_array.count);
            exit(1);
        }
        // TODO handle variadic with no named paramter here instead of parser.
        node->type = func_def->type;
        for (int i = 0; i < node->func_call.params_array.count; i++) {
            Node *func_call_ptr = get_node(&node->func_call.params_array, i);
            semantic_analysis(sema_ctx, p, nm, func_call_ptr);
            // Only type check named params, skip variadic params.
            if (i < func_def->func.params_array.count) {
                Node *func_param = get_node(&func_def->func.params_array, i);
                if (func_param->type != func_call_ptr->type) {
                    Node *casted_node = cast_node(nm, func_call_ptr, func_param->type);
                    set_node(&node->func_call.params_array, &casted_node, i);
                }
            }
            // Always downcast arrays to pointers for functions
            if (func_call_ptr->type->kind == T_ARRAY) {
                Node *casted_node = cast_node(nm, func_call_ptr, get_pointer_type(func_call_ptr->type->base));
                set_node(&node->func_call.params_array, &casted_node, i);
            }
        }
        break;
    case N_IDENTIFIER:
        Symbol *ident_symbol = p_get_symbol(p, node->identifier.name, ANY);
        if (!ident_symbol) {
            printf("Failed to find symbol %s\n", node->identifier.name);
            exit(1);
        }
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
        case ANY:
            printf("Should be unreachable\n");
            exit(1);
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
            printf("Cannot call return outside of a function\n");
            exit(1);
        }
        Type *return_type = sema_ctx->func->func.type->type;
        // Early exit if return type is void, and node is `return;`
        if (node->_return.expr) {
            semantic_analysis(sema_ctx, p, nm, node->_return.expr);
            if (node->_return.expr->type != return_type) {
                node->_return.expr = cast_node(nm, node->_return.expr, return_type);
            }
        } else {
            if (return_type != type_void) {
                printf("Non-void type function \'%s\' should return a value\n", sema_ctx->func->func.name);
                exit(1);
            }
        }
        sema_ctx->func = NULL;
        break;
    case N_LITERAL:
        char *data = malloc(node->literal.len + 1);
        if (!data) {
            printf("Failed to allocate for sema literal analysis\n");
            exit(1);
        }
        memcpy(data, node->literal.raw_rata, node->literal.len);
        data[node->literal.len] = '\0';
        switch (node->literal.kind) {
        case L_INT:
            node->type = type_i32;
            node->literal.i = parse_int(data, node->literal.len);
            free(data);
            break;
        case L_FLOAT:
            const bool is_float = node->literal.raw_rata[node->literal.len - 1] == 'f';
            node->type = is_float ? type_f32 : type_f64;
            node->literal.f = parse_float(data, node->literal.len);
            free(data);
            break;
        case L_CHAR:
            node->type = type_i8;
            node->literal.c = node->literal.raw_rata[0];
            if (node->literal.len != 1) {
                printf("Expected char len of 1, found %d\n", node->literal.len);
                exit(1);
            }
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
    case N_TYPE:
        if (node->type->kind == T_ENUM) {
            for (int i = 0; i < node->type->_enum.fields_array.count; i++) {
                p_append_enum_const(p, get_enum_field(node->type, i));
            }
        }
        break;
    case N_CONTINUE:
        if (!sema_ctx->loop) {
            printf("Cannot call continue outside of a loop\n");
            exit(1);
        }
        node->_continue.loop = sema_ctx->loop;
        break;
    case N_BREAK:
        if (!sema_ctx->loop) {
            printf("Cannot call break outside of a loop or switch statement\n");
            exit(1);
        }
        node->_break.loop = sema_ctx->loop;
        break;
    case N_INIT_LIST:
        if (node->type == type_invalid) {
            printf("Semantic Analysis recieved an untyped initializer list\n");
            exit(1);
        }
        switch (node->type->kind) {
        case T_ARRAY:
            // Infer the size from the initializer list
            if (node->type->_array.array_len == -1) {
                if (!node || node->init_list.elements_array.count < 1) {
                    printf("Inferred array must be initialized, and cannot be empty.\n");
                    exit(1);
                }
                node->type = infer_array_length(node->type, node->init_list.elements_array.count);
            }

            int a_index = 0;
            for (int i = 0; i < node->init_list.elements_array.count; i++) {
                Node *e = get_node(&node->init_list.elements_array, i);
                if (a_index > node->type->_array.array_len - 1 && e->kind != N_ELEMENT_ASSIGN) {
                    printf("Too many initializers for ");
                    print_type(node->type);
                    printf("\n");
                    exit(1);
                }
                Node *value = e->kind == N_ELEMENT_ASSIGN ? e->element_assign.value : e;
                semantic_analysis(sema_ctx, p, nm, value);
                if (e->kind == N_ELEMENT_ASSIGN) a_index = e->element_assign.index;

                if (e->kind != N_ELEMENT_ASSIGN) {
                    Node *element_assign = new_node(nm, N_ELEMENT_ASSIGN);
                    element_assign->element_assign.value = e;
                    element_assign->element_assign.index = a_index;
                    set_node(&node->init_list.elements_array, &element_assign, i);
                    e = element_assign;
                }
                e->type = node->type->base;
                e->element_assign.index = a_index;

                if (value->type != node->type->base) {
                    Node *casted_node = cast_node(nm, value, node->type->base);
                    set_node(&node->init_list.elements_array, &casted_node, a_index);
                }
                a_index++;
            }
            break;
        case T_STRUCT:
            int s_index = 0;
            for (int i = 0; i < node->init_list.elements_array.count; i++) {
                Node *e = get_node(&node->init_list.elements_array, i);
                if (s_index > node->type->_struct.members_array.count - 1 && e->kind != N_MEMBER_ASSIGN) {
                    printf("Too many initializers for ");
                    print_type(node->type);
                    printf("\n");
                    exit(1);
                }
                Node *value = e->kind == N_MEMBER_ASSIGN ? e->member_assign.value : e;
                semantic_analysis(sema_ctx, p, nm, value);
                StructMember *member = e->kind == N_MEMBER_ASSIGN ? get_struct_member_named(node->type, e->member_assign.name, &s_index)
                                                                  : get_struct_member(node->type, s_index);
                if (e->kind != N_MEMBER_ASSIGN) {
                    Node *member_assign = new_node(nm, N_MEMBER_ASSIGN);
                    member_assign->member_assign.value = e;
                    member_assign->member_assign.name = member->name;
                    set_node(&node->init_list.elements_array, &member_assign, i);
                    e = member_assign;
                }

                e->type = member->type;
                e->member_assign.member = member;

                if (value->type != member->type) {
                    Node *casted_node = cast_node(nm, value, member->type);
                    set_node(&node->init_list.elements_array, &casted_node, s_index);
                }

                s_index++;
            }
            break;
        default:
            printf("Initializer list can only be used for struct and arrays");
            exit(1);
        }
        break;
    case N_MEMBER_ACCESS:
        semantic_analysis(sema_ctx, p, nm, node->member_access.identifier);
        Type *lhs_t = node->member_access.identifier->type;
        if (node->member_access.op == TK_ARROW) {
            if (lhs_t->kind != T_POINTER) {
                printf("Dereference \'->\' can only be used on pointers\n");
                exit(1);
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
        StructMember *member_f = get_member(lhs_t, node->member_access.member->identifier.name);
        node->member_access.member->type = member_f->type;
        node->member_access.offset = member_f->offset;
        node->type = member_f->type;

        break;
    case N_SWITCH:
        p_push_scope(p);
        sema_ctx->loop = node;
        semantic_analysis(sema_ctx, p, nm, node->_switch.test);
        semantic_analysis(sema_ctx, p, nm, node->_switch.block);
        p_pop_scope(p);
        break;
    case N_CASE:
        semantic_analysis(sema_ctx, p, nm, node->_case.test);
        if (!node->_case.test) break;
        if (!(node->_case.test->type->kind == T_INT || node->_case.test->type->kind == T_ENUM)) {
            printf("Not ready to handle non int test cases\n");
            exit(1);
        }
        break;
    case N_COMPOUND_LITERAL:
        node->compound_literal.value->type = node->type;
        semantic_analysis(sema_ctx, p, nm, node->compound_literal.value);
        lower_compound_literal(sema_ctx, p, nm, node);
        break;
    case N_TYPEDEF:
    case N_GOTO:
    case N_LABEL:
        break;
    case N_ELEMENT_ASSIGN:
    case N_MEMBER_ASSIGN:
        printf("Unreachable\n");
        exit(1);
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
        Node *n = &nm->nodes[i];
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
