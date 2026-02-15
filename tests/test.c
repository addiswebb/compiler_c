struct Pair {
    int a;
    int b;
};

int main() {
    struct Pair p1;
    p1.a = 4;
    p1.b = 10;

    struct Pair p2;
    p2 = p1; // Struct copy

    return p2.a + p2.b; // Expected exit code: 14
}
