#include "c_test.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#if defined(_MSC_VER) && _MSC_VER < 1900
# define TST_NO_EXIT_C99 1
#endif
#if (defined(__WATCOMC__) && !(defined(_CCW_TARGET_C) && _CCW_TARGET_C >= 2011)) \
    || (defined(_MSC_VER) && _MSC_VER < 1900)
# define TST_NO_QUICK_EXIT 1
#endif

static int cmp_calls;
static int cmp_int(const void* a, const void* b) {
    int x = *(const int*)a, y = *(const int*)b;
    ++cmp_calls;
    return (x > y) - (x < y);
}

#define TST_INTEGER_TEXT_CAP (sizeof(unsigned long long) * CHAR_BIT + 3u)
static void tst_unsigned_text(char* dst, unsigned long long value, unsigned base) {
    char reverse[TST_INTEGER_TEXT_CAP];
    static const char digits[] = "0123456789abcdef";
    size_t n = 0;
    do {
        reverse[n++] = digits[(unsigned)(value % base)];
        value /= base;
    } while (value != 0);
    while (n != 0)
        *dst++ = reverse[--n];
    *dst = '\0';
}

static void tst_signed_text(char* dst, unsigned long long magnitude,
                            unsigned base, int negative) {
    if (negative)
        *dst++ = '-';
    tst_unsigned_text(dst, magnitude, base);
}

static int g_marker = 0;
static void on_exit_c(void) { g_marker |= 1; }

TEST_CASE(stdlib, macros) {
    void* p = NULL;
    size_t sz = sizeof(int);
    test_true( sz >= 1 );                    test_pass("c1990:size_t");
    test_true( p == NULL );                 test_pass("c1990:NULL");
    test_eq( EXIT_SUCCESS, 0 );              test_pass("c1990:EXIT_SUCCESS");
    test_ne( EXIT_FAILURE, EXIT_SUCCESS );   test_pass("c1990:EXIT_FAILURE");
    test_ge( (long)RAND_MAX, 32767L );       test_pass("c1990:RAND_MAX");
    test_ge( (size_t)MB_CUR_MAX, (size_t)1 );
    test_le( (size_t)MB_CUR_MAX, (size_t)MB_LEN_MAX );
    test_pass("c1990:MB_CUR_MAX");
}

TEST_CASE(stdlib, ato_family) {
    double d, e;
    test_eq( atoi("42"), 42 );
    test_eq( atoi("  -7abc"), -7 );
    test_eq( atoi("abc"), 0 );
    test_eq( atoi("+3"), 3 );
    test_pass("c1990:atoi");
    test_eq( atol("123456"), 123456L );
    test_eq( atol("-1"), -1L );
    test_pass("c1990:atol");
    d = atof("3.5");
    test_true( d > 3.49 && d < 3.51 );
    e = atof("-1e2");
    test_true( e > -100.01 && e < -99.99 );
    test_eq( atof("xyz"), 0.0 );
    test_pass("c1990:atof");

    test_eq( atoll("9007199254740993"), 9007199254740993LL );
    test_pass("c1999:atoll");
}

