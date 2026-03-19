typedef struct {
    int x;
    int y;
    int z;
    int a;
    int b;
} A;

A foo(int x, int y, int z) {
    A a;
    a.x = x;
    a.y = y;
    // a.z = z;
    return a;
}

int main() {
    A b = foo(5, 7, 2);
    return b.x + b.y; // + b.z;
}
