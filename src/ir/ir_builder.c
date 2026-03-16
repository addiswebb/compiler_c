#include "compiler_c/core/array.h"
#include "compiler_c/ir/ir_gen.h"
#include <stdio.h>
#include <stdlib.h>

IR_Value ir_load(IR_Context *ctx, IR_Value addr, Type *type) {
    IR_Instruction i;
    i.op = IR_LOAD;
    i.ops[1] = addr;
    i.load.type = type;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_store(IR_Context *ctx, IR_Value dst, IR_Value src, Type *type) {
    IR_Instruction i;
    i.op = IR_STORE;
    i.ops[1] = src;
    i.store.type = type;
    i.ops[0] = dst;
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_store_mem(IR_Context *ctx, IR_Value dst, IR_Value src, Type *type) {
    IR_Instruction i;
    i.op = IR_STORE_MEM;
    i.ops[1] = src;
    i.store.type = type;
    i.ops[0] = dst;
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
// TODO: Always use the .LCx label, and replace it in analysis with the int literal (if type is compatible integer)
IR_Value ir_const(IR_Context *ctx, IR_Value c, Type *type) {
    IR_Instruction i;
    i.op = IR_CONST;
    i.ops[1] = c;
    i._const.type = type;
    // Use the .LCx literal for strings, otherwise it was lowered to an asm literal and stored in a register.
    i.ops[0] = type->kind == T_ARRAY && type->base == type_i8 ? c : ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_unary(IR_Context *ctx, IR_UNARY_OP op, IR_Value expr_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_UNOP;
    i.unary.op = op;
    i.unary.type = type;
    i.ops[1] = expr_reg;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_binary(IR_Context *ctx, IR_BINOP_OP op, IR_Value dst, IR_Value lhs_reg, IR_Value rhs_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_BINOP;
    i.binop.op = op;
    i.binop.type = type;
    i.ops[1] = lhs_reg;
    i.ops[2] = rhs_reg;
    i.ops[0] = dst;
    i.op_count = 3;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_cmp(IR_Context *ctx, IR_CMP_OP op, IR_Value lhs_reg, IR_Value rhs_reg, Type *type) {
    IR_Instruction i;
    i.op = IR_CMP;
    i.cmp.op = op;
    i.cmp.type = type;
    i.ops[1] = lhs_reg;
    i.ops[2] = rhs_reg;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 3;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_call(IR_Context *ctx, const Node *expr) {
    IR_Instruction i;
    i.op = IR_CALL;
    i.call.callee = ir_get_func_def(ctx, expr->func_call.identifier->identifier.name);
    array_init(&i.call.arg_array, expr->func_call.params_array.count, sizeof(IR_Var));
    i.call.type = expr->type; // TODO change to func def given type maybe? Currently trusting sema
    for (int j = 0; j < i.call.arg_array.capacity; j++) {
        Node *param = get_node(&expr->func_call.params_array, j);
        append(&i.call.arg_array, &(IR_Var){.name = NULL, .type = param->type, .reg = ir_gen_rvalue(ctx, param)});
    }
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 1;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_return(IR_Context *ctx, IR_Value reg) {
    IR_Instruction i;
    i.op = IR_RET;
    i.ops[0] = reg;
    i.op_count = 1;
    append(&ctx->block->instruction_array, &i);
    return ir_no_value;
}
IR_Value ir_branch(IR_Context *ctx, IR_Block *block) {
    IR_Instruction i;
    i.op = IR_BR;
    i.br.block = block;
    i.op_count = 0;
    append(&ctx->block->instruction_array, &i);
    return ir_no_value;
}

IR_Value ir_jmp(IR_Context *ctx, const char *name) {
    IR_Instruction i;
    i.op = IR_JMP;
    i.jmp.name = name;
    i.op_count = 0;
    append(&ctx->block->instruction_array, &i);
    return ir_no_value;
}

IR_Value ir_label(IR_Context *ctx, const char *name) {
    IR_Instruction i;
    i.op = IR_LABEL;
    i.label.name = name;
    i.op_count = 0;
    append(&ctx->block->instruction_array, &i);
    return ir_no_value;
}
IR_Value ir_branch_cond(IR_Context *ctx, IR_Value cond_reg, IR_Block *t_block, IR_Block *f_block) {
    IR_Instruction i;
    i.op = IR_BR_COND;
    i.ops[0] = cond_reg;
    i.br_cond.t_block = t_block;
    i.br_cond.f_block = f_block;
    i.op_count = 1;
    append(&ctx->block->instruction_array, &i);
    return ir_no_value;
}
IR_Value ir_cast(IR_Context *ctx, IR_Value src, Type *to, Type *from) {
    IR_Instruction i;
    i.op = IR_CAST;
    i.cast.from = from;
    i.cast.to = to;
    i.ops[1] = src;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_address(IR_Context *ctx, IR_Value src, int offset) {
    IR_Instruction i;
    i.op = IR_ADDR;
    i.ops[1] = src;
    i.addr.offset = offset;
    i.ops[1].offset = offset;
    i.ops[0] = ir_next_virtual_reg(ctx->func);
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
IR_Value ir_alloca(IR_Context *ctx, IR_Value dst, int size, int al) {
    IR_Instruction i;
    i.op = IR_ALLOCA;
    i.alloca.size = size;
    i.op_count = 1;
    i.ops[0] = dst;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}

IR_Value ir_memcpy(IR_Context *ctx, IR_Value from_reg, IR_Value to_reg, int size) {
    IR_Instruction i;
    i.op = IR_MEMCPY;
    i.ops[1] = from_reg;
    i.memcpy.size = size;
    i.ops[0] = to_reg;
    i.op_count = 2;
    append(&ctx->block->instruction_array, &i);
    return i.ops[0];
}
