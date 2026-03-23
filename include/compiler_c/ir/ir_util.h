#ifndef COMPILER_C_IR_UTIL_H
#define COMPILER_C_IR_UTIL_H

#include "compiler_c/ir/ir_module.h"
#include "compiler_c/tokenize/tokenizer.h"

/* Converts token to IR_CMP_OP */
IR_CMP_OP ir_cmp_op(const TokenType type);
/* Converts token to IR_UNARY_OP */
IR_UNARY_OP ir_unary_op(const TokenType type);
/* Converts token to IR_BINARY_OP */
IR_BINOP_OP ir_binary_op(const TokenType type);

void print_ir_module(const IR_Context *ctx,const IR_Module *module);
void print_ir_value(const IR_Value *v);
void print_ir_phys_reg(const PhysReg *r);

static void print_ir_block(const IR_Context *ctx, const IR_Block *block);
void print_ir_function(const IR_Context *ctx, const IR_Function *func);
void print_ir_instruction(const IR_Context *ctx,const IR_Instruction *instr);

static void print_unary_op(IR_UNARY_OP op);
static void print_binary_op(IR_BINOP_OP op);
static void print_cmp_op(IR_CMP_OP op);

#endif // COMPILER_C_IR_UTIL_H
