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
#include <compiler_c/abi/abi.h>
#include <compiler_c/core/node.h>
#include <compiler_c/ir/ir_builder.h>
#include <compiler_c/ir/ir_gen.h>

IR_Value ir_gen_lvalue(IR_Context *ctx, const Node *expr) {
    switch (expr->kind) {
    case N_IDENTIFIER:
        return ir_address(ctx, ir_symbol_value(expr->identifier.symbol), 0);
    case N_LITERAL:
        ASSERT(expr->literal.kind == L_STRING, "Only string literal can be lvalue\n");
        const IR_Literal l = ir_literal(expr);
        IR_Value v = ir_const(ctx, ir_append_literal(ctx->module, &l), expr->type);
        return ir_address(ctx, v, 0);
    case N_UNARY:
        ASSERT(expr->unary.op == TK_MULTIPLY, "Can only generate *expr lvalue\n");
        return ir_gen_rvalue(ctx, expr->unary.expr);
    case N_BINARY:
        return ir_gen_rvalue(ctx, expr);
    case N_INDEX:
        // Uses more complex lowering for ptr -integer arithmetic in ir_gen_rvalue,
        //  By spoofing as binary `a-b` node instead of a[x]
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
        return ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), addr, ir_integer_literal(expr->member_access.offset), type_u64);
    case N_CAST:
        // Decay/implicit casting
        if (expr->cast.expr->type->kind == T_ARRAY || expr->cast.expr->type->kind == T_STRUCT ||
            expr->cast.expr->type->kind == T_FUNCTION) {
            return ir_gen_lvalue(ctx, expr->cast.expr);
        }
        PANIC("bad Lvalue of N_CAST\n");
    default:
        break;
    }

    PANIC("Tried to ir_gen_lvalue for a node which is not an lvalue\n");
}

