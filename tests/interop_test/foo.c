#include <stdarg.h>

struct S {
    long long a;
    long long b;
};

long long sum_struct(int n, ...) {
    va_list args;
    va_start(args, n);
    long long s = 0;
    for (int i = 0; i < n; i++) {
        struct S v = va_arg(args, struct S);
        s += v.a + v.b;
    }
    va_end(args);
    return s;
}
