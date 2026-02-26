#ifndef COMPILER_C_IR_GEN_H
#define COMPILER_C_IR_GEN_H

#include "compiler_c/ir/ir_module.h"

/*
    Generates the corresponding IR instructions from the given node.
    Returns the IR Value which represents a way to access the memory the node represents.
    The IR Value must be assignable.
*/
static IR_Value ir_gen_lvalue(IR_Context *ctx, const Node *expr);
/* Lowers an N_LITERAL node to an IR_LITERAL. */
IR_Literal ir_literal(const Node *node);

/*
    Generates the corresponding IR instructions to evaluate the given expression node.
    Returns the IR VReg Value which holds the evaluation.
*/
IR_Value ir_gen_rvalue(IR_Context *ctx, const Node *expr);

static void ir_gen_block_item(IR_Context *ctx, const Node *item);
static void ir_gen_compound(IR_Context *ctx, const Node *comp);
static void ir_gen_while_loop(IR_Context *ctx,const Node *_while);
static void ir_gen_for_loop(IR_Context *ctx,const Node *_for);
static void ir_gen_if_statement(IR_Context *ctx, const Node *_if);
static void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl);
static void ir_gen_statement(IR_Context *ctx,const Node *stmt);
static void ir_gen_return(IR_Context *ctx, const Node *_return);
static void ir_gen_goto(IR_Context *ctx, const Node *_goto);
static void ir_gen_label(IR_Context *ctx, const Node *label);
static IR_Function *ir_gen_function(IR_Context *ctx, const Node *func);

int ir_is_within_cond(IR_Context *ctx);
/* Sets the context's true and false blocks, to allow for early out on (a && b) comparisons. */
void ir_set_cond_block(IR_Context *ctx, IR_Block *true_block, IR_Block *false_block);
/* Clears the context's true and false blocks, used at the end of a cond node. */
void ir_reset_cond_block(IR_Context *ctx);

#endif
