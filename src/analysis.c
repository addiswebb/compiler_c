#include "compiler_c/analysis.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/type.h"

#include <assert.h>
#include <stdlib.h>

const GP_Reg win64_caller_saved[7] = {RAX, RCX, RDX, R8, R9, R10, R11};
const GP_Reg win64_callee_saved[8] = {RBX, RBP, RDI, RSI, R12, R13, R14, R15};
const GP_Reg win64_int_param_regs[WIN64_PARAM_REGISTERS] = {RCX, RDX, R8, R9};
const XMM_Reg win64_float_param_regs[8] = {
    XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7,
};

const char *gp_register_str[16][4] = {
    [RAX] = {"%al", "%ax", "%eax", "%rax"},      [RBX] = {"%bl", "%bx", "%ebx", "%rbx"},      [RCX] = {"%cl", "%cx", "%ecx", "%rcx"},
    [RDX] = {"%dl", "%dx", "%edx", "%rdx"},      [RSI] = {"%sil", "%si", "%esi", "%rsi"},     [RDI] = {"%dil", "%di", "%edi", "%rdi"},
    [RBP] = {"%bpl", "%bp", "%ebp", "%rbp"},     [RSP] = {"%spl", "%sp", "%esp", "%rsp"},     [R8] = {"%r8b", "%r8w", "%r8d", "%r8"},
    [R9] = {"%r9b", "%r9w", "%r9d", "%r9"},      [R10] = {"%r10b", "%r10w", "%r10d", "%r10"}, [R11] = {"%r11b", "%r11w", "%r11d", "%r11"},
    [R12] = {"%r12b", "%r12w", "%r12d", "%r12"}, [R13] = {"%r13b", "%r13w", "%r13d", "%r13"}, [R14] = {"%r14b", "%r14w", "%r14d", "%r14"},
    [R15] = {"%r15b", "%r15w", "%r15d", "%r15"},
};

const char *xmm_register_str[16] = {
    [XMM0] = "%xmm0",   [XMM1] = "%xmm1",   [XMM2] = "%xmm2",   [XMM3] = "%xmm3",   [XMM4] = "%xmm4",   [XMM5] = "%xmm5",
    [XMM6] = "%xmm6",   [XMM7] = "%xmm7",   [XMM8] = "%xmm8",   [XMM9] = "%xmm9",   [XMM10] = "%xmm10", [XMM11] = "%xmm11",
    [XMM12] = "%xmm12", [XMM13] = "%xmm13", [XMM14] = "%xmm14", [XMM15] = "%xmm15",
};

void bitset_init(BitSet *s, int reg_count) {
    s->num_bits = reg_count;
    s->capacity = (s->num_bits + 31) / 32;
    if (reg_count) {
        s->data = calloc(s->capacity, sizeof(int));
        if (!s->data) {
            printf("Failed to alloc for bitset init\n");
            exit(1);
        }
    } else {
        s->data = NULL;
    }
}
void bitset_clear(BitSet *s) {
    for (int i = 0; i < s->capacity; i++) {
        s->data[i] = 0;
    }
}
void bitset_add(BitSet *s, int reg) {
    if (reg >= s->num_bits) {
        printf("%d is too large a register for this bitset\n", reg);
        exit(1);
    }
    assert(reg >= 0);
    int word = reg / 32;
    int offset = reg % 32;
    s->data[word] |= (1 << offset);
}
void bitset_remove(BitSet *s, int reg) {
    assert(reg >= 0);
    int word = reg / 32;
    int offset = reg % 32;
    s->data[word] &= ~(1 << offset);
}
int bitset_has(BitSet *s, int reg) {
    assert(reg >= 0);
    int word = reg / 32;
    int offset = reg % 32;
    return (s->data[word] & (1 << offset)) != 0;
}
void bitset_union(BitSet *dst, BitSet *src) {
    assert(dst->capacity == src->capacity);
    for (int i = 0; i < dst->capacity; i++)
        dst->data[i] |= src->data[i];
}
void bitset_intersect(BitSet *dst, BitSet *src) {
    assert(dst->capacity == src->capacity);
    for (int i = 0; i < dst->capacity; i++)
        dst->data[i] &= src->data[i];
}

