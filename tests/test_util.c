#include "../util.c"
#include <stdio.h>

void test_num_str(char *str) {
    if (is_num_str(str)) {
        printf("true");
    } else {
        printf("false");
    }
    printf(": %s", str);
    printf("\n");
}
void test_num(char c) {
    if (is_digit(c)) {
        printf("true");
    } else {
        printf("false");
    }
    printf(": %c", c);
    printf("\n");
}
int main(void) {
    test_num_str("12");
    test_num_str("1");
    test_num_str("123456789123456789");
    test_num_str("10.");
    test_num_str(".10");
    test_num_str("10.10");
    test_num_str("a");
    test_num_str("abcdef");
    test_num_str("");
    test_num_str("\0");
    return 0;
}
