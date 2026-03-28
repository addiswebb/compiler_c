// RUN: %check_exit 6 %t

int add(int a, int b) { return a + b; }

int (*get_op())(int, int) {
    int (*x)(int, int) = add;
    return x;
}

int main() { return get_op()(2, 4); }
