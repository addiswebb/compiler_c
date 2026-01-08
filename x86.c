#include "ir.c"

/*
    x86-64 Assembly
    AT&T dialect
    System V AMD 64
*/

static int ir_reg_to_rbp(int a) { return a * 8 + 8; }

void x86_gen_instruction(FILE *fp, IR_Instruction *instr) {
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
        break;
        break;
    }
}

void x86_gen_function(FILE *fp, IR_Function *func) {
    int locals_size = func->local_count * 8;
    int stack_size = (locals_size + 15) & ~15;
    fprintf(fp, ".global %s\n", func->name);
    fprintf(fp, "%s:\n", func->name);
    fprintf(fp, "    push %%rbp\n");
    fprintf(fp, "    mov %%rsp, %%rbp\n");
    fprintf(fp, "    subq $%d, %%rsp\n\n", stack_size);
    for (int i = 0; i < func->count; i++) {
        x86_gen_instruction(fp, &func->instructions[i]);
    }
    fprintf(fp, "\n    mov %%rbp, %%rsp\n");
    fprintf(fp, "    pop %%rbp\n");
    fprintf(fp, "    ret\n");
}

void x86_gen_module(FILE *fp, IR_Module *module) {
    for (int i = 0; i < module->count; i++) {
        x86_gen_function(fp, module->functions[i]);
    }
};
