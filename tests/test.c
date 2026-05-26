#include <stdarg.h>
#include <stdio.h>
int vsum(int n, va_list ap);
int sum(int n, ...) {
    int sum = 0;
    va_list ap;
    va_start(ap, n);
    sum = vsum(n, ap);
    va_end(ap);
    return sum;
}

int vsum(int n, va_list ap) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += va_arg(ap, int);
    }
    return sum;
}

int main() { return sum(5, 1, 2, 3, 4, 5); }
