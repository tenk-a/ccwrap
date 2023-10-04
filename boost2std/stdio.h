// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDIO_H_INCLUDED
#define __CCWRAP_STDIO_H_INCLUDED


#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdio.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stdio.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdio.h)
#else
  #include <../include/stdio.h>
#endif

#if _MSC_VER && _MSC_VER < 1900

#include <stdarg.h>

#pragma warning(push)
#pragma warning(disable:4996)

__ccwrap_forceinline int vsnprintf(char buf[], size_t bufsz, char const* fmt, va_list a) {
 #if _MSC_VER >= 1400
    return vsprintf_s(buf, bufsz, fmt, a);
 #else  // Note! Sloppy handling without buffer checking.
    return vsprintf(buf, fmt, a);
 #endif
}

inline int   snprintf(char buf[], size_t bufsz, char const* fmt, ...) {
    int      n;
    va_list  a;
    va_start(a,fmt);
    n = vsnprintf(buf, bufsz, fmt, a);
    va_end(a);
    return n;
}

__ccwrap_forceinline int vsnwprintf(wchar_t buf[], size_t bufsz, wchar_t const* fmt, va_list a) {
 #if _MSC_VER >= 1400
    return vswprintf_s(buf, bufsz, fmt, a);
 #else  // Note! Sloppy handling without buffer checking.
    return vswprintf(buf, fmt, a);
 #endif
}

inline int   snwprintf(wchar_t buf[], size_t bufsz, wchar_t const* fmt, ...) {
    int      n;
    va_list  a;
    va_start(a,fmt);
    n = vsnwprintf(buf, bufsz, fmt, a);
    va_end(a);
    return n;
}

#pragma warning(pop)

#endif //_MSC_VER

#endif
