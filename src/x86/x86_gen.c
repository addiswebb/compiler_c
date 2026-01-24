#include "compiler_c/ir/ir_module.h"
#include "compiler_c/type.h"
#include "compiler_c/x86.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void x86_gen_memcpy_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->ops[1].kind) {
    case IR_STACK:
        fprintf(fp, "    movq %d(%%rbp), %%rdx\n", instr->ops[1].stack_offset);
        break;
    case IR_LITERAL:
        fprintf(fp, "    lea .LC%d(%%rip), %%rdx\n", instr->ops[1].const_index);
        break;
    case IR_REG:
    case IR_MEM:
    case IR_UNDEFINED:
        printf("Sanity check failed\n");
        exit(1);
    }

    switch (instr->ops[0].kind) {
    case IR_STACK:
        fprintf(fp, "    lea %d(%%rbp), %%rcx\n", instr->ops[0].stack_offset);
        break;
    case IR_LITERAL:
        fprintf(fp, "    lea .LC%d(%%rip), %%rcx\n", instr->ops[0].const_index);
        break;
    case IR_REG:
    case IR_MEM:
    case IR_UNDEFINED:
        printf("Sanity check failed\n");
        exit(1);
    }
    fprintf(fp, "    mov $%d, %%r8d\n", instr->memcpy.size);
    fprintf(fp, "    call memcpy\n");
}

static void x86_gen_alloca_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    // printf("dst reg = %d", instr->ops[0].stack_offset);
    // exit(1);
    // below is incorrect(baked in offset!!)
    // fprintf(fp, "    lea %d(%%rbp), %%rax\n", instr->ops[0].stack_offset);
    // fprintf(fp, "    movq %%rax, %d(%%rbp)\n", instr->ops[0].stack_offset);
}

static void x86_gen_addr_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->ops[1].kind) {
    case IR_STACK:
        fprintf(fp, "    lea %d(%%rbp), %%rax\n", instr->ops[1].stack_offset - instr->addr.offset);
        break;
    case IR_LITERAL:
        fprintf(fp, "    lea .LC%d(%%rip), %%rax\n", instr->ops[1].const_index);
        break;
    case IR_REG:
    case IR_MEM:
    case IR_UNDEFINED:
        printf("Sanity check failed\n");
        exit(1);
    }
    fprintf(fp, "    movq %%rax, %d(%%rbp)\n", instr->ops[0].stack_offset);
}
static void x86_gen_cast_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    // char -> int : zero-extend
    x86_emit_cast(fp, instr->ops[1].stack_offset, instr->ops[0].stack_offset, instr->cast.from, instr->cast.to);
}
static void x86_gen_const_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    IR_Const *c = &ctx->module->const_pool.consts[instr->ops[1].const_index];
    x86_emit_const(fp, instr->ops[0].stack_offset, instr->_const.type, c, instr->ops[1].const_index);
}
static void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) { x86_emit_call(fp, ctx, instr); }
static void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_store(fp, instr->ops[1].stack_offset, instr->ops[0].stack_offset, instr->store.type);
}
static void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_load(fp, instr->ops[1].stack_offset, instr->ops[0].stack_offset, instr->load.type);
}
static void x86_gen_unary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_unary(fp, instr->ops[0].stack_offset, instr->ops[1].stack_offset, instr->unary.op, instr->unary.type);
}
static void x86_gen_binary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_binary(fp, instr->ops[0].stack_offset, instr->ops[1].stack_offset, instr->ops[2].stack_offset, instr->binop.op,
                    instr->binop.type);
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
        x86_gen_addr_instruction(fp, ctx, instr);
        break;
    case IR_ALLOCA:
        x86_gen_alloca_instruction(fp, ctx, instr);
        break;
    case IR_MEMCPY:
        x86_gen_memcpy_instruction(fp, ctx, instr);
        break;
    case IR_RET:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", instr->ops[0].stack_offset);
        fprintf(fp, "    mov %%rbp, %%rsp\n");
        fprintf(fp, "    pop %%rbp\n");
        fprintf(fp, "    ret\n");
        return;
    case IR_BR:
        fprintf(fp, "    jmp %s_%d\n", ctx->func->name, instr->br.label);
        return;
    case IR_CMP:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", instr->ops[1].stack_offset);
        fprintf(fp, "    cmpl %d(%%rbp), %%eax\n", instr->ops[2].stack_offset);
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
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", instr->ops[0].stack_offset);
        return;
    case IR_BR_COND:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", instr->ops[0].stack_offset);
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
    const int stack_size = ctx->func->stack_size;
    const int aligned_stack_size = stack_size + 15 & ~15;

    fprintf(fp, "%s:\n", ctx->func->name);
    fprintf(fp, "    push %%rbp\n");
    fprintf(fp, "    mov %%rsp, %%rbp\n");
    fprintf(fp, "    subq $%d, %%rsp\n", aligned_stack_size);
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
            } else if (c->type->kind == T_ARRAY && c->type->base == type_char) {
                fprintf(fp, ".LC%d:\n    .string \"%s\"\n", i, c->s.data);
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
