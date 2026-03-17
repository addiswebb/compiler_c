#include "compiler_c/core/arena.h"
#include "compiler_c/core/array.h"
#include "compiler_c/core/type.h"
#include "compiler_c/log/logger.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <compiler_c/core/node.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NodeManager new_node_manager() {
    NodeManager nm;
    arena_init(&nm, NODE_ARENA_SIZE, sizeof(Node));
    return nm;
}

void free_node_manager(NodeManager *nm) { arena_free(nm); }

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
                printf("value= %lld]\n", node->literal.i);
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
        printf(": [name= %s, param_count= %d, type= ", node->func.name, node->type->_func.params.count);
        print_type(node->type);

        printf(", has_initializer=");
        if (node->func.is_defined) printf("true");
        else printf("false");

        if (node->type->_func.is_variadic) printf(", variadic");

        if (node->func.storage_class == EXTERN) printf(", extern");
        if (node->func.storage_class == STATIC) printf(", static");
        printf("]");
        if (node->type->_func.params.count > 0) {
            printf(" { ");
            for (int i = 0; i < node->type->_func.params.count; i++) {
                print_param_decl((ParamDecl *)get(&node->type->_func.params, i));
            }
            if (node->type->_func.is_variadic) printf(", ... ");
            printf("}");
        }
        printf("\n");
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
        if (node->_if.if_false != NULL) {
            print_node(node->_if.if_false, depth + 1);
        }
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
        if (node->designated_init.kind == T_ARRAY) printf(": [index= %d, type= ", node->designated_init._array.index);
        else printf(": [name= %s, type= ", node->designated_init._struct.name);
        print_type(node->type);
        printf("]\n");
        print_node(node->designated_init.value, depth + 1);
        break;
    }
}

/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm) { print_node(arena_get(nm, 0), 0); }
