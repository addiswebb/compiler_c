int add(int a, int b) { return a + b; }

int (*get_op())(int, int) { return add; }

int main() { return get_op()(2, 4); }
