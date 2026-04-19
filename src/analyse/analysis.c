#include "compiler_c/analyse/analysis.h"
#include "compiler_c/abi/abi.h"
#include "compiler_c/analyse/analysis_types.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"

#include <stdio.h>
#include <stdlib.h>

const char *gp_register_str[16][4] = {
    [RAX] = {"%al", "%ax", "%eax", "%rax"},      [RBX] = {"%bl", "%bx", "%ebx", "%rbx"},
    [RCX] = {"%cl", "%cx", "%ecx", "%rcx"},      [RDX] = {"%res.memorydl", "%dx", "%edx", "%rdx"},
    [RSI] = {"%sil", "%si", "%esi", "%rsi"},     [RDI] = {"%dil", "%di", "%edi", "%rdi"},
    [RBP] = {"%bpl", "%bp", "%ebp", "%rbp"},     [RSP] = {"%spl", "%sp", "%esp", "%rsp"},
    [R8] = {"%r8b", "%r8w", "%r8d", "%r8"},      [R9] = {"%r9b", "%r9w", "%r9d", "%r9"},
    [R10] = {"%r10b", "%r10w", "%r10d", "%r10"}, [R11] = {"%r11b", "%r11w", "%r11d", "%r11"},
    [R12] = {"%r12b", "%r12w", "%r12d", "%r12"}, [R13] = {"%r13b", "%r13w", "%r13d", "%r13"},
    [R14] = {"%r14b", "%r14w", "%r14d", "%r14"}, [R15] = {"%r15b", "%r15w", "%r15d", "%r15"},
};

const char *sse_register_str[16] = {
    [XMM0] = "%xmm0",   [XMM1] = "%xmm1",   [XMM2] = "%xmm2",   [XMM3] = "%xmm3",   [XMM4] = "%xmm4",   [XMM5] = "%xmm5",
    [XMM6] = "%xmm6",   [XMM7] = "%xmm7",   [XMM8] = "%xmm8",   [XMM9] = "%xmm9",   [XMM10] = "%xmm10", [XMM11] = "%xmm11",
    [XMM12] = "%xmm12", [XMM13] = "%xmm13", [XMM14] = "%xmm14", [XMM15] = "%xmm15",
};

void bitset_init(BitSet *s, const int reg_count) {
    s->num_bits = reg_count;
    s->capacity = (s->num_bits + 31) / 32;
    if (reg_count) {
        s->data = calloc(s->capacity, sizeof(int));
        ASSERT(s->data, "Failed to alloc for bitset init\n");
    } else {
        s->data = NULL;
    }
}
void bitset_clear(const BitSet *s) {
    for (int i = 0; i < s->capacity; i++) {
        s->data[i] = 0;
    }
}
void bitset_add(const BitSet *s, const int reg) {
    if (reg >= s->num_bits) {
        PANIC("%d is too large a register for this bitset\n", reg);
    }
    ASSERT(reg >= 0, "Reg must be >= 0\n");
    const int word = reg / 32;
    const unsigned int offset = reg % 32;
    s->data[word] |= (1u << offset);
}
void bitset_remove(const BitSet *s, const int reg) {
    ASSERT(reg >= 0, "Reg must be >= 0\n");
    const int word = reg / 32;
    const unsigned int offset = reg % 32;
    s->data[word] &= ~(1u << offset);
}
int bitset_has(const BitSet *s, const int reg) {
    ASSERT(reg >= 0, "Reg must be >= 0\n");
    const int word = reg / 32;
    const unsigned int offset = reg % 32;
    return (s->data[word] & (1u << offset)) != 0;
}
void bitset_union(const BitSet *dst, const BitSet *src) {
    ASSERT(dst->capacity == src->capacity, "Capacities must be equal\n");
    for (int i = 0; i < dst->capacity; i++) dst->data[i] |= src->data[i];
}
void bitset_intersect(const BitSet *dst, const BitSet *src) {
    ASSERT(dst->capacity == src->capacity, "Capacities must be equal\n");
    for (int i = 0; i < dst->capacity; i++) dst->data[i] &= src->data[i];
}

