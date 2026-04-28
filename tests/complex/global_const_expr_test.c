// RUN: %cc %s -o %t
// RUN: %t | %check_output "hello world 15"

int printf(const char *, ...);

char *strs[2] = {[0] = "hello", [1] = "world"};
int a = 10;
int c = 15;
int *b = &a + 1;

int main() {
    printf("%s %s %d\n", strs[0], strs[1], *b);
    return 0;
}
