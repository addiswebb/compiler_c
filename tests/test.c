#include <stdarg.h>

double sum_mix(int n, ...) {
    va_list args;
    va_start(args, n);
    double s = 0;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            s += (double)va_arg(args, int);
        } else {
            s += va_arg(args, double);
        }
    }
    va_end(args);
    return s;
}

int main() { return (int)sum_mix(4, 1, 2.5, 3, 4.5); }
