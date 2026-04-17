// RUN: %cc %s -o %t
// RUN: %check_exit 3 %t

int main() {
    int x = 3;
    for (int i = 0; i < 2; i++) {
        int x = i + 1;
        if ((x > 1 && i == 1) || x == 1) x += 5;
    }
    return x; // outer x still 3
}
