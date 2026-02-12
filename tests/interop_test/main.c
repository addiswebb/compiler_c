// stdlib.h minimal
#include <stdio.h>
extern int abs(int x);

// stdio.h minimal
extern int putchar(int c);

int main() {
    // puts("hello world\n");
    char str[6] = "hello";
    puts(str);
    return putchar('c');
}
