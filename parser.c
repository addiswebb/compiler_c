#include <math.h>
#include <stdlib.h>

#include "node.c"

#define MAX_STATEMENTS_PER_BLOCK 4
typedef struct{
    int index;
    int size;
    TokenArray* src;
} Parser;

static Parser parser;

void p_init(){
    parser.size = tokenizer.tokens.size;
    parser.index = 0;
    parser.src = &tokenizer.tokens;
    init_node_manager();
}

void p_free(){
    free_node_manager();
}

/*
Is End of token array?
*/
bool p_is_last_token(){
    return parser.index >= parser.size;
}

Token* p_peek_n(int n){
    if (parser.index + n > parser.src->size){
        printf("P_peek_n Tried peeking past eota\n");
        return NULL;
    }else{
        return &parser.src->data[parser.index+n];
    }
}
Token* p_peek(){
    return p_peek_n(0);
}
Token* p_peek_next(){
    return p_peek_n(1);
}

Token* p_consume_n(int n){
    if (parser.index + n > parser.src->size){
        printf("P_consume_n Reached the end of the token list\n");
        return NULL;
    }else{
        Token* token = &parser.src->data[parser.index];
        parser.index+=n;
        return token;
    }
}

Token* p_consume(){
    return p_consume_n(1);
}

void p_skip_n(int n){
    p_consume_n(n);
}
void p_skip(){
    p_consume_n(1);
}


/*
    Error on type mismatch
*/
void p_expect(TokenType expected_type){
    if (!p_is_last_token()){
        TokenType token_type = parser.src->data[parser.index].type;
        if(token_type != expected_type){
            printf("Expected \'");
            print_token_type(expected_type);
            printf("\' got \'");
            print_token_type(token_type);
            printf("\'\n");
            exit(1);
        }
    }
}

Token* p_consume_a(TokenType type){
    p_expect(type);
    return p_consume();
}
Node* init_translation_unit(){
    Node* node = new_node(N_TRANSLATION_UNIT);
    node->translation_unit.declarations = malloc(sizeof(*node->translation_unit.declarations) * MAX_STATEMENTS_PER_BLOCK);
    if (node->translation_unit.declarations == NULL){
        printf("Failed to initialize translation unit");
        exit(1);
    }
    node->translation_unit.capacity = MAX_STATEMENTS_PER_BLOCK;
    node->translation_unit.count = 0;
    return node;
}

Node* new_compound_node(){
    Node* node = new_node(N_COMPOUND);
    node->compound.statements = malloc(sizeof(*node->compound.statements) * MAX_STATEMENTS_PER_BLOCK);
    if (node->translation_unit.declarations == NULL){
        printf("Failed to create new compound node");
        exit(1);
    }
    node->compound.capacity = MAX_STATEMENTS_PER_BLOCK;
    node->compound.count = 0;
    return node;
}

Node *p_parse_expression(){
    Node* primary = NULL;
    switch(p_peek()->type){
        case TK_INT_LITERAL:
            primary = new_node(N_LITERAL);
            primary->literal.type = p_peek()->type;
            primary->literal.i = atoi(p_consume()->value);
            break;
        case TK_FLT_LITERAL:
            primary = new_node(N_LITERAL);
            primary->literal.type = p_peek()->type;
            primary->literal.f = atof(p_consume()->value);
            break;
        case TK_IDENTIFIER:
            primary = new_node(N_IDENTIFIER);
            primary->identifer.name = p_consume()->value;
            break;
        default:
            printf("Expected expression got \'");
            print_token_type(p_peek()->type);
            printf("\'\n");
            exit(1);
    }
    while(
        is_arithmetic(p_peek()->type) &&
       !p_is_last_token()             //&&
    ){
        Node* binary = new_node(N_BINARY);
        binary->binary.lhs = primary;
        binary->binary.op = p_consume()->type; // Arithmetic operator
        binary->binary.rhs = NULL;
        switch(p_peek()->type){
            case TK_INT_LITERAL:
            case TK_FLT_LITERAL:
            case TK_IDENTIFIER:
                binary->binary.rhs = p_parse_expression();
                break;
            default:
                printf("Expected expression after ");
                print_token_type(binary->binary.op);
                printf(", found ");
                print_token_type(p_peek()->type);
                exit(1);
        }
        primary = binary;
    }

    return primary;
}

