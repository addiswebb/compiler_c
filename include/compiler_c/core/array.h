#ifndef COMPILER_C_ARRAY_H
#define COMPILER_C_ARRAY_H

typedef struct{
    int count;
    int capacity;
    int element_size;
    void *data;
}Array;

void array_init(Array *arr, int initial_capacity,int element_size);
void array_free(Array *arr);

void *append(Array *arr, const void *element);
void *insert(Array *arr, const void *element, int index);
void pop(Array *arr);

void *get(const Array *arr, int index);
void set(const Array *arr, const void *element, int index);

#endif // COMPILER_C_UTIL_H
