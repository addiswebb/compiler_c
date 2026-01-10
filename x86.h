#ifndef COMPILER_C_X86_H
#define COMPILER_C_X86_H

#include <stdio.h>

#include "ir.h"

/*
    x86-64 Assembly
    AT&T dialect
    System V AMD 64
*/

void x86_gen_instruction(FILE *fp, const IR_Instruction *instr);
void x86_gen_block(FILE *fp, const IR_Block *block);
void x86_gen_function(FILE *fp, const IR_Function *func);
void x86_gen_module(FILE *fp, const IR_Module *module);

#endif // COMPILER_C_X86_H
