#include "compiler_c/core/type.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/node.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/parse/parser.h"
#include "compiler_c/tokenize/tokenizer.h"

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
    typepool.capacity = 64;
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
        PANIC("Failed to alloc for new type\n");
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
        PANIC("Too many types [%d/%d]\n", typepool.count, typepool.capacity);
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
Type *new_function_type(Type *type, Array params, bool is_variadic) {
    Type *fn_type = new_type();
    fn_type->kind = T_FUNCTION;
    fn_type->size = sizeof(void);
    fn_type->align = sizeof(void);
    fn_type->base = NULL;
    fn_type->_func.return_type = type;
    fn_type->_func.params = params;
    fn_type->_func.is_variadic = is_variadic;
    return fn_type;
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

Type *get_float_type(int size) {
    ASSERT(size > 0 && size > 8, "Size must be between [1-8]");
    if (size <= 4) return type_f32;
    else return type_f64;
}
Type *get_integer_type(int size) {
    switch (size) {
    case 1:
        return type_i8;
    case 2:
        return type_i16;
    case 3:
    case 4:
        return type_i32;
    case 5:
    case 6:
    case 7:
    case 8:
        return type_i64;
    default:
        PANIC("Invalid integer size\n");
    }
}

Type *promote_integer(Type *from, Type *to) { return from->is_signed ? to : get_unsigned_type(to); }

Type *get_pointer_type(Type *type) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].base == type && typepool.types[i].kind == T_POINTER) {
            return &typepool.types[i];
        }
    }

    return new_pointer_type(type);
}

Type *get_array_type(Type *type, int len) {
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].base == type && typepool.types[i].kind == T_ARRAY && typepool.types[i]._array.array_len == len) {
            return &typepool.types[i];
        }
    }
    return new_array_type(type, len);
}

Type *get_function_type(Type *type, Array params, bool is_variadic) {
    ASSERT(type->kind != T_ARRAY, "Functions cannot return arrays\n");
    for (int i = 0; i < typepool.count; i++) {
        Type *t = &typepool.types[i];
        if (t->kind == T_FUNCTION && t->_func.return_type == type && t->_func.is_variadic == is_variadic) {
            if (t->_func.params.count != params.count) continue;
            bool match = true;
            for (int j = 0; j < params.count; j++) {
                ParamDecl *p_a = get(&params, j);
                ParamDecl *p_b = get(&t->_func.params, j);
                if (p_a->type != p_b->type) {
                    match = false;
                    break;
                }
                if (p_a->name != p_b->name) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return t;
            }
        }
    }
    return new_function_type(type, params, is_variadic);
}

Type *get_modified_type(Type *type, Declarator *decl) {
    if (decl->modifiers.count == 0) return type;
    for (int i = decl->modifiers.count - 1; i >= 0; i--) {
        Modifier *mod = (Modifier *)get(&decl->modifiers, i);
        if (mod->kind == MOD_POINTER) {
            type = get_pointer_type(type);
        } else if (mod->kind == MOD_ARRAY) {
            type = get_array_type(type, mod->array_size);
        } else if (mod->kind == MOD_FUNCTION) {
            type = get_function_type(type, mod->function.params, mod->function.is_variadic);
        }
    }
    return type;
}

Type *get_qualified_type(Type *type, unsigned int qualifiers) {
    for (int i = 0; i < typepool.count; i++) {
        Type *t = &typepool.types[i];

        if (t->base == type->base && t->kind == type->kind && t->size == type->size && t->qualifiers == qualifiers &&
            t->is_signed == type->is_signed) {
            return t;
        }
    }

    return new_qualified_type(type, qualifiers);
}
bool cmp_func_types(const Type *a, const Type *b) {
    ASSERT(a->kind == T_FUNCTION, "Can only compare function types\n");
    ASSERT(b->kind == T_FUNCTION, "Can only compare function types\n");
    if (a->_func.is_variadic != b->_func.is_variadic) return false;
    if (a->_func.return_type != b->_func.return_type) return false;
    if (a->_func.params.data != b->_func.params.data) {
        if (a->_func.params.count != b->_func.params.count) return false;
        for (int i = 0; i < a->_func.params.count; i++) {
            ParamDecl *a_p = get(&a->_func.params, i);
            ParamDecl *b_p = get(&b->_func.params, i);
            if (a_p->type != b_p->type) return false;
        }
    }
    return true;
}

Type *get_unsigned_type(Type *type) {
    if (type->kind != T_INT) {
        PANIC("Cannot retrieve signed/unsigned variant of non-integer type\n");
    }
    if (type->is_signed == UNSIGNED) return type;
    for (int i = 0; i < typepool.count; i++) {
        Type *t = &typepool.types[i];
        if (t->kind == type->kind && t->size == type->size && t->is_signed == UNSIGNED && t->qualifiers == type->qualifiers) {
            return t;
        }
    }

    return new_unsigned_type(type);
}

