#include "compiler_c/type.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Type *type_char;
Type *type_short;
Type *type_int;
Type *type_long;

Type *type_float;
Type *type_double;

Type *type_void;

Type *type_void_ptr;
Type *type_invalid;

TypePool typepool;

void init_types() {
    type_char = init_type(T_INT, sizeof(char));
    type_short = init_type(T_INT, sizeof(short));
    type_int = init_type(T_INT, sizeof(int));
    type_long = init_type(T_INT, sizeof(int64_t));

    type_float = init_type(T_FLOAT, sizeof(float));
    type_double = init_type(T_FLOAT, sizeof(double));

    type_void = init_type(T_VOID, sizeof(void));

    type_void_ptr = init_type(T_POINTER, sizeof(void *));
    type_invalid = init_type(T_INVALID, -1);

    typepool.count = 0;
    typepool.capacity = 32;
    typepool.types = malloc(sizeof(Type) * typepool.capacity);
    if (!typepool.types) {
        printf("Failed to allocate for global type pool\n");
    }
}

Type *init_type(TypeKind type, int size) {
    Type *t = malloc(sizeof(Type));
    if (!t) {
        printf("Failed to alloc for new type\n");
        exit(1);
    }
    t->kind = type;
    t->size = size;
    t->align = size;
    t->base = type_invalid;
    return t;
}
Type *new_type() {
    if (typepool.count >= typepool.capacity) {
        printf("Too many types\n");
        exit(1);
    }
    return &typepool.types[typepool.count++];
}

Type *new_array_type(Type *type, int len) {
    Type *arr_type = new_type();
    arr_type->kind = T_ARRAY;
    arr_type->size = type->size * len;
    arr_type->align = type->align;
    arr_type->base = type;
    arr_type->array_len = len;
    return arr_type;
}
Type *infer_array_length(Type *arr_type, int len) {
    arr_type->array_len = len;
    arr_type->size = len * arr_type->base->size;
    return arr_type;
}
Type *new_pointer_type(Type *type) {
    Type *ptr_type = new_type();
    ptr_type->kind = T_POINTER;
    ptr_type->size = sizeof(void *);
    ptr_type->align = ptr_type->size;
    ptr_type->base = type;
    return ptr_type;
}

Type *get_array_type(Type *type, int len) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].base == type && typepool.types[i].kind == T_ARRAY && typepool.types[i].array_len == len) {
            return &typepool.types[i];
        }
    }
    return new_array_type(type, len);
}
// Retrieves the given "type" wrapped in a pointer from global type pool if it exits, otherwise it creates one and adds it to the pool
Type *get_pointer_type(Type *type) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].base == type && typepool.types[i].kind == T_POINTER) {
            return &typepool.types[i];
        }
    }

    return new_pointer_type(type);
}
