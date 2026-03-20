#include "compiler_c/ir/ir_module.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_builder.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IR_OpInfo op_info[] = {
    [IR_CONST] = {.def_mask = 0b001, .use_mask = 0b000},
    [IR_LOAD] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_STORE] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_STORE_MEM] = {.def_mask = 0b000, .use_mask = 0b011},
    [IR_RET] = {.def_mask = 0b000, .use_mask = 0b001},
    [IR_BR] = {.def_mask = 0b000, .use_mask = 0b000},
    [IR_BR_COND] = {.def_mask = 0b000, .use_mask = 0b001},
    [IR_CMP] = {.def_mask = 0b001, .use_mask = 0b110},
    [IR_CAST] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_ADDR] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_ALLOCA] = {.def_mask = 0b001, .use_mask = 0b000},
    [IR_MEMCPY] = {.def_mask = 0b000, .use_mask = 0b011},
    [IR_BINOP] = {.def_mask = 0b001, .use_mask = 0b110},
    [IR_UNOP] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_CALL] = {.def_mask = 0b001, .use_mask = 0b010}, // IR_CALL param uses handled separately
    [IR_BUILTIN_VA_START] = {.def_mask = 0b000, .use_mask = 0b011},
    [IR_BUILTIN_VA_ARG] = {.def_mask = 0b001, .use_mask = 0b010},
};
const IR_Value ir_no_value = (IR_Value){IR_UNDEFINED, 0, 0, 0};

IR_Context ir_init_ctx() {
    IR_Context ctx;
    ctx.module = NULL;
    ctx.func = NULL;
    ctx.block = NULL;
    ctx.true_block = NULL;
    ctx.false_block = NULL;
    ctx.func_not_address = false;
    array_init(&ctx.loop_stack_array, 4, sizeof(IR_LoopContext));
    return ctx;
}

void free_ir_ctx(IR_Context *ctx) { array_free(&ctx->loop_stack_array); }

void ir_push_loop_ctx(IR_Context *ctx, IR_Block *continue_block, IR_Block *break_block) {
    IR_Block *new_continue_block = continue_block;
    // Try retrieve current continue block, if provided with NULL (switch statement has no continue block to jump to)
    if (!continue_block) new_continue_block = ctx->loop_stack_array.count < 1 ? NULL : get_loop_ctx(ctx)->continue_block;
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
    IR_Scope s;
    s.reg_count = 0;
    s.stack_pointer = func->stack_size;
    array_init(&s.var_array, 4, sizeof(int));
    append(&func->scopes_array, &s);
}

/*
    Pops variables declared within the scope from the IR virtual stack.
*/
void ir_end_scope(IR_Function *func) {
    if (func->scopes_array.count > 0) {
        if (func->next_reg >= func->max_reg) {
            func->max_reg = func->next_reg;
        }
        IR_Scope *current_scope = get_current_scope(func);
        func->stack_size = current_scope->stack_pointer;
        array_free(&current_scope->var_array);
        pop(&func->scopes_array);
    }
}

IR_Value ir_next_virtual_slot(const IR_Function *func, const int size, const int align) {
    get_current_scope(func)->reg_count++;
    IR_Value v;
    v.kind = IR_MEM;
    v.size = size;
    v.align = align;
    v.mem = func->locals_array.count;
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
        PANIC("Failed to allocate new IR module\n");
    }
    array_init(&module->const_array, 4, sizeof(IR_Literal));
    array_init(&module->global_array, 4, sizeof(IR_Global));
    array_init(&module->func_defs_array, 4, sizeof(IR_Func_Def));
    array_init(&module->functions_array, 4, sizeof(IR_Function *));
    array_init(&module->labeled_block_array, 4, sizeof(IR_LabeledBlock));

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

