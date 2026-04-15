// RUN: %cc %s -o %t
// RUN: %check_exit 7 %t

typedef int T;

int main() {
    T x = 7;
    return x;
}
