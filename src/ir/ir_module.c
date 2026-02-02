/*
    Begin an IR Scope,
    Tracks any variables added afterwards, and pops them from the IR virtual stack when `ir_end_scope()` is called.
*/
#include "compiler_c/ir/ir_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IR_OpInfo op_info[] = {
    [IR_CONST] = {.def_mask = 0b001, .use_mask = 0b000},   [IR_LOAD] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_STORE] = {.def_mask = 0b001, .use_mask = 0b010},   [IR_STORE_MEM] = {.def_mask = 0b000, .use_mask = 0b011},
    [IR_RET] = {.def_mask = 0b000, .use_mask = 0b001},     [IR_BR] = {.def_mask = 0b000, .use_mask = 0b000},
    [IR_BR_COND] = {.def_mask = 0b000, .use_mask = 0b001}, [IR_CMP] = {.def_mask = 0b001, .use_mask = 0b110},
    [IR_CAST] = {.def_mask = 0b001, .use_mask = 0b010},    [IR_ADDR] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_ALLOCA] = {.def_mask = 0b001, .use_mask = 0b000},  [IR_MEMCPY] = {.def_mask = 0b000, .use_mask = 0b010},
    [IR_BINOP] = {.def_mask = 0b001, .use_mask = 0b110},   [IR_UNOP] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_CALL] = {.def_mask = 0b001, .use_mask = 0b000} // IR_CALL uses handled seperately
};
const IR_Value ir_no_value = (IR_Value){IR_UNDEFINED, 0, 0, 0};

IR_Context ir_init_ctx() {
    IR_Context ctx;
    ctx.module = NULL;
    ctx.func = NULL;
    ctx.block = NULL;
    ctx.true_block = NULL;
    ctx.false_block = NULL;
    ctx.loop_stack.size = 0;
    ctx.loop_stack.capacity = 4;
    ctx.loop_stack.data = malloc(sizeof(IR_LoopContext) * ctx.loop_stack.capacity);
    if (!ctx.loop_stack.data) {
        printf("Failed to allocate for IR Context Loop Stack\n");
        exit(1);
    }
    return ctx;
}

void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block *break_block) {
    IR_LoopStack *s = &ctx->loop_stack;
    if (s->size >= s->capacity) {
        s->capacity *= 2;
        IR_LoopContext *new_data = realloc(s->data, sizeof(IR_LoopContext) * s->capacity);
        if (!new_data) {
            printf("Failed to reallocate for IR Context Loop Stack\n");
            free(s->data);
            exit(1);
        }
        s->data = new_data;
    }
    s->data[s->size++] = (IR_LoopContext){continue_block, break_block};
}

void ir_pop_loop_ctx(IR_Context *ctx) { ctx->loop_stack.size--; }
IR_LoopContext *ir_loop_ctx(IR_Context *ctx) { return &ctx->loop_stack.data[ctx->loop_stack.size - 1]; }

IR_Value ir_mem_value(int mem_reg, Type *type) {
    IR_Value v;
    v.kind = IR_MEM;
    v.mem = mem_reg;
    v.offset = 0;
    v.size = type->size;
    v.align = type->align;
    return v;
}
IR_Value ir_reg_value(int reg, Type *type) {
    IR_Value v;
    v.kind = IR_REG;
    v.reg = reg;
    v.stack_offset = 0;
    v.stack_slot = 0;
    v.size = type->size;
    v.align = type->align;
    return v;
}

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
    v.mem = func->local_count;
    v.offset = 0;
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

    block->capacity = 4;
    block->count = 0;
    block->id = -1;
    block->instructions = malloc(sizeof(IR_Instruction) * block->capacity);

    block->cfg.succ = NULL;
    block->cfg.succ_count = 0;

    block->cfg.pred = NULL;
    block->cfg.pred_count = 0;

    block->live.live_in.capacity = 0;
    block->live.live_in.num_bits = 0;
    block->live.live_in.data = NULL;

    block->live.live_out.capacity = 0;
    block->live.live_out.num_bits = 0;
    block->live.live_out.data = NULL;

    block->live.def.capacity = 0;
    block->live.def.num_bits = 0;
    block->live.def.data = NULL;

    block->live.use.capacity = 0;
    block->live.use.num_bits = 0;
    block->live.use.data = NULL;

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
    func->blocks = malloc(sizeof(IR_Block *) * func->block_capacity);
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

IR_Block *ir_add_block(IR_Context *ctx) { return ir_append_block(ctx, ir_new_block()); }

IR_Block *ir_append_block(IR_Context *ctx, IR_Block *block) {
    IR_Function *func = ctx->func;
    if (func->block_count >= func->block_capacity) {
        func->block_capacity *= 2;
        IR_Block **new_blocks = realloc(func->blocks, sizeof(IR_Block *) * func->block_capacity);
        if (!new_blocks) {
            printf("Failed to reallocate for new Ir block");
            exit(1);
        }
        func->blocks = new_blocks;
    }
    block->id = func->block_count;
    func->blocks[func->block_count++] = block;
    ctx->block = func->blocks[func->block_count - 1];
    return block;
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
    IR_Value v;
    v.kind = IR_LITERAL;
    v.const_index = module->const_pool.count++;
    v.size = new_const->type->size;
    v.align = new_const->type->align;
    return v;
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
        for (int j = func->scopes[i].var_count - 1; j >= 0; j--) {
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
            free(func->blocks[j]->instructions);
        }
        free(func->locals);
        free(func->scopes);
        free(func->blocks);
        free(func);
    }
    free(module->functions);
    free(module);
}

IR_Block *current_block(const IR_Function *func) { return func->blocks[func->block_count - 1]; }