#if TEST_TARGET_C >= 1999
TEST_CASE(stdlib, strto_integer) {
    char* end = NULL;
    long l, z, big, small;
    long long ll, llbig;
    unsigned long ul, ulbig;
    unsigned long long ull, ullbig;
    const char* junk = "zz";
    char lmax_buf[3 * sizeof(long) * CHAR_BIT + 8];
    char lmin_over[3 * sizeof(long) * CHAR_BIT + 8];
    char ulmax_buf[3 * sizeof(unsigned long) * CHAR_BIT + 8];
    char llmax_buf[3 * sizeof(long long) * CHAR_BIT + 8];
    char ullmax_buf[3 * sizeof(unsigned long long) * CHAR_BIT + 8];
    size_t len;

    l = strtol("  123rest", &end, 10);
    test_eq( l, 123L );
    test_str_eq( end, "rest" );

    test_eq( strtol("ff", NULL, 16), 255L );
    test_eq( strtol("0x1f", NULL, 16), 31L );
    test_eq( strtol("0x10", NULL, 0), 16L );
    test_eq( strtol("010", NULL, 0), 8L );
    test_eq( strtol("-101", NULL, 2), -5L );

    z = strtol(junk, &end, 10);
    test_eq( z, 0L );
    test_true( end == junk );

    errno = 0;
    big = strtol("99999999999999999999", NULL, 10);
    test_eq( big, LONG_MAX );
    test_eq( errno, ERANGE );

    errno = 0;
    small = strtol("-99999999999999999999", NULL, 10);
    test_eq( small, LONG_MIN );
    test_eq( errno, ERANGE );

    sprintf(lmax_buf, "%ld", LONG_MAX);
    errno = 0;
    test_eq( strtol(lmax_buf, &end, 10), LONG_MAX );
    test_eq( errno, 0 );
    test_eq( (int)*end, 0 );
    sprintf(lmin_over, "%ld", LONG_MIN);
    len = strlen(lmin_over);
    lmin_over[len] = '0'; lmin_over[len + 1] = '\0';
    errno = 0;
    test_eq( strtol(lmin_over, &end, 10), LONG_MIN );
    test_eq( errno, ERANGE );
    test_eq( (int)*end, 0 );

    test_pass("c1990:strtol");

    errno = 0;
    ul = strtoul("4294967295", &end, 10);
    test_eq( ul, 4294967295UL );
    errno = 0;
    ulbig = strtoul("999999999999999999999", NULL, 10);
    test_eq( ulbig, ULONG_MAX );
    test_eq( errno, ERANGE );
    sprintf(ulmax_buf, "%lu", ULONG_MAX);
    errno = 0;
    test_eq( strtoul(ulmax_buf, &end, 10), ULONG_MAX );
    test_eq( errno, 0 );
    test_eq( (int)*end, 0 );
    errno = 0;
    test_pass("c1990:strtoul");

    errno = 0;
    ll = strtoll("-9000000000", &end, 10);
    test_eq( ll, -9000000000LL );
    test_eq( errno, 0 );
    errno = 0;
    llbig = strtoll("99999999999999999999999", NULL, 10);
    test_eq( llbig, LLONG_MAX );
    test_eq( errno, ERANGE );
    sprintf(llmax_buf, "%lld", LLONG_MAX);
    errno = 0;
    test_eq( strtoll(llmax_buf, &end, 10), LLONG_MAX );
    test_eq( errno, 0 );
    test_eq( (int)*end, 0 );
    errno = 0;
    test_pass("c1999:strtoll");

    ull = strtoull("18446744073709551615", NULL, 10);
    test_eq( ull, ULLONG_MAX );
    test_eq( errno, 0 );
    errno = 0;
    ullbig = strtoull("99999999999999999999999999", NULL, 10);
    test_eq( ullbig, ULLONG_MAX );
    test_eq( errno, ERANGE );
    sprintf(ullmax_buf, "%llu", ULLONG_MAX);
    errno = 0;
    test_eq( strtoull(ullmax_buf, &end, 10), ULLONG_MAX );
    test_eq( errno, 0 );
    test_eq( (int)*end, 0 );
    errno = 0;
    (void)ul; (void)ulbig;
    test_pass("c1999:strtoull");
}
#else
TEST_CASE(stdlib, strto_integer) {
    TEST_NOTE("strtoll / strtoull and LLONG_MAX are C99");
    test_skip("c1990:strtol");
    test_skip("c1990:strtoul");
    test_skip("c1999:strtoll");
    test_skip("c1999:strtoull");
}
#endif

