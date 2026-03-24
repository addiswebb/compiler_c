#ifndef COMPILER_C_ANALYSIS_H
#define COMPILER_C_ANALYSIS_H

#include "compiler_c/ir/ir_module.h"

#define DEBUG_LIFETIMES 0

IR_Value ir_stack_value(int size, int align, int offset);

/*
    Converts the given IR_Value to an IR_STACK value with a stack offset. Uses the precomputed stack slot and offset from Lifetimes.
*/
void ir_lower_vreg_value(IR_Value *v, const Lifetime *lts, int lts_count);

void ir_lower_symbol_value(IR_Value *v, const Array *symbol_slots, const Array *symbol_map);

void ir_lower_const_value(IR_Value *v);

/*
    Constructs a control flow graph.
    Computes the liveness of every virtual register.
    Allocates the unique virtual register to shared and reusable stack slots.
    Lowers IR_Values to assembly compatible identifiers, stack offset or label.
*/
void analysis(const IR_Context *ctx);

void symbol_slot_allocation(const IR_Function *f, int *frame_size, Array *symbol_slots, Array *symbol_map);
int get_symbol_index(const Array *symbol_map, Symbol *symbol);

// TODO: convert these from "bitset" to "block", so add_defined(IR_BLOCK*b), adds to b->defined;

/*
    Adds the given virtual register to a block's `defined` bitset.
    Returns 1 when adding a IR_VREG, and 0 after skipping a non IR_VREG IR_Value.
*/
int bitset_add_defined(const BitSet *defined, const IR_Value *v);

/*
    Adds the given virtual stack slot to a block's `used` bitset, on the condition it was not already defined within the block.
    Returns 1 when adding a IR_VREG, and 0 after skipping a non IR_VREG IR_Value.
*/
void bitset_add_used(const BitSet *defined, const BitSet *used, const IR_Value *v);

/*
    Fills the used and defined bitsets for every block in the given function.
*/
int reg_bitset(const IR_Function *f);

/*
    Handles ABI conversions like struct to chunks/hidden pointer
*/
void lower_ir_for_asm(IR_Function *f);
/*
    Converts virtual stack registers to physical offsets. (use physical soon...)
    Converts function param slots to physical registers or offsets.
    Converts virtual mem slots to physical stack slots.
    Converts ir_store for structs types to memcpy
*/
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const Array *symbol_slots, const Array *symbol_map);
/*
    Add a successor block to the given `from` block.
*/
void add_successor(IR_Function *func, IR_Block *from, IR_Block *to);

/*
    Computes the function relative line at which all virtual registers are first and last used.
*/
Lifetime *compute_lifetimes(const IR_Function *f, const int defined, const int *rpo);

/*
    Computes the order, effectively, that blocks will be executed.
    In the case where its either block a or b, both are sequential in this order.
*/
void compute_reverse_postorder(IR_Function *func, int *rpo);
/*
    Computes the live_in, live_out bitsets for a given function. The defined and used bitsets must be defined already.
*/
void compute_bitset(const IR_Function *f, const int *rpo);

int cmp_lifetime(const void *a, const void *b);
/*
    Allocates availiable stack slots to virtual registers.
    In the case where no stack slots are availible, or are unsuitable/too small, a new stack slot is added.
*/
void linear_stack_slot_allocation(Lifetime *lts, const int count, int *stack_size);


void bitset_init(BitSet *s, int reg_count);
void bitset_expand(BitSet *s);
void bitset_add(const BitSet *s, int reg);
void bitset_remove(const BitSet *s, int reg);
int bitset_has(const BitSet *s, int reg);
void bitset_union(const BitSet *dst, const BitSet *src);
void bitset_intersect(const BitSet *dst, const BitSet *src);
void bitset_difference(const BitSet *dst, const BitSet *src);
void bitset_copy(const BitSet *dst, const BitSet *src);
int bitset_equal(const BitSet *a, const BitSet *b);

void print_bitset(const BitSet *bs);
void print_cfg(const IR_Function *func);

#endif // COMPILER_C_ANALYSIS_H
