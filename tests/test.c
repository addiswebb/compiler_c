#include <stdarg.h>

// double sum_mix(int n, ...) {
//     va_list args;
//     va_start(args, n);
//     double s = 0;
//     int a = 10;
//     int b = 10;
//     for (int i = 0; i < n; i++) {
//         if (i % 2 == 0) {
//             s += va_arg(args, int);
//         } else {
//             s += va_arg(args, double);
//         }
//         a++;
//         b++;
//     }
//     va_end(args);
//     return s;
// }

int sum(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += va_arg(ap, int);
    }
    va_end(ap);
    return sum;
}

// int main() { return (int)sum_mix(4, 1, 2.5, 3, 4.5); }
int main() { return sum(4, 1, 2, 3, 4, 5, 6); }
