#if __CCWRAP_C1990_OR_CXX1998

#if 1	//

#include "../../ccwrap/ccwrap_misc/inttypes_impl.hh"

#else	//
#pragma once

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct imaxdiv_t {
    intmax_t	quot;
    intmax_t	rem;
} imaxdiv_t;

static __forceinline intmax_t	imaxabs(intmax_t j) { return (j < 0) ? -j : j; }
static __forceinline imaxdiv_t	imaxdiv(intmax_t number, intmax_t denom) { imaxdiv_t d = { number / denom, number % denom }; return d; }

static __forceinline intmax_t	strtoimax(const char* s, char** endp, int radix) { return (intmax_t)_strtoi64(s, endp, radix); }
static __forceinline intmax_t	strtoumax(const char* s, char** endp, int radix) { return (uintmax_t)_strtoui64(s, endp, radix); }

static __forceinline intmax_t	wcstoimax(const wchar_t* s, wchar_t** endp, int radix) { return (intmax_t)_wcstoi64(s, endp, radix); }
static __forceinline intmax_t	wcstoumax(const wchar_t* s, wchar_t** endp, int radix) { return (uintmax_t)_wcstoui64(s, endp, radix); }

#ifdef __cplusplus
}
#endif
#include "../../ccwrap/ccwrap_misc/inttypes_macro_impl.hh"
#endif	//

#endif
