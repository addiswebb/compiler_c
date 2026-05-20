#ifndef COMPILER_C_UTIL_H
#define COMPILER_C_UTIL_H
#include "../libc/stdbool.h"

/* Is any character `a-z`, or `A-Z` */
bool is_alpha(char c);
/* Is any character `0-9` */
bool is_num(char c);
/* Is either alpha or numeric character */
bool is_alpha_num(char c);
/* String contains exclusively alpha and numeric characters. */
bool is_alpha_numeric_str(const char *c);

/* Safely converts uppercase characters to lowercase. Must recieve any alpha character. */
char to_lower_case(const char c);

/* Is any character `0-9`, `a-f`, `A-F`  */
bool is_hex(const char c);
/* Is any character `0-7` */
bool is_oct(const char c);
/* Is any character `0-1` */
bool is_binary(const char c);
/* Is any character `\n`, `\t`, `\f`, `\r`, `\v`, ` `, */
bool is_whitespace(char c);
#endif // COMPILER_C_UTIL_H
