#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/sema.h"
#include "compiler_c/tokenizer.h"
#include "compiler_c/type.h"
#include <compiler_c/ir/ir_builder.h>
#include <compiler_c/ir/ir_gen.h>
#include <compiler_c/node.h>

static IR_Value ir_gen_lvalue(IR_Context *ctx, const Node *expr) {
    switch (expr->kind) {
    case N_IDENTIFIER:
        return ir_get_var_reg(ctx, expr->identifier.name);
    case N_UNARY:
        if (expr->unary.op != TK_MULTIPLY) break;
        return ir_gen_rvalue(ctx, expr->unary.expr);
    case N_INDEX:
        Node bin;
        bin.kind = N_BINARY;
        bin.binary.lhs = expr->index.identifier;
        bin.binary.op = TK_PLUS;
        bin.binary.rhs = expr->index.index;
        bin.type = expr->index.index->type;
        return ir_gen_rvalue(ctx, &bin);
    case N_MEMBER_ACCESS:
        IR_Value addr = ir_gen_lvalue(ctx, expr->member_access.identifier);
        if (!expr->member_access.offset) return addr;
        IR_Value c =
            ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){.type = type_long, .i = expr->member_access.offset}), type_long);
        return ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), addr, c, type_void_ptr);
    case N_CAST:
        return ir_gen_rvalue(ctx, expr);
    default:
        break;
    }

    printf("Tried to ir_gen_lvalue for a node which is not an lvalue\n");
    exit(1);
}

IR_Literal ir_gen_literal(const Node *node) {
    IR_Literal c;
    c.type = node->type;
    switch (c.type->kind) {
    case T_INT:
        c.i = node->literal.i;
        break;
    case T_FLOAT:
        c.f = node->literal.f;
        break;
    case T_POINTER:
    case T_ARRAY:
        if (c.type->base == type_char) {
            c.s.data = node->literal.s.data;
            c.s.len = node->literal.s.len;
            break;
        }
    case T_INVALID:
    default:
        printf("Tried to create IR_CONST instruction with an invalid type\n");
        exit(1);
    }
    return c;
}

