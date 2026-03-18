#include "stdio.h"
#include <string.h>

int main() {
    char str[] = "Hello Worlo";
    str[strlen(str) - 1] = 'd';
    printf("%s\n", str);
    return 0;
}