void bitset_difference(BitSet *dst, BitSet *src) {
    assert(dst->capacity == src->capacity);
    for (int i = 0; i < dst->capacity; i++)
        dst->data[i] &= ~src->data[i];
}
void bitset_copy(BitSet *dst, BitSet *src) {
    assert(dst->capacity == src->capacity);
    for (int i = 0; i < dst->capacity; i++)
        dst->data[i] = src->data[i];
}
int bitset_equal(BitSet *a, BitSet *b) {
    assert(a->capacity == b->capacity);
    for (int i = 0; i < a->capacity; i++) {
        if (a->data[i] != b->data[i]) return false;
    }
    return true;
}
static int contains(int *arr, int count, int val) {
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

void dfs_postorder(IR_Function *func, int block_id, bool *visited, int *postorder, int *count) {
    if (visited[block_id]) return;
    visited[block_id] = true;
    IR_Block *b = func->blocks[block_id];
    for (int i = 0; i < b->cfg.succ_count; i++) {
        dfs_postorder(func, b->cfg.succ[i], visited, postorder, count);
    }
    postorder[(*count)++] = block_id;
}

void compute_reverse_postorder(IR_Function *func, int *rpo) {
    bool *visited = calloc(func->block_count, sizeof(bool));
    int *postorder = malloc(func->block_count * sizeof(int));
    int count = 0;
    dfs_postorder(func, 0, visited, postorder, &count);

    for (int i = 0; i < count; i++) {
        rpo[i] = postorder[count - 1 - i];
    }
}

int bitset_add_defined(BitSet *defined, IR_Value *v) {
    if (v->kind == IR_VREG) {
        if (!bitset_has(defined, v->reg)) {
            bitset_add(defined, v->reg);
            return 1;
        }
    }
    return 0;
}
void bitset_add_used(BitSet *defined, BitSet *used, IR_Value *v) {
    if (v->kind == IR_VREG) {
        if (!bitset_has(defined, v->reg)) {
            bitset_add(used, v->reg);
        }
    }
}

void ir_init_func_cfg(IR_Function *f) {
    for (int j = 0; j < f->block_count; j++) {
        IR_Block *b = f->blocks[j];

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
    for (int j = 0; j < f->block_count; j++) {
        IR_Block *b = f->blocks[j];

        if (b->count == 0) continue;
        bool found = false;
        for (int i = 0; i < b->count; i++) {
            // IR_Instruction *end_instr = &b->instructions[b->count - 1];
            IR_Instruction *instr = &b->instructions[i];
            switch (instr->op) {
            case IR_BR:
                add_successor(f, b, instr->br.block);
                found = true;
                break;
            case IR_BR_COND:
                if (instr->br_cond.f_block) add_successor(f, b, instr->br_cond.f_block);
                if (instr->br_cond.t_block) add_successor(f, b, instr->br_cond.t_block);
                found = true;
                break;
            case IR_RET:
            default:
                break;
            }
        }
        if (!found) {
            if (j < f->block_count - 1) {
                // TODO: give this more thought, can it fail...?
                add_successor(f, b, f->blocks[j + 1]);
            }
        }
    }
}

int reg_bitset(IR_Function *f) {
    int defined = 0;
    for (int j = 0; j < f->block_count; j++) {
        IR_Block *b = f->blocks[j];
        for (int k = 0; k < b->count; k++) {
            IR_Instruction *instr = &b->instructions[k];
            for (int i = 0; i < instr->op_count; i++) {
                if (instr->ops[i].reg < 0) continue;
                if (instr->ops[i].kind == IR_LITERAL || instr->ops[i].kind == IR_MEM) continue;
                if (op_info[instr->op].def_mask & (1 << i)) {
                    // TODO: consider sideeffects of defined not overwriting. (should be alr...)
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

void compute_bitset(IR_Function *f, int *rpo) {
    BitSet old_live_out;
    BitSet old_live_in;
    BitSet tmp;
    bitset_init(&old_live_out, f->max_reg);
    bitset_init(&old_live_in, f->max_reg);
    bitset_init(&tmp, f->max_reg);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < f->block_count; i++) {
            IR_Block *b = f->blocks[rpo[i]];
            bitset_clear(&old_live_out);
            bitset_clear(&old_live_in);
            bitset_clear(&tmp);
            bitset_copy(&old_live_out, &b->live.live_out);
            bitset_copy(&old_live_in, &b->live.live_in);

            bitset_clear(&b->live.live_out);
            for (int j = 0; j < b->cfg.succ_count; j++)
                bitset_union(&b->live.live_out, &f->blocks[b->cfg.succ[j]]->live.live_in);
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
int cmp(const void *a, const void *b) { return ((Lifetime *)a)->start - ((Lifetime *)b)->start; }

void linear_stack_slot_allocation(Lifetime *lts, int count, int *rpo, int *stack_size, int *slot_count) {
    StackSlot *slots = NULL;
    for (int i = 0; i < count; i++) {
        Lifetime *l = &lts[i];
        int found_slot = 0;
        for (int j = 0; j < *slot_count; j++) {
            StackSlot *s = &slots[j];
            if (s->free_at <= l->start) {
                s->free_at = l->end;
                l->stack_slot = j;
                l->stack_offset = s->offset;
                found_slot = 1;
                break;
            }
        }
        if (!found_slot) {
            StackSlot *new_slots = realloc(slots, sizeof(StackSlot) * (*slot_count + 1));
            if (!new_slots) {
                printf("Failed to realloc new_slots\n");
                exit(1);
            }
            slots = new_slots;
            slots[*slot_count].size = l->v->size;
            slots[*slot_count].align = l->v->align;
            slots[*slot_count].free_at = l->end;
            slots[*slot_count].id = *slot_count;
            slots[*slot_count].offset = *stack_size;
            l->stack_offset = *stack_size;
            *stack_size += l->v->size;
            l->stack_slot = (*slot_count)++;
        }
    }
    if (slots) free(slots);
}
RegSize reg_size(int size) {
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
        printf("Given too large a size for a register, should have already been handled tho\n");
        exit(1);
    }
}
/*
    Gets the correct register for a function parameter only currently.
*/
void physical_register(IR_Value *v) {
    int reg_index = v->reg;
    if (v->reg < 0) reg_index = -v->reg - 1;
    v->kind = IR_PHYS_REG;
    // if (v->type == T_INT || v->type == T_POINTER) {
    v->phys_reg.kind = REG_GP;
    v->phys_reg.gp_reg = win64_int_param_regs[reg_index];
    // } else if (v->type == T_FLOAT) {
    //     v->phys_reg.kind = REG_XMM;
    //     v->phys_reg.xmm_reg = sysv_float_param_regs[reg_index];
    // } else {
    //     printf("Cannot use physical register with non int/float/pointer type");
    //     exit(1);
    // }
    v->phys_reg.size = reg_size(v->size);
}
void param_offset(IR_Value *v) {
    v->kind = IR_STACK;
    v->stack_offset = (-v->reg - 1) * 8 + 16;
}

void stack_offset(IR_Value *v, Lifetime *lts) {
    v->kind = IR_STACK;
    v->stack_offset = -(lts[v->reg].stack_offset + 8);
}

void update_values_with_stack_offsets(IR_Function *f, Lifetime *lts, StackSlot *mem_slots) {
    for (int i = 0; i < f->block_count; i++) {
        IR_Block *b = f->blocks[i];
        for (int j = 0; j < b->count; j++) {
            IR_Instruction *instr = &b->instructions[j];
            int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                IR_Value *a = k < instr->op_count ? &instr->ops[k] : &instr->call.args[k - instr->op_count].reg;
                if (a->kind == IR_LITERAL) continue;
                // offload the following switch to a function which correctly lowers IR_VREG, IR_MEM to IR_STACK
                // leaving IR_GLOBAL, IR_LITERAL, IR_STACK
                switch (a->kind) {
                case IR_VREG:
                    // a->reg is negative for function parameters
                    if (a->reg < 0) {
                        if (-a->reg - 1 < WIN64_PARAM_REGISTERS) {
                            physical_register(a);
                        } else {
                            param_offset(a);
                        }
                    } else {
                        stack_offset(a, lts);
                    }
                    break;
                case IR_MEM:
                    a->stack_offset = -(mem_slots[a->mem].offset - a->offset);
                    a->kind = IR_STACK;
                    break;
                case IR_STACK:
                case IR_LITERAL:
                case IR_GLOBAL:
                case IR_PHYS_REG:
                    break;
                case IR_UNDEFINED:
                    printf("An undefined IR value made it to analysis!!\n");
                    exit(1);
                }
            }
        }
    }
}

void verify_completion(IR_Function *f) {
    for (int i = 0; i < f->block_count; i++) {
        IR_Block *b = f->blocks[i];
        for (int j = 0; j < b->count; j++) {
            IR_Instruction *instr = &b->instructions[j];
            int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                IR_Value *a = k < instr->op_count ? &instr->ops[k] : &instr->call.args[k - instr->op_count].reg;
                if (a->kind == IR_LITERAL && instr->op != IR_CALL) continue;
                if (a->kind == IR_GLOBAL || a->kind == IR_PHYS_REG) continue;
                if (a->kind != IR_STACK) {
                    print_ir_value(a);
                    printf(" was not converted to stack offset\n");
                    exit(1);
                }
            }
        }
    }
}

StackSlot *locals_stack_allocation(IR_Function *f, int *frame_size, int *slot_count) {
    StackSlot *mem_slots = malloc(sizeof(StackSlot) * f->local_count);
    if (!mem_slots) {
        printf("Failed to allocate memslots\n");
        exit(1);
    }
    for (int j = 0; j < f->local_count; j++) {
        Type *t = f->locals[j].type;
        int k = f->locals[j].reg.mem;
        mem_slots[k].size = align(t->size, 8);
        mem_slots[k].align = 8;
        mem_slots[k].id = *slot_count;
        *frame_size += mem_slots[k].size;
        mem_slots[k].offset = *frame_size;
        mem_slots[k].free_at = -1;
        *frame_size += mem_slots[k].size;
    }
    return mem_slots;
}

void analysis(IR_Context *ctx) {
    for (int i = 0; i < ctx->module->func_count; i++) {
        IR_Function *f = ctx->module->functions[i];
        // Initialize Control Flow Graph Variables per block
        ir_init_func_cfg(f);
        // Compute Function dependecies (successors, predecessors)
        ir_compute_func_io(f);

        Lifetime *lifetimes = NULL;
        int *rpo = malloc(f->block_count * sizeof(int));
        int reg_count = 0;
        // If any registers were used, compute their lifetimes
        if (f->max_reg > 0) {
            reg_count = reg_bitset(f);
            compute_reverse_postorder(f, rpo);
            compute_bitset(f, rpo);

            lifetimes = compute_lifetimes(ctx, f, reg_count, rpo);
            if (DEBUG_LIFETIMES) {
                for (int j = 0; j < reg_count; j++) {
                    printf("r%d = [%d -> %d]\n", lifetimes[j].reg, lifetimes[j].start, lifetimes[j].end);
                }
            }
            qsort(lifetimes, reg_count, sizeof(Lifetime), cmp);
        }

        int frame_size = 0;
        int mem_slots_count = 0;
        // Allocate local variables
        StackSlot *mem_slots = locals_stack_allocation(f, &frame_size, &mem_slots_count);

        int slot_count = 0;
        // Allocate virtual registers
        linear_stack_slot_allocation(lifetimes, reg_count, rpo, &frame_size, &slot_count);

        // Update all instances of IR_Value with the correct stack offsets
        update_values_with_stack_offsets(f, lifetimes, mem_slots);

        // Verify all IR_Values are now of IR_STACK kind,
        verify_completion(f);
        f->stack_size = frame_size;

        free(rpo);
        free(lifetimes);
        free(mem_slots);
    }
}

Lifetime *compute_lifetimes(IR_Context *ctx, IR_Function *f, int defined, int *rpo) {
    Lifetime *lts = malloc(sizeof(Lifetime) * defined);
    int count = 0;
    int pc = 0;

    for (int i = 0; i < f->block_count; i++) {
        IR_Block *b = f->blocks[rpo[i]];
        for (int j = 0; j < b->count; j++) {
            IR_Instruction *instr = &b->instructions[j];
            int value_count = instr->op == IR_CALL ? instr->op_count + instr->call.arg_count : instr->op_count;
            for (int k = 0; k < value_count; k++) {
                IR_Value *a = k < instr->op_count ? &instr->ops[k] : &instr->call.args[k - instr->op_count].reg;
                bool is_call_arg = k >= +instr->op_count;
                if (a->kind == IR_VREG) {
                    if (a->reg < 0) continue;
                    if (op_info[instr->op].def_mask & (1 << k)) {
                        lts[instr->ops[k].reg] = (Lifetime){instr->ops[k].reg, pc, -1, 0, 0, &instr->ops[k]};
                    }
                    if (is_call_arg || op_info[instr->op].use_mask & (1 << k)) {
                        if (lts[a->reg].end < pc) {
                            lts[a->reg].end = pc;
                        }
                    }
                }
            }
            pc++;
        }
    }
    return lts;
}

void print_bitset(BitSet *bs) {
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

void print_cfg(IR_Function *func) {
    printf("Func Analysis: %s\n", func->name);
    for (int i = 0; i < func->block_count; i++) {
        IR_Block *b = func->blocks[i];
        printf("L%d:\n", i);

        // Successors
        printf("  Succ: ");
        if (b->cfg.succ_count == 0) printf("None");
        for (int j = 0; j < b->cfg.succ_count; j++)
            printf("L%d ", b->cfg.succ[j]);
        printf("\n");

        // Predecessors
        printf("  Pred: ");
        if (b->cfg.pred_count == 0) printf("None");
        for (int j = 0; j < b->cfg.pred_count; j++)
            printf("L%d ", b->cfg.pred[j]);
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
