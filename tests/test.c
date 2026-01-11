int main() {
    int n = 12;

    int a = 0;
    int b = 1;
    for (int i = 2; n - i + 1; i = i + 1) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}
