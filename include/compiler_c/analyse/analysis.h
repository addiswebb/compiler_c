#ifndef COMPILER_C_ANALYSIS_H
#define COMPILER_C_ANALYSIS_H

#include "compiler_c/analyse/analysis_types.h"
#include "compiler_c/ir/ir_module.h"

#define DEBUG_LIFETIMES 0

/*
    Gets the correct register for a function parameter only currently.
*/
void physical_register(IR_Value *v);
/*
    Converts the given IR_Value to a IR_STACK value with a stack offset for a spilled function parameter.
*/
void param_offset(IR_Value *v);

/*
    Converts the given IR_Value to an IR_STACK value with a stack offset. Uses the precomputed stack slot and offset from Lifetimes.
*/
void stack_offset(IR_Value *v, const Lifetime *lts, int lts_count);

/*
    Constructs a control flow graph.
    Computes the liveness of every virtual register.
    Allocates the unique virtual register to shared and reusable stack slots.
    Lowers IR_Values to assembly compatible identifiers, stack offset or label.
*/
void analysis(const IR_Context *ctx);

// TODO: convert these from "bitset" to "block", so add_defined(IR_BLOCK*b);

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
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const StackSlot *mem_slots);
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
void linear_stack_slot_allocation(Lifetime *lts, int count, int *stack_size, int *slot_count);

ABI_TypeClass merge(ABI_TypeClass chunk_class, ABI_TypeClass field_class);

ABI_Result classify_struct(Type *type);
ABI_Result classify(Type *type);

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
