// RUN: %cc %s -o %t
// RUN: %check_exit 0 %t
#ifdef _WIN64
#define RESULT 10
#else
#define RESULT 14
#endif

int main() {
    int a = 1;
    long b = 2;
    short c = 3;
    int size_sum = sizeof(a) + sizeof(b) + sizeof(c); // assuming 4+8+2=14 on 64-bit
    return size_sum - RESULT;
}
