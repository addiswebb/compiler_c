#ifndef COMPILER_C_IR_MODULE_H
#define COMPILER_C_IR_MODULE_H

#define DEBUG_LOWERED_IR 1
#define DEBUG_IR_INSTRUCTIONS 1

#include "compiler_c/core/node.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/analyse/analysis_types.h"
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
    IR_LOAD, IR_STORE, IR_RET, IR_CALL,
    IR_BR, IR_BR_COND,
    IR_CMP,
    IR_CAST,
    IR_ADDR,
    IR_ALLOCA,
    IR_MEMCPY,
    IR_LABEL,
    IR_JMP,
    IR_BUILTIN_VA_START,
    IR_BUILTIN_VA_ARG,
} IR_OP;

typedef enum{
     IR_UNDEFINED, IR_SYMBOL, IR_VREG, IR_CONSTANT, IR_PHYS_REG, IR_INT_LITERAL,
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
    Symbol *symbol;
    IR_Literal val;
}IR_Global;

typedef struct PhysReg PhysReg;

/* Represents every possible way to represent values and memory in IR */
typedef struct IR_Value{
    IR_ValueKind kind;
    int size;
    int align;
    union{
        // IR_SYMBOL
        Symbol *symbol;
        // IR_VREG
        int vreg;
        // IR_CONSTANT
        int const_index;
        // IR_PHYS_REG
        PhysReg phys_reg;
        int64_t int_literal;
    };
}IR_Value;

typedef struct{
    Type *type;
    IR_Value v;
}IR_CallArg;

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
    bool is_variadic;
    StorageClass storage_class;
} IR_Func_Def;

typedef struct {
    IR_OP op;
    IR_Value ops[3];
    int op_count;
    union {
        struct { Type *type; } _const;
        struct { Type *type; } load;
        struct { Type *type; } store;
        struct { Type *type; } ret;
        struct { IR_UNARY_OP op; Type *type; } unary;
        struct { IR_BINOP_OP op; Type *type; } binop;
        struct { IR_CMP_OP op; Type *type; } cmp;
        struct { Array arg_array; Type *type; } call;
        struct { IR_Block *block; } br;
        struct { IR_Block *t_block, *f_block; } br_cond;
        struct { Type *from, *to; } cast;
        struct { int offset; } addr;
        struct { int size; } alloca;
        struct { int size; } memcpy;
        struct { const char *name; } label;
        struct { const char *name; } jmp;
        struct {  Type *type;} builtin_va_arg;
    };
} IR_Instruction;

/* A physical, stack allocated slot of statically sized memory. Used for virtual registers and locals/IR_MEMs.*/
typedef struct{
    int offset;
    int size;
    int align;
    int free_at;
}StackSlot;

/* Represents an array registers */
typedef struct{
    unsigned int *data;
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
    int stack_slot_count;
    int stack_slot_capacity;
    Linkage linkage;
    Storage storage;
    Type *return_type;
} IR_Function;


/* Global global variable pool, stores definitions in a global array. */
typedef struct{
    int count;
    int capacity;
    IR_Global *globals;
}IR_Global_Pool;

typedef struct{
    IR_Block *block;
    const char *label;
    bool placeholder;
}IR_LabeledBlock;

/* Root of the intermediate representation of a source file. */
typedef struct {
    Array functions_array;
    Array func_defs_array;
    Array const_array;
    Array global_array;
    Array labeled_block_array;
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
    Arena *symbol_table;
    bool func_not_address;
} IR_Context;

/* The IR_Value used for instructions which do not have a return value / destination register */
extern const IR_Value ir_no_value;

/* Initializes the IR Context and allocates memory for the dynamic loop stack. */
IR_Context ir_init_ctx(Parser *p);

void free_ir_ctx(IR_Context *ctx);

/* Root call to generate IR for a given source file/translation unit. Expects the root AST node, N_TRANSLATION_UNIT. */
IR_Module *ir_gen_translation_unit(IR_Context *ctx,const Node *tu);

/* Append a LoopContext to the top of the loopstack. */
void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block*break_block);
/* Pop off the top of the loopstack. */
void ir_pop_loop_ctx(IR_Context *ctx);

/* Returns an IR Const Value using the constant at `const_index`. */
IR_Value ir_literal_value(int const_index);

void ir_append_instruction(IR_Block *b, IR_Instruction *instr);
void ir_free_module(IR_Module *module);

