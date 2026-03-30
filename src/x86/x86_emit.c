#include "compiler_c/abi/abi.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/x86/x86.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

const char *x86_reg(const IR_Value *v) {
    if (v->phys_reg.kind == REG_GP) return gp_register_str[v->phys_reg.gp_reg][v->phys_reg.size];
    else return sse_register_str[v->phys_reg.sse_reg];
}

void x86_operand(const IR_Value *v, char *buf, const int n) {
    int len = 0;
    switch (v->kind) {
    case IR_CONSTANT:
        // WARN("Not sure if ir const value in x86 is okay\n");
        snprintf(buf, n, ".LC%d(%%rip)", v->const_index);
        return;
    case IR_PHYS_REG:
        const PhysReg *r = &v->phys_reg;
        switch (r->data_kind) {
        case REG_DATA_LABEL:
            len += snprintf(buf, n, "%s", r->label);
            break;
        case REG_DATA_OFFSET:
            len += snprintf(buf, n, "%d", r->offset);
            break;
        case REG_DATA_CONST_INDEX:
            len += snprintf(buf, n, ".LC%d", r->const_index);
            break;
        case REG_DATA_NONE:
            break;
        }
        if (r->data_kind != REG_DATA_NONE) len += snprintf(buf + len, n - len, "(");
        switch (r->kind) {
        case REG_GP:
            len += snprintf(buf + len, n - len, "%s", gp_register_str[r->gp_reg][r->size]);
            break;
        case REG_XMM:
            len += snprintf(buf + len, n - len, "%s", sse_register_str[r->sse_reg]);
            break;
        case REG_IP:
            len += snprintf(buf + len, n - len, "%%rip");
            break;
        }
        if (r->data_kind != REG_DATA_NONE) snprintf(buf + len, n - len, ")");
        return;
    case IR_INT_LITERAL:
        snprintf(buf, n, "$%" PRId64, v->int_literal);
        return;
    case IR_UNDEFINED:
    case IR_VREG:
    case IR_SYMBOL:
        PANIC("Undefined operand\n");
    }
    // TODO print IR_PHYS_REG properly
    // switch (v->kind) {
    // case IR_STACK:
    //     snprintf(buf, n, "%d(%%rbp)", v->stack_offset);
    //     return;
    // case IR_LITERAL:
    //     snprintf(buf, n, ".LC%d(%%rip)", v->const_index);
    //     return;
    // case IR_GLOBAL:
    //     snprintf(buf, n, "%s(%%rip)", v->global->name);
    //     return;
    // case IR_PHYS_REG:
    //     snprintf(buf, n, "%s", x86_reg(v));
    //     return;
    // case IR_FUNCTION:
    //     snprintf(buf, n, "%s(%%rip)", v->func.name);
    //     return;
    // case IR_VREG:
    // case IR_MEM:
    // case IR_UNDEFINED:
    //     PANIC("Tried to gen assembly for undefined IR_Value\n");
    // }
}

void x86_emit_rx(FILE *fp, const char *instr, const char *s1, const char *s2, const char *src, const IR_Value *dst) {
    char dst_buf[MAX_OPERAND_BUFFER_SIZE];
    x86_operand(dst, dst_buf, sizeof(dst_buf));
    fprintf(fp, "    %s%s%s %s, %s\n", instr, s1, s2, src, dst_buf);
}

void x86_emit_xx(FILE *fp, const char *instr, const char *s1, const char *s2, const IR_Value *src, const IR_Value *dst) {
    char src_buf[MAX_OPERAND_BUFFER_SIZE];
    char dst_buf[MAX_OPERAND_BUFFER_SIZE];
    x86_operand(src, src_buf, sizeof(src_buf));
    x86_operand(dst, dst_buf, sizeof(dst_buf));
    fprintf(fp, "    %s%s%s %s, %s\n", instr, s1, s2, src_buf, dst_buf);
}
void x86_emit_xr(FILE *fp, const char *instr, const char *s1, const char *s2, const IR_Value *src, const char *dst) {
    char src_buf[MAX_OPERAND_BUFFER_SIZE];
    x86_operand(src, src_buf, sizeof(src_buf));
    fprintf(fp, "    %s%s%s %s, %s\n", instr, s1, s2, src_buf, dst);
}

void x86_emit_rr(FILE *fp, const char *instr, const char *s1, const char *s2, const char *src, const char *dst) {
    fprintf(fp, "    %s%s%s %s, %s\n", instr, s1, s2, src, dst);
}