IR_Value ir_gen_rvalue(IR_Context *ctx, const Node *expr) {
    switch (expr->kind) {
    case N_MEMBER_ACCESS:
    case N_INDEX:
        return ir_load(ctx, ir_gen_lvalue(ctx, expr), expr->type);
    case N_IDENTIFIER:
        return ir_gen_lvalue(ctx, expr);
    case N_LITERAL:
        IR_Literal c = ir_gen_literal(expr);
        return ir_const(ctx, ir_append_const(ctx->module, &c), expr->type);
    case N_BINARY:
        if (is_assignment_op(expr->binary.op)) {
            IR_Value addr = ir_gen_lvalue(ctx, expr->binary.lhs);
            IR_Value val = ir_gen_rvalue(ctx, expr->binary.rhs);
            bool dereference = expr->binary.lhs->kind == N_INDEX || expr->binary.lhs->kind == N_MEMBER_ACCESS || is_deref(expr->binary.lhs);
            if (expr->binary.op != TK_EQ) {
                IR_Value binop_val = dereference ? ir_load(ctx, addr, expr->binary.lhs->unary.expr->type) : addr;
                if (expr->type->kind == T_POINTER) {
                    printf("Cannot x= pointers rn\n");
                    exit(1);
                }
                val = ir_binary(ctx, ir_binary_op(get_underlying_op(expr->binary.op)), ir_next_virtual_reg(ctx->func), binop_val, val,
                                expr->type);
            } else if (expr->type->kind == T_STRUCT) {
                // memcpy for `struct = struct;`
                // addr = ir_address(ctx, addr, 0);
                // IR_Value val_addr = ir_address(ctx, val, 0);
                ir_memcpy(ctx, val, addr, expr->type->size);
                return val;
            }
            if (dereference) ir_store_mem(ctx, addr, val, expr->type);
            else ir_store(ctx, addr, val, expr->type);

            return val;
        }
        IR_Value lhs = ir_gen_rvalue(ctx, expr->binary.lhs);

        if (expr->binary.op == TK_OR_OR || expr->binary.op == TK_AND_AND) {
            IR_Value zero = ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){type_int, 0}), type_int);
            IR_Value lhs_cmp = ir_cmp(ctx, NEQ, lhs, zero);

            if (ir_within_cond(ctx)) {
                if (expr->binary.op == TK_AND_AND) ir_branch_cond(ctx, lhs_cmp, NULL, ctx->false_block);
                if (expr->binary.op == TK_OR_OR) ir_branch_cond(ctx, lhs_cmp, ctx->true_block, NULL);
            }

            IR_Value rhs = ir_gen_rvalue(ctx, expr->binary.rhs);
            IR_Value rhs_cmp = ir_cmp(ctx, NEQ, rhs, zero);
            // No need to cmp both results, if we reach here it means lhs is 1 or rhs represents (lhs op rhs)
            // Early out
            if (ir_within_cond(ctx)) return rhs_cmp;

            // Otherwise generate the whole || or && result
            return ir_binary(ctx, expr->binary.op == TK_OR_OR ? BW_OR : BW_AND, ir_next_virtual_reg(ctx->func), lhs_cmp, rhs_cmp, type_int);
        }

        IR_Value rhs = ir_gen_rvalue(ctx, expr->binary.rhs);
        if (is_comparison_op(expr->binary.op)) {
            return ir_cmp(ctx, ir_cmp_op(expr->binary.op), lhs, rhs);
        } else {
            if (expr->binary.lhs->type->kind == T_POINTER && expr->binary.rhs->type->kind == T_INT) {
                IR_Value c =
                    ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){type_long, expr->binary.lhs->type->base->size}), type_long);
                rhs = ir_binary(ctx, MUL, ir_next_virtual_reg(ctx->func), rhs, c, type_long);
            } else if (expr->binary.lhs->type->kind == T_INT && expr->binary.rhs->type->kind == T_POINTER) {
                IR_Value c =
                    ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){type_long, expr->binary.rhs->type->base->size}), type_long);
                lhs = ir_binary(ctx, MUL, ir_next_virtual_reg(ctx->func), lhs, c, type_long);
            }
            return ir_binary(ctx, ir_binary_op(expr->binary.op), ir_next_virtual_reg(ctx->func), lhs, rhs, expr->type);
        }
    case N_UNARY:
        if (expr->unary.op == TK_INCR || expr->unary.op == TK_DECR) {
            if (expr->unary.expr->kind != N_IDENTIFIER) {
                printf("Can only increment on a identifieir/variable\n");
                exit(1);
            }
            IR_Literal c;
            c.type = expr->type;
            switch (expr->type->kind) {
            case T_INT:
                c.i = 1;
                break;
            case T_FLOAT:
                c.f = 1.0;
                break;
            default:
                printf("Tried to increment a value which is neither float or int\n");
                exit(1);
            }
            IR_Value addr_reg = ir_gen_lvalue(ctx, expr->unary.expr);

            const IR_Value const_dst = ir_const(ctx, ir_append_const(ctx->module, &c), expr->type);
            const IR_Value store_dst = ir_store(ctx, ir_next_virtual_reg(ctx->func), addr_reg, expr->type);
            const IR_Value binary_dst = ir_binary(ctx, expr->unary.op == TK_INCR ? ADD : SUB, addr_reg, addr_reg, const_dst, expr->type);
            return expr->unary.associativity ? store_dst : binary_dst;
        } else if (expr->unary.op == TK_AND) { // & ref
            const IR_Value addr = ir_gen_lvalue(ctx, expr->unary.expr);
            if (expr->unary.expr->kind == N_INDEX) {
                return addr;
            }
            return ir_address(ctx, addr, 0);
        } else if (expr->unary.op == TK_MULTIPLY) { // * deref
            const IR_Value addr = ir_gen_rvalue(ctx, expr->unary.expr);
            return ir_load(ctx, addr, expr->type);
        } else if (expr->unary.op == TK_SIZEOF) {
            return ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){type_int, expr->unary.expr->type->size}), type_int);
        }
        const IR_Value expr_reg = ir_gen_rvalue(ctx, expr->unary.expr);
        return ir_unary(ctx, ir_unary_op(expr->unary.op), expr_reg, expr->type);
    case N_FUNCTION_CALL:
        return ir_call(ctx, expr);
    case N_CAST:
        const IR_Value src = ir_gen_rvalue(ctx, expr->cast.expr);
        return ir_cast(ctx, src, expr->type, expr->cast.from);
    default:
        break;
    }
    printf("Failed to gen expr for ");
    print_node_type(expr->kind);
    printf("\n");
    exit(1);
}

