typedef struct {
    int a;
    int b;
    int c;
    int d;
    int e;
} A;

typedef struct {
    int x;
    int y;
} B;

int foo(A a) { return a.a + a.b + a.c + a.d + a.e; }
int buzz(B b) { return b.x + b.y; }

int main() {
    A a = {1, 2, 3, 4, 5};
    B b = {10, 20};
    return foo(a) + buzz(b);
}
