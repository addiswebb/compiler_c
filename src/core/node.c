#include "compiler_c/core/arena.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <compiler_c/core/node.h>

#include "inttypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *builtin_names[BUILTIN_COUNT] = {[BUILTIN_MEMCPY] = "__builtin_memcpy",
                                            [BUILTIN_VA_ARG] = "__builtin_va_arg",
                                            [BUILTIN_VA_START] = "__builtin_va_start",
                                            [BUILTIN_VA_END] = "__builtin_va_end"};

BuiltinKind get_builtin_kind(const char *name) {
    if (strcmp(name, "") == 0) return BUILTIN_NONE;
    for (int i = 1; i <= BUILTIN_COUNT; i++) {
        if (strcmp(builtin_names[i], name) == 0) return (BuiltinKind)i;
    }
    return BUILTIN_NONE;
}

NodeManager new_node_manager() {
    NodeManager nm;
    arena_init(&nm, NODE_ARENA_SIZE, sizeof(Node));
    return nm;
}

/*
    Handles creating a Node, pushing it to the global node array
*/
Node *new_node(NodeManager *nm, const NodeKind kind) {
    Node *node = arena_append(nm, &(Node){0});
    node->kind = kind;
    node->type = type_invalid;
    return node;
}

Node *cast_node_unchecked(NodeManager *nm, Node *node, Type *type) {
    Node *cast = new_node(nm, N_CAST);
    cast->cast.from = node->type;
    cast->cast.to = type;
    cast->cast.expr = node;
    return cast;
}
Node *cast_node(NodeManager *nm, Node *node, Type *type) {
    if (is_func_ptr(node->type) && is_func_ptr(type)) {
        if (cmp_func_types(node->type->base, type->base)) {
            return node;
        }
    }
    if (node->type->kind == T_FUNCTION && type->kind == T_POINTER && type->base->kind == T_FUNCTION &&
        cmp_func_types(node->type, type->base)) {

        node->type = get_pointer_type(node->type);
        return node;
    }

    if (!is_valid_cast(node->type, type)) {
        log_start(LOG_ERROR);
        printf("Invalid conversion from ");
        print_type(node->type);
        printf(" to ");
        print_type(type);
        printf("\n");
        exit(1);
    }

    Node *cast = new_node(nm, N_CAST);
    cast->type = type;
    // cast->cast.to = type;
    cast->cast.from = node->type;
    cast->cast.expr = node;
    return cast;
}

bool is_valid_cast(const Type *from, const Type *to) {
    if (from->kind == T_INVALID || to->kind == T_INVALID) return false;
    if (from->kind == T_FUNCTION && to->kind == T_POINTER) {
        return cmp_func_types(from, to->base);
    }
    if (from->kind == T_ARRAY) {
        // Can only cast array->pointer (pointer decay)
        return to->kind == T_POINTER && (from->base == to->base || to->base == type_void);
    }
    if (to->kind == T_POINTER) return to->base == from;
    return true;
}

LiteralKind literal_kind(const TokenType type) {
    switch (type) {
    case TK_INT_LITERAL:
        return L_INT;
    case TK_FLT_LITERAL:
        return L_FLOAT;
    case TK_CHAR_LITERAL:
        return L_CHAR;
    case TK_STRING_LITERAL:
        return L_STRING;
    default:
        PANIC("Given a non literal token\n");
    }
}
void print_node_type(const NodeKind type) {
    switch (type) {
    case N_TRANSLATION_UNIT:
        printf("Translation Unit");
        break;
    case N_FUNCTION:
        printf("Function");
        break;
    case N_COMPOUND:
        printf("Compound");
        break;
    case N_VAR_DECL:
        printf("Var Declaration");
        break;
    case N_RETURN:
        printf("Return");
        break;
    case N_BINARY:
        printf("Binary");
        break;
    case N_LITERAL:
        printf("Literal");
        break;
    case N_IDENTIFIER:
        printf("Identifier");
        break;
    case N_IF:
        printf("If");
        break;
    case N_WHILE:
        printf("While");
        break;
    case N_FOR:
        printf("For");
        break;
    case N_FUNCTION_CALL:
        printf("Func Call");
        break;
    case N_UNARY:
        printf("Unary");
        break;
    case N_CAST:
        printf("Cast");
        break;
    case N_INDEX:
        printf("Index");
        break;
    case N_TYPE:
        printf("Type");
        break;
    case N_CONTINUE:
        printf("Continue");
        break;
    case N_BREAK:
        printf("Break");
        break;
    case N_INIT_LIST:
        printf("Init List");
        break;
    case N_MEMBER_ACCESS:
        printf("Member Access");
        break;
    case N_SWITCH:
        printf("Switch");
        break;
    case N_CASE:
        printf("Case");
        break;
    case N_TYPEDEF:
        printf("Typedef");
        break;
    case N_GOTO:
        printf("Goto");
        break;
    case N_LABEL:
        printf("Label");
        break;
    case N_COMPOUND_LITERAL:
        printf("Compound Literal");
        break;
    case N_DESIGNATED_INITIALIZER:
        printf("Designated Initializer");
        break;
    case N_BUILTIN:
        printf("__Builtin");
        break;
    }
}

