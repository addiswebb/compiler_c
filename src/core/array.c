#include "compiler_c/core/array.h"
#include "compiler_c/log/logger.h"
#include <stdlib.h>
#include <string.h>

void array_init(Array *arr, const int initial_capacity, const int element_size) {
    ASSERT(element_size > 0, "Array cannot have element size <= 0\n");
    ASSERT(initial_capacity > 0, "Array cannot have element capacity <= 0\n");
    arr->element_size = element_size;
    arr->capacity = initial_capacity;
    arr->data = malloc(arr->element_size * arr->capacity);
    if (!arr->data) {
        PANIC("Failed to initialize Array\n");
    }
    arr->count = 0;
}

static void ensure_capacity(Array *arr, const int size) {
    if (size >= arr->capacity) {
        arr->capacity *= 2;
        void *new_data = realloc(arr->data, arr->element_size * arr->capacity);
        if (!new_data) {
            PANIC("Failed to resize array\n");
        }
        arr->data = new_data;
    }
}

/* Conservatively realloc */
static void reserve_capacity(Array *arr, const int size) {
    ASSERT(size > 0, "Cannot reserve 0 capacity\n");
    if (size > arr->capacity) {
        arr->capacity = size;
        void *new_data = realloc(arr->data, arr->capacity);
        ASSERT(new_data, "Failed to resize array\n");
        arr->data = new_data;
    }
}
static void ensure_index(const Array *arr, const int index) {
#ifdef __COMPILER_C__
    if (index >= arr->count || index < 0) {
#else
    if (__builtin_expect(index >= arr->count || index < 0, 0)) {
#endif
        PANIC("Index of %d is out of Array bounds of %d\n", index, arr->count);
    }
}

void *append(Array *arr, const void *element) {
    ensure_capacity(arr, arr->count);
    memcpy((char *)arr->data + arr->count * arr->element_size, element, arr->element_size);
    return (char *)arr->data + arr->count++ * arr->element_size;
}

void *insert(Array *arr, const void *element, const int index) {
    // TODO max(arr->count, index);
    ensure_capacity(arr, arr->count);
    ASSERT(index <= arr->capacity && index >= 0, "Index must be within capacity to insert in array\n");

    char *src = (char *)arr->data + index * arr->element_size;
    char *dst = src + arr->element_size;
    memmove(dst, src, arr->element_size * (arr->count - index));

    memcpy((char *)arr->data + index * arr->element_size, element, arr->element_size);
    arr->count++;
    return (char *)arr->data + index * arr->element_size;
}

void pop(Array *arr) { arr->count--; }

void *get(const Array *arr, const int index) {
    ensure_index(arr, index);
    return (char *)arr->data + index * arr->element_size;
}

void set(const Array *arr, const void *element, const int index) {
    ensure_index(arr, index);
    memcpy(((char *)arr->data + index * arr->element_size), element, arr->element_size);
}

void ptr_array_free(Array *arr) {
    for (int i = 0; i < arr->count; i++) {
        free(*(void **)get(arr, i));
    }
    array_free(arr);
}

void array_free(Array *arr) {
    arr->capacity = 0;
    free(arr->data);
    arr->data = NULL;
}

void array_str_cpy(Array *arr, const char *str) {
    arr->count = 0;
    while (*str != '\0') {
        append(arr, str++);
    }
    append(arr, &(char){'\0'});
}

void array_str_catn(Array *arr, const char *str, const int n) {
    reserve_capacity(arr, arr->count + n + 1);
    memcpy(arr->data + arr->count, str, n);
    arr->count += n;
    append(arr, &(char){'\0'});
    arr->count--;
}
