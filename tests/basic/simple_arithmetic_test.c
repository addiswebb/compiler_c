// RUN: %cc %s -o %t
// RUN: %check_exit -5 %t

int main() {
    int a = 5;
    int b = 10;
    a += b;       // a = 15
    b *= 2;       // b = 20
    return a - b; // -5
}
