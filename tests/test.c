// courtesy of ChatGPT :)
int main() {
    /* Equality Inequality */
    int cmp_eq = (10 == 10); // expected: 1
    int cmp_ne = (10 != 5);  // expected: 1

    /* Signed comparisons */
    int cmp_lt = (3 < 5);  // expected: 1
    int cmp_le = (5 <= 5); // expected: 1
    int cmp_gt = (7 > 3);  // expected: 1
    int cmp_ge = (5 >= 5); // expected: 1

    /* Extra: negative numbers */
    int cmp_lt_neg = (-2 < 0);  // expected: 1
    int cmp_gt_neg = (-1 > -5); // expected: 1

    /* Optional: check zero results */
    int cmp_eq_zero = (3 == 5); // expected: 0
    int cmp_ne_zero = (4 != 4); // expected: 0
    int cmp_lt_zero = (5 < 3);  // expected: 0
    int cmp_gt_zero = (2 > 8);  // expected: 0

    /* Test using values in conditional statements */
    return cmp_eq + cmp_ne + cmp_lt + cmp_le + cmp_gt + cmp_ge + cmp_lt_neg + cmp_gt_neg + cmp_eq_zero + cmp_ne_zero + cmp_lt_zero +
           cmp_gt_zero + 4; // 12
}
