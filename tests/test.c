extern int printf(const char *format, ...);

int main(void) {
    int b[4] = {3, [2] = 3, 1};
    for (int i = 0; i < 4; i++) {
        printf("%d, ", b[i]);
    }
    return 0;
}
