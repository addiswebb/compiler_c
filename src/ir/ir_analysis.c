#include "compiler_c/ir/ir_analysis.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"

#include <assert.h>
#include <stdlib.h>

void bitset_init(BitSet *s, int reg_count) {
    s->num_bits = reg_count;
    s->capacity = (s->num_bits + 31) / 32;
    s->data = calloc(s->capacity, sizeof(int));
    if (!s->data) {
        printf("Failed to alloc for bitset init\n");
        exit(1);
    }
}
void bitset_clear(BitSet *s) {
    for (int i = 0; i < s->capacity; i++) {
        s->data[i] = 0;
    }
}
void bitset_expand(BitSet *s) {
    s->num_bits *= 2;
    s->capacity = (s->num_bits + 31) / 32;
    int *new_data = realloc(s->data, sizeof(int) * s->capacity);
    if (!new_data) {
        printf("Failed to realloc for bitset expand: %d\n", s->capacity);
        free(s->data);
        exit(1);
    }
    s->data = new_data;
}
void bitset_add(BitSet *s, int reg) {
    if (reg >= s->num_bits) {
        bitset_expand(s);
    }
    int word = reg / 32;
    int offset = reg % 32;
    s->data[word] |= (1 << offset);
}
void bitset_remove(BitSet *s, int reg) {
    int word = reg / 32;
    int offset = reg % 32;
    s->data[word] &= ~(1 << offset);
}
int bitset_has(BitSet *s, int reg) {
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

void add_successor(IR_Function *func, int from_idx, int to_idx) {
    IR_Block *from = &func->blocks[from_idx];
    IR_Block *to = &func->blocks[to_idx];

    // Check if it already exists
    if (!contains(from->cfg.succ, from->cfg.succ_count, to_idx)) {
        from->cfg.succ = realloc(from->cfg.succ, sizeof(int) * (from->cfg.succ_count + 1));
        from->cfg.succ[from->cfg.succ_count++] = to_idx;
    }

    if (!contains(to->cfg.pred, to->cfg.pred_count, from_idx)) {
        to->cfg.pred = realloc(to->cfg.pred, sizeof(int) * (to->cfg.pred_count + 1));
        to->cfg.pred[to->cfg.pred_count++] = from_idx;
    }
}

void dfs_postorder(IR_Function *func, int block_id, bool *visited, int *postorder, int *count) {
    if (visited[block_id]) return;
    visited[block_id] = true;
    IR_Block *b = &func->blocks[block_id];
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

void bitset_add_defined(BitSet *defined, IR_Value *v) {
    if (v->kind == IR_REG) {
        bitset_add(defined, v->reg);
    } else {
        print_ir_value(v);
        printf(" is not IR_REG\n");
    }
}
void bitset_add_used(BitSet *defined, BitSet *used, IR_Value *v) {
    if (v->kind == IR_REG) {
        if (!bitset_has(defined, v->reg)) {
            bitset_add(used, v->reg);
        }
    } else {
        print_ir_value(v);
        printf(" is not IR_REG");
    }
}

void ir_init_func_cfg(IR_Function *f) {
    for (int j = 0; j < f->block_count; j++) {
        IR_Block *b = &f->blocks[j];
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
        IR_Block *b = &f->blocks[j];

        if (b->count == 0) continue;
        IR_Instruction *end_instr = &b->instructions[b->count - 1];
        switch (end_instr->op) {
        case IR_BR:
            add_successor(f, j, end_instr->br.label);
            break;
        case IR_BR_COND:
            add_successor(f, j, end_instr->br_cond.f_label);
            add_successor(f, j, end_instr->br_cond.t_label);
            break;
        case IR_RET:
            break;
        default:
            break;
        }
    }
}

int ir_reg_bitset(IR_Function *f) {
    int defined = 0;
    for (int j = 0; j < f->block_count; j++) {
        IR_Block *b = &f->blocks[j];
        for (int k = 0; k < b->count; k++) {
            IR_Instruction *instr = &b->instructions[k];
            switch (instr->op) {
            case IR_STORE:
                bitset_add_used(&b->live.def, &b->live.use, &instr->store.src);
                break;
            case IR_UNOP:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                bitset_add_used(&b->live.def, &b->live.use, &instr->unary.expr);
                break;
            case IR_BINOP:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                bitset_add_used(&b->live.def, &b->live.use, &instr->binop.lhs);
                bitset_add_used(&b->live.def, &b->live.use, &instr->binop.rhs);
                break;
            case IR_RET:
                bitset_add_used(&b->live.def, &b->live.use, &instr->ret.value);
                break;
            case IR_CMP:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                bitset_add_used(&b->live.def, &b->live.use, &instr->cmp.lhs);
                bitset_add_used(&b->live.def, &b->live.use, &instr->cmp.rhs);
                break;
            case IR_CAST:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                bitset_add_used(&b->live.def, &b->live.use, &instr->cast.src);
                break;
            case IR_ADDR:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                bitset_add_used(&b->live.def, &b->live.use, &instr->addr.src);
                break;
            case IR_MEMCPY:
                bitset_add_used(&b->live.def, &b->live.use, &instr->memcpy.from_reg);
                bitset_add_used(&b->live.def, &b->live.use, &instr->memcpy.to_reg);
                break;
            case IR_LOAD:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                bitset_add_used(&b->live.def, &b->live.use, &instr->load.addr);
                break;
            case IR_BR:
                break;
            case IR_BR_COND:
                bitset_add_used(&b->live.def, &b->live.use, &instr->br_cond.cond);
                break;
            case IR_ALLOCA:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                break;
            case IR_CALL:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                break;
            case IR_CONST:
                bitset_add_defined(&b->live.def, &instr->dst);
                defined++;
                break;
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
            IR_Block *b = &f->blocks[rpo[i]];
            bitset_clear(&old_live_out);
            bitset_clear(&old_live_in);
            bitset_clear(&tmp);
            bitset_copy(&old_live_out, &b->live.live_out);
            bitset_copy(&old_live_in, &b->live.live_in);

            bitset_clear(&b->live.live_out);
            for (int j = 0; j < b->cfg.succ_count; j++)
                bitset_union(&b->live.live_out, &f->blocks[b->cfg.succ[j]].live.live_in);
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

IR_StackSlot *linear_stack_slot_allocation(Lifetime *lts, int count, int *rpo, int *stack_size, int *slot_count) {
    IR_StackSlot *slots = NULL;
    for (int i = 0; i < count; i++) {
        Lifetime *l = &lts[i];
        int found_slot = 0;
        for (int j = 0; j < *slot_count; j++) {
            IR_StackSlot *s = &slots[j];
            if (s->free_at < l->start) {
                s->free_at = l->end;
                l->stack_slot = j;
                l->stack_offset = s->offset;
                found_slot = 1;
            }
        }
        if (!found_slot) {
            slots = realloc(slots, sizeof(IR_StackSlot) * (*slot_count + 1));
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
    return slots;
}
void ir_reg_to_stack(IR_Function *f, Lifetime *lts, int reg_count, IR_StackSlot *slots, int slot_count, IR_StackSlot *mem_slots,
                     int mem_slot_count) {
    for (int i = 0; i < f->block_count; i++) {
        IR_Block *b = &f->blocks[i];
        for (int j = 0; j < b->count; j++) {
            IR_Instruction *instr = &b->instructions[j];
            IR_Value *a = NULL;
            IR_Value *b = NULL;
            switch (instr->op) {
            case IR_UNOP:
                a = &instr->unary.expr;
                break;
            case IR_BINOP:
                a = &instr->binop.lhs;
                b = &instr->binop.rhs;
                break;
            case IR_LOAD:
                a = &instr->load.addr;
                break;
            case IR_STORE:
                a = &instr->store.src;
                break;
            case IR_RET:
                a = &instr->ret.value;
                break;
            case IR_BR_COND:
                a = &instr->br_cond.cond;
                break;
            case IR_CMP:
                a = &instr->cmp.lhs;
                b = &instr->cmp.rhs;
                break;
            case IR_CAST:
                a = &instr->cast.src;
                break;
            case IR_ADDR:
                a = &instr->addr.src;
                break;
            case IR_MEMCPY:
                a = &instr->memcpy.from_reg;
                b = &instr->memcpy.to_reg;
                break;
            default:
                break;
            }

            if (a != NULL) {
                if (a->kind == IR_REG) {
                    a->kind = IR_STACK;
                    a->stack_offset = lts[a->reg].stack_offset;
                } else if (a->kind == IR_MEM) {
                    a->kind = IR_STACK;
                    a->stack_offset = mem_slots[a->i].offset;
                }
                printf(" a offset: -%d\n", a->stack_offset);
            }
            if (b != NULL) {
                if (b->kind == IR_REG) {
                    b->kind = IR_STACK;
                    b->stack_offset = lts[b->reg].stack_offset;
                } else if (b->kind == IR_MEM) {
                    b->kind = IR_STACK;
                    b->stack_offset = mem_slots[b->i].offset;
                }
                printf(" b offset: -%d\n", b->stack_offset);
            }
            if (instr->dst.kind == IR_REG) {
                instr->dst.kind = IR_STACK;
                instr->dst.stack_offset = lts[instr->dst.reg].stack_offset;
                printf(" dst offset: -%d\n", instr->dst.stack_offset);
            } else if (instr->dst.kind == IR_MEM) {
                instr->dst.kind = IR_STACK;
                instr->dst.stack_offset = mem_slots[instr->dst.i].offset;
                printf(" dst offset: -%d\n", instr->dst.stack_offset);
            }
        }
    }
}

void verify_completion(IR_Function *f) {
    for (int i = 0; i < f->block_count; i++) {
        IR_Block *b = &f->blocks[i];
        for (int j = 0; j < b->count; j++) {
            IR_Instruction *instr = &b->instructions[j];
            IR_Value *a = NULL;
            IR_Value *b = NULL;
            switch (instr->op) {
            case IR_UNOP:
                a = &instr->unary.expr;
                break;
            case IR_BINOP:
                a = &instr->binop.lhs;
                b = &instr->binop.rhs;
                break;
            case IR_LOAD:
                a = &instr->load.addr;
                break;
            case IR_STORE:
                a = &instr->store.src;
                break;
            case IR_RET:
                a = &instr->ret.value;
                break;
            case IR_BR_COND:
                a = &instr->br_cond.cond;
                break;
            case IR_CMP:
                a = &instr->cmp.lhs;
                b = &instr->cmp.rhs;
                break;
            case IR_CAST:
                a = &instr->cast.src;
                break;
            case IR_ADDR:
                a = &instr->addr.src;
                break;
            case IR_MEMCPY:
                a = &instr->memcpy.from_reg;
                b = &instr->memcpy.to_reg;
                break;
            default:
                break;
            }
            if (a != NULL) {
                if (a->kind != IR_STACK) {
                    print_ir_value(a);
                    printf(" was not converted to stack offset\n");
                    exit(1);
                }
                a->stack_offset = -(a->stack_offset + 8);
            }
            if (b != NULL) {
                if (b->kind != IR_STACK) {
                    print_ir_value(b);
                    printf(" was not converted to stack offset\n");
                    exit(1);
                }
                b->stack_offset = -(b->stack_offset + 8);
            }
            if (instr->dst.kind != IR_STACK) {
                print_ir_value(b);
                printf(" was not converted to stack offset\n");
                exit(1);
            }
            instr->dst.stack_offset = -(instr->dst.stack_offset + 8);
        }
    }
}

void ir_analysis(IR_Context *ctx) {
    for (int i = 0; i < ctx->module->func_count; i++) {
        IR_Function *f = ctx->module->functions[i];
        // Build cfg
        ir_init_func_cfg(f);
        ir_compute_func_io(f);
        int reg_count = ir_reg_bitset(f);

        int *rpo = malloc(f->block_count * sizeof(int));
        compute_reverse_postorder(f, rpo);
        compute_bitset(f, rpo);

        Lifetime *lifetimes = compute_lifetimes(ctx, f, reg_count, rpo);
        qsort(lifetimes, reg_count, sizeof(Lifetime), cmp);
        int frame_size = 0;
        int slot_count = 0;
        IR_StackSlot *mem_slots = malloc(sizeof(IR_StackSlot) * f->local_count);
        int mem_slot_count;
        if (!mem_slots) {
            printf("Failed to allocate memslots\n");
            exit(1);
        }
        for (int j = 0; j < f->local_count; j++) {
            Type *t = f->locals[j].type;
            int k = f->locals[j].reg.i;
            mem_slots[k].size = align(t->size, 8);
            mem_slots[k].align = t->align;
            mem_slots[k].id = slot_count;
            mem_slots[k].offset = frame_size;
            mem_slots[k].free_at = -1;
            frame_size += mem_slots[k].size;
        }
        IR_StackSlot *slots = linear_stack_slot_allocation(lifetimes, reg_count, rpo, &frame_size, &slot_count);

        ir_reg_to_stack(f, lifetimes, reg_count, slots, slot_count, mem_slots, mem_slot_count);

        verify_completion(f);
        f->stack_size = frame_size;

        free(rpo);
        free(lifetimes);
    }
}

static inline int assign_if_ir_reg(IR_Value *v, int reg) { return v->kind == IR_REG ? reg : -1; }

void handle_uses(IR_Instruction *instr, Lifetime *lts, int pc) {
    IR_Value *reg_a = NULL;
    IR_Value *reg_b = NULL;
    switch (instr->op) {
    case IR_STORE:
        reg_a = &instr->store.src;
        break;
    case IR_UNOP:
        reg_a = &instr->unary.expr;
        break;
    case IR_BINOP:
        reg_a = &instr->binop.lhs;
        reg_b = &instr->binop.rhs;
        break;
    case IR_RET:
        reg_a = &instr->ret.value;
        break;
    case IR_CMP:
        reg_a = &instr->cmp.lhs;
        reg_b = &instr->cmp.rhs;
        break;
    case IR_CAST:
        reg_a = &instr->cast.src;
        break;
    case IR_ADDR:
        reg_a = &instr->addr.src;
        break;
    case IR_MEMCPY:
        reg_a = &instr->memcpy.from_reg;
        reg_b = &instr->memcpy.to_reg;
        break;
    case IR_LOAD:
        reg_a = &instr->load.addr;
        break;
    case IR_BR_COND:
        reg_a = &instr->br_cond.cond;
        break;
    default:
        break;
    }
    if (reg_a != NULL && reg_a->kind == IR_REG) {
        if (lts[reg_a->reg].end < pc) {
            lts[reg_a->reg].end = pc;
        }
    }

    if (reg_b != NULL && reg_b->kind == IR_REG) {
        if (lts[reg_b->reg].end < pc) {
            lts[reg_b->reg].end = pc;
        }
    }
}

Lifetime *compute_lifetimes(IR_Context *ctx, IR_Function *f, int defined, int *rpo) {
    Lifetime *lts = malloc(sizeof(Lifetime) * defined);
    int count = 0;
    int pc = 0;

    for (int i = 0; i < f->block_count; i++) {
        IR_Block *b = &f->blocks[rpo[i]];
        for (int j = 0; j < b->count; j++) {
            IR_Instruction *instr = &b->instructions[j];
            // handle defines
            switch (instr->op) {
            case IR_UNOP:
            case IR_BINOP:
            case IR_CMP:
            case IR_CAST:
            case IR_ADDR:
            case IR_ALLOCA:
            case IR_CALL:
            case IR_CONST:
                if (instr->dst.kind != IR_REG) {
                    printf("Defining a non ir_reg irvalue somehow!\n");
                    exit(1);
                }
                lts[instr->dst.reg] = (Lifetime){instr->dst.reg, pc, -1, 0, 0, &instr->dst};
                break;
            default:
                break;
            }

            handle_uses(instr, lts, pc);
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
        IR_Block *b = &func->blocks[i];
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
