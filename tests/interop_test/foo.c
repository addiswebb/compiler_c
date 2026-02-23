// #include <stdio.h>
extern int putchar(int character);

int test(int a, int b, int c, int d, int e, int f, int g) {
    putchar('a' + a - 1);
    putchar('a' + b - 1);
    putchar('a' + c - 1);
    putchar('a' + d - 1);
    putchar('a' + e - 1);
    putchar('a' + f - 1);
    putchar('a' + g - 1);
    return a + b + c + d + e + f + g;
}
