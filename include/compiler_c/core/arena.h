#ifndef COMPILER_C_ARENA_H
#define COMPILER_C_ARENA_H

#include "compiler_c/core/array.h"

typedef struct{
    Array blocks;
    int element_size;
    int block_capacity;
    int count;
}Arena;

void arena_init(Arena *arena, int block_size, int element_size);

static void arena_add_block(Arena *arena);
void arena_free(Arena *arena);

void *arena_append(Arena *arena, const void *element);

void *arena_get(Arena *arena, int index);
void arena_set(Arena *arena, const void *element, int index);

#endif
