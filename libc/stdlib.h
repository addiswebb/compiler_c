#ifndef _STDLIB_H
#define _STDLIB_H
#include "stdint.h"

void exit(int);

void *malloc(size_t);
void *realloc(void *, size_t);
void *calloc(size_t, size_t);
void free(void *);
int system(const char *);

typedef int (*__compar_fn_t)(const void *, const void *);
extern void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);

#endif
