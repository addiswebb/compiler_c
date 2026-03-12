#include "compiler_c/core/type.h"
#include "compiler_c/core/array.h"
#include "compiler_c/parse/parser.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Type *type_i8;
Type *type_i16;
Type *type_i32;
Type *type_i64;

Type *type_u8;
Type *type_u16;
Type *type_u32;
Type *type_u64;

Type *type_f32;
Type *type_f64;

Type *type_void;

Type *type_void_ptr;
Type *type_invalid;

TypePool typepool;

void init_types() {
    typepool.count = 0;
    typepool.capacity = 32;
    typepool.types = malloc(sizeof(Type) * typepool.capacity);
    if (!typepool.types) {
        printf("Failed to allocate for global type pool\n");
    }

    type_i8 = init_global_type(T_INT, sizeof(char), QUAL_NONE, SIGNED);
    type_i16 = init_global_type(T_INT, sizeof(short), QUAL_NONE, SIGNED);
    type_i32 = init_global_type(T_INT, sizeof(int), QUAL_NONE, SIGNED);
    type_i64 = init_global_type(T_INT, sizeof(int64_t), QUAL_NONE, SIGNED);

    type_u8 = init_global_type(T_INT, sizeof(char), QUAL_NONE, UNSIGNED);
    type_u16 = init_global_type(T_INT, sizeof(short), QUAL_NONE, UNSIGNED);
    type_u32 = init_global_type(T_INT, sizeof(int), QUAL_NONE, UNSIGNED);
    type_u64 = init_global_type(T_INT, sizeof(int64_t), QUAL_NONE, UNSIGNED);

    type_f32 = init_global_type(T_FLOAT, sizeof(float), QUAL_NONE, SIGNED);
    type_f64 = init_global_type(T_FLOAT, sizeof(double), QUAL_NONE, SIGNED);

    type_void = init_global_type(T_VOID, sizeof(void), QUAL_NONE, SIGNED);
    type_void_ptr = get_pointer_type(type_void);
    type_invalid = init_global_type(T_INVALID, -1, QUAL_NONE, SIGNED);
}

Type *init_global_type(TypeKind type, int size, unsigned int qualifiers, bool is_signed) {
    Type *t = new_type();
    if (!t) {
        printf("Failed to alloc for new type\n");
        exit(1);
    }
    t->kind = type;
    t->size = size;
    t->align = size;
    t->base = type_invalid;
    t->qualifiers = qualifiers;
    t->is_signed = is_signed;
    return t;
}
Type *new_type() {
    if (typepool.count >= typepool.capacity) {
        printf("Too many types [%d/%d]\n", typepool.count, typepool.capacity);
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
    arr_type->_array.array_len = len;
    arr_type->is_signed = SIGNED;
    arr_type->qualifiers = QUAL_NONE;
    return arr_type;
}
Type *infer_array_length(Type *arr_type, int len) {
    arr_type->_array.array_len = len;
    arr_type->size = len * arr_type->base->size;
    return arr_type;
}
Type *new_pointer_type(Type *type) {
    Type *ptr_type = new_type();
    ptr_type->kind = T_POINTER;
    ptr_type->size = sizeof(void *);
    ptr_type->align = ptr_type->size;
    ptr_type->base = type;
    ptr_type->is_signed = SIGNED;
    ptr_type->qualifiers = QUAL_NONE;
    return ptr_type;
}

Type *new_qualified_type(Type *type, unsigned int qualifiers) {
    Type *qual_type = new_type();
    qual_type->kind = type->kind;
    qual_type->size = type->size;
    qual_type->align = type->align;
    qual_type->is_signed = type->is_signed;
    qual_type->qualifiers = qualifiers;
    qual_type->base = type->kind == T_POINTER ? type->base : type;
    return qual_type;
}
Type *new_unsigned_type(Type *type) {
    Type *unsigned_type = new_type();
    unsigned_type->kind = type->kind;
    unsigned_type->size = type->size;
    unsigned_type->align = type->align;
    unsigned_type->is_signed = UNSIGNED;
    unsigned_type->qualifiers = type->qualifiers;
    unsigned_type->base = type;
    return unsigned_type;
}

Type *get_array_type(Type *type, int len) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].base == type && typepool.types[i].kind == T_ARRAY && typepool.types[i]._array.array_len == len) {
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

Type *get_qualified_type(Type *type, unsigned int qualifiers) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].base == type && typepool.types[i].kind == type->kind && typepool.types[i].size == type->size &&
            typepool.types[i].qualifiers == qualifiers && typepool.types[i].is_signed == type->is_signed) {
            return &typepool.types[i];
        }
    }

    return new_qualified_type(type, qualifiers);
}

Type *get_unsigned_type(Type *type) {
    if (type->kind != T_INT) {
        printf("Cannot retrieve signed/unsigned variant of non-integer type\n");
        exit(1);
    }
    if (type->is_signed == UNSIGNED) return type;
    for (int i = 0; i < typepool.count; i++) {
        Type *t = &typepool.types[i];
        if (typepool.types[i].kind == type->kind && typepool.types[i].size == type->size && typepool.types[i].is_signed == UNSIGNED &&
            typepool.types[i].qualifiers == type->qualifiers) {
            return &typepool.types[i];
        }
    }

    return new_unsigned_type(type);
}

