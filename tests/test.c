int main() {
    int n = 10;
    int a = 0;
    for (int i = 0; i < n; i++) {
        a = a + 2;
        continue;
    }
    return a;
}
