struct S {
    long long a;
    long long b;
};

extern long long sum_struct(int n, ...);

int main() {
    struct S s1 = {1, 2};
    struct S s2 = {3, 4};
    return (int)sum_struct(2, s1, s2);
}
