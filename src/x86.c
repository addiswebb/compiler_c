#include <compiler_c/ir.h>
#include <compiler_c/x86.h>

static int offset(const int a) {
    if (a < 0) {
        return -offset(a * -1);
    }
    return -(a * 8 + 8);
}

void x86_gen_instruction(FILE *fp, IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_BINOP:
        switch (instr->binop.op) {
        case ADD:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    addl %d(%%rbp), %%eax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case SUB:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    subl %d(%%rbp), %%eax\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case MUL:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    imull %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case DIV:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    cltd\n");
            fprintf(fp, "    idivl %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case MOD:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    cltd\n");
            fprintf(fp, "    idivl %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%edx, %d(%%rbp)\n", offset(instr->dst));
            break;
        case AND:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    andl %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case OR:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    orl %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case XOR:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    xorl %d(%%rbp)\n", offset(instr->binop.rhs));
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case SHL:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    movl %d(%%rbp), %%ecx\n", offset(instr->binop.rhs));
            fprintf(fp, "    shll %%cl %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        case SHR:
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->binop.lhs));
            fprintf(fp, "    movl %d(%%rbp), %%ecx\n", offset(instr->binop.rhs));
            fprintf(fp, "    sarl %%cl %%eax\n");
            fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
            break;
        }
        break;
    case IR_LOAD:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->load.addr));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        break;
    case IR_STORE:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->store.addr));
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        break;
    case IR_RET:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->ret.value));
        fprintf(fp, "    mov %%rbp, %%rsp\n");
        fprintf(fp, "    pop %%rbp\n");
        fprintf(fp, "    ret\n");
        break;
    case IR_BR:
        fprintf(fp, "    jmp %s_%d\n", ctx->func->name, instr->br.label);
        break;
    case IR_CMP:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->br_cond.cond));
        fprintf(fp, "    testl %%eax, %%eax\n");
        fprintf(fp, "    jz %s_%d\n", ctx->func->name, instr->br_cond.f_label);
        break;
    case IR_CALL:
        for (int i = 0; i < instr->call.arg_count; i++) {
            fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->call.args[i]));
            fprintf(fp, "    push %%rax\n");
        }
        fprintf(fp, "    call %s\n", ctx->module->defs[instr->call.callee].name);
        fprintf(fp, "    add $%d, %%rsp\n", instr->call.arg_count * 8);
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        break;
    case IR_CONST:
        fprintf(fp, "    movl $%d, %%eax\n", instr->iconst.value);
        fprintf(fp, "    movl %%eax, %d(%%rbp)\n", offset(instr->dst));
        break;
    case IR_BR_COND:
        fprintf(fp, "    movl %d(%%rbp), %%eax\n", offset(instr->br_cond.cond));
        fprintf(fp, "    testl %%eax, %%eax\n");
        fprintf(fp, "    jz %s_%d\n", ctx->func->name, instr->br_cond.f_label);
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
}

void x86_gen_module(FILE *fp, IR_Context *ctx) {
    fprintf(fp, ".global main\n");
    fprintf(fp, "    jmp main\n");
    for (int i = 0; i < ctx->module->func_count; i++) {
        ctx->func = ctx->module->functions[i];
        x86_gen_function(fp, ctx);
    }
}
