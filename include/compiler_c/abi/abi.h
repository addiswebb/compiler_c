#include "compiler_c/analyse/analysis_types.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include <stdio.h>

typedef enum{
    ABI_NO_CLASS,
    ABI_MEMORY,
    ABI_INTEGER,
    ABI_SSE,
}ABI_TypeClass;

typedef struct{
    ABI_TypeClass class[2];
    bool memory;
}ABI_Result;

ABI_Result abi_classify(Type *type);

void abi_lower_store(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i);
void abi_lower_call(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i);
void abi_lower_ret(IR_Function *f, IR_Block *b, IR_Instruction *instr, int *i);
void abi_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr);
Type *abi_func_type(Type *type);

void abi_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr);

#ifdef _WIN64

#define MAX_STRUCT_SIZE 8
#define SHADOW_SPACE 32
#define PARAM_REGISTERS 4
#define CALLER_SAVED_REGISTERS 7
#define CALLEE_SAVED_REGISTERS 8
#define HIDDEN_PTR_SIZE 8

extern const GP_Reg int_param_regs[PARAM_REGISTERS];
extern const XMM_Reg float_param_regs[PARAM_REGISTERS];
#define strdup  _strdup
#define popen   _popen
#define pclose  _pclose
#else

#define MAX_STRUCT_SIZE 16
#define SHADOW_SPACE 0
#define PARAM_REGISTERS 6
#define INTEGER_PARAM_REGISTERS 6
#define FLOAT_PARAM_REGISTERS 8
#define CALLER_SAVED_REGISTERS 9
#define CALLEE_SAVED_REGISTERS 6
#define HIDDEN_PTR_SIZE 16

ABI_TypeClass merge(ABI_TypeClass chunk_class, ABI_TypeClass field_class);
ABI_Result classify_struct(Type *type);

extern const GP_Reg int_param_regs[INTEGER_PARAM_REGISTERS];
extern const XMM_Reg float_param_regs[FLOAT_PARAM_REGISTERS];
#endif


extern const GP_Reg caller_saved_regs[CALLER_SAVED_REGISTERS];
extern const GP_Reg callee_saved_regs[CALLEE_SAVED_REGISTERS];
extern const char * gp_register_str[16][4];
extern const char * sse_register_str[16];
