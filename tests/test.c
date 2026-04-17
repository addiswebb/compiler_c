int main() {
    int a[4] = {1, 2, 3, 4};
    char *p = (char *)a;

    p += sizeof(int); // move to a[1]
    // p = p + sizeof(int);
    return a[0] + a[1] * 10 + a[3];
}
