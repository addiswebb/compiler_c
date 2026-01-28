#ifndef COMPILER_C_PARSER_UTIL_H
#define COMPILER_C_PARSER_UTIL_H

#include <stdint.h>
int64_t parse_int(const char *raw, int len);
int64_t parse_binary(const char *raw, int len);
int64_t parse_oct(const char *raw, int len);
int64_t parse_dec(const char *raw, int len);
int64_t parse_hex(const char *raw, int len);
double parse_float(const char *raw, int len);

#endif
