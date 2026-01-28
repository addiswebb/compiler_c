#include <compiler_c/util.h>
#include <stdio.h>

bool is_alpha(const char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
char to_lower_case(const char c) {
    printf("%c to %c", c, c | (1 << 5));
    return c | (1 << 5);
}

bool is_digit(const char c) { return c >= '0' && c <= '9'; }

bool is_alpha_num(const char c) { return c == '_' || is_digit(c) || is_alpha(c); }

bool is_hex(const char c) {
    if (is_digit(c)) return true;
    char lower = c | 0x20;
    return lower <= 'f' && lower >= 'a';
}

bool is_oct(const char c) { return c >= '0' && c <= '7'; }
bool is_binary(const char c) { return c == '0' || c == '1'; }

bool is_valid_int_str(const char *c) {
    while (*c != '\0') {
        if (!is_hex(*c)) {
            return false;
        }
        c++;
    }
    return true;
}

bool is_alpha_numeric_str(const char *c) {
    while (*c != '\0') {
        if (!is_alpha_num(*c)) {
            return false;
        }
        c++;
    }
    return true;
}

bool is_int_or_float(const char *c, bool *is_int) {
    int i = 0;
    bool found_decimal = false;
    while (c[i] != '\0') {
        // Number cannot contain more than one decimal
        if (c[i] == '.' && !found_decimal) {
            // Decimal cannot be first or last character,
            if (i == 0 || c[i + 1] == '\0') {
                return false;
            }
            found_decimal = true;
        }
        if (!(is_digit(c[i]) || c[i] == '.')) {
            return false;
        }
        i++;
    }
    if (is_int) {
        *is_int = !found_decimal;
    }
    // Weeds out any empty string or "\0"
    return i > 0;
}
bool is_num_str(const char *c) { return is_int_or_float(c, 0); }

bool is_whitespace(const char c) {
    switch (c) {
    case '\n':
    case '\t':
    case '\f':
    case '\r':
    case '\v':
    case ' ':
        return true;
    default:
        return false;
    }
}
