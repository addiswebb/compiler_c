#include "compiler_c/parse/parse_util.h"
#include "compiler_c/util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int64_t parse_int(const char *raw, int len) {
    if (len > 20) {
        printf("Cannot parse an integer larger than 64 bytes\n");
        exit(1);
    }
    if (raw[0] == '0' && len > 1) {
        switch (raw[1]) {
        case 'x':
        case 'X':
            return parse_hex(raw + 2, len - 2);
        case 'b':
        case 'B':
            return parse_binary(raw + 2, len - 2);
        default:
            return parse_oct(raw + 1, len - 1);
        }
    }
    return parse_dec(raw, len);
}

int64_t parse_dec(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        res = res * 10 + (*raw - '0');
        raw++;
    }
    return res;
}
int64_t parse_binary(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value = (*raw - '0');
        if (value > 1) {
            printf("Parse Binary Failed: digit cannot be larger than 1\n");
            exit(1);
        }
        res = res * 2 + value;
        raw++;
    }
    return res;
}
int64_t parse_oct(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value = (*raw - '0');
        if (value > 7) {
            printf("Parse Octal Failed: digit cannot be larger than 7\n");
            exit(1);
        }
        res = res * 8 + value;
        raw++;
    }
    return res;
}

int64_t parse_hex(const char *raw, int len) {
    int64_t res = 0;
    const char *start = raw;
    while (raw < start + len) {
        int value;
        if (is_num(*raw)) value = *raw - '0';
        char c = *raw | 0x20;
        if (c <= 'f' && c >= 'a') value = c - 'a' + 10;
        res = res * 16 + value;
        raw++;
    }
    return res;
}

double parse_float(const char *raw, int len) {
    double res = 0;
    const char *end = raw + len;
    double m = 0;
    while (raw < end) {
        if (*raw == '.') {
            m = 0.1;
            raw++;
            continue;
        }
        int digit = *raw - '0';
        if (m) {
            res += digit * m;
            m *= 0.1;
        } else {
            res = res * 10.0 + digit;
        }
        raw++;
    }

    return res;
}
