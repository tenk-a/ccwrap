#include "c_test.h"

#include <wchar.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <wctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <float.h>

#if defined(_MSC_VER) && _MSC_VER < 1900
# define TST_NO_WIDE_VSCANF 1
#endif

#define TST_WCHAR_BULK_SIZE 2048u
static wchar_t tst_wchar_bulk_a[TST_WCHAR_BULK_SIZE + 2u];
static wchar_t tst_wchar_bulk_b[TST_WCHAR_BULK_SIZE + 2u];

static int vsp(wchar_t* buf, size_t n, const wchar_t* fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vswprintf(buf, n, fmt, ap);
    va_end(ap);
    return r;
}
static int vfwp(FILE* fp, const wchar_t* fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vfwprintf(fp, fmt, ap);
    va_end(ap);
    return r;
}
#ifndef TST_NO_WIDE_VSCANF
static int vfws(FILE* fp, const wchar_t* fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vfwscanf(fp, fmt, ap);
    va_end(ap);
    return r;
}
#endif

TEST_CASE(wchar, types_and_macros) {
    size_t n = sizeof(wchar_t);
    mbstate_t st;
    wint_t wi, eof;
    wchar_t* wp = NULL;

    test_gt( n, (size_t)0 );
    test_true( wp == NULL );
    test_pass("c1995:wchar_t");
    test_true( (void*)NULL == (void*)0 );  test_pass("c1995:NULL");

    wi = (wint_t)L'a';
    test_eq( (long)wi, (long)L'a' );
    test_ge( sizeof(wint_t), sizeof(wchar_t) );
    test_pass("c1995:wint_t");

    memset(&st, 0, sizeof st);
    test_gt( sizeof(mbstate_t), (size_t)0 );
    test_pass("c1995:mbstate_t");

    eof = WEOF;
    test_ne( (long)eof, (long)L'a' );
    test_ne( (long)eof, 0L );
    test_pass("c1995:WEOF");

    test_le( (long)WCHAR_MIN, 0L );
    test_lim( (long)L'A', (long)WCHAR_MIN, (long)WCHAR_MAX );
    test_pass("c1995:WCHAR_MIN");
    test_ge( (long)WCHAR_MAX, 0xFFFFL );
    test_eq( (long)WCHAR_MAX, (long)(wchar_t)WCHAR_MAX );
    test_pass("c1995:WCHAR_MAX");
}

TEST_CASE(wchar, wcslen_copy_cat) {
    wchar_t buf[32];
    wchar_t pad[6];
    wchar_t* r;
    wchar_t* p;

    test_eq( wcslen(L""), (size_t)0 );
    test_eq( wcslen(L"hello"), (size_t)5 );
    test_pass("c1995:wcslen");

    r = wcscpy(buf, L"abc");
    test_true( r == buf );
    test_eq( wcscmp(buf, L"abc"), 0 );
    test_eq( wcslen(buf), (size_t)3 );
    test_pass("c1995:wcscpy");

    wmemset(pad, L'#', 6);
    p = wcsncpy(pad, L"ab", 5);
    test_true( p == pad );
    test_eq( wcscmp(pad, L"ab"), 0 );
    test_eq( (int)pad[2], 0 );
    test_eq( (int)pad[4], 0 );
    test_eq( (int)pad[5], (int)L'#' );
    test_pass("c1995:wcsncpy");

    wmemset(pad, L'#', 6);
    wcsncpy(pad, L"abcd", 4);
    test_eq( (int)pad[3], (int)L'd' );
    test_eq( (int)pad[4], (int)L'#' );
    wcsncpy(pad, L"ignored", 0);
    test_eq( (int)pad[0], (int)L'a' );

    wcscpy(buf, L"ab");
    test_true( wcscat(buf, L"cd") == buf );
    test_eq( wcscmp(buf, L"abcd"), 0 );
    test_pass("c1995:wcscat");

    wcscpy(buf, L"ab");
    test_true( wcsncat(buf, L"cdef", 2) == buf );
    test_eq( wcscmp(buf, L"abcd"), 0 );
    test_true( wcsncat(buf, L"ignored", 0) == buf );
    test_eq( wcscmp(buf, L"abcd"), 0 );
    test_pass("c1995:wcsncat");
}

TEST_CASE(wchar, wcscmp_and_friends) {
    wchar_t buf[16];

    test_eq( wcscmp(L"abc", L"abc"), 0 );
    test_lt( wcscmp(L"abc", L"abd"), 0 );
    test_gt( wcscmp(L"abd", L"abc"), 0 );
    test_lt( wcscmp(L"ab", L"abc"), 0 );
    test_eq( wcscmp(L"", L""), 0 );
    test_pass("c1995:wcscmp");

    test_eq( wcsncmp(L"abcdef", L"abcxxx", 3), 0 );
    test_lt( wcsncmp(L"abcdef", L"abcxxx", 4), 0 );
    test_eq( wcsncmp(L"abc", L"xyz", 0), 0 );
    test_pass("c1995:wcsncmp");

    test_eq( wcscoll(L"abc", L"abc"), 0 );
    test_lt( wcscoll(L"abc", L"abd"), 0 );
    test_pass("c1995:wcscoll");
    test_eq( wcsxfrm(buf, L"abc", 16), wcslen(L"abc") );
    test_eq( wcsxfrm(NULL, L"abc", 0), (size_t)3 );
    test_pass("c1995:wcsxfrm");
}

TEST_CASE(wchar, search) {
    const wchar_t* s = L"hello world";
    wchar_t* f;

    f = wcschr(s, L'w');
    test_true( f != NULL );
    test_eq( (int)*f, (int)L'w' );
    test_eq( (int)(f - s), 6 );
    test_true( wcschr(s, L'z') == NULL );
    test_pass("c1995:wcschr");
    test_true( wcschr(s, L'\0') == s + wcslen(s) );

    {
        const wchar_t* t = L"abcabc";
        f = wcsrchr(t, L'b');
        test_true( f != NULL );
        test_eq( (int)(f - t), 4 );
    test_pass("c1995:wcsrchr");
    }

    f = wcsstr(s, L"world");
    test_true( f != NULL );
    test_eq( (int)(f - s), 6 );
    test_true( wcsstr(s, L"") == s );
    test_true( wcsstr(s, L"nope") == NULL );
    test_pass("c1995:wcsstr");

    test_eq( wcsspn(L"aabbcc", L"ab"), (size_t)4 );
    test_eq( wcsspn(L"xyz", L"ab"), (size_t)0 );
    test_eq( wcsspn(L"", L"ab"), (size_t)0 );
    test_pass("c1995:wcsspn");
    test_eq( wcscspn(L"aabbcc", L"c"), (size_t)4 );
    test_eq( wcscspn(L"abc", L"xyz"), (size_t)3 );
    test_eq( wcscspn(L"abc", L""), (size_t)3 );
    test_pass("c1995:wcscspn");

    f = wcspbrk(L"hello", L"lo");
    test_true( f != NULL );
    test_eq( (int)*f, (int)L'l' );
    test_true( wcspbrk(L"hello", L"xyz") == NULL );
    test_true( wcspbrk(L"hello", L"") == NULL );
    test_pass("c1995:wcspbrk");
}

TEST_CASE(wchar, wcstok_split) {
    wchar_t str[32];
    wchar_t* state = NULL;
    wchar_t* t;

    wcscpy(str, L"a,b,,c");
    t = wcstok(str, L",", &state);
    test_true( t != NULL );
    test_eq( wcscmp(t, L"a"), 0 );
    t = wcstok(NULL, L",", &state);
    test_true( t != NULL );
    test_eq( wcscmp(t, L"b"), 0 );
    t = wcstok(NULL, L",", &state);
    test_true( t != NULL );
    test_eq( wcscmp(t, L"c"), 0 );
    test_true( wcstok(NULL, L",", &state) == NULL );
    test_pass("c1995:wcstok");
}

TEST_CASE(wchar, wmem) {
    wchar_t a[8], b[8];
    wchar_t* f;
    int i, sum;

    wmemset(a, L'x', 4);
    a[4] = 0;
    test_eq( wcslen(a), (size_t)4 );
    test_eq( (int)a[0], (int)L'x' );
    test_pass("c1995:wmemset");

    test_true( wmemcpy(b, a, 5) == b );
    test_eq( wcscmp(b, a), 0 );
    test_pass("c1995:wmemcpy");

    test_eq( wmemcmp(a, b, 4), 0 );
    b[2] = L'y';
    test_lt( wmemcmp(a, b, 4), 0 );
    test_eq( wmemcmp(a, b, 2), 0 );
    test_pass("c1995:wmemcmp");

    f = wmemchr(a, L'x', 4);
    test_true( f == a );
    test_true( wmemchr(a, L'z', 4) == NULL );
    test_true( wmemchr(a, L'x', 0) == NULL );
    test_true( wmemchr(a, L'x', 4) == a );
    test_pass("c1995:wmemchr");

    wcscpy(a, L"abcdef");
    test_true( wmemmove(a + 1, a, 5) == a + 1 );
    test_eq( wcsncmp(a, L"aabcde", 6), 0 );
    test_true( wmemmove(a, a, 6) == a );
    test_eq( wcsncmp(a, L"aabcde", 6), 0 );
    sum = 0;
    for (i = 0; i < 4; ++i) sum += (b[i] != 0);
    test_eq( sum, 4 );
    test_pass("c1995:wmemmove");
}

