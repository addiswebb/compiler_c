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

#ifndef WIN64
#define WIN64
#endif

#ifdef WIN64
#define SHADOW_SPACE 32
#define PARAM_REGISTERS 4
#else
#define SHADOW_SPACE 0
#define PARAM_REGISTERS 6
#endif

#define WIN64_PARAM_REGISTERS 4

extern const GP_Reg win64_int_param_regs[WIN64_PARAM_REGISTERS];
extern const GP_Reg win64_caller_saved[7];
extern const GP_Reg win64_callee_saved[8];
extern const XMM_Reg win64_float_param_regs[8];
extern const char * gp_register_str[16][4];
extern const char * xmm_register_str[16];


RegSize reg_size(int size);

#endif
