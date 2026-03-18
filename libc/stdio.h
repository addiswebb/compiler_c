#ifndef _STDIO_H
#define _STDIO_H

#include "stdint.h"

#define NULL ((void*)0)

#define EOF (-1)

typedef struct FILE FILE;

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

FILE *fopen(const char *, const char *);
int fclose(FILE *);

int fflush(FILE *);

int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);

char *fgets(char *, int, FILE *);
char *gets(char *);

int fputs(const char *, FILE *);
int puts(const char *);

int printf(const char *, ...);
int fprintf(FILE *, const char *, ...);
int sprintf(char *, const char *, ...);
int snprintf(char *, size_t, const char *, ...);

FILE *popen(const char *, const char *);
int pclose(FILE *);

#endif
