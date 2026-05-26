typedef struct A {
    long a;
    long b;
    long c;
    long d;
    long e;
} A;
int foo(A x);
int main() {
    A x; // ={.a = 1, .b = 2, .c = 3, .d = 4, .e = 5};
    x.a = 1;
    x.b = 2;
    x.c = 3;
    x.d = 4;
    x.e = 5;
    return foo(x);
}
