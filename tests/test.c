typedef struct {
    int a;
    int b;
} A;

A foo(int x, int y) {
    A s = {.a = x, .b = y};
    return s;
}

int main() {
    A z = foo(1, 5);
    return z.a;
}
