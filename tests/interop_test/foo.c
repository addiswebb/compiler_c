typedef struct A {
    long a;
    long b;
    long c;
    long d;
    long e;
} A;
int foo(A x) { return x.a + x.b + x.c; }
