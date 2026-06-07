#include <stdarg.h>

#include <stdio.h>

void vprint(const char *fmt, va_list ap) {
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
            case '%':
                putchar('%');
                break;
            case 'd':
                printf("%d", __builtin_va_arg(ap, int));
                break;
            case 'f':
                printf("%f", __builtin_va_arg(ap, double));
                break;
            case 's':
                printf("%s", __builtin_va_arg(ap, char *));
                break;
            case 'p':
                printf("%p", __builtin_va_arg(ap, void *));
                break;
            case 'c':
                char c = __builtin_va_arg(ap, int);
                putchar(c);
                break;
            default:
                break;
            }
            fmt++;
        } else {
            putchar(*fmt);
            fmt++;
        }
    }
}
