// RUN: %cc %s -o %t
// RUN: %check_exit 6 %t

int main() {
    // ((1-2) > 0) ? (3*4) : ((5+6) ? (7-1) : 8) = 6
    return 1 - 2 > 0 ? 3 * 4 : 5 + 6 ? 7 - 1 : 8;
}
