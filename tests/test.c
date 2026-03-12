extern int printf(const char *format, ...);

typedef struct {
    int a;
    int b;
} S;

int foo(S s) { return s.b; }

int main() {
    S s = {1234569, 6};
    return foo(s);
}
