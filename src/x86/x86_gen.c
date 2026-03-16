#include "compiler_c/abi/abi.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/x86/x86.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void x86_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) { abi_gen_memcpy_instruction(fp, instr); }

static void x86_gen_addr_instruction(FILE *fp, const IR_Instruction *instr) { x86_emit_addr(fp, &instr->ops[1], &instr->ops[0]); }
static void x86_gen_cast_instruction(FILE *fp, const IR_Instruction *instr) {
    // char -> int : zero-extend
    x86_emit_cast(fp, &instr->ops[1], &instr->ops[0], instr->cast.from, instr->cast.to);
}
static void x86_gen_const_instruction(FILE *fp, const IR_Context *ctx, const IR_Instruction *instr) {
    IR_Literal *c = get_const(ctx, instr->ops[1].const_index);
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
        if (instr->ops[0].kind != IR_UNDEFINED) x86_emit_xr(fp, "mov", "l", "", &instr->ops[0], "%eax");
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
        bool use_unsigned = instr->cmp.type->kind == T_INT && !instr->cmp.type->is_signed;
        const char *al_reg = "%al";
        switch (instr->cmp.op) {
        case LT:
            x86_emit_r(fp, "set", use_unsigned ? "b" : "l", "", al_reg);
            break;
        case LE:
            x86_emit_r(fp, "set", use_unsigned ? "be" : "le", "", al_reg);
            break;
        case GT:
            x86_emit_r(fp, "set", use_unsigned ? "a" : "g", "", al_reg);
            break;
        case GE:
            x86_emit_r(fp, "set", use_unsigned ? "ae" : "ge", "", al_reg);
            break;
        case EQ:
            x86_emit_r(fp, "set", "e", "", al_reg);
            break;
        case NEQ:
            x86_emit_r(fp, "set", "ne", "", al_reg);
            break;
        }
        x86_emit_rr(fp, "mov", "zbl", "", "%al", "%eax");
        x86_emit_rx(fp, "mov", "l", "", "%eax", &instr->ops[0]);
        return;
    case IR_BR_COND:
        x86_emit_xr(fp, "mov", "l", "", &instr->ops[0], "%eax");
        fprintf(fp, "    testl %%eax, %%eax\n");
        if (instr->br_cond.f_block) fprintf(fp, "    jz %s_%d\n", ctx->func->name, instr->br_cond.f_block->id);
        break;
    case IR_LABEL:
        fprintf(fp, "%s:\n", instr->label.name);
        break;
    case IR_JMP:
        fprintf(fp, "    jmp %s\n", instr->jmp.name);
        break;
    }
}
static void x86_gen_block(FILE *fp, IR_Context *ctx) {
    for (int i = 0; i < ctx->block->instruction_array.count; i++) {
        x86_gen_instruction(fp, ctx, get_instruction(&ctx->block->instruction_array, i));
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
    for (int i = 0; i < ctx->func->blocks_array.count; i++) {
        fprintf(fp, "%s_%d:\n", ctx->func->name, i);
        ctx->block = get_block(ctx->func, i);
        x86_gen_block(fp, ctx);
    }
}
void x86_gen_module(FILE *fp, IR_Context *ctx) {
#ifndef _WIN64
    fprintf(fp, ".section .note.GNU-stack,\"\",@progbits\n");
#endif
    // Const data floats/strings
    if (ctx->module->const_array.count > 0) {
        fprintf(fp, ".section .rodata\n");
        for (int i = 0; i < ctx->module->const_array.count; i++) {
            const IR_Literal *c = get_const(ctx, i);
            if (c->type == type_f64) {
                uint64_t bits;
                memcpy(&bits, &c->f, sizeof(bits));
                fprintf(fp, ".align 8\n.LC%d:\n    .quad 0x%016llx\n", i, bits);
            } else if (c->type == type_f32) {
                uint32_t bits;
                float f = (float)c->f;
                memcpy(&bits, &f, sizeof(bits));
                fprintf(fp, ".align 4\n.LC%d:\n    .long 0x%08x\n", i, bits);
            } else if (c->type->kind == T_ARRAY && c->type->base == type_i8) {
                fprintf(fp, ".LC%d:\n", i);
                x86_emit_string(fp, c->s.data);
            }
        }
    }
    for (int i = 0; i < ctx->module->global_array.count; i++) {
        const IR_Global *g = get_global(ctx, i);
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
                PANIC("Received invalid type, probably an uninitialized global with incorrect storage specifier\n");
            }
            if (c->type == type_f64) {
                uint64_t bits;
                memcpy(&bits, &c->f, sizeof(bits));
                fprintf(fp, ".align 8\n%s:\n    .quad 0x%016llx\n", g->name, bits);
            } else if (c->type == type_f32) {
                uint32_t bits;
                memcpy(&bits, &c->f, sizeof(bits));
                fprintf(fp, ".align 4\n%s:\n    .long 0x%08x\n", g->name, bits);
            } else if (c->type->kind == T_ARRAY && c->type->base == type_i8) {
                fprintf(fp, ".align 8\n%s:\n", g->name);
                x86_emit_string(fp, c->s.data);
            } else if (c->type == type_i32) {
                fprintf(fp, ".align 4\n%s:\n    .long %d\n", g->name, (int)c->i);
            } else if (c->type == type_i8) {
                fprintf(fp, "%s:\n    .byte %d\n", g->name, (char)c->i);
            } else if (c->type == type_i16) {
                fprintf(fp, ".align 2\n%s:\n    .word %d\n", g->name, (short)c->i);
            } else if (c->type == type_i64) {
                fprintf(fp, ".align 8\n%s:\n    .quad %lld\n", g->name, c->i);
            }
        }
    }

    fprintf(fp, "\n.text\n");
    for (int i = 0; i < ctx->module->functions_array.count; i++) {
        ctx->func = get_func(ctx->module, i);
        x86_gen_function(fp, ctx);
    }
}
