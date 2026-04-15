// RUN: %cc %s -o %t
// RUN: %t | %check_output "1 2 3 4 5"

extern void qsort(int *Base, long NumOfElements, long SizeOfElements, int (*cmp)(const int *, const int *));
extern int printf(const char *fmt, ...);

int cmp_int(const int *a, const int *b) {
    int x = *a;
    int y = *b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int main() {
    int arr[] = {5, 1, 4, 2, 3};
    int n = 5;

    qsort(arr, n, 4, cmp_int);

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return 0;
}
