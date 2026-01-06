#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buffer.c"
#include "util.c"

/*
    Adding a Keyword Checklist
    1. add to TokenType,
    2. KEYWORDS_N ++
    3. add to KEYWORDS array
    4. Update print_token_type
*/
typedef enum{
    // Keywords
    TK_EXIT,
    TK_INT,
    TK_FLOAT,
    TK_RETURN,
    TK_VOID,
    // Special Characters
    TK_EQ,
    TK_SEMI,
    TK_PLUS,
    TK_MINUS,
    TK_MULTIPLY,
    TK_DIVIDE,
    TK_OPEN_PAREN,
    TK_CLOSE_PAREN,
    TK_OPEN_CURLY,
    TK_CLOSE_CURLY,
    TK_COMMA,
    // Other
    TK_INT_LITERAL,
    TK_FLT_LITERAL,
    TK_EXPR,
    TK_IDENTIFIER,
} TokenType;

#define KEYWORDS_N 5
const char* KEYWORDS[KEYWORDS_N] = {
    "exit",
    "int",
    "float",
    "return",
    "void",
};

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
        if(!new_data) {
            printf("Failed to reallocate token array\n");
            exit(1);
        }
        arr->capacity = new_capacity;
        arr->data = new_data;
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
    const char* src;
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

void print_token_type(TokenType type){
    switch(type){
        case TK_EXIT:
            printf("Exit");
            break;
        case TK_INT_LITERAL:
            printf("Integer Literal");
            break;
        case TK_FLT_LITERAL:
            printf("Float Literal");
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
        case TK_EQ:
            printf("Equal");
            break;
        case TK_INT:
            printf("Integer");
            break;
        case TK_FLOAT:
            printf("Float");
            break;
        case TK_VOID:
            printf("Void");
            break;
        case TK_OPEN_PAREN:
            printf("Open Parenthesis");
            break;
        case TK_CLOSE_PAREN:
            printf("Close Parenthesis");
            break;
        case TK_OPEN_CURLY:
            printf("Open Curly");
            break;
        case TK_CLOSE_CURLY:
            printf("Close Curly");
            break;
        case TK_COMMA:
            printf("Comma");
            break;
        case TK_RETURN:
            printf("Return");
            break;
        case TK_IDENTIFIER:
            printf("Identifier");
            break;
        default:
            printf("Undefined");
            break;
    }
}

void print_token(Token* token){
    printf("Token { Type: ");

    print_token_type(token->type);

    if(token->value != NULL){
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
        print_token(&tokenizer.tokens.data[i]);
    }
}
void t_init(char* src, int size){
    tokenizer.src = src;
    tokenizer.index = 0;
    tokenizer.size = size;
    ta_init(&tokenizer.tokens);
}

void t_free(){
    free((void*)tokenizer.src);
    tokenizer.src = NULL;
    tokenizer.index = 0;
    tokenizer.size = 0;
    ta_free(&tokenizer.tokens);
}

/*
Is End of file?
*/
static bool t_is_eof(){
    return tokenizer.index >= tokenizer.size;
}

/*
    peek at the current char
*/
static char t_peek(){
    if(!t_is_eof()){
        return tokenizer.src[tokenizer.index];
    }else{
        printf("T_peek Tried peeking past eof\n");
        return '\0';
    }
}

static char t_peek_next(){
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
static void t_consume(){
    if (!t_is_eof()){
        tokenizer.buf.buf[tokenizer.buf.size++] = tokenizer.src[tokenizer.index++];
    }else{
        printf("T_Consume Reached the end of the file");
    }
}
static void t_skip(){
    if(!t_is_eof()){
        tokenizer.index++;
    }else{
        printf("T_Skip Reached end of the file");
    }
}

static void t_buffer_reset(){
    tokenizer.buf.size = 0;
    memset(tokenizer.buf.buf, 0, sizeof(tokenizer.buf.buf));
}
static void t_push_buffer(TokenType type){
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

static void t_parse_and_push_buffer(){
    if (tokenizer.buf.size == 0){
        return;
    }
    Token token;
    token.value = NULL;

    bool is_keyword = false;
    for(int i = 0; i < KEYWORDS_N; i++){
        if(strcmp(tokenizer.buf.buf, KEYWORDS[i]) == 0){
            token.type = (TokenType) i;
            is_keyword = true;
            break;
        }
    }
    if(!is_keyword){
        token.type = TK_IDENTIFIER;
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
        case '=':
            return TK_EQ;
        case '(':
            return TK_OPEN_PAREN;
        case ')':
            return TK_CLOSE_PAREN;
        case '{':
            return TK_OPEN_CURLY;
        case '}':
            return TK_CLOSE_CURLY;
        case ',':
            return TK_COMMA;
        default:
            return TK_IDENTIFIER;
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
