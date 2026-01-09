#include "parser.c"

#include <stdio.h>
#include <stdlib.h>

typedef enum { IR_ADD, IR_SUB, IR_MUL, IR_DIV, IR_LOAD, IR_STORE, IR_RET, IR_BR, IR_BR_EQ } IR_OP;

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
    IR_Instruction *instructions;
    int count;
    int capacity;
} IR_Block;

typedef struct {
    int var_count;
} IR_Scope;

typedef struct {
    const char *name;
    IR_Block *blocks;
    int block_count;
    int block_capacity;
    int next_reg;
    IR_Var *locals;
    int local_count;
    int local_capacity;
    IR_Scope *scopes;
    int scope_count;
    int scope_capacity;
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

void ir_begin_scope(IR_Function *func) {
    if (func->scope_count >= func->scope_capacity) {
        func->scope_capacity *= 2;
        func->scopes = realloc(func->scopes, sizeof(IR_Scope) * func->scope_capacity);
        if (func->scopes == NULL) {
            printf("Failed to realloc for func scopes\n");
            exit(1);
        }
    }
    func->scopes[func->scope_count++] = (IR_Scope){0};
}

void ir_end_scope(IR_Function *func) {
    if (func->scope_count > 0) {
        func->scope_count -= 1;
        func->local_count -= func->scopes[func->scope_count].var_count;
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

void ir_append_block(IR_Function *func, IR_Block *block);

IR_Block *ir_new_block() {
    IR_Block *block = malloc(sizeof(*block));
    if (!block) {
        printf("Failed to allocate new block\n");
        exit(1);
    }
    block->capacity = 4;
    block->count = 0;
    block->instructions = malloc(sizeof(IR_Instruction) * block->capacity);
    if (!block->instructions) {
        printf("Failed to allocate for new block\n");
        exit(1);
    }

    return block;
}

IR_Function *ir_new_function(const char *name) {
    IR_Function *func = malloc(sizeof(*func));
    if (!func) {
        printf("Failed to allocate IR_Function\n");
        exit(1);
    }
    func->name = name;
    func->next_reg = 0;
    func->block_capacity = 4;
    func->block_count = 0;
    func->blocks = malloc(sizeof(IR_Block) * func->block_capacity);
    if (!func->blocks) {
        printf("Failed to allocate IR_Blocks\n");
        free(func);
        exit(1);
    }

    func->local_capacity = 4;
    func->local_count = 0;
    func->locals = malloc(sizeof(IR_Var) * func->local_capacity);
    if (!func->locals) {
        printf("Failed to allocated IR_Locals\n");
        free(func->blocks);
        free(func);
        exit(1);
    }

    func->scope_capacity = 4;
    func->scope_count = 0;
    func->scopes = malloc(sizeof(IR_Scope) * func->scope_capacity);
    if (!func->scopes) {
        printf("Failed to allocated IR_Scopes\n");
        free(func->blocks);
        free(func->locals);
        free(func);
        exit(1);
    }
    ir_append_block(func, ir_new_block());

    return func;
}

void ir_append_block(IR_Function *func, IR_Block *block) {
    if (func->block_count >= func->block_capacity) {
        func->block_capacity *= 2;
        func->blocks = realloc(func->blocks, sizeof(IR_Block) * func->block_capacity);
        if (func->blocks == NULL) {
            printf("Failed to reallocate for new Ir block");
            exit(1);
        }
    }
    func->blocks[func->block_count++] = *block;
    free(block);
}

void ir_append_instruction(IR_Block *block, IR_Instruction *instruction) {
    if (block->count >= block->capacity) {
        block->capacity *= 2;
        block->instructions = realloc(block->instructions, sizeof(IR_Instruction) * block->capacity);
        if (block->instructions == NULL) {
            printf("Failed to reallocate for new Ir instr");
            exit(1);
        }
    }
    block->instructions[block->count++] = *instruction;
}

int ir_new_var(IR_Function *func, const char *name) {
    if (func->local_count >= func->local_capacity) {
        func->local_capacity *= 2;
        func->locals = realloc(func->locals, sizeof(IR_Var) * func->local_capacity);
        if (func->locals == NULL) {
            printf("Failed to allocated for new local variable");
            exit(1);
        }
    }
    int next_reg = func->next_reg++;
    func->locals[func->local_count++] = (IR_Var){name, next_reg};
    if (func->scope_count > 0) {
        func->scopes[func->scope_count - 1].var_count++;
    }
    return next_reg;
}

int ir_get_var_reg(IR_Function *func, const char *name) {
    int sp = func->local_count - 1;
    for (int i = func->scope_count - 1; i >= 0; i--) {
        for (int j = 0; j < func->scopes[i].var_count; j++) {
            if (sp < 0) {
                printf("Locals and scope virtual stack pointer is corrupt or cooked\n");
                exit(1);
            }
            if (strcmp(func->locals[sp].name, name) == 0) {
                return func->locals[sp--].reg;
            }
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
        IR_Function *func = module->functions[i];
        for (int j = 0; j < func->block_count; j++) {
            free(func->blocks[j].instructions);
        }
        free(func->locals);
        free(func->scopes);
        free(func->blocks);
        free(func);
    }
    free(module->functions);
    free(module);
}

IR_Block *current_block(IR_Function *func) { return &func->blocks[func->block_count - 1]; }

int ir_gen_expression(IR_Function *func, Node *expr) {
    switch (expr->type) {
    case N_LITERAL:
        switch (expr->literal.type) {
        case TK_INT_LITERAL:
            int dst = func->next_reg++;
            ir_append_instruction(&func->blocks[func->block_count - 1], &(IR_Instruction){IR_LOAD, dst, expr->literal.i, 0});
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
        ir_append_instruction(current_block(func), &(IR_Instruction){op, dst, a, b});
        return dst;
    default:
        break;
    }
    printf("Failed to gen expr");
    exit(1);
}

void ir_gen_statement(IR_Function *func, Node *stmt);

void ir_gen_compound(IR_Function *func, Node *comp) {
    ir_begin_scope(func);
    for (int i = 0; i < comp->compound.count; i++) {
        ir_gen_statement(func, comp->compound.statements[i]);
    }
    ir_end_scope(func);
}

void ir_gen_if_statement(IR_Function *func, Node *_if) {
    int cond_reg = ir_gen_expression(func, _if->_if.cond);
    int if_true_id = func->block_count;
    int if_false_id = if_true_id + 1; // if no else, then this is the end block
    IR_Instruction br_eq_instr = {IR_BR_EQ, cond_reg, if_true_id, if_false_id};
    ir_append_instruction(current_block(func), &br_eq_instr);
    ir_append_block(func, ir_new_block()); // IF true block
    ir_gen_compound(func, _if->_if.if_true);
    if (_if->_if.if_false == NULL) { // No else, means branch to the end after compound
        IR_Instruction br_instr = {IR_BR, if_false_id, 0, 0};
        ir_append_instruction(current_block(func), &br_instr);
        ir_append_block(func, ir_new_block()); // IF else or endblock
        return;
    } else {
        int end_id = if_false_id + 1;
        IR_Instruction br_instr = {IR_BR, end_id, 0, 0};
        ir_append_instruction(current_block(func), &br_instr);
        ir_append_block(func, ir_new_block()); // IF else or endblock
        ir_gen_compound(func, _if->_if.if_false);
        IR_Instruction br_end_instr = {IR_BR, end_id, 0, 0};
        ir_append_instruction(current_block(func), &br_end_instr);
        ir_append_block(func, ir_new_block()); // end
    }

    return;
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
        ir_append_instruction(current_block(func), &var_decl_instr);
        return;
    case N_RETURN:
        int ret_reg = ir_gen_expression(func, stmt->_return.expr);
        IR_Instruction ret_instr = {IR_RET, ret_reg, 0, 0};
        ir_append_instruction(current_block(func), &ret_instr);
        return;
    case N_BINARY:
        if (stmt->binary.op == TK_EQ && stmt->binary.lhs->type == N_IDENTIFIER) {
            int var_reg = ir_get_var_reg(func, stmt->binary.lhs->identifer.name);
            int expr_reg = ir_gen_expression(func, stmt->binary.rhs);
            IR_Instruction assign_instr = {IR_STORE, var_reg, expr_reg, 0};
            ir_append_instruction(current_block(func), &assign_instr);
            return;
        } else {
            printf("Given binary op statement that is not an assignment\n");
            exit(1);
        }
    case N_COMPOUND:
        ir_gen_compound(func, stmt);
        return;
    case N_IF:
        ir_gen_if_statement(func, stmt);
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
    case IR_BR:
        printf("BR    ");
        break;
    case IR_BR_EQ:
        printf("BREQ  ");
        break;
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

void print_ir_block(IR_Block *block) {
    for (int i = 0; i < block->count; i++) {
        print_ir_instruction(&block->instructions[i]);
    }
}

void print_ir_function(IR_Function *func) {
    printf("%s:\n", func->name);
    for (int i = 0; i < func->block_count; i++) {
        printf("block_%d:\n", i);
        print_ir_block(&func->blocks[i]);
    }
}

void print_ir_module(IR_Module *module) {
    for (int i = 0; i < module->count; i++) {
        print_ir_function(module->functions[i]);
    }
}
