#include <stdarg.h>
int sum(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += va_arg(ap, int);
    }
    va_end(ap);
    return sum;
}

int main() { return sum(5, 1, 2, 3, 4, 5); }
