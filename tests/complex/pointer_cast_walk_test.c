// RUN: %cc %s -o %t
// RUN: %check_exit 55 %t

int main() {
    int a[4] = {1, 2, 3, 4};
    char *p = (char *)a;

    p += sizeof(int);  // move to a[1]
    *(int *)p += a[2]; // a[1] = 2 + 3 = 5

    // 1 + 50 + 4 = 55
    return a[0] + a[1] * 10 + a[3];
}
