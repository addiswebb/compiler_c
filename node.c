#include "node.h"

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
        if (node->type == N_TRANSLATION_UNIT) {
            for (int j = 0; j < node->translation_unit.count; j++) {
                free(node->translation_unit.declarations[j]);
            }
            free(node->translation_unit.declarations);
        } else if (node->type == N_COMPOUND) {
            for (int j = 0; j < node->compound.count; j++) {
                free(node->compound.statements[j]);
            }
            free(node->compound.statements);
        } else if (node->type == N_FUNCTION && node->function.body->type == N_COMPOUND) {
            for (int j = 0; j < node->function.body->compound.count; j++) {
                free(node->function.body->compound.statements[j]);
            }
            free(node->function.body->compound.statements);
        }
    }
    free(nm->nodes);
}

/*
    Handles creating a Node, pushing it to the global node array
*/
Node *new_node(NodeManager *nm,const NodeType type) {
    if (nm->count >= nm->capacity) {
        // In the future, create a new arena for more nodes and link them.
        printf("Node Arena overflow");
        exit(1);
    }
    Node *node = &nm->nodes[nm->count++];
    memset(node, 0, sizeof(Node));
    node->type = type;
    return node;
}

void print_node_type(const NodeType type) {
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
    default:
        printf("\nTried to print an unknown node type\n");
        exit(1);
    }
}
// Prints a single node
void print_node_flat(const Node *node) {
    printf("Node {\n");
    printf("\ttype: ");
    print_node_type(node->type);
    printf(",\n");
    switch (node->type) {
    case N_TRANSLATION_UNIT:
        printf("\t");
        printf("count: %d", node->translation_unit.count);
        break;
    case N_FUNCTION:
        printf("\tname: %s,\n", node->function.name);
        printf("\tn_params: %d,\n", node->function.param_count);
        printf("\treturn type: ");
        print_token_type(node->function.return_type);
        printf(",\n");
        printf("\tbody: {}");
        break;
    case N_VAR_DECL:
        printf("\tname: %s,\n", node->var_decl.name);
        printf("\tvar_type: ");
        print_token_type(node->var_decl.type);
        if (node->var_decl.expr != NULL) {
            printf(",\n");
            switch (node->var_decl.expr->literal.type) {
            case TK_INT_LITERAL:
                printf("\tvalue: %d", node->var_decl.expr->literal.i);
                break;
            case TK_FLT_LITERAL:
                printf("\tvalue: %g", node->var_decl.expr->literal.f);
                break;
            default:
                break;
            }
        }
        break;
    case N_LITERAL:
        switch (node->literal.type) {
        case TK_INT_LITERAL:
            printf("\tvalue: %d", node->literal.i);
            break;
        case TK_FLT_LITERAL:
            printf("\tvalue: %g", node->literal.f);
            break;
        default:
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
    default:
        printf("\t");
        printf("[Unimplemented]");
        break;
    }
    printf("\n}\n");
}

void print_indent(const int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
}

void print_node(const Node *node,const int depth) {
    print_indent(depth);
    print_node_type(node->type);
    switch (node->type) {
    case N_TRANSLATION_UNIT:
        printf("\n");
        for (int i = 0; i < node->translation_unit.count; i++) {
            print_node(node->translation_unit.declarations[i], depth + 1);
        }
        break;
    case N_COMPOUND:
        printf("\n");
        for (int i = 0; i < node->compound.count; i++) {
            print_node(node->compound.statements[i], depth + 1);
        }
        break;
    case N_BINARY:
        printf(": [op= ");
        print_token_type(node->binary.op);
        printf("]\n");
        print_node(node->binary.lhs, depth + 1);
        print_node(node->binary.rhs, depth + 1);
        break;
    case N_LITERAL:
        printf(": [type= ");
        print_token_type(node->literal.type);
        switch (node->literal.type) {
        case TK_INT_LITERAL:
            printf(", value: %d]\n", node->literal.i);
            break;
        case TK_FLT_LITERAL:
            printf(", value: %g]\n", node->literal.f);
            break;
        default:
            break;
        }
        break;
    case N_FUNCTION:
        printf(": [name= %s, params= %d, return_type= ", node->function.name, node->function.param_count);
        print_token_type(node->function.return_type);
        printf("]\n");
        print_node(node->function.body, depth + 1);
        break;
    case N_VAR_DECL:
        printf(": [type= ");
        print_token_type(node->var_decl.type);
        printf(", name= %s]\n", node->var_decl.name);
        print_node(node->var_decl.expr, depth + 1);
        break;
    case N_RETURN:
        printf("\n");
        print_node(node->_return.expr, depth + 1);
        break;
    case N_IDENTIFIER:
        printf(": [name: %s]\n", node->identifier.name);
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
    default:
        printf("Tried to print an known node type\n");
        exit(1);
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
