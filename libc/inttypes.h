#ifndef _INTTYPES_H
#define _INTTYPES_H

#ifdef _WIN64
    #define PRId64 "lld"
    #define PRIx64 "llx"
    #define PRIu64 "llu"
#else
    #define PRId64 "ld"
    #define PRIx64 "lx"
    #define PRIu64 "lu"
#endif

#endif