void print_indent(const int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
}

void print_node(const Node *node, const int depth) {
    print_indent(depth);
    if (node == NULL) {
        printf("-\n");
        return;
    }
    print_node_type(node->kind);
    printf(" |");
    print_type(node->type);
    printf("| ");
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        printf("\n");
        for (int i = 0; i < node->translation_unit.declarations_array.count; i++) {
            print_node(get_node(&node->translation_unit.declarations_array, i), depth + 1);
        }
        break;
    case N_COMPOUND:
        printf("\n");
        for (int i = 0; i < node->compound.items_array.count; i++) {
            print_node(get_node(&node->compound.items_array, i), depth + 1);
        }
        break;
    case N_BINARY:
        printf(": [op= ");
        print_token_type(node->binary.op);
        printf(", type= ");
        print_type(node->type);
        printf(" ]\n");
        print_node(node->binary.lhs, depth + 1);
        print_node(node->binary.rhs, depth + 1);
        break;
    case N_LITERAL:
        printf(": [type= ");
        print_type(node->type);
        printf(", ");
        if (node->type == type_i8) {
            printf("value= %c]\n", (char)node->literal.i);
            break;
        }
        switch (node->type->kind) {
        case T_ENUM:
        case T_INT:
            switch (node->type->size) {
            case 1:
                printf("value= '%c']\n", (char)node->literal.i);
                break;
            case 2:
                printf("value= %d]\n", (short)node->literal.i);
                break;
            case 4:
                printf("value= %d]\n", (int)node->literal.i);
                break;
            case 8:
                printf("value= %" PRId64 "]\n", node->literal.i);
                break;
            default:
                printf("size: %d\n", node->type->size);
                PANIC("Given invalid size of int to print int literal\n");
            }
            break;
        case T_FLOAT:
            printf("value= %g]\n", node->literal.f);
            break;
        case T_INVALID:
            printf("value= [###]]\n");
            break;
        case T_ARRAY:
            if (node->type->base == type_i8) {
                printf("value= \"");
                for (int i = 0; i < node->type->_array.array_len; i++) {
                    printf("%c", node->literal.s.data[i]);
                }
                printf("\"]\n");
                break;
            }
        case T_POINTER:
            if (node->type->base == type_i8) {
                printf("value= \"%s\"]\n", node->literal.s.data);
                break;
            }
        default:
            printf(" (dont know how to print this type)]\n");
            break;
        }
        break;
    case N_FUNCTION:
        printf(": [name= %s, param_count= %d, has_initializer=", node->func.name, node->type->_func.params.count);
        if (node->func.is_defined) printf("true");
        else printf("false");

        if (node->type->_func.is_variadic) printf(", variadic");

        if (node->func.storage_class == EXTERN) printf(", extern");
        if (node->func.storage_class == STATIC) printf(", static");
        printf("]\n");
        if (node->func.is_defined) print_node(node->func.body, depth + 1);
        break;
    case N_VAR_DECL:
        printf(": [type= ");
        print_type(node->type);
        printf(", name= %s", node->var_decl.identifier->identifier.name);
        printf(", has_initializer=");
        if (node->var_decl.is_defined) printf("true");
        else printf("false");
        if (node->var_decl.storage_class == EXTERN) printf(", extern");
        if (node->var_decl.storage_class == STATIC) printf(", static");
        if (node->var_decl.is_global) printf(", global");
        printf("]\n");
        if (node->var_decl.expr) print_node(node->var_decl.expr, depth + 1);
        break;
    case N_RETURN:
        printf("\n");
        print_node(node->_return.expr, depth + 1);
        break;
    case N_IDENTIFIER:
        printf(": [name= %s, type= ", node->identifier.name);
        print_type(node->type);
        printf("]\n");
        break;
    case N_IF:
        printf(": [cond, true, false]\n");
        print_node(node->_if.cond, depth + 1);
        print_node(node->_if.if_true, depth + 1);
        if (node->_if.if_false != NULL) print_node(node->_if.if_false, depth + 1);
        break;
    case N_WHILE:
        printf(": [cond, true]\n");
        print_node(node->_while.cond, depth + 1);
        print_node(node->_while.block, depth + 1);
        break;
    case N_FOR:
        printf(": [(init, cond, iter), block]\n");
        print_node(node->_for.init, depth + 1);
        print_node(node->_for.cond, depth + 1);
        print_node(node->_for.iter, depth + 1);
        print_node(node->_for.block, depth + 1);
        break;
    case N_FUNCTION_CALL:
        printf(": [name, param_count: %d]\n", node->func_call.params_array.count);
        print_node(node->func_call.callee, depth + 1);
        print_indent(depth + 1);
        printf("Params: (\n");
        for (int i = 0; i < node->func_call.params_array.count; i++) {
            print_node(get_node(&node->func_call.params_array, i), depth + 2);
        }
        print_indent(depth + 1);
        printf(")\n");
        break;
    case N_UNARY:
        printf(": [op= ");
        print_token_type(node->unary.op);
        printf(", associativity= %d]\n", node->unary.associativity);
        print_node(node->unary.expr, depth + 1);
        break;
    case N_CAST:
        printf(": [from= ");
        print_type(node->cast.from);
        printf(", to= ");
        print_type(node->type);
        printf(" ]\n");
        print_node(node->cast.expr, depth + 1);
        break;
    case N_INDEX:
        printf(": [index, identifier]\n");
        print_node(node->index.index, depth + 1);
        print_node(node->index.identifier, depth + 1);
        break;
    case N_TYPE:
        printf(": [type= ");
        print_type(node->type);
        printf("]\n");
        break;
    case N_CONTINUE:
    case N_BREAK:
        printf("\n");
        break;
    case N_INIT_LIST:
        printf(": [count= %d, type=", node->init_list.elements_array.count);
        print_type(node->type);
        printf("]\n");
        for (int i = 0; i < node->init_list.elements_array.count; i++) {
            print_node(get_node(&node->init_list.elements_array, i), depth + 1);
        }
        break;
    case N_MEMBER_ACCESS:
        printf(": [type= ");
        print_type(node->type);
        printf(", op=");
        print_token_type(node->member_access.op);
        printf(", offset=%d]\n", node->member_access.offset);
        print_node(node->member_access.identifier, depth + 1);
        print_node(node->member_access.member, depth + 1);
        break;
    case N_SWITCH:
        printf(": [cases= %d]\n", node->_switch.cases_array.count);
        print_node(node->_switch.test, depth + 1);
        print_node(node->_switch.block, depth + 1);
        break;
    case N_CASE:
        if (node->_case.test) {
            printf(": [index=%d]\n", node->_case.i);
            print_node(node->_case.test, depth + 1);
        } else printf(": Default\n");
        break;
    case N_TYPEDEF:
        printf(": [type= ");
        print_type(node->type);
        printf(", name= %s ]\n", node->_typedef.name);
        break;
    case N_GOTO:
        printf(": [ %s ]\n", node->_goto.identifier->identifier.name);
        break;
    case N_LABEL:
        printf(": [ %s ]\n", node->label.identifier->identifier.name);
        break;
    case N_COMPOUND_LITERAL:
        printf(": [type= ");
        print_type(node->type);
        printf("]\n");
        print_node(node->compound_literal.value, depth + 1);
        break;
    case N_DESIGNATED_INITIALIZER:
        if (node->designated_init.kind == T_ARRAY) printf(": [index= %d]\n", node->designated_init._array.index);
        else printf(": [name= %s]\n", node->designated_init._struct.name);
        print_node(node->designated_init.value, depth + 1);
        break;
    case N_BUILTIN:
        printf(": [name= %s, param_count= %d]\n", builtin_names[node->_builtin.kind], node->_builtin.params.count);
        for (int i = 0; i < node->_builtin.params.count; i++) {
            print_node(get_node(&node->_builtin.params, i), depth + 1);
        }
        break;
    }
}

