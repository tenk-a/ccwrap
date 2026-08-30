// ccwrap <stdarg.h>
#if defined(__need___va_list) || defined(__need_va_list) || defined(__need_va_arg)
#include_next <stdarg.h>
#else
#pragma once

#include <ccwrap_common.h>

#include_next <stdarg.h>

#ifndef va_copy
 #if defined(__GNUC__) || defined(__clang__)
  #define va_copy(dst, src)     __builtin_va_copy(dst, src)
 #else
  #define va_copy(dst, src)     ((dst) = (src))
 #endif
#endif
#ifndef __va_copy
 #define __va_copy(dst, src)    va_copy((dst), (src))
#endif
#endif // __need___va_list
