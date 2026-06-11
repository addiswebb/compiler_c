#ifndef COMPILER_C_UTIL_H
#define COMPILER_C_UTIL_H
#include "../libc/stdbool.h"

/* Is any character `a-z`, or `A-Z` */
bool is_alpha(char c);
/* Is any character `0-9` */
bool is_num(char c);
/* Is any alpha/numeric/`_` */
bool is_alpha_num(const char c);
/* Is any character `0-9`, `a-f`, `A-F`  */
bool is_hex(char c);
/* Is any character `0-7` */
bool is_oct(char c);
/* Is any character `0-1` */
bool is_binary(char c);
/* Is any character `\n`, `\t`, `\f`, `\r`, `\v`, ` `, */
bool is_whitespace(char c);
#endif // COMPILER_C_UTIL_H
