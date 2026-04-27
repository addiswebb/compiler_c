// int a = sizeof(int) + 16;

// int main() {
//     int arr[1 + 2 * 3 - 2] = {[2 + 1] = 21};
//     return arr[2 * 2 - 1] + a; // 21 + 4 + 16 = 41
// }

int arr[11] = {1, [7] = 3};

int main() { return arr[0]; }