Type *get_enum_type(const char *name) {
    if (name == NULL) return NULL;
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].kind == T_ENUM && strcmp(name, typepool.types[i]._enum.name) == 0) {
            return &typepool.types[i];
        }
    }
    return NULL;
}

Type *get_union_type(const char *name) {
    if (name == NULL) return NULL;
    for (int i = 0; i < typepool.count; i++) {
        if (typepool.types[i].kind == T_UNION && strcmp(name, typepool.types[i]._union.name) == 0) {
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

void append_union_member(Type *u, UnionMember *m) {
    if (m->type->size > u->size) u->size = align(m->type->size, m->type->align);
    append(&u->_union.members_array, m);
}
void append_struct_member(Type *s, StructMember *m) {
    if (m->type->align > s->align) s->align = m->type->align;
    s->size = align(s->size, s->align);
    m->offset = s->size;
    append(&s->_struct.members_array, m);
    s->size += align(m->type->size, s->align);
}

Type union_type() {
    Type u;
    u.kind = T_UNION;
    u.base = NULL;
    u.align = 0;
    u.size = 0;
    u.is_signed = SIGNED;
    u.qualifiers = QUAL_NONE;
    u._union.complete = false;
    u._union.name = NULL;
    u._union.members_array.capacity = 0;
    u._union.members_array.count = 0;
    u._union.members_array.element_size = -1;
    u._union.members_array.data = NULL;
    return u;
}
Type struct_type() {
    Type s;
    s.kind = T_STRUCT;
    s.base = NULL;
    s.align = 0;
    s.size = 0;
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
    PANIC("No member named \"%s\" in struct %s\n", name, struct_t->_struct.name);
}

bool is_func_ptr(Type *t) { return t->kind == T_POINTER && t->base->kind == T_FUNCTION; }

void print_type(const Type *type) {
    if (!type) {
        printf("NULL");
        return;
    }
    if (type->qualifiers & QUAL_CONST) printf("%s ", KEYWORDS[TK_CONST]);
    if (type->qualifiers & QUAL_VOLATILE) printf("%s ", KEYWORDS[TK_VOLATILE]);
    if (type->kind == T_INT && !type->is_signed) printf("%s ", KEYWORDS[TK_UNSIGNED]);
    switch (type->kind) {
    case T_INVALID:
        printf("[###]");
        break;
    case T_ARRAY:
        print_type(type->base);
        printf("[%d]", type->_array.array_len);
        break;
    case T_INT:
        switch (type->size) {
        case 1:
            print_token_type(TK_CHAR);
            break;
        case 2:
            print_token_type(TK_SHORT);
            break;
        case 4:
            print_token_type(TK_INT);
            break;
        case 8:
            print_token_type(TK_LONG);
            break;
        default:
            PANIC("Tried to type of int, with invalid size\n");
        }
        break;
    case T_FLOAT:
        switch (type->size) {
        case 4:
            print_token_type(TK_FLOAT);
            break;
        case 8:
            print_token_type(TK_DOUBLE);
            break;
        default:
            PANIC("Tried to type of float, with invalid size\n");
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
    case T_UNION:
        printf("union %s ", type->_union.name);
        if (DEBUG_STRUCT_DETAILED) {
            printf("{");
            for (int i = 0; i < type->_union.members_array.count; i++) {
                UnionMember *member = get_union_member(type, i);
                print_type(member->type);
                printf(" %s:[%d] ", member->name, member->type->size);
                if (i < type->_union.members_array.count - 1) printf(", ");
            }
            printf("}");
        }
        break;
    case T_VOID:
        print_token_type(TK_VOID);
        break;
    case T_FUNCTION:
        print_type(type->_func.return_type);
        printf("(");
        if (type->_func.params.count == 0) printf("void");
        for (int i = 0; i < type->_func.params.count; i++) {
            ParamDecl *param = (ParamDecl *)get(&type->_func.params, i);
            print_param_decl(param);
            if (i < type->_func.params.count - 1) printf(", ");
        }
        if (type->_func.is_variadic) printf(", ... ");
        printf(")");
        break;
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

void print_typepool() {
    printf("---- Type Pool -----\n");
    for (int i = 0; i < typepool.count; i++) {
        print_type(&typepool.types[i]);
        printf("\n");
    }
}

void print_param_decl(ParamDecl *decl) {
    print_type(decl->type);
    printf(" ");
    if (decl->name) printf("%s", decl->name);
}