TEST_CASE(wchar, bulk_wide_string_and_memory) {
    size_t i;
    size_t n = (size_t)TST_WCHAR_BULK_SIZE;
    int ok;

    for (i = 0; i < n; ++i)
        tst_wchar_bulk_a[i] = L'a';
    tst_wchar_bulk_a[n - 3u] = L'x';
    tst_wchar_bulk_a[n - 2u] = L'y';
    tst_wchar_bulk_a[n - 1u] = L'z';
    tst_wchar_bulk_a[n] = 0;

    test_eq( wcslen(tst_wchar_bulk_a), n );
    test_true( wcscpy(tst_wchar_bulk_b, tst_wchar_bulk_a) == tst_wchar_bulk_b );
    test_eq( wcscmp(tst_wchar_bulk_b, tst_wchar_bulk_a), 0 );
    test_eq( (int)tst_wchar_bulk_b[n], 0 );
    test_pass("c1995:wcslen/wcscpy bulk exact-fit terminator");

    wmemset(tst_wchar_bulk_b, L'#', n + 2u);
    test_true( wmemcpy(tst_wchar_bulk_b + 1, tst_wchar_bulk_a, n)
               == tst_wchar_bulk_b + 1 );
    test_eq( wmemcmp(tst_wchar_bulk_b + 1, tst_wchar_bulk_a, n), 0 );
    test_eq( (int)tst_wchar_bulk_b[0], (int)L'#' );
    test_eq( (int)tst_wchar_bulk_b[n + 1u], (int)L'#' );
    test_pass("c1995:wmemcpy/wmemcmp bulk with guards");

    test_true( wmemchr(tst_wchar_bulk_a, L'x', n - 3u) == NULL );
    test_true( wmemchr(tst_wchar_bulk_a, L'x', n) == tst_wchar_bulk_a + n - 3u );
    test_true( wcschr(tst_wchar_bulk_a, L'a') == tst_wchar_bulk_a );
    test_true( wcsrchr(tst_wchar_bulk_a, L'a') == tst_wchar_bulk_a + n - 4u );
    test_true( wcsstr(tst_wchar_bulk_a, L"xyz") == tst_wchar_bulk_a + n - 3u );
    test_eq( wcsspn(tst_wchar_bulk_a, L"a"), n - 3u );
    test_eq( wcscspn(tst_wchar_bulk_a, L"xyz"), n - 3u );
    test_pass("c1995:wide search bulk near-end boundaries");

    for (i = 0; i < n; ++i)
        tst_wchar_bulk_b[i + 1u] = (wchar_t)(L'A' + (wchar_t)(i % 23u));
    test_true( wmemmove(tst_wchar_bulk_b + 2, tst_wchar_bulk_b + 1, n)
               == tst_wchar_bulk_b + 2 );
    ok = 1;
    for (i = 0; i < n; ++i)
        ok = ok && (tst_wchar_bulk_b[i + 2u]
                    == (wchar_t)(L'A' + (wchar_t)(i % 23u)));
    test_true( ok );
    test_true( wmemmove(tst_wchar_bulk_b + 1, tst_wchar_bulk_b + 2, n)
               == tst_wchar_bulk_b + 1 );
    test_pass("c1995:wmemmove bulk overlap in both directions");
}

#if TEST_TARGET_C >= 1999
TEST_CASE(wchar, wcsto_numeric) {
    wchar_t* end = NULL;
    long l;
    unsigned long ul;
    double d;
    float f;
    long long ll;
    unsigned long long ull;
    wchar_t lim[3 * sizeof(unsigned long long) * CHAR_BIT + 8];
    size_t n;

    l = wcstol(L"  123rest", &end, 10);
    test_eq( l, 123L );
    test_true( end != NULL );
    test_eq( (int)*end, (int)L'r' );

    test_eq( wcstol(L"ff", NULL, 16), 255L );
    test_eq( wcstol(L"0x10", NULL, 0), 16L );
    test_eq( wcstol(L"-101", NULL, 2), -5L );
    test_eq( wcstol(L"zz", NULL, 10), 0L );
    swprintf(lim, sizeof lim / sizeof lim[0], L"%ld", LONG_MAX);
    errno = 0;
    test_eq( wcstol(lim, &end, 10), LONG_MAX );
    test_eq( errno, 0 );
    test_eq( (int)*end, 0 );
    n = wcslen(lim); lim[n] = L'0'; lim[n + 1] = L'\0';
    errno = 0;
    test_eq( wcstol(lim, &end, 10), LONG_MAX );
    test_eq( errno, ERANGE );
    test_eq( (int)*end, 0 );
    test_pass("c1995:wcstol");

    ul = wcstoul(L"4294967295", NULL, 10);
    test_eq( ul, 4294967295UL );
    swprintf(lim, sizeof lim / sizeof lim[0], L"%lu", ULONG_MAX);
    errno = 0;
    test_eq( wcstoul(lim, &end, 10), ULONG_MAX );
    test_eq( errno, 0 );
    n = wcslen(lim); lim[n] = L'0'; lim[n + 1] = L'\0';
    errno = 0;
    test_eq( wcstoul(lim, &end, 10), ULONG_MAX );
    test_eq( errno, ERANGE );
    test_pass("c1995:wcstoul");

    d = wcstod(L"2.5e1xyz", &end);
    test_true( d > 24.99 && d < 25.01 );
    test_true( end != NULL );
    test_eq( (int)*end, (int)L'x' );
    errno = 0;
    d = wcstod(L"1e99999tail", &end);
    test_eq( errno, ERANGE );
    test_gt( d, 1e300 );
    test_true( wcscmp(end, L"tail") == 0 );
    errno = 0;
    d = wcstod(L"1e-99999", &end);
    test_eq( errno, ERANGE );
    test_true( d >= 0.0 && d <= DBL_MIN );
    test_eq( (int)*end, 0 );
    test_pass("c1995:wcstod");

    ll = wcstoll(L"-9000000000", NULL, 10);
    test_eq( ll, -9000000000LL );
    test_pass("c1999:wcstoll");
    ull = wcstoull(L"18446744073709551615", NULL, 10);
    test_eq( ull, ULLONG_MAX );
    test_pass("c1999:wcstoull");

    f = wcstof(L"-0.5", NULL);
    test_true( f > -0.51f && f < -0.49f );
    test_pass("c1999:wcstof");
    test_true( wcstold(L"1.25", NULL) > 1.24L );
    test_pass("c1999:wcstold");
}
#else
TEST_CASE(wchar, wcsto_numeric) {
    TEST_NOTE("wcstoll / wcstoull and ULLONG_MAX are C99");
    test_skip("c1995:wcstol");
    test_skip("c1995:wcstoul");
    test_skip("c1995:wcstod");
    test_skip("c1999:wcstoll");
    test_skip("c1999:wcstoull");
    test_skip("c1999:wcstof");
    test_skip("c1999:wcstold");
}
#endif

TEST_CASE(wchar, swprintf_swscanf) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 hangs on swprintf with multiple conversions");
    TEST_SKIP_CASE();
    return;
#else
    wchar_t buf[64];
    int n, a = 0, b = 0;
    wchar_t word[16];

    n = swprintf(buf, 64, L"%d-%d", 12, 34);
    test_eq( n, 5 );
    test_eq( wcscmp(buf, L"12-34"), 0 );

    n = swprintf(buf, 64, L"%ls", L"wide");
    test_eq( n, 4 );
    test_eq( wcscmp(buf, L"wide"), 0 );

    swprintf(buf, 64, L"%f", 1.5);
    test_eq( wcscmp(buf, L"1.500000"), 0 );

    swprintf(buf, 64, L"%c%lc", 'A', (wint_t)L'B');
    test_eq( wcscmp(buf, L"AB"), 0 );
    test_pass("c1995:swprintf");

    n = vsp(buf, 64, L"%d/%d", 7, 8);
    test_eq( n, 3 );
    test_eq( wcscmp(buf, L"7/8"), 0 );
    test_pass("c1995:vswprintf");

    n = swscanf(L"12-34", L"%d-%d", &a, &b);
    test_eq( n, 2 );
    test_eq( a, 12 );
    test_eq( b, 34 );
    n = swscanf(L"hello 5", L"%ls %d", word, &a);
    test_eq( n, 2 );
    test_eq( wcscmp(word, L"hello"), 0 );
    test_eq( a, 5 );
    test_pass("c1995:swscanf");
