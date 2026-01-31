#ifndef COMPILER_C_IR_BUILDER_H
#define COMPILER_C_IR_BUILDER_H

#include "compiler_c/ir/ir_module.h"

IR_Value ir_load(IR_Context *ctx, IR_Value addr, Type *type);
IR_Value ir_store(IR_Context *ctx, IR_Value dst, IR_Value addr, Type *type);
IR_Value ir_store_mem(IR_Context *ctx, IR_Value dst, IR_Value src, Type *type);
IR_Value ir_const(IR_Context *ctx, IR_Value c, Type *type);
IR_Value ir_unary(IR_Context *ctx, IR_UNARY_OP op, IR_Value expr_reg, Type *type);
IR_Value ir_binary(IR_Context *ctx, IR_BINOP_OP op, IR_Value dst, IR_Value lhs_reg, IR_Value rhs_reg, Type *type);
IR_Value ir_cmp(IR_Context *ctx, IR_CMP_OP op, IR_Value lhs_reg,IR_Value rhs_reg );
IR_Value ir_call(IR_Context *ctx, const Node *expr);
IR_Value ir_return(IR_Context *ctx,IR_Value reg);
IR_Value ir_branch(IR_Context *ctx, IR_Block *block);
IR_Value ir_branch_cond(IR_Context *ctx,IR_Value cond_reg, IR_Block *t_block, IR_Block *f_block);
IR_Value ir_cast(IR_Context *ctx,IR_Value src, Type *to, Type *from);
IR_Value ir_address(IR_Context *ctx,IR_Value src, int offset);
IR_Value ir_alloca(IR_Context *ctx, IR_Value dst, int size, int al);
IR_Value ir_memcpy(IR_Context *ctx, IR_Value from_reg, IR_Value to_reg, int size);

#endif
