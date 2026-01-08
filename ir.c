#include "parser.c"

#include <stdio.h>
#include <stdlib.h>

typedef enum { IR_ADD, IR_SUB, IR_MUL, IR_DIV, IR_LOAD, IR_STORE, IR_RET } IR_OP;

typedef struct {
    const char *name;
    int reg;
} IR_Var;

typedef struct {
    IR_OP op;
    int dst;
    int a;
    int b;
} IR_Instruction;

typedef struct {
    const char *name;
    IR_Instruction *instructions;
    int count;
    int capacity;
    int next_reg;

    IR_Var *locals;
    int local_count;
    int local_capicity;
} IR_Function;

typedef struct {
    IR_Function **functions;
    int count;
    int capacity;
} IR_Module;

IR_OP token_to_ir_op(TokenType type) {
    switch (type) {
    case TK_PLUS:
        return IR_ADD;
    case TK_MINUS:
        return IR_SUB;
    case TK_MULTIPLY:
        return IR_MUL;
    case TK_DIVIDE:
        return IR_DIV;
    default:
        printf("Given an unsupported operator for binary op: ");
        print_token_type(type);
        printf("\n");
        exit(1);
    }
}
IR_Module *ir_new_module() {
    IR_Module *module = malloc(sizeof(*module));
    if (module == NULL) {
        printf("Failed to allocate new IR module\n");
        exit(1);
    }
    module->capacity = 4;
    module->count = 0;
    module->functions = malloc(sizeof(IR_Function *) * module->capacity);
    if (module->functions == NULL) {
        printf("Failed to allocate IR module functions\n");
        exit(1);
    }
    return module;
}

IR_Function *ir_new_function(const char *name) {
    IR_Function *func = malloc(sizeof(*func));
    if (func == NULL) {
        printf("Failed to allocate new IR function\n");
        exit(1);
    }
    func->capacity = 4;
    func->count = 0;
    func->name = name;
    func->next_reg = 0;
    func->instructions = malloc(sizeof(IR_Instruction) * func->capacity);
    if (func->instructions == NULL) {
        printf("Failed to allocate for IR function instructions\n");
        exit(1);
    }

    func->local_capicity = 4;
    func->local_count = 0;
    func->locals = malloc(sizeof(IR_Var) * func->local_capicity);
    if (func->locals == NULL) {
        printf("Failed to allocate for IR function local variables\n");
        exit(1);
    }
    return func;
}

void ir_append_instruction(IR_Function *func, IR_Instruction *instruction) {
    if (func->count >= func->capacity) {
        func->capacity *= 2;
        func->instructions = realloc(func->instructions, sizeof(IR_Instruction) * func->capacity);
        if (func->instructions == NULL) {
            printf("Failed to allocate for new Ir func");
            exit(1);
        }
    }
    func->instructions[func->count++] = *instruction;
}

int ir_new_var(IR_Function *func, const char *name) {
    if (func->local_count >= func->local_capicity) {
        func->local_capicity *= 2;
        func->locals = realloc(func->locals, sizeof(IR_Var) * func->local_capicity);
        if (func->locals == NULL) {
            printf("Failed to allocated for new local variable");
            exit(1);
        }
    }
    int next_reg = func->next_reg++;
    func->locals[func->local_count++] = (IR_Var){name, next_reg};
    return next_reg;
}

int ir_get_var_reg(IR_Function *func, const char *name) {
    for (int i = 0; i < func->local_count; i++) {
        if (strcmp(func->locals[i].name, name) == 0) {
            return func->locals[i].reg;
        }
    }
    return -1;
}

void ir_append_function(IR_Module *module, IR_Function *func) {
    if (module->count >= module->capacity) {
        module->capacity *= 2;
        module->functions = realloc(module->functions, sizeof(IR_Function *) * module->capacity);
        if (module->functions == NULL) {
            printf("Failed to allocate for new Ir Module");
            exit(1);
        }
    }
    module->functions[module->count++] = func;
}

void ir_free_module(IR_Module *module) {
    for (int i = 0; i < module->count; i++) {
        free(module->functions[i]->instructions);
        free(module->functions[i]);
    }
    free(module->functions);
    free(module);
}

