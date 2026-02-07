// Comprehensive compiler feature test
// Returns a deterministic integer result

int mix(int a, int b, int *p, char c) {
    int local = a + b;
    local += *p;
    local ^= c;
    return local;
}

int main() {
    int a = 10;
    int result = mix(10, 10, &a, 10);

    return result;
}
