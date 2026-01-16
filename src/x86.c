#include "compiler_c/ir.h"
#include "compiler_c/type.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int offset(const int a) {
    if (a < 0) {
        return -offset(a * -1);
    }
    return -(a * 8 + 8);
}
static void x86_gen_addr_instr(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    fprintf(fp, "    lea %d(%%rbp), %%rax\n", offset(instr->addr.src) - instr->addr.offset);
    fprintf(fp, "    movq %%rax, %d(%%rbp)\n", offset(instr->dst));
}
static void x86_gen_cast_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    Type *from = instr->cast.from;
    Type *to = instr->cast.to;
    int dst_offset = offset(instr->dst);
    int src_offset = offset(instr->cast.src);
    // char -> int : zero-extend
    if (from->kind == T_CHAR && to->kind == T_INT) {
        fprintf(fp, "    movzbl %d(%%rbp), %%eax\n", src_offset);
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", dst_offset);
        return;
    }
    // int -> char : truncate
    if (from->kind == T_INT && to->kind == T_CHAR) {
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", src_offset);
        fprintf(fp, "    movb %%al, %d(%%rbp)\n", dst_offset);
        return;
    }
    // int -> int : (just load to %eax)
    if (from->kind == T_INT && to->kind == T_INT) {
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", src_offset);
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", dst_offset);
        return;
    }
    // int/char -> float
    if ((from->kind == T_INT || from->kind == T_CHAR) && to->kind == T_FLOAT) {
        if (to->kind == T_CHAR) {
            fprintf(fp, "    movb %d(%%rbp), %%al\n", src_offset);
        } else {
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", src_offset);
        }
        fprintf(fp, "    cvtsi2ss %%eax, %%xmm0\n");
        fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", dst_offset);
        return;
    }

    // float -> int/char : truncate
    if (from->kind == T_FLOAT && (to->kind == T_INT || to->kind == T_CHAR)) {
        fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", src_offset);
        fprintf(fp, "    cvttss2si %%xmm0, %%eax\n");
        if (to->kind == T_CHAR) {
            fprintf(fp, "    movb %%al, %d(%%rbp)\n", dst_offset);
        } else {
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", dst_offset);
        }
        return;
    }

    // float -> float (just load into %xmm0)
    if (from->kind == T_FLOAT && to->kind == T_FLOAT) {
        fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", src_offset);
        fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", dst_offset);
        return;
    }

    printf("Cast node did literally nothing?\n");
    exit(1);
}
static void x86_gen_const_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    IR_Const *c = &ctx->module->const_pool.consts[instr->_const.pool_index];
    switch (instr->_const.type->kind) {
    case T_INT:
        fprintf(fp, "    movl $%d, %%eax\n", c->i);
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_FLOAT:
        fprintf(fp, "    movss .LC%d(%%rip), %%xmm0\n", instr->_const.pool_index);
        fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_CHAR:
        fprintf(fp, "    movb $%d, %%al\n", c->c);
        fprintf(fp, "    movb %%al, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_POINTER:
        if (instr->_const.type->ptr_to == type_char) {
            fprintf(fp, "    lea .LC%d(%%rip), %%rax\n", instr->_const.pool_index);
            fprintf(fp, "    movq %%rax, %d(%%rbp)\n", offset(instr->dst));
            return;
        }
    default:
        printf("Tried to gen x86 IR_CONST for unsupported type\n");
        exit(1);
    }
}
static void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->call.type->kind) {
    case T_INT:
        for (int i = 0; i < instr->call.arg_count; i++) {
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->call.args[i]));
            fprintf(fp, "    push %%rax\n");
        }
        fprintf(fp, "    call %s\n", ctx->module->defs[instr->call.callee].name);
        fprintf(fp, "    add $%d, %%rsp\n", instr->call.arg_count * 8);
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_FLOAT:
        for (int i = 0; i < instr->call.arg_count; i++) {
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->call.args[i]));
            fprintf(fp, "    sub $8, %%rsp\n");
            fprintf(fp, "    movss %%xmm0, (%%rsp)\n");
        }
        fprintf(fp, "    call %s\n", ctx->module->defs[instr->call.callee].name);
        fprintf(fp, "    add $%d, %%rsp\n", instr->call.arg_count * 8);
        fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_CHAR:
        for (int i = 0; i < instr->call.arg_count; i++) {
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->call.args[i]));
            fprintf(fp, "    push %%rax\n");
        }
        fprintf(fp, "    call %s\n", ctx->module->defs[instr->call.callee].name);
        fprintf(fp, "    add $%d, %%rsp\n", instr->call.arg_count * 8);
        fprintf(fp, "    movb %%al, %d(%%rbp)\n", offset(instr->dst));
        return;
    default:
        printf("Tried to gen x86 IR_CALL for unsupported type\n");
        exit(1);
    }
}
static void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->store.type->kind) {
    case T_INT:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->store.src));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_FLOAT:
        fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->store.src));
        fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_CHAR:
        fprintf(fp, "    movb %d(%%rbp), %%al\n", offset(instr->store.src));
        fprintf(fp, "    movb %%al, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_POINTER:
        fprintf(fp, "    movq %d(%%rbp), %%rax\n", offset(instr->store.src));
        fprintf(fp, "    movq %%rax, %d(%%rbp)\n", offset(instr->dst));
        return;
    default:
        printf("Tried to gen x86 IR_STORE for unsupported type\n");
        exit(1);
    }
}
static void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    fprintf(fp, "    movq %d(%%rbp), %%rax\n", offset(instr->load.addr));
    switch (instr->load.type->kind) {
    case T_INT:
        fprintf(fp, "    movl (%%rax), %%eax\n");
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        return;
    case T_FLOAT:
        fprintf(fp, "    movss (%%rax), %%xmm0\n");
        fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
        break;
    case T_CHAR:
        fprintf(fp, "    movzbl (%%rax), %%eax\n");
        fprintf(fp, "    movb %%al, %d(%%rbp)\n", offset(instr->dst));
        return;
    default:
        printf("Tried to gen x86 IR_LOAD for unsupported type\n");
        exit(1);
    }
}
static void x86_gen_unary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->unary.type->kind) {
    case T_INT:
        switch (instr->unary.op) {
        case POS:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->unary.expr));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case NEG:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->unary.expr));
            fprintf(fp, "    negl %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case LNOT:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->unary.expr));
            fprintf(fp, "    testl %%eax, %%eax\n");
            fprintf(fp, "    sete %%al\n");
            fprintf(fp, "    movzbl %%al, %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case BNOT:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->unary.expr));
            fprintf(fp, "    notl %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        }
    case T_FLOAT:
        switch (instr->unary.op) {
        case POS:
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->unary.expr));
            fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
            return;
        case NEG:
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->unary.expr));
            fprintf(fp, "    xorps %%xmm1, %%xmm1\n");
            fprintf(fp, "    subss %%xmm0, %%xmm1\n");
            fprintf(fp, "    movss %%xmm1, %d(%%rbp)\n", offset(instr->dst));
            return;
        case LNOT:
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->unary.expr));
            fprintf(fp, "    xorps %%xmm1, %%xmm1\n");
            fprintf(fp, "    ucomiss %%xmm1, %%xmm0\n");
            fprintf(fp, "    sete %%al\n");
            fprintf(fp, "    movzbl %%al, %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case BNOT:
            printf("Tried to perform Bitwise Not ~ on Type float\n");
            exit(1);
        }
    default:
        printf("Tried to gen x86 for IR_UNARY which is neither float or int");
        exit(1);
    }
}
static void x86_gen_binary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->binop.type->kind) {
    case T_INT:
        switch (instr->binop.op) {
        case ADD:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    addl %d(%%rbp), %%eax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case SUB:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    subl %d(%%rbp), %%eax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case MUL:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    imull %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case DIV:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    cltd\n");
            fprintf(fp, "    idivl %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case MOD:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    cltd\n");
            fprintf(fp, "    idivl %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%edx, %d(%%rbp)\n", offset(instr->dst));
            return;
        case AND:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    andl %d(%%rbp), %%eax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case OR:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    orl %d(%%rbp), %%eax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case XOR:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    xorl %d(%%rbp), %%eax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case SHL:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    movl %d(%%rbp), %%ecx\n", offset(instr->binop.rhs));
            fprintf(fp, "    shll %%cl, %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case SHR:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    movl %d(%%rbp), %%ecx\n", offset(instr->binop.rhs));
            fprintf(fp, "    sarl %%cl, %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            return;
        }
    case T_FLOAT:
        switch (instr->binop.op) {
        case ADD:
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->binop.lhs));
            fprintf(fp, "    addss %d(%%rbp), %%xmm0\n", offset(instr->binop.rhs));
            fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
            return;
        case SUB:
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->binop.lhs));
            fprintf(fp, "    subss %d(%%rbp), %%xmm0\n", offset(instr->binop.rhs));
            fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
            return;
        case MUL:
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->binop.lhs));
            fprintf(fp, "    mulss %d(%%rbp), %%xmm0\n", offset(instr->binop.rhs));
            fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
            return;
        case DIV:
            fprintf(fp, "    movss %d(%%rbp), %%xmm0\n", offset(instr->binop.lhs));
            fprintf(fp, "    divss %d(%%rbp), %%xmm0\n", offset(instr->binop.rhs));
            fprintf(fp, "    movss %%xmm0, %d(%%rbp)\n", offset(instr->dst));
            return;
        default:
            printf("Tried to perform unsuported binary operation on type float\n");
            exit(1);
        }
    case T_POINTER:
        switch (instr->binop.op) {
        // lhs: pointer, rhs: int
        case ADD:
            fprintf(fp, "    movq %d(%%rbp), %%rax\n", offset(instr->binop.lhs));
            fprintf(fp, "    addq %d(%%rbp), %%rax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movq %%rax, %d(%%rbp)\n", offset(instr->dst));
            return;
        case SUB:
            fprintf(fp, "    movq %d(%%rbp), %%rax\n", offset(instr->binop.lhs));
            fprintf(fp, "    subq %d(%%rbp), %%rax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movq %%rax, %d(%%rbp)\n", offset(instr->dst));
            return;
        // todo: add pointer-pointer subtraction
        default:
            printf("Tried to perform unsupported binary op on pointer\n");
            exit(1);
        }
    default:
        printf("Tried to gen x86 for IR_BINARY which is neither float or int");
        exit(1);
    }
}
static void x86_gen_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_UNOP:
        x86_gen_unary_instruction(fp, ctx, instr);
        return;
    case IR_BINOP:
        x86_gen_binary_instruction(fp, ctx, instr);
        return;
    case IR_LOAD:
        x86_gen_load_instruction(fp, ctx, instr);
        return;
    case IR_STORE:
        x86_gen_store_instruction(fp, ctx, instr);
        return;
    case IR_CALL:
        x86_gen_call_instruction(fp, ctx, instr);
        return;
    case IR_CONST:
        x86_gen_const_instruction(fp, ctx, instr);
        return;
    case IR_CAST:
        x86_gen_cast_instruction(fp, ctx, instr);
        return;
    case IR_ADDR:
        x86_gen_addr_instr(fp, ctx, instr);
        break;
    case IR_RET:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->ret.value));
        fprintf(fp, "    mov %%rbp, %%rsp\n");
        fprintf(fp, "    pop %%rbp\n");
        fprintf(fp, "    ret\n");
        return;
    case IR_BR:
        fprintf(fp, "    jmp %s_%d\n", ctx->func->name, instr->br.label);
        return;
    case IR_CMP:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->cmp.lhs));
        fprintf(fp, "    cmpl %d(%%rbp), %%eax\n", offset(instr->cmp.rhs));
        switch (instr->cmp.op) {
        case LT:
            fprintf(fp, "    setl %%al\n");
            break;
        case LE:
            fprintf(fp, "    setle %%al\n");
            break;
        case GT:
            fprintf(fp, "    setg %%al\n");
            break;
        case GE:
            fprintf(fp, "    setge %%al\n");
            break;
        case EQ:
            fprintf(fp, "    sete %%al\n");
            break;
        case NEQ:
            fprintf(fp, "    setne %%al\n");
            break;
        }
        fprintf(fp, "    movzbl %%al, %%eax\n");
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        return;
    case IR_BR_COND:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->br_cond.cond));
        fprintf(fp, "    testl %%eax, %%eax\n");
        fprintf(fp, "    jz %s_%d\n", ctx->func->name, instr->br_cond.f_label);
        break;
    }
}
static void x86_gen_block(FILE *fp, IR_Context *ctx) {
    for (int i = 0; i < ctx->block->count; i++) {
        x86_gen_instruction(fp, ctx, &ctx->block->instructions[i]);
    }
}
static void x86_gen_function(FILE *fp, IR_Context *ctx) {
    const int locals_size = ctx->func->max_reg * 8;
    const int stack_size = locals_size + 15 & ~15;
    fprintf(fp, "%s:\n", ctx->func->name);
    fprintf(fp, "    push %%rbp\n");
    fprintf(fp, "    mov %%rsp, %%rbp\n");
    fprintf(fp, "    subq $%d, %%rsp\n", stack_size);
    for (int i = 0; i < ctx->func->block_count; i++) {
        fprintf(fp, "%s_%d:\n", ctx->func->name, i);
        ctx->block = &ctx->func->blocks[i];
        x86_gen_block(fp, ctx);
    }
}
void x86_gen_module(FILE *fp, IR_Context *ctx) {
    // Const floats/strings
    if (ctx->module->const_pool.count > 0) {
        fprintf(fp, ".section .rodata\n");
        for (int i = 0; i < ctx->module->const_pool.count; i++) {
            IR_Const *c = &ctx->module->const_pool.consts[i];
            if (c->type == type_float) {
                uint32_t bits;
                memcpy(&bits, &c->f, sizeof(bits));
                fprintf(fp, ".LC%d:\n    .align 4\n    .long 0x%08x\n", i, bits);
            } else if (c->type->kind == T_POINTER && c->type->ptr_to == type_char) {
                fprintf(fp, ".LC%d:\n    .string \"%s\"\n", i, c->s);
            }
        }
        fprintf(fp, ".section .text\n\n");
    }

    fprintf(fp, ".global main\n");
    for (int i = 0; i < ctx->module->func_count; i++) {
        ctx->func = ctx->module->functions[i];
        x86_gen_function(fp, ctx);
    }
}
