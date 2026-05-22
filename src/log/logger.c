#include "compiler_c/log/logger.h"
#include "compiler_c/analyse/const_expr.h"
#include "compiler_c/core/type.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <stdio.h>

Logger logger = {};

void vprint(const char *fmt, va_list ap) {
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
            case '%':
                putchar('%');
                break;
            case 'd':
                printf("%d", va_arg(ap, int));
                break;
            case 'f':
                printf("%f", va_arg(ap, double));
                break;
            case 't':
                if (*(fmt + 1) == 'k') {
                    fmt++;
                    TokenType token_type = va_arg(ap, TokenType);
                    print_token_type(token_type);
                    break;
                }
                Type *t = va_arg(ap, Type *);
                print_type(t);
                break;
            case 's':
                printf("%s", va_arg(ap, char *));
                break;
            case 'c':
                char c = va_arg(ap, int);
                putchar(c);
                break;
            case 'C':
                ConstLiteral *cl = va_arg(ap, ConstLiteral *);
                print_const_literal(cl);
                break;
            default:
                WARN("Logger print doesnt handle '%c' in '%s'\n", *fmt, fmt);
                break;
            }
            fmt++;
        } else {
            putchar(*fmt);
            fmt++;
        }
    }
}
void print(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprint(fmt, ap);
    va_end(ap);
}
