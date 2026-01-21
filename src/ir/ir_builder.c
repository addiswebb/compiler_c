#include "compiler_c/ir/ir_gen.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/node.h"
#include <stdio.h>
#include <stdlib.h>

IR_Value ir_load(IR_Context *ctx, IR_Value addr, Type *type) {
    IR_Instruction i;
    i.op = IR_LOAD;
    i.load.addr = addr;
    i.load.type = type;
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_store(IR_Context *ctx, IR_Value dst, IR_Value src, Type *type) {
    IR_Instruction i;
    i.op = IR_STORE;
    i.store.src = src;
    i.store.type = type;
    if (dst.kind != IR_MEM) {
        printf("Store destination must be a var memory slot\n");
        exit(1);
    }
    i.dst = dst;
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_const(IR_Context *ctx, IR_Value c, Type *type) {
    IR_Instruction i;
    i.op = IR_CONST;
    i._const.c = c;
    i._const.type = type;
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_unary(IR_Context *ctx, IR_UNARY_OP op, IR_Value expr_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_UNOP;
    i.unary.op = op;
    i.unary.expr = expr_reg;
    i.unary.type = type;
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_binary(IR_Context *ctx, IR_BINOP_OP op, IR_Value dst, IR_Value lhs_reg, IR_Value rhs_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_BINOP;
    i.binop.op = op;
    i.binop.lhs = lhs_reg;
    i.binop.rhs = rhs_reg;
    i.binop.type = type;
    i.dst = dst;
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_cmp(IR_Context *ctx, IR_CMP_OP op, IR_Value lhs_reg, IR_Value rhs_reg) {
    IR_Instruction i;
    i.op = IR_CMP;
    i.cmp.op = op;
    i.cmp.lhs = lhs_reg;
    i.cmp.rhs = rhs_reg;
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_call(IR_Context *ctx, const Node *expr) {
    IR_Instruction i;
    i.op = IR_CALL;
    i.call.callee = ir_get_func_def(ctx, expr->func_call.identifier->identifier.name);
    i.call.arg_count = expr->func_call.param_count;
    i.call.args = malloc(sizeof(IR_Var) * i.call.arg_count);
    i.call.type = expr->type;
    if (!i.call.args) {
        printf("Failed to alloc for IR_FUNC_CALL args\n");
        exit(1);
    }
    for (int j = 0; j < i.call.arg_count; j++) {
        i.call.args[j].reg = ir_gen_rvalue(ctx, expr->func.params[j]);
        i.call.args[j].name = expr->func.params[j]->var_decl.name;
        i.call.args[j].type = expr->func.params[j]->type;
    }
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_return(IR_Context *ctx, IR_Value reg) {
    IR_Instruction i;
    i.op = IR_RET;
    i.ret.value = reg;
    i.dst = ir_reg_value(-1);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_branch(IR_Context *ctx, int label) {
    IR_Instruction i;
    i.op = IR_BR;
    i.br.label = label;
    i.dst = ir_reg_value(-1);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_branch_cond(IR_Context *ctx, IR_Value cond_reg, int t_label, int f_label) {
    IR_Instruction i;
    i.op = IR_BR_COND;
    i.br_cond.cond = cond_reg;
    i.br_cond.t_label = t_label;
    i.br_cond.f_label = f_label;
    i.dst = ir_reg_value(-1);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_cast(IR_Context *ctx, IR_Value src, Type *to, Type *from) {
    IR_Instruction i;
    i.op = IR_CAST;
    i.cast.from = from;
    i.cast.to = to;
    i.cast.src = src;
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_address(IR_Context *ctx, IR_Value src, int offset) {
    IR_Instruction i;
    i.op = IR_ADDR;
    i.addr.src = src;
    i.addr.offset = offset;
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
IR_Value ir_alloca(IR_Context *ctx, int size, int al) {
    IR_Instruction i;
    i.op = IR_ALLOCA;
    i.alloca.size = align(size, al);

    IR_Value v = ir_next_virtual_slot(ctx->func, i.alloca.size, al);
    i.dst = ir_next_virtual_reg(ctx->func);
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}

IR_Value ir_memcpy(IR_Context *ctx, IR_Value from_reg, IR_Value to_reg, int size) {
    IR_Instruction i;
    i.op = IR_MEMCPY;
    i.memcpy.from_reg = from_reg;
    i.memcpy.to_reg = to_reg;
    i.memcpy.size = size;
    i.dst = to_reg;
    ir_append_instruction(ctx->block, &i);
    return i.dst;
}
