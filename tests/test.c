int main() {
    int arr[3] = {1, 2, 3};

    *(arr + 1) += 5;
    return arr[1];
}
