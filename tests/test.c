int main() {
    int x = 10;
    int *p = &x;
    *p += 5; // x = 15
    int **pp = &p;
    **pp -= 3; // x = 12
    return x;  // 12
}