void bitset_difference(const BitSet *dst, const BitSet *src) {
    ASSERT(dst->capacity == src->capacity, "Capacities must be equal\n");
    for (int i = 0; i < dst->capacity; i++) dst->data[i] &= ~src->data[i];
}
void bitset_copy(const BitSet *dst, const BitSet *src) {
    ASSERT(dst->capacity == src->capacity, "Capacities must be equal\n");
    for (int i = 0; i < dst->capacity; i++) dst->data[i] = src->data[i];
}
int bitset_equal(const BitSet *a, const BitSet *b) {
    ASSERT(a->capacity == b->capacity, "Capacities must be equal\n");
    for (int i = 0; i < a->capacity; i++) {
        if (a->data[i] != b->data[i]) return false;
    }
    return true;
}
static int contains(const int *arr, const int count, const int val) {
    for (int i = 0; i < count; i++) {
        if (arr[i] == val) return true;
    }
    return false;
}

void add_successor(IR_Function *func, IR_Block *from, IR_Block *to) {
    // Check if it already exists
    if (!contains(from->cfg.succ, from->cfg.succ_count, to->id)) {
        from->cfg.succ = realloc(from->cfg.succ, sizeof(int) * (from->cfg.succ_count + 1));
        from->cfg.succ[from->cfg.succ_count++] = to->id;
    }

    if (!contains(to->cfg.pred, to->cfg.pred_count, from->id)) {
        to->cfg.pred = realloc(to->cfg.pred, sizeof(int) * (to->cfg.pred_count + 1));
        to->cfg.pred[to->cfg.pred_count++] = from->id;
    }
}

void dfs_postorder(IR_Function *func, const int block_id, bool *visited, int *postorder, int *count) {
    if (visited[block_id]) return;
    visited[block_id] = true;
    const IR_Block *b = get_block(func, block_id);
    for (int i = 0; i < b->cfg.succ_count; i++) {
        dfs_postorder(func, b->cfg.succ[i], visited, postorder, count);
    }
    postorder[(*count)++] = block_id;
}

void compute_reverse_postorder(IR_Function *func, int *rpo) {
    bool *visited = calloc(func->blocks_array.count, sizeof(bool));
    if (!visited) {
        PANIC("Failed to calloc for visited\n");
    }
    int *postorder = malloc(func->blocks_array.count * sizeof(int));
    if (!postorder) {
        free(visited);
        PANIC("Failed to calloc for postorder\n");
    }
    int count = 0;
    dfs_postorder(func, 0, visited, postorder, &count);

    for (int i = 0; i < count; i++) {
        rpo[i] = postorder[count - 1 - i];
    }
    free(postorder);
    free(visited);
}

int bitset_add_defined(const BitSet *defined, const IR_Value *v) {
    if (v->kind == IR_VREG) {
        if (!bitset_has(defined, v->vreg)) {
            bitset_add(defined, v->vreg);
            return 1;
        }
    }
    return 0;
}
void bitset_add_used(const BitSet *defined, const BitSet *used, const IR_Value *v) {
    if (v->kind == IR_VREG) {
        if (!bitset_has(defined, v->vreg)) {
            bitset_add(used, v->vreg);
        }
    }
}

void ir_init_func_cfg(const IR_Function *f) {
    for (int j = 0; j < f->blocks_array.count; j++) {
        IR_Block *b = get_block(f, j);

        b->cfg.succ = NULL;
        b->cfg.succ_count = 0;

        b->cfg.pred = NULL;
        b->cfg.pred_count = 0;

        bitset_init(&b->live.live_in, f->max_reg);
        bitset_init(&b->live.live_out, f->max_reg);
        bitset_init(&b->live.def, f->max_reg);
        bitset_init(&b->live.use, f->max_reg);
    }
}

void ir_compute_func_io(IR_Function *f) {
    for (int j = 0; j < f->blocks_array.count; j++) {
        IR_Block *b = get_block(f, j);

        bool found = false;
        for (int i = 0; i < b->instruction_array.count; i++) {
            const IR_Instruction *instr = get_instruction(&b->instruction_array, i);
            switch (instr->op) {
            case IR_BR:
                add_successor(f, b, instr->br.block);
                found = true;
                break;
            case IR_BR_COND:
                if (instr->br_cond.f_block) add_successor(f, b, instr->br_cond.f_block);
                if (instr->br_cond.t_block) add_successor(f, b, instr->br_cond.t_block);
                found = instr->br_cond.f_block && instr->br_cond.t_block;
                break;
            case IR_RET:
            default:
                break;
            }
        }
        // Means we may still be within the block, allow fallthrough to sequentially next block.
        if (!found && j < f->blocks_array.count - 1) add_successor(f, b, get_block(f, j + 1));
    }
}

