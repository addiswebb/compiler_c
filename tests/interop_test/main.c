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

extern int foo(A s);
// int foo(A s) { return s.a + s.b + s.c + s.d + s.e; }

int main() {
    // A s = {1, 2, 3, 4, 5};
    A s = {1, 'a', 2};
    return foo(s);
}