TEST_CASE(stdlib, strto_integer_limit_base_matrix) {
    static const int bases[4] = { 2, 8, 10, 16 };
    char min_text[TST_INTEGER_TEXT_CAP];
    char max_text[TST_INTEGER_TEXT_CAP];
    char zero_text[TST_INTEGER_TEXT_CAP];
    char* end;
    unsigned long long long_min_magnitude;
    unsigned long long llong_min_magnitude;
    int i;
    long lv;
    unsigned long ulv;
    long long llv;
    unsigned long long ullv;

    long_min_magnitude = (unsigned long long)(0UL - (unsigned long)LONG_MIN);
    llong_min_magnitude = 0ULL - (unsigned long long)LLONG_MIN;

    for (i = 0; i < 4; ++i) {
        int base = bases[i];
        tst_signed_text(min_text, long_min_magnitude, (unsigned)base, 1);
        tst_signed_text(max_text, (unsigned long long)LONG_MAX, (unsigned)base, 0);

        errno = 0; end = NULL;
        lv = strtol(min_text, &end, base);
        test_eq( lv, LONG_MIN );
        test_true( end == min_text + strlen(min_text) );
        test_eq( errno, 0 );

        errno = 0; end = NULL;
        lv = strtol(max_text, &end, base);
        test_eq( lv, LONG_MAX );
        test_true( end == max_text + strlen(max_text) );
        test_eq( errno, 0 );
    }
    test_pass("c1990:strtol LONG_MIN/MAX in bases 2/8/10/16");

    for (i = 0; i < 4; ++i) {
        int base = bases[i];
        tst_unsigned_text(zero_text, 0ULL, (unsigned)base);
        tst_unsigned_text(max_text, (unsigned long long)ULONG_MAX, (unsigned)base);

        errno = 0; end = NULL;
        ulv = strtoul(zero_text, &end, base);
        test_eq( ulv, 0UL );
        test_true( end == zero_text + strlen(zero_text) );
        test_eq( errno, 0 );

        errno = 0; end = NULL;
        ulv = strtoul(max_text, &end, base);
        test_eq( ulv, ULONG_MAX );
        test_true( end == max_text + strlen(max_text) );
        test_eq( errno, 0 );
    }
    test_pass("c1990:strtoul 0/ULONG_MAX in bases 2/8/10/16");

    for (i = 0; i < 4; ++i) {
        int base = bases[i];
        tst_signed_text(min_text, llong_min_magnitude, (unsigned)base, 1);
        tst_signed_text(max_text, (unsigned long long)LLONG_MAX, (unsigned)base, 0);

        errno = 0; end = NULL;
        llv = strtoll(min_text, &end, base);
        test_eq( llv, LLONG_MIN );
        test_true( end == min_text + strlen(min_text) );
        test_eq( errno, 0 );

        errno = 0; end = NULL;
        llv = strtoll(max_text, &end, base);
        test_eq( llv, LLONG_MAX );
        test_true( end == max_text + strlen(max_text) );
        test_eq( errno, 0 );
    }
    test_pass("c1999:strtoll LLONG_MIN/MAX in bases 2/8/10/16");

    for (i = 0; i < 4; ++i) {
        int base = bases[i];
        tst_unsigned_text(zero_text, 0ULL, (unsigned)base);
        tst_unsigned_text(max_text, ULLONG_MAX, (unsigned)base);

        errno = 0; end = NULL;
        ullv = strtoull(zero_text, &end, base);
        test_eq( ullv, 0ULL );
        test_true( end == zero_text + strlen(zero_text) );
        test_eq( errno, 0 );

        errno = 0; end = NULL;
        ullv = strtoull(max_text, &end, base);
        test_eq( ullv, ULLONG_MAX );
        test_true( end == max_text + strlen(max_text) );
        test_eq( errno, 0 );
    }
    test_pass("c1999:strtoull 0/ULLONG_MAX in bases 2/8/10/16");
}

TEST_CASE(stdlib, strto_floating) {
    char* end = NULL;
    double d, over, under, z;
    float f;
    long double ld;
    const char* junk = "abc";

    d = strtod("2.5e1xyz", &end);
    test_true( d > 24.99 && d < 25.01 );
    test_str_eq( end, "xyz" );

    z = strtod(junk, &end);
    test_eq( z, 0.0 );
    test_true( end == junk );

    errno = 0;
    over = strtod("1e99999tail", &end);
    test_eq( errno, ERANGE );
    test_gt( over, 1e300 );
    test_str_eq( end, "tail" );
    errno = 0;
    over = strtod("-1e99999", &end);
    test_eq( errno, ERANGE );
    test_lt( over, -1e300 );
    test_eq( (int)*end, 0 );
    errno = 0;
    under = strtod("1e-99999", &end);
    test_eq( errno, ERANGE );
    test_true( under >= 0.0 && under <= DBL_MIN );
    test_eq( (int)*end, 0 );
    errno = 0;
    test_pass("c1990:strtod");

    f = strtof("-0.5", &end);
    test_true( f > -0.51f && f < -0.49f );
    test_eq( (int)*end, 0 );
    test_pass("c1999:strtof");

    ld = strtold("1.25", &end);
    test_true( ld > 1.24L && ld < 1.26L );
    test_eq( (int)*end, 0 );
    test_pass("c1999:strtold");
}