/*
    Consumes
    `(type) identifier = [= expr]?;`
    Where [= expr] is optional
*/
Node *p_parse_var_declaration(){
    Node* node = new_node(N_VAR_DECL);
    node->var_decl.type = p_consume()->type;
    p_expect(TK_IDENTIFIER);
    node->var_decl.name = p_consume()->value;
    if (p_peek()->type == TK_EQ){
        p_consume();
        node->var_decl.expr = p_parse_expression();
    }else{
        node->var_decl.expr = NULL;
    }
    p_consume_a(TK_SEMI);
    return node;
}

/*
    Appends a declaration to the given translation unit,
    Resizes its declaration array if necessary.
*/
void p_append_declaration(Node* root, Node* decl){
    if(root->translation_unit.count >= root->translation_unit.capacity){
        int new_capacity = root->translation_unit.capacity * 2;
        Node** new_decls = realloc(root->translation_unit.declarations, sizeof(Node) * new_capacity);
        if(!new_decls){
            printf("Failed to append declaration");
            exit(1);
        }
        root->translation_unit.capacity = new_capacity;
        root->translation_unit.declarations = new_decls;
    }

    root->translation_unit.declarations[root->translation_unit.count++] = decl;
}

/*
    Appends a statement to the given compound node,
    Resizes its statement array if necessary.
*/
void p_append_statement(Node* root, Node* stmt){
    if(root->compound.count >= root->compound.capacity){
        int new_capacity = root->compound.capacity * 2;
        Node** new_decls = realloc(root->compound.statements, sizeof(Node) * new_capacity);
        if(!new_decls){
            printf("Failed to append declaration");
            exit(1);
        }
        root->compound.capacity = new_capacity;
        root->compound.statements= new_decls;
    }
    if (stmt != NULL){
        root->compound.statements[root->compound.count++] = stmt;
    }else{
        printf("Skipping empty node");
    }
}

/*
    Consumes
    `return [expr]?;
    Where [expr] is optional.
*/
Node *p_parse_return(){
    Node* node = new_node(N_RETURN);
    p_consume(); // -> return
    node->_return.expr = p_parse_expression();
    p_consume_a(TK_SEMI);
    return node;
}

/*
    Consumes any of,
    `(type) identifier = [= expr]?;`
    `return [expr]?`
    `[expr];`

    Never conumes `;`, other functions must consume it.
*/
Node *p_parse_statement(){
    Node* node = NULL;
    switch(p_peek()->type){
        case TK_INT:
        case TK_FLOAT:
            return p_parse_var_declaration();
        case TK_RETURN:
            return p_parse_return();
        default:
            return p_parse_expression();
    }
    return node;
}

/*
    Consumes
    `{[statement]*}`
    Where any amount of statements is allowed including zero.
*/
Node *p_parse_compound(){
    Node* node = new_compound_node();
    p_consume_a(TK_OPEN_CURLY);
    while(p_peek()->type != TK_CLOSE_CURLY && !p_is_last_token()){
        p_append_statement(node, p_parse_statement());
    }
    p_consume_a(TK_CLOSE_CURLY);
    return node;
}

/*
    Consumes
    `(type) identifier ([var decl]*) {[statement]*}`

    () contains any amount of var declarations, including zero,
    and {} contains any amount of statements, including zero.
*/
Node *p_parse_function(){
    Node* node = new_node(N_FUNCTION);
    node->function.return_type = p_consume()->type;
    node->function.name = p_consume()->value;
    p_consume_a(TK_OPEN_PAREN);
    while(p_peek()->type != TK_CLOSE_PAREN && !p_is_last_token()){
        // Skip all params for now...
        p_skip();
    }
    p_consume_a(TK_CLOSE_PAREN);
    node->function.param_count = 0;
    node->function.params = NULL;
    node->function.body = p_parse_compound();
    return node;
}


bool is_function_ahead(){
    return (
        (p_peek()->type == TK_INT || p_peek()->type == TK_FLOAT) &&
        p_peek_n(1)->type == TK_IDENTIFIER &&
        p_peek_n(2)->type == TK_OPEN_PAREN
    );
}

Node *p_parse_declaration(){
    if(is_function_ahead()){
        return p_parse_function();
    }else{
        return p_parse_var_declaration();
    }
}



Node* p_parse_translation_unit(){
    Node* root = init_translation_unit();

    if(parser.size == 0){
        printf("The token array is empty,\n Don't forget to initialize the parser after tokenization.");
        exit(1);
    }

    while(!p_is_last_token()){
        p_append_declaration(root, p_parse_declaration());
    }
    return root;
}

void print_nodes(){
    for(int i = 0; i < node_manager.count; i ++){
        print_node(&node_manager.nodes[i]);
    }
}
