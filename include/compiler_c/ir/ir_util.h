#ifndef COMPILER_C_IR_UTIL_H
#define COMPILER_C_IR_UTIL_H

#include "compiler_c/ir/ir_module.h"
#include "compiler_c/tokenizer.h"

/* Converts token to IR_CMP_OP */
IR_CMP_OP ir_cmp_op(const TokenType type);
/* Converts token to IR_UNARY_OP */
IR_UNARY_OP ir_unary_op(const TokenType type);
/* Converts token to IR_BINARY_OP */
IR_BINOP_OP ir_binary_op(const TokenType type);

void print_ir_module(IR_Context *ctx,const IR_Module *module);
void print_ir_value(const IR_Value *v);

static void print_ir_function(IR_Context *ctx, const IR_Function *func);
static void print_ir_block(IR_Context *ctx, const IR_Block *block);
void print_ir_instruction(IR_Context *ctx,const IR_Instruction *instr);

static void print_unary_op(IR_UNARY_OP op);
static void print_binary_op(IR_BINOP_OP op);
static void print_cmp_op(IR_CMP_OP op);

#endif // COMPILER_C_IR_UTIL_H
