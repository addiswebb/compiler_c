typedef struct A {
    long a;
    long b;
    long c;
    long d;
    long e;
} A;
int foo(int *z, A x, A y) {
    return x.a + x.b + x.c + y.d + y.e;
    // return y.a;
}
