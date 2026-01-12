#include <compiler_c/ir.h>
#include <compiler_c/x86.h>

static int ir_reg_to_rbp(const int a) {
    if (a < 0) {
        return -ir_reg_to_rbp(a * -1);
    }
    return -(a * 8 + 8);
}

void x86_gen_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_ADD:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    addl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_SUB:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    subl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_MUL:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    imull %d(%%rbp)\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_DIV:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    idivl %d(%%rbp)\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_LOAD:
        fprintf(fp, "    movl $%d, %d(%%rbp)\n", instr->a, ir_reg_to_rbp(instr->dst));
        break;
    case IR_STORE:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_RET:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->dst));
        fprintf(fp, "    mov %%rbp, %%rsp\n");
        fprintf(fp, "    pop %%rbp\n");
        fprintf(fp, "    ret\n");
        break;
    case IR_BR:
        fprintf(fp, "    jmp %s_%d\n", ctx->func->name, instr->dst);
        break;
    case IR_BR_EQ:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->dst));
        fprintf(fp, "    testl %%eax, %%eax\n");
        fprintf(fp, "    jz %s_%d\n", ctx->func->name, instr->b);
        break;
    case IR_CALL:
        fprintf(fp, "    call %s\n", ctx->module->defs[instr->dst].name);
        fprintf(fp, "    add $%d, %%rsp\n", instr->b * 8);
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", ir_reg_to_rbp(instr->a));
        break;
    case IR_PUSH:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->dst));
        fprintf(fp, "    push %%rax\n");
        break;
    default:
        break;
    }
}

void x86_gen_block(FILE *fp, IR_Context *ctx) {
    for (int i = 0; i < ctx->block->count; i++) {
        x86_gen_instruction(fp, ctx, &ctx->block->instructions[i]);
    }
}

void x86_gen_function(FILE *fp, IR_Context *ctx) {
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
    // fprintf(fp, ".section .note.GNU-stack,\"\",@progbits\n"); FOR LINUX COMPILER
}

void x86_gen_module(FILE *fp, IR_Context *ctx) {
    fprintf(fp, ".global main\n");
    fprintf(fp, "    jmp main\n");
    for (int i = 0; i < ctx->module->func_count; i++) {
        ctx->func = ctx->module->functions[i];
        x86_gen_function(fp, ctx);
    }
}
