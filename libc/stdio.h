#ifndef _STDIO_H
#define _STDIO_H

#include "stdarg.h"
#include "stdint.h"

#define NULL ((void *)0)

#define EOF (-1)

typedef struct FILE FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

FILE *fopen(const char *, const char *);
FILE *fdopen(int, const char *);
int fclose(FILE *);

int fflush(FILE *);

int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);

char *fgets(char *, int, FILE *);
char fgetc(FILE *);
char *gets(char *);

int fputs(const char *, FILE *);
int puts(const char *);

int printf(const char *, ...);
int fprintf(FILE *, const char *, ...);
int sprintf(char *, const char *, ...);
int snprintf(char *, size_t, const char *, ...);
extern int vfprintf(FILE *, const char *, va_list);

FILE *popen(const char *, const char *);
int pclose(FILE *);

#endif
