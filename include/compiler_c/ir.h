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
    IR_UNOP,IR_BINOP,
    IR_LOAD, IR_STORE, IR_RET, IR_CALL,
    IR_BR, IR_BR_COND,
    IR_CMP,
    IR_CAST,
    IR_ADDR,
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
        struct {int addr; Type*type;} load;
        struct {int src; Type*type; } store;
        struct {IR_UNARY_OP op; int expr; Type *type;} unary;
        struct {IR_BINOP_OP op; int lhs, rhs; Type *type;} binop;
        struct {IR_CMP_OP op; int lhs, rhs; } cmp;
        struct {int callee; int *args; int arg_count; Type *type;} call;
        struct {int value;} ret;
        struct {int label;} br;
        struct {int cond, t_label, f_label;} br_cond;
        struct {Type *from, *to; int src;} cast;
        struct {int src, offset;} addr;
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
        char* s;
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

// ir_module.c
void ir_free_module(IR_Module *module);
IR_Module *ir_gen_translation_unit(IR_Context *ctx,const Node *tu);

void ir_begin_scope(IR_Function *func);
void ir_end_scope(IR_Function *func);
int ir_next_reg(IR_Function *func);

IR_Module *ir_new_module();
IR_Function *ir_new_function(IR_Context *ctx,const char *name);
void ir_new_func_def(IR_Module *module, IR_Function *func);
int ir_new_var(IR_Function *func, const char *name, Type *type);
static IR_Block *ir_new_block();

int ir_add_block(IR_Context *ctx);
void ir_append_function(IR_Module *module, IR_Function *func);
void ir_append_instruction(IR_Block *block, const IR_Instruction *instruction);
int ir_append_const(IR_Module *module, IR_Const *new_const);
static int ir_append_block(IR_Context *ctx, IR_Block *block);

int ir_get_func_def(const IR_Context *ctx, const char *name);
int ir_get_var_reg(const IR_Context *ctx, const char *name);
IR_Block *current_block(const IR_Function *func);

// ir_gen.c
static int ir_gen_lvalue(IR_Context *ctx, const Node *expr);
int ir_gen_rvalue(IR_Context *ctx, const Node *expr);
static void ir_gen_block_item(IR_Context *ctx, const Node *item);
static void ir_gen_compound(IR_Context *ctx, const Node *comp);
static void ir_gen_while_loop(IR_Context *ctx,const Node *_while);
static void ir_gen_for_loop(IR_Context *ctx,const Node *_for);
static void ir_gen_if_statement(IR_Context *ctx, const Node *_if);
static void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl);
static void ir_gen_statement(IR_Context *ctx,const Node *stmt);
static void ir_gen_return(IR_Context *ctx, const Node *_return);
static IR_Function *ir_gen_function(IR_Context *ctx, const Node *func);

// ir_util.c
IR_CMP_OP ir_cmp_op(const TokenType type);
IR_UNARY_OP ir_unary_op(const TokenType type);
IR_BINOP_OP ir_binary_op(const TokenType type);

void print_ir_module(IR_Context *ctx,const IR_Module *module);

static void print_unary_op(IR_UNARY_OP op);
static void print_binary_op(IR_BINOP_OP op);
static void print_cmp_op(IR_CMP_OP op);
static void print_ir_op(IR_OP op);
static void print_ir_instruction(IR_Context *ctx,const IR_Instruction *instr);
static void print_ir_block(IR_Context *ctx, const IR_Block *block);
static void print_ir_function(IR_Context *ctx, const IR_Function *func);

// IR BUILDER:
int ir_load(IR_Context *ctx, int addr, Type *type);
int ir_store(IR_Context *ctx, int dst, int addr, Type *type);
int ir_const(IR_Context *ctx, int pool_index, Type *type);
int ir_unary(IR_Context *ctx, IR_UNARY_OP op, int expr_reg, Type *type);
int ir_binary(IR_Context *ctx, IR_BINOP_OP op, int dst, int lhs_reg, int rhs_reg, Type *type);
int ir_cmp(IR_Context *ctx, IR_CMP_OP op, int lhs_reg, int rhs_reg );
int ir_call(IR_Context *ctx, const Node *expr);
int ir_return(IR_Context *ctx, int reg);
int ir_branch(IR_Context *ctx, int label);
int ir_branch_cond(IR_Context *ctx, int cond_reg, int t_label, int f_label);
int ir_cast(IR_Context *ctx, int src, Type *to, Type *from);
int ir_address(IR_Context *ctx, int src, int offset);
#endif // COMPILER_C_IR_H
