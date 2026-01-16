#include <stdio.h>
#include <stdlib.h>

#include "compiler_c/tokenizer.h"
#include "compiler_c/type.h"
#include <compiler_c/ir.h>
#include <compiler_c/node.h>

static int ir_gen_lvalue(IR_Context *ctx, const Node *expr) {
    switch (expr->kind) {
    case N_IDENTIFIER:
        return ir_get_var_reg(ctx, expr->identifier.name);
    case N_UNARY:
        if (expr->unary.op != TK_MULTIPLY) break;
        return ir_gen_rvalue(ctx, expr->unary.expr);
    // TODO: case N_BINARY index [x]
    default:
        break;
    }

    printf("Tried to ir_gen_lvalue for a node which is not an lvalue\n");
    exit(1);
}

int ir_gen_rvalue(IR_Context *ctx, const Node *expr) {
    switch (expr->kind) {
    case N_IDENTIFIER:
        return ir_gen_lvalue(ctx, expr);
    case N_LITERAL:
        IR_Const c;
        c.type = expr->type;
        switch (expr->type->kind) {
        case T_INT:
            c.i = expr->literal.i;
            break;
        case T_FLOAT:
            c.f = expr->literal.f;
            break;
        case T_CHAR:
            c.c = expr->literal.c;
            break;
        default:
            printf("Tried to create IR_CONST instruction with an invalid type\n");
            exit(1);
        }
        return ir_const(ctx, ir_append_const(ctx->module, &c), expr->type);
    case N_BINARY:
        if (expr->binary.op == TK_EQ) {
            int addr = ir_gen_lvalue(ctx, expr->binary.lhs);
            int val = ir_gen_rvalue(ctx, expr->binary.rhs);
            ir_store(ctx, addr, val, expr->type);
            return val;
        }
        int lhs = ir_gen_rvalue(ctx, expr->binary.lhs);
        int rhs = ir_gen_rvalue(ctx, expr->binary.rhs);
        if (is_comparison_op(expr->binary.op)) {
            return ir_cmp(ctx, ir_cmp_op(expr->binary.op), lhs, rhs);
        } else {
            return ir_binary(ctx, ir_binary_op(expr->binary.op), ir_next_reg(ctx->func), lhs, rhs, expr->type);
        }
    case N_UNARY:
        if (expr->unary.op == TK_INCR || expr->unary.op == TK_DECR) {
            if (expr->unary.expr->kind != N_IDENTIFIER) {
                printf("Can only increment on a identifieir/variable\n");
                exit(1);
            }
            IR_Const c;
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
            int addr_reg = ir_gen_lvalue(ctx, expr->unary.expr);
            const int const_dst = ir_const(ctx, ir_append_const(ctx->module, &c), expr->type);
            const int store_dst = ir_store(ctx, ir_next_reg(ctx->func), addr_reg, expr->type);
            const int binary_dst = ir_binary(ctx, expr->unary.op == TK_INCR ? ADD : SUB, addr_reg, addr_reg, const_dst, expr->type);
            return expr->unary.associativity ? store_dst : binary_dst;
        } else if (expr->unary.op == TK_AND) {
            const int addr = ir_gen_lvalue(ctx, expr->unary.expr);
            return ir_address(ctx, addr, 0);
        } else if (expr->unary.op == TK_MULTIPLY) {
            const int addr = ir_gen_lvalue(ctx, expr->unary.expr);
            return ir_load(ctx, addr, expr->type);
        }
        const int expr_reg = ir_gen_rvalue(ctx, expr->unary.expr);
        ir_unary(ctx, ir_unary_op(expr->unary.op), expr_reg, expr->type);
    case N_FUNCTION_CALL:
        return ir_call(ctx, expr);
    case N_CAST:
        const int src = ir_gen_rvalue(ctx, expr->cast.expr);
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
    if (item->kind == N_VAR_DECL) {
        ir_gen_var_decl(ctx, item);
    } else {
        ir_gen_statement(ctx, item);
    }
}

static void ir_gen_compound(IR_Context *ctx, const Node *comp) {
    ir_begin_scope(ctx->func);
    for (int i = 0; i < comp->compound.count; i++) {
        ir_gen_block_item(ctx, comp->compound.items[i]);
    }
    ir_end_scope(ctx->func);
}

static void ir_gen_while_loop(IR_Context *ctx, const Node *_while) {
    const int cond_id = ir_add_block(ctx); // cond:
    const int cond_reg = ir_gen_rvalue(ctx, _while->_while.cond);
    const int block_id = cond_id + 1;
    const int end_id = cond_id + 2;
    ir_branch_cond(ctx, cond_reg, block_id, end_id);
    ir_add_block(ctx); // block:
    ir_gen_statement(ctx, _while->_while.block);
    ir_branch(ctx, cond_id);
    ir_add_block(ctx); // end:
}
static void ir_gen_for_loop(IR_Context *ctx, const Node *_for) {
    ir_gen_block_item(ctx, _for->_for.init);

    const int cond_id = ir_add_block(ctx); // cond:
    const int block_id = cond_id + 1;
    const int end_id = cond_id + 2;
    const int cond_reg = ir_gen_rvalue(ctx, _for->_for.cond);

    ir_branch_cond(ctx, cond_reg, block_id, end_id);
    ir_add_block(ctx); // block:
    ir_gen_statement(ctx, _for->_for.block);
    ir_gen_rvalue(ctx, _for->_for.iter);
    ir_branch(ctx, cond_id);
    ir_add_block(ctx); // end:
}

static void ir_gen_if_statement(IR_Context *ctx, const Node *_if) {
    const int cond_reg = ir_gen_rvalue(ctx, _if->_if.cond);
    const int if_true_id = ctx->func->block_count;
    const int if_false_id = if_true_id + 1; // if no else, then this is the end block
    ir_branch_cond(ctx, cond_reg, if_true_id, if_false_id);
    ir_add_block(ctx); // IF true block
    ir_gen_statement(ctx, _if->_if.if_true);
    if (_if->_if.if_false == NULL) { // No else, means branch to the end after compound
        ir_branch(ctx, if_false_id);
        ir_add_block(ctx); // IF else or endblock
    } else {
        if (_if->_if.if_false->kind == N_IF) {
            ir_branch(ctx, if_false_id);
            ir_add_block(ctx); // IF else or endblock
            ir_gen_if_statement(ctx, _if->_if.if_false);
        } else {
            const int end_id = if_false_id + 1;
            ir_branch(ctx, end_id);
            ir_add_block(ctx); // IF else or endblock
            ir_gen_statement(ctx, _if->_if.if_false);
            ir_branch(ctx, end_id);
            ir_add_block(ctx); // end
        }
    }
}

static void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl) {
    const int dst = ir_new_var(ctx->func, var_decl->var_decl.name, var_decl->type);
    const int addr = ir_gen_rvalue(ctx, var_decl->var_decl.expr);
    ir_store(ctx, dst, addr, var_decl->type);
}

