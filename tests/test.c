int main() {
    int a = 0;
    int n = 10;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            if (i % 3 == 0) {
                a = i;
            }
        }
    }
    return a;
}
