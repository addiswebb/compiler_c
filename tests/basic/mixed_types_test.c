// RUN: %cc %s -o %t
// RUN: %check_exit 13 %t

int main() {
    int i = 10;
    float f = 2.5;
    double d = 4.0;
    i += (int)f;       // i = 12
    d /= f;            // d = 1.6
    return (int)d + i; // 1 + 12 = 13
}
