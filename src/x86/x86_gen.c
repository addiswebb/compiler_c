#include "compiler_c/ir/ir_module.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/type.h"
#include "compiler_c/x86.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void x86_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) {
    switch (instr->ops[1].kind) {
    case IR_STACK:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rdx");
        break;
    case IR_LITERAL:
    case IR_GLOBAL:
    case IR_PHYS_REG:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[1], "%rdx");
        break;
    case IR_VREG:
    case IR_MEM:
    case IR_UNDEFINED:
        printf("Sanity check failed\n");
        exit(1);
    }

    switch (instr->ops[0].kind) {
    case IR_STACK:
    case IR_LITERAL:
    case IR_GLOBAL:
    case IR_PHYS_REG:
        x86_emit_xr(fp, "lea", "", "", &instr->ops[0], "%rcx");
        break;
    case IR_VREG:
    case IR_MEM:
    case IR_UNDEFINED:
        printf("Sanity check failed\n");
        exit(1);
    }
    fprintf(fp, "    mov $%d, %%r8d\n", instr->memcpy.size);
    fprintf(fp, "    call memcpy\n");
}

static void x86_gen_addr_instruction(FILE *fp, const IR_Instruction *instr) {
    x86_emit_xr(fp, "lea", "", "", &instr->ops[1], "%rax");
    x86_emit_rx(fp, "mov", "q", "", "%rax", &instr->ops[0]);
}
static void x86_gen_cast_instruction(FILE *fp, const IR_Instruction *instr) {
    // char -> int : zero-extend
    x86_emit_cast(fp, &instr->ops[1], &instr->ops[0], instr->cast.from, instr->cast.to);
}
static void x86_gen_const_instruction(FILE *fp, const IR_Context *ctx, const IR_Instruction *instr) {
    IR_Literal *c = &ctx->module->const_pool.consts[instr->ops[1].const_index];
    x86_emit_const(fp, &instr->ops[0], instr->_const.type, c, instr->ops[1].const_index);
}
static void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) { x86_emit_call(fp, ctx, instr); }
static void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_store(fp, &instr->ops[1], &instr->ops[0], instr->store.type);
}

