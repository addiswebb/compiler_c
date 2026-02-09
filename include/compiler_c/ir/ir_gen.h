#ifndef COMPILER_C_IR_GEN_H
#define COMPILER_C_IR_GEN_H

#include "compiler_c/ir/ir_module.h"

static IR_Value ir_gen_lvalue(IR_Context *ctx, const Node *expr);
IR_Literal ir_gen_literal(const Node *node);
IR_Value ir_gen_rvalue(IR_Context *ctx, const Node *expr);
static void ir_gen_block_item(IR_Context *ctx, const Node *item);
static void ir_gen_compound(IR_Context *ctx, const Node *comp);
static void ir_gen_while_loop(IR_Context *ctx,const Node *_while);
static void ir_gen_for_loop(IR_Context *ctx,const Node *_for);
static void ir_gen_if_statement(IR_Context *ctx, const Node *_if);
static void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl);
static void ir_gen_statement(IR_Context *ctx,const Node *stmt);
static void ir_gen_return(IR_Context *ctx, const Node *_return);
static IR_Function *ir_gen_function(IR_Context *ctx, const Node *func);

int ir_within_cond(IR_Context *ctx);
void ir_set_cond_block(IR_Context *ctx, IR_Block *true_block, IR_Block *false_block);
void ir_reset_cond_block(IR_Context *ctx);
#endif
