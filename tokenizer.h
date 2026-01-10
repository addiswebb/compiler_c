#ifndef COMPILER_C_TOKENIZER_H
#define COMPILER_C_TOKENIZER_H

#include <stdbool.h>

#define MIN_BINARY_OP_PRECEDENCE 0
#define LEFT_ASSOCIATIVITY 1
#define RIGHT_ASSOCIATIVITY 0


#define BUFFER_SIZE 1024

typedef struct {
    char buf[BUFFER_SIZE];
    int size;
} Buffer;

#define KEYWORDS_N 8
extern const char* KEYWORDS[KEYWORDS_N];

/*
    Adding a Keyword Checklist
    1. add to TokenType,
    2. KEYWORDS_N ++
    3. add to KEYWORDS array
    4. Update print_token_type
*/
typedef enum {
    // Keywords
    TK_ELSE,
    TK_EXIT,
    TK_IF,
    TK_INT,
    TK_FLOAT,
    TK_RETURN,
    TK_VOID,
    TK_WHILE,
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
    TK_EXP,
    TK_IDENTIFIER,
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct {
    Token *data;
    int size;
    int capacity;
} TokenArray;

typedef struct {
    const char *src;
    int index;
    int size;
    TokenArray tokens;
    Buffer buf;
} Tokenizer;

void ta_init(TokenArray *arr);
void print_token_type(TokenType type);

void print_token(const Token *token);

bool is_binary_operator(TokenType type);

int associativity(TokenType type);
int precedence(TokenType type);
void t_print_tokens(const Tokenizer *tk);
Tokenizer t_new_tokenizer(const char *src, int src_size);
void t_free(Tokenizer *tokenizer);

TokenType char_to_token_type(char c);
void t_tokenize(Tokenizer *tk);
#endif // COMPILER_C_TOKENIZER_H
