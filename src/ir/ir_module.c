/*
    Begin an IR Scope,
    Tracks any variables added afterwards, and pops them from the IR virtual stack when `ir_end_scope()` is called.
*/
#include "compiler_c/ir/ir_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
IR_OpInfo op_info[] = {
    [IR_CONST] = {.def_mask = 0b001, .use_mask = 0b000},  [IR_LOAD] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_STORE] = {.def_mask = 0b001, .use_mask = 0b010},  [IR_RET] = {.def_mask = 0b000, .use_mask = 0b001},
    [IR_BR] = {.def_mask = 0b000, .use_mask = 0b000},     [IR_BR_COND] = {.def_mask = 0b000, .use_mask = 0b001},
    [IR_CMP] = {.def_mask = 0b001, .use_mask = 0b110},    [IR_CAST] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_ADDR] = {.def_mask = 0b001, .use_mask = 0b010},   [IR_ALLOCA] = {.def_mask = 0b001, .use_mask = 0b000},
    [IR_MEMCPY] = {.def_mask = 0b000, .use_mask = 0b010}, [IR_BINOP] = {.def_mask = 0b001, .use_mask = 0b110},
    [IR_UNOP] = {.def_mask = 0b001, .use_mask = 0b010},
    // IR_CALL handled seperately
};
IR_Value ir_reg_value(int reg) { return (IR_Value){IR_REG, reg, 1}; }
IR_Value ir_literal_value(int i) { return (IR_Value){IR_LITERAL, i, 1}; }

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
    int *var_indices = malloc(sizeof(int) * 4);
    if (!var_indices) {
        printf("Failed to allocate for scope var indices\n");
        exit(1);
    }
    func->scopes[func->scope_count++] = (IR_Scope){0, 0, func->stack_size, 4, var_indices};
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
        func->stack_size = func->scopes[func->scope_count].stack_pointer;
        // func->local_count -= func->scopes[func->scope_count].var_count;
    }
}

IR_Value ir_next_virtual_slot(IR_Function *func, int size, int align) {
    func->scopes[func->scope_count - 1].reg_count++;
    IR_Value v;
    v.kind = IR_MEM;
    v.size = size;
    v.align = align;
    v.i = func->local_count;
    return v;
}
// Returns the IR_Value of the next virtual register (8,8 stack slot);
IR_Value ir_next_virtual_reg(IR_Function *func) {
    func->scopes[func->scope_count - 1].reg_count++;
    IR_Value v;
    v.kind = IR_REG;
    v.size = 8;
    v.align = 8;
    v.reg = func->next_reg++;
    return v;
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
    module->const_pool.capacity = 4;
    module->const_pool.count = 0;
    module->const_pool.consts = malloc(sizeof(IR_Const) * module->const_pool.capacity);
    if (!module->const_pool.consts) {
        printf("Failed to allocate IR module const pool\n");
        free(module->defs);
        free(module->functions);
        free(module->const_pool.consts);
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
    func->stack_size = 0;
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

    func->stack_slot_capacity = 4;
    func->stack_slot_count = 0;
    func->stack_slots = malloc(sizeof(IR_StackSlot) * func->stack_slot_capacity);
    if (!func->stack_slots) {
        printf("Failed to allocated IR_Stack_Objects\n");
        free(func->blocks);
        free(func->locals);
        free(func->stack_slots);
        free(func);
        exit(1);
    }

    ctx->func = func;
    ir_add_block(ctx);
    return func;
}

int ir_add_block(IR_Context *ctx) { return ir_append_block(ctx, ir_new_block()); }

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

IR_Value ir_append_const(IR_Module *module, IR_Const *new_const) {
    if (module->const_pool.count >= module->const_pool.capacity) {
        module->const_pool.capacity *= 2;
        IR_Const *new_consts = realloc(module->const_pool.consts, sizeof(IR_Const) * module->const_pool.capacity);
        if (!new_consts) {
            printf("Failed to reallocate for new ir_consts\n");
            exit(1);
        }
        module->const_pool.consts = new_consts;
    }
    module->const_pool.consts[module->const_pool.count] = *new_const;
    IR_Const *c = &module->const_pool.consts[module->const_pool.count];
    return ir_literal_value(module->const_pool.count++);
}

IR_Value ir_new_var(IR_Function *func, const char *name, Type *type) {
    if (func->local_count >= func->local_capacity) {
        func->local_capacity *= 2;
        IR_Var *new_locals = realloc(func->locals, sizeof(IR_Var) * func->local_capacity);
        if (!new_locals) {
            printf("Failed to allocated for new local variable");
            exit(1);
        }
        func->locals = new_locals;
    }
    const IR_Value next_var = ir_next_virtual_slot(func, align(type->size, 8), 8);
    func->locals[func->local_count++] = (IR_Var){name, next_var, type};
    if (func->scope_count > 0) {
        IR_Scope *scope = &func->scopes[func->scope_count - 1];
        if (scope->var_count >= scope->var_capacity) {
            scope->var_capacity *= 2;
            int *new_var_indices = realloc(scope->var_indices, sizeof(int) * scope->var_capacity);
            if (!new_var_indices) {
                printf("Failed to realloc for new scope var indices\n");
                exit(1);
            }
            scope->var_indices = new_var_indices;
        }
        scope->var_indices[scope->var_count++] = func->local_count - 1;
    } else {
        printf("cooked");
        exit(1);
    }
    return next_var;
}

int ir_get_func_def(const IR_Context *ctx, const char *name) {
    for (int i = 0; i < ctx->module->func_count; i++) {
        if (strcmp(ctx->module->defs[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
IR_Value ir_get_var_reg(const IR_Context *ctx, const char *name) {
    IR_Function *func = ctx->func;
    for (int i = func->scope_count - 1; i >= 0; i--) {
        for (int j = 0; j < func->scopes[i].var_count; j++) {
            int k = func->scopes[i].var_indices[j];
            if (strcmp(func->locals[k].name, name) == 0) {
                return func->locals[k].reg;
            }
        }
    }

    printf("Undefined local variable \'%s\' \n", name);
    exit(1);
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

void compute_stack_lifetimes(IR_Context *ctx) {}
