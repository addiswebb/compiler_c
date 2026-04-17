// RUN: %cc %s -o %t
// RUN: %t | %check_output "0 5 0  0 5 7  1 0 3  4 0 0  1 0 9  6 2 3  1 2 3  1 2 3"

extern int printf(const char *format, ...);

int main() {

    int a1[3] = {[1] = 5};
    int a2[3] = {[1] = 5, 7};
    int a3[3] = {[2] = 3, [0] = 1};
    int a4[3] = {[0] = 1, [0] = 4};
    int a5[3] = {1, [2] = 9};
    int a6[3] = {[0] = 1, [0] = 6, 2, 3};
    int a7[3] = {[0] = 1, [1] = 2, [2] = 3};
    int a8[3] = {1, 2, 3};

    printf("%d %d %d  %d %d %d  %d %d %d  %d %d %d  %d %d %d  %d %d %d  %d %d %d  %d %d %d\n",
           a1[0], a1[1], a1[2],
           a2[0], a2[1], a2[2],
           a3[0], a3[1], a3[2],
           a4[0], a4[1], a4[2],
           a5[0], a5[1], a5[2],
           a6[0], a6[1], a6[2],
           a7[0], a7[1], a7[2],
           a8[0], a8[1], a8[2]);

    return 0;
}
