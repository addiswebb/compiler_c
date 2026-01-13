
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler_c/tokenizer.h"
#include <compiler_c/ir.h>
#include <compiler_c/node.h>

IR_CMP_OP ir_cmp_op(const TokenType type) {
    switch (type) {
    case TK_LT:
        return LT;
    case TK_LE:
        return LE;
    case TK_GT:
        return GT;
    case TK_GE:
        return GE;
    case TK_EQ:
        return EQ;
    case TK_NEQ:
        return NEQ;
    default:
        printf("Given an unsupported token to convert to IR cmp op: ");
        print_token_type(type);
        printf("\n");
        exit(1);
    }
}
IR_BINOP_OP ir_bin_op(const TokenType type) {
    switch (type) {
    case TK_PLUS:
        return ADD;
    case TK_MINUS:
        return SUB;
    case TK_MULTIPLY:
        return MUL;
    case TK_DIVIDE:
        return DIV;
    case TK_MOD:
        return MOD;
    case TK_AND:
        return AND;
    case TK_OR:
        return OR;
    case TK_XOR:
        return XOR;
    case TK_SHL:
        return SHL;
    case TK_SHR:
        return SHR;
    default:
        printf("Given an unsupported token to convert to IR Binary op: ");
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
    module->defs = malloc(sizeof(IR_Func_Def) * module->func_capacity);
    if (module->defs == NULL) {
        printf("Failed to allocate IR module function definitions\n");
        free(module->defs);
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

IR_Function *ir_new_function(IR_Context *ctx, const char *name) {
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
    ctx->func = func;
    ir_append_block(ctx, ir_new_block());
    return func;
}

int ir_append_block(IR_Context *ctx, IR_Block *block) {
    IR_Function *func = ctx->func;
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
    ctx->block = &func->blocks[func->block_count - 1];
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

int ir_get_func_def(const IR_Context *ctx, const char *name) {
    for (int i = 0; i < ctx->module->func_count; i++) {
        if (strcmp(ctx->module->defs[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
int ir_get_var_reg(const IR_Context *ctx, const char *name) {
    IR_Function *func = ctx->func;
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

void ir_new_func_def(IR_Module *module, IR_Function *func) {
    for (int i = 0; i < module->func_count; i++) {
        if (strcmp(module->defs[i].name, func->name) == 0) {
            printf("Function %s is already defined at [%d]", func->name, i);
            exit(1);
        }
    }
    IR_Func_Def def = {func->name, module->func_count};
    module->defs[module->func_count] = def;
}
void ir_append_function(IR_Module *module, IR_Function *func) {
    if (module->func_count >= module->func_capacity) {
        module->func_capacity *= 2;
        IR_Function **new_functions = realloc(module->functions, sizeof(IR_Function *) * module->func_capacity);
        if (!new_functions) {
            printf("Failed to allocate for new Ir Module");
            exit(1);
        }
        IR_Func_Def *new_func_defs = realloc(module->defs, sizeof(IR_Func_Def) * module->func_capacity);
        if (!new_func_defs) {
            printf("Failed to allocate for new Ir Module");
            exit(1);
        }
        module->functions = new_functions;
        module->defs = new_func_defs;
    }
    ir_new_func_def(module, func);
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

int ir_gen_expression(IR_Context *ctx, const Node *expr) {
    switch (expr->type) {
    case N_LITERAL:
        switch (expr->literal.type) {
        case TK_INT_LITERAL:
            IR_Instruction instr;
            instr.op = IR_CONST;
            instr.dst = ir_next_reg(ctx->func);
            instr.iconst.value = expr->literal.i;
            ir_append_instruction(ctx->block, &instr);
            return instr.dst;
        case TK_FLT_LITERAL:
            printf("Cannot handle floats yet soz");
            exit(1);
        default:
            printf("Given unknown literal");
            exit(1);
        }
    case N_IDENTIFIER:
        const int var_reg = ir_get_var_reg(ctx, expr->identifier.name);
        if (var_reg == -1) {
            printf("Undefined local variable \'%s\' \n", expr->identifier.name);
            exit(1);
        }
        return var_reg;
    case N_BINARY:
        int lhs = ir_gen_expression(ctx, expr->binary.lhs);
        int rhs = ir_gen_expression(ctx, expr->binary.rhs);
        if (expr->binary.op == TK_EQ) {
            IR_Instruction assign_instr;
            assign_instr.op = IR_STORE;
            assign_instr.dst = lhs;
            assign_instr.store.addr = rhs;
            ir_append_instruction(ctx->block, &assign_instr);
            return lhs;
        }
        IR_Instruction binop;
        binop.op = IR_BINOP;
        binop.binop.op = ir_bin_op(expr->binary.op);
        binop.binop.lhs = lhs;
        binop.binop.rhs = rhs;
        binop.dst = ir_next_reg(ctx->func);
        ir_append_instruction(ctx->block, &binop);
        return binop.dst;
    case N_FUNCTION_CALL:
        IR_Instruction func_call;
        func_call.op = IR_CALL;
        func_call.call.callee = ir_get_func_def(ctx, expr->func_call.identifier->identifier.name);
        func_call.call.arg_count = expr->func_call.param_count;
        func_call.call.args = malloc(sizeof(int) * func_call.call.arg_count);
        if (!func_call.call.args) {
            printf("Failed to malloc for IR_CALL args\n");
            exit(1);
        }
        for (int i = 0; i < func_call.call.arg_count; i++) {
            const int arg_reg = ir_gen_expression(ctx, expr->func_call.params[i]);
            func_call.call.args[i] = arg_reg;
        }
        func_call.dst = ir_next_reg(ctx->func);
        ir_append_instruction(ctx->block, &func_call);
        return func_call.dst;
    default:
        break;
    }
    printf("Failed to gen expr for ");
    print_node_type(expr->type);
    printf("\n");
    exit(1);
}

void ir_gen_block_item(IR_Context *ctx, const Node *item) {
    if (item->type == N_VAR_DECL) {
        ir_gen_var_decl(ctx, item);
    } else {
        ir_gen_statement(ctx, item);
    }
}

void ir_gen_compound(IR_Context *ctx, const Node *comp) {
    ir_begin_scope(ctx->func);
    for (int i = 0; i < comp->compound.count; i++) {
        ir_gen_block_item(ctx, comp->compound.items[i]);
    }
    ir_end_scope(ctx->func);
}

void ir_gen_while_loop(IR_Context *ctx, const Node *_while) {
    const int cond_id = ir_append_block(ctx, ir_new_block()); // cond:
    const int cond_reg = ir_gen_expression(ctx, _while->_while.cond);
    const int block_id = cond_id + 1;
    const int end_id = cond_id + 2;
    IR_Instruction br_eq_instr;
    br_eq_instr.op = IR_BR_COND;
    br_eq_instr.dst = -1;
    br_eq_instr.br_cond.cond = cond_reg;
    br_eq_instr.br_cond.t_label = block_id;
    br_eq_instr.br_cond.f_label = end_id;
    ir_append_instruction(ctx->block, &br_eq_instr);
    ir_append_block(ctx, ir_new_block()); // block:
    ir_gen_statement(ctx, _while->_while.block);
    IR_Instruction br_instr;
    br_instr.op = IR_BR;
    br_instr.dst = -1;
    br_instr.br.label = cond_id;
    ir_append_instruction(ctx->block, &br_instr);
    ir_append_block(ctx, ir_new_block()); // end:
}
void ir_gen_for_loop(IR_Context *ctx, const Node *_for) {
    ir_gen_block_item(ctx, _for->_for.init);

    const int cond_id = ir_append_block(ctx, ir_new_block()); // cond:
    const int block_id = cond_id + 1;
    const int end_id = cond_id + 2;
    const int cond_reg = ir_gen_expression(ctx, _for->_for.cond);

    IR_Instruction br_eq_instr;
    br_eq_instr.op = IR_BR_COND;
    br_eq_instr.dst = -1;
    br_eq_instr.br_cond.cond = cond_reg;
    br_eq_instr.br_cond.t_label = block_id;
    br_eq_instr.br_cond.f_label = end_id;
    ir_append_instruction(ctx->block, &br_eq_instr);

    ir_append_block(ctx, ir_new_block()); // block:
    ir_gen_statement(ctx, _for->_for.block);
    ir_gen_expression(ctx, _for->_for.iter);

    IR_Instruction br_to_cond;
    br_to_cond.op = IR_BR;
    br_to_cond.dst = -1;
    br_to_cond.br.label = cond_id;
    ir_append_instruction(ctx->block, &br_to_cond);
    ir_append_block(ctx, ir_new_block()); // end:
}

void ir_gen_if_statement(IR_Context *ctx, const Node *_if) {
    const int cond_reg = ir_gen_expression(ctx, _if->_if.cond);
    const int if_true_id = ctx->func->block_count;
    const int if_false_id = if_true_id + 1; // if no else, then this is the end block
    IR_Instruction br_eq_instr;
    br_eq_instr.op = IR_BR_COND;
    br_eq_instr.dst = -1;
    br_eq_instr.br_cond.cond = cond_reg;
    br_eq_instr.br_cond.t_label = if_true_id;
    br_eq_instr.br_cond.f_label = if_false_id;

    ir_append_instruction(ctx->block, &br_eq_instr);
    ir_append_block(ctx, ir_new_block()); // IF true block
    ir_gen_statement(ctx, _if->_if.if_true);
    if (_if->_if.if_false == NULL) { // No else, means branch to the end after compound
        IR_Instruction br_instr;
        br_instr.op = IR_BR;
        br_instr.dst = -1;
        br_instr.br.label = if_false_id;
        ir_append_instruction(ctx->block, &br_instr);
        ir_append_block(ctx, ir_new_block()); // IF else or endblock
    } else {
        if (_if->_if.if_false->type == N_IF) {
            IR_Instruction br_instr;
            br_instr.op = IR_BR;
            br_instr.dst = -1;
            br_instr.br.label = if_false_id;
            ir_append_instruction(ctx->block, &br_instr);
            ir_append_block(ctx, ir_new_block()); // IF else or endblock
            ir_gen_if_statement(ctx, _if->_if.if_false);
        } else {
            const int end_id = if_false_id + 1;
            IR_Instruction br_instr;
            br_instr.op = IR_BR;
            br_instr.dst = -1;
            br_instr.br.label = end_id;
            ir_append_instruction(ctx->block, &br_instr);
            ir_append_block(ctx, ir_new_block()); // IF else or endblock
            ir_gen_statement(ctx, _if->_if.if_false);
            IR_Instruction br_end_instr;
            br_instr.op = IR_BR;
            br_end_instr.dst = -1;
            br_end_instr.br.label = end_id;
            ir_append_instruction(ctx->block, &br_end_instr);
            ir_append_block(ctx, ir_new_block()); // end
        }
    }
}

void ir_gen_var_decl(IR_Context *ctx, const Node *var_decl) {
    if (var_decl->var_decl.type == TK_FLOAT) {
        printf("Soz cant handle floats yet, only integers\n");
        exit(1);
    }
    IR_Instruction var_decl_instr; //= {IR_STORE, var_reg, expr_reg, 0};
    var_decl_instr.op = IR_STORE;
    var_decl_instr.dst = ir_new_var(ctx->func, var_decl->var_decl.name);
    var_decl_instr.store.addr = ir_gen_expression(ctx, var_decl->var_decl.expr);
    ir_append_instruction(ctx->block, &var_decl_instr);
    return;
}

void ir_gen_statement(IR_Context *ctx, const Node *stmt) {
    switch (stmt->type) {
    case N_RETURN:
        ir_gen_return(ctx, stmt);
        return;
    case N_BINARY:
        ir_gen_expression(ctx, stmt);
        return;
    case N_COMPOUND:
        ir_gen_compound(ctx, stmt);
        return;
    case N_IF:
        ir_gen_if_statement(ctx, stmt);
        return;
    case N_WHILE:
        ir_gen_while_loop(ctx, stmt);
        return;
    case N_FOR:
        ir_gen_for_loop(ctx, stmt);
        return;
    case N_FUNCTION_CALL:
        ir_gen_expression(ctx, stmt);
        return;
    default:
        // given invalid statement? probably an expression
        printf("Dont know what to do with the given statemnet: ir_gen_statement: ");
        print_node_type(stmt->type);
        printf("\n");
        exit(1);
    }
}

void ir_gen_return(IR_Context *ctx, const Node *_return) {
    const int ret_reg = ir_gen_expression(ctx, _return->_return.expr);
    IR_Instruction ret_instr;
    ret_instr.op = IR_RET;
    ret_instr.dst = -1;
    ret_instr.ret.value = ret_reg;
    ir_append_instruction(ctx->block, &ret_instr);
}

IR_Function *ir_gen_function(IR_Context *ctx, const Node *func) {
    if (func->type != N_FUNCTION) {
        printf("Tried ir_gen_function but given node is not a function!\n");
        exit(1);
    }

    IR_Function *fn = ir_new_function(ctx, func->func.name);
    if (func->func.body->type != N_COMPOUND) {
        printf("Function body is not a compound,\n");
        exit(1);
    }

    ir_begin_scope(fn);
    // handle (params)
    for (int i = 0; i < func->func.param_count; i++) {
        int test = ir_new_var(ctx->func, func->func.params[i]->var_decl.name);
        IR_Instruction store_instr;
        store_instr.op = IR_STORE;
        store_instr.dst = i;
        if (test != i) {
            printf("I WAS WRONG!!\n");
            exit(1);
        }
        store_instr.store.addr = -func->func.param_count + i;
        ir_append_instruction(ctx->block, &store_instr);
    }
    // handle {[statement]*}
    for (int i = 0; i < func->func.body->compound.count; i++) {
        ir_gen_block_item(ctx, func->func.body->compound.items[i]);
    }
    ir_end_scope(fn);

    return fn;
}

IR_Module *ir_gen_translation_unit(IR_Context *ctx, const Node *tu) {
    if (tu->type != N_TRANSLATION_UNIT) {
        printf("Tried ir_gen_function but given node is not a translation unit!\n");
        exit(1);
    }

    IR_Module *module = ir_new_module();
    ctx->module = module;
    for (int i = 0; i < tu->translation_unit.count; i++) {
        switch (tu->translation_unit.declarations[i]->type) {
        case N_FUNCTION:
            ir_append_function(ctx->module, ir_gen_function(ctx, tu->translation_unit.declarations[i]));
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

void print_bin_op(IR_BINOP_OP op) {
    switch (op) {
    case ADD:
        printf("ADD");
        break;
    case SUB:
        printf("SUB");
        break;
    case MUL:
        printf("MUL");
        break;
    case DIV:
        printf("DIV");
        break;
    case MOD:
        printf("MOD");
        break;
    case AND:
        printf("AND");
        break;
    case OR:
        printf("OR");
        break;
    case XOR:
        printf("XOR");
        break;
    case SHR:
        printf("SHR");
        break;
    case SHL:
        printf("SHL");
        break;
    }
}
void print_cmp_op(IR_CMP_OP op) {
    switch (op) {
    case LT:
        printf("LT");
        break;
    case LE:
        printf("LE");
        break;
    case GT:
        printf("GT");
        break;
    case GE:
        printf("GE");
        break;
    case EQ:
        printf("EQ");
        break;
    case NEQ:
        printf("NEQ");
        break;
    }
}

void print_ir_instruction(IR_Context *ctx, const IR_Instruction *instr) {
    switch (instr->op) {
    case IR_CONST:
        printf("    r%d = CONST $%d", instr->dst, instr->iconst.value);
        break;
    case IR_BINOP:
        printf("    r%d = BINOP ", instr->dst);
        printf("r%d, r%d, ", instr->binop.lhs, instr->binop.rhs);
        print_bin_op(instr->binop.op);
        break;
    case IR_LOAD:
        printf("    r%d = LOAD r%d", instr->dst, instr->load.addr);
        break;
    case IR_STORE:
        printf("    STORE r%d -> r%d", instr->store.addr, instr->dst);
        break;
    case IR_RET:
        printf("    RET r%d", instr->ret.value);
        break;
    case IR_CALL:
        printf("    r%d = CALL %s, %d:", instr->dst, ctx->module->defs[instr->call.callee].name, instr->call.arg_count);
        printf("[ ");
        for (int i = 0; i < instr->call.arg_count; i++) {
            printf("r%d", instr->call.args[i]);
            if (i < instr->call.arg_count - 1) {
                printf(", ");
            }
        }
        printf(" ]");
        break;
    case IR_BR:
        printf("    BR %d", instr->br.label);
        break;
    case IR_BR_COND:
        printf("    BR_COND r%d, %d %d", instr->br_cond.cond, instr->br_cond.t_label, instr->br_cond.f_label);
        break;
    case IR_CMP:
        printf("    r%d = CMP ", instr->dst);
        printf("r%d, r%d, ", instr->cmp.lhs, instr->cmp.rhs);
        print_cmp_op(instr->cmp.op);
        break;
    }
    printf("\n");
}

void print_ir_block(IR_Context *ctx, const IR_Block *block) {
    for (int i = 0; i < block->count; i++) {
        print_ir_instruction(ctx, &block->instructions[i]);
    }
}

void print_ir_function(IR_Context *ctx, const IR_Function *func) {
    printf("%s:\n", func->name);
    for (int i = 0; i < func->block_count; i++) {
        printf("L%d:\n", i);
        print_ir_block(ctx, &func->blocks[i]);
    }
}

void print_ir_module(IR_Context *ctx, const IR_Module *module) {
    for (int i = 0; i < module->func_count; i++) {
        print_ir_function(ctx, module->functions[i]);
    }
}
