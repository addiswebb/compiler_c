#ifndef COMPILER_C_ANALYSIS_H
#define COMPILER_C_ANALYSIS_H

#include "compiler_c/ir/ir_module.h"

/* Creates PhysReg with given GP register. e.g %rax */
IR_Value ir_gp_register_value(GP_Reg reg);
/* Creates PhysReg with given GP register with offest. e.g. -8(%rax) */
IR_Value ir_gp_register_offset_value(GP_Reg reg, int offset);
/* Creates PhysReg with offset. e.g -8(%rbp) */
IR_Value ir_stack_value(int size, int align, int offset);

/* Converts the given IR_VREG to IR_STACK using precomputed stack slot and offset from Lifetimes. */
void ir_lower_vreg_value(IR_Value *v, const Lifetime *lts, int lts_count);
/* Converts the given IR_SYMBOL to IR_STACK using precomputed symbol offsets. */
void ir_lower_symbol_value(IR_Value *v, const Array *symbol_slots, const Array *symbol_map);

/*
    Constructs a control flow graph (CFG).
    Computes the liveness of every virtual register (First use, and last use).
    Allocates unique stack slots to every local variables.
    Allocates the unique virtual registers to shared and reusable stack slots.
    Finally lowers IR_Values to assembly compatible versions, e.g. IR_STACK/IR_CONST
*/
void analysis(const IR_Context *ctx);

/* Generates ABI version of every function type in the typepool. */
void gen_abi_func_types();
/* Allocates unique stack slots for every symbol in a function. */
void symbol_slot_allocation(const IR_Context *ctx, const IR_Function *f, int *frame_size, Array *symbol_slots, Array *symbol_map);
/* Get the index of the symbol in the symbol map, which corresponds to the symbols index in the symbol_slots array. */
int get_symbol_index(const Array *symbol_map, const Symbol *symbol);

// TODO: convert these from "bitset" to "block", so add_defined(IR_BLOCK*b), adds to b->defined;

/*
    Adds the given virtual register to a block's `defined` bitset.
    Returns 1 when the block did not already have the vreg, otherwise returns 0.
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
*/
void lower_ir_values_to_stack(const IR_Function *f, const Lifetime *lts, const int lts_count, const Array *symbol_slots,
                              const Array *symbol_map);
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
