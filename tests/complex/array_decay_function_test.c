// RUN: %cc %s -o %t
// RUN: %check_exit 5 %t

int sum(int *a) { return a[0] + a[1]; }

int main() {
    int arr[2] = {2, 3};
    return sum(arr);
}