int reg_bitset(const IR_Function *f) {
    int defined = 0;
    for (int j = 0; j < f->blocks_array.count; j++) {
        const IR_Block *b = get_block(f, j);
        for (int k = 0; k < b->instruction_array.count; k++) {
            // const IR_Instruction *instr = &b->instructions[k];
            const IR_Instruction *instr = get_instruction(&b->instruction_array, k);
            for (int i = 0; i < instr->op_count; i++) {
                if (instr->ops[i].kind != IR_VREG) continue;
                if (op_info[instr->op].def_mask & (1 << i)) {
                    // TODO: consider side effects of defined not overwriting. (should be alr...)
                    if (bitset_add_defined(&b->live.def, &instr->ops[i])) defined++;
                }
                if (op_info[instr->op].use_mask & (1 << i)) {
                    bitset_add_used(&b->live.def, &b->live.use, &instr->ops[i]);
                }
            }
        }
    }
    return defined;
}

void compute_bitset(const IR_Function *f, const int *rpo) {
    BitSet old_live_out;
    BitSet old_live_in;
    BitSet tmp;
    bitset_init(&old_live_out, f->max_reg);
    bitset_init(&old_live_in, f->max_reg);
    bitset_init(&tmp, f->max_reg);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < f->blocks_array.count; i++) {
            const IR_Block *b = get_block(f, rpo[i]);
            bitset_clear(&old_live_out);
            bitset_clear(&old_live_in);
            bitset_clear(&tmp);
            bitset_copy(&old_live_out, &b->live.live_out);
            bitset_copy(&old_live_in, &b->live.live_in);

            bitset_clear(&b->live.live_out);
            for (int j = 0; j < b->cfg.succ_count; j++) bitset_union(&b->live.live_out, &get_block(f, b->cfg.succ[j])->live.live_in);
            bitset_copy(&tmp, &b->live.live_out);
            bitset_difference(&tmp, &b->live.def);
            bitset_copy(&b->live.live_in, &b->live.use);
            bitset_union(&b->live.live_in, &tmp);
            if (!bitset_equal(&b->live.live_out, &old_live_out) || !bitset_equal(&b->live.live_in, &old_live_in)) changed = true;
        }
    }

    free(old_live_out.data);
    free(old_live_in.data);
    free(tmp.data);
}
int cmp_lifetime(const void *a, const void *b) { return ((Lifetime *)a)->start - ((Lifetime *)b)->start; }

void linear_stack_slot_allocation(Lifetime *lts, const int count, int *stack_size) {
    RegisterSlot *slots = NULL;
    int slot_count = 0;

    for (int i = 0; i < count; i++) {
        Lifetime *l = &lts[i];
        int found_slot = 0;
        for (int j = 0; j < slot_count; j++) {
            RegisterSlot *s = &slots[j];
            if (s->free_at <= l->start) {
                s->free_at = l->end;
                l->stack_slot = j;
                l->stack_offset = s->v.phys_reg.offset;
                found_slot = 1;
                break;
            }
        }
        if (!found_slot) {
            RegisterSlot *new_slots = realloc(slots, sizeof(RegisterSlot) * (slot_count + 1));
            if (!new_slots) {
                PANIC("Failed to realloc new_slots\n");
            }
            slots = new_slots;
            l->stack_offset = -(*stack_size) - l->v->size;
            slots[slot_count].v = ir_stack_value(l->v->size, l->v->align, l->stack_offset);
            slots[slot_count].free_at = l->end;
            l->stack_slot = slot_count++;
            *stack_size += l->v->size;
        }
    }
    if (slots) free(slots);
}
RegSize reg_size(const int size) {
    switch (size) {
    case 1:
        return REG_8;
    case 2:
        return REG_16;
    case 4:
        return REG_32;
    case 8:
        return REG_64;
    default:
        // Todo handle size of 3,5,6,7 for chars
        PANIC("Incompatible size %d, should have already been handled tho\n", size);
    }
}

