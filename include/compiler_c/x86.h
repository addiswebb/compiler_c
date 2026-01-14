#ifndef COMPILER_C_X86_H
#define COMPILER_C_X86_H

#include <stdio.h>

#include "ir.h"

/*
    x86-64 Assembly
    AT&T dialect
    System V AMD 64
*/

void x86_gen_cast_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_const_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_unary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_binary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_instruction(FILE *fp,IR_Context *ctx, const IR_Instruction *instr);
void x86_gen_block(FILE *fp, IR_Context *ctx);
void x86_gen_function(FILE *fp, IR_Context *ctx);
void x86_gen_module(FILE *fp, IR_Context *ctx);

#endif // COMPILER_C_X86_H
