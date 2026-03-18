#include "compiler_c/tokenize/tokenizer.h"
#include "compiler_c/abi/abi.h"
#include "compiler_c/core/util.h"
#include "compiler_c/log/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *KEYWORDS[KEYWORDS_N] = {
    [TK_AUTO] = "auto",     [TK_BREAK] = "break",       [TK_CASE] = "case",       [TK_CHAR] = "char",
    [TK_CONST] = "const",   [TK_CONTINUE] = "continue", [TK_DEFAULT] = "default", [TK_DO] = "do",
    [TK_DOUBLE] = "double", [TK_ELSE] = "else",         [TK_ENUM] = "enum",       [TK_EXTERN] = "extern",
    [TK_FLOAT] = "float",   [TK_FOR] = "for",           [TK_GOTO] = "goto",       [TK_IF] = "if",
    [TK_INLINE] = "inline", [TK_INT] = "int",           [TK_LONG] = "long",       [TK_REGISTER] = "register",
    [TK_RETURN] = "return", [TK_SHORT] = "short",       [TK_SIGNED] = "signed",   [TK_SIZEOF] = "sizeof",
    [TK_STATIC] = "static", [TK_STRUCT] = "struct",     [TK_SWITCH] = "switch",   [TK_TYPEDEF] = "typedef",
    [TK_UNION] = "union",   [TK_UNSIGNED] = "unsigned", [TK_VOID] = "void",       [TK_VOLATILE] = "volatile",
    [TK_WHILE] = "while",
};

static void t_buffer_reset(Tokenizer *tk) {
    tk->buf.size = 0;
    memset(tk->buf.buf, 0, sizeof(tk->buf.buf));
}

Tokenizer t_new_tokenizer(const char *src, const int src_size) {
    Tokenizer tokenizer;
    tokenizer.index = 0;
    tokenizer.size = src_size;
    tokenizer.src = src;

    t_buffer_reset(&tokenizer);
    tokenizer.buf.size = 0;
    array_init(&tokenizer.tokens_array, 16, sizeof(Token));
    return tokenizer;
}
void free_token(Token *token) {
    free(token->value);
    token->value = NULL;
}

void t_free(Tokenizer *tokenizer) {
    for (int i = 0; i < tokenizer->tokens_array.count; i++) {
        free_token(get_token(&tokenizer->tokens_array, i));
    }
    tokenizer->src = NULL;
    tokenizer->index = 0;
    tokenizer->size = 0;
    array_free(&tokenizer->tokens_array);
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

static char t_peek_n(const Tokenizer *tk, const int n) {
    if (tk->index + n > tk->size) {
        printf("t_peek_n Tried peeking past eof\n");
        return '\0';
    }
    return tk->src[tk->index + n];
}

static char t_peek_next(const Tokenizer *tk) { return t_peek_n(tk, 1); }

static void t_consume_n(Tokenizer *tk, const int n) {
    if (tk->index + n > tk->size) {
        PANIC("T_Consume Reached the end of the file");
    } else {
        for (int i = 0; i < n; i++) {
            if (DEBUG_TOKENIZER) {
                printf("%c", tk->src[tk->index]);
            }
            tk->buf.buf[tk->buf.size++] = tk->src[tk->index++];
        }
    }
}
/*
    Append the current char to buffer and step forward
*/
static void t_consume(Tokenizer *tk) { t_consume_n(tk, 1); }

static void t_consume_a(Tokenizer *tk, const char c) { tk->buf.buf[tk->buf.size++] = c; }

static void t_skip_n(Tokenizer *tk, int n) {
    if (tk->index + n > tk->size) {
        PANIC("T_Skip Reached end of the file");
    } else {
        tk->index += n;
    }
}
static void t_skip(Tokenizer *tk) { t_skip_n(tk, 1); }

static void t_push_buffer(Tokenizer *tk, const TokenType type) {
    if (tk->buf.size == 0) {
        WARN("Tried to push an empty buffer to TokenArray, skipping.\n");
        return;
    }
    // TODO! tk->buf.size+1 or use strdup
    char *buf_dupe = malloc(sizeof(char) * tk->buf.size);
    // buf_dupe[tk->buf.size] ='\0';
    if (!buf_dupe) {
        PANIC("Failed to allocate for buffer duplicate\n");
    }
    memcpy(buf_dupe, tk->buf.buf, sizeof(char) * tk->buf.size);
    append(&tk->tokens_array, &(Token){.type = type, .value = buf_dupe, .size = tk->buf.size});
    t_buffer_reset(tk);
}

static void t_parse_and_push_buffer(Tokenizer *tk) {
    if (tk->buf.size == 0) return;

    Token token = {.type = TK_VOID, .value = NULL, .size = 0};

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
        // TODO! replace with explicit malloc +memcpy
        token.value = strdup(tk->buf.buf);
    }
    append(&tk->tokens_array, &token);
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

static int is_op_start(const char c) {
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

static TokenMatch t_match_operator(const Tokenizer *tk) {
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
        PANIC("Unknown operator");
    }
}

static void t_consume_operator(Tokenizer *tk) {
    const TokenMatch m = t_match_operator(tk);
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
        PANIC("Unexpected '%c'\n", t_peek(tk));
    }
    t_consume(tk);
    t_consume_a(tk, '\0');
    t_push_buffer(tk, type);
}
static char t_parse_escape_sequence(Tokenizer *tk, int *length) {
    t_skip(tk); // '\\'
    char c = t_peek(tk);
    *length = 1;
    switch (c) {
    case 'n':
        return '\n';
    case 't':
        return '\t';
    case 'r':
        return '\r';
    case '\\':
        return '\\';
    case '"':
        return '\"';
    case '\'':
        return '\'';
    case '0':
        return 0;
    // Dont forget to update *length to correctly skip escaped characters.
    case 'x':
        PANIC("Escaped hex sequences are currently unsupported\n");
    default:
        if (is_oct(c)) {
            PANIC("Escaped oct sequences are currently unsupported\n");
        }
        PANIC("Invalid escape sequence\n");
    }
}
static void t_consume_char_literal(Tokenizer *tk) {
    t_skip(tk);
    for (int i = 0; i < 4; i++) {
        if (t_peek(tk) == '\'') break;

        if (t_peek(tk) == '\\') {
            int n = 0;
            t_consume_a(tk, t_parse_escape_sequence(tk, &n));
            t_skip_n(tk, n);
        } else t_consume(tk);
    }
    // Multi literal char might be longer than 4, skip all these characters until \'
    while (t_peek(tk) != '\'') t_skip(tk);
    t_skip(tk);
    t_push_buffer(tk, TK_CHAR_LITERAL);
}

