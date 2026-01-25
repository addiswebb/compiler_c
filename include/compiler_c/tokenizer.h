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

#define KEYWORDS_N 14
extern const char* KEYWORDS[KEYWORDS_N];

/*
    Adding a Keyword Checklist:
    1. add to TokenType,
    2. KEYWORDS_N ++
    3. add to KEYWORDS array
    4. Update print_token_type
*/
typedef enum {
    // Keywords
    TK_CHAR,
    TK_DOUBLE,
    TK_ELSE,
    TK_EXIT,
    TK_IF,
    TK_INT,
    TK_FOR,
    TK_FLOAT,
    TK_LONG,
    TK_RETURN,
    TK_SIZEOF,
    TK_SHORT,
    TK_VOID,
    TK_WHILE,
    // Unary Operators
    TK_L_NOT, TK_BW_NOT, TK_INCR, TK_DECR,
    // Binary Operators
    TK_EQ, TK_PLUS, TK_MINUS, TK_MULTIPLY, TK_DIVIDE, TK_MOD,
    TK_EQ_EQ, TK_PLUS_EQ, TK_MINUS_EQ, TK_MULTIPLY_EQ, TK_DIVIDE_EQ, TK_MOD_EQ,
    TK_NEQ,
    TK_LT, TK_LE, TK_GT, TK_GE,
    TK_SHL, TK_SHR,
    TK_SHL_EQ, TK_SHR_EQ,
    TK_AND, TK_AND_AND, TK_AND_EQ,
    TK_OR, TK_OR_OR, TK_OR_EQ,
    TK_XOR, TK_XOR_EQ,
    // Semantics
    TK_OPEN_PAREN, TK_CLOSE_PAREN,
    TK_OPEN_CURLY, TK_CLOSE_CURLY,
    TK_OPEN_SQUARE, TK_CLOSE_SQUARE,
    TK_COMMA,
    TK_SEMI,
    // Other
    TK_INT_LITERAL, TK_FLT_LITERAL, TK_CHAR_LITERAL, TK_STRING_LITERAL,
    TK_EXPR,
    TK_IDENTIFIER,
} TokenType;

typedef struct {
    TokenType type;
    char *value;
    int size;
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

typedef struct{
    TokenType type;
    int n_chars;
}TokenMatch;

void ta_init(TokenArray *arr);
void print_token_type(TokenType type);

void print_token(const Token *token);

bool is_type_token(const TokenType tk);
bool is_unary_operator(const TokenType type);
bool is_binary_operator(const TokenType type);
bool is_assignment_op(const TokenType type);
bool is_arithmetic_op(const TokenType type);
bool is_bitwise_op(const TokenType type);
bool is_comparison_op(const TokenType type);
bool is_logical_op(const TokenType type);
TokenType get_underlying_op(const TokenType type);

int associativity(TokenType type);
int precedence(TokenType type);
void t_print_tokens(const Tokenizer *tk);
Tokenizer t_new_tokenizer(const char *src, int src_size);
void t_free(Tokenizer *tokenizer);

void t_tokenize(Tokenizer *tk);
#endif // COMPILER_C_TOKENIZER_H
