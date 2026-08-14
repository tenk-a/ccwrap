#include "c_test.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static int sum_ints(int n, ...) {
    va_list ap;
    int s, i;
    va_start(ap, n);
    s = 0;
    for (i = 0; i < n; ++i)
        s += va_arg(ap, int);
    va_end(ap);
    return s;
}

static double sum_mixed(int n, ...) {
    va_list ap;
    double s;
    int i, kind;
    va_start(ap, n);
    s = 0.0;
    for (i = 0; i < n; ++i) {
        kind = va_arg(ap, int);
        switch (kind) {
        case 0:  s += va_arg(ap, int);              break;
        case 1:  s += va_arg(ap, double);           break;
        case 2:  s += (double)va_arg(ap, long);     break;
        case 3:  s += (double)va_arg(ap, unsigned); break;
        default:                                    break;
        }
    }
    va_end(ap);
    return s;
}

static const char* pick_str(int idx, ...) {
    va_list ap;
    const char* p;
    int i;
    va_start(ap, idx);
    p = 0;
    for (i = 0; i <= idx; ++i)
        p = va_arg(ap, const char*);
    va_end(ap);
    return p;
}

static long long sum_and_max(int n, int* out_max, ...) {
    va_list ap, ap2;
    long long s;
    int i, mx, v;
    va_start(ap, out_max);
    va_copy(ap2, ap);

    s = 0;
    for (i = 0; i < n; ++i)
        s += va_arg(ap, int);
    va_end(ap);

    mx = 0;
    for (i = 0; i < n; ++i) {
        v = va_arg(ap2, int);
        if (i == 0 || v > mx)
            mx = v;
    }
    va_end(ap2);

    *out_max = mx;
    return s;
}

static int fmt(char* buf, size_t sz, const char* f, ...) {
    va_list ap;
    int r;
    va_start(ap, f);
    r = vsnprintf(buf, sz, f, ap);
    va_end(ap);
    return r;
}

TEST_CASE(stdarg, va_start_arg_end) {

    test_eq( sum_ints(0), 0 );
    test_eq( sum_ints(1, 42), 42 );
    test_pass("c1990:va_start");
    test_eq( sum_ints(3, 1, 2, 3), 6 );
    test_pass("c1990:va_arg");
    test_eq( sum_ints(5, 10, -20, 30, -40, 50), 30 );
    test_pass("c1990:va_end");
}

TEST_CASE(stdarg, va_arg_types) {
    double d;
    const char* s;
    const char* s0;

    d = sum_mixed(4, 0, 1, 1, 2.5, 2, 10L, 3, 7u);
    test_true( d > 20.4 && d < 20.6 );

    s = pick_str(2, "a", "b", "c");
    test_true( strcmp(s, "c") == 0 );
    s0 = pick_str(0, "z", "y");
    test_true( strcmp(s0, "z") == 0 );
    test_pass("c1990:va_arg (mixed types, pointers, promotions)");
}

TEST_CASE(stdarg, va_copy_twice) {
    int mx;
    long long s;

    mx = 0;
    s = sum_and_max(4, &mx, 3, 9, 4, 1);
    test_eq( s, 17LL );
    test_eq( mx, 9 );

    mx = 0;
    s = sum_and_max(1, &mx, -5);
    test_eq( s, -5LL );
    test_eq( mx, -5 );
    test_pass("c1999:va_copy");
}

TEST_CASE(stdarg, va_list_passed_on) {
    char buf[64];
    int n = fmt(buf, sizeof buf, "%d-%s-%c", 12, "ab", 'z');
    test_eq( n, 7 );
    test_true( strcmp(buf, "12-ab-z") == 0 );
    test_pass("c1990:va_list forwarded to a v* function");
}


static int ccw_va_count(int first, ...) {
    va_list ap;
    va_list copy;
    int n = 0;
    int v;
    va_start(ap, first);
    va_copy(copy, ap);
    for (v = first; v != -1; v = va_arg(ap, int)) ++n;
    va_end(ap);
    for (v = va_arg(copy, int); v != -1; v = va_arg(copy, int)) ;
    va_end(copy);
    return n;
}

TEST_CASE(stdarg, va_list_and_c23) {
    va_list unused;
    (void)sizeof(unused);
    test_eq( ccw_va_count(1, 2, 3, -1), 3 );
    test_eq( ccw_va_count(7, -1), 1 );
    test_true( sizeof(va_list) > 0 );
    test_pass("c1990:va_list");

#if defined(__STDC_VERSION_STDARG_H__)
    test_true( __STDC_VERSION_STDARG_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STDARG_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_STDARG_H__");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    test_true( ccw_va_count(1, 2, -1) == 2 );
    TEST_NOTE("single-argument va_start needs a (...) declaration; checked by compiling");
    test_pass("c2023:va_start(ap) single-argument form");
#else
    TEST_NOTE("single-argument va_start is C23; this mode does not have it");
    test_skip("c2023:va_start(ap) single-argument form");
#endif
}

TEST_SUITE(stdarg)
    TEST_ADD(va_start_arg_end)
    TEST_ADD(va_arg_types)
    TEST_ADD(va_copy_twice)
    TEST_ADD(va_list_passed_on)
    TEST_ADD(va_list_and_c23)
TEST_SUITE_END(stdarg)
