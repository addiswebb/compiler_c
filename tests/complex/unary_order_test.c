// RUN: %cc %s -o %t
// RUN: %check_exit 73 %t

int main() {
    int x = 1;
    int y = x++ + ++x + x;
    // x++ -> 1 (x=2)
    // ++x -> 3
    // x   -> 3
    // y = 1 + 3 + 3 = 7
    return y * 10 + x; // 70 + 3 = 73
}
