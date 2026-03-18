#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool is_num(char c) { return c >= 48 && c <= 57; }

int main() {
    char str[] = "Hello Worlo";
    str[strlen(str) - 1] = 'd';

    if (is_num('a') == true) {
        return 5;
    }

    printf("%s\n", str);
    return 0;
}