const Lifetime *get_lifetime(const Lifetime *lts, const int lts_count, int reg) {
    for (int i = 0; i < lts_count; i++) {
        if (lts[i].reg == reg) {
            return &lts[i];
        }
    }
    PANIC("Failed to find lifetime of r%d\n", reg);
}
void ir_lower_symbol_value(IR_Value *v, const Array *symbol_slots, const Array *symbol_map) {
    IR_Value old = *v;
    switch (old.symbol->kind) {
    case VAR:
        // Handle global variables
        if (old.symbol->storage == STORAGE_NONE) {
            int index = get_symbol_index(symbol_map, v->symbol);
            ASSERT(index != -1, "Tried to find symbol index of %s\n", v->symbol->name);
            v->kind = IR_PHYS_REG;
            *v = ((RegisterSlot *)get(symbol_slots, index))->v;
            break;
        }
        // !!INTENTIONAL PASSTHROUGH!!
    case FUNC:
        v->kind = IR_PHYS_REG;
        v->phys_reg.kind = REG_IP;
        v->phys_reg.size = REG_64;
        v->phys_reg.data_kind = REG_DATA_LABEL;
        v->phys_reg.label = old.symbol->name;
        break;
    case TYPEDEF:
    case ANY:
    case ENUM:
        PANIC("ir_lower_symbol_value dont know how to handle\n");
        break;
    }
}

IR_Value ir_gp_register(GP_Reg reg) {
    return (IR_Value){.kind = IR_PHYS_REG,
                      .size = 8,
                      .align = 8,
                      .phys_reg = (PhysReg){
                          .kind = REG_GP,
                          .gp_reg = reg,
                          .data_kind = REG_DATA_NONE,
                          .size = REG_64,
                      }};
}
IR_Value ir_stack_value(int size, int align, int offset) {
    return (IR_Value){.kind = IR_PHYS_REG,
                      .size = size,
                      .align = align,
                      .phys_reg = (PhysReg){
                          .kind = REG_GP,
                          .gp_reg = RBP,
                          .data_kind = REG_DATA_OFFSET,
                          .size = REG_64,
                          .offset = offset,
                      }};
}
void ir_lower_vreg_value(IR_Value *v, const Lifetime *lts, int lts_count) {
    ASSERT(lts, "LTS is null\n");
    ASSERT(v->kind == IR_VREG, "Expected VREG IR Value\n");
    *v = ir_stack_value(v->size, v->align, get_lifetime(lts, lts_count, v->vreg)->stack_offset);
}

void ir_lower_const_value(IR_Value *v) {
    return;
    IR_Value old = *v;
    v->kind = IR_PHYS_REG;
    v->phys_reg.kind = REG_IP;
    v->phys_reg.size = REG_64;
    v->phys_reg.data_kind = REG_DATA_CONST_INDEX;
    v->phys_reg.const_index = old.const_index;
}

void verify_completion(const IR_Function *f) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        const IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            const IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                const IR_Value *a = k < instr->op_count ? &instr->ops[k] : &get_call_arg(instr, k - instr->op_count)->v;
                if (a->kind == IR_CONSTANT) continue; // TMP
                if (a->kind == IR_CONSTANT && instr->op != IR_CALL) continue;
                if (a->kind == IR_PHYS_REG || a->kind == IR_INT_LITERAL) continue;
                // Allow undefined IR Values for the following:
                if (instr->op == IR_RET && instr->ret.type == type_void) continue;
                if (instr->op == IR_CALL && instr->call.type->_func.return_type == type_void) continue;
                // Otherwise throw error
                log_start(LOG_ERROR);
                print_ir_value(a);
                printf(" was not converted to stack offset\n");
                exit(1);
            }
        }
    }
}

