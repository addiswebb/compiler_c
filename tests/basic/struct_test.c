// RUN: %cc %s -o %t
// RUN: %check_exit 7 %t

// test_struct_basic.c
struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    p.x = 3;
    p.y = 4;

    return p.x + p.y;  // Expected exit code: 7
}
