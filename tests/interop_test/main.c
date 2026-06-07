#include <stdarg.h>

void print(const char *fmt, ...);
void vprint(const char *fmt, va_list ap);

void print(const char *fmt, ...) {
    va_list ap;
    __builtin_va_start(ap, fmt);
    vprint(fmt, ap);
    __builtin_va_end(ap);
}

int main() {
    print("%d %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9);
    return 0;
}
