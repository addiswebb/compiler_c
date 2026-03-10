extern int printf(const char *format, ...);
static inline int foo(int x) {
    printf("Foo: %d\n", x);
    return x;
}

int main() { return foo(10); }
