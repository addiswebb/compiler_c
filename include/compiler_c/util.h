#ifndef COMPILER_C_UTIL_H
#define COMPILER_C_UTIL_H
#include <stdbool.h>

bool is_alpha(char c);
bool is_digit(char c);
bool is_alpha_num(char c);
bool is_alpha_numeric_str(const char *c);

char to_lower_case(const char c);

bool is_hex(const char c);
bool is_oct(const char c);
bool is_binary(const char c);

bool is_valid_int_str(const char *c);

bool is_int_or_float(const char *c, bool *is_int);
bool is_num_str(const char *c);
bool is_whitespace(char c);

#endif // COMPILER_C_UTIL_H
