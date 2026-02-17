#ifndef COMPILER_C_TOKENIZER_H
#define COMPILER_C_TOKENIZER_H

#include "compiler_c/array.h"
#include <stdbool.h>

#define DEBUG_TOKENIZER 0
#define MIN_BINARY_OP_PRECEDENCE 0

#define LEFT_ASSOCIATIVITY 1
#define RIGHT_ASSOCIATIVITY 0

#define BUFFER_SIZE 1024

typedef struct {
    char buf[BUFFER_SIZE];
    int size;
} Buffer;

#define KEYWORDS_N 24

/* All standard C keywords */
extern const char* KEYWORDS[KEYWORDS_N];

/*
    Adding a Keyword Checklist:
    1. add to TokenType,
    2. KEYWORDS_N ++
    3. add to KEYWORDS array
    4. Update print_token_type
    // Keep alphabetical order for KEYWORDS and TokenType
*/

/* Set of all tokens */
typedef enum {
    // Keywords
    TK_BREAK,
    TK_CASE,
    TK_CHAR,
    TK_CONTINUE,
    TK_DEFAULT,
    TK_DOUBLE,
    TK_ELSE,
    TK_ENUM,
    TK_EXIT,
    TK_EXTERN,
    TK_FLOAT,
    TK_FOR,
    TK_IF,
    TK_INT,
    TK_LONG,
    TK_RETURN,
    TK_SIZEOF,
    TK_SHORT,
    TK_STATIC,
    TK_STRUCT,
    TK_SWITCH,
    TK_TYPEDEF,
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
    // Member Access
    TK_DOT, TK_ARROW,
    // Semantics
    TK_OPEN_PAREN, TK_CLOSE_PAREN,
    TK_OPEN_CURLY, TK_CLOSE_CURLY,
    TK_OPEN_SQUARE, TK_CLOSE_SQUARE,
    TK_COMMA,
    TK_SEMI, TK_COLON,
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

/* Dynamic array of tokens used by tokenizer to store tokens */
typedef struct {
    Token *data;
    int size;
    int capacity;
} TokenArray;

/* Converts a given src file to tokens */
typedef struct {
    const char *src;
    int index;
    int size;
    // TokenArray tokens;
    Array tokens_array;
    Buffer buf;
} Tokenizer;

/* Defines number of characters a specific token type is comprised of. Used for correctly parsing +=,/=,-= etc. */
typedef struct{
    TokenType type;
    int n_chars;
}TokenMatch;

/* Creates a new tokenizer for a given source file. Also handles the token array and buffer. */
Tokenizer t_new_tokenizer(const char *src, int src_size);
void t_free(Tokenizer *tokenizer);
/* Converts the tokenizer's source file into tokens in the token array. */
void t_tokenize(Tokenizer *tk);

bool is_unary_operator(const TokenType type);
bool is_binary_operator(const TokenType type);
bool is_assignment_op(const TokenType type);
bool is_arithmetic_op(const TokenType type);
bool is_bitwise_op(const TokenType type);
bool is_comparison_op(const TokenType type);
bool is_logical_op(const TokenType type);

/* Returns the underlying operator of combined operators. E.g `+=` returns `+`, `/=` returns `/`. */
TokenType get_underlying_op(TokenType type);

/*
    Returns the associativity of a given operator token type.
    **Left associativity**: `1 + 2 + 3` => `(1 + 2) + 3`.
    **Right associativity**: `1 + 2 + 3` => `1 + (2 + 3)`.
*/
int op_associativity(TokenType type);
/* Returns the precedence of the given operator token type. */
int op_precedence(TokenType type);

void print_token_type(TokenType type);
void print_token(const Token *token);

static inline Token *get_token(Array *arr, int index) { return (Token *)get(arr, index); }

#endif // COMPILER_C_TOKENIZER_H
