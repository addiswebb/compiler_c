#ifndef COMPILER_C_ANALYSIS_H
#define COMPILER_C_ANALYSIS_H

#include "compiler_c/ir/ir_module.h"

#define DEBUG_LIFETIMES 0

void physical_gp_register(IR_Value *v, GP_Reg reg, int size);
void physical_xmm_register(IR_Value *v, XMM_Reg reg, int size);
void param_offset(IR_Value *v);
void stack_offset(IR_Value *v, const Lifetime *lts);

void analysis(const IR_Context *ctx);
int bitset_add_defined(const BitSet *defined, const IR_Value *v);
void bitset_add_used(const BitSet *defined, const BitSet *used, const IR_Value *v);

int reg_bitset(const IR_Function *f);
void lower_for_asm_gen(const IR_Function *f, const Lifetime *lts, const StackSlot *mem_slots);
void add_successor(IR_Function *func, IR_Block *from, IR_Block *to);

Lifetime *compute_lifetimes(const IR_Function *f, int defined, const int *rpo);
void compute_reverse_postorder(IR_Function *func, int *rpo);
void compute_bitset(const IR_Function *f, const int *rpo);

int cmp(const void *a, const void *b);
void linear_stack_slot_allocation(Lifetime *lts, int count, int *stack_size, int *slot_count);

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

#endif
