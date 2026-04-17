// RUN: %cc %s -o %t
// RUN: %check_exit 5 %t

int main() {
    int a = 0;
    int b = 5;

    if (a && ++b) { // a is 0, so ++b should NOT be evaluated
        a = b;
    }

    return b; // 5
}
