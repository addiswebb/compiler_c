typedef int bool;
bool is_alpha(char c);
bool is_num(char c);
bool is_alpha_num(char c);
bool is_alpha_numeric_str(const char *c);
char to_lower_case(const char c);
bool is_hex(const char c);
bool is_oct(const char c);
bool is_binary(const char c);
bool is_whitespace(char c);
bool is_alpha(const char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
char to_lower_case(const char c) { return c | (1 << 5); }
bool is_num(const char c) { return c >= '0' && c <= '9'; }
bool is_alpha_num(const char c) { return c == '_' || is_num(c) || is_alpha(c); }
bool is_hex(const char c) {
    if (is_num(c)) return 1;
    char lower = c | 0x20;
    return lower <= 'f' && lower >= 'a';
}
bool is_oct(const char c) { return c >= '0' && c <= '7'; }
bool is_binary(const char c) { return c == '0' || c == '1'; }
bool is_alpha_numeric_str(const char *c) {
    while (*c != '\0') {
        if (!is_alpha_num(*c)) {
            return 0;
        }
        c++;
    }
    return 1;
}
bool is_int_or_float(const char *c, bool *is_int) {
    int i = 0;
    bool found_decimal = 0;
    while (c[i] != '\0') {
        if (c[i] == '.' && !found_decimal) {
            if (i == 0 || c[i + 1] == '\0') {
                return 0;
            }
            found_decimal = 1;
        }
        if (!(is_num(c[i]) || c[i] == '.')) {
            return 0;
        }
        i++;
    }
    if (is_int) {
        *is_int = !found_decimal;
    }
    return i > 0;
}
bool is_whitespace(const char c) {
    switch (c) {
    case '\n':
    case '\t':
    case '\f':
    case '\r':
    case '\v':
    case ' ':
        return 1;
    default:
        return 0;
    }
}
