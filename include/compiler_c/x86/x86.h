#ifndef COMPILER_C_X86_H
#define COMPILER_C_X86_H

#include "compiler_c/ir/ir_module.h"

#include <stdio.h>
#include <stdint.h>

#define MAX_OPERAND_BUFFER_SIZE 64

void x86_gen_module(FILE *fp, IR_Context *ctx);

void x86_operand(const IR_Value *v, char *buf, int n);
void x86_emit_rx(FILE *fp, const char *instr, const char *s1, const char *s2, const char *src, const IR_Value *dst);
void x86_emit_xr(FILE *fp, const char *instr, const char *s1, const char *s2, const IR_Value *src, const char *dst);
void x86_emit_rr(FILE *fp, const char *instr, const char *s1, const char *s2, const char *src, const char *dst);
void x86_emit_x(FILE *fp, const char *instr, const char *s1, const char *s2, const IR_Value *operand);
void x86_emit_r(FILE *fp, const char *instr, const char *s1, const char *s2, const char *r);

const char *x86_rax_reg(Type *t);
const char *x86_rbx_reg(const Type *t);
const char *x86_rcx_reg(const Type *t);
const char *x86_rdx_reg(const Type *t);
const char *x86_op_suffix(const Type *t);

void x86_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_emit_binary(FILE *fp, const IR_Value *dst, const IR_Value *lhs, const IR_Value *rhs, IR_BINOP_OP op, Type *t);
void x86_emit_unary(FILE *fp, const IR_Value *dst, const IR_Value *expr, IR_UNARY_OP op, Type *t);
void x86_emit_cast(FILE *fp, const IR_Value *src, const IR_Value *dst, Type *from, Type *to);
void x86_emit_const(FILE *fp, const IR_Value *dst, Type *t, const IR_Literal *c, int pool_index);
void x86_emit_store(FILE *fp, const IR_Value *src, const IR_Value *dst, Type *t);
void x86_emit_store_mem(FILE *fp,const  IR_Value *src, const IR_Value *dst, const Type *t);
void x86_emit_load(FILE *fp, const IR_Value *addr, const IR_Value *dst, Type *t);
void x86_emit_string(FILE *fp, const char *str);

static void x86_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr);
static void x86_gen_addr_instruction(FILE *fp, const IR_Instruction *instr);
static void x86_gen_cast_instruction(FILE *fp, const IR_Instruction *instr);
static void x86_gen_const_instruction(FILE *fp, const IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_unary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_binary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
static void x86_gen_block(FILE *fp, IR_Context *ctx);
static void x86_gen_function(FILE *fp, IR_Context *ctx);
#endif // COMPILER_C_X86_H
