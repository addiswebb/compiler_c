// RUN: %cc %s -o %t
// RUN: %check_exit 15 %t

int main() {
    int x = 4;
    int r = 0;

    switch (x) {
    default:
        r += 10;
    case 2:
        r += 5;
        break;
    case 3:
        r += 100;
    }

    return r;   // expected: 15
}
