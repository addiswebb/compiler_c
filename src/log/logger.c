#include "compiler_c/log/logger.h"
#include "compiler_c/analyse/const_expr.h"
#include "compiler_c/core/node.h"
#include "compiler_c/core/type.h"
#include "compiler_c/ir/ir_module.h"
#include "compiler_c/ir/ir_util.h"
#include "compiler_c/tokenize/tokenizer.h"
#include <stdio.h>

Logger logger = {};

// #if defined(__COMPILER_C__) && defined(_WIN64)

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
            case 'p':
                printf("%p", va_arg(ap, void *));
                break;
            case 'c':
                char c = va_arg(ap, int);
                putchar(c);
                break;
            case 'C':
                ConstLiteral *cl = va_arg(ap, ConstLiteral *);
                print_const_literal(cl);
                break;
            case 'i':
                if (*(fmt + 1) == 'r') {
                    fmt++;
                    IR_Value *value = va_arg(ap, IR_Value *);
                    print_ir_value(value);
                    break;
                }
                break;
            case 'n':
                if (*(fmt + 1) == 'k') {
                    fmt++;
                    NodeKind kind = va_arg(ap, NodeKind);
                    print_node_kind(kind);
                    break;
                }
                Node *node = va_arg(ap, Node *);
                print_node(node, 0);
                break;
            default:
                WARN("Print doesnt handle '%c' in '%s'\n", *fmt, fmt);
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
