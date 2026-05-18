#include "compiler_c/core/arena.h"
#include "compiler_c/core/array.h"
#include "compiler_c/log/logger.h"
#include <string.h>

void arena_init(Arena *arena, int block_capacity, int element_size) {
    ASSERT(block_capacity > 0, "Arena Cannot have block_capacity <= 0\n");
    array_init(&arena->blocks, 1, sizeof(Array));
    arena->block_capacity = block_capacity;
    arena->count = 0;
    arena->element_size = element_size;
    arena_add_block(arena);
}

static void arena_add_block(Arena *arena) {
    Array block;
    array_init(&block, arena->block_capacity, arena->element_size);
    append(&arena->blocks, &block);
}

void arena_free(Arena *arena) {
    for (int i = 0; i < arena->blocks.count; i++) {
        Array *block = get(&arena->blocks, i);
        array_free(block);
    }
    array_free(&arena->blocks);
}

static void ensure_index(const Arena *arena, int index) {
#ifdef __COMPILER_C__
    if (index >= arena->count || index < 0) {
#else
    if (__builtin_expect(index >= arena->count || index < 0, 0)) {
#endif
        PANIC("Index of %d is out of Arena bounds of %d\n", index, arena->count);
    }
}

static int arena_capacity(const Arena *arena) { return arena->blocks.count * arena->block_capacity; }

static void ensure_capacity(Arena *arena, int index) {
    if (index >= arena_capacity(arena)) {
        arena_add_block(arena);
    }
}

void *arena_append(Arena *arena, const void *element) {
    ensure_capacity(arena, arena->count);
    int block_index = arena->count / arena->block_capacity;
    Array *block = get(&arena->blocks, block_index);
    arena->count++;
    return append(block, element);
}

Array *arena_get_block(const Arena *arena, int index) { return get(&arena->blocks, index / arena->block_capacity); }

void *arena_get(const Arena *arena, int index) {
    ensure_index(arena, index);
    int block_relative_index = index % (arena->block_capacity);
    Array *block = arena_get_block(arena, index);
    return get(block, block_relative_index);
}
void arena_set(Arena *arena, const void *element, int index) {
    ensure_index(arena, index);
    int block_relative_index = index % arena->block_capacity;
    Array *block = arena_get_block(arena, index);
    memcpy(((char *)block->data + block_relative_index * block->element_size), element, block->element_size);
}
