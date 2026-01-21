#ifndef COMPILER_C_IR_ANALYSIS_H
#define COMPILER_C_IR_ANALYSIS_H

#include "compiler_c/ir/ir_module.h"

void ir_analysis(IR_Context *ctx);
void bitset_add_defined(BitSet *defined, IR_Value *v);
void bitset_add_used(BitSet *defined, BitSet *used, IR_Value *v);

int ir_reg_bitset(IR_Function *f);
void add_successor(IR_Function *func, int from, int to);

Lifetime *compute_lifetimes(IR_Context *ctx, IR_Function *f,int defined, int *rpo);
void compute_reverse_postorder(IR_Function *func, int *rpo);
void compute_bitset(IR_Function *f, int *rpo);

int cmp(const void *a, const void *b);
IR_StackSlot *linear_stack_slot_allocation(Lifetime *lts, int count, int *rpo, int *stack_size, int *slot_count);

void bitset_init(BitSet *s, int reg_count);
void bitset_expand(BitSet *s);
void bitset_add(BitSet *s, int reg);
void bitset_remove(BitSet *s, int reg);
int bitset_has(BitSet *s, int reg);
void bitset_union(BitSet *dst, BitSet *src);
void bitset_intersect(BitSet *dst, BitSet *src);
void bitset_difference(BitSet *dst, BitSet *src);
void bitset_copy(BitSet *dst, BitSet *src);
int bitset_equal(BitSet *dst, BitSet *src);

void print_bitset(BitSet *bs);
void print_cfg(IR_Function *func);

#endif
