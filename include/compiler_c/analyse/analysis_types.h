#ifndef COMPILER_C_ANALYSIS_TYPES_H
#define COMPILER_C_ANALYSIS_TYPES_H

#include "../libc/stdbool.h"

/* x86-64 General Purpose Registers */
typedef enum{
    RAX, RBX, RCX, RDX,
    RSI, RDI,
    RBP, RSP,
    R8, R9, R10, R11, R12, R13, R14, R15,
}GP_Reg;

/* x86-64 XMM Registers */
typedef enum{
    XMM0, XMM1, XMM2, XMM3,
    XMM4, XMM5, XMM6, XMM7,
    XMM8, XMM9, XMM10, XMM11,
    XMM12, XMM13, XMM14, XMM15,
}XMM_Reg;

/* Register size variant */
typedef enum{
    REG_8,
    REG_16,
    REG_32,
    REG_64,
}RegSize;

typedef enum{
    REG_GP,
    REG_XMM,
    REG_IP
}RegKind;

typedef enum {
    // foo(%rip)
    REG_DATA_LABEL,
    // -8(%rbp)
    REG_DATA_OFFSET,
    // .LC0(%rip)
    REG_DATA_CONST_INDEX,
    // %rax
    REG_DATA_NONE,
}RegDataKind;

struct PhysReg{
    RegKind kind;
    union{
        GP_Reg gp_reg;
        XMM_Reg sse_reg;
    };
    RegSize size;
    RegDataKind data_kind;
    union{
        const char* label;
        struct{
            int offset;
            int scale;
        };
        int const_index;
    };
};

/* Convert integer size to RegSize. */
RegSize reg_size(int size);

#endif // COMPILER_C_ANALYSIS_TYPES_H
