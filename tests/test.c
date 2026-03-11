extern int printf(const char *format, ...);
void buzz() { printf("buzz\n"); }
void foo() {
    printf("foo ");
    return buzz();
}
int main() {
    foo();
    return 0;
}
