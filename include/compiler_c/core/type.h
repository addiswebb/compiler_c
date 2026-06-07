#ifndef COMPILER_C_TYPE_H
#define COMPILER_C_TYPE_H

/* Include enum fields when printing an enum type */
#include "compiler_c/core/arena.h"

#include "../libc/stdbool.h"
#include <stdint.h>

#define SIGNED 1
#define UNSIGNED 0

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
    // signed / unsigned
    bool is_signed;
    // const, volatile
    unsigned int qualifiers;
    // False if the type has const expressions which need evaluating
    bool is_resolved;
    // Prevent recursively printing nested types in DEBUG.
    bool printing;
    union {
        // Underlying type for pointers
        Type *base;
        // ABI specific version of parsed type
        struct {
            Type *type;
            int gp_count;
            int fp_count;
        } abi;
    };
    union {
        // T_ARRAY
        struct {
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
        // T_UNION
        struct {
            bool complete;
            char *name;
            Array members_array;
        } _union;
        // T_FUNCTION
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
    CONST_ZERO,
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
        int zero_bytes;
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
    Stores all canonical types in a statically sized array.
    Allowing for easy type comparisons
*/
extern Arena typepool;

/* Aligns the given size to the correct alignment */
static int align(const int size, const int align) { return (size + align - 1) & ~(align - 1); }

/* Initializes all global types and the typepool */
void init_typepool();
void free_typepool();

/* Helper for initializing a C standard types. */
Type *init_global_type(TypeKind type, int size, unsigned int qualifiers, bool is_signed);

/* Appends a new empty type to the typepool and returns its pointer. */
Type *new_type();

/* Returns pointer to the given type */
Type *new_pointer_type(Type *type);
/* Returns new identical type with given qualifiers. */
Type *new_qualified_type(const Type *type, unsigned int qualifiers);
/* Returns unsigned variant of given type */
Type *new_unsigned_type(Type *type);
/* Creates a sized array of the given type */
Type *new_array_type(Type *type, int len);

/* Creates an incomplete array type with an unevaluated const_expr for its length. */
Type *new_incomplete_array_type(Type *type, Node *const_expr);
/* Returns either f32 or f64 based of size. */
Type *get_float_type(int size);
/* Returns signed integer of correct size. */
Type *get_integer_type(int size);
/*
    All get_x_type() functions search the typepool for the existing type,

    Searches the typepool first for corresponding type falls back by creating a new corresponding type.
    Prevents duplicate type definitions.
*/

Type *get_pointer_type(Type *type);
Type *get_array_type(Type *type, int len);
Type *get_qualified_type(const Type *type, unsigned int qualifiers);
Type *get_function_type(Type *type, Array params, bool is_variadic);
Type *get_modified_type(Type *type, const Declarator *decl);
/* Returns unsigned version of type, which must be some signed intger. */
Type *get_unsigned_type(Type *type);

/* These get_aggregate_type by name functions return NULL on failure instead of creating a new type. */

Type *get_enum_type(const char *name);
Type *get_union_type(const char *name);
Type *get_struct_type(const char *name);

/*
    Compares two function types a and b by their return type and parameter types.
    Ignoring param names, as multiple functionally identical function types may exist.
    e.g `int(int, float) === int(int a, float b)`
*/
bool cmp_func_types(const Type *a, const Type *b);

/* Update the length of the given array type, which was previously uninitialized. */
Type *infer_array_length(Type *arr_type, int inferred_len);

/* Appends the union field, updating the union size and alignment. */
void append_union_member(Type *u, const UnionMember *m);
/* Appends the struct field, updating the struct size and alignment. */
void append_struct_member(Type *s, StructMember *f);
/* Appends the enum field. */
void append_enum_field(Type *e, EnumField *f);

/* Return empty union type. */
Type union_type();
/* Return empty struct type. */
Type struct_type();
/* Return empty enum type. */
Type enum_type();

/*
    Tries to retrieve the member by name.
    Will also explore unnamed root level unions and structs.
    Emits error if is_root is true and fails to find the named member.
*/
AggrMember *get_member(const Type *struct_t, const char *name, bool is_root, int *offset, int *index);

bool is_func_ptr(const Type *t);
bool is_scalar_type(const Type *t);

void print_type(Type *type);
void print_param_decl(const ParamDecl *decl);
void print_typepool();

#endif // COMPILER_C_TYPE_H
