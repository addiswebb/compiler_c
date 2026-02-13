int main() {
    int x = 3;
    {
        int x = 10; // shadow outer x
        x += 5;     // x = 15
    }
    return x; // outer x = 3
}
