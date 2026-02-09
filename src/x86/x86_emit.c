#include "compiler_c/ir/ir_module.h"
#include "compiler_c/type.h"
#include "compiler_c/x86.h"

#include <stdio.h>
#include <stdlib.h>

void x86_operand(const IR_Value *v, char *buf, int n) {
    switch (v->kind) {
    case IR_STACK:
        snprintf(buf, n, "%d(%%rbp)", v->stack_offset + v->offset);
        return;
    case IR_LITERAL:
        snprintf(buf, n, ".LC%d(%%rip)", v->const_index);
        return;
    case IR_GLOBAL:
        snprintf(buf, n, "%s(%%rip)", v->global->name);
        return;
    case IR_REG:
    case IR_MEM:
    case IR_UNDEFINED:
        printf("Tried to gen assembly for undefined IR_Value\n");
        exit(1);
    }
}

void x86_emit_rx(FILE *fp, const char *instr, const char *s1, const char *s2, const char *src, const IR_Value *dst) {
    char dst_buf[MAX_OPERAND_BUFFER_SIZE];
    x86_operand(dst, dst_buf, sizeof(dst_buf));
    fprintf(fp, "    %s%s%s %s, %s\n", instr, s1, s2, src, dst_buf);
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
            printf("Tried to get int register of unsupported size %d\n", t->size);
            exit(1);
        }
    }
    if (t->kind == T_POINTER) return "%rax";
    if (t->kind == T_ARRAY) return "%rax";
    printf("Tried to get %%rax register of unsupported type ");
    print_type(t);
    printf("\n");
    exit(1);
}

const char *x86_rbx_reg(Type *t) {
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
            printf("Tried to get int register of unsupported size\n");
            exit(1);
        }
    }
    if (t->kind == T_POINTER) return "%rbx";
    if (t->kind == T_ARRAY) return "%rbx";
    printf("Tried to get %%rbx register of unsupported type\n");
    exit(1);
}

const char *x86_rcx_reg(Type *t) {
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
            printf("Tried to get int register of unsupported size\n");
            exit(1);
        }
    }
    if (t->kind == T_POINTER) return "%rcx";
    if (t->kind == T_ARRAY) return "%rcx";
    printf("Tried to get %%rcx register of unsupported type\n");
    exit(1);
}

const char *x86_rdx_reg(Type *t) {
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
            printf("Tried to get int register of unsupported size\n");
            exit(1);
        }
    }

    if (t->kind == T_POINTER) return "%rdx";
    if (t->kind == T_ARRAY) return "%rdx";
    printf("Tried to get %%rdx register of unsupported type\n");
    exit(1);
}

const char *x86_op_suffix(Type *t) {
    if (t->kind == T_FLOAT) {
        switch (t->size) {
        case 4:
            return "ss";
        case 8:
            return "sd";
        default:
            printf("Tried to get float suffix of unsupported size\n");
            exit(1);
        }
    }
    if (t->kind == T_INT) {
        switch (t->size) {
        case 1:
            return "b";
        case 2:
            return "w";
        case 4:
            return "l";
        case 8:
            return "q";
        default:
            printf("Tried to get int suffix of unsupported size\n");
            exit(1);
        }
    }
    if (t->kind == T_POINTER) return "q";
    if (t->kind == T_ARRAY) return "q";
    printf("Tried to op of unsupported type\n");
    exit(1);
}

void x86_emit_call(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    int dst_offset = instr->ops[0].stack_offset;
    Type *t = instr->call.type;

    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);

    for (int i = 0; i < instr->call.arg_count; i++) {
        IR_Var *v = &instr->call.args[i];
        switch (v->type->kind) {
        case T_INT:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", x86_op_suffix(v->type), v->reg.stack_offset, x86_rax_reg(v->type));
            fprintf(fp, "    push %%rax\n");
            break;
        case T_FLOAT:
            const char *f_suffix = x86_op_suffix(v->type);
            fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", f_suffix, v->reg.stack_offset);
            fprintf(fp, "    sub $8, %%rsp\n");
            fprintf(fp, "    mov%s %%xmm0, (%%rsp)\n", f_suffix);
            break;
        case T_POINTER:
            fprintf(fp, "    movq %d(%%rbp), %%rax\n", v->reg.stack_offset);
            fprintf(fp, "    push %%rax\n");
            break;
        default:
            printf("Tried to emit call arg for unsupported type\n");
            exit(1);
        }
    }

    fprintf(fp, "    call %s\n", ctx->module->defs[instr->call.callee].name);
    fprintf(fp, "    add $%d, %%rsp\n", instr->call.arg_count * 8);
    fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
}