void x86_emit_x(FILE *fp, const char *instr, const char *s1, const char *s2, const IR_Value *operand) {
    char operand_buf[MAX_OPERAND_BUFFER_SIZE];
    x86_operand(operand, operand_buf, sizeof(operand_buf));
    fprintf(fp, "    %s%s%s %s\n", instr, s1, s2, operand_buf);
}

void x86_emit_r(FILE *fp, const char *instr, const char *s1, const char *s2, const char *r) {
    fprintf(fp, "    %s%s%s %s\n", instr, s1, s2, r);
}

const char *x86_rax_reg(Type *t) {
    if (t->kind == T_FLOAT) return "%xmm0";
    if (t->kind == T_INT) {
        switch (t->size) {
        case 1:
            return "%al";
        case 2:
            return "%ax";
        case 4:
            return "%eax";
        case 8:
            return "%rax";
        default:
            PANIC("Tried to get int register of unsupported size %d\n", t->size);
        }
    }
    if (t->kind == T_POINTER || t->kind == T_ARRAY || t->kind == T_FUNCTION) return "%rax";
    log_start(LOG_ERROR);
    printf("Tried to get %%rax register of unsupported type ");
    print_type(t);
    printf("\n");
    exit(1);
}

const char *x86_rbx_reg(const Type *t) {
    if (t->kind == T_FLOAT) return "%xmm1";
    if (t->kind == T_INT) {
        switch (t->size) {
        case 1:
            return "%bl";
        case 2:
            return "%bx";
        case 4:
            return "%ebx";
        case 8:
            return "%rbx";
        default:
            PANIC("Tried to get int register of unsupported size\n");
        }
    }
    if (t->kind == T_POINTER) return "%rbx";
    if (t->kind == T_ARRAY) return "%rbx";
    PANIC("Tried to get %%rbx register of unsupported type\n");
}

const char *x86_rcx_reg(const Type *t) {
    if (t->kind == T_FLOAT) return "%xmm2";
    if (t->kind == T_INT) {
        switch (t->size) {
        case 1:
            return "%cl";
        case 2:
            return "%cx";
        case 4:
            return "%ecx";
        case 8:
            return "%rcx";
        default:
            PANIC("Tried to get int register of unsupported size\n");
        }
    }
    if (t->kind == T_POINTER) return "%rcx";
    if (t->kind == T_ARRAY) return "%rcx";
    PANIC("Tried to get %%rcx register of unsupported type\n");
}

const char *x86_rdx_reg(const Type *t) {
    if (t->kind == T_FLOAT) return "%xmm3";
    if (t->kind == T_INT) {
        switch (t->size) {
        case 1:
            return "%dl";
        case 2:
            return "%dx";
        case 4:
            return "%edx";
        case 8:
            return "%rdx";
        default:
            PANIC("Tried to get int register of unsupported size\n");
        }
    }

    if (t->kind == T_POINTER) return "%rdx";
    if (t->kind == T_ARRAY) return "%rdx";
    PANIC("Tried to get %%rdx register of unsupported type\n");
}

const char *x86_float_op_suffix(int size) {
    switch (size) {
    case 4:
        return "ss";
    case 8:
        return "sd";
    default:
        PANIC("Tried to get float suffix of unsupported size\n");
    }
}

const char *x86_integer_op_suffix(int size) {
    switch (size) {
    case 1:
        return "b";
    case 2:
        return "w";
    case 4:
        return "l";
    case 8:
        return "q";
    default:
        PANIC("Tried to get int suffix of unsupported size\n");
    }
}
const char *x86_op_suffix(const Type *t) {
    if (t->kind == T_FLOAT) return x86_float_op_suffix(t->size);
    if (t->kind == T_INT) return x86_integer_op_suffix(t->size);
    if (t->kind == T_POINTER || t->kind == T_ARRAY || t->kind == T_FUNCTION) return "q";
    PANIC("Tried to op of unsupported type\n");
}
void x86_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) { abi_emit_call(fp, ctx, instr); }