/* Begins a new scope to track locals defined. */
void ir_begin_scope(IR_Function *func);
/* Ends the scope, popping all locals defined within it. */
void ir_end_scope(IR_Function *func);

/* Returns an IR VReg Value pointing to the next virtual register */
IR_Value ir_next_virtual_reg(IR_Function *func);

IR_Value ir_integer_literal(int64_t i);

/* Initializes IR Module, and functions, func_defs, globals, and consts arrays. */
IR_Module *ir_new_module();

/* Creates a new IR Function with initialized blocks, locals, scopes and stack slots arrays. */
IR_Function *ir_new_function(IR_Context *ctx, const char *name, Type *type);
/*
    Appends a function definition to the global array.
    At this point the FuncDef is a placeholder. Its actual index is -1/ undefined.
    If it is defined, immediately after, its index will be updated.
    Otherwise it gets updated when a defined N_Function node is lowered.
*/
IR_Func_Def *ir_append_func_def(const IR_Context *ctx, const char *name, const bool is_defined, const bool is_variadic, const StorageClass storage_class);
/* Generates an IR Mem Value for the variable. Also appends it to the current scope.  */
IR_Value ir_new_var(IR_Function *func, const char *name, Type *type);

/* Creates a new empty IR Block and initializes its instructions array. */
IR_Block *ir_new_block();
/* Handles creating a new block and appending it to the current function's blocks array. */
IR_Block *ir_add_block(IR_Context *ctx);

IR_Value ir_symbol_value(Symbol *s);
/*
    Appends the IR Function to the modules function array.
    Also handles updating the corresponding IR FuncDef with the correct index into the function array,
    FuncDef is also defined at this point.
*/
void ir_append_function(const IR_Context *ctx, IR_Function *func);
/* Appends the given global variable to the module's global dynamic variable array. */
void ir_append_global(IR_Module *module, Symbol *symbol, const IR_Literal *literal);
/* Appends the given Literal to the module's dynamic const array. */
int ir_append_literal(IR_Module *module, const IR_Literal *literal);
/* Appends the given IR Block to the context's current function. */
IR_Block *ir_append_block(IR_Context *ctx, IR_Block *block);
/* Appends, uniquely, a new labeled block. */
IR_LabeledBlock *ir_append_labeled_block(IR_Context *ctx, const char*label);

IR_Func_Def *ir_get_func_def(const IR_Context *ctx, const char *name);
/*
    Retrieves the IR Value corresponding to the given variable name.
    First checks the scope stack, top down. Then checks module globals array.
    Immediately returning the Value if found, otherwise it is considered an undefined variable.
*/
IR_Value ir_get_symbol_value(IR_Context *ctx, const char *name, bool give_lvalue);

IR_Value ir_value_from_global(IR_Global *g);
IR_Value ir_value_from_func_def(IR_Func_Def *f);
/*
    Retrieves the Block which has the corresponding label.
*/
IR_LabeledBlock *ir_get_labeled_block(IR_Context *ctx, const char *label);

/* Helper functions for the generic Array type. */
static inline IR_Instruction *get_instruction(const Array *arr, int index){
    return (IR_Instruction *) get(arr, index);
}

/* Retrieve the current LoopContext, which lies at the top of the loopstack. */
static inline IR_LoopContext * get_loop_ctx(const IR_Context *ctx){
    return (IR_LoopContext*) get(&ctx->loop_stack_array, ctx->loop_stack_array.count-1);
}

/* Retrieve the scope which lies at the top of the scope stack. */
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
static inline Symbol* get_local(const IR_Function *func, int index){
    return *(Symbol**) get(&func->locals_array, index);
}

static inline int get_var_index(const IR_Scope *scope, int index){
    return *(int*) get(&scope->var_array, index);
}

static inline IR_Func_Def * get_func_def(const IR_Context *ctx, int index){
    return (IR_Func_Def*) get(&ctx->module->func_defs_array, index);
}

static inline IR_Function * get_func(const IR_Module *module, int index){
    return *(IR_Function**) get(&module->functions_array, index);
}

static inline IR_LabeledBlock *get_labeled_block(const IR_Module *module, int index){
    return (IR_LabeledBlock*) get(&module->labeled_block_array, index);
}

static inline IR_CallArg *get_call_arg(const IR_Instruction *call, int index){
    return (IR_CallArg*) get(&call->call.arg_array, index);
}
#endif // COMPILER_C_IR_MODULE_H
