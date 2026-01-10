#include "tokenizer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

const char* KEYWORDS[KEYWORDS_N] = {"else", "exit", "if", "int", "float", "return", "void", "while"};

void ta_init(TokenArray *arr) {
    arr->capacity = 16;
    arr->data = malloc(sizeof(Token) * arr->capacity);
    if (!arr->data) {
        printf("Failed to allocate token array\n");
        exit(1);
    }
    arr->size = 0;
}

static int ta_push(TokenArray *arr, const Token tk) {
    if (arr->size >= arr->capacity) {
        // Resize array
        const int new_capacity = arr->capacity * 2;
        Token *new_data = realloc(arr->data, sizeof(Token) * new_capacity);
        if (!new_data) {
            printf("Failed to reallocate token array\n");
            exit(1);
        }
        arr->capacity = new_capacity;
        arr->data = new_data;
    }
    arr->data[arr->size++] = tk;
    return 1;
}

static void ta_free(TokenArray *arr) {
    for (int i = 0; i < arr->size; i++) {
        free(arr->data[i].value);
    }
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}


void print_token_type(const TokenType type) {
    switch (type) {
    case TK_EXIT:
        printf("Exit");
        break;
    case TK_INT_LITERAL:
        printf("Int Literal");
        break;
    case TK_FLT_LITERAL:
        printf("Float Literal");
        break;
    case TK_SEMI:
        printf("\';\'");
        break;
    case TK_PLUS:
        printf("\'+\'");
        break;
    case TK_MINUS:
        printf("\'-\'");
        break;
    case TK_MULTIPLY:
        printf("\'*\'");
        break;
    case TK_DIVIDE:
        printf("\'/\'");
        break;
    case TK_EXP:
        printf("\'^\'");
        break;
    case TK_EXPR:
        printf("Expr");
        break;
    case TK_EQ:
        printf("\'=\'");
        break;
    case TK_INT:
        printf("Int");
        break;
    case TK_FLOAT:
        printf("Float");
        break;
    case TK_VOID:
        printf("void");
        break;
    case TK_OPEN_PAREN:
        printf("\'(\'");
        break;
    case TK_CLOSE_PAREN:
        printf("\')\'");
        break;
    case TK_OPEN_CURLY:
        printf("\'{\'");
        break;
    case TK_CLOSE_CURLY:
        printf("\'}\'");
        break;
    case TK_COMMA:
        printf("\',\'");
        break;
    case TK_RETURN:
        printf("Return");
        break;
    case TK_IDENTIFIER:
        printf("Identifier");
        break;
    case TK_IF:
        printf("If");
        break;
    case TK_ELSE:
        printf("Else");
        break;
    case TK_WHILE:
        printf("While");
        break;
    default:
        printf("Undefined: %d", type);
        break;
    }
}

void print_token(const Token *token) {
    printf("Token { Type: ");
    print_token_type(token->type);
    if (token->value != NULL) {
        printf(", value: ");
        if (token->value[0] == '\0') {
            printf("\\0");
        } else if (token->value[0] == '\n') {
            printf("\\n");
        }
        printf("%s ", token->value);
    }
    printf("}\n");
}

bool is_binary_operator(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_MINUS:
    case TK_MULTIPLY:
    case TK_DIVIDE:
    case TK_EXP:
        return true;
    default:
        return false;
    }
}

int associativity(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_MINUS:
    case TK_MULTIPLY:
    case TK_DIVIDE:
        return LEFT_ASSOCIATIVITY;
    case TK_EXP:
        return RIGHT_ASSOCIATIVITY;
    default:
        printf("Tried to get the associativity of a token which is not a binary "
               "operator");
        exit(1);
    }
}

int precedence(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_MINUS:
        return 0;
    case TK_MULTIPLY:
    case TK_DIVIDE:
        return 1;
    case TK_EXP:
        return 2;
    default:
        print_token_type(type);
        printf("Tried to get the precedence of a token which is not a binary "
               "operator");
        exit(1);
    }
}

void t_print_tokens(const Tokenizer *tk) {
    printf("tokenizer tokens size:%d", tk->tokens.size);
    for (int i = 0; i < tk->tokens.size; i++) {
        print_token(&tk->tokens.data[i]);
    }
}
Tokenizer t_new_tokenizer(const char *src, const int src_size){
    Tokenizer tokenizer;
    tokenizer.index = 0;
    tokenizer.size = src_size;
    tokenizer.src = src;
    tokenizer.buf.size = 0;
    ta_init(&tokenizer.tokens);
    return tokenizer;
}