static void t_consume_string_literal(Tokenizer *tk) {
    t_skip(tk); // "
    for (;;) {
        char c = t_peek(tk);
        if (c == '\n') {
            PANIC("Found '\\n' in string literal.");
        }
        if (c == '\"') {
            t_skip(tk); // "
            break;
        }

        if (c == '\\') {
            int n = 0;
            t_consume_a(tk, t_parse_escape_sequence(tk, &n));
            t_skip_n(tk, n);
        } else t_consume(tk);
    }
    t_push_buffer(tk, TK_STRING_LITERAL);
}

void t_tokenize(Tokenizer *tk) {
    while (!t_is_eof(tk)) {
        const char c = t_peek(tk);
        if (c == '.' && !is_num(t_peek_next(tk))) {
            t_consume(tk);
            if (t_peek(tk) == '.' && t_peek_next(tk) == '.') {
                t_consume_n(tk, 2);
                t_push_buffer(tk, TK_ELLIPSES);
            } else t_push_buffer(tk, TK_DOT);
        } else if (is_num(c) || c == '.' || c == '-' && is_num(t_peek_next(tk))) {
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
                    while (is_num(t_peek(tk))) {
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
                while (is_num(t_peek(tk))) {
                    t_consume(tk);
                }
                if (t_peek(tk) == '.') {
                    t_consume(tk);
                    is_float = true;
                    while (is_num(t_peek(tk))) {
                        t_consume(tk);
                    }
                }
            }
            int l_count = 0;
            int u_count = 0;
            int f_count = 0;
            for (;;) {
                char c = t_peek(tk);
                if (c == 'u' || c == 'U') {
                    if (u_count >= 1) break;
                    u_count++;
                    t_consume(tk);
                } else if (c == 'l' || c == 'L') {
                    if (l_count >= 2) break;
                    l_count++;
                    t_consume(tk);
                } else if (c == 'f' || c == 'F') {
                    if (f_count >= 1) break;
                    f_count++;
                    is_float = true;
                    t_consume(tk);
                } else break;
            }
            t_push_buffer(tk, is_float ? TK_FLT_LITERAL : TK_INT_LITERAL);
        } else if (is_alpha(c) || c == '_') {
            t_consume(tk);
            while (is_alpha_num(t_peek(tk))) {
                t_consume(tk);
            }
            t_parse_and_push_buffer(tk);
            t_buffer_reset(tk);
        } else if (is_whitespace(c)) t_skip(tk);
        else if (t_peek(tk) == '/' && (t_peek_next(tk) == '/' || t_peek_next(tk) == '*')) t_skip_comments(tk);
        else if (t_peek(tk) == '\'') t_consume_char_literal(tk);
        else if (t_peek(tk) == '\"') t_consume_string_literal(tk);
        else if (is_op_start(c)) t_consume_operator(tk);
        else t_consume_special_char(tk);
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

bool is_postfix_operator(const TokenType type) {
    switch (type) {
    case TK_INCR:
    case TK_DECR:
    case TK_OPEN_PAREN:
    case TK_OPEN_SQUARE:
    case TK_DOT:
    case TK_ARROW:
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
        log_start(LOG_ERROR);
        printf("Tried to get the underlying operator of a non-eq operator\n");
        print_token_type(type);
        exit(1);
    }
}

int op_associativity(const TokenType type) {
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
        PANIC("Tried to get the associativity of a token which is not a binary "
              "operator");
    }
}

int op_precedence(const TokenType type) {
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
        log_start(LOG_ERROR);
        printf("Tried to get the precedence of a token which is not a binary operator");
        print_token_type(type);
        exit(1);
    }
}

