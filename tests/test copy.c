// Comprehensive compiler feature test
// Returns a deterministic integer result

int mix(int a, int b, int *p, char c) {
    int local = a + b;
    local += *p;
    local ^= c;
    return local;
}

int main() {
    // ===== Literals & Types =====
    int dec = 10;
    int hex = 0x10;   // 16
    int oct = 010;    // 8
    int bin = 0b1010; // 10

    float f = 2.5;
    double d = 3.0;
    char ch = 'A';
    short s = 4;
    long l = 100;

    // ===== Arrays & C-Strings =====
    int arr[5] = {1, 2, 3, 4, 5};
    char str[4] = "abc";

    // ===== Pointers & Address Arithmetic =====
    int *p = arr;
    *(p + 2) += 5; // arr[2] = 8

    // ===== Unary Operators =====
    int u = +dec;
    u = -u;
    u = !u;
    u = ~u;
    u++;

    // ===== sizeof =====
    int size_test = sizeof(arr) / sizeof(arr[0]);

    // ===== Binary Operators =====
    int result = 0;
    result += dec + hex;
    result -= oct;
    result *= bin;
    result /= 2;

    result &= 0xFF;
    result |= 0x10;
    result ^= 0x3;

    result <<= 1;
    result >>= 1;

    // ===== Logical & Comparisons =====
    if ((dec < hex && oct <= bin) || (hex != bin)) {
        result += 5;
    } else {
        result -= 5;
    }
    // ===== For Loop =====
    // for (int i = 0; i < 5; i++) {
    //     result += arr[i];
    // }
    // result += arr[0];
    // result += arr[1];
    // result += arr[2];
    result += arr[3];
    // result += arr[4];
    return result;

    // // ===== While Loop + Shadowing =====
    // int i = 0;
    // while (i < 3) {
    //     int result_shadow = i * 2; // shadowing
    //     result += result_shadow;
    //     i++;
    // }

    // // ===== Function Call + Type Cast =====
    // int cast_val = (int)(f + d);
    // result += mix(result, cast_val, &arr[1], ch);

    // // ===== String & Char Access =====
    // result += str[0]; // 'a' = 97
    // result += size_test;
    // result += s;
    // result += (int)l;

    // // ===== Pointer Assignment =====
    // int *rp = &result;
    // *rp += 1;

    // // Final deterministic result
    // return result; // 389
}
