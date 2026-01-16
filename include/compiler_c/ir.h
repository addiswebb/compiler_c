#ifndef COMPILER_C_IR_H
#define COMPILER_C_IR_H

#include "node.h"

typedef enum{
    LT, // <
    LE, // <=
    GT, // >
    GE, // >=
    EQ, // ==
    NEQ, // !=
} IR_CMP_OP;

typedef enum{
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    XOR,
    SHR,
    SHL,
} IR_BINOP_OP;

typedef enum{
    POS,
    NEG,
    LNOT,
    BNOT,
    REF,
    DEREF,
} IR_UNARY_OP;

typedef enum {
    IR_CONST,
    IR_UNARYOP,IR_BINOP,
    IR_LOAD, IR_STORE, IR_RET, IR_CALL,
    IR_BR, IR_BR_COND,
    IR_CMP,
    IR_CAST,
} IR_OP;

typedef struct {
    const char *name;
    int reg;
    Type *type;
} IR_Var;

typedef struct {
    IR_OP op;
    int dst;
    union {
        struct {int pool_index; Type *type; } _const;
        struct {int src; Type *type;} mov;
        struct {int addr; Type*type;} load;
        struct {int addr; Type*type; } store;
        struct {IR_UNARY_OP op; int expr; Type *type;} unary;
        struct {IR_BINOP_OP op; int lhs, rhs; Type *type;} binop;
        struct {IR_CMP_OP op; int lhs, rhs; } cmp;
        struct {int callee; int *args; int arg_count; Type *type;} call;
        struct {int value;} ret;
        struct {int label;} br;
        struct {int cond, t_label, f_label;} br_cond;
        struct {Type *from, *to; int src;} cast;
    };
} IR_Instruction;

typedef struct {
    IR_Instruction *instructions;
    int count;
    int capacity;
} IR_Block;

typedef struct {
    int var_count;
    int reg_count;
} IR_Scope;

typedef struct {
    const char *name;
    IR_Block *blocks;
    int block_count;
    int block_capacity;
    int next_reg;
    int max_reg;
    IR_Var *locals;
    int local_count;
    int local_capacity;
    IR_Scope *scopes;
    int scope_count;
    int scope_capacity;
} IR_Function;

typedef struct{
    const char *name;
    int index;
} IR_Func_Def;


typedef struct{
    Type *type;
    union{
        int i;
        float f;
        char c;
    };
}IR_Const;

typedef struct{
    int count;
    int capacity;
    IR_Const *consts;
}IR_Const_Pool;

typedef struct {
    IR_Function **functions;
    int func_count;
    int func_capacity;
    IR_Func_Def *defs;
    IR_Const_Pool const_pool;
} IR_Module;


typedef struct{
    IR_Module *module;
    IR_Function *func;
    IR_Block *block;
} IR_Context;

IR_CMP_OP ir_cmp_op(const TokenType type);
IR_UNARY_OP ir_unary_op(const TokenType type);
IR_BINOP_OP ir_binary_op(const TokenType type);

/*
    Begin an IR Scope,
    Tracks any variables added afterwards, and pops them from the IR virtual stack when `ir_end_scope()` is called.
*/
void ir_begin_scope(IR_Function *func);

/*
    Pops variables declared within the scope from the IR virtual stack.
*/
void ir_end_scope(IR_Function *func);

int ir_next_reg(IR_Function *func);

/*
    Allocates for a new IR Module,
    Also initializes/allocates for its functions array
*/
IR_Module *ir_new_module();
/*
    Allocates for a new IR_Block
*/
IR_Block *ir_new_block();
IR_Function *ir_new_function(IR_Context *ctx,const char *name);
int ir_new_var(IR_Function *func, const char *name, Type *type);

void ir_free_module(IR_Module *module);

void ir_new_func_def(IR_Module *module, IR_Function *func);
void ir_append_function(IR_Module *module, IR_Function *func);
int ir_append_block(IR_Context *ctx, IR_Block *block);
void ir_append_instruction(IR_Block *block, const IR_Instruction *instruction);
int ir_append_const(IR_Module *module, IR_Const *new_const);

int ir_get_func_def(const IR_Context *ctx, const char *name);
int ir_get_var_reg(const IR_Context *ctx, const char *name);

IR_Block *current_block(const IR_Function *func);

void ir_gen_block_item(IR_Context *ctx, const Node *item);
int ir_gen_expression(IR_Context *ctx,const Node *expr);
void ir_gen_compound(IR_Context *ctx, const Node *comp);
void ir_gen_while_loop(IR_Context *ctx,const Node *_while);
void ir_gen_for_loop(IR_Context *ctx,const Node *_for);
void ir_gen_if_statement(IR_Context *ctx, const Node *_if);
void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl);
void ir_gen_statement(IR_Context *ctx,const Node *stmt);
void ir_gen_return(IR_Context *ctx, const Node *_return);
IR_Function *ir_gen_function(IR_Context *ctx, const Node *func);
IR_Module *ir_gen_translation_unit(IR_Context *ctx,const Node *tu);

void print_unary_op(IR_UNARY_OP op);
void print_binary_op(IR_BINOP_OP op);
void print_cmp_op(IR_CMP_OP op);
void print_ir_op(IR_OP op);
void print_ir_instruction(IR_Context *ctx,const IR_Instruction *instr);
void print_ir_block(IR_Context *ctx, const IR_Block *block);
void print_ir_function(IR_Context *ctx, const IR_Function *func);
void print_ir_module(IR_Context *ctx,const IR_Module *module);

#endif // COMPILER_C_IR_H
