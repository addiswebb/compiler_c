#ifndef COMPILER_C_TYPE_H
#define COMPILER_C_TYPE_H

#include <stdbool.h>
typedef enum {
    // Yet to be symatically analysed
    T_INT,
    T_FLOAT,
    T_POINTER,
    T_ARRAY,
    T_VOID,
    T_INVALID,
} TypeKind;

typedef struct Type Type;

typedef struct{
    int count;
    int capacity;
    Type *types;
} TypePool;

struct Type{
    TypeKind kind;
    int size;
    int align;
    bool is_signed;
    Type *base;
    int array_len;
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


void init_types();
Type *init_type(TypeKind type, int size);

Type *new_type();
Type *get_pointer_type(Type *type);
Type *new_pointer_type(Type *type);

Type *new_array_type(Type *type, int len);
Type *get_array_type(Type *type, int len);

#endif // COMPILER_C_TYPE_H
