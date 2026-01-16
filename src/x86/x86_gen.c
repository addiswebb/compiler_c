#include "compiler_c/ir.h"
#include "compiler_c/type.h"
#include "compiler_c/x86.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int offset(const int a) {
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
    // char -> int : zero-extend
    x86_emit_cast(fp, instr->cast.src, instr->dst, instr->cast.from, instr->cast.to);
}
static void x86_gen_const_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    IR_Const *c = &ctx->module->const_pool.consts[instr->_const.pool_index];
    x86_emit_const(fp, instr->dst, instr->_const.type, c, instr->_const.pool_index);
}
static void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) { x86_emit_call(fp, ctx, instr); }
static void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_store(fp, instr->store.src, instr->dst, instr->store.type);
}
static void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_load(fp, instr->load.addr, instr->dst, instr->load.type);
}
static void x86_gen_unary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_unary(fp, instr->dst, instr->unary.expr, instr->unary.op, instr->unary.type);
}
static void x86_gen_binary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_binary(fp, instr->dst, instr->binop.lhs, instr->binop.rhs, instr->binop.op, instr->binop.type);
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
            if (c->type == type_double) {
                uint64_t bits;
                memcpy(&bits, &c->f, sizeof(bits));
                fprintf(fp, ".align 8\n.LC%d:\n    .quad 0x%016llx\n", i, bits);
            } else if (c->type == type_float) {
                uint32_t bits;
                memcpy(&bits, &c->f, sizeof(bits));
                fprintf(fp, ".align 4\n.LC%d:\n    .long 0x%08x\n", i, bits);
            } else if (c->type->kind == T_POINTER && c->type->base == type_char) {
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