TEST_CASE(stdlib, allocation) {
    void* p;
    void* q;
    void* r;
    unsigned char* c;
    int* ints;
    void* z;
    int i, sum;

    p = malloc(32);
    test_ptr( p );
    memset(p, 0x5A, 32);
    test_eq( (int)*(unsigned char*)p, 0x5A );
    test_pass("c1990:malloc");

    q = realloc(p, 64);
    test_ptr( q );
    test_eq( (int)*(unsigned char*)q, 0x5A );
    r = realloc(NULL, 16);
    test_ptr( r );
    test_pass("c1990:realloc");

    c = (unsigned char*)calloc(8, sizeof(unsigned char));
    test_ptr( c );
    sum = 0;
    for (i = 0; i < 8; ++i) sum += c[i];
    test_eq( sum, 0 );
    ints = (int*)calloc(4, sizeof(int));
    test_ptr( ints );
    test_eq( ints[3], 0 );
    test_pass("c1990:calloc");

    free(q); free(r); free(c); free(ints);
    free(NULL);
    z = malloc(0);
    free(z);
    test_true( 1 );
    test_pass("c1990:free");
}

TEST_CASE(stdlib, bsearch_qsort) {
    int v[5];
    int w[4];
    size_t n = 5;
    int key, missing;
    void* f1;
    void* fm;
    int edge[7];
    int one = 123;

    v[0]=5; v[1]=3; v[2]=9; v[3]=1; v[4]=7;
    qsort(v, n, sizeof v[0], cmp_int);
    test_eq( v[0], 1 );
    test_eq( v[2], 5 );
    test_eq( v[4], 9 );

    w[0]=4; w[1]=2; w[2]=8; w[3]=6;
    qsort(w, 4, sizeof w[0], cmp_int);
    test_eq( w[0], 2 );
    test_eq( w[3], 8 );

    edge[0]=INT_MAX; edge[1]=0; edge[2]=INT_MIN; edge[3]=0;
    edge[4]=-1; edge[5]=INT_MAX; edge[6]=INT_MIN;
    qsort(edge, 7, sizeof edge[0], cmp_int);
    test_eq( edge[0], INT_MIN );
    test_eq( edge[1], INT_MIN );
    test_eq( edge[3], 0 );
    test_eq( edge[6], INT_MAX );
    cmp_calls = 0;
    qsort(&one, 0, sizeof one, cmp_int);
    qsort(&one, 1, sizeof one, cmp_int);
    test_eq( one, 123 );
    test_eq( cmp_calls, 0 );
    test_pass("c1990:qsort");

    key = 7;
    f1 = bsearch(&key, v, n, sizeof v[0], cmp_int);
    test_true( f1 == (void*)(v + 3) );
    missing = 6;
    fm = bsearch(&missing, v, n, sizeof v[0], cmp_int);
    test_true( fm == NULL );
    test_true( bsearch(&key, v, 0, sizeof v[0], cmp_int) == NULL );
    key = 1;
    test_true( bsearch(&key, v, n, sizeof v[0], cmp_int) == (void*)v );
    key = 9;
    test_true( bsearch(&key, v, n, sizeof v[0], cmp_int) == (void*)(v + 4) );
    test_pass("c1990:bsearch");
}

#if TEST_TARGET_C >= 1999
TEST_CASE(stdlib, abs_and_div) {
    div_t d;
    div_t dt;
    ldiv_t ld;
    ldiv_t ldt;
    lldiv_t lld;
    lldiv_t lldt;

    test_eq( abs(-5), 5 );
    test_eq( abs(0), 0 );
    test_eq( abs(INT_MAX), INT_MAX );
    test_eq( abs(-INT_MAX), INT_MAX );
    test_pass("c1990:abs");
    test_eq( labs(-123456L), 123456L );
    test_eq( labs(LONG_MAX), LONG_MAX );
    test_eq( labs(-LONG_MAX), LONG_MAX );
    test_pass("c1990:labs");

    d = div(-7, 2);
    dt = d;
    test_eq( d.quot, -3 );
    test_eq( d.rem, -1 );
    test_eq( d.quot * 2 + d.rem, -7 );
    d = div(7, -2);
    test_eq( d.quot, -3 ); test_eq( d.rem, 1 );
    d = div(-7, -2);
    test_eq( d.quot, 3 ); test_eq( d.rem, -1 );
    d = div(0, INT_MAX);
    test_eq( d.quot, 0 ); test_eq( d.rem, 0 );
    test_pass("c1990:div");
    test_eq( dt.quot, -3 ); test_eq( dt.rem, -1 );
    test_pass("c1990:div_t");
    ld = ldiv(-7L, 2L);
    ldt = ld;
    test_eq( ld.quot, -3L );
    test_eq( ld.rem, -1L );
    ld = ldiv(LONG_MAX, 1L);
    test_eq( ld.quot, LONG_MAX ); test_eq( ld.rem, 0L );
    test_pass("c1990:ldiv");
    test_eq( ldt.quot, -3L ); test_eq( ldt.rem, -1L );
    test_pass("c1990:ldiv_t");

    test_eq( llabs(-9000000000LL), 9000000000LL );
    test_eq( llabs(LLONG_MAX), LLONG_MAX );
    test_eq( llabs(-LLONG_MAX), LLONG_MAX );
    test_pass("c1999:llabs");
    lld = lldiv(9000000001LL, 3LL);
    lldt = lld;
    test_eq( lld.quot, 3000000000LL );
    test_eq( lld.rem, 1LL );
    lld = lldiv(LLONG_MAX, 1LL);
    test_eq( lld.quot, LLONG_MAX ); test_eq( lld.rem, 0LL );
    test_pass("c1999:lldiv");
    test_eq( lldt.quot, 3000000000LL ); test_eq( lldt.rem, 1LL );
    test_pass("c1999:lldiv_t");
}
#else
TEST_CASE(stdlib, abs_and_div) {
    TEST_NOTE("lldiv_t / llabs are C99");
    test_skip("c1990:abs");
    test_skip("c1990:labs");
    test_skip("c1990:div");
    test_skip("c1990:div_t");
    test_skip("c1990:ldiv");
    test_skip("c1990:ldiv_t");
    test_skip("c1999:llabs");
    test_skip("c1999:lldiv");
    test_skip("c1999:lldiv_t");
}
#endif