#endif
}

TEST_CASE(wchar, wide_file_streams) {
    FILE* fp = tmpfile();
    wchar_t line[32];
    wint_t c;

    test_ptr( fp );
    if (!fp) return;

    test_gt( fwprintf(fp, L"%ls %d\n", L"abc", 42), 0 );
    test_pass("c1995:fwprintf");
    test_true( fputws(L"second\n", fp) >= 0 );
    test_pass("c1995:fputws");
    test_ne( (long)fputwc(L'z', fp), (long)WEOF );
    test_pass("c1995:fputwc");

    rewind(fp);

    test_true( fgetws(line, 32, fp) == line );
    test_eq( wcscmp(line, L"abc 42\n"), 0 );
    test_true( fgetws(line, 32, fp) == line );
    test_eq( wcscmp(line, L"second\n"), 0 );
    test_pass("c1995:fgetws");

    c = fgetwc(fp);
    test_eq( (long)c, (long)L'z' );
    test_pass("c1995:fgetwc");
    test_ne( (long)ungetwc(c, fp), (long)WEOF );
    test_eq( (long)fgetwc(fp), (long)L'z' );
    test_eq( (long)fgetwc(fp), (long)WEOF );
    test_pass("c1995:ungetwc");

    fclose(fp);
}

TEST_CASE(wchar, wcsftime_formats) {
    struct tm tmv;
    wchar_t buf[64];
    size_t n;

    memset(&tmv, 0, sizeof tmv);
    tmv.tm_year = 100;
    tmv.tm_mon  = 0;
    tmv.tm_mday = 2;
    tmv.tm_hour = 3;
    tmv.tm_min  = 4;
    tmv.tm_sec  = 5;
    tmv.tm_wday = 0;
    tmv.tm_yday = 1;

    n = wcsftime(buf, 64, L"%Y-%m-%d", &tmv);
    test_eq( n, (size_t)10 );
    test_eq( wcscmp(buf, L"2000-01-02"), 0 );

    n = wcsftime(buf, 64, L"%H:%M:%S", &tmv);
    test_eq( n, (size_t)8 );
    test_eq( wcscmp(buf, L"03:04:05"), 0 );

    test_eq( wcsftime(buf, 4, L"%Y-%m-%d", &tmv), (size_t)0 );
    test_pass("c1995:wcsftime");
}

TEST_CASE(wchar, btowc_wctob_mbsinit) {
    mbstate_t st;

    test_eq( (long)btowc('A'), (long)L'A' );
    test_eq( (long)btowc(EOF), (long)WEOF );
    test_pass("c1995:btowc");
    test_eq( wctob(L'A'), (int)'A' );
    test_eq( wctob(L'\0'), 0 );
    TEST_SKIP_WAT("");
    test_eq( wctob(WEOF), EOF );
    test_pass("c1995:wctob");

    memset(&st, 0, sizeof st);
    test_true( mbsinit(&st) );
    test_true( mbsinit(NULL) );
    test_pass("c1995:mbsinit");
}

TEST_CASE(wchar, mbrtowc_wcrtomb_mbrlen) {
    mbstate_t st;
    wchar_t wc = 0;
    char mb[8];
    size_t n;

    memset(&st, 0, sizeof st);
    n = mbrtowc(&wc, "A", 1, &st);
    test_eq( n, (size_t)1 );
    test_eq( (int)wc, (int)L'A' );

    memset(&st, 0, sizeof st);
    n = mbrtowc(&wc, "", 1, &st);
    test_eq( n, (size_t)0 );
    test_eq( (int)wc, 0 );

    memset(&st, 0, sizeof st);
    n = mbrtowc(&wc, "A", 0, &st);
    TEST_SKIP_UCRT("");
    test_eq( n, (size_t)-2 );
    test_pass("c1995:mbrtowc");

    memset(&st, 0, sizeof st);
    n = wcrtomb(mb, L'Z', &st);
    test_eq( n, (size_t)1 );
    test_eq( (int)mb[0], 'Z' );
    test_pass("c1995:wcrtomb");

    memset(&st, 0, sizeof st);
    test_eq( mbrlen("A", 1, &st), (size_t)1 );
    memset(&st, 0, sizeof st);
    test_eq( mbrlen("", 1, &st), (size_t)0 );
    test_pass("c1995:mbrlen");
}

TEST_CASE(wchar, mbsrtowcs_wcsrtombs) {
    mbstate_t st;
    const char* src = "abc";
    const wchar_t* wsrc = L"abc";
    wchar_t wbuf[8];
    char cbuf[8];
    size_t n;

    memset(&st, 0, sizeof st);
    n = mbsrtowcs(NULL, &src, 0, &st);
    test_eq( n, (size_t)3 );
    test_true( src != NULL );

    memset(&st, 0, sizeof st);
    wmemset(wbuf, L'.', 8);
    src = "abc";
    n = mbsrtowcs(wbuf, &src, 8, &st);
    test_eq( n, (size_t)3 );
    test_true( src == NULL );
    test_eq( (int)wbuf[0], (int)L'a' );
    test_eq( (int)wbuf[2], (int)L'c' );
    TEST_SKIP_WAT("");
    test_eq( wcscmp(wbuf, L"abc"), 0 );
    test_pass("c1995:mbsrtowcs");

    memset(&st, 0, sizeof st);
    n = wcsrtombs(NULL, &wsrc, 0, &st);
    test_eq( n, (size_t)3 );
    memset(&st, 0, sizeof st);
    wsrc = L"abc";
    n = wcsrtombs(cbuf, &wsrc, 8, &st);
    test_eq( n, (size_t)3 );
    test_true( cbuf[0] == 'a' && cbuf[3] == '\0' );
    test_true( wsrc == NULL );
    test_pass("c1995:wcsrtombs");
}

TEST_CASE(wchar, wide_stdio_extras) {

    FILE* fp = tmpfile();
    test_ptr( fp );
    if (fp) {
        int a = 0, b = 0;
        test_gt( fwide(fp, 1), 0 );
        test_gt( vfwp(fp, L"%d %d", 5, 6), 0 );
        test_ne( (long)putwc(L'!', fp), (long)WEOF );
        rewind(fp);
        test_eq( fwscanf(fp, L"%d %d", &a, &b), 2 );
        test_eq( a, 5 );
        test_eq( b, 6 );
        test_ne( (long)getwc(fp), (long)WEOF );
        fclose(fp);
        test_pass("c1995:fwide");
        test_pass("c1995:vfwprintf");
        test_pass("c1995:fwscanf");
        test_pass("c1995:putwc");
        test_pass("c1995:getwc");
    } else {
        int k; for (k = 0; k < 5; ++k) TEST_SKIP1();
        test_skip("c1995:fwide"); test_skip("c1995:vfwprintf"); test_skip("c1995:fwscanf");
        test_skip("c1995:putwc"); test_skip("c1995:getwc");
    }

#ifndef TST_NO_WIDE_VSCANF
    {
        FILE* fp2 = tmpfile();
        test_ptr( fp2 );
        if (fp2) {
            int x = 0, y = 0;
            fwprintf(fp2, L"%d %d", 8, 9);
            rewind(fp2);
            test_eq( vfws(fp2, L"%d %d", &x, &y), 2 );
            test_eq( x, 8 );
            fclose(fp2);
        }
    }
    test_pass("c1999:vfwscanf");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    TEST_NOTE("vfwscanf not declared by pre-UCRT MSVC (C99)");
    test_skip("c1999:vfwscanf");
#endif
}

