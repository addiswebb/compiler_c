#include "ir.c"

static int ir_reg_to_rbp(int a) { return a * 8 + 8; }

void x86_gen_instruction(IR_Instruction *instr) {
    switch (instr->op) {
    case IR_ADD:
        printf("    mov rax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->a));
        printf("    add rax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->b));
        printf("    mov -%d(%%rbp), rax\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_SUB:
        printf("    \n");
        break;
    case IR_MUL:
        printf("    \n");
        break;
    case IR_DIV:
        printf("    \n");
        break;
    case IR_LOAD:
        printf("    mov QWORD PTR -%d(%%rbp), %d\n", ir_reg_to_rbp(instr->dst), instr->a);
        break;
    case IR_STORE:
        printf("    mov rax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->a));
        printf("    mov -%d(%%rbp), rax\n", ir_reg_to_rbp(instr->dst));
        break;
    case IR_RET:
        printf("    mov rax, -%d(%%rbp)\n", ir_reg_to_rbp(instr->dst));
        break;
        break;
    }
}

void x86_gen_function(IR_Function *func) {
    int locals_size = func->local_count * 8;
    int stack_size = (locals_size + 15) & ~15;
    printf(".global %s\n", func->name);
    printf("%s:\n", func->name);
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, %d\n\n", stack_size);
    for (int i = 0; i < func->count; i++) {
        x86_gen_instruction(&func->instructions[i]);
    }
    printf("\n    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
}

void x86_gen_module(IR_Module *module) {
    for (int i = 0; i < module->count; i++) {
        x86_gen_function(module->functions[i]);
    }
};
