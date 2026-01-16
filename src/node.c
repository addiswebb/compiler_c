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
Node *new_node(NodeManager *nm, const NodeKind type) {
    if (nm->count >= nm->capacity) {
        // In the future, create a new arena for more nodes and link them.
        printf("Node Arena overflow");
        exit(1);
    }
    Node *node = &nm->nodes[nm->count++];
    memset(node, 0, sizeof(Node));
    node->kind = type;
    return node;
}
Node *cast_node(NodeManager *nm, Node *node, Type *type) {
    Node *cast = new_node(nm, N_CAST);
    cast->type = type;
    cast->cast.from = node->type;
    cast->cast.expr = node;
    return cast;
}

bool is_valid_cast(Type *from, Type *to) { return true; }

void print_type(Type *type) {
    if (!type) {
        printf("NULL");
    }
    switch (type->kind) {
    case T_INVALID:
        printf("[INVALID TYPE]");
        break;
    case T_INT:
        printf("int");
        break;
    case T_FLOAT:
        printf("float");
        break;
    case T_CHAR:
        printf("char");
        break;
    case T_POINTER:
        printf("*");
        print_type(type->ptr_to);
        break;
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
// Prints a single node
void print_node_flat(const Node *node) {
    printf("Node {\n");
    printf("\ttype: ");
    print_type(node->type);
    printf(",\n");
    switch (node->kind) {
    case N_TRANSLATION_UNIT:
        printf("\t");
        printf("count: %d", node->translation_unit.count);
        break;
    case N_FUNCTION:
        printf("\tname: %s,\n", node->func.name);
        printf("\tn_params: %d,\n", node->func.param_count);
        printf("\treturn type: ");
        print_type(node->type);
        printf(",\n");
        printf("\tbody: {}");
        break;
    case N_VAR_DECL:
        printf("\tname: %s,\n", node->var_decl.name);
        printf("\tvar_type: ");
        print_type(node->type);
        if (node->var_decl.expr != NULL) {
            printf(",\n");
            switch (node->var_decl.expr->type->kind) {
            case T_INT:
                printf("\tvalue: %d", node->var_decl.expr->literal.i);
                break;
            case T_FLOAT:
                printf("\tvalue: %g", node->var_decl.expr->literal.f);
                break;
            case T_CHAR:
                printf("\tvalue: \'%c\'", node->var_decl.expr->literal.c);
                break;
            case T_POINTER:
                printf("\tvalue: \"%s\"", node->var_decl.expr->literal.s);
                break;
            case T_INVALID:
                printf("\tvalue: INVALID TYPE");
                break;
            }
        }
        break;
    case N_LITERAL:
        switch (node->type->kind) {
        case T_INT:
            printf("\tvalue: %d", node->literal.i);
            break;
        case T_FLOAT:
            printf("\tvalue: %g", node->literal.f);
            break;
        case T_CHAR:
            printf("\tvalue: %c", node->literal.c);
            break;
        case T_INVALID:
            printf("\tvalue: %s", node->literal.s);
            break;
        case T_POINTER:
            break;
        }
        break;
    case N_BINARY:
        printf("\top: ");
        print_token_type(node->binary.op);
        break;
    case N_COMPOUND:
        printf("\tn_statements: %d,\n", node->compound.count);
        break;
    case N_RETURN:
        break;
    case N_IDENTIFIER:
        printf("\tname: %s\n", node->identifier.name);
        break;
    case N_FUNCTION_CALL:
        printf("\tname: %s\n", node->func_call.identifier->identifier.name);
        printf("\tparam count: %d\n", node->func_call.param_count);
        break;
    case N_IF:
    case N_WHILE:
    case N_FOR:
        break;
    case N_UNARY:
        printf("\tunary: ");
        node->unary.associativity ? print_token_type(node->unary.op) : printf(" expr ");
        !node->unary.associativity ? print_token_type(node->unary.op) : printf(" expr ");
        printf("\n");
        break;
    case N_CAST:
        printf("\tcast\n");
        break;
    case N_INDEX:
        printf("\tindex\n");
        break;
    }
    printf("\n}\n");
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
        switch (node->type->kind) {
        case T_INT:
            printf(", value: %d]\n", node->literal.i);
            break;
        case T_FLOAT:
            printf(", value: %g]\n", node->literal.f);
            break;
        case T_CHAR:
            printf(", value: %c]\n", node->literal.c);
            break;
        case T_INVALID:
            printf(", INVALID TYPE]\n");
            break;
        default:
            if (node->type == get_pointer_type(type_char)) printf(", value: \"%s\"]\n", node->literal.s);
            else printf(" (dont know how to print this type)]\n");
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
        printf(": [to= ");
        print_type(node->type);
        printf(", from= ");
        print_type(node->cast.from);
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

void print_nodes(NodeManager *nm) {
    for (int i = 0; i < nm->count; i++) {
        print_node_flat(&nm->nodes[i]);
    }
}
/*
    Recursively prints the parse tree starting with the translation unit
*/
void print_ast(const NodeManager *nm) { print_node(&nm->nodes[0], 0); }
