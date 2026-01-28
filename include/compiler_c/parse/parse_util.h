#ifndef COMPILER_C_PARSER_UTIL_H
#define COMPILER_C_PARSER_UTIL_H

#include <stdint.h>
int64_t parse_int(const char *raw, int len);
int64_t parse_hex(const char *raw, int len);
int64_t parse_oct(const char *raw, int len);

#endif