static void ir_gen_statement(IR_Context *ctx, const Node *stmt) {
    switch (stmt->kind) {
    case N_RETURN:
        ir_gen_return(ctx, stmt);
        return;
    case N_COMPOUND:
        ir_gen_compound(ctx, stmt);
        return;
    case N_IF:
        ir_gen_if_statement(ctx, stmt);
        return;
    case N_WHILE:
        ir_gen_while_loop(ctx, stmt);
        return;
    case N_FOR:
        ir_gen_for_loop(ctx, stmt);
        return;
    case N_FUNCTION_CALL:
    case N_BINARY:
    case N_UNARY:
        ir_gen_rvalue(ctx, stmt);
        return;
    case N_IDENTIFIER:
    case N_LITERAL:
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

    IR_Function *fn = ir_new_function(ctx, func->func.name);
    if (func->func.body->kind != N_COMPOUND) {
        printf("Function body is not a compound,\n");
        exit(1);
    }

    ir_begin_scope(fn);
    // handle (params)
    for (int i = 0; i < func->func.param_count; i++) {
        ir_new_var(ctx->func, func->func.params[i]->var_decl.name, func->func.params[i]->type);
        ir_store(ctx, i, -func->func.param_count + i, func->func.params[i]->type);
    }
    // handle {[statement]*}
    for (int i = 0; i < func->func.body->compound.count; i++) {
        ir_gen_block_item(ctx, func->func.body->compound.items[i]);
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
    for (int i = 0; i < tu->translation_unit.count; i++) {
        switch (tu->translation_unit.declarations[i]->kind) {
        case N_FUNCTION:
            ir_append_function(ctx->module, ir_gen_function(ctx, tu->translation_unit.declarations[i]));
            break;
        case N_VAR_DECL:
            // Add support for globals eventually
        default:
            printf("Globals and other bs are not supported yet.\n");
            exit(1);
        }
    }

    return module;
}
