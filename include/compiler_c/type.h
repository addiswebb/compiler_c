#ifndef COMPILER_C_TYPE_H
#define COMPILER_C_TYPE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum {
    // Yet to be symatically analysed
    T_INT,
    T_FLOAT,
    T_POINTER,
    T_ARRAY,
    T_VOID,
    T_STRUCT,
    T_INVALID,
} TypeKind;

typedef struct Type Type;

typedef struct{
    int count;
    int capacity;
    Type *types;
} TypePool;

typedef struct{
    char *name;
    Type *type;
    int offset;
}StructField;

struct Type{
    TypeKind kind;
    int size;
    int align;
    bool is_signed;
    Type *base;
    union{
        // T_ARRAY
        int array_len;
        // T_STRUCT
        struct{
            char *name;
            int count;
            int capacity;
            StructField* fields;
            bool complete;
        }_struct;
    };
};

extern Type *type_char;
extern Type *type_short;
extern Type *type_int;
extern Type *type_long;

extern Type *type_float;
extern Type *type_double;

extern Type *type_void;

extern Type *type_void_ptr;
extern Type *type_invalid;

extern TypePool typepool;

static inline int align(int size, int align) {
    if ((align & (align - 1)) != 0) {
        printf("Invalid alignment: %d", align);
        exit(1);
    }
    return (size + align - 1) & ~(align - 1);
}

void init_types();
Type *init_type(TypeKind type, int size);

Type *new_type();
Type *get_pointer_type(Type *type);
Type *get_struct_type(const char *name);
Type *new_pointer_type(Type *type);

Type *new_array_type(Type *type, int len);
Type *get_array_type(Type *type, int len);
Type *infer_array_length(Type *arr_type, int len);

void append_struct_field(Type *_struct, StructField *field);

Type struct_type();
StructField *get_member(Type *t, const char *name);
void print_type(Type* type);

#endif // COMPILER_C_TYPE_H
