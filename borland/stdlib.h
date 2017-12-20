#ifndef __CCWRAP_STDLIB_H
#define __CCWRAP_STDLIB_H

#pragma once

#include <ccwrap_header.h>
//#include <stddef.h>

#if __BORLANDC__ >= 0x700
 #include <../crtl/stdlib.h>
#else
 #include __CCWRAP_NATIVE_C_HEADER_PATH(stdlib.h)
 #if __BORLANDC__ < 0x561
  #ifdef __cplusplus
    namespace std {
  #endif
    static inline __int64 strtoll(char const* p, char** pe, int rdx) { return strtol(p, pe, rdx); }
    static inline unsigned __int64 strtoull(char const* p, char** pe, int rdx) { return strtoul(p, pe, rdx); }
  #ifdef __cplusplus
    }
    using std::strtoll;
    using std::strtoull;
  #endif
 #endif
#endif

#endif