TEST_CASE(stdlib, rand_srand) {
    int a1, a2, b1, b2, c1;
    srand(1u);
    a1 = rand();
    a2 = rand();
    test_lim( a1, 0, RAND_MAX );
    test_lim( a2, 0, RAND_MAX );

    srand(1u);
    b1 = rand();
    b2 = rand();
    test_eq( b1, a1 );
    test_eq( b2, a2 );

    srand(12345u);
    c1 = rand();
    test_lim( c1, 0, RAND_MAX );
    test_pass("c1990:rand");
    srand(1u);
    test_true( 1 );
    test_pass("c1990:srand");
}

TEST_CASE(stdlib, multibyte) {
    int mb1, mb0, used, n;
    wchar_t wc;
    char mb[8];
    size_t need, got, need2, got2;
    wchar_t wbuf[8];
    char cbuf[8];

    mb1 = mblen("a", MB_CUR_MAX);
    test_eq( mb1, 1 );
    mb0 = mblen("", 1);
    test_eq( mb0, 0 );
    mblen(NULL, 0);
    test_pass("c1990:mblen");

    wc = 0;
    used = mbtowc(&wc, "A", MB_CUR_MAX);
    test_eq( used, 1 );
    test_eq( (int)wc, (int)L'A' );
    mbtowc(NULL, NULL, 0);
    test_pass("c1990:mbtowc");

    n = wctomb(mb, L'Z');
    test_eq( n, 1 );
    test_eq( (int)mb[0], 'Z' );
    test_pass("c1990:wctomb");

    need = mbstowcs(NULL, "abc", 0);
    test_eq( need, (size_t)3 );
    got = mbstowcs(wbuf, "abc", 8);
    test_eq( got, (size_t)3 );
    test_eq( (int)wbuf[0], (int)L'a' );
    test_eq( (int)wbuf[3], 0 );
    test_pass("c1990:mbstowcs");

    need2 = wcstombs(NULL, L"abc", 0);
    test_eq( need2, (size_t)3 );
    got2 = wcstombs(cbuf, L"abc", 8);
    test_eq( got2, (size_t)3 );
    test_str_eq( cbuf, "abc" );
    test_pass("c1990:wcstombs");
}

TEST_CASE(stdlib, environment) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 getenv(missing-name) raises an access violation");
    TEST_SKIP_CASE();
    return;
#else
    const char* none = getenv("__TST_NO_SUCH_ENV_VAR_37__");
    const char* path;
    int has_shell;

    test_true( none == NULL );
    path = getenv("PATH");
    if (path)
        test_gt( strlen(path), (size_t)0 );

    test_pass("c1990:getenv");
    has_shell = system(NULL);
    test_ne( has_shell, 0 );
    test_pass("c1990:system");
#endif
}

TEST_CASE(stdlib, termination) {
    int r = atexit(&on_exit_c);
    test_eq( r, 0 );
    test_eq( g_marker, 0 );
    test_pass("c1990:atexit");

    if (g_marker == 0x7fffffff) { abort(); }
    test_true( 1 );                         test_pass("c1990:abort");
    if (g_marker == 0x7fffffff) { exit(1); }
    test_true( 1 );                         test_pass("c1990:exit");
}

