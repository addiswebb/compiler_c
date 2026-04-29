#ifndef _STDINT_H
#define _STDINT_H

#ifdef _WIN64

typedef long long int64_t;
typedef unsigned long long size_t;

#else

typedef long int64_t;
typedef unsigned long size_t;
typedef short int8_t;
typedef unsigned short uint8_t;

#endif

#endif