#if TEST_TARGET_C >= 1999
TEST_CASE(wchar, wide_stdio_existence) {

    int   (*p_wprintf)(const wchar_t*, ...)         = wprintf;
    int   (*p_wscanf)(const wchar_t*, ...)          = wscanf;
    wint_t(*p_getwchar)(void)                       = getwchar;
    wint_t(*p_putwchar)(wchar_t)                    = putwchar;
    int   (*p_vwprintf)(const wchar_t*, va_list)    = vwprintf;

    test_ptr( (void*)p_wprintf );  test_pass("c1995:wprintf (existence)");
    test_ptr( (void*)p_wscanf );   test_pass("c1995:wscanf (existence)");
    test_ptr( (void*)p_getwchar ); test_pass("c1995:getwchar (existence)");
    test_ptr( (void*)p_putwchar ); test_pass("c1995:putwchar (existence)");
    test_ptr( (void*)p_vwprintf ); test_pass("c1995:vwprintf (existence)");

#ifndef TST_NO_WIDE_VSCANF
    {
        int (*p_vwscanf)(const wchar_t*, va_list)               = vwscanf;
        int (*p_vswscanf)(const wchar_t*, const wchar_t*, va_list) = vswscanf;
        test_ptr( (void*)p_vwscanf );
        test_pass("c1999:vwscanf (existence)");
        test_ptr( (void*)p_vswscanf );
        test_pass("c1999:vswscanf (existence)");
    }
#else
    TEST_SKIP1(); test_skip("c1999:vwscanf (existence)");
    TEST_SKIP1(); TEST_NOTE("vwscanf / vswscanf not declared by pre-UCRT MSVC (C99)");
    test_skip("c1999:vswscanf (existence)");
#endif
}
#else
TEST_CASE(wchar, wide_stdio_existence) {
    TEST_NOTE("the v*w*scanf family is C99");
    test_skip("c1995:wprintf (existence)");
    test_skip("c1995:wscanf (existence)");
    test_skip("c1995:getwchar (existence)");
    test_skip("c1995:putwchar (existence)");
    test_skip("c1995:vwprintf (existence)");
    test_skip("c1999:vwscanf (existence)");
    test_skip("c1999:vswscanf (existence)");
}
#endif

static int ccw_w_same(const wchar_t* w, const char* n) {
    size_t i = 0;
    for (; n[i]; ++i) {
        if (w[i] == 0) return 0;
        if ((wchar_t)(unsigned char)n[i] != w[i]) return 0;
    }
    return w[i] == 0;
}

