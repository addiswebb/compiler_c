
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <compiler_c/ir.h>
#include <compiler_c/node.h>

IR_OP token_to_ir_op(const TokenType type) {
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
        printf("Given an unsupported token to convert to IR_Op: ");
        print_token_type(type);
        printf("\n");
        exit(1);
    }
}

/*
    Begin an IR Scope,
    Tracks any variables added afterwards, and pops them from the IR virtual stack when `ir_end_scope()` is called.
*/
void ir_begin_scope(IR_Function *func) {
    if (func->scope_count >= func->scope_capacity) {
        func->scope_capacity *= 2;
        IR_Scope *new_scopes = realloc(func->scopes, sizeof(IR_Scope) * func->scope_capacity);
        if (!new_scopes) {
            printf("Failed to realloc for func scopes\n");
            exit(1);
        }
        func->scopes = new_scopes;
    }
    func->scopes[func->scope_count++] = (IR_Scope){0};
}

/*
    Pops variables declared within the scope from the IR virtual stack.
*/
void ir_end_scope(IR_Function *func) {
    if (func->scope_count > 0) {
        func->scope_count -= 1;
        if (func->next_reg >= func->max_reg) {
            func->max_reg = func->next_reg;
        }
        func->local_count -= func->scopes[func->scope_count].var_count;
        func->next_reg -= func->scopes[func->scope_count].reg_count;
    }
}

int ir_next_reg(IR_Function *func) {
    func->scopes[func->scope_count - 1].reg_count++;
    return func->next_reg++;
}

/*
    Allocates for a new IR Module,
    Also initializes/allocates for its functions array
*/
IR_Module *ir_new_module() {
    IR_Module *module = malloc(sizeof(IR_Module));
    if (!module) {
        printf("Failed to allocate new IR module\n");
        exit(1);
    }
    module->func_capacity = 4;
    module->func_count = 0;
    module->functions = malloc(sizeof(IR_Function *) * module->func_capacity);
    if (module->functions == NULL) {
        printf("Failed to allocate IR module functions\n");
        free(module->functions);
        free(module);
        exit(1);
    }
    return module;
}

