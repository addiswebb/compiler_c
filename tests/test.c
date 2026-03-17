extern int printf(const char *fmt, ...);

// int square(int x) { return x * x; }

// int main() {
//     int (*fp)(int) = &square;

//     int (*table[2])(int) = {square, fp};

//     printf("%d\n", table[0](4));
//     printf("%d\n", table[1](4));
//     return 0;
// }

int foo(int x) { return x * 5; }

int main() {
    int (*f)(int);
    f = foo;
    return f(4);
}
