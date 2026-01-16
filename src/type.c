#include "compiler_c/type.h"
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
    type_long = init_type(T_INT, sizeof(long long));
    
    type_float = init_type(T_FLOAT, sizeof(float));
    type_double = init_type(T_FLOAT, sizeof(double));
    
    type_void = init_type(T_VOID, sizeof(void));

    type_void_ptr = init_type(T_POINTER, sizeof(void *));
    type_invalid = init_type(T_INVALID, -1);

    typepool.count = 0;
    typepool.capacity = 4;
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
    t->base = type_invalid;
    return t;
}

Type *new_pointer_type(Type *type) {
    if (typepool.count >= typepool.capacity) {
        typepool.capacity *= 2;
        Type *new_pool = realloc(typepool.types, sizeof(Type) * typepool.capacity);
        if (!new_pool) {
            printf("Failed to realloc for type pool\n");
            exit(1);
        }
        typepool.types = new_pool;
    }
    Type ptr_type;
    ptr_type.kind = T_POINTER;
    ptr_type.size = sizeof(void *);
    ptr_type.base = type;
    typepool.types[typepool.count] = ptr_type;
    return &typepool.types[typepool.count++];
}

// Retrieves the given "type" wrapped in a pointer from global type pool if it exits, otherwise it creates one and adds it to the pool
Type *get_pointer_type(Type *type) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].base == type) {
            return &typepool.types[i];
        }
    }

    return new_pointer_type(type);
}