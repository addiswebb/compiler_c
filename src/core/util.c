#include <compiler_c/core/util.h>

bool is_alpha(const char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

bool is_num(const char c) { return c >= '0' && c <= '9'; }


bool is_alpha_num(const char c) { return c == '_' || is_num(c) || is_alpha(c); }

bool is_hex(const char c) {
    if (is_num(c)) return true;
    const char lower = c | 0x20;
    return lower <= 'f' && lower >= 'a';
}

bool is_oct(const char c) { return c >= '0' && c <= '7'; }
bool is_binary(const char c) { return c == '0' || c == '1'; }

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
