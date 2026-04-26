// int a = sizeof(int) + 16;

// int main() {
//     int arr[1 + 2 * 3 - 2] = {[2 + 1] = 21};
//     return arr[2 * 2 - 1] + a; // 21 + 4 + 16 = 41
// }

enum E { A = 5, B, C };

int main() {
    return A + B + C; // 5 + 6 + 7 = 18
}
