extern int printf(const char *fmt, ...);
typedef struct {
    int a;
    int b;
    int c;
} S;

int main() {
    printf("%d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
    return 0;
}
