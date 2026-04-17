int main() {
    int a = 1;
    long b = 2;
    short c = 3;
    int size_sum = sizeof(a) + sizeof(b) + sizeof(c); // assuming 4+8+2=14 on 64-bit
    return size_sum;
}
