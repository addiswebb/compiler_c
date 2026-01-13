int add(int a, int b) {
    int result = a + b;
    return result;
}

int mul(int x, int y) {
    int r = x * y;
    return r;
}

int sum_to_n(int n) {
    int sum = 0;
    int i = 0;

    for (i = 1; n - i + 1; i = i + 1) {
        sum = sum + i;
    }

    return sum;
}

int mixed(int a, int b, int c) {
    int x = add(a, b);
    int y = mul(x, c);

    {
        int x = y + 1; // shadowing test
        y = x + y;
    }

    return y;
}

int main() {
    int a = 3;
    int b = 4;
    int c = 5;

    int s = add(a, b);
    int p = mul(s, c);

    int t = sum_to_n(10);
    int m = mixed(a, b, c);

    int result = p + t + m;
    return result;
}
