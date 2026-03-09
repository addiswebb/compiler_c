#ifndef COMPILER_C_TYPE_H
#define COMPILER_C_TYPE_H

/* Include enum fields when printing an enum type */
#include "compiler_c/core/array.h"
#define DEBUG_ENUM_DETAILED 0
/* Include struct members when printing an enum type */
#define DEBUG_STRUCT_DETAILED 0

#include <stdbool.h>

typedef enum {
    T_INT,
    T_FLOAT,
    T_POINTER,
    T_ARRAY,
    T_VOID,
    T_STRUCT,
    T_ENUM,
    T_INVALID,
} TypeKind;

typedef struct Type Type;

/*
    Stores all cannonical types in a statically sized array.
    Allowing for easy type comparison
*/
typedef struct{
    int count;
    int capacity;
    Type *types;
} TypePool;

/* Represents a member within a defined struct */
typedef struct{
    char *name;
    Type *type;
    int offset;
}StructMember;

/* Represents an enumerator within a defined enum */
typedef struct{
    char *name;
    int value;
    Type* _enum_t;
}EnumField;

/* Represents a canonical type of any TypeKind */
struct Type{
    TypeKind kind;
    int size;
    int align;
    bool is_signed;
    Type *base;
    // Data for special types
    union{
        // T_ARRAY
        struct{
            int array_len;
        }_array;
        // T_STRUCT
        struct{
            char *name;
            bool complete;
            Array members_array;
        }_struct;
        // T_ENUM
        struct{
            char *name;
            bool complete;
            Array fields_array;
        }_enum;
    };
};

/* Global canonical definitions for all predefined C types */
extern Type *type_char;
extern Type *type_short;
extern Type *type_int;
extern Type *type_long;

extern Type *type_float;
extern Type *type_double;

extern Type *type_void;

/* Other Useful types */
extern Type *type_void_ptr;
extern Type *type_invalid;


/* Global typepool */
extern TypePool typepool;

/* Aligns the given size to the correct alignment */
static inline int align(int size, int align) {
    return (size + align - 1) & ~(align - 1);
}

/* Initialises all global types and the typepool */
void init_types();

/* Helper for initialising a global type outside of the typepool */
Type *init_global_type(TypeKind type, int size);

/* Returns pointer to a new type in the typepool */
Type *new_type();

/* Wraps the given type in a pointer type */
Type *new_pointer_type(Type *type);
/* Creates a sized array of the given type */
Type *new_array_type(Type *type, int len);

/*
    Searches the typepool first for corresponding type falls back by creating a new corresponding type.
    Prevents duplicate type definitions.
*/
Type *get_pointer_type(Type *type);
Type *get_enum_type(const char *name);
Type *get_struct_type(const char *name);
Type *get_array_type(Type *type, int len);

/*
    Update the length of the given array type, which was previously uninitialized.

    e.g `int a[] = { ... };`. At, int a[], the size is unknown. After { ... } is semantically analysed,
    the size can be inferred and updated.
*/
Type *infer_array_length(Type *arr_type, int len);

/* Appends the given enum field, sizing its array if needed */
void append_enum_field(Type *e, EnumField *f);
/* Appends the given struct field, sizing its array if needed */
void append_struct_member(Type *s, StructMember *f);

/* Helper for defining struct types */
Type struct_type();
/* Helper for defining enum types */
Type enum_type();

/* Gets the struct member by name from a struct type */
StructMember *get_member(Type *struct_t, const char *name);

/* Prints the given type as seen in C */
void print_type(Type* type);

#endif // COMPILER_C_TYPE_H
