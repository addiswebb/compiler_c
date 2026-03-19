// #include <stdio.h>
// typedef enum {
//     IR_CONST,
//     IR_UNOP,
//     IR_BINOP,
//     IR_LOAD,
//     IR_STORE,
//     IR_STORE_MEM,
//     IR_RET,
//     IR_CALL,
//     IR_BR,
//     IR_BR_COND,
//     IR_CMP,
//     IR_CAST,
//     IR_ADDR,
//     IR_ALLOCA,
//     IR_MEMCPY,
//     IR_LABEL,
//     IR_JMP,
// } IR_OP;

// typedef int IR_Value;

// typedef struct {
//     IR_OP op;
//     IR_Value ops[3];
//     int op_count;
// } IR_Instruction;

// int main() {
//     IR_Instruction instr = {.op = IR_ADDR, .op_count = 2, .ops = {[0] = 5, [1] = 7}};
//     printf("INSTR %d %d %d %d\n", instr.op, instr.op_count, instr.ops[0], instr.ops[1]);
//     return 0;
// }

typedef struct {
    int b[2];
} A;

int main() {
    A a;
    a.b[0] = 5;
    return a.b[0];
}
