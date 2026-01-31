#ifndef COMPILER_C_X86_H
#define COMPILER_C_X86_H

#include "compiler_c/ir/ir_module.h"

#include <stdio.h>
#include <stdint.h>

static int current_offset = 0;

void x86_gen_module(FILE *fp, IR_Context *ctx);

void x86_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_emit_binary(FILE *fp, int dst_reg, int lhs_reg, int rhs_reg, IR_BINOP_OP op, Type *t);
void x86_emit_unary(FILE *fp, int dst_reg, int expr_reg, IR_UNARY_OP op, Type *t);
void x86_emit_cast(FILE *fp, int src_reg, int dst_reg, Type *from, Type *to);
void x86_emit_const(FILE *fp, int dst_reg, Type *t, IR_Const *c, int pool_index);
void x86_emit_store(FILE *fp, int src_reg, int dst_reg, Type *t);
void x86_emit_store_mem(FILE *fp, int src_offset, int dst_offset, Type *t);
void x86_emit_load(FILE *fp, int addr_reg, int dst_reg, Type *t);

static void x86_gen_memcpy_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_alloca_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_addr_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_cast_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_const_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_unary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_binary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_block(FILE *fp, IR_Context *ctx);
static void x86_gen_function(FILE *fp, IR_Context *ctx);
#endif // COMPILER_C_X86_H
