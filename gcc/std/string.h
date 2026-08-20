// ccwrap <string.h>
#pragma once

#include <ccwrap_common.h>

#include_next <string.h>

#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__APPLE__)
# define _CCW_STRING_H_HAVE_STRNLEN 1
#endif

#if !defined(__USE_XOPEN2K8) && !defined(_CCW_STRING_H_HAVE_STRNLEN)
# define _CCW_STRING_H_HAVE_STRNLEN 1
# ifdef __cplusplus
extern "C++" {
# endif
static inline size_t strnlen(const char* __s, size_t __n) {
    size_t __i = 0;
    while (__i < __n && __s[__i]) ++__i;
    return __i;
}
# ifdef __cplusplus
}
# endif
#endif // !__USE_XOPEN2K8 && !_CCW_STRING_H_HAVE_STRNLEN