int get_symbol_index(const Array *symbol_map, Symbol *symbol) {
    for (int i = 0; i < symbol_map->count; i++) {
        Symbol *s = *(Symbol **)get(symbol_map, i);
        if (s == symbol) return i;
    }
    return -1;
}
void symbol_slot_allocation(const IR_Context *ctx, const IR_Function *f, int *frame_size, Array *symbol_slots, Array *symbol_map) {
    int slot_count = f->locals_array.count + ctx->module->global_array.count;
    if (slot_count == 0) return;
    // TODO account parameters,

    array_init(symbol_map, slot_count, sizeof(Symbol *));
    array_init(symbol_slots, slot_count, sizeof(RegisterSlot));

    for (int i = 0; i < ctx->module->global_array.count; i++) {
        Symbol *global_symbol = get_global(ctx, i)->symbol;
        int size = align(global_symbol->type->size, 8);
        // Todo track scopes on symbols, so that we can reuse slots for symbols aswell, (instead of '-1' currently)
        int offset = -(*frame_size) - size;

        append(symbol_slots, &(RegisterSlot){.v = ir_symbol_value(global_symbol), .free_at = -1});
        append(symbol_map, &global_symbol);
        *frame_size += size;
    }

    for (int i = 0; i < f->locals_array.count; i++) {
        Symbol *local_symbol = get_local_symbol(f, i);
        int size = align(local_symbol->type->size, 8);
        // Todo track scopes on symbols, so that we can reuse slots for symbols aswell, (instead of '-1' currently)
        int offset = -(*frame_size) - size;

        append(symbol_slots, &(RegisterSlot){.v = ir_stack_value(size, 8, offset), .free_at = -1});
        append(symbol_map, &local_symbol);
        *frame_size += size;
    }
}

void generate_types() {
    int n = typepool.count;
    for (int i = 0; i < n; i++) {
        Type *t = (Type *)arena_get(&typepool, i);
        if (t->kind == T_FUNCTION) abi_func_type_gen(t);
    }
}
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const Array *symbol_slots,
                              const Array *symbol_map) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        const IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                bool is_arg_param = k >= instr->op_count;
                int instr_index = is_arg_param ? k - instr->op_count : k;
                IR_CallArg *arg = is_arg_param ? get_call_arg(instr, instr_index) : NULL;
                IR_Value *a = is_arg_param ? &arg->v : &instr->ops[instr_index];
                // Lower IR_VREG & IR_SYMBOL to IR_PHYS_REG
                switch (a->kind) {
                case IR_VREG:
                    ir_lower_vreg_value(a, lts, lts_count);
                    break;
                case IR_SYMBOL:
                    ir_lower_symbol_value(a, symbol_slots, symbol_map);
                    break;
                case IR_CONSTANT:
                    ir_lower_const_value(a);
                    break;
                case IR_PHYS_REG:
                case IR_INT_LITERAL:
                    break;
                case IR_UNDEFINED:
                    if (instr->op == IR_RET && instr->ret.type == type_void) break;
                    if (instr->op == IR_CALL && instr->call.type->_func.return_type == type_void) break;
                    PANIC("An undefined IR value made it to analysis!!\n");
                    break;
                }
            }
        }
    }
}
void lower_ir_for_asm(IR_Function *f) {
    for (int i = 0; i < f->blocks_array.count; i++) {
        IR_Block *b = get_block(f, i);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            if (instr->op == IR_RET) {
                abi_lower_ret(f, b, instr, &j);
            } else if (instr->op == IR_PARAM) {
                abi_lower_param(f, b, instr, &j);
            } else if (instr->op == IR_LOAD) {
                // Will fail if size > 8 bytes
                ASSERT(instr->load.type->size <= 8, "ir_load of type sized larger than 8 bytes\n");
                if (instr->load.type->kind == T_STRUCT) instr->load.type = get_integer_type(instr->load.type->size);
            } else if (instr->op == IR_STORE) {
                abi_lower_store(f, b, instr, &j);
            }
        }
    }
}
void analysis(const IR_Context *ctx) {
    for (int i = 0; i < ctx->module->functions_array.count; i++) {
        IR_Function *f = get_func(ctx->module, i);

        lower_ir_for_asm(f);

        if (DEBUG_LOWERED_IR) {
            printf("vvvvvvvvvvvvvvvvvvvvv\n");
            print_ir_function(ctx, f);
            printf("\n^^^^^^^^^^^^^^^^^^^^^\n");
        }

        // Initialize Control Flow Graph Variables per block
        ir_init_func_cfg(f);
        // Compute Function dependencies (successors, predecessors)
        ir_compute_func_io(f);

        Lifetime *lifetimes = NULL;
        int *rpo = malloc(f->blocks_array.count * sizeof(int));
        if (!rpo) {
            PANIC("Failed to allocate rpo\n");
        }
        int reg_count = 0;
        // If any registers were used, compute their lifetimes
        if (f->max_reg > 0) {
            reg_count = reg_bitset(f);
            compute_reverse_postorder(f, rpo);
            compute_bitset(f, rpo);

            lifetimes = compute_lifetimes(f, reg_count, rpo);
            qsort(lifetimes, reg_count, sizeof(Lifetime), cmp_lifetime);

            if (DEBUG_LIFETIMES) {
                for (int j = 0; j < reg_count; j++) {
                    printf("r%d = [%d -> %d]\n", lifetimes[j].reg, lifetimes[j].start, lifetimes[j].end);
                }
                printf("\n");
            }
        }

        Array symbol_map = {};
        Array symbol_slots = {};

        const int variadic_space = f->type->_func.is_variadic ? 176 : 0;
        int frame_size = variadic_space;

        // Allocate local variables
        symbol_slot_allocation(ctx, f, &frame_size, &symbol_slots, &symbol_map);
        // Allocate virtual registers
        linear_stack_slot_allocation(lifetimes, reg_count, &frame_size);
        // Update all instances of IR_Value with the correct stack offsets
        lower_ir_values_to_stack(f, lifetimes, reg_count, &symbol_slots, &symbol_map);

        // Verify all IR_Values are now x86 compatible (IR_PHYS_REG, IR_CONSTANT, IR_INT_LITERAL, or void return types),
        verify_completion(f);
        f->stack_size = frame_size;

        free(rpo);
        free(lifetimes);
        array_free(&symbol_map);
        array_free(&symbol_slots);
    }
}

