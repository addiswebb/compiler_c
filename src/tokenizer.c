#include <compiler_c/tokenizer.h>
#include <compiler_c/util.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *KEYWORDS[KEYWORDS_N] = {"break", "case", "char", "continue", "double", "else",  "enum",   "exit",   "float", "for",
                                    "if",    "int",  "long", "return",   "sizeof", "short", "struct", "switch", "void",  "while"};

static void t_buffer_reset(Tokenizer *tk) {
    tk->buf.size = 0;
    memset(tk->buf.buf, 0, sizeof(tk->buf.buf));
}
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

void t_print_tokens(const Tokenizer *tk) {
    for (int i = 0; i < tk->tokens.size; i++) {
        print_token(&tk->tokens.data[i]);
    }
}
Tokenizer t_new_tokenizer(const char *src, const int src_size) {
    Tokenizer tokenizer;
    tokenizer.index = 0;
    tokenizer.size = src_size;
    tokenizer.src = src;

    t_buffer_reset(&tokenizer);
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
static char t_peek(const Tokenizer *tk) {
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

static char t_peek_n(const Tokenizer *tk, int n) {
    if (tk->index + n > tk->size) {
        printf("t_peek_n Tried peeking past eof\n");
        return '\0';
    }
    return tk->src[tk->index + n];
}

static void t_consume_n(Tokenizer *tk, int n) {
    if (tk->index + n > tk->size) {
        printf("T_Consume Reached the end of the file");
        exit(1);
    } else {
        for (int i = 0; i < n; i++) {
            tk->buf.buf[tk->buf.size++] = tk->src[tk->index++];
        }
    }
}
/*
    Append the current char to buffer and step forward
*/
static void t_consume(Tokenizer *tk) { t_consume_n(tk, 1); }

static void t_skip_n(Tokenizer *tk, int n) {
    if (tk->index + n > tk->size) {
        printf("T_Skip Reached end of the file");
        exit(1);
    } else {
        tk->index += n;
    }
}
static void t_skip(Tokenizer *tk) { t_skip_n(tk, 1); }

static void t_push_buffer(Tokenizer *tk, const TokenType type) {
    if (tk->buf.size == 0) {
        printf("Tried to push an empty buffer to TokenArray, skipping.\n");
        return;
    }
    char *buf_dupe = malloc(sizeof(char) * tk->buf.size);
    if (!buf_dupe) {
        printf("Failed to allocate for buffer duplicate\n");
        exit(1);
    }
    memcpy(buf_dupe, tk->buf.buf, sizeof(char) * tk->buf.size);
    ta_push(&tk->tokens, (Token){type, buf_dupe, tk->buf.size});
    if (DEBUG_TOKENIZER) {
        printf("Buf: %.10s\n", tk->buf.buf);
    }
    t_buffer_reset(tk);
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

static int is_op_start(char c) {
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case '!':
    case '~':
    case '<':
    case '>':
    case '|':
    case '&':
    case '^':
    case '.':
        return 1;
    default:
        return 0;
    }
}

static TokenMatch t_match_operator(Tokenizer *tk) {
    const char next = t_peek_next(tk);
    const int eq = next == '=';
    switch (t_peek(tk)) {
    case '+':
        if (next == '+') return (TokenMatch){TK_INCR, 2};
        return eq ? (TokenMatch){TK_PLUS_EQ, 2} : (TokenMatch){TK_PLUS, 1};
    case '-':
        if (next == '-') return (TokenMatch){TK_DECR, 2};
        if (next == '>') return (TokenMatch){TK_ARROW, 2};
        return eq ? (TokenMatch){TK_MINUS_EQ, 2} : (TokenMatch){TK_MINUS, 1};
    case '*':
        return eq ? (TokenMatch){TK_MULTIPLY_EQ, 2} : (TokenMatch){TK_MULTIPLY, 1};
    case '/':
        return eq ? (TokenMatch){TK_DIVIDE_EQ, 2} : (TokenMatch){TK_DIVIDE, 1};
    case '=':
        return eq ? (TokenMatch){TK_EQ_EQ, 2} : (TokenMatch){TK_EQ, 1};
    case '^':
        return eq ? (TokenMatch){TK_XOR_EQ, 2} : (TokenMatch){TK_XOR, 1};
    case '%':
        return eq ? (TokenMatch){TK_MOD_EQ, 2} : (TokenMatch){TK_MOD, 1};
    case '!':
        return eq ? (TokenMatch){TK_NEQ, 2} : (TokenMatch){TK_L_NOT, 1};
    case '~':
        return (TokenMatch){TK_BW_NOT, 1};
    case '&':
        return next == '&' ? (TokenMatch){TK_AND_AND, 2} : (eq) ? (TokenMatch){TK_AND_EQ, 2} : (TokenMatch){TK_AND, 1};
    case '|':
        return next == '|' ? (TokenMatch){TK_OR_OR, 2} : (eq) ? (TokenMatch){TK_OR_EQ, 2} : (TokenMatch){TK_OR, 1};
    case '<':
        if (next == '<') {
            return t_peek_n(tk, 2) == '=' ? (TokenMatch){TK_SHL_EQ, 3} : (TokenMatch){TK_SHL, 2};
        }
        return eq ? (TokenMatch){TK_LE, 2} : (TokenMatch){TK_LT, 1};
    case '>':
        if (next == '>') {
            return t_peek_n(tk, 2) == '=' ? (TokenMatch){TK_SHR_EQ, 3} : (TokenMatch){TK_SHR, 2};
        }
        return eq ? (TokenMatch){TK_GE, 2} : (TokenMatch){TK_GT, 1};
    case '.':
        return (TokenMatch){TK_DOT, 1};
    default:
        printf("Unknown operator");
        exit(1);
    }
}

static void t_consume_operator(Tokenizer *tk) {
    TokenMatch m = t_match_operator(tk);
    t_consume_n(tk, m.n_chars);
    t_push_buffer(tk, m.type);
}

static void t_consume_special_char(Tokenizer *tk) {
    TokenType type;
    switch (t_peek(tk)) {
    case '(':
        type = TK_OPEN_PAREN;
        break;
    case ')':
        type = TK_CLOSE_PAREN;
        break;
    case '{':
        type = TK_OPEN_CURLY;
        break;
    case '}':
        type = TK_CLOSE_CURLY;
        break;
    case '[':
        type = TK_OPEN_SQUARE;
        break;
    case ']':
        type = TK_CLOSE_SQUARE;
        break;
    case ';':
        type = TK_SEMI;
        break;
    case ',':
        type = TK_COMMA;
        break;
    case ':':
        type = TK_COLON;
        break;
    default:
        printf("Unexpected \'%c\'\n", t_peek(tk));
        exit(1);
        return;
    }
    t_consume(tk);
    t_push_buffer(tk, type);
}
static void t_consume_char_literal(Tokenizer *tk) {
    if (t_peek_n(tk, 2) == '\'') {
        t_skip(tk);
        t_consume(tk);
        t_skip(tk);
        t_push_buffer(tk, TK_CHAR_LITERAL);
    } else {
        printf("Expected single character char 'c' \n");
        exit(1);
    }
}

static void t_consume_string_literal(Tokenizer *tk) {
    t_skip(tk); // "
    while (t_peek(tk) != '\"') {
        t_consume(tk);
    }
    t_skip(tk); // "
    t_push_buffer(tk, TK_STRING_LITERAL);
}

void t_tokenize(Tokenizer *tk) {
    while (!t_is_eof(tk)) {
        const char c = t_peek(tk);
        bool starts_with_decimal = c == '.';
        if (c == '.' && !is_digit(t_peek_next(tk))) {
            t_consume(tk);
            t_push_buffer(tk, TK_DOT);
        } else if (is_digit(c) || c == '.') {
            int is_float = 0;
            t_consume(tk);
            if (c == '0') {
                switch (t_peek(tk)) {
                case 'x':
                    t_consume(tk);
                    while (is_hex(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                case 'b':
                    t_consume(tk);
                    while (is_binary(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                case '.':
                    is_float = true;
                    t_consume(tk);
                    while (is_digit(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                default:
                    while (is_oct(t_peek(tk))) {
                        t_consume(tk);
                    }
                    break;
                }
            } else {
                while (is_digit(t_peek(tk))) {
                    t_consume(tk);
                }
                if (t_peek(tk) == '.') {
                    t_consume(tk);
                    is_float = true;
                    while (is_digit(t_peek(tk))) {
                        t_consume(tk);
                    }
                }
            }
            t_push_buffer(tk, is_float ? TK_FLT_LITERAL : TK_INT_LITERAL);
        } else if (is_alpha(c)) {
            t_consume(tk);
            while (is_alpha_num(t_peek(tk))) {
                t_consume(tk);
            }
            t_parse_and_push_buffer(tk);
            t_buffer_reset(tk);
        } else if (is_whitespace(c)) {
            t_skip(tk);
        } else if (t_peek(tk) == '/' && (t_peek_next(tk) == '/' || t_peek_next(tk) == '*')) {
            t_skip_comments(tk);
        } else if (t_peek(tk) == '\'') {
            t_consume_char_literal(tk);
        } else if (t_peek(tk) == '\"') {
            t_consume_string_literal(tk);
        } else if (is_op_start(c)) {
            t_consume_operator(tk);
        } else {
            t_consume_special_char(tk);
        }
    }
}

bool is_type_token(const TokenType tk) {
    switch (tk) {
    case TK_CHAR:
    case TK_SHORT:
    case TK_INT:
    case TK_LONG:
    case TK_FLOAT:
    case TK_DOUBLE:
    case TK_VOID:
    case TK_STRUCT:
    case TK_ENUM:
        return true;
    default:
        return false;
    }
}

bool is_unary_operator(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_MINUS:
    case TK_INCR:
    case TK_DECR:
    case TK_L_NOT:
    case TK_BW_NOT:
    case TK_AND:
    case TK_MULTIPLY:
    case TK_SIZEOF:
        return true;
    default:
        return false;
    }
}

bool is_binary_operator(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_MINUS:
    case TK_MULTIPLY:
    case TK_DIVIDE:
    case TK_XOR:
    case TK_EQ:
    case TK_AND_AND:
    case TK_AND_EQ:
    case TK_EQ_EQ:
    case TK_GE:
    case TK_GT:
    case TK_LE:
    case TK_LT:
    case TK_DIVIDE_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_OR_EQ:
    case TK_OR_OR:
    case TK_NEQ:
    case TK_PLUS_EQ:
    case TK_SHL_EQ:
    case TK_SHR_EQ:
    case TK_XOR_EQ:
    case TK_SHL:
    case TK_SHR:
    case TK_OR:
    case TK_AND:
    case TK_MOD:
    case TK_MOD_EQ:
        return true;
    default:
        return false;
    }
}

bool is_assignment_op(const TokenType type) {
    switch (type) {
    case TK_EQ:
    case TK_PLUS_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE_EQ:
    case TK_MOD_EQ:
    case TK_OR_EQ:
    case TK_AND_EQ:
    case TK_XOR_EQ:
    case TK_SHL_EQ:
    case TK_SHR_EQ:
        return true;
    default:
        return false;
    }
}

bool is_arithmetic_op(const TokenType type) {
    switch (type) {
    case TK_PLUS:
    case TK_PLUS_EQ:
    case TK_MINUS:
    case TK_MINUS_EQ:
    case TK_MULTIPLY:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE:
    case TK_DIVIDE_EQ:
    case TK_MOD:
    case TK_MOD_EQ:
    case TK_INCR:
    case TK_DECR:
        return true;
    default:
        return false;
    }
}
bool is_bitwise_op(const TokenType type) {
    switch (type) {
    case TK_OR:
    case TK_AND:
    case TK_SHL:
    case TK_SHR:
    case TK_XOR:
    case TK_OR_EQ:
    case TK_AND_EQ:
    case TK_SHL_EQ:
    case TK_SHR_EQ:
    case TK_XOR_EQ:
    case TK_BW_NOT:
        return true;
    default:
        return false;
    }
}

bool is_comparison_op(const TokenType type) {
    switch (type) {
    case TK_EQ_EQ:
    case TK_NEQ:
    case TK_GT:
    case TK_GE:
    case TK_LT:
    case TK_LE:
        return true;
    default:
        return false;
    }
}

bool is_logical_op(const TokenType type) {
    switch (type) {
    case TK_AND_AND:
    case TK_EQ_EQ:
    case TK_OR_OR:
    case TK_L_NOT:
        return true;
    default:
        return false;
    }
}

TokenType get_underlying_op(const TokenType type) {
    switch (type) {
    case TK_PLUS_EQ:
        return TK_PLUS;
    case TK_MINUS_EQ:
        return TK_MINUS;
    case TK_MULTIPLY_EQ:
        return TK_MULTIPLY;
    case TK_DIVIDE_EQ:
        return TK_DIVIDE;
    case TK_MOD_EQ:
        return TK_MOD;
    case TK_OR_EQ:
        return TK_OR;
    case TK_AND_EQ:
        return TK_AND;
    case TK_XOR_EQ:
        return TK_XOR;
    case TK_SHL_EQ:
        return TK_SHL;
    case TK_SHR_EQ:
        return TK_SHR;
    case TK_EQ:
        return TK_EQ;
    default:
        printf("Tried to get the underlying operator of a non-eq operator\n");
        print_token_type(type);
        exit(1);
    }
}

int associativity(const TokenType type) {
    switch (type) {
    case TK_MULTIPLY:
    case TK_DIVIDE:
    case TK_MOD:
    case TK_PLUS:
    case TK_MINUS:
    case TK_SHR:
    case TK_SHL:
    case TK_LT:
    case TK_LE:
    case TK_GT:
    case TK_GE:
    case TK_EQ_EQ:
    case TK_NEQ:
    case TK_AND:
    case TK_XOR:
    case TK_OR:
    case TK_AND_AND:
    case TK_OR_OR:
        return LEFT_ASSOCIATIVITY;
    case TK_SHR_EQ:
    case TK_SHL_EQ:
    case TK_AND_EQ:
    case TK_XOR_EQ:
    case TK_OR_EQ:
    case TK_EQ:
    case TK_PLUS_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE_EQ:
    case TK_MOD_EQ:
        return RIGHT_ASSOCIATIVITY;
    default:
        printf("Tried to get the associativity of a token which is not a binary "
               "operator");
        exit(1);
    }
}

int precedence(const TokenType type) {
    switch (type) {
    case TK_EQ:
    case TK_PLUS_EQ:
    case TK_MINUS_EQ:
    case TK_MULTIPLY_EQ:
    case TK_DIVIDE_EQ:
    case TK_MOD_EQ:
        return 0;
    case TK_SHR_EQ:
    case TK_SHL_EQ:
    case TK_AND_EQ:
    case TK_XOR_EQ:
    case TK_OR_EQ:
        return 1;
    case TK_OR_OR:
        return 2;
    case TK_AND_AND:
        return 3;
    case TK_OR:
        return 4;
    case TK_XOR:
        return 5;
    case TK_AND:
        return 6;
    case TK_EQ_EQ:
    case TK_NEQ:
        return 7;
    case TK_LT:
    case TK_LE:
    case TK_GT:
    case TK_GE:
        return 8;
    case TK_SHR:
    case TK_SHL:
        return 9;
    case TK_PLUS:
    case TK_MINUS:
        return 10;
    case TK_MULTIPLY:
    case TK_DIVIDE:
    case TK_MOD:
        return 11;
    default:
        printf("Tried to get the precedence of a token which is not a binary operator");
        print_token_type(type);
        exit(1);
    }
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
    case TK_CHAR_LITERAL:
        printf("Char Literal");
        break;
    case TK_STRING_LITERAL:
        printf("String Literal");
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
    case TK_XOR:
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
    case TK_CHAR:
        printf("Char");
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
    case TK_FOR:
        printf("For");
        break;
    case TK_MOD:
        printf("\'%%\'");
        break;
    case TK_EQ_EQ:
        printf("\'==\'");
        break;
    case TK_PLUS_EQ:
        printf("\'+=\'");
        break;
    case TK_MINUS_EQ:
        printf("\'-=\'");
        break;
    case TK_MULTIPLY_EQ:
        printf("\'*=\'");
        break;
    case TK_DIVIDE_EQ:
        printf("\'/=\'");
        break;
    case TK_MOD_EQ:
        printf("\'%%=\'");
        break;
    case TK_NEQ:
        printf("\'!=\'");
        break;
    case TK_LT:
        printf("\'<\'");
        break;
    case TK_LE:
        printf("\'<=\'");
        break;
    case TK_GT:
        printf("\'>\'");
        break;
    case TK_GE:
        printf("\'>=\'");
        break;
    case TK_SHL:
        printf("\'<<\'");
        break;
    case TK_SHR:
        printf("\'>>\'");
        break;
    case TK_SHL_EQ:
        printf("\'<<=\'");
        break;
    case TK_SHR_EQ:
        printf("\'>>=\'");
        break;
    case TK_AND:
        printf("\'&\'");
        break;
    case TK_AND_AND:
        printf("\'&&\'");
        break;
    case TK_AND_EQ:
        printf("\'&=\'");
        break;
    case TK_OR:
        printf("\'|\'");
        break;
    case TK_OR_OR:
        printf("\'||\'");
        break;
    case TK_OR_EQ:
        printf("\'|=\'");
        break;
    case TK_XOR_EQ:
        printf("\'^=\'");
        break;
    case TK_L_NOT:
        printf("\'!\'");
        break;
    case TK_BW_NOT:
        printf("\'~\'");
        break;
    case TK_INCR:
        printf("\'++\'");
        break;
    case TK_DECR:
        printf("\'--\'");
        break;
    case TK_OPEN_SQUARE:
        printf("\'[\'");
        break;
    case TK_CLOSE_SQUARE:
        printf("\']\'");
        break;
    case TK_DOUBLE:
        printf("double");
        break;
    case TK_LONG:
        printf("long");
        break;
    case TK_SIZEOF:
        printf("sizeof");
        break;
    case TK_SHORT:
        printf("short");
        break;
    case TK_BREAK:
        printf("break");
        break;
    case TK_CONTINUE:
        printf("continue");
        break;
    case TK_STRUCT:
        printf("struct");
        break;
    case TK_DOT:
        printf("\'.\'");
        break;
    case TK_ARROW:
        printf("\'->\'");
        break;
    case TK_ENUM:
        printf("enum");
        break;
    case TK_CASE:
        printf("Case");
        break;
    case TK_SWITCH:
        printf("Switch");
        break;
    case TK_COLON:
        printf("\':\'");
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
