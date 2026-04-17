// RUN: %cc %s -o %t
// RUN: %check_exit 5 %t

int add(int a, int b) { return a + b; }

int main() {
    int (*f)(int, int) = add;
    return f(2, 3);
}