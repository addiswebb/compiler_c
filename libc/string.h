#ifndef _STRING_H
#define _STRING_H

#include "stdint.h"
#include <string.h>

void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
int memcmp(const void *, const void *, size_t);

char *strcpy(char *, const char *);
char *strncpy(char *, const char *, size_t);

int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);

size_t strlen(const char *);

char *strrchr(const char *, int);

#ifdef _WIN64
char *_strdup(const char *);
char *_strndup(const char *, size_t);
#else
char *strdup(const char *);
char *strndup(const char *, size_t);
#endif

#endif
