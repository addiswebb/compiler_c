int foo(int x) { return x; }

int main() {
    int (*a)(int);
    a = foo;
    return 0;
}
