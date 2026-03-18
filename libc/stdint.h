#ifndef _STDINT_H
#define _STDINT_H

#ifdef _WIN64

typedef long long int64_t;
typedef unsigned long long size_t;

#else

typedef long int64_t;
typedef unsigned long size_t;

#endif

#endif
