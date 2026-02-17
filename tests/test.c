int main() {
    int a[3] = {4, 2, 1};
    int *p = a;

    if ((int)(*(p + 1) / (float)a[2]) == 2) {
        a[0] <<= 1; // 8
    } else {
        a[0] >>= 1;
    }

    return a[0] + a[1] + a[2]; // 8 + 2 + 1 = 11
}