void x86_emit_binary(FILE *fp, const IR_Value *dst, const IR_Value *lhs, const IR_Value *rhs, IR_BINOP_OP op, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);
    switch (t->kind) {
    case T_INT:
        switch (op) {
        case ADD:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_xr(fp, "add", op_suffix, "", rhs, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case SUB:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_xr(fp, "sub", op_suffix, "", rhs, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case MUL:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_x(fp, "imul", op_suffix, "", rhs);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case DIV:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
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
                printf("Tried to divide int with unsupported size\n");
                exit(1);
            }
            x86_emit_x(fp, "idiv", op_suffix, "", rhs);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case MOD:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            switch (t->size) {
            case 1:
                fprintf(fp, "    cbw\n");
                x86_emit_x(fp, "idiv", "b", "", rhs);
                x86_emit_rx(fp, "mov", "b", "", "%ah", dst);
                return;
            case 2:
                fprintf(fp, "    cwde\n");
                x86_emit_x(fp, "idiv", "w", "", rhs);
                x86_emit_rx(fp, "mov", "w", "", "%dx", dst);
                return;
            case 4:
                fprintf(fp, "    cltd\n");
                x86_emit_x(fp, "idiv", "l", "", rhs);
                x86_emit_rx(fp, "mov", "l", "", "%edx", dst);
                return;
            case 8:
                fprintf(fp, "    cqo\n");
                x86_emit_x(fp, "idiv", "q", "", rhs);
                x86_emit_rx(fp, "mov", "q", "", "%rdx", dst);
                return;
            default:
                printf("Tried to modulo int with unsupported size\n");
                exit(1);
            }
        case BW_AND:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_xr(fp, "and", op_suffix, "", rhs, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case BW_OR:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_xr(fp, "or", op_suffix, "", rhs, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case XOR:
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_xr(fp, "xor", op_suffix, "", rhs, reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case SHL:
            const char *rcx_reg_l = x86_rcx_reg(t);
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_xr(fp, "mov", op_suffix, "", rhs, rcx_reg_l);
            x86_emit_rr(fp, "shl", op_suffix, "", "%cl", reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case SHR:
            const char *rcx_reg_r = x86_rcx_reg(t);
            x86_emit_xr(fp, "mov", op_suffix, "", lhs, reg);
            x86_emit_xr(fp, "mov", op_suffix, "", rhs, rcx_reg_r);
            x86_emit_rr(fp, "sar", op_suffix, "", "%cl", reg);
            x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
            return;
        case L_AND:
        case L_OR:
            printf("Logical operators && and || should not be given to x86 gen\n");
            exit(1);
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
            printf("Tried to perform unsuported binary operation on type float\n");
            exit(1);
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
        case SUB:
            x86_emit_xr(fp, "mov", "q", "", lhs, "%rax");
            x86_emit_xr(fp, "sub", "q", "", rhs, "%rax");
            x86_emit_rx(fp, "mov", "q", "", "%rax", dst);
            return;
        default:
            printf("Tried to perform unsupported binary op on pointer\n");
            exit(1);
        }
    default:
        printf("Tried to emit binary of unsupported type\n");
        exit(1);
    }
}
void x86_emit_unary(FILE *fp, const IR_Value *dst, const IR_Value *expr, IR_UNARY_OP op, Type *t) {
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
            printf("Tried to perform Bitwise Not ~ on Type float\n");
            exit(1);
        default:
            return;
        }
    default:
        printf("Tried to emit unary of unsupported type\n");
        exit(1);
    }
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
    if (from->kind == T_ARRAY && to->kind == T_POINTER) {
        x86_emit_xr(fp, "lea", "", "", src, from_reg);
        x86_emit_rx(fp, "mov", "q", "", from_reg, dst);
        return;
    }
    // char/short/int/long/ -> char/short/int/long
    if (from->kind == T_INT && to->kind == T_INT) {
        if (from->size < to->size) {
            x86_emit_xr(fp, "mov", from_op_suffix, to_op_suffix, src, from_reg);
        } else {
            x86_emit_xr(fp, "mov", from_op_suffix, "", src, from_reg);
        }
        x86_emit_rx(fp, "mov", from_op_suffix, "", to_reg, dst);
        return;
    }

    // char/short/int/long -> float/double
    if (from->kind == T_INT && to->kind == T_FLOAT) {
        // fprintf(fp, "    movs%sq %d(%%rbp), %%rax\n", from_op_suffix, src_offset);
        x86_emit_xr(fp, "mov", from_op_suffix, "q", src, "%rax");
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

    printf("Cast node did literally nothing?\n");
    exit(1);
}
void x86_emit_const(FILE *fp, const IR_Value *dst, Type *t, IR_Literal *c, int pool_index) {
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
            fprintf(fp, "    movl $%lld, %%eax\n", c->i);
            break;
        default:
            printf("Tried to emit const int of unsupported size\n");
            exit(1);
        }
        break;
    case T_FLOAT:
        switch (t->size) {
        case 4:
        case 8:
            fprintf(fp, "    mov%s .LC%d(%%rip), %s\n", op_suffix, pool_index, reg);
            break;
        default:
            printf("Tried to emit const float of unsupported size\n");
            exit(1);
        }
        break;
    case T_POINTER:
    case T_ARRAY:
        if (t->base == type_char) {
            fprintf(fp, "    lea .LC%d(%%rip), %%rax\n", pool_index);
            break;
        }
    default:
        printf("Tried to emit const of unsupported type\n");
    }
    x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
}
void x86_emit_store(FILE *fp, const IR_Value *src, const IR_Value *dst, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);
    x86_emit_xr(fp, "mov", op_suffix, "", src, reg);
    x86_emit_rx(fp, "mov", op_suffix, "", reg, dst);
}
void x86_emit_store_mem(FILE *fp, const IR_Value *src, const IR_Value *dst, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *v = x86_rbx_reg(t);
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
