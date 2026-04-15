// RUN: %cc %s -o %t
// RUN: %check_exit 2 %t

int main() {
    int x = 0;
    int y = 1;

    if (x || ++y) { // x is 0, so ++y is evaluated
        x = y;      // x = 2
    }

    return x; // 2
}
