#ifndef COMPILER_C_ARRAY_H
#define COMPILER_C_ARRAY_H

/*
    Array Allocator
    Dynamic array where whole array moves on resize.
 */
typedef struct Array {
    int count;
    int capacity;
    int element_size;
    void *data;
} Array;

void array_init(Array *arr, int initial_capacity, int element_size);

void array_free(Array *arr);
/* Recursively free each ptr element before freeing the array. */
void ptr_array_free(Array *arr);

/* Append element to the array, resizing if necessary. */
void *append(Array *arr, const void *element);
/* Insert element at the given index, shifting later elements over. */
void *insert(Array *arr, const void *element, int index);
/* Remove last element in the Array. */
void pop(Array *arr);

/* Dynamically copy from null terminated str to arr. */
void array_str_cpy(Array *arr, const char *str);
/* Concatenate n characters from str to arr. */
void array_str_catn(Array *arr, const char *str, int n);

/* Get Array element by index. */
void *get(const Array *arr, int index);
/* Set Array element by index. */
void set(const Array *arr, const void *element, int index);

#endif // COMPILER_C_ARRAY_H
