// RUN: %cc %s -o %t
// RUN: %check_exit 14 %t

int main() {
    char str[] = "abc";
    str[0] = 'z';
    int nums[3] = {1, 2, 3};
    nums[1] *= 5;                       // nums[1] = 10
    return nums[0] + nums[1] + nums[2]; // 1 + 10 + 3 = 14
}