int ir_gen_expression(IR_Function *func, Node *expr) {
    switch (expr->type) {
    case N_LITERAL:
        switch (expr->literal.type) {
        case TK_INT_LITERAL:
            int dst = func->next_reg++;
            ir_append_instruction(func, &(IR_Instruction){IR_LOAD, dst, expr->literal.i, 0});
            return dst;
        case TK_FLT_LITERAL:
            printf("Cannot handle floats yet soz");
            exit(1);
        default:
            printf("Given unknown literal");
            exit(1);
        }
    case N_IDENTIFIER:
        int var_reg = ir_get_var_reg(func, expr->identifer.name);
        if (var_reg == -1) {
            printf("Undefined local variable \'%s\' \n", expr->identifer.name);
            exit(1);
        }
        return var_reg;
    case N_BINARY:
        int a = ir_gen_expression(func, expr->binary.lhs);
        int b = ir_gen_expression(func, expr->binary.rhs);
        int dst = func->next_reg++;
        IR_OP op = token_to_ir_op(expr->binary.op);
        ir_append_instruction(func, &(IR_Instruction){op, dst, a, b});
        return dst;
    default:
        break;
    }
    printf("Failed to gen expr");
    exit(1);
}

void ir_gen_statement(IR_Function *func, Node *stmt);

void ir_gen_compound(IR_Function *func, Node *comp) {
    for (int i = 0; i < comp->compound.count; i++) {
        ir_gen_statement(func, comp->compound.statements[i]);
    }
}

void ir_gen_statement(IR_Function *func, Node *stmt) {
    switch (stmt->type) {
    case N_VAR_DECL:
        if (stmt->var_decl.type == TK_FLOAT) {
            printf("Soz cant handle floats yet, only integers\n");
            exit(1);
        }
        int var_reg = ir_new_var(func, stmt->var_decl.name);
        int expr_reg = ir_gen_expression(func, stmt->var_decl.expr);
        IR_Instruction var_decl_instr = {IR_STORE, var_reg, expr_reg, 0};
        ir_append_instruction(func, &var_decl_instr);
        return;
    case N_RETURN:
        int ret_reg = ir_gen_expression(func, stmt->_return.expr);
        IR_Instruction ret_instr = {IR_RET, ret_reg, 0, 0};
        ir_append_instruction(func, &ret_instr);
        return;
    case N_BINARY:
        if (stmt->binary.op == TK_EQ && stmt->binary.lhs->type == N_IDENTIFIER) {
            int var_reg = ir_get_var_reg(func, stmt->binary.lhs->identifer.name);
            int expr_reg = ir_gen_expression(func, stmt->binary.rhs);
            IR_Instruction assign_instr = {IR_STORE, var_reg, expr_reg, 0};
            ir_append_instruction(func, &assign_instr);
            return;
        } else {
            printf("Given binary op statement that is not an assignment\n");
            exit(1);
        }
    case N_COMPOUND:
        // No scopes currently
        ir_gen_compound(func, stmt);
        return;
    default:
        // given invalid statement? probably an expression
        printf("Dont know what to do with the given statemnet: ir_gen_statement\n");
        exit(1);
    }
}

IR_Function *ir_gen_function(Node *func) {
    if (func->type != N_FUNCTION) {
        printf("Tried ir_gen_translation_unit on a node which is not a translation unit!\n");
        exit(1);
    }

    IR_Function *fn = ir_new_function(func->function.name);
    switch (func->function.body->type) {
    case N_COMPOUND:
        ir_gen_compound(fn, func->function.body);
        break;
    default:
        printf("Function body is not a compound, gg\n");
        exit(1);
    }

    return fn;
}

IR_Module *ir_gen_translation_unit(Node *tu) {
    if (tu->type != N_TRANSLATION_UNIT) {
        printf("Tried ir_gen_translation_unit on a node which is not a translation unit!\n");
        exit(1);
    }

    IR_Module *module = ir_new_module();
    for (int i = 0; i < tu->translation_unit.count; i++) {
        ir_append_function(module, ir_gen_function(tu->translation_unit.declarations[i]));
    }

    return module;
}

void print_ir_op(IR_OP op) {
    switch (op) {
    case IR_ADD:
        printf("ADD   ");
        return;
    case IR_SUB:
        printf("SUB   ");
        return;
    case IR_MUL:
        printf("MUL   ");
        return;
    case IR_DIV:
        printf("DIV   ");
        return;
    case IR_LOAD:
        printf("LOAD  ");
        return;
    case IR_STORE:
        printf("STORE ");
        return;
    case IR_RET:
        printf("RET   ");
        return;
    default:
        printf("!!!   ");
        return;
    }
}

void print_ir_instruction(IR_Instruction *instr) {
    printf("    ");
    print_ir_op(instr->op);
    printf(" %d  %d  %d\n", instr->dst, instr->a, instr->b);
}

void print_ir_function(IR_Function *func) {
    printf("%s:\n", func->name);
    for (int i = 0; i < func->count; i++) {
        print_ir_instruction(&func->instructions[i]);
    }
}

void print_ir_module(IR_Module *module) {
    for (int i = 0; i < module->count; i++) {
        print_ir_function(module->functions[i]);
    }
}
