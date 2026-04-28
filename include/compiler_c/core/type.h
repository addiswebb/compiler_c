#ifndef COMPILER_C_TYPE_H
#define COMPILER_C_TYPE_H

/* Include enum fields when printing an enum type */
#include "compiler_c/core/arena.h"
#include "compiler_c/core/array.h"
#include <stdint.h>
#define SIGNED 1
#define UNSIGNED 0

#include <stdbool.h>

typedef enum {
    T_VOID,
    T_INT,
    T_FLOAT,
    T_POINTER,
    T_ARRAY,
    T_STRUCT,
    T_ENUM,
    T_UNION,
    T_FUNCTION,
    T_INVALID,
} TypeKind;

typedef struct Type Type;

typedef enum {
    MOD_POINTER,
    MOD_ARRAY,
    MOD_FUNCTION,
} ModifierKind;

typedef struct {
    const char *name;
    Array modifiers;
} Declarator;

typedef struct Symbol Symbol;

typedef struct {
    Type *type;
    const char *name;
    Symbol *symbol;
} ParamDecl;

typedef struct Node Node;

typedef struct {
    ModifierKind kind;
    union {
        Node *array_bounds;
        struct {
            Array params;
            bool is_variadic;
        } function;
    };
} Modifier;

/* Represents a member within a defined struct or union */
typedef struct {
    const char *name;
    Type *type;
    int offset;
} AggrMember;

typedef AggrMember StructMember;
typedef AggrMember UnionMember;

/* Represents an enumerator within a defined enum */
typedef struct {
    char *name;
    union {
        int value;
        Node *const_expr;
    };
    Type *_enum_t;
} EnumField;

typedef enum {
    QUAL_NONE = 0u,
    QUAL_CONST = 1u << 0,
    QUAL_VOLATILE = 1u << 1,
} TypeQualifier;

/* Represents a canonical type of any TypeKind */
struct Type {
    TypeKind kind;
    int size;
    int align;
    bool is_signed;
    unsigned int qualifiers;
    union {
        Type *base;
        struct {
            Type *type;
            int gp_count;
            int fp_count;
        } abi;
    };
    // Data for special types
    union {
        // T_ARRAY
        struct {
            bool is_complete;
            union {
                int64_t array_len;
                Node *const_expr;
            };
        } _array;
        // T_STRUCT
        struct {
            bool complete;
            char *name;
            Array members_array;
        } _struct;
        // T_ENUM
        struct {
            bool complete;
            char *name;
            Array fields_array;
        } _enum;
        struct {
            bool complete;
            char *name;
            Array members_array;
        } _union;
        struct {
            Type *return_type;
            Array params;
            bool is_variadic;
        } _func;
    };
};

typedef enum {
    CONST_INTEGER,
    CONST_FLOAT,
    CONST_STRING,
    CONST_ARRAY,
    CONST_LABEL,
    CONST_REFERENCE,
} ConstLiteralKind;

typedef struct {
    Type *type;
    ConstLiteralKind kind;
    union {
        double f;
        int64_t i;
        struct {
            const char *data;
            int len;
        } s;
        Array arr;
        int const_index;
        struct {
            Symbol *symbol;
            int offset;
        } ref;
    };
} ConstLiteral;

/* Global canonical definitions for all predefined C types */
extern Type *type_i8;
extern Type *type_i16;
extern Type *type_i32;
extern Type *type_i64;

extern Type *type_u8;
extern Type *type_u16;
extern Type *type_u32;
extern Type *type_u64;

extern Type *type_f32;
extern Type *type_f64;

extern Type *type_void;

/* Other Useful types */
extern Type *type_void_ptr;
extern Type *type_invalid;

/*
    Stores all cannonical types in a statically sized array.
    Allowing for easy type comparison
*/
extern Arena typepool;

/* Aligns the given size to the correct alignment */
static inline int align(int size, int align) { return (size + align - 1) & ~(align - 1); }

/* Initialises all global types and the typepool */
void init_typepool();

void free_typepool();

/* Helper for initialising a global type outside of the typepool */
Type *init_global_type(TypeKind type, int size, unsigned int qualifiers, bool is_signed);

/* Returns pointer to a new type in the typepool */
Type *new_type();

/* Wraps the given type in a pointer type */
Type *new_pointer_type(Type *type);

Type *new_qualified_type(Type *type, unsigned int qualifiers);
Type *new_unsigned_type(Type *type);
/* Creates a sized array of the given type */
Type *new_array_type(Type *type, int len);

/* Creates a array type with const expr for array_bounds */
Type *new_incomplete_array_type(Type *type, Node *const_expr);

Type *get_float_type(int size);
Type *get_integer_type(int size);
/*
    Searches the typepool first for corresponding type falls back by creating a new corresponding type.
    Prevents duplicate type definitions.
*/
Type *get_pointer_type(Type *type);
Type *get_enum_type(const char *name);
Type *get_union_type(const char *name);
Type *get_struct_type(const char *name);
Type *get_array_type(Type *type, int len);
Type *get_qualified_type(Type *type, unsigned int qualifiers);
Type *get_function_type(Type *type, Array params, bool is_variadic);
Type *get_modified_type(Type *type, Declarator *decl);
Type *get_unsigned_type(Type *type);

bool cmp_func_types(const Type *a, const Type *b);

/* Expects the signed type to promote to, selects sign from original type. */
Type *promote_integer(Type *from, Type *to);

/*
    Update the length of the given array type, which was previously uninitialized.

    e.g `int a[] = { ... };`. At, int a[], the size is unknown. After { ... } is semantically analysed,
    the size can be inferred and updated.
*/
Type *infer_array_length(Type *arr_type, int len);

/* Appends the given union field, sizing its array if needed */
void append_union_member(Type *u, UnionMember *m);
/* Appends the given enum field, sizing its array if needed */
void append_enum_field(Type *e, EnumField *f);
/* Appends the given struct field, sizing its array if needed */
void append_struct_member(Type *s, StructMember *f);

/* Helper for defining struct types */
Type union_type();
/* Helper for defining struct types */
Type struct_type();
/* Helper for defining enum types */
Type enum_type();

/* Gets the struct member by name from a struct type */
StructMember *get_member(Type *struct_t, const char *name, bool is_root);

bool is_func_ptr(Type *t);

/* Prints the given type as seen in C */
void print_type(const Type *type);
void print_struct_type(Type *s);
void print_param_decl(ParamDecl *decl);
void print_typepool();

#endif // COMPILER_C_TYPE_H
