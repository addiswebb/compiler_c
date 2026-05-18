#ifndef _STDINT_H
#define _STDINT_H

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#ifdef _WIN64

typedef unsigned long long size_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

#else

typedef unsigned long size_t;

typedef long int64_t;
typedef unsigned long uint64_t;

#endif

#endif
