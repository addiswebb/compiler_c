// RUN: %cc %s -o %t
// RUN: %check_exit 12 %t

int main() {
    double d = 5.7;
    int i = (int)d * 2;      // 5*2=10
    float f = (float)i / 4;  // 10/4=2.5
    return (int)f + i;       // 2+10=12
}
