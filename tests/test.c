// RUN: %check_exit 42 %t

struct S {
    int x;
};

struct S make() {
    struct S s;
    s.x = 42;
    return s;
}

int main() {
    struct S s = make();
    return s.x;
}