IR_Literal ir_literal(const Node *node) {
    ASSERT(node->kind == N_LITERAL, "ir_literal expects a N_LITERAL node\n");
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
    case N_IDENTIFIER:
        // TODO check if array also needs to be included here (passes tests without)
        // if (expr->type->kind == T_FUNCTION || expr->type->kind == T_ARRAY)
        if (expr->type->kind == T_FUNCTION) return ir_symbol_value(expr->identifier.symbol);
        // else if (expr->type->kind == T_STRUCT) return ir_gen_lvalue(ctx, expr);
        return ir_load(ctx, ir_gen_lvalue(ctx, expr), expr->type);
    case N_LITERAL:
        IR_Literal c = ir_literal(expr);
        return ir_smart_const(ctx, &c, expr->type);
    case N_BINARY:
        if (is_assignment_op(expr->binary.op)) {
            if (expr->type->kind == T_STRUCT) {
                ASSERT(expr->binary.op == TK_EQ, "Only direct assignment '=' is allowed between structs\n");
                return ir_memcpy(ctx, ir_gen_lvalue(ctx, expr->binary.rhs), ir_gen_lvalue(ctx, expr->binary.lhs), expr->type->size);
            }

            IR_Value addr = ir_gen_lvalue(ctx, expr->binary.lhs);
            IR_Value val = ir_gen_rvalue(ctx, expr->binary.rhs);
            if (val.kind == IR_SYMBOL && val.symbol->kind == FUNC) {
                // DEBUG("NEEDED IR_FUNC CHECK\n");
                val = ir_address(ctx, val, 0);
            }
            bool dereference = expr->binary.lhs->kind == N_INDEX || expr->binary.lhs->kind == N_MEMBER_ACCESS || is_deref(expr->binary.lhs);
            // If it is assignment & binary op
            if (expr->binary.op != TK_EQ) {
                IR_Value binop_val = ir_load(ctx, addr, dereference ? expr->binary.lhs->unary.expr->type : expr->binary.lhs->type);
                // if (expr->type->kind == T_POINTER) {
                //     PANIC("Cannot x= pointers rn\n");
                // }
                val = ir_binary(ctx, ir_binary_op(get_underlying_op(expr->binary.op)), ir_next_virtual_reg(ctx->func), binop_val, val,
                                expr->type);
            }
            ir_store(ctx, addr, val, expr->type);

            return val;
        }
        IR_Value lhs = ir_gen_rvalue(ctx, expr->binary.lhs);

        // Handle early branching for '&&' and '||' binary operations
        if (expr->binary.op == TK_OR_OR || expr->binary.op == TK_AND_AND) {
            IR_Value lhs_cmp = ir_cmp(ctx, NEQ, lhs, ir_integer_literal(0), expr->type);

            if (ir_is_within_cond(ctx)) {
                if (expr->binary.op == TK_AND_AND) ir_branch_cond(ctx, lhs_cmp, NULL, ctx->false_block);
                if (expr->binary.op == TK_OR_OR) ir_branch_cond(ctx, lhs_cmp, ctx->true_block, NULL);
            }

            IR_Value rhs = ir_gen_rvalue(ctx, expr->binary.rhs);
            IR_Value rhs_cmp = ir_cmp(ctx, NEQ, rhs, ir_integer_literal(0), expr->type);
            // No need to cmp both results, if we reach here it means lhs is 1 or rhs represents (lhs op rhs)
            // Early out
            if (ir_is_within_cond(ctx)) return rhs_cmp;

            // Otherwise generate the whole || or && result
            return ir_binary(ctx, expr->binary.op == TK_OR_OR ? BW_OR : BW_AND, ir_next_virtual_reg(ctx->func), lhs_cmp, rhs_cmp, type_i32);
        }

        IR_Value rhs = ir_gen_rvalue(ctx, expr->binary.rhs);
        if (is_comparison_op(expr->binary.op)) {
            return ir_cmp(ctx, ir_cmp_op(expr->binary.op), lhs, rhs, expr->type);
        }

        // Otherwise it is a arithmetic binary operation

        // Scale integer by ptr base size if pointer arithmetic
        if (expr->binary.lhs->type->kind == T_POINTER && expr->binary.rhs->type->kind == T_INT) {
            rhs =
                ir_binary(ctx, MUL, ir_next_virtual_reg(ctx->func), rhs, ir_integer_literal(expr->binary.lhs->type->base->size), type_i64);
        } else if (expr->binary.lhs->type->kind == T_INT && expr->binary.rhs->type->kind == T_POINTER) {
            lhs =
                ir_binary(ctx, MUL, ir_next_virtual_reg(ctx->func), lhs, ir_integer_literal(expr->binary.rhs->type->base->size), type_i64);
        }
        lhs = ir_binary(ctx, ir_binary_op(expr->binary.op), ir_next_virtual_reg(ctx->func), lhs, rhs, expr->type);

        // Divide (ptr - ptr) difference by base type to get correct result
        if (expr->binary.lhs->type->kind == T_POINTER && expr->binary.rhs->type->kind == T_POINTER) {
            Type *base = expr->binary.lhs->type->base;
            if (base->size == 1) return lhs;
            ASSERT(expr->type == type_i64, "Recieved non ptrdiff type in (ptr-ptr) binary op\n");
            return ir_binary(ctx, DIV, ir_next_virtual_reg(ctx->func), lhs, ir_integer_literal(base->size), expr->type);
        }
        return lhs;
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
            IR_Value val_addr = ir_gen_lvalue(ctx, expr->unary.expr);
            IR_Value val = ir_load(ctx, val_addr, expr->unary.expr->type);

            const IR_Value binary_dst = ir_binary(ctx, expr->unary.op == TK_INCR ? ADD : SUB, ir_next_virtual_reg(ctx->func), val,
                                                  ir_smart_const(ctx, &c, expr->type), expr->type);
            const IR_Value store_dst = ir_store(ctx, val_addr, binary_dst, expr->type);
            return expr->unary.associativity ? val : binary_dst;
        } else if (expr->unary.op == TK_AND) return ir_gen_lvalue(ctx, expr->unary.expr);                              // & ref
        else if (expr->unary.op == TK_MULTIPLY) return ir_load(ctx, ir_gen_rvalue(ctx, expr->unary.expr), expr->type); // * deref
        else if (expr->unary.op == TK_SIZEOF) return ir_integer_literal(expr->unary.expr->type->size);
        else return ir_unary(ctx, ir_unary_op(expr->unary.op), ir_gen_rvalue(ctx, expr->unary.expr), expr->type); // +/-/!/~(expr)
    case N_FUNCTION_CALL:
        return ir_call(ctx, expr);
    case N_CAST:
        if (expr->cast.from->kind == T_ARRAY && expr->type->kind == T_POINTER && expr->cast.from->base->kind == expr->type->base->kind) {
            // WARN("cast skipped HERE\n");
            return ir_gen_lvalue(ctx, expr->cast.expr);
            // return ir_gen_rvalue(ctx, expr->cast.expr);
        }
        return ir_cast(ctx, ir_gen_rvalue(ctx, expr->cast.expr), expr->type, expr->cast.from);
    case N_BUILTIN:
        return abi_gen_builtin(ctx, expr);
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
    free(cases);
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

