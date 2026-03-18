#include "compiler_c/core/array.h"
#include "compiler_c/log/logger.h"
#include <stdlib.h>
#include <string.h>

void array_init(Array *arr, int initial_capacity, int element_size) {
    ASSERT(element_size > 0, "Array cannot have element capacity <= 0\n");
    ASSERT(initial_capacity > 0, "Array cannot have element capacity <= 0\n");
    arr->element_size = element_size;
    arr->capacity = initial_capacity;
    arr->data = malloc(arr->element_size * arr->capacity);
    if (!arr->data) {
        PANIC("Failed to initialize Array\n");
    }
    arr->count = 0;
}

static void ensure_capacity(Array *arr) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        void *new_data = realloc(arr->data, arr->element_size * arr->capacity);
        if (!new_data) {
            PANIC("Failed to resize array\n");
        }
        arr->data = new_data;
    }
}
static void ensure_index(const Array *arr, int index) {
    if (__builtin_expect(index >= arr->count || index < 0, 0)) {
        PANIC("Index of %d is out of Array bounds of %d\n", index, arr->count);
    }
}

void *append(Array *arr, const void *element) {
    ensure_capacity(arr);
    memcpy((char *)arr->data + arr->count * arr->element_size, element, arr->element_size);
    return (char *)arr->data + arr->count++ * arr->element_size;
}

void *insert(Array *arr, const void *element, int index) {
    ensure_capacity(arr);
    ensure_index(arr, index);

    char *src = (char *)arr->data + index * arr->element_size;
    char *dst = src + arr->element_size;
    memmove(dst, src, arr->element_size * (arr->count - index));

    memcpy((char *)arr->data + index * arr->element_size, element, arr->element_size);
    arr->count++;
    return (char *)arr->data + index * arr->element_size;
}

void pop(Array *arr) { arr->count--; }

void *get(const Array *arr, int index) {
    ensure_index(arr, index);
    return (char *)arr->data + index * arr->element_size;
}

void set(const Array *arr, const void *element, int index) {
    ensure_index(arr, index);
    memcpy(((char *)arr->data + index * arr->element_size), element, arr->element_size);
}

void array_free(Array *arr) {
    free(arr->data);
    arr->data = NULL;
}
