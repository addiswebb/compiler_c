struct Inner {
    int x;
};

struct Outer {
    struct Inner inner;
};

int main() {
    struct Outer o;

    o.inner.x = 5;

    struct Inner tmp;
    tmp.x = 15;
    o.inner = tmp;

    // // Reassign nested struct
    // struct Inner tmp;
    // tmp.c = 3;
    // tmp.x = 7;
    // o.inner = tmp;

    // return o.a + o.inner.c + o.inner.x + o.tail;
    // Expected exit code: 5 + 3 + 7 + 2 = 17
    return o.inner.x;
}
