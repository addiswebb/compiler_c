// RUN: %cc %s -o %t
// RUN: %check_exit 51 %t

enum E { A = 5, B = 0, C };

int main() {
    return A * 10 + B * 3 + C;   // 5*10 + 0 + 1 = 51
}
