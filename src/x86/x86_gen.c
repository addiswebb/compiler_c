#include "compiler_c/abi/abi.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/x86/x86.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void x86_gen_memset_instruction(FILE *fp, const IR_Instruction *instr) { abi_gen_memset_instruction(fp, instr); }
static void x86_gen_memcpy_instruction(FILE *fp, const IR_Instruction *instr) { abi_gen_memcpy_instruction(fp, instr); }
static void x86_gen_cmp_instruction(FILE *fp, const IR_Instruction *instr) {
    x86_emit_cmp(fp, instr->cmp.op, &instr->ops[0], &instr->ops[1], &instr->ops[2], instr->cmp.type);
}

static void x86_gen_addr_instruction(FILE *fp, const IR_Instruction *instr) { x86_emit_addr(fp, &instr->ops[1], &instr->ops[0]); }
static void x86_gen_cast_instruction(FILE *fp, const IR_Instruction *instr) {
    // char -> int : zero-extend
    x86_emit_cast(fp, &instr->ops[1], &instr->ops[0], instr->cast.from, instr->cast.to);
}
static void x86_gen_const_instruction(FILE *fp, const IR_Context *ctx, const IR_Instruction *instr) {
    if (instr->ops[1].kind == IR_CONSTANT) {
        ConstLiteral *c = get_const(ctx, instr->ops[1].const_index);
        x86_emit_const(fp, &instr->ops[0], instr->_const.type, c, instr->ops[1].const_index);
        return;
    }

    const char *reg = x86_rax_reg(instr->_const.type);
    const char *op_suffix = x86_op_suffix(instr->_const.type);
    x86_emit_xr(fp, "mov", op_suffix, "", &instr->ops[1], reg);
    x86_emit_rx(fp, "mov", op_suffix, "", reg, &instr->ops[0]);
}
static void x86_gen_call_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) { x86_emit_call(fp, ctx, instr); }
static void x86_gen_store_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_store(fp, &instr->ops[1], &instr->ops[0], instr->store.type);
}

static void x86_gen_load_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_load(fp, &instr->ops[1], &instr->ops[0], instr->load.type);
}
static void x86_gen_move_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    x86_emit_move(fp, &instr->ops[0], &instr->ops[1]);
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
    case IR_MOVE:
        x86_gen_move_instruction(fp, ctx, instr);
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
    case IR_MEMSET:
        x86_gen_memset_instruction(fp, instr);
        break;
    case IR_MEMCPY:
        x86_gen_memcpy_instruction(fp, instr);
        break;
    case IR_CMP:
        x86_gen_cmp_instruction(fp, instr);
        break;
    case IR_RET:
        // TODO: fix so instr->ret.type != type_void check works full
        /*
            I think it is related to struct return type handling,
            possibly converted to void return type, but still holds a value
         */
        if (instr->ops[0].kind != IR_UNDEFINED && instr->ret.type != type_void)
            x86_emit_xr(fp, "mov", x86_op_suffix(instr->ret.type), "", &instr->ops[0], x86_rax_reg(instr->ret.type));
        fprintf(fp, "    mov %%rbp, %%rsp\n");
        fprintf(fp, "    pop %%rbp\n");
        fprintf(fp, "    ret\n");
        return;
    case IR_BR:
        fprintf(fp, "    jmp %s_%d\n", ctx->func->name, instr->br.block->id);
        return;
    case IR_BR_COND:
        x86_emit_xr(fp, "mov", "l", "", &instr->ops[0], "%eax");
        fprintf(fp, "    testl %%eax, %%eax\n");
        if (instr->br_cond.f_block) fprintf(fp, "    jz %s_%d\n", ctx->func->name, instr->br_cond.f_block->id);
        else if (instr->br_cond.t_block) fprintf(fp, "    jnz %s_%d\n", ctx->func->name, instr->br_cond.t_block->id);
        break;
    case IR_LABEL:
        fprintf(fp, "%s:\n", instr->label.name);
        break;
    case IR_JMP:
        fprintf(fp, "    jmp %s\n", instr->jmp.name);
        break;
    case IR_BUILTIN_VA_START:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[0], "%rax");
        x86_emit_rx(fp, "mov", "q", "", "%rax", &instr->ops[1]);
        break;
    case IR_BUILTIN_VA_ARG:
        x86_emit_xr(fp, "mov", "q", "", &instr->ops[1], "%rax");
        fprintf(fp, "    addq $%d, %%rax\n", instr->builtin_va_arg.type->size);
        x86_emit_rr(fp, "mov", "q", "", "(%rax)", "%rcx");
        x86_emit_rx(fp, "mov", "q", "", "%rcx", &instr->ops[0]);
        break;
    case IR_PARAM:
        if (instr->op_count == 1) break;
        if (instr->param.type->size > 8) break;
        if (!memcmp(&instr->ops[0], &instr->ops[1], sizeof(IR_Value))) break;
        const char *param_op_suffix = x86_op_suffix(instr->param.type);
        if (instr->param.param_index < PARAM_REGISTERS && instr->param.param_index != -1) {
            x86_emit_xx(fp, "mov", param_op_suffix, "", &instr->ops[1], &instr->ops[0]);
        } else {
            const char *rax_reg = x86_rax_reg(instr->param.type);
            x86_emit_xr(fp, "mov", param_op_suffix, "", &instr->ops[1], rax_reg);
            x86_emit_rx(fp, "mov", param_op_suffix, "", rax_reg, &instr->ops[0]);
        }
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
    const int aligned_stack_size = STACK_ALIGN(stack_size);

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
            const ConstLiteral *c = get_const(ctx, i);
            if (c->type->align > 1) fprintf(fp, ".align %d\n", c->type->align);
            fprintf(fp, ".LC%d:\n", i);
            x86_emit_literal(fp, c);
        }
    }

    for (int i = 0; i < ctx->module->global_array.count; i++) {
        const IR_Global *g = get_global(ctx, i);
        const ConstLiteral *c = &g->val;
        if (g->symbol->storage == STORAGE_NONE) {
            fprintf(fp, ".extern %s\n", g->symbol->name);
            continue;
        }
        if (g->symbol->linkage == LINK_EXTERNAL) fprintf(fp, ".global %s\n", g->symbol->name);
        if (g->symbol->storage == STORAGE_DATA) fprintf(fp, ".data\n");
        if (g->symbol->storage == STORAGE_BSS) {
            fprintf(fp, ".bss\n.align %d\n%s:\n    .zero %d\n", g->symbol->type->align, g->symbol->name, g->symbol->type->size);
        } else {
            ASSERT(c->type != type_invalid, "Received invalid type, probably an uninitialized global with incorrect storage specifier\n");
            if (g->symbol->type->align > 1) fprintf(fp, ".align %d\n", g->symbol->type->align);
            fprintf(fp, "%s:\n", g->symbol->name);
            x86_emit_literal(fp, &g->val);
        }
    }

    fprintf(fp, "\n.text\n");
    for (int i = 0; i < ctx->module->functions_array.count; i++) {
        ctx->func = get_func(ctx->module, i);
        x86_gen_function(fp, ctx);
    }
}
