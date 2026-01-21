#include "compiler_c/tokenizer.h"
#include "compiler_c/type.h"
#include <compiler_c/node.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NodeManager new_node_manager() {
    NodeManager nm;
    nm.capacity = NODE_ARENA_SIZE;
    nm.nodes = malloc(sizeof(Node) * nm.capacity);
    if (nm.nodes == NULL) {
        printf("Failed to allocate node manager array");
        exit(1);
    }
    nm.count = 0;
    return nm;
}

void free_node_manager(const NodeManager *nm) {
    for (int i = 0; i < nm->count; i++) {
        const Node *node = &nm->nodes[i];
        if (node->kind == N_TRANSLATION_UNIT) {
            for (int j = 0; j < node->translation_unit.count; j++) {
                free(node->translation_unit.declarations[j]);
            }
            free(node->translation_unit.declarations);
        } else if (node->kind == N_COMPOUND) {
            for (int j = 0; j < node->compound.count; j++) {
                free(node->compound.items[j]);
            }
            free(node->compound.items);
        } else if (node->kind == N_FUNCTION && node->func.body->kind == N_COMPOUND) {
            for (int j = 0; j < node->func.body->compound.count; j++) {
                free(node->func.body->compound.items[j]);
            }
            free(node->func.body->compound.items);
        }
    }
    free(nm->nodes);
}

/*
    Handles creating a Node, pushing it to the global node array
*/
Node *new_node(NodeManager *nm, const NodeKind kind) {
    if (nm->count >= nm->capacity) {
        // In the future, create a new arena for more nodes and link them.
        printf("Node Arena overflow");
        exit(1);
    }
    Node *node = &nm->nodes[nm->count++];
    memset(node, 0, sizeof(Node));
    node->kind = kind;
    node->type = type_invalid;
    return node;
}
Node *cast_node(NodeManager *nm, Node *node, Type *type) {
    if (!is_valid_cast(node->type, type)) {
        printf("Invalid conversion from ");
        print_type(node->type);
        printf(" to ");
        print_type(type);
        printf("\n");
        exit(1);
    }
    Node *cast = new_node(nm, N_CAST);
    cast->type = type;
    cast->cast.from = node->type;
    cast->cast.expr = node;
    return cast;
}

bool is_valid_cast(Type *from, Type *to) {
    if (from->kind == T_ARRAY) {
        // Can only cast array->pointer (pointer decay)
        return to->kind == T_POINTER;
    }
    return true;
}

LiteralKind literal_kind(TokenType type) {
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
        printf("Given Tokentype which is not a literal\n");
        exit(1);
    }
}
void print_type(Type *type) {
    if (!type) {
        printf("NULL");
    }
    switch (type->kind) {
    case T_INVALID:
        printf("[INVALID TYPE]");
        break;
    case T_ARRAY:
        print_type(type->base);
        printf("[%d]", type->array_len);
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
        printf("*");
        print_type(type->base);
        break;
    default:
        printf("Not handling other types in print_type\n");
        exit(1);
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
    }
}

void print_indent(const int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
}

void print_node(const Node *node, const int depth) {
    print_indent(depth);
    print_node_type(node->kind);
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        printf("\n");
        for (int i = 0; i < node->translation_unit.count; i++) {
            print_node(node->translation_unit.declarations[i], depth + 1);
        }
        break;
    case N_COMPOUND:
        printf("\n");
        for (int i = 0; i < node->compound.count; i++) {
            print_node(node->compound.items[i], depth + 1);
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
        if (node->type == type_char) {
            printf("value= %c]\n", (char)node->literal.i);
            break;
        }
        switch (node->type->kind) {
        case T_INT:
            switch (node->type->size) {
            case 1:
                printf("value= \'%c\']\n", (char)node->literal.i);
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
                printf("Given invalid size of int to print node_flat\n");
                exit(1);
            }
            break;
        case T_FLOAT:
            printf("value= %g]\n", node->literal.f);
            break;
        case T_INVALID:
            printf("value= [INVALID TYPE]]\n");
            break;
        case T_ARRAY:
            if (node->type->base == type_char) {
                printf("value= \"");
                for (int i = 0; i < node->type->array_len; i++) {
                    printf("%c", node->literal.s.data[i]);
                }
                printf("\"]\n");
                break;
            }
        case T_POINTER:
            if (node->type->base == type_char) {
                printf("value= \"%s\"]\n", node->literal.s.data);
                break;
            }
        default:
            printf(" (dont know how to print this type)]\n");
            break;
        }
        break;
    case N_FUNCTION:
        printf(": [name= %s, param_count= %d, return_type= ", node->func.name, node->func.param_count);
        print_type(node->type);
        printf("]");
        if (node->func.param_count > 0) {
            printf(" { ");
            for (int i = 0; i < node->func.param_count; i++) {
                printf("int %s ", node->func.params[i]->var_decl.name);
            }
            printf("}");
        }
        printf("\n");
        print_node(node->func.body, depth + 1);
        break;
    case N_VAR_DECL:
        printf(": [type= ");
        print_type(node->type);
        printf(", name= %s]\n", node->var_decl.name);
        print_node(node->var_decl.expr, depth + 1);
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
        printf(": [name: %s, param_count: %d]\n", node->func_call.identifier->identifier.name, node->func_call.param_count);
        for (int i = 0; i < node->func_call.param_count; i++) {
            print_node(node->func_call.params[i], depth + 1);
        }
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
        printf(": [index,indentifier]\n");
        print_node(node->index.index, depth + 1);
        print_node(node->index.identifier, depth + 1);
        break;
    }
}

/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm) { print_node(&nm->nodes[0], 0); }
