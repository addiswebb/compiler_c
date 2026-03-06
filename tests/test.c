extern int printf(const char *format, ...);

typedef struct Point {
    int x;
    int y;
} Point;

int main(void) {
    Point p = (Point){1, 2};
    printf("Point [%d, %d]\n", p.x, p.y);
    return 0;
}
