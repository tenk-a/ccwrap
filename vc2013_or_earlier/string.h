#pragma once

#include <ccwrap_header.h>
#include <stddef.h>
#include __CCWRAP_NATIVE_UC_HEADER_PATH(string.h)

__ccwrap_force_inline char* strdup(char const* s) { return _strdup(s); }
//__ccwrap_force_inline char* strupr(char* p) { return _strupr(s); }
//__ccwrap_force_inline char* strlwr(char* p) { return _strlwr(s); }

#if 0
__ccwrap_force_inline int strcasecmp(char const* l, char const* r) { return _stricmp(l, r); }
__ccwrap_force_inline int strncasecmp(char const* l, char const* r, size_t n) { return _strnicmp(l, r, n); }
__ccwrap_force_inline int wcscasecmp(wchar_t const* l, wchar_t const* r) { return _wcsicmp(l, r); }
__ccwrap_force_inline int wcsncasecmp(wchar_t const* l, wchar_t const* r, size_t n) { return _wcsnicmp(l, r, n); }
#endif