const char *token_type_str(const TokenType type) {
    switch (type) {
    case TK_INT_LITERAL:
        return "Int Literal";
    case TK_FLT_LITERAL:
        return "Float Literal";
    case TK_CHAR_LITERAL:
        return "Char Literal";
    case TK_STRING_LITERAL:
        return "String Literal";
    case TK_SEMI:
        return "';'";
    case TK_PLUS:
        return "'+'";
    case TK_MINUS:
        return "'-'";
    case TK_MULTIPLY:
        return "'*'";
    case TK_DIVIDE:
        return "'/'";
    case TK_XOR:
        return "'^'";
    case TK_EXPR:
        return "Expr";
    case TK_EQ:
        return "'='";
    case TK_OPEN_PAREN:
        return "'('";
    case TK_CLOSE_PAREN:
        return "')'";
    case TK_OPEN_CURLY:
        return "'{'";
    case TK_CLOSE_CURLY:
        return "'}'";
    case TK_COMMA:
        return "','";
    case TK_IDENTIFIER:
        return "Identifier";
    case TK_MOD:
        return "\'%%\'";
    case TK_EQ_EQ:
        return "\'==\'";
    case TK_PLUS_EQ:
        return "\'+=\'";
    case TK_MINUS_EQ:
        return "\'-=\'";
    case TK_MULTIPLY_EQ:
        return "\'*=\'";
    case TK_DIVIDE_EQ:
        return "\'/=\'";
    case TK_MOD_EQ:
        return "\'%%=\'";
    case TK_NEQ:
        return "\'!=\'";
    case TK_LT:
        return "\'<\'";
    case TK_LE:
        return "\'<=\'";
    case TK_GT:
        return "\'>\'";
    case TK_GE:
        return "\'>=\'";
    case TK_SHL:
        return "\'<<\'";
    case TK_SHR:
        return "\'>>\'";
    case TK_SHL_EQ:
        return "\'<<=\'";
    case TK_SHR_EQ:
        return "\'>>=\'";
    case TK_AND:
        return "\'&\'";
    case TK_AND_AND:
        return "\'&&\'";
    case TK_AND_EQ:
        return "\'&=\'";
    case TK_OR:
        return "\'|\'";
    case TK_OR_OR:
        return "\'||\'";
    case TK_OR_EQ:
        return "\'|=\'";
    case TK_XOR_EQ:
        return "\'^=\'";
    case TK_L_NOT:
        return "\'!\'";
    case TK_BW_NOT:
        return "\'~\'";
    case TK_INCR:
        return "\'++\'";
    case TK_DECR:
        return "\'--\'";
    case TK_OPEN_SQUARE:
        return "\'[\'";
    case TK_CLOSE_SQUARE:
        return "\']\'";
    case TK_DOT:
        return "\'.\'";
    case TK_ARROW:
        return "\'->\'";
    case TK_COLON:
        return "\':\'";
    case TK_ELLIPSES:
        return "\'...\'";
    case TK_AUTO:
    case TK_BREAK:
    case TK_CASE:
    case TK_CHAR:
    case TK_CONST:
    case TK_CONTINUE:
    case TK_DEFAULT:
    case TK_DO:
    case TK_DOUBLE:
    case TK_ELSE:
    case TK_ENUM:
    case TK_EXTERN:
    case TK_FLOAT:
    case TK_FOR:
    case TK_GOTO:
    case TK_IF:
    case TK_INLINE:
    case TK_INT:
    case TK_LONG:
    case TK_REGISTER:
    case TK_RETURN:
    case TK_SHORT:
    case TK_SIGNED:
    case TK_SIZEOF:
    case TK_STATIC:
    case TK_STRUCT:
    case TK_SWITCH:
    case TK_TYPEDEF:
    case TK_UNION:
    case TK_UNSIGNED:
    case TK_VOID:
    case TK_VOLATILE:
    case TK_WHILE:
        return KEYWORDS[type];
    }
}
void print_token_type(const TokenType type) { printf("%s", token_type_str(type)); }

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
        // TODO fix this printf("%c ", token->value[0]);
    }
    printf("}\n");
}
