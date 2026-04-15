// RUN: %cc %s -o %t
// RUN: %check_exit 12 %t

enum E { A = 1, B = 3 };

int main() {
    enum E e = B;
    int r = 0;

    switch (e) {
    case A:
        r = 1;
        break;
    case B:
        r += 10;
    default:
        r += 2;
    }

    return r; // expected: 12
}