Type *get_enum_type(const char *name) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].kind == T_ENUM && strcmp(name, typepool.types[i]._struct.name) == 0) {
            return &typepool.types[i];
        }
    }
    return NULL;
}

Type *get_struct_type(const char *name) {
    if (name == NULL) return NULL;
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].kind == T_STRUCT && strcmp(name, typepool.types[i]._struct.name) == 0) {
            return &typepool.types[i];
        }
    }
    return NULL;
}

void append_enum_field(Type *e, EnumField *f) { append(&e->_enum.fields_array, f); }

void append_struct_member(Type *s, StructMember *f) {
    if (f->type->align > s->align) s->align = f->type->align;
    s->size = align(s->size, s->align);
    f->offset = s->size;
    append(&s->_struct.members_array, f);
    s->size += align(f->type->size, s->align);
}

Type struct_type() {
    Type s;
    s.kind = T_STRUCT;
    s.base = NULL;
    s.align = 0;
    s.size = 0;
    s._array.array_len = 0;
    s.is_signed = SIGNED;
    s.qualifiers = QUAL_NONE;
    s._struct.complete = false;
    s._struct.name = NULL;
    s._struct.members_array.capacity = 0;
    s._struct.members_array.count = 0;
    s._struct.members_array.element_size = -1;
    s._struct.members_array.data = NULL;
    return s;
}

Type enum_type() {
    Type e;
    e.kind = T_ENUM;
    e.base = type_i32;
    e.align = 4;
    e.size = 4;
    e._array.array_len = 0;
    e.is_signed = SIGNED;
    e.qualifiers = NONE;
    e._enum.complete = false;
    e._enum.name = NULL;
    e._enum.fields_array.capacity = 0;
    e._enum.fields_array.count = 0;
    e._enum.fields_array.element_size = -1;
    e._enum.fields_array.data = NULL;
    return e;
}

StructMember *get_member(Type *struct_t, const char *name) {
    for (int i = 0; i < struct_t->_struct.members_array.count; i++) {
        StructMember *member = get_struct_member(struct_t, i);
        if (strcmp(name, member->name) == 0) return member;
    }
    printf("No member named \"%s\" in struct %s\n", name, struct_t->_struct.name);
    exit(1);
}

void print_type(Type *type) {
    if (!type) {
        printf("NULL");
        return;
    }
    if (type->qualifiers & QUAL_CONST) printf("const ");
    if (type->qualifiers & QUAL_VOLATILE) printf("volatile ");
    if (!type->is_signed) printf("unsigned ");
    switch (type->kind) {
    case T_INVALID:
        printf("[INVALID TYPE]");
        break;
    case T_ARRAY:
        print_type(type->base);
        printf("[%d]", type->_array.array_len);
        break;
    case T_INT:
        switch (type->size) {
        case 1:
            printf("char");
            break;
        case 2:
            printf("short");
            break;
        case 4:
            printf("int");
            break;
        case 8:
            printf("long");
            break;
        default:
            printf("Tried to type of int, with invalid size\n");
            exit(1);
        }
        break;
    case T_FLOAT:
        switch (type->size) {
        case 4:
            printf("float");
            break;
        case 8:
            printf("double");
            break;
        default:
            printf("Tried to type of float, with invalid size\n");
            exit(1);
        }
        break;
    case T_POINTER:
        print_type(type->base);
        printf("*");
        break;
    case T_STRUCT:
        printf("struct %s ", type->_struct.name);
        if (DEBUG_STRUCT_DETAILED) {
            printf("{");
            for (int i = 0; i < type->_struct.members_array.count; i++) {
                StructMember *member = get_struct_member(type, i);
                print_type(member->type);
                printf(" %s:[%d@%d], ", member->name, member->offset, member->type->size);
            }
            printf("}");
        }
        break;
    case T_ENUM:
        printf("enum %s ", type->_enum.name);
        if (DEBUG_ENUM_DETAILED) {
            printf("{");
            for (int i = 0; i < type->_enum.fields_array.count; i++) {
                EnumField *field = get_enum_field(type, i);
                printf(" %s = %d, ", field->name, field->value);
            }
            printf("}");
        }
        break;
    case T_VOID:
        printf("void");
        break;
    default:
        printf("Not handling other types in print_type\n");
        exit(1);
    }
}

void print_struct_type(Type *s) {
    printf("struct");
    if (s->_struct.name != NULL) {
        printf("%s", s->_struct.name);
    }
    if (s->_struct.complete) {
        printf(" {\n");
        for (int i = 0; i < s->_struct.members_array.count; i++) {
            StructMember *member = get_struct_member(s, i);
            printf("    ");
            print_type(member->type);
            printf("; [%d]\n", member->offset);
        }
        printf("}\n");
    }
}
