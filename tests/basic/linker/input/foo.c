// RUN: echo 1
// UNSUPPORTED: a
#include "../foo.h"

int printf(const char *, ...);

int foo(int a, double b, char *c) {
    printf("%d %lf %s\n", a, b, c);
    return a + b + c[0];
}
