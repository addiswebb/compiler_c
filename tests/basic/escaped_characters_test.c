// RUN: %cc %s -o %t
// RUN: %t | %check_output "707"

extern int printf(const char *, ...);

int main() {
    // Char literals
    int a = '\n';   // 10
    int b = '\\';   // 92
    int c = '\t';   // 9
    int d = '\x41'; // hex: 65 ('A')
    int e = '\101'; // octal: 65 ('A')

    // String literals
    char s1[] = "ab\x43\104"; // 'a'=97 'b'=98 hex C=67 octal D=68 -> 330
    char s2[] = "\\\"\n";     // 92+34+10 -> 136

    int sum = a + b + c + d + e;
    // 10+92+9+65+65 = 241

    for (int i = 0; s1[i]; i++) {
        sum += s1[i];
    }
    for (int i = 0; s2[i]; i++) {
        sum += s2[i];
    }
    // 241 + 330 + 136 = 707

    printf("%d\n", sum);
    return 0;
}
