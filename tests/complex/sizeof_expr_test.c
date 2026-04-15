// RUN: %cc %s -o %t
// RUN: %check_exit 1 %t

int main() {
    int x = 5;
    return sizeof(x) == sizeof(int);
}
