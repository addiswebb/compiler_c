#ifndef COMPILER_C_ANALYSIS_TYPES_H
#define COMPILER_C_ANALYSIS_TYPES_H

#include <stdbool.h>
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


RegSize reg_size(int size);
#endif // COMPILER_C_ANALYSIS_TYPES_H
