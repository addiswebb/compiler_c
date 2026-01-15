#ifndef COMPILER_C_X86_H
#define COMPILER_C_X86_H

#include "compiler_c/ir.h"
#include <stdio.h>
#include <stdint.h>

void x86_gen_module(FILE *fp, IR_Context *ctx);

#endif // COMPILER_C_X86_H
