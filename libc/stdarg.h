#ifndef _STDARG_H
#define _STDARG_H

#ifdef __linux__
typedef struct {
    unsigned int gp_offset; // 0
    unsigned int fp_offset; // 4
    void *overflow_args;    // 8
    void *reg_save_area;    // 16
} va_list[1];
#endif

#ifdef _WIN64
typedef char *va_list;
#endif

#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_copy(d, s) __builtin_va_copy(d, s)

#endif
