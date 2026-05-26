#ifndef COMPILER_C_ARRAY_H
#define COMPILER_C_ARRAY_H

typedef struct Array {
    int count;
    int capacity;
    int element_size;
    void *data;
} Array;

void array_init(Array *arr, int initial_capacity, int element_size);

void array_free(Array *arr);
void ptr_array_free(Array *arr);

void *append(Array *arr, const void *element);
void *insert(Array *arr, const void *element, int index);
void pop(Array *arr);

void array_str_cpy(Array *arr, const char *str);
void array_str_catn(Array *arr, const char *str, int n);

void *get(const Array *arr, int index);
void set(const Array *arr, const void *element, int index);

#endif // COMPILER_C_UTIL_H
