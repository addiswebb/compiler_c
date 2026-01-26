int test(int x){return x;}

int main() {
    int i = test(sizeof(double[5]));
    return i;
}
