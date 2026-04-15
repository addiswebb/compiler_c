// RUN: %cc %s -o %t
// RUN: %check_exit 339 %t

int main() {
    char s[4] = "xyz";
    s[0] = s[1]; // 'y','y','z'
    char *p = s;
    *(p + 2) = 'a';            // 'y','y','a'
    return s[0] + s[1] + s[2]; // 'y'+ 'y'+ 'a'=121+121+97=339
}
