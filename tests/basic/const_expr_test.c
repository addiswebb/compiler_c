// RUN: %cc %s -o %t
// RUN: %check_exit 54 %t

int a = sizeof(int) + 16;
int b[11] = {1, [7 / 2] = 3 + 5 * 2};

int main() {
    int arr[1 + 2 * 3 - 2] = {[2 + 1] = 21};
    switch (a) {
    case 20 + 7 * 0:
        return arr[2 * 2 - 1] + a + b[7 / 2]; // 21 + 4 + 16 + 13= 54
    default:
        return 1;
    }
    return 0;
}
