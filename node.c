#include "tokenizer.c"
#include <stdlib.h>

typedef enum{
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

struct Node{
    NodeType type;
    union{
        struct{
            struct Node **declarations;
            int capacity;
            int count;
        } translation_unit;
        struct{
            char* name;
            int param_count;
            struct Node **params;
            TokenType return_type;
            struct Node *body;
        } function;
        struct{
            struct Node **statements;
            int capacity;
            int count;
        } compound;
        struct{
            struct Node *lhs;
            struct Node *rhs;
            TokenType op;
        }binary;
        struct {
            struct Node *expr;
        } _return;
        struct{
            TokenType type;
            union{
                int i;
                float f;
            };
        }literal;
        struct{
            char* name;
        }identifer;
        struct{
            char* name;
            TokenType type;
            struct Node *expr;
        } var_decl;
    };
};

typedef struct{
    int count;
    int capacity;
    Node* nodes;
} NodeManager;

static NodeManager node_manager;

void init_node_manager(){
    node_manager.nodes = malloc(sizeof(Node) * 8);
    node_manager.capacity = 8;
    node_manager.count = 0;
}

void free_node_manager(){
    for(int i = 0 ; i < node_manager.count; i ++){
        Node* node = &node_manager.nodes[i];
        if (node->type == N_TRANSLATION_UNIT){
            free(node->translation_unit.declarations);
        } else if (node->type == N_COMPOUND){
            free(node->compound.statements);
        } else if (node->type == N_FUNCTION){
            free(node->function.params);
        }
    }
    free(node_manager.nodes);
}
/*
    Handles creating a Node, pushing it to the global node array
*/
Node* new_node(NodeType type){
    if(node_manager.count >= node_manager.capacity){
        // Resize Node Manager Array
        int new_capacity = node_manager.capacity * 2;
        Node* new_nodes = realloc(node_manager.nodes, sizeof(Node) * new_capacity);
        if(!new_nodes) {
            printf("Failed to reallocate token array\n");
            exit(1);
        }
        node_manager.capacity = new_capacity;
        node_manager.nodes = new_nodes;
    }
    Node* node = &node_manager.nodes[node_manager.count++];
    memset(node, 0, sizeof(Node));
    node->type = type;
    return node;
}

void print_node_type(NodeType type){
    switch(type){
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

void print_node(Node* node){
    printf("Node {\n");
    printf("\ttype: ");
    print_node_type(node->type);
    printf(",\n");
    switch(node->type){
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
            if (node->var_decl.expr != NULL){
                printf(",\n");
                switch(node->var_decl.expr->literal.type){
                    case TK_INT_LITERAL:
                        printf("\tvalue: %d",node->var_decl.expr->literal.i);
                        break;
                    case TK_FLT_LITERAL:
                        printf("\tvalue: %g",node->var_decl.expr->literal.f);
                        break;
                    default:
                        break;
                }
            }
            break;
        case N_LITERAL:
            switch(node->literal.type){
                case TK_INT_LITERAL:
                    printf("\tvalue: %d",node->literal.i);
                    break;
                case TK_FLT_LITERAL:
                    printf("\tvalue: %g",node->literal.f);
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
        default:
            printf("\t");
            printf("[Unimplemented]");
            break;
    }
    printf("\n}\n");
}
