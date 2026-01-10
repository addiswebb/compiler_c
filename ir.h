#ifndef COMPILER_C_IR_H
#define COMPILER_C_IR_H

#include "node.h"

typedef enum { IR_ADD, IR_SUB, IR_MUL, IR_DIV, IR_LOAD, IR_STORE, IR_RET, IR_BR, IR_BR_EQ } IR_OP;

typedef struct {
    const char *name;
    int reg;
} IR_Var;

typedef struct {
    IR_OP op;
    int dst;
    int a;
    int b;
} IR_Instruction;

typedef struct {
    IR_Instruction *instructions;
    int count;
    int capacity;
} IR_Block;

typedef struct {
    int var_count;
} IR_Scope;

typedef struct {
    const char *name;
    IR_Block *blocks;
    int block_count;
    int block_capacity;
    int next_reg;
    IR_Var *locals;
    int local_count;
    int local_capacity;
    IR_Scope *scopes;
    int scope_count;
    int scope_capacity;
} IR_Function;

typedef struct {
    IR_Function **functions;
    int count;
    int capacity;
} IR_Module;

IR_OP token_to_ir_op(TokenType type);

/*
    Begin an IR Scope,
    Tracks any variables added afterwards, and pops them from the IR virtual stack when `ir_end_scope()` is called.
*/
void ir_begin_scope(IR_Function *func);

/*
    Pops variables declared within the scope from the IR virtual stack.
*/
void ir_end_scope(IR_Function *func);

/*
    Allocates for a new IR Module,
    Also initializes/allocates for its functions array
*/
IR_Module *ir_new_module();
/*
    Allocates for a new IR_Block
*/
IR_Block *ir_new_block();
IR_Function *ir_new_function(const char *name);
int ir_new_var(IR_Function *func, const char *name);

void ir_free_module(IR_Module *module);

void ir_append_function(IR_Module *module, IR_Function *func);
int ir_append_block(IR_Function *func, IR_Block *block);
void ir_append_instruction(IR_Block *block, IR_Instruction *instruction);

int ir_get_var_reg(IR_Function *func, const char *name);

IR_Block *current_block(const IR_Function *func);

int ir_gen_expression(IR_Function *func, Node *expr);
void ir_gen_compound(IR_Function *func, const Node *comp);
void ir_gen_while_statement(IR_Function *func, Node *_while);
void ir_gen_if_statement(IR_Function *func, const Node *_if);
void ir_gen_statement(IR_Function *func, Node *stmt);
IR_Function *ir_gen_function(const Node *func);
IR_Module *ir_gen_translation_unit(const Node *tu);

void print_ir_op(IR_OP op);
void print_ir_instruction(const IR_Instruction *instr);
void print_ir_block(const IR_Block *block);
void print_ir_function(const IR_Function *func);
void print_ir_module(const IR_Module *module);

#endif // COMPILER_C_IR_H
