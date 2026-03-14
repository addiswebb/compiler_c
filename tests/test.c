typedef struct {
    int a;
    int b;
    int c;
} S;

int foo(S s) { return s.b; }

int main() {
    S s = {.a = 5, .b = 'b', .c = 'a'};
    return foo(s);
}
