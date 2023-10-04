#pragma once

#include <ccwrap_header.h>
#include <stddef.h>
#include __CCWRAP_NATIVE_UC_HEADER_PATH(stdlib.h)

#if _MSC_VER < 1600
__ccwrap_forceinline __int64	    	llabs(__int64 v) { return _abs64(v); }
#endif
#if _MSC_VER < 1800
__ccwrap_forceinline __int64	    	atoll(char const* p) { return _atoi64(p); }
__ccwrap_forceinline __int64	    	strtoll(char const* p, char **e, int r) { return _strtoi64(p,e,r); }
__ccwrap_forceinline unsigned __int64	strtoull(char const* p, char **e, int r) { return _strtoui64(p,e,r); }
__ccwrap_forceinline __int64	    	wtoll(wchar_t const* p) { return _wtoi64(p); }
__ccwrap_forceinline __int64	    	wcstoll(wchar_t const* p, wchar_t **e, int r) { return _wcstoi64(p,e,r); }
__ccwrap_forceinline unsigned __int64	wcstoull(wchar_t const* p, wchar_t **e, int r) { return _wcstoui64(p,e,r); }

__ccwrap_forceinline float 	    	strtof(char const* p, char **e) { return (float)strtod(p,e); }
__ccwrap_forceinline float 	    	wcstof(wchar_t const* p, wchar_t **e) { return (float)wcstod(p,e); }
__ccwrap_forceinline long double   	strtold(char const* p, char **e) { return strtod(p,e); }
__ccwrap_forceinline long double   	wcstold(wchar_t const* p, wchar_t **e) { return wcstod(p,e); }
#endif
