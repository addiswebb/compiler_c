#include "compiler_c/ir/ir_module.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

IR_OpInfo op_info[] = {
    [IR_CONST] = {.def_mask = 0b001, .use_mask = 0b000},
    [IR_LOAD] = {.def_mask = 0b001, .use_mask = 0b010},
    [IR_STORE] = {.def_mask = 0b000, .use_mask = 0b011},
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
    [IR_PARAM] = {.def_mask = 0b001, .use_mask = 0b000},
    // IR_CALL all given params are 'used'
    [IR_CALL] = {.def_mask = 0b001, .use_mask = 0b010},
    // ---- Builtins ----
    [IR_BUILTIN_VA_START] = {.def_mask = 0b000, .use_mask = 0b011},
    [IR_BUILTIN_VA_ARG] = {.def_mask = 0b001, .use_mask = 0b010},
};
const IR_Value ir_no_value = (IR_Value){IR_UNDEFINED, 0, 0, 0};

IR_Context ir_init_ctx(Parser *p) {
    IR_Context ctx = {0};
    ctx.symbol_table = &p->symbols_arena;
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

// Returns the IR_Value of the next virtual register (8,8 stack slot);
IR_Value ir_next_virtual_reg(IR_Function *func) {
    get_current_scope(func)->reg_count++;
    IR_Value v;
    v.kind = IR_VREG;
    v.size = 8;
    v.align = 8;
    v.vreg = func->next_reg++;
    return v;
}

IR_Value ir_integer_literal(int64_t i) { return (IR_Value){.kind = IR_INT_LITERAL, .int_literal = i}; }

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
    array_init(&func->locals_array, 4, sizeof(Symbol *));

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

void ir_append_global(IR_Module *module, Symbol *symbol, const IR_Literal *literal) {
    append(&module->global_array, &(IR_Global){
                                      .symbol = symbol,
                                      .val = literal ? *literal : (IR_Literal){.type = type_invalid, .i = 0},
                                  });
}
int ir_append_literal(IR_Module *module, const IR_Literal *literal) {
    append(&module->const_array, literal);
    return module->const_array.count - 1;
}

IR_Value ir_symbol_value(Symbol *s) {
    ASSERT(s->name, "IR Symbol Value must be named.\n");
    return (IR_Value){.kind = IR_SYMBOL, .symbol = s};
}

void ir_append_function(const IR_Context *ctx, IR_Function *func) { append(&ctx->module->functions_array, &func); }

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
    array_free(&module->const_array);
    array_free(&module->global_array);
    array_free(&module->labeled_block_array);
    free(module);
}

void ir_append_instruction(IR_Block *b, IR_Instruction *instr) {
    if (DEBUG_IR_INSTRUCTIONS) print_ir_instruction(NULL, instr);

    append(&b->instruction_array, instr);
}