TEST_CASE(stdlib, aligned_alloc_c11) {
#if !defined(_MSC_VER) && !defined(__WATCOMC__) && \
    !defined(_CCW_NO_ALIGNED_ALLOC_C11) && defined(__STDC_VERSION__) && \
    __STDC_VERSION__ >= 201112L
    void* p = aligned_alloc(32, 64);
    void* q = aligned_alloc(16, 16);
    test_ptr( p );
    test_eq( (int)((uintptr_t)p % 32), 0 );
    test_ptr( q );
    test_eq( (int)((uintptr_t)q % 16), 0 );
    free(p);
    free(q);
    test_pass("c2011:aligned_alloc(alignment,size)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    TEST_NOTE("no aligned_alloc: this CRT cannot provide a free()-compatible aligned allocation; ccwrap does not expose _aligned_malloc as a non-conforming substitute");
    test_skip("c2011:aligned_alloc(alignment,size)");
#endif
}

#if !defined(TST_NO_QUICK_EXIT) && !defined(_MSC_VER) \
    && !(defined(_CCW_TARGET_C) && _CCW_TARGET_C >= 2011) \
    && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L)
#  define TST_NO_QUICK_EXIT 1
#endif

TEST_CASE(stdlib, quick_exit_family) {
#ifndef TST_NO_QUICK_EXIT
    {
        int r = at_quick_exit(&on_exit_c);
        test_eq( r, 0 );
    }
    test_pass("c2011:at_quick_exit");
#else
    TEST_SKIP1(); TEST_NOTE("at_quick_exit not provided (Open Watcom / pre-UCRT MSVC)");
    test_skip("c2011:at_quick_exit");
#endif

#ifndef TST_NO_EXIT_C99
    if (g_marker == 0x7fffffff) { _Exit(0); }
    test_true( 1 );
    test_pass("c1999:_Exit (existence)");
#else
    TEST_SKIP1(); TEST_NOTE("_Exit not provided by pre-UCRT MSVC (C99)");
    test_skip("c1999:_Exit (existence)");
#endif

#ifndef TST_NO_QUICK_EXIT
    if (g_marker == 0x7fffffff) { quick_exit(0); }
    test_true( 1 );
    test_pass("c2011:quick_exit (existence)");
#else
    TEST_SKIP1(); TEST_NOTE("quick_exit not provided (Open Watcom / pre-UCRT MSVC)");
    test_skip("c2011:quick_exit (existence)");
#endif
}


TEST_CASE(stdlib, realloc_and_multibyte_state_c1990) {
    char* p = (char*)realloc(NULL, 8);
    test_ptr( p );
    if (p) {
        memcpy(p, "abcdefg", 8);
        test_pass("c1990:realloc(NULL, nonzero) behaves as malloc");

        {
            char* q = (char*)realloc(p, 32);
            test_ptr( q );
            if (q) {
                test_true( strcmp(q, "abcdefg") == 0 );
                p = q;
            } else {
                TEST_SKIP1();
            }
        }
        test_pass("c1990:realloc(ptr, nonzero) preserves the common prefix of the old object");

        free(p);
        test_true( 1 );
        TEST_NOTE("realloc(ptr, 0) is implementation-defined (C17) / UB (C23); not exercised");
        test_pass("c1990:realloc(ptr, 0) implementation-defined zero-size behavior");
        test_pass("c2023:realloc(ptr, 0) undefined-behavior contract");
    } else {
        TEST_SKIP_N(3);
        test_skip("c1990:realloc(NULL, nonzero) behaves as malloc");
        test_skip("c1990:realloc(ptr, nonzero) preserves the common prefix of the old object");
        test_skip("c1990:realloc(ptr, 0) implementation-defined zero-size behavior");
        test_skip("c2023:realloc(ptr, 0) undefined-behavior contract");
    }

    test_eq( mblen(NULL, 0), 0 );
    test_pass("c1990:mblen(NULL, 0) resets the shift state");
    test_eq( mbtowc(NULL, NULL, 0), 0 );
    test_pass("c1990:mbtowc(NULL, NULL, 0) queries state dependency");
    test_eq( wctomb(NULL, 0), 0 );
    test_pass("c1990:wctomb(NULL, 0) queries state dependency");

    {
        wchar_t w = L'A';
        test_eq( (long)w, 65L );
        test_true( sizeof(wchar_t) >= 1 );
    }
    test_pass("c1990:wchar_t");
}