static void x86_gen_store_instruction_mem(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_store_mem(fp, &instr->ops[1], &instr->ops[0], instr->store.type);
}
static void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_load(fp, &instr->ops[1], &instr->ops[0], instr->load.type);
}
static void x86_gen_unary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_unary(fp, &instr->ops[0], &instr->ops[1], instr->unary.op, instr->unary.type);
}
static void x86_gen_binary_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_binary(fp, &instr->ops[0], &instr->ops[1], &instr->ops[2], instr->binop.op, instr->binop.type);
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
    case IR_STORE_MEM:
        x86_gen_store_instruction_mem(fp, ctx, instr);
        return;
    case IR_CALL:
        x86_gen_call_instruction(fp, ctx, instr);
        return;
    case IR_CONST:
        x86_gen_const_instruction(fp, ctx, instr);
        return;
    case IR_CAST:
        x86_gen_cast_instruction(fp, instr);
        return;
    case IR_ADDR:
        x86_gen_addr_instruction(fp, instr);
        break;
    case IR_ALLOCA:
        break;
    case IR_MEMCPY:
        x86_gen_memcpy_instruction(fp, instr);
        break;
    case IR_RET:
        x86_emit_xr(fp, "mov", "l", "", &instr->ops[0], "%eax");
        fprintf(fp, "    mov %%rbp, %%rsp\n");
        fprintf(fp, "    pop %%rbp\n");
        fprintf(fp, "    ret\n");
        return;
    case IR_BR:
        fprintf(fp, "    jmp %s_%d\n", ctx->func->name, instr->br.block->id);
        return;
    case IR_CMP:
        x86_emit_xr(fp, "mov", "l", "", &instr->ops[1], "%eax");
        x86_emit_xr(fp, "cmp", "l", "", &instr->ops[2], "%eax");
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
        x86_emit_rx(fp, "mov", "l", "", "%eax", &instr->ops[0]);
        return;
    case IR_BR_COND:
        x86_emit_xr(fp, "mov", "l", "", &instr->ops[0], "%eax");
        fprintf(fp, "    testl %%eax, %%eax\n");
        if (instr->br_cond.f_block) fprintf(fp, "    jz %s_%d\n", ctx->func->name, instr->br_cond.f_block->id);
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

    if (ctx->func->linkage == LINK_EXTERNAL) fprintf(fp, ".global %s\n", ctx->func->name);
    fprintf(fp, "%s:\n", ctx->func->name);
    fprintf(fp, "    push %%rbp\n");
    fprintf(fp, "    mov %%rsp, %%rbp\n");
    fprintf(fp, "    subq $%d, %%rsp\n", aligned_stack_size);
    for (int i = 0; i < ctx->func->block_count; i++) {
        fprintf(fp, "%s_%d:\n", ctx->func->name, i);
        ctx->block = ctx->func->blocks[i];
        x86_gen_block(fp, ctx);
    }
}
void x86_gen_module(FILE *fp, IR_Context *ctx) {
    // Const floats/strings
    if (ctx->module->const_pool.count > 0) {
        fprintf(fp, ".section .rodata\n");
        for (int i = 0; i < ctx->module->const_pool.count; i++) {
            const IR_Literal *c = &ctx->module->const_pool.consts[i];
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
    }
    if (ctx->module->global_pool.count > 0) {
        for (int i = 0; i < ctx->module->global_pool.count; i++) {
            const IR_Global *g = &ctx->module->global_pool.globals[i];
            const IR_Literal *c = &g->val;
            if (g->storage == STORAGE_NONE) {
                fprintf(fp, ".extern %s\n", g->name);
                continue;
            }
            if (g->linkage == LINK_EXTERNAL) fprintf(fp, ".global %s\n", g->name);
            if (g->storage == STORAGE_DATA) fprintf(fp, ".data\n");
            if (g->storage == STORAGE_BSS) {
                fprintf(fp, ".bss\n.align %d\n%s:\n    .zero %d\n", g->type->align, g->name, g->type->size);
            } else {
                if (c->type == type_invalid) {
                    printf("Received invalid type, probably an uninitialized global with incorrect storage specifier\n");
                    exit(1);
                }
                if (c->type == type_double) {
                    uint64_t bits;
                    memcpy(&bits, &c->f, sizeof(bits));
                    fprintf(fp, ".align 8\n%s:\n    .quad 0x%016llx\n", g->name, bits);
                } else if (c->type == type_float) {
                    uint32_t bits;
                    memcpy(&bits, &c->f, sizeof(bits));
                    fprintf(fp, ".align 4\n%s:\n    .long 0x%08x\n", g->name, bits);
                } else if (c->type->kind == T_ARRAY && c->type->base == type_char) {
                    fprintf(fp, ".align 8\n%s:\n    .string \"%s\"\n", g->name, c->s.data);
                } else if (c->type == type_int) {
                    fprintf(fp, ".align 4\n%s:\n    .long %d\n", g->name, (int)c->i);
                } else if (c->type == type_char) {
                    fprintf(fp, "%s:\n    .byte %d\n", g->name, (char)c->i);
                } else if (c->type == type_short) {
                    fprintf(fp, ".align 2\n%s:\n    .word %d\n", g->name, (short)c->i);
                } else if (c->type == type_long) {
                    fprintf(fp, ".align 8\n%s:\n    .quad %lld\n", g->name, c->i);
                }
            }
        }
    }

    fprintf(fp, "\n.text\n\n");
    for (int i = 0; i < ctx->module->func_count; i++) {
        ctx->func = ctx->module->functions[i];
        x86_gen_function(fp, ctx);
    }
}
