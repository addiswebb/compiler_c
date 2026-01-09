int main() {
    int i = 10;
    int x = 0;

    while (i) {
        if (x) {
            return i;
        } else {
            x = x + 1;
        }
        i = i - 1;
    }
    return i;
}