/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm) { print_node(arena_get(nm, 0), 0); }

void free_node(Node *node) {
    if (!node) {
        // WARN("Tried to free NULL node\n");
        return;
    }
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        for (int i = 0; i < node->translation_unit.declarations_array.count; i++) {
            free_node(get_node(&node->translation_unit.declarations_array, i));
        }
        array_free(&node->translation_unit.declarations_array);
        break;
    case N_FUNCTION:
        node->func.name = NULL;
        free_node(node->func.body);
        node->func.body = NULL;
        break;
    case N_COMPOUND:
        for (int i = 0; i < node->compound.items_array.count; i++) {
            free_node(get_node(&node->compound.items_array, i));
        }
        array_free(&node->compound.items_array);
        break;
    case N_VAR_DECL:
        free_node(node->var_decl.identifier);
        node->var_decl.identifier = NULL;
        free_node(node->var_decl.expr);
        node->var_decl.expr = NULL;
        break;
    case N_IF:
        free_node(node->_if.cond);
        node->_if.cond = NULL;
        free_node(node->_if.if_true);
        node->_if.if_true = NULL;
        free_node(node->_if.if_false);
        node->_if.if_false = NULL;
        break;
    case N_WHILE:
        free_node(node->_while.cond);
        node->_while.cond = NULL;
        free_node(node->_while.block);
        node->_while.block = NULL;
        break;
    case N_FOR:
        free_node(node->_for.init);
        node->_for.init = NULL;
        free_node(node->_for.cond);
        node->_for.cond = NULL;
        free_node(node->_for.iter);
        node->_for.iter = NULL;
        free_node(node->_for.block);
        node->_for.block = NULL;
        break;
    case N_SWITCH:
        for (int i = 0; i < node->_switch.cases_array.count; i++) {
            free_node(get_node(&node->_switch.cases_array, i));
        }
        array_free(&node->_switch.cases_array);
        free_node(node->_switch.block);
        node->_switch.block = NULL;
        free_node(node->_switch.test);
        node->_switch.test = NULL;
        break;
    case N_CASE:
        free_node(node->_case.test);
        node->_case.test = NULL;
        break;
    case N_RETURN:
        free_node(node->_return.expr);
        node->_return.expr = NULL;
        break;
    case N_UNARY:
        free_node(node->unary.expr);
        node->unary.expr = NULL;
        break;
    case N_BINARY:
        free_node(node->binary.lhs);
        node->binary.lhs = NULL;
        free_node(node->binary.rhs);
        node->binary.rhs = NULL;
        break;
    case N_LITERAL:
        node->literal.raw_rata = NULL;
        if (node->literal.kind == L_STRING) {
            free(node->literal.s.data);
            node->literal.s.data = NULL;
        }
        break;
    case N_IDENTIFIER:
        node->identifier.name = NULL;
        break;
    case N_FUNCTION_CALL:
        for (int i = 0; i < node->func_call.params_array.count; i++) {
            free_node(get_node(&node->func_call.params_array, i));
        }
        array_free(&node->func_call.params_array);
        free_node(node->func_call.callee);
        node->func_call.callee = NULL;
        break;
    case N_CAST:
        free_node(node->cast.expr);
        node->cast.expr = NULL;
        break;
    case N_INDEX:
        free_node(node->index.identifier);
        node->index.identifier = NULL;
        free_node(node->index.index);
        node->index.index = NULL;
        break;
    case N_TYPE:
        break;
    case N_TYPEDEF:
        node->_typedef.name = NULL;
        break;
    case N_CONTINUE:
        node->_continue.loop = NULL;
        break;
    case N_BREAK:
        node->_break.loop = NULL;
        break;
    case N_INIT_LIST:
        for (int i = 0; i < node->init_list.elements_array.count; i++) {
            free_node(get_node(&node->init_list.elements_array, i));
        }
        array_free(&node->init_list.elements_array);
        break;
    case N_MEMBER_ACCESS:
        free_node(node->member_access.identifier);
        node->member_access.identifier = NULL;
        free_node(node->member_access.member);
        node->member_access.member = NULL;
        break;
    case N_GOTO:
        free_node(node->_goto.identifier);
        node->_goto.identifier = NULL;
        break;
    case N_LABEL:
        free_node(node->label.identifier);
        node->label.identifier = NULL;
        break;
    case N_COMPOUND_LITERAL:
        free_node(node->compound_literal.value);
        node->compound_literal.value = NULL;
        break;
    case N_DESIGNATED_INITIALIZER:
        switch (node->designated_init.kind) {
        case T_ARRAY:
            break;
        case T_STRUCT:
            node->designated_init._struct.name = NULL;
            node->designated_init._struct.member = NULL;
            break;
        case T_UNION:
            node->designated_init._union.name = NULL;
            node->designated_init._union.member = NULL;
            break;
        default:
            break;
        }
        free_node(node->designated_init.value);
        node->designated_init.value = NULL;
        break;
    case N_BUILTIN:
        for (int i = 0; i < node->_builtin.params.count; i++) {
            free_node(get_node(&node->_builtin.params, i));
        }
        array_free(&node->_builtin.params);
        break;
    }
}
