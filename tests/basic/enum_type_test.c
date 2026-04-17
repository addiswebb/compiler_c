// RUN: %cc %s -o %t
// RUN: %check_exit 3 %t

enum E { A = 3 };

int main() {
    enum E x = A;
    return x;   // 3
}
