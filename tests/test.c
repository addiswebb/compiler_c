typedef struct {
    int a;
    int b;
    int c;
} A;

int foo(A a) {
    // return a.a + a.b;
    return a.a + a.b + a.c;
}

int main() {
    A a = {
        1,
        7,
        3,
    };
    return foo(a);
}
