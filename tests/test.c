extern int printf(const char *fmt, ...);
typedef struct {
    int a;
    int b;
    int c;
} S;

int foo(S s) {
    printf("Hello %d\n", s.b);
    return s.b;
}

int main() {
    S s = {.a = 5, .b = 'b', .c = 'a'};
    return foo(s);
}
