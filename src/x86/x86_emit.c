#include "compiler_c/type.h"
#include "compiler_c/x86.h"

#include <stdio.h>
#include <stdlib.h>

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
            printf("Tried to get int register of unsupported size\n");
            exit(1);
        }
    }
    if (t->kind == T_POINTER) return "%rax";
    if (t->kind == T_ARRAY) return "%rax";
    printf("Tried to get %%rax register of unsupported type\n");
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
        printf("need to handle +positive offsets natively for args\n");
        exit(1);
        IR_Var *v = &instr->call.args[i];
        switch (v->type->kind) {
        case T_INT:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", x86_op_suffix(v->type), reg_offset(v->reg.reg), x86_rax_reg(v->type));
            fprintf(fp, "    push %%rax\n");
            break;
        case T_FLOAT:
            const char *f_suffix = x86_op_suffix(v->type);
            fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", f_suffix, reg_offset(v->reg.reg));
            fprintf(fp, "    sub $8, %%rsp\n");
            fprintf(fp, "    mov%s %%xmm0, (%%rsp)\n", f_suffix);
            break;
        case T_POINTER:
            fprintf(fp, "    movq %d(%%rbp), %%rax\n", reg_offset(v->reg.reg));
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

void x86_emit_binary(FILE *fp, int dst_offset, int lhs_offset, int rhs_offset, IR_BINOP_OP op, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);
    switch (t->kind) {
    case T_INT:
        switch (op) {
        case ADD:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    add%s %d(%%rbp), %s\n", op_suffix, rhs_offset, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case SUB:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    sub%s %d(%%rbp), %s\n", op_suffix, rhs_offset, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case MUL:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    imul%s %d(%%rbp)\n", op_suffix, rhs_offset);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case DIV:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
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
            fprintf(fp, "    idiv%s %d(%%rbp)\n", op_suffix, rhs_offset);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case MOD:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            switch (t->size) {
            case 1:
                fprintf(fp, "    cbw\n");
                fprintf(fp, "    idivb %d(%%rbp)\n", rhs_offset);
                fprintf(fp, "    movb %%ah, %d(%%rbp)\n", dst_offset);
                return;
            case 2:
                fprintf(fp, "    cwde\n");
                fprintf(fp, "    idivw %d(%%rbp)\n", rhs_offset);
                fprintf(fp, "    movw %%dx, %d(%%rbp)\n", dst_offset);
                return;
            case 4:
                fprintf(fp, "    cltd\n");
                fprintf(fp, "    idivl %d(%%rbp)\n", rhs_offset);
                fprintf(fp, "    movl %%edx, %d(%%rbp)\n", dst_offset);
                return;
            case 8:
                fprintf(fp, "    cqo\n");
                fprintf(fp, "    idivq %d(%%rbp)\n", rhs_offset);
                fprintf(fp, "    movq %%rdx, %d(%%rbp)\n", dst_offset);
                return;
            default:
                printf("Tried to modulo int with unsupported size\n");
                exit(1);
            }
        case AND:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    and%s %d(%%rbp), %s\n", op_suffix, rhs_offset, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case OR:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    or%s %d(%%rbp), %s\n", op_suffix, rhs_offset, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case XOR:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    xor%s %d(%%rbp), %s\n", op_suffix, rhs_offset, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case SHL:
            const char *rcx_reg_l = x86_rcx_reg(t);
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, rhs_offset, rcx_reg_l);
            fprintf(fp, "    shl%s %%cl, %s\n", op_suffix, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case SHR:
            const char *rcx_reg_r = x86_rcx_reg(t);
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, lhs_offset, reg);
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, rhs_offset, rcx_reg_r);
            fprintf(fp, "    sar%s %%cl, %s\n", op_suffix, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        }
    case T_FLOAT:
        fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", op_suffix, lhs_offset);
        switch (op) {
        case ADD:
            fprintf(fp, "    add%s %d(%%rbp), %%xmm0\n", op_suffix, rhs_offset);
            break;
        case SUB:
            fprintf(fp, "    sub%s %d(%%rbp), %%xmm0\n", op_suffix, rhs_offset);
            break;
        case MUL:
            fprintf(fp, "    mul%s %d(%%rbp), %%xmm0\n", op_suffix, rhs_offset);
            break;
        case DIV:
            fprintf(fp, "    div%s %d(%%rbp), %%xmm0\n", op_suffix, rhs_offset);
            break;
        default:
            printf("Tried to perform unsuported binary operation on type float\n");
            exit(1);
        }
        fprintf(fp, "    mov%s %%xmm0, %d(%%rbp)\n", op_suffix, dst_offset);
        break;
    case T_POINTER:
        switch (op) {
        case ADD:
            fprintf(fp, "    lea %d(%%rbp), %%rax\n", lhs_offset);
            fprintf(fp, "    addq %d(%%rbp), %%rax\n", rhs_offset);
            fprintf(fp, "    movq %%rax, %d(%%rbp)\n", dst_offset);
            return;
        case SUB:
            fprintf(fp, "    movq %d(%%rbp), %%rax\n", lhs_offset);
            fprintf(fp, "    subq %d(%%rbp), %%rax\n", rhs_offset);
            fprintf(fp, "    movq %%rax, %d(%%rbp)\n", dst_offset);
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
void x86_emit_unary(FILE *fp, int dst_offset, int expr_offset, IR_UNARY_OP op, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);
    switch (t->kind) {
    case T_INT:
        switch (op) {
        case POS:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, expr_offset, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case NEG:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, expr_offset, reg);
            fprintf(fp, "    neg%s %s\n", op_suffix, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case LNOT:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, expr_offset, reg);
            fprintf(fp, "    test%s %s, %s\n", op_suffix, reg, reg);
            fprintf(fp, "    sete %%al\n");
            fprintf(fp, "    movzb%s %%al, %s\n", op_suffix, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        case BNOT:
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, expr_offset, reg);
            fprintf(fp, "    not%s %s\n", op_suffix, reg);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
            return;
        default:
            return;
        }
    case T_FLOAT:
        switch (op) {
        case POS:
            fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", op_suffix, expr_offset);
            fprintf(fp, "    mov%s %%xmm0, %d(%%rbp)\n", op_suffix, dst_offset);
            return;
        case NEG:
            fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", op_suffix, expr_offset);
            if (t->size == 4) fprintf(fp, "    xorps %%xmm1, %%xmm1\n");
            else fprintf(fp, "    xorpd %%xmm1, %%xmm1\n");
            fprintf(fp, "    sub%s %%xmm0, %%xmm1\n", op_suffix);
            fprintf(fp, "    mov%s %%xmm1, %d(%%rbp)\n", op_suffix, dst_offset);
            return;
        case LNOT: // float->int
            fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", op_suffix, expr_offset);
            if (t->size == 4) fprintf(fp, "    xorps %%xmm1, %%xmm1\n");
            else fprintf(fp, "    xorpd %%xmm1, %%xmm1\n");
            fprintf(fp, "    ucomi%s %%xmm1, %%xmm0\n", op_suffix);
            fprintf(fp, "    sete %%al\n");
            fprintf(fp, "    movzbl %%al, %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", dst_offset);
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
void x86_emit_cast(FILE *fp, int src_offset, int dst_offset, Type *from, Type *to) {
    const char *from_reg = x86_rax_reg(from);
    const char *from_op_suffix = x86_op_suffix(from);

    const char *to_reg = x86_rax_reg(to);
    const char *to_op_suffix = x86_op_suffix(to);

    if (from == to) {
        fprintf(fp, "    mov%s %d(%%rbp), %s\n", from_op_suffix, src_offset, from_reg);
        fprintf(fp, "    mov%s %s, %d(%%rbp)\n", from_op_suffix, from_reg, dst_offset);
        return;
    }
    // char/short/int/long/ -> char/short/int/long
    if (from->kind == T_INT && to->kind == T_INT) {
        if (from->size < to->size) {
            fprintf(fp, "    movs%s%s %d(%%rbp), %s\n", from_op_suffix, to_op_suffix, src_offset, to_reg);
        } else {
            fprintf(fp, "    mov%s %d(%%rbp), %s\n", from_op_suffix, src_offset, from_reg);
        }
        fprintf(fp, "    mov%s %s, %d(%%rbp)\n", to_op_suffix, to_reg, dst_offset);
        return;
    }

    // char/short/int/long -> float/double
    if (from->kind == T_INT && to->kind == T_FLOAT) {
        fprintf(fp, "    movs%sq %d(%%rbp), %%rax\n", from_op_suffix, src_offset);
        fprintf(fp, "    cvtsi2%s %%rax, %s\n", to_op_suffix, to_reg);
        fprintf(fp, "    mov%s %s, %d(%%rbp)\n", to_op_suffix, to_reg, dst_offset);
        return;
    }
    // float/double -> char/short/int/long
    if (from->kind == T_FLOAT && to->kind == T_INT) {
        if (to->size == 8) {
            fprintf(fp, "    cvtt%s2sq %d(%%rbp), %%rax\n", from_op_suffix, src_offset);
            fprintf(fp, "    movq %%rax, %d(%%rbp)\n", dst_offset);
        } else {
            fprintf(fp, "    cvtt%s2si %d(%%rbp), %%eax\n", from_op_suffix, src_offset);
            fprintf(fp, "    mov%s %s, %d(%%rbp)\n", to_op_suffix, to_reg, dst_offset);
        }
        return;
    }
    // float/double -> float/double
    if (from->kind == T_FLOAT && to->kind == T_FLOAT) {
        fprintf(fp, "    mov%s %d(%%rbp), %%xmm0\n", from_op_suffix, src_offset);
        fprintf(fp, "    cvt%s2%s %%xmm0, %%xmm1\n", from_op_suffix, to_op_suffix);
        fprintf(fp, "    mov%s %%xmm1, %d(%%rbp)\n", to_op_suffix, dst_offset);
        return;
    }

    printf("Cast node did literally nothing?\n");
    exit(1);
}
void x86_emit_const(FILE *fp, int dst_offset, Type *t, IR_Const *c, int pool_index) {
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
    fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
}
void x86_emit_store(FILE *fp, int src_offset, int dst_offset, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);

    fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, src_offset, reg);
    fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
}
void x86_emit_load(FILE *fp, int addr_offset, int dst_offset, Type *t) {
    const char *reg = x86_rax_reg(t);
    const char *op_suffix = x86_op_suffix(t);

    fprintf(fp, "    movq %d(%%rbp), %%rax\n", addr_offset);
    fprintf(fp, "    mov%s (%%rax), %s\n", op_suffix, reg);
    // fprintf(fp, "    mov%s %d(%%rbp), %s\n", op_suffix, addr_offset, reg);
    fprintf(fp, "    mov%s %s, %d(%%rbp)\n", op_suffix, reg, dst_offset);
}
