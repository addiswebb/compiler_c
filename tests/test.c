extern int printf(char *format, ...);

void faa() {
    printf("faa\n");
    return;
}

void foo() { return faa(); }

int main(void) {
    foo();
    return 0;
}