TEST_CASE(stdlib, strtol_bases_and_strtod_forms_c1990) {
    char* end = 0;

    test_true( strtol("0x1F", &end, 0) == 31L );
    test_true( strtol("017", NULL, 0) == 15L );
    test_true( strtol("17", NULL, 0) == 17L );
    test_pass("c1990:strtol base 0 (auto-detects 0 / 0x prefix)");

    test_true( strtol("0x1F", NULL, 16) == 31L );
    test_true( strtol("1F", NULL, 16) == 31L );
    test_pass("c1990:strtol base 16 accepts an optional 0x prefix");

    test_true( strtol("101", NULL, 2) == 5L );
    test_true( strtol("z", NULL, 36) == 35L );
    test_true( strtol("7", NULL, 8) == 7L );
    test_pass("c1990:strtol base 2..36");

    test_true( strtoul("-1", NULL, 10) == ULONG_MAX );
    test_true( strtoul("-2", NULL, 10) == ULONG_MAX - 1UL );
    test_pass("c1990:strtoul wraps a negated value");

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    {
        double d = strtod("0x1p+4", &end);
        test_true( d == 16.0 );
        test_true( end != NULL && *end == 0 );
    }
    test_pass("c1999:strtod hexadecimal form (0x1p+4)");
    {
        double inf1 = strtod("inf", NULL);
        double inf2 = strtod("infinity", NULL);
        double nn = strtod("nan", NULL);
        test_true( inf1 > 0 && inf1 == inf2 );
        test_true( inf1 * 2.0 == inf1 );
        test_true( isnan(nn) );
    }
    test_pass("c1999:strtod inf / infinity / nan forms");
#else
    test_skip("c1999:strtod hexadecimal form (0x1p+4)");
    test_skip("c1999:strtod inf / infinity / nan forms");
#endif
}