void t_free(Tokenizer *tokenizer) {
    tokenizer->src = NULL;
    tokenizer->index = 0;
    tokenizer->size = 0;
    ta_free(&tokenizer->tokens);
}

/*
Is End of file?
*/
static bool t_is_eof(const Tokenizer *tk) { return tk->index >= tk->size; }

/*
    peek at the current char
*/
static char t_peek(const Tokenizer*tk) {
    if (!t_is_eof(tk)) {
        return tk->src[tk->index];
    }
    printf("T_peek Tried peeking past eof\n");
    return '\0';
}

static char t_peek_next(const Tokenizer *tk) {
    if (tk->index + 1 > tk->size) {
        printf("t_peek_next Tried peeking past eof\n");
        return '\0';
    }
    return tk->src[tk->index + 1];
}

/*
    Append the current char to buffer and step forward
*/
static void t_consume(Tokenizer *tk) {
    if (!t_is_eof(tk)) {
        tk->buf.buf[tk->buf.size++] = tk->src[tk->index++];
    } else {
        printf("T_Consume Reached the end of the file");
    }
}
static void t_skip(Tokenizer *tk) {
    if (!t_is_eof(tk)) {
        tk->index++;
    } else {
        printf("T_Skip Reached end of the file");
    }
}

static void t_buffer_reset(Tokenizer *tk) {
    tk->buf.size = 0;
    memset(tk->buf.buf, 0, sizeof(tk->buf.buf));
}
static void t_push_buffer(Tokenizer *tk,const TokenType type) {
    if (tk->buf.size == 0) {
        printf("EMPTY BUFFER");
        return;
    }
    const Token token = {type, strdup(tk->buf.buf)};
    t_buffer_reset(tk);
    ta_push(&tk->tokens, token);
}

static void t_parse_and_push_buffer(Tokenizer *tk) {
    if (tk->buf.size == 0) {
        return;
    }
    Token token = {TK_VOID, NULL};

    bool is_keyword = false;
    for (int i = 0; i < KEYWORDS_N; i++) {
        if (strcmp(tk->buf.buf, KEYWORDS[i]) == 0) {
            token.type = (TokenType)i;
            is_keyword = true;
            break;
        }
    }
    if (!is_keyword) {
        token.type = TK_IDENTIFIER;
        token.value = strdup(tk->buf.buf);
    }
    ta_push(&tk->tokens, token);
}

TokenType char_to_token_type(const char c) {
    switch (c) {
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
    case '^':
        return TK_EXP;
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

static void t_skip_comments(Tokenizer *tk) {
    t_skip(tk); // '/'
    // Single line comment
    if (t_peek(tk) == '/') {
        t_skip(tk); // '/'
        while (t_peek(tk) != '\n') {
            t_skip(tk);
        }
        t_skip(tk);                 // '\n'
    } else if (t_peek(tk) == '*') { // Multi-line comments
        t_skip(tk);                 // '*'
        while (t_peek(tk) != '*' && t_peek_next(tk) != '/') {
            t_skip(tk);
        }
        t_skip(tk);
        t_skip(tk);
    }
}

// Change to Token* [Array of tokens]
void t_tokenize(Tokenizer *tk) {
    // Loop until eof
    while (!t_is_eof(tk)) {
        const char c = t_peek(tk);
        if (is_digit(c)) {
            t_consume(tk);
            while (is_digit(t_peek(tk))) {
                t_consume(tk);
            }
            if (t_peek(tk) == '.' && is_digit(t_peek_next(tk))) {
                t_consume(tk);
                while (is_digit(t_peek(tk))) {
                    t_consume(tk);
                }
                t_push_buffer(tk,TK_FLT_LITERAL);
            } else {
                t_push_buffer(tk,TK_INT_LITERAL);
            }
        } else if (is_alpha(c)) {
            t_consume(tk);
            while (is_alpha_num(t_peek(tk))) {
                t_consume(tk);
            }
            t_parse_and_push_buffer(tk);
            t_buffer_reset(tk);
        } else if (is_whitespace(c)) {
            t_skip(tk);
        } else if (t_peek(tk) == '/') {
            t_skip_comments(tk);
        } else {
            // Handle special cases
            t_consume(tk);
            t_push_buffer(tk,char_to_token_type(c));
        }
    }
}
