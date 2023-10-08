/**
 *  @file   cinttypes
 *  @brief  Format conversion of integer types for old-vc
 */
#if __CCWRAP_C1990_OR_CXX1998

//#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#ifndef __CCWRAP_NO_WCHAR
#include <wchar.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct imaxdiv_t {
    intmax_t	quot;
    intmax_t	rem;
} imaxdiv_t;

static inline intmax_t	imaxabs(intmax_t j) { return (j < 0) ? -j : j; }
static inline imaxdiv_t	imaxdiv(intmax_t number, intmax_t denom) { imaxdiv_t d = { number / denom, number % denom }; return d; }

static inline intmax_t	strtoimax(const char* s, char** endp, int radix) { return (intmax_t)strtoll(s, endp, radix); }
static inline intmax_t	strtoumax(const char* s, char** endp, int radix) { return (uintmax_t)strtoull(s, endp, radix); }
#ifndef __CCWRAP_NO_WCHAR
static inline intmax_t	wcstoimax(const wchar_t* s, wchar_t** endp, int radix) { return (intmax_t)wcstoll(s, endp, radix); }
static inline intmax_t	wcstoumax(const wchar_t* s, wchar_t** endp, int radix) { return (uintmax_t)wcstoull(s, endp, radix); }
#endif

#ifdef __cplusplus
}
#endif

#include "../ccwrap_misc/inttypes_macro_impl.hh"

#endif
