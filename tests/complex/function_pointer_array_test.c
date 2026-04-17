// RUN: %cc %s -o %t
// RUN: %check_exit 9 %t

int add1(int x) { return x + 1; }
int add2(int x) { return x + 2; }

int main() {
    int (*f[2])(int);
    f[0] = add1;
    f[1] = add2;

    return f[0](3) + f[1](3); // 4 + 5
}
