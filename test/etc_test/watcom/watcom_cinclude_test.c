#include <ccwrap_common.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

static int failures = 0;

static void ck(int cond, const char *what) {
    if (!cond) { printf("[watcom_cinclude] FAIL: %s\n", what); ++failures; }
}

int main(void) {

    uint32_t u32 = UINT32_MAX;
    int64_t  i64 = INT64_MIN;
    uintmax_t um = UINTMAX_MAX;

    ck(sizeof(int8_t) == 1, "int8_t size");
    ck(sizeof(int16_t) == 2, "int16_t size");
    ck(sizeof(int32_t) == 4, "int32_t size");
    ck(sizeof(int64_t) == 8, "int64_t size");
    ck(u32 == 4294967295u, "UINT32_MAX");
    ck(i64 < 0, "INT64_MIN");
    ck(um > 0, "UINTMAX_MAX");

    ck(INT8_WIDTH == 8, "INT8_WIDTH");
    ck(UINT8_WIDTH == 8, "UINT8_WIDTH");
    ck(INT16_WIDTH == 16, "INT16_WIDTH");
    ck(INT32_WIDTH == 32, "INT32_WIDTH");
    ck(INT64_WIDTH == 64, "INT64_WIDTH");
    ck(INTMAX_WIDTH == 64, "INTMAX_WIDTH");
    ck(UINTMAX_WIDTH == 64, "UINTMAX_WIDTH");
    ck(INTPTR_WIDTH == 32 || INTPTR_WIDTH == 64 || INTPTR_WIDTH == 16, "INTPTR_WIDTH");
    ck(SIZE_WIDTH == 32 || SIZE_WIDTH == 64 || SIZE_WIDTH == 16, "SIZE_WIDTH");
    ck(PTRDIFF_WIDTH == 32 || PTRDIFF_WIDTH == 64 || PTRDIFF_WIDTH == 16, "PTRDIFF_WIDTH");
    ck(WCHAR_WIDTH >= 8, "WCHAR_WIDTH");
    ck(SIG_ATOMIC_WIDTH >= 8, "SIG_ATOMIC_WIDTH");

#if INT32_WIDTH != 32
    ck(0, "INT32_WIDTH usable in #if");
#endif
#ifndef __STDC_VERSION_STDINT_H__
    ck(0, "__STDC_VERSION_STDINT_H__ defined");
#else
    ck(__STDC_VERSION_STDINT_H__ >= 202311L, "__STDC_VERSION_STDINT_H__ value");
#endif

    ck(INT32_C(5) == 5, "INT32_C");
    ck(UINT64_C(5) == 5u, "UINT64_C");
    ck(INTMAX_C(5) == 5, "INTMAX_C");

    ck(CHAR_BIT == 8, "CHAR_BIT");
    ck(SCHAR_WIDTH == 8, "SCHAR_WIDTH (C23 supplement)");
    ck(INT_WIDTH == 32, "INT_WIDTH (C23 supplement)");
    ck(LLONG_WIDTH == 64, "LLONG_WIDTH (C23 supplement)");
    ck(BOOL_WIDTH == 1, "BOOL_WIDTH (C23 supplement)");
    ck(WCHAR_MIN == 0, "WCHAR_MIN (C23 addition to <limits.h>)");
    ck(WCHAR_MAX == 65535u, "WCHAR_MAX (C23 addition to <limits.h>)");
    ck(WCHAR_WIDTH == 16, "WCHAR_WIDTH (C23 addition to <limits.h>)");
    ck(WINT_MIN == WCHAR_MIN, "WINT_MIN");
    ck(WINT_MAX == WCHAR_MAX, "WINT_MAX");
    ck(WINT_WIDTH == WCHAR_WIDTH, "WINT_WIDTH");
#ifndef __STDC_VERSION_LIMITS_H__
    ck(0, "__STDC_VERSION_LIMITS_H__ defined");
#else
    ck(__STDC_VERSION_LIMITS_H__ >= 202311L, "__STDC_VERSION_LIMITS_H__ value");
#endif
#if INT_WIDTH != 32
    ck(0, "INT_WIDTH usable in #if");
#endif
    ck(FLT_DECIMAL_DIG >= 6, "FLT_DECIMAL_DIG (C11 supplement)");
    ck(DBL_DECIMAL_DIG >= 15, "DBL_DECIMAL_DIG (C11 supplement)");
#ifndef __STDC_VERSION_FLOAT_H__
    ck(0, "__STDC_VERSION_FLOAT_H__ defined");
#else
    ck(__STDC_VERSION_FLOAT_H__ >= 202311L, "__STDC_VERSION_FLOAT_H__ value");
#endif
    ck(FLT_HAS_SUBNORM == 1 || FLT_HAS_SUBNORM == 0 || FLT_HAS_SUBNORM == -1,
       "FLT_HAS_SUBNORM (C11 supplement)");
    ck(DBL_NORM_MAX > 0.0, "DBL_NORM_MAX (C11 supplement)");

    if (failures == 0)
        printf("[watcom_cinclude] all checks passed\n");
    return failures;
}
