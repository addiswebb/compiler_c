// RUN: %cc %s -o %t
// RUN: %check_exit 14 %t

int main() {
    int nums[] = {1, 2, 3};
    int *p = nums;
    *(p + 1) += 5;                      // nums[1] = 7
    *(nums + 2) *= 2;                   // nums[2] = 6
    return nums[0] + nums[1] + nums[2]; // 1 + 7 + 6 = 14
}