static void ir_gen_block_item(IR_Context *ctx, const Node *item) {
    if (item->kind == N_VAR_DECL) ir_gen_var_decl(ctx, item);
    else ir_gen_statement(ctx, item);
}

static void ir_gen_compound(IR_Context *ctx, const Node *comp) {
    ir_begin_scope(ctx->func);
    for (int i = 0; i < comp->compound.items_array.count; i++) {
        ir_gen_block_item(ctx, get_node(&comp->compound.items_array, i));
    }
    ir_end_scope(ctx->func);
}

static void ir_gen_while_loop(IR_Context *ctx, const Node *_while) {
    ir_begin_scope(ctx->func);
    IR_Block *cond_block = ir_add_block(ctx);
    IR_Block *block_block = ir_new_block();
    IR_Block *end_block = ir_new_block();

    ir_push_loop_ctx(ctx, block_block, end_block);

    ir_set_cond_block(ctx, block_block, end_block);
    const IR_Value cond_reg = ir_gen_rvalue(ctx, _while->_while.cond);
    ir_reset_cond_block(ctx);

    ir_branch_cond(ctx, cond_reg, block_block, end_block);

    ir_append_block(ctx, block_block);
    ir_gen_statement(ctx, _while->_while.block);
    ir_branch(ctx, cond_block);

    ir_append_block(ctx, end_block);

    ir_pop_loop_ctx(ctx);
    ir_end_scope(ctx->func);
}
static void ir_gen_switch_statement(IR_Context *ctx, const Node *_switch) {
    IR_Value test = ir_gen_rvalue(ctx, _switch->_switch.test);
    if (_switch->_switch.cases_array.count == 0) return;
    IR_Block **cases = malloc(sizeof(IR_Block *) * _switch->_switch.cases_array.count);
    if (!cases) {
        printf("Failed to allocate for ir_gen_switch cases\n");
        exit(1);
    }
    IR_Block *default_block = ir_new_block();
    IR_Block *end_block = ir_new_block();
    int block_index = 0;
    for (int i = 0; i < _switch->_switch.cases_array.count; i++) {
        // Is a case x:
        Node *_case = get_node(&_switch->_switch.cases_array, i);
        if (_case->_case.test) {
            cases[block_index++] = ir_new_block();
            IR_Value test_case = ir_gen_rvalue(ctx, _case->_case.test);
            IR_Value cmp_reg = ir_cmp(ctx, NEQ, test, test_case);
            // branch for fallthrough to the next test.
            ir_branch_cond(ctx, cmp_reg, NULL, cases[block_index - 1]);
        }
    }
    ir_branch(ctx, default_block);

    ir_begin_scope(ctx->func);
    ir_push_loop_ctx(ctx, NULL, end_block);
    int j = 0;
    for (int i = 0; i < _switch->_switch.block->compound.items_array.count; i++) {
        Node *node = get_node(&_switch->_switch.block->compound.items_array, i);
        if (node->kind == N_CASE) {
            if (node->_case.test) ir_append_block(ctx, cases[j++]);
            else ir_append_block(ctx, default_block);
        } else {
            ir_gen_block_item(ctx, node);
        }
    }
    ir_pop_loop_ctx(ctx);
    ir_end_scope(ctx->func);

    ir_append_block(ctx, end_block);
}

static void ir_gen_for_loop(IR_Context *ctx, const Node *_for) {
    ir_begin_scope(ctx->func);
    ir_gen_block_item(ctx, _for->_for.init);

    IR_Block *cond_block = ir_add_block(ctx);
    IR_Block *block_block = ir_new_block();
    IR_Block *iter_block = ir_new_block();
    IR_Block *end_block = ir_new_block();

    // Update ctx for continue/break statements
    ir_push_loop_ctx(ctx, iter_block, end_block);

    ir_set_cond_block(ctx, block_block, end_block);
    const IR_Value cond_reg = ir_gen_rvalue(ctx, _for->_for.cond);
    ir_reset_cond_block(ctx);

    ir_branch_cond(ctx, cond_reg, block_block, end_block);

    ir_append_block(ctx, block_block);
    ir_gen_statement(ctx, _for->_for.block);

    ir_branch(ctx, iter_block);
    ir_append_block(ctx, iter_block);
    ir_gen_rvalue(ctx, _for->_for.iter);

    ir_branch(ctx, cond_block);

    ir_append_block(ctx, end_block);
    // Reset ctx for continue/break statements
    ir_pop_loop_ctx(ctx);
    ir_end_scope(ctx->func);
}

