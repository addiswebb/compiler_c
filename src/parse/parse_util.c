#include "compiler_c/parse/parse_util.h"
int64_t parse_int(const char *raw, int len) {
    /*
        Hex
        Oct
        Dec
    */
    if (raw[0] == '0') {
        if (raw[1] == 'x') {
            return parse_hex(raw, len);
        }
        return parse_oct(raw, len);
    }
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        res = res * 10 + (*raw - '0');
        raw++;
    }
    return res;
}

int64_t parse_dec(const char *raw, int len) {}
int64_t parse_oct(const char *raw, int len) {}

int64_t parse_hex(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value = (*raw - '0');
        res = res * 10 + (*raw - '0');
        raw++;
    }
}
