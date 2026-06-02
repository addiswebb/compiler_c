typedef enum X {
    A,
    B,
    C,
} X;

int foo(X a) { return a; }
int main() { return foo(B); }
