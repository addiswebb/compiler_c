#ifndef COMPILER_C_ANALYSIS_TYPES_H
#define COMPILER_C_ANALYSIS_TYPES_H

typedef enum{
    RAX, RBX, RCX, RDX,
    RSI, RDI,
    RBP, RSP,
    R8, R9, R10, R11, R12, R13, R14, R15,
}GP_Reg;

typedef enum{
    XMM0, XMM1, XMM2, XMM3,
    XMM4, XMM5, XMM6, XMM7,
    XMM8, XMM9, XMM10, XMM11,
    XMM12, XMM13, XMM14, XMM15,
}XMM_Reg;

typedef enum{
    REG_8,
    REG_16,
    REG_32,
    REG_64,
}RegSize;

typedef enum{
    REG_GP,
    REG_XMM,
}RegKind;

struct PhysReg{
    RegKind kind;
    union{
        GP_Reg gp_reg;
        XMM_Reg xmm_reg;
    };
    RegSize size;
};

#ifdef _WIN64

#define STRUCT_IN_REG(s) ((s)==1 || (s)==2 || (s)==4 || (s)==8)
#define SHADOW_SPACE 32
#define PARAM_REGISTERS 4
#define CALLER_SAVED_REGISTERS 7
#define CALLEE_SAVED_REGISTERS 8

#else

#define STRUCT_IN_REG(s) (s <= 16)
#define SHADOW_SPACE 0
#define PARAM_REGISTERS 6
#define INTEGER_PARAM_REGISTERS 6
#define FLOAT_PARAM_REGISTERS 8
#define CALLER_SAVED_REGISTERS 9
#define CALLEE_SAVED_REGISTERS 6

#endif

extern const GP_Reg caller_saved_regs[CALLER_SAVED_REGISTERS];
extern const GP_Reg callee_saved_regs[CALLEE_SAVED_REGISTERS];
extern const GP_Reg int_param_regs[PARAM_REGISTERS];
extern const XMM_Reg float_param_regs[PARAM_REGISTERS];
extern const char * gp_register_str[16][4];
extern const char * xmm_register_str[16];

RegSize reg_size(int size);
#endif // COMPILER_C_ANALYSIS_TYPES_H
