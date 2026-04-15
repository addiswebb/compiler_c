// RUN: %cc %s -o %t
// RUN: %check_exit 1 %t

enum { A, B, C };

int main() {
    return B;   // 1
}
