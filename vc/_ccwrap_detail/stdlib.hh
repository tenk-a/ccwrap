#if _MSC_VER && _MSC_VER < 1800

#if _MSC_VER < 1600
__forceinline __int64	    	llabs(__int64 v) { return _abs64(v); }
#endif
__forceinline __int64	    	atoll(char const* p) { return _atoi64(p); }
__forceinline __int64	    	strtoll(char const* p, char **e, int r) { return _strtoi64(p,e,r); }
__forceinline unsigned __int64	strtoull(char const* p, char **e, int r) { return _strtoui64(p,e,r); }
__forceinline __int64	    	wtoll(wchar_t const* p) { return _wtoi64(p); }
__forceinline __int64	    	wcstoll(wchar_t const* p, wchar_t **e, int r) { return _wcstoi64(p,e,r); }
__forceinline unsigned __int64	wcstoull(wchar_t const* p, wchar_t **e, int r) { return _wcstoui64(p,e,r); }

__forceinline float 	    	strtof(char const* p, char **e) { return (float)strtod(p,e); }
__forceinline float 	    	wcstof(wchar_t const* p, wchar_t **e) { return (float)wcstod(p,e); }
__forceinline long double   	strtold(char const* p, char **e) { return strtod(p,e); }
__forceinline long double   	wcstold(wchar_t const* p, wchar_t **e) { return wcstod(p,e); }

#endif	// _MSC_VER
