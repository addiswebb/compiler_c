/*
    Begin an IR Scope,
    Tracks any variables added afterwards, and pops them from the IR virtual stack when `ir_end_scope()` is called.
*/
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/array.h"
#include "compiler_c/ir/ir_builder.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IR_OpInfo op_info[] = {
    [IR_CONST] = {.def_mask = 0b001, .use_mask = 0b000},   [IR_LOAD] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_STORE] = {.def_mask = 0b001, .use_mask = 0b010},   [IR_STORE_MEM] = {.def_mask = 0b000, .use_mask = 0b011},
    [IR_RET] = {.def_mask = 0b000, .use_mask = 0b001},     [IR_BR] = {.def_mask = 0b000, .use_mask = 0b000},
    [IR_BR_COND] = {.def_mask = 0b000, .use_mask = 0b001}, [IR_CMP] = {.def_mask = 0b001, .use_mask = 0b110},
    [IR_CAST] = {.def_mask = 0b001, .use_mask = 0b010},    [IR_ADDR] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_ALLOCA] = {.def_mask = 0b001, .use_mask = 0b000},  [IR_MEMCPY] = {.def_mask = 0b000, .use_mask = 0b011},
    [IR_BINOP] = {.def_mask = 0b001, .use_mask = 0b110},   [IR_UNOP] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_CALL] = {.def_mask = 0b001, .use_mask = 0b000} // IR_CALL uses handled separately
};
const IR_Value ir_no_value = (IR_Value){IR_UNDEFINED, 0, 0, 0};

IR_Context ir_init_ctx() {
    IR_Context ctx;
    ctx.module = NULL;
    ctx.func = NULL;
    ctx.block = NULL;
    ctx.true_block = NULL;
    ctx.false_block = NULL;
    array_init(&ctx.loop_stack_array, 4, sizeof(IR_LoopContext));
    return ctx;
}

void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block *break_block) {
    IR_Block *new_continue_block = continue_block;
    // Try retrieve current continue block, if provided with NULL (switch statement has no continue block to jump to)
    if (!continue_block) new_continue_block = ctx->loop_stack_array.count < 1 ? NULL : ir_loop_ctx(ctx)->continue_block;
    append(&ctx->loop_stack_array, &(IR_LoopContext){new_continue_block, break_block});
}

void ir_pop_loop_ctx(IR_Context *ctx) { pop(&ctx->loop_stack_array); }

IR_Value ir_mem_value(const int mem_reg, const Type *type) {
    IR_Value v;
    v.kind = IR_MEM;
    v.mem = mem_reg;
    v.offset = 0;
    v.size = type->size;
    v.align = type->align;
    return v;
}
IR_Value ir_vreg_value(const int reg, const Type *type) {
    IR_Value v;
    v.kind = IR_VREG;
    v.reg = reg;
    v.stack_offset = 0;
    v.stack_slot = 0;
    v.size = type->size;
    v.align = type->align;
    return v;
}

void ir_begin_scope(IR_Function *func) {
    int *var_indices = malloc(sizeof(int) * 4);
    if (!var_indices) {
        printf("Failed to allocate for scope var indices\n");
        exit(1);
    }
    append(&func->scopes_array, &(IR_Scope){0, 0, func->stack_size, 4, var_indices});
}

/*
    Pops variables declared within the scope from the IR virtual stack.
*/
void ir_end_scope(IR_Function *func) {
    if (func->scopes_array.count > 0) {
        if (func->next_reg >= func->max_reg) {
            func->max_reg = func->next_reg;
        }
        func->stack_size = get_current_scope(func)->stack_pointer;
        pop(&func->scopes_array);
    }
}

