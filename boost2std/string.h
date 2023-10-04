// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STRING_H_INCLUDED
#define __CCWRAP_STRING_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <string.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(string.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(string.h)
#else
  #include <../include/string.h>
#endif

#if defined(_MSC_VER)

__ccwrap_forceinline char* strdup(char const* s) { return _strdup(s); }
//__ccwrap_forceinline char* strupr(char* p) { return _strupr(s); }
//__ccwrap_forceinline char* strlwr(char* p) { return _strlwr(s); }

#if 0
__ccwrap_forceinline int strcasecmp(char const* l, char const* r) { return _stricmp(l, r); }
__ccwrap_forceinline int strncasecmp(char const* l, char const* r, size_t n) { return _strnicmp(l, r, n); }
__ccwrap_forceinline int wcscasecmp(wchar_t const* l, wchar_t const* r) { return _wcsicmp(l, r); }
__ccwrap_forceinline int wcsncasecmp(wchar_t const* l, wchar_t const* r, size_t n) { return _wcsnicmp(l, r, n); }
#endif

#endif	// _MSC_VER

#endif
