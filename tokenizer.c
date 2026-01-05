#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buffer.c"
#include "util.c"

typedef enum{
    TK_EXIT,
    TK_INT_LITERAL,
    TK_FLT_LITERAL,
    TK_EXPR,
    TK_SEMI,
    TK_PLUS,
    TK_MINUS,
    TK_MULTIPLY,
    TK_DIVIDE,
    TK_OTHER,
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

typedef struct {
    Token *data;
    int size;
    int capacity;
} TokenArray;

void ta_init(TokenArray* arr){
    arr->data = malloc(sizeof(Token) * 8);
    arr->size = 0;
    arr->capacity = 8;
}

int ta_push(TokenArray* arr, Token tk){
    if(arr->size >= arr->capacity){
        // Resize array
        int new_capacity = arr->capacity * 2;
        Token* new_data = realloc(arr->data, sizeof(Token) * new_capacity);
        if(!new_data) return 0;
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    arr->data[arr->size++] = tk;
    return 1;
}

void ta_free(TokenArray* arr){
    for(int i = 0; i < arr->size; i++){
        free(arr->data[i].value);
    }
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

typedef struct {
    char* src;
    int index;
    int size;
    TokenArray tokens;
    Buffer buf;
} Tokenizer;

typedef struct{
    char c;
    bool is_some;
} peekResult;

static Tokenizer tokenizer;

void t_print_token(Token* token){
    printf("Token { Type: ");
    switch(token->type){
        case TK_EXIT:
            printf("Exit");
            break;
        case TK_INT_LITERAL:
            printf("Integer");
            break;
        case TK_FLT_LITERAL:
            printf("Float");
            break;
        case TK_SEMI:
            printf("Semi-colon");
            break;
        case TK_PLUS:
            printf("Plus");
            break;
        case TK_MINUS:
            printf("Minus");
            break;
        case TK_MULTIPLY:
            printf("Multiply");
            break;
        case TK_DIVIDE:
            printf("Divide");
            break;
        case TK_EXPR:
            printf("Expr");
            break;
        case TK_OTHER:
        default:
            printf("Other");
            break;
    }
    if(token->value != NULL){
        // printf(", value: %s ", token->value);
        printf(", value: ");
        if (token->value[0] == '\0'){
            printf("\\0");
        } else if (token->value[0] == '\n'){
            printf("\\n");
        }
        printf("%s ", token->value);
    }
    printf("}\n");
}

void t_print_tokens(){
    for(int i = 0; i < tokenizer.tokens.size; i++){
        t_print_token(&tokenizer.tokens.data[i]);
    }
}
void t_init(char* src, int size){
    tokenizer.src = src;
    tokenizer.index = 0;
    tokenizer.size = size;
    ta_init(&tokenizer.tokens);
}

void t_free(){
    free(tokenizer.src);
    tokenizer.src = NULL;
    tokenizer.index = 0;
    tokenizer.size = 0;
    ta_free(&tokenizer.tokens);
}

/*
Is End of file?
*/
bool t_is_eof(){
    return tokenizer.index >= tokenizer.size;
}

/*
    peek at the current char
*/
char t_peek(){
    if(t_is_eof()){
        printf("T_peek Tried peeking past eof\n");
        return '\0';
    }else{
        return tokenizer.src[tokenizer.index];
    }
}

char t_peek_next(){
    if (tokenizer.index + 1 > tokenizer.size){
        printf("T_peek_Next Tried peeking past eof\n");
        return '\0';
    }else{
        return tokenizer.src[tokenizer.index+1];
    }
}

/*
    Append the current char to buffer and step forward
*/
void t_consume(){
    if (!t_is_eof()){
        tokenizer.buf.buf[tokenizer.buf.size++] = tokenizer.src[tokenizer.index++];
    }else{
        printf("T_Consume Reached the end of the file");
    }
}
void t_skip(){
    if(!t_is_eof()){
        tokenizer.index++;
    }else{
        printf("T_Skip Reached end of the file");
    }
}

void t_buffer_reset(){
    tokenizer.buf.size = 0;
    memset(tokenizer.buf.buf, 0, sizeof(tokenizer.buf.buf));
}
void t_push_buffer(TokenType type){
    if (tokenizer.buf.size == 0){
        printf("EMPTY BUFFER");
        return;
    }
    Token token;
    token.type = type;
    token.value = strdup(tokenizer.buf.buf);
    t_buffer_reset();
    ta_push(&tokenizer.tokens, token);
}
void t_parse_and_push_buffer(){
    if (tokenizer.buf.size == 0){
        return;
    }
    Token token;
    token.value = NULL;

    bool is_int;
    if (strcmp(tokenizer.buf.buf, "exit") == 0){
        token.type = TK_EXIT;
    }else{
        token.type = TK_OTHER;
        token.value = strdup(tokenizer.buf.buf);
    }
    ta_push(&tokenizer.tokens, token);
    return;
}

TokenType char_to_token_type(char c){
    switch (c){
        case ';':
            return TK_SEMI;
        case '+':
            return TK_PLUS;
        case '-':
            return TK_MINUS;
        case '*':
            return TK_MULTIPLY;
        case '/':
            return TK_DIVIDE;
        default:
            return TK_OTHER;
    }
}

// Change to Token* [Array of tokens]
void t_tokenize(){
    // Loop until eof
    while(!t_is_eof()){
        char c = t_peek();
        if (is_digit(c)){
            t_consume();
            while(is_digit(t_peek())){
                t_consume();
            }
            if(t_peek() == '.' && is_digit(t_peek_next())){
                t_consume();
                while(is_digit(t_peek())){
                    t_consume();
                }
                t_push_buffer(TK_FLT_LITERAL);
            }else{
                t_push_buffer(TK_INT_LITERAL);
            }
        }else if (is_alpha(c)){
            t_consume();
            while(is_alpha_num(t_peek())){
                t_consume();
            }
            t_parse_and_push_buffer();
            t_buffer_reset();
        }else if(is_whitespace(c)){
            t_skip();
        }else{
            // Handle special cases
            t_consume();
            t_push_buffer(char_to_token_type(c));
        }
    }
}
