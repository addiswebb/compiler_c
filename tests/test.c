int main() {
    int a[4] = {1, 2, 3, 4};
    int *p = a;

    // p = p + sizeof(int); // move to a[1]
    p = p + 1;
    // Works
    // p[0] += a[2]; // a[1] = 2 + 3 = 5
    // Doesnt work
    *p = 5;

    // return a[0] + a[1] * 10 + a[3];
    return *p;
}
