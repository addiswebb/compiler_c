#ifndef COMPILER_C_IR_MODULE_H
#define COMPILER_C_IR_MODULE_H

#include "compiler_c/node.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/type.h"
#include "compiler_c/analysis_types.h"
#include <stdint.h>

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
    BW_AND,
    L_AND,
    BW_OR,
    L_OR,
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
    IR_LOAD, IR_STORE, IR_STORE_MEM, IR_RET, IR_CALL,
    IR_BR, IR_BR_COND,
    IR_CMP,
    IR_CAST,
    IR_ADDR,
    IR_ALLOCA,
    IR_MEMCPY,
} IR_OP;

typedef enum{
     IR_UNDEFINED, IR_PHYS_REG, IR_VREG, IR_MEM, IR_STACK, IR_LITERAL, IR_GLOBAL
}IR_ValueKind;

typedef struct{
    Type *type;
    union{
        int64_t i;
        double f;
        struct {
            const char* data;
            int len;
        }s;
    };
}IR_Literal;

typedef struct{
    const char *name;
    Type *type;
    IR_Literal val;
    Storage storage;
    Linkage linkage;
}IR_Global;

typedef struct PhysReg PhysReg;

typedef struct IR_Value{
    IR_ValueKind kind;
    union{
        // TODO: seperate IR_REG and IR_STACK data
        // IR_VREG
        struct{
            int reg;
        // IR_STACK
            int stack_slot;
            int stack_offset;
        };
        // IR_MEM
        struct{
            int mem;
            int offset;
        };
        // IR_LITERAL
        int const_index;
        // IR_GLOBAL
        IR_Global *global;
        // IR_PHYS_REG
        PhysReg phys_reg;
    };
    int size;
    int align;
}IR_Value;

typedef struct {
    const char *name;
    IR_Value reg;
    Type *type;
} IR_Var;

typedef struct{
    uint8_t def_mask;
    uint8_t use_mask;
} IR_OpInfo;

extern IR_OpInfo op_info[];
typedef struct IR_Block IR_Block;

typedef struct{
    const char *name;
    int index;
    bool is_defined;
} IR_Func_Def;

typedef struct {
    IR_OP op;
    IR_Value ops[3];
    int op_count;
    union {
        struct { Type *type; } _const;
        struct { Type *type; } load;
        struct { Type *type; } store;
        struct { IR_UNARY_OP op; Type *type; } unary;
        struct { IR_BINOP_OP op; Type *type; } binop;
        struct { IR_CMP_OP op; } cmp;
        struct { IR_Func_Def* callee; IR_Var *args; int arg_count; Type *type; } call;
        struct { IR_Block *block; } br;
        struct { IR_Block *t_block, *f_block; } br_cond;
        struct { Type *from, *to; } cast;
        struct { int offset; } addr;
        struct { int size; } alloca;
        struct { int size; } memcpy;
    };
} IR_Instruction;

typedef struct{
    int id;
    int offset;
    int size;
    int align;
    int free_at;
}StackSlot;

typedef struct{
    int *data;
    int num_bits;
    int capacity;
} BitSet;

typedef struct{
    int *succ;
    int succ_count;
    int *pred;
    int pred_count;
}IR_BlockCFG;

typedef struct{
    BitSet def;
    BitSet use;
    BitSet live_in;
    BitSet live_out;
}IR_BlockLiveness;

typedef struct{
    int reg;
    int start;
    int end;
    int stack_slot;
    int stack_offset;
    IR_Value *v;
} Lifetime;

struct IR_Block {
    int id;
    IR_Instruction *instructions;
    int count;
    int capacity;
    IR_BlockCFG cfg;
    IR_BlockLiveness live;
};

typedef struct {
    int var_count;
    int reg_count;
    int stack_pointer;
    int var_capacity;
    int *var_indices;
} IR_Scope;

typedef struct {
    const char *name;
    IR_Block **blocks;
    int block_count;
    int block_capacity;
    int next_reg;
    int max_reg;
    int stack_size;
    IR_Var *locals;
    int local_count;
    int local_capacity;
    IR_Scope *scopes;
    int scope_count;
    int scope_capacity;
    StackSlot *stack_slots;
    int stack_slot_count;
    int stack_slot_capacity;
    Linkage linkage;
    Storage storage;
    int param_count;
} IR_Function;


typedef struct{
    int count;
    int capacity;
    IR_Literal *consts;
}IR_Const_Pool;

typedef struct{
    int count;
    int capacity;
    IR_Global *globals;
}IR_Global_Pool;

typedef struct {
    IR_Function **functions;
    int func_count;
    int func_capacity;
    IR_Func_Def *func_defs;
    int func_def_count;
    int func_def_capacity;
    IR_Const_Pool const_pool;
    IR_Global_Pool global_pool;
} IR_Module;

typedef struct{
    IR_Block *continue_block;
    IR_Block *break_block;
} IR_LoopContext;

typedef struct{
    IR_LoopContext *data;
    int size;
    int capacity;
} IR_LoopStack;

typedef struct{
    IR_Module *module;
    IR_Function *func;
    IR_Block *block;
    IR_LoopStack loop_stack;
    IR_Block *true_block;
    IR_Block *false_block;
} IR_Context;

extern const IR_Value ir_no_value;

IR_Context ir_init_ctx();
void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block*break_block);
void ir_pop_loop_ctx(IR_Context *ctx);
IR_LoopContext *ir_loop_ctx(const IR_Context *ctx);

IR_Value ir_mem_value(int mem_reg, const Type *type);
IR_Value ir_vreg_value(int reg, const Type *type);

IR_Value ir_literal_value(int const_index);

void ir_free_module(IR_Module *module);
IR_Module *ir_gen_translation_unit(IR_Context *ctx,const Node *tu);

void ir_begin_scope(IR_Function *func);
void ir_end_scope(IR_Function *func);
IR_Value ir_next_virtual_slot(const IR_Function *func, int size, int align);
IR_Value ir_next_virtual_reg(IR_Function *func);

IR_Module *ir_new_module();
IR_Function *ir_new_function(IR_Context *ctx,const char *name);
IR_Func_Def *ir_append_func_def(const IR_Context *ctx, const char *name, bool is_defined);
IR_Value ir_new_var(IR_Function *func, const char *name, Type *type);
IR_Block *ir_new_block();

IR_Block *ir_add_block(IR_Context *ctx);
void ir_append_function(const IR_Context *ctx,IR_Func_Def *func_def, IR_Function *func);
void ir_append_instruction(IR_Block *block, const IR_Instruction *instruction);
void ir_append_global(IR_Module *module, const char *name, Type *type, const IR_Literal *literal, Linkage linkage, Storage storage);
IR_Value ir_append_const(IR_Module *module, const IR_Literal *literal);
IR_Block *ir_append_block(IR_Context *ctx, IR_Block *block);

IR_Func_Def *ir_get_func_def(const IR_Context *ctx, const char *name);
IR_Value ir_get_var_reg(const IR_Context *ctx, const char *name);
IR_Block *current_block(const IR_Function *func);

#endif
