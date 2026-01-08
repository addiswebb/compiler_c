#include "tokenizer.c"
#include <stdlib.h>

typedef enum {
    N_TRANSLATION_UNIT,
    N_FUNCTION,
    N_COMPOUND,
    N_VAR_DECL,
    N_RETURN,
    N_BINARY,
    N_LITERAL,
    N_IDENTIFIER,
} NodeType;

typedef struct Node Node;

struct Node {
    NodeType type;
    union {
        struct {
            struct Node **declarations;
            int capacity;
            int count;
        } translation_unit;
        struct {
            const char *name;
            int param_count;
            struct Node **params;
            TokenType return_type;
            struct Node *body;
        } function;
        struct {
            struct Node **statements;
            int capacity;
            int count;
        } compound;
        struct {
            struct Node *lhs;
            struct Node *rhs;
            TokenType op;
        } binary;
        struct {
            struct Node *expr;
        } _return;
        struct {
            TokenType type;
            union {
                int i;
                float f;
            };
        } literal;
        struct {
            char *name;
        } identifer;
        struct {
            char *name;
            TokenType type;
            struct Node *expr;
        } var_decl;
    };
};

typedef struct {
    int count;
    int capacity;
    Node *nodes;
} NodeManager;

static NodeManager node_manager;

#define NODE_ARRAY_SIZE

void init_node_manager() {
    node_manager.capacity = 1024;
    node_manager.nodes = malloc(sizeof(Node) * node_manager.capacity);
    if (node_manager.nodes == NULL) {
        printf("Failed to allocate node manager array");
        exit(1);
    }
    node_manager.count = 0;
}

void free_node_manager() {
    for (int i = 0; i < node_manager.count; i++) {
        Node *node = &node_manager.nodes[i];
        if (node->type == N_TRANSLATION_UNIT) {
            free(node->translation_unit.declarations);
        } else if (node->type == N_COMPOUND) {
            free(node->compound.statements);
        } else if (node->type == N_FUNCTION) {
            free(node->function.params);
        }
    }
    free(node_manager.nodes);
}
/*
    Handles creating a Node, pushing it to the global node array
*/
Node *new_node(NodeType type) {
    if (node_manager.count >= node_manager.capacity) {
        // In the future, create a new arena for more nodes and link them.
        printf("Node Arena overflow");
        exit(1);
    }
    Node *node = &node_manager.nodes[node_manager.count++];
    memset(node, 0, sizeof(Node));
    node->type = type;
    return node;
}

void print_node_type(NodeType type) {
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
    default:
        printf("Undefined");
        break;
    }
}

void print_node_flat(Node *node) {
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
        printf("\tname: %s\n", node->identifer.name);
        break;
    default:
        printf("\t");
        printf("[Unimplemented]");
        break;
    }
    printf("\n}\n");
}

void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
}

void print_node(Node *node, int depth) {
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
        printf(": [name= %s, params= %d, return_type= ", node->function.name,
               node->function.param_count);
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
        printf(": [name: %s]\n", node->identifer.name);
        break;
    default:
        break;
    }
}

void print_ast() { print_node(&node_manager.nodes[0], 0); }