static void ir_gen_init_list(IR_Context *ctx, IR_Value dst, int offset, Type *node_type, Node *l) {
    IR_Value zero = ir_integer_literal(0);
    int member_offset = 0;
    Type *type;
    IR_Value ir_v;

    switch (node_type->kind) {
    case T_INT:
    case T_FLOAT:
    case T_POINTER:
    case T_UNION:
        if (l->init_list.elements_array.count == 0) {
            type = node_type;
            ir_v = zero;
        } else {
            Node *e = get_node(&l->init_list.elements_array, 0);
            type = e->type;
            ir_v = ir_gen_rvalue(ctx, e->kind == N_DESIGNATED_INITIALIZER ? e->designated_init.value : e);
        }
        ir_store(ctx, dst, ir_v, type);
        break;
    case T_ARRAY:
    case T_STRUCT:
        bool is_array = node_type->kind == T_ARRAY;
        int len = is_array ? node_type->_array.array_len : node_type->_struct.members_array.count;

        if (is_array) {
            type = node_type->base;
        }

        for (int i = 0; i < len; i++) {
            Node *value = NULL;
            StructMember *member = is_array ? NULL : get_struct_member(node_type, i);
            for (int j = l->init_list.elements_array.count - 1; j >= 0; j--) {
                Node *e = get_node(&l->init_list.elements_array, j);
                if (is_array ? e->designated_init._array.index == i : strcmp(member->name, e->designated_init._struct.name) == 0) {
                    value = e->designated_init.value;
                    break;
                }
            }
            if (is_array) member_offset = type->align * i + offset;
            else {
                type = member->type;
                member_offset = member->offset + offset;
            }

            // If the corresponding value was found in the init list, use that, otherwise use a zero,
            if (value) {
                if (value->kind == N_INIT_LIST) ir_gen_init_list(ctx, dst, member_offset, type, value);
                else ir_v = ir_gen_rvalue(ctx, value);
            } else ir_v = zero;

            IR_Value final_dst = dst;
            if (member_offset)
                final_dst = ir_binary(ctx, ADD, ir_next_virtual_reg(ctx->func), dst, ir_integer_literal(member_offset), type_u64);
            ir_store(ctx, final_dst, ir_v, type);
        }
        break;
    default:
        PANIC("Recieving unsupported type to lower var decl with initlist\n");
    }
    return;
}

static void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl) {
    // Handle globals seperately to locals
    if (var_decl->var_decl.is_global) {
        IR_Literal x;
        IR_Literal *l = &x;
        if (var_decl->var_decl.is_defined) {
            Node *x = var_decl->var_decl.expr;
            *l = ir_literal(var_decl->var_decl.expr);
        } else l = NULL;
        return ir_append_global(ctx->module, var_decl->var_decl.symbol, l);
    }

    append(&ctx->func->locals_array, &var_decl->var_decl.symbol);
    if (!var_decl->var_decl.is_defined) return;
    // Handle locals
    IR_Value dst = ir_gen_lvalue(ctx, var_decl->var_decl.identifier);

    if (var_decl->var_decl.expr->kind == N_INIT_LIST) return ir_gen_init_list(ctx, dst, 0, var_decl->type, var_decl->var_decl.expr);

    IR_Value rhs = ir_gen_rvalue(ctx, var_decl->var_decl.expr);
    if (rhs.kind == IR_SYMBOL && rhs.symbol->kind == FUNC) rhs = ir_address(ctx, rhs, 0);
    if (var_decl->type->kind == T_ARRAY || var_decl->type->kind == T_STRUCT) {
        // ir_alloca(ctx, dst, align(var_decl->type->size, 8), 8);
        // dst = ir_address(ctx, dst, 0);
        ir_memcpy(ctx, rhs, dst, var_decl->type->size);
    } else ir_store(ctx, dst, rhs, var_decl->type);
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
    case N_BUILTIN:
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
    ir_return(ctx, return_value, _return->type);
}

static IR_Function *ir_gen_function(IR_Context *ctx, const Node *func) {
    if (func->kind != N_FUNCTION) {
        PANIC("Tried ir_gen_function but given node is not a function!\n");
    }
    if (!func->func.is_defined) {
        return NULL;
    }

    IR_Function *fn = ir_new_function(ctx, func->func.name, func->type);
    if (func->func.body->kind != N_COMPOUND) {
        PANIC("Function body is not a compound,\n");
    }

    /*
        Technically redundant to store func symbol, linkage can be infered from storage_class
        and storage is always STORAGE_TEXT
    */
    fn->linkage = func->func.symbol->linkage;
    fn->storage = func->func.symbol->storage;

    ir_begin_scope(fn);

    Type *abi_type = func->type->abi.type;
    ASSERT(abi_type, "Function did not recieve ABI type\n");

    abi_gen_params(ctx, fn);

    // handle {[statement]*}
    for (int i = 0; i < func->func.body->compound.items_array.count; i++) {
        ir_gen_block_item(ctx, get_node(&func->func.body->compound.items_array, i));
    }
    if (func->type->_func.return_type == type_void) ir_return(ctx, ir_no_value, type_void);

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
            if (n->func.is_defined) ir_append_function(ctx, ir_gen_function(ctx, n));
            break;
        case N_TYPEDEF:
        case N_TYPE:
            break;
        case N_VAR_DECL:
            if (n->var_decl.is_global && n->var_decl.storage_class == EXTERN) break;
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
