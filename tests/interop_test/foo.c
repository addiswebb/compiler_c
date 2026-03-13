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

// extern int foo(Point p);
// int foo(A s) { return s.a + s.b + s.c + s.d + s.e; }
int foo(A s) { return s.a + s.b + s.c; }
