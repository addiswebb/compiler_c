#ifndef COMPILER_C_TYPE_H
#define COMPILER_C_TYPE_H

typedef enum {
    // Yet to be symatically analysed
    T_INVALID,
    T_INT,
    T_FLOAT,
    T_CHAR,
    T_POINTER,
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
    Type *ptr_to;
};

extern Type *type_int;
extern Type *type_float;
extern Type *type_char;
extern Type *type_void_ptr;
extern Type *type_invalid;

extern TypePool typepool;


void init_types();
Type *init_type(TypeKind type, int size);

Type *get_pointer_type(Type *type);
Type *new_pointer_type(Type *type);

#endif // COMPILER_C_TYPE_H