void x86_emit_binary(FILE *fp, const IR_Value *dst, const IR_Value *lhs, const IR_Value *rhs, const IR_BINOP_OP op, Type *t) {
    const char *rax_reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);
    const bool use_i = !(t->kind == T_INT && !t->is_signed);
    const char *sign_prefix = use_i ? "i" : "";
    switch (t->kind) {
    case T_INT:
        switch (op) {
        case ADD:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            x86_emit_xr(fp, "add", op_suffix, "", rhs, rax_reg);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case SUB:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            x86_emit_xr(fp, "sub", op_suffix, "", rhs, rax_reg);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case MUL:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            if (use_i) {
                if (rhs->kind == IR_INT_LITERAL) x86_emit_xr(fp, "imul", op_suffix, "", rhs, rax_reg);
                else x86_emit_x(fp, "imul", op_suffix, "", rhs);
            } else {
                x86_emit_x(fp, sign_prefix, "mul", op_suffix, rhs);
            }
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case DIV:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            switch (t->size) {
            case 1:
                fprintf(fp, "    cbw\n");
                break;
            case 2:
                fprintf(fp, "    cwde\n");
                break;
            case 4:
                fprintf(fp, "    cltd\n");
                break;
            case 8:
                fprintf(fp, "    cqo\n");
                break;
            default:
                PANIC("Tried to divide int with unsupported size\n");
            }
            if (rhs->kind == IR_INT_LITERAL) {
                const char *rcx_reg = x86_rcx_reg(t);
                x86_emit_xr(fp, "mov", op_suffix, "", rhs, rcx_reg);
                x86_emit_r(fp, sign_prefix, "div", op_suffix, rcx_reg);
            } else x86_emit_x(fp, sign_prefix, "div", op_suffix, rhs);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case MOD:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            char *mod_suffix;
            char *reg;
            switch (t->size) {
            case 1:
                fprintf(fp, "    cbw\n");
                mod_suffix = "b";
                reg = "%ah";
                break;
            case 2:
                fprintf(fp, "    cwde\n");
                mod_suffix = "w";
                reg = "%dx";
                break;
            case 4:
                fprintf(fp, "    cltd\n");
                mod_suffix = "l";
                reg = "%edx";
                break;
            case 8:
                fprintf(fp, "    cqo\n");
                mod_suffix = "q";
                reg = "%rdx";
                break;
            default:
                PANIC("Tried to modulo int with unsupported size\n");
            }

            if (rhs->kind == IR_INT_LITERAL) {
                const char *rcx_reg = x86_rcx_reg(t);
                x86_emit_xr(fp, "mov", op_suffix, "", rhs, rcx_reg);
                x86_emit_r(fp, sign_prefix, "div", mod_suffix, rcx_reg);
            } else x86_emit_x(fp, sign_prefix, "div", mod_suffix, rhs);

            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case BW_AND:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            x86_emit_xr(fp, "and", op_suffix, "", rhs, rax_reg);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case BW_OR:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            x86_emit_xr(fp, "or", op_suffix, "", rhs, rax_reg);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case XOR:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            x86_emit_xr(fp, "xor", op_suffix, "", rhs, rax_reg);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case SHL:
            const char *rcx_reg_l = x86_rcx_reg(t);
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            x86_emit_xr(fp, "mov", op_suffix, "", rhs, rcx_reg_l);
            x86_emit_rr(fp, "shl", op_suffix, "", "%cl", rax_reg);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case SHR:
            const char *rcx_reg_r = x86_rcx_reg(t);
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, rax_reg);
            x86_emit_xr(fp, "mov", op_suffix, "", rhs, rcx_reg_r);
            x86_emit_rr(fp, "sar", op_suffix, "", "%cl", rax_reg);
            x86_emit_rx(fp, "mov", op_suffix, "", rax_reg, dst);
            return;
        case L_AND:
        case L_OR:
            PANIC("Logical operators && and || should not reach x86 gen\n");
        }
    case T_FLOAT:
        x86_emit_xr(fp, "mov", op_suffix, "", lhs, "%xmm0");
        switch (op) {
        case ADD:
            x86_emit_xr(fp, "add", op_suffix, "", rhs, "%xmm0");
            break;
        case SUB:
            x86_emit_xr(fp, "sub", op_suffix, "", rhs, "%xmm0");
            break;
        case MUL:
            x86_emit_xr(fp, "mul", op_suffix, "", rhs, "%xmm0");
            break;
        case DIV:
            x86_emit_xr(fp, "div", op_suffix, "", rhs, "%xmm0");
            break;
        default:
            PANIC("Tried to perform unsupported binary operation on type float\n");
        }
        x86_emit_rx(fp, "mov", op_suffix, "", "%xmm0", dst);
        break;
    case T_POINTER:
        switch (op) {
        case ADD:
            x86_emit_xr(fp, "mov", "q", "", lhs, "%rax");
            x86_emit_xr(fp, "add", "q", "", rhs, "%rax");
            x86_emit_rx(fp, "mov", "q", "", "%rax", dst);
            return;
        default:
            PANIC("Tried to perform unsupported binary op on pointer\n");
        }
    default:
        PANIC("Tried to emit binary instruction of unsupported type\n");
    }
}
void x86_emit_unary(FILE *fp, const IR_Value *dst, const IR_Value *expr, const IR_UNARY_OP op, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);
    switch (t->kind) {
    case T_INT:
        switch (op) {
        case POS:
            x86_emit_xr(fp, "mov", op_suffix, "", expr, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case NEG:
            x86_emit_xr(fp, "mov", op_suffix, "", expr, reg);
            fprintf(fp, "    neg%s %s\n", op_suffix, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case LNOT:
            x86_emit_xr(fp, "mov", op_suffix, "", expr, reg);
            fprintf(fp, "    test%s %s, %s\n", op_suffix, reg, reg);
            fprintf(fp, "    sete %%al\n");
            fprintf(fp, "    movzb%s %%al, %s\n", op_suffix, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case BNOT:
            x86_emit_xr(fp, "mov", op_suffix, "", expr, reg);
            fprintf(fp, "    not%s %s\n", op_suffix, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        default:
            return;
        }
    case T_FLOAT:
        switch (op) {
        case POS:
            x86_emit_xr(fp, "mov", op_suffix, "", expr, "%xmm0");
            x86_emit_rx(fp, "mov", op_suffix, "", "%xmm0", dst);
            return;
        case NEG:
            x86_emit_xr(fp, "mov", op_suffix, "", expr, "%xmm0");
            if (t->size == 4) fprintf(fp, "    xorps %%xmm1, %%xmm1\n");
            else fprintf(fp, "    xorpd %%xmm1, %%xmm1\n");
            x86_emit_rr(fp, "sub", op_suffix, "", "%xmm0", "%xmm1");
            x86_emit_rx(fp, "mov", op_suffix, "", "%xmm1", dst);
            return;
        case LNOT: // float->int
            x86_emit_xr(fp, "mov", op_suffix, "", expr, "%xmm0");
            if (t->size == 4) fprintf(fp, "    xorps %%xmm1, %%xmm1\n");
            else fprintf(fp, "    xorpd %%xmm1, %%xmm1\n");
            fprintf(fp, "    ucomi%s %%xmm1, %%xmm0\n", op_suffix);
            fprintf(fp, "    sete %%al\n");
            x86_emit_rr(fp, "movz", "b", "l", "%al", "%al");
            x86_emit_rx(fp, "mov", "l", "", "%eax", dst);
            return;
        case BNOT:
            PANIC("Tried to perform Bitwise Not ~ on Type float\n");
        default:
            return;
        }
    default:
        PANIC("Tried to emit unary of unsupported type\n");
    }
}

void x86_emit_addr(FILE *fp, const IR_Value *src, const IR_Value *dst) {
    x86_emit_xr(fp, "leaq", "", "", src, "%rax");
    x86_emit_rx(fp, "mov", "q", "", "%rax", dst);
}
void x86_emit_cast(FILE *fp, const IR_Value *src, const IR_Value *dst, Type *from, Type *to) {
    const char *from_reg = x86_rax_reg(from);
    const char *from_op_suffix = x86_op_suffix(from);

    const char *to_reg = x86_rax_reg(to);
    const char *to_op_suffix = x86_op_suffix(to);

    if (from == to) {
        x86_emit_xr(fp, "mov", "", from_op_suffix, src, from_reg);
        x86_emit_rx(fp, "mov", "", from_op_suffix, from_reg, dst);
        return;
    }
    if (from->kind == T_ARRAY && to->kind == T_POINTER) return;

    if (from->kind == T_FUNCTION && to->kind == T_POINTER) {
        x86_emit_xr(fp, "lea", "", "", src, from_reg);
        x86_emit_rx(fp, "mov", "q", "", from_reg, dst);
        return;
    }
    // char/short/int/long/ -> char/short/int/long
    // pointer <-> char/short/int/long
    if ((from->kind == T_INT || from->kind == T_POINTER) && (to->kind == T_INT || to->kind == T_POINTER)) {
        // if (from->kind == T_INT && to->kind == T_INT) {
        if (from->size < to->size) {
            x86_emit_xr(fp, "movs", from_op_suffix, to_op_suffix, src, to_reg);
        } else {
            x86_emit_xr(fp, "mov", from_op_suffix, "", src, from_reg);
        }
        x86_emit_rx(fp, "mov", to_op_suffix, "", to_reg, dst);
        return;
    }

    // char/short/int/long -> float/double
    if (from->kind == T_INT && to->kind == T_FLOAT) {
        x86_emit_xr(fp, "movs", from_op_suffix, "q", src, "%rax");
        x86_emit_rr(fp, "cvtsi2", to_op_suffix, "", "%rax", to_reg);
        x86_emit_rx(fp, "mov", to_op_suffix, "", to_reg, dst);
        return;
    }
    // float/double -> char/short/int/long
    if (from->kind == T_FLOAT && to->kind == T_INT) {
        if (to->size == 8) {
            x86_emit_xr(fp, "cvtt", from_op_suffix, "2sq", src, "%rax");
            x86_emit_rx(fp, "mov", "q", "", "%rax", dst);
        } else {
            x86_emit_xr(fp, "cvtt", from_op_suffix, "2si", src, "%eax");
            x86_emit_rx(fp, "mov", to_op_suffix, "", to_reg, dst);
        }
        return;
    }
    // float/double -> float/double
    if (from->kind == T_FLOAT && to->kind == T_FLOAT) {
        x86_emit_xr(fp, "mov", from_op_suffix, "", src, "%xmm0");
        fprintf(fp, "    cvt%s2%s %%xmm0, %%xmm1\n", from_op_suffix, to_op_suffix);
        x86_emit_rx(fp, "mov", to_op_suffix, "", "%xmm1", dst);
        return;
    }

    PANIC("Cast node did literally nothing?\n");
}
void x86_emit_const(FILE *fp, const IR_Value *dst, Type *t, const IR_Literal *c, const int pool_index) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);

    switch (t->kind) {
    case T_INT:
        switch (t->size) {
        case 1:
        case 2:
        case 4:
            fprintf(fp, "    movl $%d, %%eax\n", (int)c->i);
            break;
        case 8:
            fprintf(fp, "    movl $%" PRId64 ", %%eax\n", c->i);
            break;
        default:
            PANIC("Tried to emit const int of unsupported size\n");
        }
        break;
    case T_FLOAT:
        switch (t->size) {
        case 4:
        case 8:
            fprintf(fp, "    mov%s .LC%d(%%rip), %s\n", op_suffix, pool_index, reg);
            break;
        default:
            PANIC("Tried to emit const float of unsupported size\n");
        }
        break;
    case T_ARRAY:
        if (t->base == type_i8) return;
    case T_POINTER:
    default:
        printf("Tried to emit const of unsupported type\n");
    }
    x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
}
void x86_emit_store(FILE *fp, const IR_Value *src, const IR_Value *dst, Type *t) {
    // const char *reg = x86_rax_reg(t);
    // const char *op_suffix = x86_op_suffix(t);
    // x86_emit_xr(fp, "mov", op_suffix, "", src, reg);
    // x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);

    const char *v = x86_rcx_reg(t);
    const char *op_suffix = x86_op_suffix(t);
    x86_emit_xr(fp, "mov", "q", "", dst, "%rax");
    x86_emit_xr(fp, "mov", op_suffix, "", src, v);
    x86_emit_rr(fp, "mov", op_suffix, "", v, "(%rax)");
}
void x86_emit_load(FILE *fp, const IR_Value *addr, const IR_Value *dst, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);

    x86_emit_xr(fp, "mov", "q", "", addr, "%rax");
    x86_emit_rr(fp, "mov", op_suffix, "", "(%rax)", reg);
    x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
}

void x86_emit_string(FILE *fp, const char *str) {
    fprintf(fp, "    .byte ");
    while (*str) {
        unsigned char c = *str;
        if (c >= 0x20 && c <= 0x7E && c != '\'' && c != '\\' && c != '"') {
            fprintf(fp, "'%c', ", c);
        } else {
            fprintf(fp, "0x%02X, ", c);
        }
        str++;
    }
    fprintf(fp, "0\n");
}