Lifetime *compute_lifetimes(const IR_Function *f, const int defined, const int *rpo) {
    Lifetime *lts = malloc(sizeof(Lifetime) * defined);
    int pc = 0;

    for (int i = 0; i < f->blocks_array.count; i++) {
        const IR_Block *b = get_block(f, rpo[i]);
        for (int j = 0; j < b->instruction_array.count; j++) {
            IR_Instruction *instr = get_instruction(&b->instruction_array, j);
            const int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_array.count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                const IR_Value *a = k < instr->op_count ? &instr->ops[k] : &get_call_arg(instr, k - instr->op_count)->v;
                const bool is_call_arg = k >= instr->op_count;
                if (a->kind == IR_VREG) {
                    if (a->vreg < 0) continue;
                    if (op_info[instr->op].def_mask & (1 << k)) {
                        lts[instr->ops[k].vreg] = (Lifetime){instr->ops[k].vreg, pc, -1, 0, 0, .v = &instr->ops[k]};
                    }
                    if (is_call_arg || op_info[instr->op].use_mask & (1 << k)) {
                        if (lts[a->vreg].end < pc) {
                            lts[a->vreg].end = pc;
                        }
                    }
                }
            }
            pc++;
        }
    }
    return lts;
}

void print_bitset(const BitSet *bs) {
    printf("{");
    int first = 1;
    for (int i = 0; i < bs->num_bits; i++) {
        if (bitset_has(bs, i)) {
            if (!first) printf(", ");
            printf("r%d", i);
            first = 0;
        }
    }
    printf("}");
}

void print_cfg(const IR_Function *func) {
    printf("Func Analysis: %s\n", func->name);
    for (int i = 0; i < func->blocks_array.count; i++) {
        const IR_Block *b = get_block(func, i);
        printf("L%d:\n", i);

        // Successors
        printf("  Succ: ");
        if (b->cfg.succ_count == 0) printf("None");
        for (int j = 0; j < b->cfg.succ_count; j++) printf("L%d ", b->cfg.succ[j]);
        printf("\n");

        // Predecessors
        printf("  Pred: ");
        if (b->cfg.pred_count == 0) printf("None");
        for (int j = 0; j < b->cfg.pred_count; j++) printf("L%d ", b->cfg.pred[j]);
        printf("\n");

        // Defined / Used
        printf("  Def: ");
        print_bitset(&b->live.def);
        printf("\n");
        printf("  Use: ");
        print_bitset(&b->live.use);
        printf("\n");

        // Live-in / Live-out
        printf("  Live-in: ");
        print_bitset(&b->live.live_in);
        printf("\n");
        printf("  Live-out: ");
        print_bitset(&b->live.live_out);
        printf("\n");
    }
}
