#include "compiler_c/log/logger.h"
#include "compiler_c/core/type.h"
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
                Type *t = va_arg(ap, Type *);
                print_type(t);
                break;
            case 's':
                printf("%s", va_arg(ap, char *));
                break;
            default:
                WARN("Logger print doesnt handle '%c' in \" \"\n", *fmt, fmt);
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
