// RUN: %cc %s -o %t
// RUN: %check_exit 5 %t

typedef int* P;

int main() {
    int x = 5;
    P p = &x;
    return *p;
}
