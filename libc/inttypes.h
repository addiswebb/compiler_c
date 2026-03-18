#ifndef _INTTYPES_H
#define _INTTYPES_H

#ifdef _WIN64
    #define PRId64 "lld"
    #define PRIx64 "llx"
#else
    #define PRId64 "ld"
    #define PRIx64 "lx"
#endif

#endif
