int main() {
    int a = 0;
    int n = 10;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a++;
            if (j / 4 == 1) break;
        }
        continue;
    }
    return a;
}
