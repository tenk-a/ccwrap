// ccwrap <wchar.h>
#pragma once

#include <ccwrap_common.h>

#include_next <wchar.h>

#if defined(__cplusplus) && defined(_CCW_MINGW_CONST_RETURN)
extern "C++" {
inline wchar_t* wcschr(wchar_t* __p, wchar_t __c)
    { return const_cast<wchar_t*>(::wcschr(static_cast<const wchar_t*>(__p), __c)); }
inline wchar_t* wcspbrk(wchar_t* __s1, const wchar_t* __s2)
    { return const_cast<wchar_t*>(::wcspbrk(static_cast<const wchar_t*>(__s1), __s2)); }
inline wchar_t* wcsrchr(wchar_t* __p, wchar_t __c)
    { return const_cast<wchar_t*>(::wcsrchr(static_cast<const wchar_t*>(__p), __c)); }
inline wchar_t* wcsstr(wchar_t* __s1, const wchar_t* __s2)
    { return const_cast<wchar_t*>(::wcsstr(static_cast<const wchar_t*>(__s1), __s2)); }
inline wchar_t* wmemchr(wchar_t* __p, wchar_t __c, size_t __n)
    { return const_cast<wchar_t*>(::wmemchr(static_cast<const wchar_t*>(__p), __c, __n)); }
}
#endif // __cplusplus && _CCW_MINGW_CONST_RETURN
