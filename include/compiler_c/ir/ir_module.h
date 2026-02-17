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

/* A IR literal integer, float or string. */
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

/* Represents every possible way to represent values and memory in IR */
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

/* Represents a variable in IR */
typedef struct {
    const char *name;
    IR_Value reg;
    Type *type;
} IR_Var;

/* Determines of the 3 possible operands in an instruction, which are considered `used` or `defined` by the instruction. */
typedef struct{
    uint8_t def_mask;
    uint8_t use_mask;
} IR_OpInfo;

/* Global map between IR instructions and their corresponding operand infos */
extern IR_OpInfo op_info[];
typedef struct IR_Block IR_Block;

/* Serves as a map between function names and the index into IR_Module's functions array of the actual function. */
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

/* A physical, stack allocated slot of statically sized memory. Used for virtual registers and locals/IR_MEMs.*/
typedef struct{
    int id;
    int offset;
    int size;
    int align;
    int free_at;
}StackSlot;

/* Represents an array registers */
typedef struct{
    int *data;
    int num_bits;
    int capacity;
} BitSet;

/* A block's control flow graph data. Stores a static array of its successors and predecessors. */
typedef struct{
    int *succ;
    int succ_count;
    int *pred;
    int pred_count;
}IR_BlockCFG;

/*
    A block's liveness data.
    Stores whether any virtual register, is defined, used, lives in from a predecessorblock, lives out and used in another sucessor block.
*/
typedef struct{
    BitSet def;
    BitSet use;
    BitSet live_in;
    BitSet live_out;
}IR_BlockLiveness;

/*
    Represents the start and end liveness of a virtual register aswell as the actual stack slot and offset.
    Also stores a reference the operand which defines the virtual register.
*/
typedef struct{
    int reg;
    int start;
    int end;
    int stack_slot;
    int stack_offset;
    IR_Value *v;
} Lifetime;

/* IR Function component, stores an array of instructions. Handles virtual register livesness and control graph data. */
struct IR_Block {
    int id;
    Array instruction_array;
    IR_BlockCFG cfg;
    IR_BlockLiveness live;
};

/* Stores all local variables stored and registers used within a scopes lifetime */
typedef struct {
    int reg_count;
    int stack_pointer;
    Array var_array;
} IR_Scope;

typedef struct {
    const char *name;
    Array blocks_array;
    int next_reg;
    int max_reg;
    int stack_size;
    Array locals_array;
    Array scopes_array;
    StackSlot *stack_slots;
    int stack_slot_count;
    int stack_slot_capacity;
    Linkage linkage;
    Storage storage;
    int param_count;
} IR_Function;


/* Global global variable pool, stores definitions in a global array. */
typedef struct{
    int count;
    int capacity;
    IR_Global *globals;
}IR_Global_Pool;

/* Root of the intermediate representation of a source file. */
typedef struct {
    IR_Function **functions;
    int func_count;
    int func_capacity;
    Array func_defs_array;
    Array const_array;
    Array global_array;
} IR_Module;

/* Holds the correct block to jump to in the event of a `continue` or `break` statement. */
typedef struct{
    IR_Block *continue_block;
    IR_Block *break_block;
} IR_LoopContext;

/* Dynamic stack of LoopContexts to handle nested loops. */
typedef struct{
    IR_LoopContext *data;
    int size;
    int capacity;
} IR_LoopStack;
/*
    Stores the current and in use module, function, block.
    Stores the true/false blocks to early jump out of (a && b) conditions.
    Holds the loopstack also.
*/
typedef struct{
    IR_Module *module;
    IR_Function *func;
    IR_Block *block;
    Array loop_stack_array;
    IR_Block *true_block;
    IR_Block *false_block;
} IR_Context;

/* The IR_Value used for instructions which do not have a return value / destination register */
extern const IR_Value ir_no_value;

/* Initializes the IR Context and allocates memory for the dynamic loop stack. */
IR_Context ir_init_ctx();

/* Root call to generate IR for a given source file/translation unit. Expects the root AST node, N_TRANSLATION_UNIT. */
IR_Module *ir_gen_translation_unit(IR_Context *ctx,const Node *tu);

/* Append a LoopContext to the top of the loopstack. */
void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block*break_block);
/* Pop off the top of the loopstack. */
void ir_pop_loop_ctx(IR_Context *ctx);