IR_Value ir_next_virtual_slot(const IR_Function *func, const int size, const int align) {
    get_current_scope(func)->reg_count++;
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
    get_current_scope(func)->reg_count++;
    IR_Value v;
    v.kind = IR_VREG;
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
    array_init(&module->const_array, 4, sizeof(IR_Literal));

    module->func_capacity = 4;
    module->func_count = 0;
    module->functions = malloc(sizeof(IR_Function *) * module->func_capacity);
    if (module->functions == NULL) {
        printf("Failed to allocate IR module functions\n");
        free(module);
        exit(1);
    }
    module->func_def_capacity = 4;
    module->func_def_count = 0;
    module->func_defs = malloc(sizeof(IR_Func_Def) * module->func_def_capacity);
    if (module->func_defs == NULL) {
        printf("Failed to allocate IR module function definitions\n");
        free(module->functions);
        free(module);
        exit(1);
    }
    module->global_pool.capacity = 4;
    module->global_pool.count = 0;
    module->global_pool.globals = malloc(sizeof(IR_Literal) * module->global_pool.capacity);
    if (!module->global_pool.globals) {
        printf("Failed to allocate IR module const pool\n");
        free(module->func_defs);
        free(module->functions);
        array_free(&module->const_array);
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
    array_init(&block->instruction_array, 4, sizeof(IR_Instruction));

    block->id = -1;
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

    return block;
}

IR_Function *ir_new_function(IR_Context *ctx, const char *name) {
    IR_Function *func = malloc(sizeof(*func));
    if (!func) {
        printf("Failed to allocate IR_Function\n");
        exit(1);
    }
    array_init(&func->scopes_array, 4, sizeof(IR_Scope));
    array_init(&func->blocks_array, 4, sizeof(IR_Block *));
    func->name = name;
    func->next_reg = 0;
    func->max_reg = 0;
    func->param_count = 0;
    func->stack_size = 0;

    func->local_capacity = 4;
    func->local_count = 0;
    func->locals = malloc(sizeof(IR_Var) * func->local_capacity);
    if (!func->locals) {
        printf("Failed to allocated IR_Locals\n");
        array_free(&func->blocks_array);
        free(func);
        exit(1);
    }

    func->stack_slot_capacity = 4;
    func->stack_slot_count = 0;
    func->stack_slots = malloc(sizeof(StackSlot) * func->stack_slot_capacity);
    if (!func->stack_slots) {
        printf("Failed to allocated IR_Stack_Objects\n");
        array_free(&func->blocks_array);
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
    block->id = ctx->func->blocks_array.count;
    append(&ctx->func->blocks_array, &block);
    ctx->block = get_block(ctx->func, ctx->func->blocks_array.count - 1);
    return block;
}

void ir_append_global(IR_Module *module, const char *name, Type *type, const IR_Literal *literal, const Linkage linkage,
                      const Storage storage) {
    if (module->global_pool.count >= module->global_pool.capacity) {
        module->global_pool.capacity *= 2;
        IR_Global *new_globals = realloc(module->global_pool.globals, sizeof(IR_Global) * module->global_pool.capacity);
        if (!new_globals) {
            printf("Failed to reallocate for new ir_consts\n");
            exit(1);
        }
        module->global_pool.globals = new_globals;
    }
    module->global_pool.globals[module->global_pool.count++] = (IR_Global){
        .name = name,
        .type = type,
        .val = literal ? *literal : (IR_Literal){.type = type_invalid, .i = 0},
        .linkage = linkage,
        .storage = storage,
    };
}
IR_Value ir_append_const(IR_Module *module, const IR_Literal *literal) {
    append(&module->const_array, literal);
    IR_Value v;
    v.kind = IR_LITERAL;
    v.const_index = module->const_array.count - 1;
    v.size = literal->type->size;
    v.align = literal->type->align;
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
    if (func->scopes_array.count > 0) {
        IR_Scope *scope = get_current_scope(func);
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

IR_Func_Def *ir_get_func_def(const IR_Context *ctx, const char *name) {
    for (int i = 0; i < ctx->module->func_def_count; i++) {
        if (strcmp(ctx->module->func_defs[i].name, name) == 0) {
            return &ctx->module->func_defs[i];
        }
    }
    return NULL;
}

IR_Value ir_value_from_global(IR_Global *g) {
    IR_Value v;
    v.kind = IR_GLOBAL;
    v.size = g->type->size;
    v.align = g->type->align;
    v.global = g;
    return v;
}
IR_Value ir_get_var_reg(IR_Context *ctx, const char *name) {
    const IR_Function *func = ctx->func;
    for (int i = func->scopes_array.count - 1; i >= 0; i--) {
        IR_Scope *scope = get_scope(func, i);
        for (int j = scope->var_count - 1; j >= 0; j--) {
            const int k = scope->var_indices[j];
            if (strcmp(func->locals[k].name, name) == 0) {
                if (func->locals[k].type->kind == T_STRUCT) {
                    return ir_address(ctx, func->locals[k].reg, 0);
                }
                return func->locals[k].reg;
            }
        }
    }
    for (int i = 0; i < ctx->module->global_pool.count; i++) {
        if (strcmp(ctx->module->global_pool.globals[i].name, name) == 0) return ir_value_from_global(&ctx->module->global_pool.globals[i]);
    }

    printf("Undefined local or global variable \'%s\' \n", name);
    exit(1);
}

IR_Func_Def *ir_append_func_def(const IR_Context *ctx, const char *name, const bool is_defined) {
    if (ctx->module->func_def_count >= ctx->module->func_def_capacity) {
        ctx->module->func_def_capacity *= 2;
        IR_Func_Def *new_func_defs = realloc(ctx->module->func_defs, sizeof(IR_Func_Def) * ctx->module->func_def_capacity);
        if (!new_func_defs) {
            printf("Failed to realloc func_defs\n");
            exit(1);
        }
        ctx->module->func_defs = new_func_defs;
    }
    ctx->module->func_defs[ctx->module->func_def_count] = (IR_Func_Def){.name = name, .index = -1, .is_defined = is_defined};
    ;
    return &ctx->module->func_defs[ctx->module->func_def_count++];
}
void ir_append_function(const IR_Context *ctx, IR_Func_Def *func_def, IR_Function *func) {
    if (ctx->module->func_count >= ctx->module->func_capacity) {
        ctx->module->func_capacity *= 2;
        IR_Function **new_functions = realloc(ctx->module->functions, sizeof(IR_Function *) * ctx->module->func_capacity);
        if (!new_functions) {
            printf("Failed to realloc ir functions\n");
            exit(1);
        }
        ctx->module->functions = new_functions;
    }
    func_def->index = ctx->module->func_count;
    func_def->is_defined = true;
    ctx->module->functions[ctx->module->func_count++] = func;
}

void ir_free_module(IR_Module *module) {
    for (int i = 0; i < module->func_count; i++) {
        IR_Function *func = module->functions[i];
        for (int j = 0; j < func->blocks_array.count; j++) {
            array_free(&get_block(func, j)->instruction_array);
        }
        free(func->locals);
        array_free(&func->blocks_array);
        array_free(&func->scopes_array);
        free(func);
    }
    free(module->functions);
    free(module);
}