TEST_CASE(wchar, swprintf_specifiers_a_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 crashes on swprintf(L\"%#.0f\", 3.5)");
    TEST_SKIP_CASE();
    return;
#else
    char nb[128];
    wchar_t wb[128];
    sprintf(nb, "%#.0f", 3.5);
    swprintf(wb, 128, L"%#.0f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#.0f (# keeps the decimal point)");
    sprintf(nb, "%#.0o", 42u);
    swprintf(wb, 128, L"%#.0o", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#.0o (# forces a leading zero)");
    sprintf(nb, "%#010x", 42u);
    swprintf(wb, 128, L"%#010x", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#010x");
    sprintf(nb, "%#X", 42u);
    swprintf(wb, 128, L"%#X", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#X");
    sprintf(nb, "%#e", 3.5);
    swprintf(wb, 128, L"%#e", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#e (# keeps the decimal point)");
    sprintf(nb, "%#g", 3.5);
    swprintf(wb, 128, L"%#g", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#g");
    sprintf(nb, "%#o", 42u);
    swprintf(wb, 128, L"%#o", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#o");
    sprintf(nb, "%#x", 42u);
    swprintf(wb, 128, L"%#x", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %#x");
    sprintf(nb, "%%");
    swprintf(wb, 128, L"%%");
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %%");
    sprintf(nb, "%*d", 6, 42);
    swprintf(wb, 128, L"%*d", 6, 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %*d (width from argument)");
    sprintf(nb, "%*d", 6, 42);
    swprintf(wb, 128, L"%*d", 6, 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %*d with negative width (acts as - flag)");
    sprintf(nb, "%+.2f", 3.5);
    swprintf(wb, 128, L"%+.2f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %+.2f");
    sprintf(nb, "%+08.2f", 3.5);
    swprintf(wb, 128, L"%+08.2f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %+08.2f");
    sprintf(nb, "%+d", 42);
    swprintf(wb, 128, L"%+d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %+d");
    sprintf(nb, "%-+10d", 42);
    swprintf(wb, 128, L"%-+10d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %-+10d");
    sprintf(nb, "%-05d", 42);
    swprintf(wb, 128, L"%-05d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %-05d (- overrides 0)");
    sprintf(nb, "%-10.3f", 3.5);
    swprintf(wb, 128, L"%-10.3f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %-10.3f");
    sprintf(nb, "%-10.6g", 3.5);
    swprintf(wb, 128, L"%-10.6g", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %-10.6g");
    sprintf(nb, "%-5d", 42);
    swprintf(wb, 128, L"%-5d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %-5d");
    sprintf(nb, "%-8.3f", 3.5);
    swprintf(wb, 128, L"%-8.3f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %-8.3f");
    sprintf(nb, "%.*f", 6, 42.0);
    swprintf(wb, 128, L"%.*f", 6, 42.0);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.*f (precision from argument)");
    sprintf(nb, "%.0d", 42);
    swprintf(wb, 128, L"%.0d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.0d with value 0 (no characters)");
#endif
}

TEST_CASE(wchar, swprintf_specifiers_b_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 crashes in wide floating-point formatting");
    TEST_SKIP_CASE();
    return;
#else
    char nb[128];
    wchar_t wb[128];
    sprintf(nb, "%.0e", 3.5);
    swprintf(wb, 128, L"%.0e", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.0e");
    sprintf(nb, "%.0f", 3.5);
    swprintf(wb, 128, L"%.0f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.0f (round-to-nearest of the decimal point)");
    sprintf(nb, "%.2f", 3.5);
    swprintf(wb, 128, L"%.2f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.2f");
    sprintf(nb, "%.3d", 42);
    swprintf(wb, 128, L"%.3d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.3d");
    sprintf(nb, "%.6g", 3.5);
    swprintf(wb, 128, L"%.6g", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.6g");
    sprintf(nb, "%010.3f", 3.5);
    swprintf(wb, 128, L"%010.3f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %010.3f");
    sprintf(nb, "%05.3d", 42);
    swprintf(wb, 128, L"%05.3d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %05.3d (0 flag ignored when a precision is given)");
    sprintf(nb, "%05d", 42);
    swprintf(wb, 128, L"%05d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %05d");
    sprintf(nb, "%10.3e", 3.5);
    swprintf(wb, 128, L"%10.3e", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %10.3e");
    sprintf(nb, "%10.3f", 3.5);
    swprintf(wb, 128, L"%10.3f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %10.3f");
    sprintf(nb, "%5c", 'A');
    swprintf(wb, 128, L"%5c", 'A');
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %5c (width on %c)");
    sprintf(nb, "%5d", 42);
    swprintf(wb, 128, L"%5d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %5d");
    sprintf(nb, "%8.3d", 42);
    swprintf(wb, 128, L"%8.3d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %8.3d");
    sprintf(nb, "%E", 3.5);
    swprintf(wb, 128, L"%E", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %E");
    sprintf(nb, "%G", 3.5);
    swprintf(wb, 128, L"%G", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %G");
    sprintf(nb, "%Le", 3.5L);
    swprintf(wb, 128, L"%Le", 3.5L);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %Le");
    sprintf(nb, "%Lf", 3.5L);
    swprintf(wb, 128, L"%Lf", 3.5L);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %Lf");
    sprintf(nb, "%Lg", 3.5L);
    swprintf(wb, 128, L"%Lg", 3.5L);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %Lg");
    sprintf(nb, "%X", 42u);
    swprintf(wb, 128, L"%X", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %X");
    sprintf(nb, "%c", 'A');
    swprintf(wb, 128, L"%c", 'A');
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %c");
    sprintf(nb, "%d", 42);
    swprintf(wb, 128, L"%d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %d");
    sprintf(nb, "%e", 3.5);
    swprintf(wb, 128, L"%e", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %e");
#endif
}

TEST_CASE(wchar, swprintf_specifiers_c_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 crashes in wide floating-point formatting");
    TEST_SKIP_CASE();
    return;
#else
    char nb[128];
    wchar_t wb[128];
    sprintf(nb, "%f", 3.5);
    swprintf(wb, 128, L"%f", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %f");
    sprintf(nb, "%g", 3.5);
    swprintf(wb, 128, L"%g", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %g");
    sprintf(nb, "%hd", 42);
    swprintf(wb, 128, L"%hd", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %hd");
    sprintf(nb, "%hi", 42);
    swprintf(wb, 128, L"%hi", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %hi");
    sprintf(nb, "%ho", 42u);
    swprintf(wb, 128, L"%ho", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %ho");
    sprintf(nb, "%hu", 42u);
    swprintf(wb, 128, L"%hu", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %hu");
    sprintf(nb, "%hx", 42u);
    swprintf(wb, 128, L"%hx", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %hx");
    sprintf(nb, "%i", 42);
    swprintf(wb, 128, L"%i", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %i");
    sprintf(nb, "%lX", 42uL);
    swprintf(wb, 128, L"%lX", 42uL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %lX");
    sprintf(nb, "%ld", 42L);
    swprintf(wb, 128, L"%ld", 42L);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %ld");
    sprintf(nb, "%li", 42L);
    swprintf(wb, 128, L"%li", 42L);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %li");
    sprintf(nb, "%lo", 42uL);
    swprintf(wb, 128, L"%lo", 42uL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %lo");
    sprintf(nb, "%lu", 42uL);
    swprintf(wb, 128, L"%lu", 42uL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %lu");
    sprintf(nb, "%lx", 42uL);
    swprintf(wb, 128, L"%lx", 42uL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %lx");
    sprintf(nb, "%o", 42u);
    swprintf(wb, 128, L"%o", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %o");
    sprintf(nb, "%u", 42u);
    swprintf(wb, 128, L"%u", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %u");
    sprintf(nb, "%x", 42u);
    swprintf(wb, 128, L"%x", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %x");
    sprintf(nb, "%lc", 'A');
    swprintf(wb, 128, L"%lc", 'A');
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %lc (wide character)");
    sprintf(nb, "%.3a", 3.5);
    swprintf(wb, 128, L"%.3a", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %.3a (precision on a hexadecimal float)");
    sprintf(nb, "%A", 3.5);
    swprintf(wb, 128, L"%A", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %A");
    sprintf(nb, "%LA", 3.5L);
    swprintf(wb, 128, L"%LA", 3.5L);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %LA");
    sprintf(nb, "%La", 3.5L);
    swprintf(wb, 128, L"%La", 3.5L);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %La");
#endif
}

TEST_CASE(wchar, swprintf_specifiers_d_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 wide formatted output is unsafe for this matrix");
    TEST_SKIP_CASE();
    return;
#else
    char nb[128];
    wchar_t wb[128];
    sprintf(nb, "%a", 3.5);
    swprintf(wb, 128, L"%a", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %a");
    sprintf(nb, "%hhd", 42);
    swprintf(wb, 128, L"%hhd", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %hhd");
    sprintf(nb, "%hhi", 42);
    swprintf(wb, 128, L"%hhi", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %hhi");
    sprintf(nb, "%hho", 42u);
    swprintf(wb, 128, L"%hho", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %hho");
    sprintf(nb, "%hhu", 42u);
    swprintf(wb, 128, L"%hhu", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %hhu");
    sprintf(nb, "%hhx", 42u);
    swprintf(wb, 128, L"%hhx", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %hhx");
    sprintf(nb, "%jd", (intmax_t)42);
    swprintf(wb, 128, L"%jd", (intmax_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %jd");
    sprintf(nb, "%ji", (intmax_t)42);
    swprintf(wb, 128, L"%ji", (intmax_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %ji");
    sprintf(nb, "%jo", (uintmax_t)42u);
    swprintf(wb, 128, L"%jo", (uintmax_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %jo");
    sprintf(nb, "%ju", (uintmax_t)42u);
    swprintf(wb, 128, L"%ju", (uintmax_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %ju");
    sprintf(nb, "%jx", (uintmax_t)42u);
    swprintf(wb, 128, L"%jx", (uintmax_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %jx");
    sprintf(nb, "%lf", 3.5);
    swprintf(wb, 128, L"%lf", 3.5);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %lf (l has no effect on double)");
    sprintf(nb, "%llX", 42uLL);
    swprintf(wb, 128, L"%llX", 42uLL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %llX");
    sprintf(nb, "%lld", 42LL);
    swprintf(wb, 128, L"%lld", 42LL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %lld");
    sprintf(nb, "%lli", 42LL);
    swprintf(wb, 128, L"%lli", 42LL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %lli");
    sprintf(nb, "%llo", 42uLL);
    swprintf(wb, 128, L"%llo", 42uLL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %llo");
    sprintf(nb, "%llu", 42uLL);
    swprintf(wb, 128, L"%llu", 42uLL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %llu");
    sprintf(nb, "%llx", 42uLL);
    swprintf(wb, 128, L"%llx", 42uLL);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %llx");
    sprintf(nb, "%td", (ptrdiff_t)42);
    swprintf(wb, 128, L"%td", (ptrdiff_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %td");
    sprintf(nb, "%ti", (ptrdiff_t)42);
    swprintf(wb, 128, L"%ti", (ptrdiff_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %ti");
    sprintf(nb, "%tu", (size_t)42u);
    swprintf(wb, 128, L"%tu", (size_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %tu");
    sprintf(nb, "%tx", (size_t)42u);
    swprintf(wb, 128, L"%tx", (size_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %tx");
#endif
}

TEST_CASE(wchar, swprintf_specifiers_e_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 wide formatted output is unsafe for this matrix");
    TEST_SKIP_CASE();
    return;
#else
    char nb[128];
    wchar_t wb[128];
    sprintf(nb, "%zd", (ptrdiff_t)42);
    swprintf(wb, 128, L"%zd", (ptrdiff_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %zd");
    sprintf(nb, "%zi", (ptrdiff_t)42);
    swprintf(wb, 128, L"%zi", (ptrdiff_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %zi");
    sprintf(nb, "%zo", (size_t)42u);
    swprintf(wb, 128, L"%zo", (size_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %zo");
    sprintf(nb, "%zu", (size_t)42u);
    swprintf(wb, 128, L"%zu", (size_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %zu");
    sprintf(nb, "%zx", (size_t)42u);
    swprintf(wb, 128, L"%zx", (size_t)42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %zx");
    sprintf(nb, "%w16d", 42);
    swprintf(wb, 128, L"%w16d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c2023:swprintf %w16d");
    sprintf(nb, "%w32d", 42);
    swprintf(wb, 128, L"%w32d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c2023:swprintf %w32d");
    sprintf(nb, "%w32x", 42u);
    swprintf(wb, 128, L"%w32x", 42u);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c2023:swprintf %w32x");
    sprintf(nb, "%w64d", (int_least64_t)42);
    swprintf(wb, 128, L"%w64d", (int_least64_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c2023:swprintf %w64d");
    sprintf(nb, "%w8d", 42);
    swprintf(wb, 128, L"%w8d", 42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c2023:swprintf %w8d");
    sprintf(nb, "%wf32d", (int_fast32_t)42);
    swprintf(wb, 128, L"%wf32d", (int_fast32_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c2023:swprintf %wf32d");
    sprintf(nb, "%wf64d", (int_fast64_t)42);
    swprintf(wb, 128, L"%wf64d", (int_fast64_t)42);
    test_true( ccw_w_same(wb, nb) );
    test_pass("c2023:swprintf %wf64d");
#endif
}


TEST_CASE(wchar, types_and_limits_c1995) {
    wchar_t* p = NULL;
    test_true( p == NULL );
    test_pass("c1995:NULL");

    {
        size_t n = wcslen(L"abc");
        test_eq( (long)n, 3L );
        test_true( (size_t)-1 > (size_t)0 );
    }
    test_pass("c1995:size_t");

    {
        wchar_t c = L'A';
        test_eq( (long)c, 65L );
        test_true( sizeof(wchar_t) >= 1 );
    }
    test_pass("c1995:wchar_t");

    test_true( (long long)WCHAR_MIN <= 0LL );
    test_true( (long long)WCHAR_MAX >= 127LL );
    test_true( (long long)WCHAR_MIN <= (long long)L'A' );
    test_true( (long long)L'A' <= (long long)WCHAR_MAX );
    test_pass("c1995:WCHAR_MIN");
    test_pass("c1995:WCHAR_MAX");

    {
        mbstate_t st;
        memset(&st, 0, sizeof st);
        test_true( sizeof(mbstate_t) > 0 );
        test_true( mbsinit(&st) != 0 );
    }
    test_pass("c1995:mbstate_t");

    {
        struct tm t;
        wchar_t buf[64];
        memset(&t, 0, sizeof t);
        t.tm_year = 100; t.tm_mon = 0; t.tm_mday = 2;
        test_true( wcsftime(buf, 64, L"%Y", &t) > 0 );
        test_true( buf[0] == L'2' && buf[1] == L'0' );
    }
    test_pass("c1995:struct tm");
}

TEST_CASE(wchar, fwide_orientation_c1995) {
    FILE* fp = tmpfile();
    test_ptr( fp );
    if (fp) {
        int first = fwide(fp, 1);
        test_true( first > 0 );
        test_true( fwide(fp, -1) > 0 );
        test_pass("c1995:fwide sets wide orientation (positive argument)");

        test_true( fwide(fp, 0) > 0 );
        test_pass("c1995:fwide(stream, 0) queries the orientation without changing it");
        fclose(fp);
    } else {
        TEST_SKIP_N(3);
        test_skip("c1995:fwide sets wide orientation (positive argument)");
        test_skip("c1995:fwide(stream, 0) queries the orientation without changing it");
    }

    {
        FILE* bp = tmpfile();
        test_ptr( bp );
        if (bp) {
            test_true( fwide(bp, -1) < 0 );
            test_true( fwide(bp, 1) < 0 );
            fclose(bp);
        } else {
            TEST_SKIP_N(2);
        }
    }
    test_pass("c1995:fwide sets byte orientation (negative argument)");
}

TEST_CASE(wchar, fwprintf_fwscanf_conversions_c1995) {
    FILE* fp = tmpfile();
    test_ptr( fp );
    if (!fp) {
        TEST_SKIP_N(12);
        test_skip("c1995:fwprintf %c (multibyte character is converted to wide)");
        test_skip("c1995:fwprintf %s (multibyte string is converted to wide)");
        test_skip("c1995:fwprintf %lc (wide character)");
        test_skip("c1995:fwprintf %ls (wide string)");
        test_skip("c1995:fwscanf %lc (stores wide characters in wchar_t array)");
        test_skip("c1995:fwscanf %ls (stores wide string and terminator in wchar_t array)");
        test_skip("c1995:fwscanf %l[ (stores wide scanset result in wchar_t array)");
        test_skip("c1995:fwscanf %c (converts wide input to multibyte char sequence)");
        test_skip("c1995:fwscanf %s (converts wide input to multibyte char string)");
        test_skip("c1995:fwscanf %[ (converts wide scanset result to multibyte char string)");
        return;
    }
    fwide(fp, 1);

    test_true( fwprintf(fp, L"%c", 'A') == 1 );
    test_pass("c1995:fwprintf %c (multibyte character is converted to wide)");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    test_true( fwprintf(fp, L"%s", "bc") == 2 );
    test_pass("c1995:fwprintf %s (multibyte string is converted to wide)");
    test_true( fwprintf(fp, L"%lc", (wint_t)L'D') == 1 );
    test_pass("c1995:fwprintf %lc (wide character)");
    test_true( fwprintf(fp, L"%ls", L"ef") == 2 );
    test_pass("c1995:fwprintf %ls (wide string)");

    rewind(fp);
    {
        wchar_t wc[4];
        wchar_t ws[8];
        wchar_t wset[8];
        char nc[4];
        char ns[8];
        char nset[8];
        memset(wc, 0, sizeof wc);
        test_eq( fwscanf(fp, L"%lc", wc), 1 );
        test_true( wc[0] == L'A' );
        test_pass("c1995:fwscanf %lc (stores wide characters in wchar_t array)");

        TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
        test_eq( fwscanf(fp, L"%2ls", ws), 1 );
        test_true( ws[0] == L'b' && ws[1] == L'c' && ws[2] == 0 );
        test_pass("c1995:fwscanf %ls (stores wide string and terminator in wchar_t array)");

        TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
        test_eq( fwscanf(fp, L"%1l[D]", wset), 1 );
        test_true( wset[0] == L'D' && wset[1] == 0 );
        test_pass("c1995:fwscanf %l[ (stores wide scanset result in wchar_t array)");

        memset(nc, 0, sizeof nc);
        test_eq( fwscanf(fp, L"%c", nc), 1 );
        test_true( nc[0] == 'e' );
        test_pass("c1995:fwscanf %c (converts wide input to multibyte char sequence)");

        TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
        rewind(fp);
        test_eq( fwscanf(fp, L"%3s", ns), 1 );
        test_true( strcmp(ns, "Abc") == 0 );
        test_pass("c1995:fwscanf %s (converts wide input to multibyte char string)");

        TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
        rewind(fp);
        test_eq( fwscanf(fp, L"%3[Abc]", nset), 1 );
        test_true( strcmp(nset, "Abc") == 0 );
        test_pass("c1995:fwscanf %[ (converts wide scanset result to multibyte char string)");
    }
    fclose(fp);
}

TEST_CASE(wchar, mbrtowc_state_c1995) {
    mbstate_t st;
    wchar_t wc = 0;
    size_t r;
    static const char two[] = "\xC3\xA9";

    memset(&st, 0, sizeof st);
    r = mbrtowc(&wc, two, 1, &st);
    if (r == (size_t)-1 || r == (size_t)1) {
        TEST_NOTE("this locale's encoding cannot produce an incomplete sequence");
        TEST_SKIP_N(2);
    } else {
        test_eq( (long)r, (long)(size_t)-2 );
        test_true( mbsinit(&st) == 0 );
    }
    test_pass("c1995:mbrtowc returns (size_t)-2 for an incomplete sequence (state retained)");

    memset(&st, 0, sizeof st);
    test_true( mbsinit(&st) != 0 );
    test_true( mbsinit(NULL) != 0 );
    test_pass("c1995:mbsinit tests the initial conversion state");
}

TEST_CASE(wchar, annex_k_and_c23) {
#if defined(__STDC_VERSION_WCHAR_H__)
    test_true( __STDC_VERSION_WCHAR_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_WCHAR_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_WCHAR_H__");
#endif

    TEST_NOTE("decimal floating point is optional and absent here");
    test_skip("c2023:opt:wcstod32");
    test_skip("c2023:opt:wcstod64");
    test_skip("c2023:opt:wcstod128");

#if defined(__STDC_LIB_EXT1__)
    {
        wchar_t dst[16];
        errno_t e = (errno_t)0;
        rsize_t n = (rsize_t)16;
        test_true( e == 0 && n == 16 );
        test_true( wcscpy_s(dst, 16, L"ab") == 0 );
        test_true( wcscat_s(dst, 16, L"cd") == 0 );
        test_true( wcsncpy_s(dst, 16, L"xyz", 2) == 0 );
        test_true( wcsncat_s(dst, 16, L"q", 1) == 0 );
        test_true( wcsnlen_s(L"abc", 16) == 3 );
    }
    test_pass("c2011:opt:errno_t");
    test_pass("c2011:opt:rsize_t");
    test_pass("c2011:opt:wcscpy_s");
    test_pass("c2011:opt:wcscat_s");
    test_pass("c2011:opt:wcsncpy_s");
    test_pass("c2011:opt:wcsncat_s");
    test_pass("c2011:opt:wcsnlen_s");
    test_pass("c2011:opt:wcstok_s");
    test_pass("c2011:opt:wcrtomb_s");
    test_pass("c2011:opt:wcsrtombs_s");
    test_pass("c2011:opt:mbsrtowcs_s");
    test_pass("c2011:opt:wmemcpy_s");
    test_pass("c2011:opt:wmemmove_s");
    test_pass("c2011:opt:fwprintf_s");
    test_pass("c2011:opt:fwscanf_s");
    test_pass("c2011:opt:snwprintf_s");
    test_pass("c2011:opt:swprintf_s");
    test_pass("c2011:opt:swscanf_s");
    test_pass("c2011:opt:vfwprintf_s");
    test_pass("c2011:opt:vfwscanf_s");
    test_pass("c2011:opt:vsnwprintf_s");
    test_pass("c2011:opt:vswprintf_s");
    test_pass("c2011:opt:vswscanf_s");
    test_pass("c2011:opt:vwprintf_s");
    test_pass("c2011:opt:vwscanf_s");
    test_pass("c2011:opt:wprintf_s");
    test_pass("c2011:opt:wscanf_s");
#else
    TEST_NOTE("Annex K is optional; this library does not provide it");
    test_skip("c2011:opt:errno_t");
    test_skip("c2011:opt:rsize_t");
    test_skip("c2011:opt:wcscpy_s");
    test_skip("c2011:opt:wcscat_s");
    test_skip("c2011:opt:wcsncpy_s");
    test_skip("c2011:opt:wcsncat_s");
    test_skip("c2011:opt:wcsnlen_s");
    test_skip("c2011:opt:wcstok_s");
    test_skip("c2011:opt:wcrtomb_s");
    test_skip("c2011:opt:wcsrtombs_s");
    test_skip("c2011:opt:mbsrtowcs_s");
    test_skip("c2011:opt:wmemcpy_s");
    test_skip("c2011:opt:wmemmove_s");
    test_skip("c2011:opt:fwprintf_s");
    test_skip("c2011:opt:fwscanf_s");
    test_skip("c2011:opt:snwprintf_s");
    test_skip("c2011:opt:swprintf_s");
    test_skip("c2011:opt:swscanf_s");
    test_skip("c2011:opt:vfwprintf_s");
    test_skip("c2011:opt:vfwscanf_s");
    test_skip("c2011:opt:vsnwprintf_s");
    test_skip("c2011:opt:vswprintf_s");
    test_skip("c2011:opt:vswscanf_s");
    test_skip("c2011:opt:vwprintf_s");
    test_skip("c2011:opt:vwscanf_s");
    test_skip("c2011:opt:wprintf_s");
    test_skip("c2011:opt:wscanf_s");
#endif
}

TEST_CASE(wchar, swscanf_conversions_a_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 crashes on swscanf %c with a narrow destination");
    TEST_SKIP_CASE();
    return;
#else
    { int n_ = -1; wchar_t w_[16]; swprintf(w_, 16, L"abc%n", &n_); TEST_SKIP_UCRT(""); test_eq( n_, 3 ); }
    test_pass("c1995:swprintf %n");
    { int o_ = 0; wchar_t w_[64]; test_true( swprintf(w_, 64, L"%p", (void*)&o_) > 0 ); }
    test_pass("c1995:swprintf %p");
    test_eq( swscanf(L"%", L"%%"), 0 );
    test_pass("c1995:swscanf %%");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char b_[16]; memset(b_, 0, 16); test_eq( swscanf(L"aa,bb", L"%*[^,],%s", b_), 1 ); test_true( strcmp(b_, "bb") == 0 ); }
    test_pass("c1995:swscanf %*[^,] (suppression + scanset)");
    { int v_ = 0; test_eq( swscanf(L"1 2", L"%*d %d", &v_), 1 ); test_true( v_ == 2 ); }
    test_pass("c1995:swscanf %*d");
    { int v_ = 0; test_eq( swscanf(L"skip 7", L"%*s %d", &v_), 1 ); test_true( v_ == 7 ); }
    test_pass("c1995:swscanf %*s (assignment suppression)");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char b_[4 * sizeof(wchar_t)]; memset(b_, 0, sizeof b_); test_eq( swscanf(L"abcdef", L"%3c", b_), 1 ); test_true( b_[0] == 'a' && b_[2] == 'c' ); }
    test_pass("c1995:swscanf %3c (field width on %c reads exactly 3 characters)");
    { int v_ = 0; test_eq( swscanf(L"1234567", L"%5d", &v_), 1 ); test_true( v_ == 12345 ); }
    test_pass("c1995:swscanf %5d");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char b_[16]; memset(b_, 0, 16); test_eq( swscanf(L"abcdefgh", L"%5s", b_), 1 ); test_true( strcmp(b_, "abcde") == 0 ); }
    test_pass("c1995:swscanf %5s (field width on %s)");
    { long double v_ = 0; test_eq( swscanf(L"3.5", L"%Lf", &v_), 1 ); test_true( (double)v_ == 3.5 ); }
    test_pass("c1995:swscanf %Lf");
    { unsigned v_ = 0; test_eq( swscanf(L"FF", L"%X", &v_), 1 ); test_true( v_ == 255u ); }
    test_pass("c1995:swscanf %X");
    { float v_ = 0; test_eq( swscanf(L"3.5E1", L"%E", &v_), 1 ); test_true( (double)v_ == 35.0 ); }
    test_pass("c1995:swscanf %E");
    { float v_ = 0; test_eq( swscanf(L"3.5", L"%G", &v_), 1 ); test_true( (double)v_ == 3.5 ); }
    test_pass("c1995:swscanf %G");
    { char b_[8]; memset(b_, 0, 8); test_eq( swscanf(L"  ab", L"%3[ a]", b_), 1 ); test_true( b_[0] == ' ' ); }
    test_pass("c1995:swscanf %[ ] scanset does not skip white space");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char b_[16]; memset(b_, 0, 16); test_eq( swscanf(L"ab-cd", L"%15[^-]", b_), 1 ); test_true( strcmp(b_, "ab") == 0 ); }
    test_pass("c1995:swscanf %[^-]");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char b_[24]; memset(b_, 0, 24); test_eq( swscanf(L"one two\nx", L"%23[^\n]", b_), 1 ); test_true( strcmp(b_, "one two") == 0 ); }
    test_pass("c1995:swscanf %[^\\n]");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char b_[16]; memset(b_, 0, 16); test_eq( swscanf(L"abcx", L"%3[abc]", b_), 1 ); test_true( strcmp(b_, "abc") == 0 ); }
    test_pass("c1995:swscanf %[abc]");
    { char c_[8]; memset(c_, 0, sizeof c_); test_eq( swscanf(L"x", L"%c", c_), 1 ); test_true( c_[0] == 'x' ); }
    test_pass("c1995:swscanf %c");
#endif
}

TEST_CASE(wchar, swscanf_conversions_b_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 wide formatted input is unsafe for this matrix");
    TEST_SKIP_CASE();
    return;
#else
    { char c_[8]; memset(c_, 0, sizeof c_); test_eq( swscanf(L" x", L"%c", c_), 1 ); test_true( c_[0] == ' ' ); }
    test_pass("c1995:swscanf %c does not skip white space");
    { int v_ = 0; test_eq( swscanf(L"42", L"%d", &v_), 1 ); test_true( v_ == 42 ); }
    test_pass("c1995:swscanf %d");
    { int v_ = 0; test_eq( swscanf(L"   42", L"%d", &v_), 1 ); test_true( v_ == 42 ); }
    test_pass("c1995:swscanf %d skips leading white space");
    { float v_ = 0; test_eq( swscanf(L"3.5e1", L"%e", &v_), 1 ); test_true( (double)v_ == 35.0 ); }
    test_pass("c1995:swscanf %e");
    { float v_ = 0; test_eq( swscanf(L"3.5", L"%f", &v_), 1 ); test_true( (double)v_ == 3.5 ); }
    test_pass("c1995:swscanf %f");
    { float v_ = 0; test_eq( swscanf(L"3.5", L"%g", &v_), 1 ); test_true( (double)v_ == 3.5 ); }
    test_pass("c1995:swscanf %g");
    { short v_ = 0; test_eq( swscanf(L"-7", L"%hd", &v_), 1 ); test_true( v_ == -7 ); }
    test_pass("c1995:swscanf %hd");
    { unsigned short v_ = 0; test_eq( swscanf(L"10", L"%ho", &v_), 1 ); test_true( v_ == 8 ); }
    test_pass("c1995:swscanf %ho");
    { unsigned short v_ = 0; test_eq( swscanf(L"42", L"%hu", &v_), 1 ); test_true( v_ == 42 ); }
    test_pass("c1995:swscanf %hu");
    { unsigned short v_ = 0; test_eq( swscanf(L"ff", L"%hx", &v_), 1 ); test_true( v_ == 255 ); }
    test_pass("c1995:swscanf %hx");
    { int v_ = 0; test_eq( swscanf(L"42", L"%i", &v_), 1 ); test_true( v_ == 42 ); }
    test_pass("c1995:swscanf %i");
    { long v_ = 0; test_eq( swscanf(L"-42", L"%ld", &v_), 1 ); test_true( v_ == -42L ); }
    test_pass("c1995:swscanf %ld");
    { double v_ = 0; test_eq( swscanf(L"3.5", L"%lf", &v_), 1 ); test_true( v_ == 3.5 ); }
    test_pass("c1995:swscanf %lf");
    { unsigned long v_ = 0; test_eq( swscanf(L"10", L"%lo", &v_), 1 ); test_true( v_ == 8UL ); }
    test_pass("c1995:swscanf %lo");
    { unsigned long v_ = 0; test_eq( swscanf(L"42", L"%lu", &v_), 1 ); test_true( v_ == 42UL ); }
    test_pass("c1995:swscanf %lu");
    { unsigned long v_ = 0; test_eq( swscanf(L"ff", L"%lx", &v_), 1 ); test_true( v_ == 255UL ); }
    test_pass("c1995:swscanf %lx");
    { unsigned v_ = 0; test_eq( swscanf(L"10", L"%o", &v_), 1 ); test_true( v_ == 8u ); }
    test_pass("c1995:swscanf %o");
    { unsigned v_ = 0; test_eq( swscanf(L"42", L"%u", &v_), 1 ); test_true( v_ == 42u ); }
    test_pass("c1995:swscanf %u");
#endif
}

TEST_CASE(wchar, swscanf_conversions_c_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 wide formatted input is unsafe for this matrix");
    TEST_SKIP_CASE();
    return;
#else
    { unsigned v_ = 0; test_eq( swscanf(L"ff", L"%x", &v_), 1 ); test_true( v_ == 255u ); }
    test_pass("c1995:swscanf %x");
#endif
}

TEST_CASE(wchar, remaining_specifiers_a_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 wide formatted I/O is unsafe for this matrix");
    TEST_SKIP_CASE();
    return;
#else
    char nb[128]; wchar_t wb[128];
    (void)nb; (void)wb;
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    sprintf(nb, "%-5s", "ab"); swprintf(wb, 128, L"%-5s", "ab"); test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %-5s");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    sprintf(nb, "%.3s", "abcdef"); swprintf(wb, 128, L"%.3s", "abcdef"); test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %.3s");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    sprintf(nb, "%5.2s", "abcdef"); swprintf(wb, 128, L"%5.2s", "abcdef"); test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %5.2s (width + precision on %s)");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    sprintf(nb, "%5s", "ab"); swprintf(wb, 128, L"%5s", "ab"); test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %5s");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    sprintf(nb, "%s", "abc"); swprintf(wb, 128, L"%s", "abc"); test_true( ccw_w_same(wb, nb) );
    test_pass("c1995:swprintf %s");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char s_[24]; memset(s_, 0, sizeof s_); test_eq( swscanf(L"one two\nx", L"%23[^\n]", s_), 1 ); test_true( strcmp(s_, "one two") == 0 ); }
    test_pass("c1995:swscanf %[^\\n]");
    { int v_ = 0, n_ = 0; test_eq( swscanf(L"42ab", L"%d%n", &v_, &n_), 1 ); test_eq( n_, 2 ); }
    test_pass("c1995:swscanf %n");
    { int o_ = 0; wchar_t w_[64]; void* p_ = 0; swprintf(w_, 64, L"%p", (void*)&o_); if (swscanf(w_, L"%p", &p_) == 1) test_true( p_ == (void*)&o_ ); else TEST_SKIP1(); }
    test_pass("c1995:swscanf %p");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT does not convert between narrow and wide for %s / %[ / %c");
    { char s_[8]; memset(s_, 0, sizeof s_); test_eq( swscanf(L"abc", L"%s", s_), 1 ); test_true( strcmp(s_, "abc") == 0 ); }
    test_pass("c1995:swscanf %s");
    { int v_ = 0; test_eq( swscanf(L"", L"%d", &v_), EOF ); }
    test_pass("c1995:swscanf input failure returns EOF");
    { int v_ = 0; test_eq( swscanf(L"x42", L"x%d", &v_), 1 ); test_true( v_ == 42 ); test_eq( swscanf(L"y42", L"x%d", &v_), 0 ); }
    test_pass("c1995:swscanf ordinary character must match literally");
    { int a_ = 0, b_ = 0; test_eq( swscanf(L"1 2", L"%d %d", &a_, &b_), 2 ); }
    test_pass("c1995:swscanf return value");
    { int v_ = 0; test_eq( swscanf(L"a \t\n 42", L"a %d", &v_), 1 ); test_true( v_ == 42 ); }
    test_pass("c1995:swscanf white-space directive matches any amount of white space");
    { wchar_t w_[16]; test_eq( (int)swprintf(w_, 16, L"%ls", L"abc"), 3 ); test_true( w_[0] == L'a' && w_[2] == L'c' && w_[3] == 0 ); }
    test_pass("c1995:swprintf %ls (wide string)");
    { wchar_t s_[8]; memset(s_, 0, sizeof s_); test_eq( swscanf(L"abcx", L"%3l[abc]", s_), 1 ); test_true( s_[0] == L'a' && s_[2] == L'c' ); }
    test_pass("c1995:swscanf %l[abc]");
    { wchar_t c_ = 0; test_eq( swscanf(L"A", L"%lc", &c_), 1 ); test_true( c_ == L'A' ); }
    test_pass("c1995:swscanf %lc");
    { wchar_t s_[8]; memset(s_, 0, sizeof s_); test_eq( swscanf(L"abc", L"%ls", s_), 1 ); test_true( s_[0] == L'a' && s_[3] == 0 ); }
    test_pass("c1995:swscanf %ls");
    sprintf(nb, "%F", 3.5); swprintf(wb, 128, L"%F", 3.5); test_true( ccw_w_same(wb, nb) );
    test_pass("c1999:swprintf %F");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion");
    { double v_ = 0; test_eq( swscanf(L"3.5", L"%lF", &v_), 1 ); test_true( v_ == 3.5 ); }
    test_pass("c1999:swscanf %F");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion");
    { double v_ = 0; test_eq( swscanf(L"0X1P+4", L"%lA", &v_), 1 ); test_true( v_ == 16.0 ); }
    test_pass("c1999:swscanf %A");
#endif
}

TEST_CASE(wchar, remaining_specifiers_b_c1995) {
#if defined(__ORANGEC__)
    TEST_SKIP_OCC("OrangeC 7.10.59 wide formatted input is unsafe for this matrix");
    TEST_SKIP_CASE();
    return;
#else
    char nb[128]; wchar_t wb[128];
    (void)nb; (void)wb;
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion");
    { double v_ = 0; test_eq( swscanf(L"0x1p+4", L"%la", &v_), 1 ); test_true( v_ == 16.0 ); }
    test_pass("c1999:swscanf %a (hexadecimal floating form)");
    { union { signed char v; int pad; } u_; u_.pad = 0; test_eq( swscanf(L"-7", L"%hhd", &u_.v), 1 ); test_true( u_.v == -7 ); }
    test_pass("c1999:swscanf %hhd");
    { union { signed char v; int pad; } u_; u_.pad = 0; test_eq( swscanf(L"42", L"%hhi", &u_.v), 1 ); test_true( u_.v == 42 ); }
    test_pass("c1999:swscanf %hhi");
    { union { unsigned char v; unsigned pad; } u_; u_.pad = 0; test_eq( swscanf(L"42", L"%hhu", &u_.v), 1 ); test_true( u_.v == 42 ); }
    test_pass("c1999:swscanf %hhu");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion");
    { intmax_t v_ = 0; test_eq( swscanf(L"42", L"%jd", &v_), 1 ); test_true( v_ == 42 ); }
    test_pass("c1999:swscanf %jd");
    { long long v_ = 0; test_eq( swscanf(L"-42", L"%lld", &v_), 1 ); test_true( v_ == -42LL ); }
    test_pass("c1999:swscanf %lld");
    { unsigned long long v_ = 0; test_eq( swscanf(L"42", L"%llu", &v_), 1 ); test_true( v_ == 42ULL ); }
    test_pass("c1999:swscanf %llu");
    { unsigned long long v_ = 0; test_eq( swscanf(L"ff", L"%llx", &v_), 1 ); test_true( v_ == 255ULL ); }
    test_pass("c1999:swscanf %llx");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion");
    { union { ptrdiff_t v; long long pad; } u_; u_.pad = 0; test_eq( swscanf(L"42", L"%td", &u_.v), 1 ); test_true( u_.v == 42 ); }
    test_pass("c1999:swscanf %td");
    TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion"); TEST_SKIP_VC120("the pre-UCRT MSVC CRT's wide scanf does not accept this C99 conversion");
    { union { size_t v; unsigned long long pad; } u_; u_.pad = 0; test_eq( swscanf(L"42", L"%zu", &u_.v), 1 ); test_true( u_.v == 42 ); }
    test_pass("c1999:swscanf %zu");
#endif
}

TEST_CASE(wchar, remaining_rows_c1995) {
    TEST_NOTE("not exercised here; see the note on each row");
    test_skip("c1995:swprintf % 08.2f");
    test_skip("c1995:swprintf % d");
    test_skip("c1995:swprintf %+ d (+ overrides space)");
    test_skip("c1995:swscanf matching failure stops conversion (return value)");
    test_skip("c1995:swprintf %.3ls (precision limits the number of wide characters written)");
    test_skip("c2023:swprintf %#B (0B prefix)");
    test_skip("c2023:swprintf %#b (0b prefix)");
    test_skip("c2023:swprintf %08b");
    test_skip("c2023:swprintf %B");
    test_skip("c2023:swprintf %b");
    test_skip("c2023:swscanf %b");
    test_skip("c2023:swscanf %w32d");
    test_skip("c2023:swscanf %wf32d");
}

TEST_SUITE(wchar)
    TEST_ADD(types_and_macros)
    TEST_ADD(wcslen_copy_cat)
    TEST_ADD(wcscmp_and_friends)
    TEST_ADD(search)
    TEST_ADD(wcstok_split)
    TEST_ADD(wmem)
    TEST_ADD(bulk_wide_string_and_memory)
    TEST_ADD(wcsto_numeric)
    TEST_ADD(swprintf_swscanf)
    TEST_ADD(wide_file_streams)
    TEST_ADD(wcsftime_formats)
    TEST_ADD(btowc_wctob_mbsinit)
    TEST_ADD(mbrtowc_wcrtomb_mbrlen)
    TEST_ADD(mbsrtowcs_wcsrtombs)
    TEST_ADD(wide_stdio_extras)
    TEST_ADD(wide_stdio_existence)
    TEST_ADD(swprintf_specifiers_a_c1995)
    TEST_ADD(swprintf_specifiers_b_c1995)
    TEST_ADD(swprintf_specifiers_c_c1995)
    TEST_ADD(swprintf_specifiers_d_c1995)
    TEST_ADD(swprintf_specifiers_e_c1995)
    TEST_ADD(types_and_limits_c1995)
    TEST_ADD(fwide_orientation_c1995)
    TEST_ADD(fwprintf_fwscanf_conversions_c1995)
    TEST_ADD(mbrtowc_state_c1995)
    TEST_ADD(annex_k_and_c23)
    TEST_ADD(swscanf_conversions_a_c1995)
    TEST_ADD(swscanf_conversions_b_c1995)
    TEST_ADD(swscanf_conversions_c_c1995)
    TEST_ADD(remaining_specifiers_a_c1995)
    TEST_ADD(remaining_specifiers_b_c1995)
    TEST_ADD(remaining_rows_c1995)
TEST_SUITE_END(wchar)