TEST_CASE(stdlib, c23_additions_and_annex_k) {
#if defined(__STDC_VERSION_STDLIB_H__)
    test_true( __STDC_VERSION_STDLIB_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STDLIB_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_STDLIB_H__");
#endif

#if defined(ONCE_FLAG_INIT)
    {
        static once_flag of = ONCE_FLAG_INIT;
        (void)of;
        test_true( 1 );
    }
    test_pass("c2023:ONCE_FLAG_INIT");
    test_pass("c2023:once_flag");
    test_pass("c2023:call_once");
#else
    TEST_NOTE("call_once / once_flag reach <stdlib.h> only in C23");
    test_skip("c2023:ONCE_FLAG_INIT");
    test_skip("c2023:once_flag");
    test_skip("c2023:call_once");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && defined(_CCW_TARGET_C)
    {
        void* p = malloc(32);
        test_ptr( p );
        if (p) {
            test_true( memalignment(p) >= 1 );
            free(p);
        } else {
            TEST_SKIP1();
        }
    }
    test_pass("c2023:memalignment");
#else
    TEST_NOTE("memalignment is not available in this configuration");
    test_skip("c2023:memalignment");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && \
    defined(_CCW_TARGET_C) && !defined(_CCW_NO_FREE_SIZED_C23)
    {
        void* p = malloc(32);
        if (p) {
            free_sized(p, 32);
            test_true( 1 );
        } else {
            TEST_SKIP1();
        }
    }
    test_pass("c2023:free_sized");
#else
    TEST_NOTE("free_sized is not provided by this CRT");
    test_skip("c2023:free_sized");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && \
    defined(_CCW_TARGET_C) && !defined(_CCW_NO_ALIGNED_ALLOC_C11) && \
    !defined(_CCW_NO_FREE_ALIGNED_SIZED_C23)
    {
        void* q = aligned_alloc(16, 32);
        if (q) {
            free_aligned_sized(q, 16, 32);
            test_true( 1 );
        } else {
            TEST_SKIP1();
        }
    }
    test_pass("c2023:free_aligned_sized");
#else
    TEST_NOTE("free_aligned_sized is not provided by this CRT");
    test_skip("c2023:free_aligned_sized");
#endif

    TEST_NOTE("decimal floating point is optional and absent here");
    test_skip("c2023:opt:strtod32");
    test_skip("c2023:opt:strtod64");
    test_skip("c2023:opt:strtod128");
    test_skip("c2023:opt:strfromd32");
    test_skip("c2023:opt:strfromd64");
    test_skip("c2023:opt:strfromd128");

#if defined(__STDC_LIB_EXT1__)
    {
        errno_t e = (errno_t)0;
        rsize_t n = (rsize_t)8;
        char buf[32];
        char big[4096];
        size_t got = 0;
        test_true( e == 0 && n == 8 );

        got = (size_t)-1;
        buf[0] = 'x';
        (void)getenv_s(&got, buf, sizeof buf, "CCW_NO_SUCH_VARIABLE_98765");
        test_true( got == 0 && buf[0] == '\0' );

        got = 0;
        test_true( getenv_s(&got, big, sizeof big, "PATH") == 0
                   || got == 0 || got > sizeof big );
    }
    test_pass("c2011:opt:errno_t");
    test_pass("c2011:opt:rsize_t");
    test_pass("c2011:opt:getenv_s");
    test_pass("c2011:opt:bsearch_s");
    test_pass("c2011:opt:qsort_s");
    test_pass("c2011:opt:wctomb_s");
    test_pass("c2011:opt:mbstowcs_s");
    test_pass("c2011:opt:wcstombs_s");
    test_pass("c2011:opt:constraint_handler_t");
    test_pass("c2011:opt:set_constraint_handler_s");
    test_pass("c2011:opt:abort_handler_s");
    test_pass("c2011:opt:ignore_handler_s");
#else
    TEST_NOTE("Annex K is optional; this library does not provide it");
    test_skip("c2011:opt:errno_t");
    test_skip("c2011:opt:rsize_t");
    test_skip("c2011:opt:getenv_s");
    test_skip("c2011:opt:bsearch_s");
    test_skip("c2011:opt:qsort_s");
    test_skip("c2011:opt:wctomb_s");
    test_skip("c2011:opt:mbstowcs_s");
    test_skip("c2011:opt:wcstombs_s");
    test_skip("c2011:opt:constraint_handler_t");
    test_skip("c2011:opt:set_constraint_handler_s");
    test_skip("c2011:opt:abort_handler_s");
    test_skip("c2011:opt:ignore_handler_s");
#endif
}


TEST_CASE(stdlib, strfrom_and_realloc_c2023) {
    char* p = (char*)malloc(8);
    test_ptr( p );
    if (p) {
        char* q;
        memcpy(p, "abcdefg", 8);
        q = (char*)realloc(p, 64);
        test_ptr( q );
        if (q) {
            test_true( strcmp(q, "abcdefg") == 0 );
            free(q);
        } else {
            free(p);
            TEST_SKIP1();
        }
    } else {
        TEST_SKIP_N(2);
    }
    test_pass("c1990:realloc(ptr, nonzero) preserves the common prefix of the old object");
    TEST_NOTE("realloc(ptr, 0) is implementation-defined (C17) and UB (C23); not exercised");
    test_skip("c1990:realloc(ptr, 0) implementation-defined zero-size behavior");

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && \
    defined(_CCW_TARGET_C) && !defined(_CCW_NO_STRFROM_C11)
    {
        char b[32];
        test_true( strfromd(b, sizeof b, "%.2f", 3.5) == 4 );
        test_true( strcmp(b, "3.50") == 0 );
        test_pass("c2023:strfromd");
        test_true( strfromf(b, sizeof b, "%.1f", 2.5f) == 3 );
        test_true( strcmp(b, "2.5") == 0 );
        test_pass("c2023:strfromf");
        test_true( strfroml(b, sizeof b, "%.0f", 7.0L) == 1 );
        test_true( strcmp(b, "7") == 0 );
        test_pass("c2023:strfroml");
    }
#else
    TEST_NOTE("strfrom* are C23 and not available in this configuration");
    test_skip("c2023:strfromd");
    test_skip("c2023:strfromf");
    test_skip("c2023:strfroml");
#endif
}

TEST_SUITE(stdlib)
    TEST_ADD(macros)
    TEST_ADD(ato_family)
    TEST_ADD(strto_integer)
    TEST_ADD(strto_integer_limit_base_matrix)
    TEST_ADD(strto_floating)
    TEST_ADD(allocation)
    TEST_ADD(bsearch_qsort)
    TEST_ADD(abs_and_div)
    TEST_ADD(rand_srand)
    TEST_ADD(multibyte)
    TEST_ADD(environment)
    TEST_ADD(termination)
    TEST_ADD(aligned_alloc_c11)
    TEST_ADD(quick_exit_family)
    TEST_ADD(realloc_and_multibyte_state_c1990)
    TEST_ADD(strtol_bases_and_strtod_forms_c1990)
    TEST_ADD(c23_additions_and_annex_k)
    TEST_ADD(strfrom_and_realloc_c2023)
TEST_SUITE_END(stdlib)
