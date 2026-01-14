int main() {
    int n = 10;
    int x = 0;
    for (int i = 0; i < n; i++) {
        x++;
    }

    while (x < n * 2) {
        x++;
    }

    if (x == 20) {
        return 2000;
    } else {
        return -2000;
    }
    return x;
}
