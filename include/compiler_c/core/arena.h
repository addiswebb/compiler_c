#ifndef COMPILER_C_ARENA_H
#define COMPILER_C_ARENA_H

#include "compiler_c/core/array.h"

/*
    Arena Allocator
    Dynamic array where existing elements are static on resize.
*/
typedef struct {
    Array blocks;
    int element_size;
    int block_capacity;
    int count;
} Arena;

void arena_init(Arena *arena, int block_size, int element_size);

/* Allocate and append another block to the arena. */
static void arena_add_block(Arena *arena);
void arena_free(Arena *arena);

/* Append the element to the Arena, adding a new block if necessary. */
void *arena_append(Arena *arena, const void *element);

Array *arena_get_block(const Arena *arena, int index);
void *arena_get(const Arena *arena, int index);
void arena_set(const Arena *arena, const void *element, int index);

#endif
