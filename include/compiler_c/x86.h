#ifndef COMPILER_C_X86_H
#define COMPILER_C_X86_H

#include "compiler_c/ir.h"
#include <stdio.h>
#include <stdint.h>

void x86_gen_module(FILE *fp, IR_Context *ctx);

static void x86_gen_addr_instr(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
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