/* Returns an IR Mem Value using the `mem_reg` mem slot. */
IR_Value ir_mem_value(int mem_reg, const Type *type);
/* Returns an IR VReg Value using the `reg` register. */
IR_Value ir_vreg_value(int reg, const Type *type);
/* Returns an IR Const Value using the constant at `const_index`. */
IR_Value ir_literal_value(int const_index);

void ir_free_module(IR_Module *module);

/* Begins a new scope to track locals defined. */
void ir_begin_scope(IR_Function *func);
/* Ends the scope, popping all locals defined within it. */
void ir_end_scope(IR_Function *func);

/* Returns an IR Mem Value pointing to a new virtual slot with `size` and `align`. */
IR_Value ir_next_virtual_slot(const IR_Function *func, int size, int align);
/* Returns an IR VReg Value pointing to the next virtual register */
IR_Value ir_next_virtual_reg(IR_Function *func);

/* Initializes IR Module, and functions, func_defs, globals, and consts arrays. */
IR_Module *ir_new_module();

/* Creates a new IR Function with initialized blocks, locals, scopes and stack slots arrays. */
IR_Function *ir_new_function(IR_Context *ctx,const char *name);
/*
    Appends a function definition to the global array.
    At this point the FuncDef is a placeholder. Its actual index is -1/ undefined.
    If it is defined, immediately after, its index will be updated.
    Otherwise it gets updated when a defined N_Function node is lowered.
*/
IR_Func_Def *ir_append_func_def(const IR_Context *ctx, const char *name, bool is_defined);
/* Generates an IR Mem Value for the variable. Also appends it to the current scope.  */
IR_Value ir_new_var(IR_Function *func, const char *name, Type *type);

/* Creates a new empty IR Block and initializes its instructions array. */
IR_Block *ir_new_block();
/* Handles creating a new block and appending it to the current function's blocks array. */
IR_Block *ir_add_block(IR_Context *ctx);
/*
    Appends the IR Function to the modules function array.
    Also handles updating the corresponding IR FuncDef with the correct index into the function array,
    FuncDef is also defined at this point.
*/
void ir_append_function(const IR_Context *ctx,IR_Func_Def *func_def, IR_Function *func);
/* Appends the given global variable to the module's global dynamic variable array. */
void ir_append_global(IR_Module *module, const char *name, Type *type, const IR_Literal *literal, Linkage linkage, Storage storage);
/* Appends the given Literal to the module's dynamic const array. */
IR_Value ir_append_const(IR_Module *module, const IR_Literal *literal);
/* Appends the given IR Block to the context's current function. */
IR_Block *ir_append_block(IR_Context *ctx, IR_Block *block);

IR_Func_Def *ir_get_func_def(const IR_Context *ctx, const char *name);
/*
    Retrieves the IR Value corresponding to the given variable name.
    First checks the scope stack, top down. Then checks module globals array.
    Immediately returning the Value if found, otherwise it is considered an undefined variable.
*/
IR_Value ir_get_var_reg(IR_Context *ctx, const char *name);
/* Gets the most recently added block of a function. */

static inline IR_Instruction *get_instruction(const Array *arr, int index){
    return (IR_Instruction *) get(arr, index);
}

/* Retrieve the current LoopContext, which lies at the top of the loopstack. */
static inline IR_LoopContext * ir_loop_ctx(const IR_Context *ctx){
    return (IR_LoopContext*) get(&ctx->loop_stack_array, ctx->loop_stack_array.count-1);
}

static inline IR_Scope * get_current_scope(const IR_Function *func){
    return (IR_Scope*) get(&func->scopes_array, func->scopes_array.count - 1);
}

static inline IR_Scope * get_scope(const IR_Function *func, int index){
    return (IR_Scope*) get(&func->scopes_array, index);
}

static inline IR_Block * get_block(const IR_Function *func, int index){
    return *(IR_Block**) get(&func->blocks_array, index);
}

static inline IR_Literal * get_const(const IR_Context *ctx, int index){
    return (IR_Literal*) get(&ctx->module->const_array, index);
}

static inline IR_Global * get_global(const IR_Context *ctx, int index){
    return (IR_Global*) get(&ctx->module->global_array, index);
}
static inline IR_Var * get_local(const IR_Function *func, int index){
    return (IR_Var *) get(&func->locals_array, index);
}

static inline int get_var_index(const IR_Scope *scope, int index){
    return *(int *) get(&scope->var_array, index);
}

static inline IR_Func_Def * get_func_def(const IR_Context *ctx, int index){
    return (IR_Func_Def*) get(&ctx->module->func_defs_array, index);
}
#endif // COMPILER_C_IR_MODULE_H