/*
    Allocates for a new IR_Block
*/
IR_Block *ir_new_block() {
    IR_Block *block = malloc(sizeof(IR_Block));
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
    func->max_reg = 0;
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

int ir_append_block(IR_Function *func, IR_Block *block) {
    if (func->block_count >= func->block_capacity) {
        func->block_capacity *= 2;
        IR_Block *new_blocks = realloc(func->blocks, sizeof(IR_Block) * func->block_capacity);
        if (!new_blocks) {
            printf("Failed to reallocate for new Ir block");
            exit(1);
        }
        func->blocks = new_blocks;
    }
    func->blocks[func->block_count++] = *block;
    free(block);
    return func->block_count - 1;
}

void ir_append_instruction(IR_Block *block, const IR_Instruction *instruction) {
    if (block->count >= block->capacity) {
        block->capacity *= 2;

        IR_Instruction *new_instructions = realloc(block->instructions, sizeof(IR_Instruction) * block->capacity);
        if (!new_instructions) {
            printf("Failed to reallocate for new Ir instr");
            exit(1);
        }
        block->instructions = new_instructions;
    }
    block->instructions[block->count++] = *instruction;
}

int ir_new_var(IR_Function *func, const char *name) {
    if (func->local_count >= func->local_capacity) {
        func->local_capacity *= 2;
        IR_Var *new_locals = realloc(func->locals, sizeof(IR_Var) * func->local_capacity);
        if (!new_locals) {
            printf("Failed to allocated for new local variable");
            exit(1);
        }
        func->locals = new_locals;
    }
    const int next_reg = ir_next_reg(func);
    func->locals[func->local_count++] = (IR_Var){name, next_reg};
    if (func->scope_count > 0) {
        func->scopes[func->scope_count - 1].var_count++;
    }
    return next_reg;
}

int ir_get_var_reg(const IR_Function *func, const char *name) {
    int sp = func->local_count - 1;
    for (int i = func->scope_count - 1; i >= 0; i--) {
        for (int j = 0; j < func->scopes[i].var_count; j++) {
            if (sp < 0) {
                printf("Locals and scope virtual stack pointer is corrupt or cooked\n");
                exit(1);
            }
            if (strcmp(func->locals[sp].name, name) == 0) {
                return func->locals[sp].reg;
            }
            sp--;
        }
    }
    return -1;
}

void ir_append_function(IR_Module *module, IR_Function *func) {
    if (module->func_count >= module->func_capacity) {
        module->func_capacity *= 2;
        IR_Function **new_functions = realloc(module->functions, sizeof(IR_Function *) * module->func_capacity);
        if (!new_functions) {
            printf("Failed to allocate for new Ir Module");
            exit(1);
        }
        module->functions = new_functions;
    }
    module->functions[module->func_count++] = func;
}

void ir_free_module(IR_Module *module) {
    for (int i = 0; i < module->func_count; i++) {
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

IR_Block *current_block(const IR_Function *func) { return &func->blocks[func->block_count - 1]; }

int ir_gen_expression(IR_Function *func, const Node *expr) {
    switch (expr->type) {
    case N_LITERAL:
        switch (expr->literal.type) {
        case TK_INT_LITERAL:
            const int dst = ir_next_reg(func);
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
        const int var_reg = ir_get_var_reg(func, expr->identifier.name);
        if (var_reg == -1) {
            printf("Undefined local variable \'%s\' \n", expr->identifier.name);
            exit(1);
        }
        return var_reg;
    case N_BINARY:
        const int a = ir_gen_expression(func, expr->binary.lhs);
        const int b = ir_gen_expression(func, expr->binary.rhs);
        if (expr->binary.op == TK_EQ) {
            const IR_Instruction assign_instr = {IR_STORE, a, b, 0};
            ir_append_instruction(current_block(func), &assign_instr);
            return a;
        }
        const int dst = ir_next_reg(func);
        const IR_OP op = token_to_ir_op(expr->binary.op);
        ir_append_instruction(current_block(func), &(IR_Instruction){op, dst, a, b});
        return dst;
    default:
        break;
    }
    printf("Failed to gen expr");
    exit(1);
}

void ir_gen_block_item(IR_Function *func, const Node *item) {
    if (item->type == N_VAR_DECL) {
        ir_gen_var_decl(func, item);
    } else {
        ir_gen_statement(func, item);
    }
}

void ir_gen_compound(IR_Function *func, const Node *comp) {
    ir_begin_scope(func);
    for (int i = 0; i < comp->compound.count; i++) {
        ir_gen_block_item(func, comp->compound.items[i]);
    }
    ir_end_scope(func);
}

void ir_gen_while_loop(IR_Function *func, const Node *_while) {
    const int cond_id = ir_append_block(func, ir_new_block()); // cond:
    const int cond_reg = ir_gen_expression(func, _while->_while.cond);
    const int block_id = cond_id + 1;
    const int end_id = cond_id + 2;
    const IR_Instruction br_eq_instr = {IR_BR_EQ, cond_reg, block_id, end_id};
    ir_append_instruction(current_block(func), &br_eq_instr);
    ir_append_block(func, ir_new_block()); // block:
    ir_gen_statement(func, _while->_while.block);
    const IR_Instruction br_instr = {IR_BR, cond_id, 0, 0};
    ir_append_instruction(current_block(func), &br_instr);
    ir_append_block(func, ir_new_block()); // end:
}
void ir_gen_for_loop(IR_Function *func, const Node *_for) {
    ir_gen_block_item(func, _for->_for.init);

    const int cond_id = ir_append_block(func, ir_new_block()); // cond:
    const int block_id = cond_id + 1;
    const int end_id = cond_id + 2;
    const int cond_reg = ir_gen_expression(func, _for->_for.cond);
    const IR_Instruction br_eq_instr = {IR_BR_EQ, cond_reg, block_id, end_id};
    ir_append_instruction(current_block(func), &br_eq_instr);

    ir_append_block(func, ir_new_block()); // block:
    ir_gen_statement(func, _for->_for.block);
    ir_gen_expression(func, _for->_for.iter);
    const IR_Instruction br_to_cond = {IR_BR, cond_id, 0, 0};
    ir_append_instruction(current_block(func), &br_to_cond);

    ir_append_block(func, ir_new_block()); // end:
}

void ir_gen_if_statement(IR_Function *func, const Node *_if) {
    const int cond_reg = ir_gen_expression(func, _if->_if.cond);
    const int if_true_id = func->block_count;
    const int if_false_id = if_true_id + 1; // if no else, then this is the end block
    const IR_Instruction br_eq_instr = {IR_BR_EQ, cond_reg, if_true_id, if_false_id};
    ir_append_instruction(current_block(func), &br_eq_instr);
    ir_append_block(func, ir_new_block()); // IF true block
    ir_gen_statement(func, _if->_if.if_true);
    if (_if->_if.if_false == NULL) { // No else, means branch to the end after compound
        const IR_Instruction br_instr = {IR_BR, if_false_id, 0, 0};
        ir_append_instruction(current_block(func), &br_instr);
        ir_append_block(func, ir_new_block()); // IF else or endblock
    } else {
        if (_if->_if.if_false->type == N_IF) {
            const IR_Instruction br_instr = {IR_BR, if_false_id, 0, 0};
            ir_append_instruction(current_block(func), &br_instr);
            ir_append_block(func, ir_new_block()); // IF else or endblock
            ir_gen_if_statement(func, _if->_if.if_false);
        } else {
            const int end_id = if_false_id + 1;
            const IR_Instruction br_instr = {IR_BR, end_id, 0, 0};
            ir_append_instruction(current_block(func), &br_instr);
            ir_append_block(func, ir_new_block()); // IF else or endblock
            ir_gen_statement(func, _if->_if.if_false);
            const IR_Instruction br_end_instr = {IR_BR, end_id, 0, 0};
            ir_append_instruction(current_block(func), &br_end_instr);
            ir_append_block(func, ir_new_block()); // end
        }
    }
}

void ir_gen_var_decl(IR_Function *func, const Node *var_decl) {
    if (var_decl->var_decl.type == TK_FLOAT) {
        printf("Soz cant handle floats yet, only integers\n");
        exit(1);
    }
    const int var_reg = ir_new_var(func, var_decl->var_decl.name);
    const int expr_reg = ir_gen_expression(func, var_decl->var_decl.expr);
    const IR_Instruction var_decl_instr = {IR_STORE, var_reg, expr_reg, 0};
    ir_append_instruction(current_block(func), &var_decl_instr);
    return;
}

void ir_gen_statement(IR_Function *func, const Node *stmt) {
    switch (stmt->type) {
    case N_RETURN:
        ir_gen_return(func, stmt);
        return;
    case N_BINARY:
        ir_gen_expression(func, stmt);
        return;
    case N_COMPOUND:
        ir_gen_compound(func, stmt);
        return;
    case N_IF:
        ir_gen_if_statement(func, stmt);
        return;
    case N_WHILE:
        ir_gen_while_loop(func, stmt);
        return;
    case N_FOR:
        ir_gen_for_loop(func, stmt);
        return;
    default:
        // given invalid statement? probably an expression
        printf("Dont know what to do with the given statemnet: ir_gen_statement: ");
        print_node_type(stmt->type);
        printf("\n");
        exit(1);
    }
}

void ir_gen_return(IR_Function *func, const Node *_return) {
    const int ret_reg = ir_gen_expression(func, _return->_return.expr);
    const IR_Instruction ret_instr = {IR_RET, ret_reg, 0, 0};
    ir_append_instruction(current_block(func), &ret_instr);
}

IR_Function *ir_gen_function(const Node *func) {
    if (func->type != N_FUNCTION) {
        printf("Tried ir_gen_function but given node is not a function!\n");
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

IR_Module *ir_gen_translation_unit(const Node *tu) {
    if (tu->type != N_TRANSLATION_UNIT) {
        printf("Tried ir_gen_function but given node is not a translation unit!\n");
        exit(1);
    }

    IR_Module *module = ir_new_module();
    for (int i = 0; i < tu->translation_unit.count; i++) {
        switch (tu->translation_unit.declarations[i]->type) {
        case N_FUNCTION:
            ir_append_function(module, ir_gen_function(tu->translation_unit.declarations[i]));
            break;
        case N_VAR_DECL:
            // Add support for globals eventually
        default:
            printf("Globals and other bs are not supported yet.\n");
            exit(1);
        }
    }

    return module;
}

void print_ir_op(const IR_OP op) {
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
        return;
    case IR_BR_EQ:
        printf("BREQ  ");
        return;
    default:
        printf("!!!   ");
    }
}

void print_ir_instruction(const IR_Instruction *instr) {
    printf("    ");
    print_ir_op(instr->op);
    printf(" %d  %d  %d\n", instr->dst, instr->a, instr->b);
}

void print_ir_block(const IR_Block *block) {
    for (int i = 0; i < block->count; i++) {
        print_ir_instruction(&block->instructions[i]);
    }
}

void print_ir_function(const IR_Function *func) {
    printf("%s:\n", func->name);
    for (int i = 0; i < func->block_count; i++) {
        printf("block_%d:\n", i);
        print_ir_block(&func->blocks[i]);
    }
}

void print_ir_module(const IR_Module *module) {
    for (int i = 0; i < module->func_count; i++) {
        print_ir_function(module->functions[i]);
    }
}