static void ir_gen_if_statement(IR_Context *ctx, const Node *_if) {
    ir_begin_scope(ctx->func);
    IR_Block *if_true_block = ir_new_block();
    IR_Block *end_block = ir_new_block();
    IR_Block *else_block = _if->_if.if_false ? ir_new_block() : end_block;

    // Context true/false blocks should only be used within (cond) part
    ir_set_cond_block(ctx, if_true_block, else_block);
    const IR_Value cond_reg = ir_gen_rvalue(ctx, _if->_if.cond);
    ir_reset_cond_block(ctx);

    ir_branch_cond(ctx, cond_reg, if_true_block, else_block);
    ir_append_block(ctx, if_true_block);
    ir_gen_statement(ctx, _if->_if.if_true);
    ir_branch(ctx, end_block);
    if (_if->_if.if_false) { // IF there is an else {}
        ir_append_block(ctx, else_block);
        if (_if->_if.if_false->kind == N_IF) { // -> ELSE IF {}
            ir_gen_if_statement(ctx, _if->_if.if_false);
            // Might need end block context
        } else {
            ir_gen_statement(ctx, _if->_if.if_false);
            ir_branch(ctx, end_block);
        }
    }
    ir_append_block(ctx, end_block);
    ir_end_scope(ctx->func);
}

static void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl) {
    // Handle globals seperately to locals
    if (var_decl->var_decl.is_global) {
        if (var_decl->var_decl.storage_class == EXTERN) return;
        IR_Literal x;
        IR_Literal *l = &x;
        if (var_decl->var_decl.has_initializer) {
            Node *x = var_decl->var_decl.expr;
            *l = ir_gen_literal(var_decl->var_decl.expr);
        } else l = NULL;
        return ir_append_global(ctx->module, var_decl->var_decl.identifier->identifier.name, var_decl->type, l,
                                var_decl->var_decl.symbol->linkage, var_decl->var_decl.symbol->storage);
    }

    // Handle locals
    IR_Value dst = ir_new_var(ctx->func, var_decl->var_decl.identifier->identifier.name, var_decl->type);
    if (!var_decl->var_decl.has_initializer) return;

    if (var_decl->var_decl.expr->kind == N_INIT_LIST) {
        bool is_array = var_decl->type->kind == T_ARRAY;
        int len = is_array ? var_decl->type->_array.array_len : var_decl->type->_struct.members_array.count;

        Node *l = var_decl->var_decl.expr;
        Type *type;
        IR_Value zero;
        IR_Value v;
        if (is_array) {
            type = var_decl->type->base;
            zero = ir_append_const(ctx->module, &(IR_Literal){type, 0});
        }
        for (int i = 0; i < len; i++) {
            dst.offset = type->align * i;
            if (!is_array) {
                StructMember *member = get_struct_member(var_decl->type, i);
                type = member->type;
                dst.offset = member->offset;
            }
            Node *e = get_node(&l->init_list.elements_array, i);
            if (i < l->init_list.elements_array.count) v = ir_gen_rvalue(ctx, e);
            else {
                if (!is_array) zero = ir_append_const(ctx->module, &(IR_Literal){e->type, 0});
                v = ir_const(ctx, zero, type);
            }
            ir_store(ctx, dst, v, type);
        }
        return;
    }

    const IR_Value addr = ir_gen_rvalue(ctx, var_decl->var_decl.expr);

    if (var_decl->type->kind == T_ARRAY) {
        ir_alloca(ctx, dst, align(var_decl->type->size, 8), 8);
        // printf("dst: %d\n", dst.kind);
        dst = ir_address(ctx, dst, 0);
        ir_memcpy(ctx, addr, dst, var_decl->type->size);
    } else {
        ir_store(ctx, dst, addr, var_decl->type);
    }
}

