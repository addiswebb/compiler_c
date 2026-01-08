#include <stdbool.h>

bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

bool is_digit(char c) { return (c >= '0' && c <= '9'); }

bool is_alpha_num(char c) { return is_alpha(c) || is_digit(c); }

bool is_alpha_str(char *c) {
    int i = 0;
    while (c[i++] != '\0') {
        if (!is_alpha(c[i])) {
            return false;
        }
    }
    return true;
}

bool is_int_or_float(char *c, bool *is_int) {
    int i = 0;
    bool found_decimal = false;
    while (c[i] != '\0') {
        // Number cannot contain more than one decimal
        bool accept = true;
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
bool is_num_str(char *c) { return is_int_or_float(c, ((void *)0)); }

bool is_whitespace(char c) {
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
