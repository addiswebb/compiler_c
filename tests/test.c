int get(char *a, int b) { return a[b]; }

int main() {
    char x[6] = "hello";
    int i = 0;
    return get(x, i);
}