static void ir_gen_statement(IR_Context *ctx, const Node *stmt) {
    switch (stmt->kind) {
    case N_RETURN:
        return ir_gen_return(ctx, stmt);
    case N_COMPOUND:
        return ir_gen_compound(ctx, stmt);
    case N_IF:
        return ir_gen_if_statement(ctx, stmt);
    case N_WHILE:
        return ir_gen_while_loop(ctx, stmt);
    case N_FOR:
        return ir_gen_for_loop(ctx, stmt);
    case N_SWITCH:
        return ir_gen_switch_statement(ctx, stmt);
    case N_FUNCTION_CALL:
    case N_BINARY:
    case N_UNARY:
        ir_gen_rvalue(ctx, stmt);
        return;
    case N_IDENTIFIER:
    case N_LITERAL:
        return;
    case N_BREAK:
        ir_branch(ctx, ir_loop_ctx(ctx)->break_block);
        return;
    case N_CONTINUE:
        ir_branch(ctx, ir_loop_ctx(ctx)->continue_block);
        return;
    default:
        // given invalid statement? probably an expression
        printf("Dont know what to do with the given statemnet: ir_gen_statement: ");
        print_node_type(stmt->kind);
        printf("\n");
        exit(1);
    }
}

static void ir_gen_return(IR_Context *ctx, const Node *_return) { ir_return(ctx, ir_gen_rvalue(ctx, _return->_return.expr)); }

static IR_Function *ir_gen_function(IR_Context *ctx, const Node *func) {
    if (func->kind != N_FUNCTION) {
        printf("Tried ir_gen_function but given node is not a function!\n");
        exit(1);
    }
    if (!func->func.is_defined) {
        return NULL;
    }

    IR_Function *fn = ir_new_function(ctx, func->func.name);
    if (func->func.body->kind != N_COMPOUND) {
        printf("Function body is not a compound,\n");
        exit(1);
    }
    fn->linkage = func->func.symbol->linkage;
    fn->storage = func->func.symbol->storage;

    ir_begin_scope(fn);
    // handle (params)
    for (int i = 0; i < func->func.params_array.count; i++) {
        // Copy from registers intsead
        Node *param = get_node(&func->func.params_array, i);
        ir_new_var(ctx->func, param->var_decl.identifier->identifier.name, param->type);
        ir_store(ctx, ir_mem_value(i, param->type), ir_vreg_value(-i - 1, param->type), param->type);
        fn->param_count++;
    }
    // handle {[statement]*}
    for (int i = 0; i < func->func.body->compound.items_array.count; i++) {
        ir_gen_block_item(ctx, get_node(&func->func.body->compound.items_array, i));
    }
    ir_end_scope(fn);

    return fn;
}

IR_Module *ir_gen_translation_unit(IR_Context *ctx, const Node *tu) {
    if (tu->kind != N_TRANSLATION_UNIT) {
        printf("Tried ir_gen_function but given node is not a translation unit!\n");
        exit(1);
    }

    IR_Module *module = ir_new_module();
    ctx->module = module;

    for (int i = 0; i < tu->translation_unit.declarations_array.count; i++) {
        Node *n = get_node(&tu->translation_unit.declarations_array, i);
        switch (n->kind) {
        case N_FUNCTION:
            IR_Func_Def *func_def = ir_get_func_def(ctx, n->func.name);
            if (func_def) {
                if (func_def->is_defined) {
                    if (n->func.has_initializer) {
                        printf("Redefinition of %s\n", n->func.name);
                        exit(1);
                    }
                    break;
                }
            } else func_def = ir_append_func_def(ctx, n->func.name, n->func.has_initializer);

            if (n->func.has_initializer) ir_append_function(ctx, func_def, ir_gen_function(ctx, n));
            break;
        case N_TYPEDEF:
        case N_TYPE:
            break;
        case N_VAR_DECL:
            ir_gen_var_decl(ctx, n);
            // Handled by parser or smt
            break;
        default:
            printf("Recieved an unexpected thing\n");
            exit(1);
        }
    }
    return module;
}

int ir_within_cond(IR_Context *ctx) { return ctx->false_block && ctx->true_block; }

void ir_set_cond_block(IR_Context *ctx, IR_Block *true_block, IR_Block *false_block) {
    ctx->true_block = true_block;
    ctx->false_block = false_block;
}

void ir_reset_cond_block(IR_Context *ctx) {
    ctx->true_block = NULL;
    ctx->false_block = NULL;
}
