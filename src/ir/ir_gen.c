#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler_c/analyse/sema.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <compiler_c/core/node.h>
#include <compiler_c/ir/ir_builder.h>
#include <compiler_c/ir/ir_gen.h>

IR_Value ir_gen_lvalue(IR_Context *ctx, const Node *expr) {
    switch (expr->kind) {
    case N_IDENTIFIER:
        return ir_get_symbol_value(ctx, expr->identifier.name, true);
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
            ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){.type = type_i64, .i = expr->member_access.offset}), type_i64);
        return ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), addr, c, type_void_ptr);
    case N_CAST:
        return ir_gen_rvalue(ctx, expr);
    default:
        break;
    }

    PANIC("Tried to ir_gen_lvalue for a node which is not an lvalue\n");
}

IR_Literal ir_literal(const Node *node) {
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
        if (c.type->base == type_i8) {
            c.s.data = node->literal.s.data;
            c.s.len = node->literal.s.len;
            break;
        }
    case T_INVALID:
    default:
        PANIC("Tried to create IR_CONST instruction with an invalid type\n");
    }
    return c;
}

IR_Value ir_gen_rvalue(IR_Context *ctx, const Node *expr) {
    switch (expr->kind) {
    case N_MEMBER_ACCESS:
    case N_INDEX:
        return ir_load(ctx, ir_gen_lvalue(ctx, expr), expr->type);
    case N_IDENTIFIER:
        IR_Value v = ir_get_symbol_value(ctx, expr->identifier.name, false);
        return v;
    case N_LITERAL:
        IR_Literal c = ir_literal(expr);
        return ir_const(ctx, ir_append_const(ctx->module, &c), expr->type);
    case N_BINARY:
        if (is_assignment_op(expr->binary.op)) {
            IR_Value addr = ir_gen_lvalue(ctx, expr->binary.lhs);
            IR_Value val = ir_gen_rvalue(ctx, expr->binary.rhs);
            if (val.kind == IR_FUNCTION) val = ir_address(ctx, val, 0);
            bool dereference = expr->binary.lhs->kind == N_INDEX || expr->binary.lhs->kind == N_MEMBER_ACCESS || is_deref(expr->binary.lhs);
            // If it is '+=' or some '=' variant
            if (expr->binary.op != TK_EQ) {
                IR_Value binop_val = dereference ? ir_load(ctx, addr, expr->binary.lhs->unary.expr->type) : addr;
                if (expr->type->kind == T_POINTER) {
                    PANIC("Cannot x= pointers rn\n");
                }
                val = ir_binary(ctx, ir_binary_op(get_underlying_op(expr->binary.op)), ir_next_virtual_reg(ctx->func), binop_val, val,
                                expr->type);
            } else if (expr->type->kind == T_STRUCT) {
                // memcpy for `struct = struct;`
                // addr is already an address, val is not
                IR_Value val_addr = ir_address(ctx, val, 0);
                ir_memcpy(ctx, val_addr, addr, expr->type->size);
                return val;
            }
            if (dereference) ir_store_mem(ctx, addr, val, expr->type);
            else ir_store(ctx, addr, val, expr->type);

            return val;
        }
        IR_Value lhs = ir_gen_rvalue(ctx, expr->binary.lhs);

        if (expr->binary.op == TK_OR_OR || expr->binary.op == TK_AND_AND) {
            IR_Value zero = ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){expr->type, 0}), expr->type);
            IR_Value lhs_cmp = ir_cmp(ctx, NEQ, lhs, zero, expr->type);

            if (ir_is_within_cond(ctx)) {
                if (expr->binary.op == TK_AND_AND) ir_branch_cond(ctx, lhs_cmp, NULL, ctx->false_block);
                if (expr->binary.op == TK_OR_OR) ir_branch_cond(ctx, lhs_cmp, ctx->true_block, NULL);
            }

            IR_Value rhs = ir_gen_rvalue(ctx, expr->binary.rhs);
            IR_Value rhs_cmp = ir_cmp(ctx, NEQ, rhs, zero, expr->type);
            // No need to cmp both results, if we reach here it means lhs is 1 or rhs represents (lhs op rhs)
            // Early out
            if (ir_is_within_cond(ctx)) return rhs_cmp;

            // Otherwise generate the whole || or && result
            return ir_binary(ctx, expr->binary.op == TK_OR_OR ? BW_OR : BW_AND, ir_next_virtual_reg(ctx->func), lhs_cmp, rhs_cmp, type_i32);
        }

        IR_Value rhs = ir_gen_rvalue(ctx, expr->binary.rhs);
        if (is_comparison_op(expr->binary.op)) {
            return ir_cmp(ctx, ir_cmp_op(expr->binary.op), lhs, rhs, expr->type);
        } else {
            // Scale integer by ptr base size
            if (expr->binary.lhs->type->kind == T_POINTER && expr->binary.rhs->type->kind == T_INT) {
                IR_Value c =
                    ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){type_i64, expr->binary.lhs->type->base->size}), type_i64);
                rhs = ir_binary(ctx, MUL, ir_next_virtual_reg(ctx->func), rhs, c, type_i64);
            } else if (expr->binary.lhs->type->kind == T_INT && expr->binary.rhs->type->kind == T_POINTER) {
                IR_Value c =
                    ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){type_i64, expr->binary.rhs->type->base->size}), type_i64);
                lhs = ir_binary(ctx, MUL, ir_next_virtual_reg(ctx->func), lhs, c, type_i64);
            }
            lhs = ir_binary(ctx, ir_binary_op(expr->binary.op), ir_next_virtual_reg(ctx->func), lhs, rhs, expr->type);
            // Divide (ptr - ptr) difference by base type

            if (expr->binary.lhs->type->kind == T_POINTER && expr->binary.rhs->type->kind == T_POINTER) {
                Type *base = expr->binary.lhs->type->base;
                if (base->size == 1) return lhs;
                if (expr->type != type_i64) {
                    PANIC("Recieved non ptrdiff type in (ptr-ptr) binary op\n");
                }
                IR_Value size = ir_append_const(ctx->module, &(IR_Literal){.type = expr->type, .i = base->size});
                size = ir_const(ctx, size, expr->type);
                return ir_binary(ctx, DIV, ir_next_virtual_reg(ctx->func), lhs, size, expr->type);
            }
            return lhs;
        }
    case N_UNARY:
        if (expr->unary.op == TK_INCR || expr->unary.op == TK_DECR) {
            if (expr->unary.expr->kind != N_IDENTIFIER) {
                PANIC("Can only increment on a identifieir/variable\n");
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
                PANIC("Tried to increment a value which is neither float or int\n");
            }
            IR_Value addr_reg = ir_gen_lvalue(ctx, expr->unary.expr);

            const IR_Value const_dst = ir_const(ctx, ir_append_const(ctx->module, &c), expr->type);
            const IR_Value store_dst = ir_store(ctx, ir_next_virtual_reg(ctx->func), addr_reg, expr->type);
            const IR_Value binary_dst = ir_binary(ctx, expr->unary.op == TK_INCR ? ADD : SUB, addr_reg, addr_reg, const_dst, expr->type);
            return expr->unary.associativity ? store_dst : binary_dst;
        } else if (expr->unary.op == TK_AND) { // & ref
            const IR_Value addr = ir_gen_lvalue(ctx, expr->unary.expr);
            if (expr->unary.expr->kind == N_INDEX ||
                (expr->unary.expr->type->kind == T_POINTER && expr->unary.expr->type->base->kind == T_FUNCTION)) {
                return addr;
            }
            return ir_address(ctx, addr, 0);
        } else if (expr->unary.op == TK_MULTIPLY) { // * deref
            const IR_Value addr = ir_gen_rvalue(ctx, expr->unary.expr);
            return ir_load(ctx, addr, expr->type);
        } else if (expr->unary.op == TK_SIZEOF) {
            return ir_const(ctx, ir_append_const(ctx->module, &(IR_Literal){type_i32, expr->unary.expr->type->size}), type_i32);
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
    log_start(LOG_ERROR);
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
    IR_Block *block_block = ir_new_block();
    IR_Block *end_block = ir_new_block();
    IR_Block *cond_block = ir_new_block();

    if (_while->_while.is_do_while) ir_branch(ctx, block_block);

    ir_append_block(ctx, cond_block);

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
        PANIC("Failed to allocate for ir_gen_switch cases\n");
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
            IR_Value cmp_reg = ir_cmp(ctx, NEQ, test, test_case, _switch->type);
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
        if (var_decl->var_decl.is_defined) {
            Node *x = var_decl->var_decl.expr;
            *l = ir_literal(var_decl->var_decl.expr);
        } else l = NULL;
        return ir_append_global(ctx->module, var_decl->var_decl.identifier->identifier.name, var_decl->type, l,
                                var_decl->var_decl.symbol->linkage, var_decl->var_decl.symbol->storage);
    }

    // Handle locals
    IR_Value dst = ir_new_var(ctx->func, var_decl->var_decl.identifier->identifier.name, var_decl->type);
    if (!var_decl->var_decl.is_defined) return;

    if (var_decl->var_decl.expr->kind == N_INIT_LIST) {
        IR_Value zero;
        Type *type;
        IR_Value ir_v;
        Node *l = var_decl->var_decl.expr;

        switch (var_decl->type->kind) {
        case T_INT:
        case T_FLOAT:
        case T_POINTER:
        case T_UNION:
            if (l->init_list.elements_array.count == 0) {
                type = var_decl->type;
                zero = ir_append_const(ctx->module, &(IR_Literal){type, 0});
                ir_v = ir_const(ctx, zero, type);
            } else {
                Node *e = get_node(&l->init_list.elements_array, 0);
                type = e->type;
                ir_v = ir_gen_rvalue(ctx, e->kind == N_DESIGNATED_INITIALIZER ? e->designated_init.value : e);
            }
            ir_store(ctx, dst, ir_v, type);
            break;
        case T_ARRAY:
        case T_STRUCT:
            bool is_array = var_decl->type->kind == T_ARRAY;
            int len = is_array ? var_decl->type->_array.array_len : var_decl->type->_struct.members_array.count;

            if (is_array) {
                type = var_decl->type->base;
                zero = ir_append_const(ctx->module, &(IR_Literal){type, 0});
            }

            for (int i = 0; i < len; i++) {
                Node *value = NULL;
                StructMember *member = is_array ? NULL : get_struct_member(var_decl->type, i);
                for (int j = l->init_list.elements_array.count - 1; j >= 0; j--) {
                    Node *e = get_node(&l->init_list.elements_array, j);
                    if (is_array ? e->designated_init._array.index == i : strcmp(member->name, e->designated_init._struct.name) == 0) {
                        value = e->designated_init.value;
                        break;
                    }
                }
                if (is_array) dst.offset = type->align * i;
                else {
                    type = member->type;
                    dst.offset = member->offset;
                }

                // If the corresponding value was found in the init list, use that, otherwise use a zero,
                if (value) ir_v = ir_gen_rvalue(ctx, value);
                else {
                    // If it is a struct, generate a zero in the correct member's type.
                    if (!is_array) zero = ir_append_const(ctx->module, &(IR_Literal){type, 0});
                    ir_v = ir_const(ctx, zero, type);
                }

                ir_store(ctx, dst, ir_v, type);
            }
            break;
        default:
            PANIC("Recieving unsupported type to lower var decl with initlist\n");
        }
        return;
    }

    const IR_Value addr = ir_gen_rvalue(ctx, var_decl->var_decl.expr);

    if (var_decl->type->kind == T_ARRAY || var_decl->type->kind == T_STRUCT) {
        ir_alloca(ctx, dst, align(var_decl->type->size, 8), 8);
        printf("1\n");
        dst = ir_address(ctx, dst, 0);
        ir_memcpy(ctx, addr, dst, var_decl->type->size);
    } else ir_store(ctx, dst, addr, var_decl->type);
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
        ir_branch(ctx, get_loop_ctx(ctx)->break_block);
        return;
    case N_CONTINUE:
        ir_branch(ctx, get_loop_ctx(ctx)->continue_block);
        return;
    case N_GOTO:
        return ir_gen_goto(ctx, stmt);
    case N_LABEL:
        return ir_gen_label(ctx, stmt);
    default:
        // given invalid statement? probably an expression
        log_start(LOG_ERROR);
        printf("Dont know what to do with the given statement: ir_gen_statement: ");
        print_node_type(stmt->kind);
        printf("\n");
        exit(1);
    }
}

