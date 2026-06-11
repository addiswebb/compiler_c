#ifndef _STDIO_H
#define _STDIO_H

#include "stdarg.h"
#include "stdint.h"

#define NULL ((void *)0)

#define EOF (-1)

typedef struct FILE FILE;

#ifdef __linux__
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
#endif
#ifdef _WIN64
FILE *__acrt_iob_func(unsigned int _Fileno);
#define stdin (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))
#endif

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

#ifdef _WIN64
FILE *_popen(const char *, const char *);
int _pclose(FILE *);
#else
FILE *popen(const char *, const char *);
int pclose(FILE *);
#endif

#endif
