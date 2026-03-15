// typedef struct {
//     int a;
//     int b;
//     int c;
//     int d;
//     int e;
// } A;
typedef struct {
    char a;
    char b;
    char c;
} A;

extern int printf(const char *fmt, ...);
extern int foo(A s);
// int foo(A s) { return s.a + s.b + s.c + s.d + s.e; }

int main() {
    // A s = {1, 2, 3, 4, 5};
    A s = {'b', 'a', 'd'};
    printf("%c %c %c\n", s.a, s.b, s.c);
    return foo(s);
}
