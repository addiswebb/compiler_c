typedef struct {
    int a;
    int b;
    int c;
    int d;
    int e;
} A;

int foo(A s) { return s.a + s.b + s.c + s.d + s.e; }

int main() {
    A s = {1, 2, 3, 4, 5};
    return foo(s);
}
