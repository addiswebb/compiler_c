extern int printf(const char *format, ...);

typedef struct Point {
    char *a;
    int b;
    int c;
    char *s;
} Point;

int main(void) {
    // Point p = {"World", 2, 3, "Hello"};
    Point p = {"Hello", 2, 3, "World"};
    printf("Point [%s %d %d %s]\n", p.a, p.b, p.c, p.s);
    return 0;
}
