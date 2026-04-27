#include <stdio.h>
char *strs[2] = {"hello", "world"};
int main() {
    for (int i = 0; i < 12; i++) {
        printf("%c\n", *(strs[0] + i));
    }
    return 0;
}
