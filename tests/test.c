struct A {
    char c;
    int b;
};
int main() {
    struct A a = (struct A){1, 5};
    return a.b;
}
