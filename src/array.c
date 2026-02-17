#include "compiler_c/array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void array_init(Array *arr, int initial_capacity, int element_size) {
    arr->element_size = element_size;
    arr->capacity = initial_capacity;
    arr->data = malloc(arr->element_size * arr->capacity);
    if (!arr->data) {
        printf("Failed to initialize Array\n");
        exit(1);
    }
    arr->count = 0;
}

void *append(Array *arr, void *element) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        void *new_data = realloc(arr->data, arr->element_size * arr->capacity);
        if (!new_data) {
            printf("Failed to resize array\n");
            exit(1);
        }
        arr->data = new_data;
    }
    memcpy((char *)arr->data + arr->count * arr->element_size, element, arr->element_size);
    return (char *)arr->data + arr->count++ * arr->element_size;
}

void pop(Array *arr) { arr->count--; }

void *get(const Array *arr, int index) {
    if (index < 0 || index >= arr->count) {
        printf("Index of %d is out of Array bounds of %d", index, arr->count);
        exit(1);
    }
    return (char *)arr->data + index * arr->element_size;
}

void array_free(Array *arr) {
    if (arr->data) free(arr->data);
}