IR_Function *ir_new_function(IR_Context *ctx, const char *name, Type *type) {
    IR_Function *func = malloc(sizeof(*func));
    if (!func) {
        PANIC("Failed to allocate IR_Function\n");
    }
    array_init(&func->scopes_array, 4, sizeof(IR_Scope));
    array_init(&func->blocks_array, 4, sizeof(IR_Block *));
    array_init(&func->locals_array, 4, sizeof(IR_Var));

    func->name = name;
    func->next_reg = 0;
    func->max_reg = 0;
    func->stack_size = 0;
    func->return_type = type;

    func->stack_slot_capacity = 4;
    func->stack_slot_count = 0;

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

IR_LabeledBlock *ir_append_labeled_block(IR_Context *ctx, const char *label) {
    IR_Block *block = ir_new_block();
    return (IR_LabeledBlock *)append(&ctx->module->labeled_block_array,
                                     &(IR_LabeledBlock){.label = label, .block = block, .placeholder = true});
}

IR_LabeledBlock *ir_get_labeled_block(IR_Context *ctx, const char *label) {
    for (int i = 0; i < ctx->module->labeled_block_array.count; i++) {
        IR_LabeledBlock *b = get_labeled_block(ctx->module, i);
        if (strcmp(b->label, label) == 0) {
            return b;
        }
    }
    return NULL;
}

void ir_append_global(IR_Module *module, const char *name, Type *type, const IR_Literal *literal, const Linkage linkage,
                      const Storage storage) {
    append(&module->global_array, &(IR_Global){
                                      .name = name,
                                      .type = type,
                                      .val = literal ? *literal : (IR_Literal){.type = type_invalid, .i = 0},
                                      .linkage = linkage,
                                      .storage = storage,
                                  });
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
    const IR_Value next_var = ir_next_virtual_slot(func, align(type->size, 8), 8);
    append(&func->locals_array, &(IR_Var){name, next_var, type});
    append(&get_current_scope(func)->var_array, &(int){func->locals_array.count - 1});
    return next_var;
}

IR_Func_Def *ir_get_func_def(const IR_Context *ctx, const char *name) {
    for (int i = 0; i < ctx->module->func_defs_array.count; i++) {
        IR_Func_Def *func_def = get_func_def(ctx, i);
        if (strcmp(func_def->name, name) == 0) {
            return func_def;
        }
    }
    return NULL;
}
IR_Value ir_value_from_func_def(IR_Func_Def *f) {
    IR_Value v;
    v.kind = IR_FUNCTION;
    v.size = 0;
    v.align = 0;
    v.func.name = f->name;
    v.func.index = 69;
    return v;
}

IR_Value ir_value_from_global(IR_Global *g) {
    IR_Value v;
    v.kind = IR_GLOBAL;
    v.size = g->type->size;
    v.align = g->type->align;
    v.global = g;
    return v;
}
IR_Value ir_get_symbol_value(IR_Context *ctx, const char *name, bool give_lvalue) {
    const IR_Function *func = ctx->func;
    for (int i = func->scopes_array.count - 1; i >= 0; i--) {
        IR_Scope *scope = get_scope(func, i);
        for (int j = scope->var_array.count - 1; j >= 0; j--) {
            const int k = get_var_index(scope, j);
            IR_Var *local = get_local(func, k);
            if (strcmp(local->name, name) == 0) {
                if (give_lvalue) {
                    if (local->type->kind == T_STRUCT || local->type->kind == T_UNION) {
                        return ir_address(ctx, local->reg, 0);
                    }
                }
                return local->reg;
            }
        }
    }
    for (int i = 0; i < ctx->module->global_array.count; i++) {
        IR_Global *global = get_global(ctx, i);
        if (strcmp(global->name, name) == 0) return ir_value_from_global(global);
    }
    for (int i = 0; i < ctx->module->func_defs_array.count; i++) {
        IR_Func_Def *func_def = get_func_def(ctx, i);
        if (strcmp(func_def->name, name) == 0) {
            IR_Value v = ir_value_from_func_def(func_def);
            return ctx->func_not_address ? v : ir_address(ctx, v, 0);
        }
    }

    PANIC("Undefined local or global variable '%s' \n", name);
}

IR_Func_Def *ir_append_func_def(const IR_Context *ctx, const char *name, const bool is_defined, const bool is_variadic,
                                const StorageClass storage_class) {
    return (IR_Func_Def *)append(
        &ctx->module->func_defs_array,
        &(IR_Func_Def){.name = name, .index = -1, .is_defined = is_defined, .is_variadic = is_variadic, .storage_class = storage_class});
}
void ir_append_function(const IR_Context *ctx, IR_Func_Def *func_def, IR_Function *func) {
    func_def->index = ctx->module->functions_array.count;
    func_def->is_defined = true;
    append(&ctx->module->functions_array, &func);
}

void ir_free_module(IR_Module *module) {
    for (int i = 0; i < module->functions_array.count; i++) {
        IR_Function *func = get_func(module, i);
        for (int j = 0; j < func->blocks_array.count; j++) {
            IR_Block *block = get_block(func, j);
            for (int k = 0; k < block->instruction_array.count; k++) {
                IR_Instruction *instr = get_instruction(&block->instruction_array, k);
                if (instr->op == IR_CALL) {
                    array_free(&instr->call.arg_array);
                }
            }
            free(block->cfg.pred);
            free(block->cfg.succ);
            free(block->live.live_in.data);
            free(block->live.live_out.data);
            free(block->live.def.data);
            free(block->live.use.data);
            array_free(&block->instruction_array);
            free(block);
        }
        array_free(&func->locals_array);
        array_free(&func->blocks_array);
        ASSERT(func->scopes_array.count == 0, "Should be empty\n");
        array_free(&func->scopes_array);
        func->name = NULL;
        func->return_type = NULL;
        free(func);
    }
    array_free(&module->functions_array);
    array_free(&module->func_defs_array);
    array_free(&module->const_array);
    array_free(&module->global_array);
    array_free(&module->labeled_block_array);
    free(module);
}
