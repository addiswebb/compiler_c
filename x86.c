#include "x86.h"

#include "ir.h"

static int ir_reg_to_rbp(const int a) { return a * 8 + 8; }

void x86_gen_instruction(FILE *fp, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_ADD:
        fprintf(fp, "    movl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    addl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_SUB:
        fprintf(fp, "    movl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    subl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_MUL:
        fprintf(fp, "    movl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    imull -%d(%%rbp)\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_DIV:
        fprintf(fp, "    movl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    idivl -%d(%%rbp)\n", ir_reg_to_rbp(instr->b));
        fprintf(fp, "    movl %%eax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_LOAD:
        fprintf(fp, "    movl $%d, -%d(%%rbp)\n", instr->a, ir_reg_to_rbp(instr->dst));
        break;
    case IR_STORE:
        fprintf(fp, "    movl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->a));
        fprintf(fp, "    movl %%eax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_RET:
        fprintf(fp, "    movl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->dst));
        fprintf(fp, "    jmp return\n");
        break;
    case IR_BR:
        fprintf(fp, "    jmp block_%d\n", instr->dst);
        break;
    case IR_BR_EQ:
        fprintf(fp, "    movl -%d(%%rbp), %%eax\n", ir_reg_to_rbp(instr->dst));
        fprintf(fp, "    testl %%eax, %%eax\n");
        fprintf(fp, "    jz block_%d\n", instr->b);
        break;
    default:
        break;
    }
}

void x86_gen_block(FILE *fp, const IR_Block *block) {
    for (int i = 0; i < block->count; i++) {
        x86_gen_instruction(fp, &block->instructions[i]);
    }
}

void x86_gen_function(FILE *fp, const IR_Function *func) {
    const int locals_size = func->local_count * 8;
    const int stack_size = (locals_size + 15) & ~15;
    fprintf(fp, ".global %s\n", func->name);
    fprintf(fp, "%s:\n", func->name);
    fprintf(fp, "    push %%rbp\n");
    fprintf(fp, "    mov %%rsp, %%rbp\n");
    fprintf(fp, "    subq $%d, %%rsp\n", stack_size);
    for (int i = 0; i < func->block_count; i++) {
        fprintf(fp, "block_%d:\n", i);
        x86_gen_block(fp, &func->blocks[i]);
    }
    fprintf(fp, "return:\n");
    fprintf(fp, "    mov %%rbp, %%rsp\n");
    fprintf(fp, "    pop %%rbp\n");
    fprintf(fp, "    ret\n");
    fprintf(fp, ".section .note.GNU-stack,\"\",@progbits\n");
}

void x86_gen_module(FILE *fp, const IR_Module *module) {
    for (int i = 0; i < module->count; i++) {
        x86_gen_function(fp, module->functions[i]);
    }
}