static void ir_gen_goto(IR_Context *ctx, const Node *_goto) {
    IR_LabeledBlock *goto_lb = ir_get_labeled_block(ctx, _goto->_goto.identifier->identifier.name);
    if (!goto_lb) goto_lb = ir_append_labeled_block(ctx, _goto->_goto.identifier->identifier.name);
    ir_branch(ctx, goto_lb->block);
}

static void ir_gen_label(IR_Context *ctx, const Node *label) {
    IR_LabeledBlock *lb = ir_get_labeled_block(ctx, label->label.identifier->identifier.name);
    if (lb) {
        if (!lb->placeholder) {
            PANIC("Redefinition of label '%s'\n", label->label.identifier->identifier.name);
        }
    } else lb = ir_append_labeled_block(ctx, label->label.identifier->identifier.name);

    lb->placeholder = false;
    ir_append_block(ctx, lb->block);
}

static void ir_gen_return(IR_Context *ctx, const Node *_return) {
    IR_Value return_value = _return->_return.expr ? ir_gen_rvalue(ctx, _return->_return.expr) : ir_no_value;
    ir_return(ctx, return_value);
}

static IR_Function *ir_gen_function(IR_Context *ctx, const Node *func) {
    if (func->kind != N_FUNCTION) {
        PANIC("Tried ir_gen_function but given node is not a function!\n");
    }
    if (!func->func.is_defined) {
        return NULL;
    }

    IR_Function *fn = ir_new_function(ctx, func->func.name, func->type->_func.return_type);
    if (func->func.body->kind != N_COMPOUND) {
        PANIC("Function body is not a compound,\n");
    }
    fn->linkage = func->func.symbol->linkage;
    fn->storage = func->func.symbol->storage;

    ir_begin_scope(fn);
    // handle (params)
    for (int i = 0; i < func->type->_func.params.count; i++) {
        // Copy from registers instead
        ParamDecl *param = (ParamDecl *)get(&func->type->_func.params, i);
        ir_new_var(ctx->func, param->name, param->type);
        ir_store(ctx, ir_mem_value(i, param->type), ir_vreg_value(-i - 1, param->type), param->type);
        fn->param_count++;
    }
    // handle {[statement]*}
    for (int i = 0; i < func->func.body->compound.items_array.count; i++) {
        ir_gen_block_item(ctx, get_node(&func->func.body->compound.items_array, i));
    }
    if (func->type->_func.return_type == type_void) ir_return(ctx, ir_no_value);

    ir_end_scope(fn);

    return fn;
}

IR_Module *ir_gen_translation_unit(IR_Context *ctx, const Node *tu) {
    if (tu->kind != N_TRANSLATION_UNIT) {
        PANIC("Tried ir_gen_function but given node is not a translation unit!\n");
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
                    if (n->func.is_defined) {
                        PANIC("Redefinition of %s\n", n->func.name);
                    }
                    break;
                }
            } else func_def = ir_append_func_def(ctx, n->func.name, n->func.is_defined, n->type->_func.is_variadic, n->func.storage_class);

            if (n->func.is_defined) ir_append_function(ctx, func_def, ir_gen_function(ctx, n));
            break;
        case N_TYPEDEF:
        case N_TYPE:
            break;
        case N_VAR_DECL:
            ir_gen_var_decl(ctx, n);
            // Handled by parser or smt
            break;
        default:
            PANIC("Recieved an unexpected thing\n");
        }
    }
    return module;
}

int ir_is_within_cond(IR_Context *ctx) { return ctx->false_block && ctx->true_block; }

void ir_set_cond_block(IR_Context *ctx, IR_Block *true_block, IR_Block *false_block) {
    ctx->true_block = true_block;
    ctx->false_block = false_block;
}

void ir_reset_cond_block(IR_Context *ctx) {
    ctx->true_block = NULL;
    ctx->false_block = NULL;
}
