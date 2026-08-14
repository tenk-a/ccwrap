#include "c_test.h"

#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#define TST_STRING_BULK_SIZE 4096u
static char tst_string_bulk_a[TST_STRING_BULK_SIZE + 2u];
static char tst_string_bulk_b[TST_STRING_BULK_SIZE + 2u];

static unsigned char tst_bulk_value(size_t i) {
    return (unsigned char)(1u + (unsigned)(i % 251u));
}

TEST_CASE(string, size_t_null_defined) {
    size_t n = sizeof("abc");
    test_eq( n, (size_t)4 );
    test_pass("c1990:size_t");
    test_true( (void*)0 == NULL );
    test_pass("c1990:NULL");
}

TEST_CASE(string, memchr_fn) {
    char a[8];
    int i;
    for (i = 0; i < 8; ++i) a[i] = (char)('0' + i);
    test_true( memchr(a, '3', 8) == a + 3 );
    test_true( memchr(a, 'x', 8) == NULL );
    test_true( memchr(a, '3', 3) == NULL );
    test_true( memchr(a, '0', 0) == NULL );
    test_true( memchr(a, '7', 7) == NULL );
    test_true( memchr(a, '7', 8) == a + 7 );
    test_pass("c1990:memchr");
}

TEST_CASE(string, bulk_memory_boundaries) {
    size_t i;
    size_t n = (size_t)TST_STRING_BULK_SIZE;
    int ok;

    tst_string_bulk_a[0] = (char)0x31;
    tst_string_bulk_b[0] = (char)0x32;
    tst_string_bulk_b[n + 1u] = (char)0x33;
    for (i = 0; i < n; ++i)
        tst_string_bulk_a[i + 1u] = (char)tst_bulk_value(i);

    test_true( memcpy(tst_string_bulk_b + 1, tst_string_bulk_a + 1, n)
               == (void*)(tst_string_bulk_b + 1) );
    test_eq( memcmp(tst_string_bulk_b + 1, tst_string_bulk_a + 1, n), 0 );
    test_eq( (unsigned char)tst_string_bulk_b[0], (unsigned char)0x32 );
    test_eq( (unsigned char)tst_string_bulk_b[n + 1u], (unsigned char)0x33 );
    test_true( memcpy(tst_string_bulk_b + 1, tst_string_bulk_a + 1, 0)
               == (void*)(tst_string_bulk_b + 1) );
    test_pass("c1990:memcpy bulk 0/4096 with guards");

    test_true( memchr(tst_string_bulk_a + 1, tst_bulk_value(0), 1)
               == (void*)(tst_string_bulk_a + 1) );
    tst_string_bulk_a[n] = (char)UCHAR_MAX;
    test_true( memchr(tst_string_bulk_a + 1, UCHAR_MAX, n - 1u) == NULL );
    test_true( memchr(tst_string_bulk_a + 1, UCHAR_MAX, n)
               == (void*)(tst_string_bulk_a + n) );
    test_pass("c1990:memchr bulk first/last/count boundary");

    memset(tst_string_bulk_b, 0x22, n + 2u);
    test_true( memset(tst_string_bulk_b + 1, UCHAR_MAX, n)
               == (void*)(tst_string_bulk_b + 1) );
    ok = ((unsigned char)tst_string_bulk_b[0] == (unsigned char)0x22)
      && ((unsigned char)tst_string_bulk_b[n + 1u] == (unsigned char)0x22);
    for (i = 1; i <= n; ++i)
        ok = ok && ((unsigned char)tst_string_bulk_b[i] == (unsigned char)UCHAR_MAX);
    test_true( ok );
    test_pass("c1990:memset bulk UCHAR_MAX with guards");

    for (i = 0; i < n; ++i)
        tst_string_bulk_b[i + 1u] = (char)tst_bulk_value(i);
    tst_string_bulk_b[0] = (char)0x41;
    test_true( memmove(tst_string_bulk_b + 2, tst_string_bulk_b + 1, n)
               == (void*)(tst_string_bulk_b + 2) );
    ok = ((unsigned char)tst_string_bulk_b[0] == (unsigned char)0x41);
    for (i = 0; i < n; ++i)
        ok = ok && ((unsigned char)tst_string_bulk_b[i + 2u] == tst_bulk_value(i));
    test_true( ok );
    test_true( memmove(tst_string_bulk_b + 1, tst_string_bulk_b + 2, n)
               == (void*)(tst_string_bulk_b + 1) );
    test_eq( memcmp(tst_string_bulk_b + 1, tst_string_bulk_a + 1, n - 1u), 0 );
    test_pass("c1990:memmove bulk overlap in both directions");

    memcpy(tst_string_bulk_b + 1, tst_string_bulk_a + 1, n);
    test_eq( memcmp(tst_string_bulk_a + 1, tst_string_bulk_b + 1, 0), 0 );
    test_eq( memcmp(tst_string_bulk_a + 1, tst_string_bulk_b + 1, n), 0 );
    tst_string_bulk_b[n] = (char)0;
    test_eq( memcmp(tst_string_bulk_a + 1, tst_string_bulk_b + 1, n - 1u), 0 );
    test_ne( memcmp(tst_string_bulk_a + 1, tst_string_bulk_b + 1, n), 0 );
    test_pass("c1990:memcmp bulk 0/last/count boundary");
}

TEST_CASE(string, bulk_string_boundaries) {
    size_t i;
    size_t n = (size_t)TST_STRING_BULK_SIZE;
    size_t half = n / 2u;

    for (i = 0; i < n; ++i)
        tst_string_bulk_a[i] = 'a';
    tst_string_bulk_a[n - 3u] = 'x';
    tst_string_bulk_a[n - 2u] = 'y';
    tst_string_bulk_a[n - 1u] = 'z';
    tst_string_bulk_a[n] = '\0';

    test_eq( strlen(tst_string_bulk_a), n );
    test_true( strcpy(tst_string_bulk_b, tst_string_bulk_a) == tst_string_bulk_b );
    test_eq( strcmp(tst_string_bulk_b, tst_string_bulk_a), 0 );
    test_eq( (int)tst_string_bulk_b[n], 0 );
    test_pass("c1990:strlen/strcpy bulk exact-fit terminator");

    memset(tst_string_bulk_b, '#', n + 2u);
    test_true( strncpy(tst_string_bulk_b, tst_string_bulk_a, n) == tst_string_bulk_b );
    test_eq( memcmp(tst_string_bulk_b, tst_string_bulk_a, n), 0 );
    test_eq( (int)tst_string_bulk_b[n], (int)'#' );
    test_true( strncpy(tst_string_bulk_b, tst_string_bulk_a, n + 1u)
               == tst_string_bulk_b );
    test_eq( (int)tst_string_bulk_b[n], 0 );
    test_eq( (int)tst_string_bulk_b[n + 1u], (int)'#' );
    test_pass("c1990:strncpy bulk n/n+1 boundary");

    for (i = 0; i < half; ++i)
        tst_string_bulk_b[i] = 'b';
    tst_string_bulk_b[half] = '\0';
    tst_string_bulk_a[half] = '\0';
    test_true( strcat(tst_string_bulk_b, tst_string_bulk_a) == tst_string_bulk_b );
    tst_string_bulk_a[half] = 'a';
    test_eq( strlen(tst_string_bulk_b), n );
    test_eq( (int)tst_string_bulk_b[half - 1u], (int)'b' );
    test_eq( (int)tst_string_bulk_b[half], (int)'a' );
    test_eq( (int)tst_string_bulk_b[n], 0 );
    test_pass("c1990:strcat bulk exact-fit terminator");

    strcpy(tst_string_bulk_b, tst_string_bulk_a);
    test_eq( strncmp(tst_string_bulk_a, tst_string_bulk_b, n), 0 );
    tst_string_bulk_b[n - 1u] = 'w';
    test_eq( strncmp(tst_string_bulk_a, tst_string_bulk_b, n - 1u), 0 );
    test_gt( strncmp(tst_string_bulk_a, tst_string_bulk_b, n), 0 );
    test_pass("c1990:strncmp bulk last/count boundary");

    test_true( strchr(tst_string_bulk_a, 'a') == tst_string_bulk_a );
    test_true( strrchr(tst_string_bulk_a, 'a') == tst_string_bulk_a + n - 4u );
    test_true( strstr(tst_string_bulk_a, "xyz") == tst_string_bulk_a + n - 3u );
    test_true( strpbrk(tst_string_bulk_a, "zy") == tst_string_bulk_a + n - 2u );
    test_eq( strspn(tst_string_bulk_a, "a"), n - 3u );
    test_eq( strcspn(tst_string_bulk_a, "xyz"), n - 3u );
    test_pass("c1990:string search bulk near-end boundaries");
}

TEST_CASE(string, memccpy_c23) {
#if defined(TEST_TARGET_C) && TEST_TARGET_C >= 2023
    char dst[8];
    void* p;
    void* q;
    memset(dst, 0, sizeof dst);
    p = memccpy(dst, "abcdef", 'c', sizeof dst);
    test_true( p != NULL );
    test_true( memcmp(dst, "abc", 3) == 0 );
    test_eq( (int)((char*)p - dst), 3 );

    memset(dst, 0, sizeof dst);
    dst[2] = 'Q';
    q = memccpy(dst, "xy", 'z', 2);
    test_true( q == NULL );
    test_true( memcmp(dst, "xy", 2) == 0 );
    test_eq( (int)dst[2], (int)'Q' );

    memset(dst, 'Q', sizeof dst);
    q = memccpy(dst, "xy", 'x', 0);
    test_true( q == NULL );
    test_eq( (int)dst[0], (int)'Q' );
    test_pass("c2023:memccpy");
#else
    TEST_SKIP_N(8);
    TEST_NOTE("memccpy is C23 (exercised at TEST_TARGET_C >= 2023)");
    test_skip("c2023:memccpy");
#endif
}

#if (defined(__WATCOMC__) && !defined(_CCW_TARGET_C)) || (defined(__GLIBC__) && !defined(_POSIX_C_SOURCE) && !defined(_DEFAULT_SOURCE))
TEST_CASE(string, strnlen_c23) {
    TEST_SKIP_N(4);
}
#else
TEST_CASE(string, strnlen_c23) {
    test_eq( strnlen("hello", 10), (size_t)5 );
    test_eq( strnlen("hello", 3),  (size_t)3 );
    test_eq( strnlen("", 4),       (size_t)0 );
    test_eq( strnlen("abc", 0),    (size_t)0 );
}
#endif

TEST_CASE(string, memcpy_memmove) {
    char dst[8];
    const char src[] = "abcdefg";
    char buf[]  = "abcdef";
    char buf2[] = "abcdef";
    char z[]    = "xy";
    void* r;
    void* r2;

    r = memcpy(dst, src, sizeof src);
    test_true( r == (void*)dst );
    test_str_eq( dst, "abcdefg" );
    memcpy(z, "AB", 0);
    test_str_eq( z, "xy" );
    memcpy(z, "AB", 1);
    test_eq( (int)z[0], (int)'A' );
    test_eq( (int)z[1], (int)'y' );
    test_pass("c1990:memcpy");

    r2 = memmove(buf, buf + 1, 6);
    test_true( r2 == (void*)buf );
    test_str_eq( buf, "bcdef" );

    memmove(buf2 + 1, buf2, 5);
    test_str_eq( buf2, "aabcde" );
    test_true( memmove(buf2, buf2, sizeof buf2) == (void*)buf2 );
    test_str_eq( buf2, "aabcde" );
    test_pass("c1990:memmove");
}

TEST_CASE(string, memset_memcmp) {
    char b[6];
    void* r;
    unsigned char hi[1];
    unsigned char lo[1];

    r = memset(b, 'x', 5);
    b[5] = '\0';
    test_true( r == (void*)b );
    test_str_eq( b, "xxxxx" );

    memset(b, 0, sizeof b);
    test_eq( (int)b[0], 0 );
    test_eq( (int)b[4], 0 );
    b[0] = b[1] = 'Q';
    memset(b, 0x1ff, 1);
    test_eq( (int)(unsigned char)b[0], 0xff );
    test_eq( (int)b[1], (int)'Q' );
    test_pass("c1990:memset");

    test_eq( memcmp("abc", "abc", 3), 0 );
    test_lt( memcmp("abc", "abd", 3), 0 );
    test_gt( memcmp("abd", "abc", 3), 0 );
    test_eq( memcmp("abX", "abY", 2), 0 );
    test_eq( memcmp("", "", 0), 0 );

    hi[0] = 0x80; lo[0] = 0x01;
    test_gt( memcmp(hi, lo, 1), 0 );
    test_pass("c1990:memcmp");
}

TEST_CASE(string, strcpy_strncpy) {
    char d[8];
    char p[6];
    char t[4];
    char* r;
    char* r2;

    r = strcpy(d, "abc");
    test_true( r == d );
    test_str_eq( d, "abc" );
    test_pass("c1990:strcpy");

    memset(p, 'Z', sizeof p);
    r2 = strncpy(p, "ab", 5);
    test_true( r2 == p );
    test_eq( (int)p[0], 'a' );
    test_eq( (int)p[2], 0 );
    test_eq( (int)p[4], 0 );
    test_eq( (int)p[5], 'Z' );

    t[0] = t[1] = t[2] = t[3] = 'Q';
    strncpy(t, "abcdef", 3);
    test_eq( (int)t[0], 'a' );
    test_eq( (int)t[2], 'c' );
    test_eq( (int)t[3], 'Q' );
    strncpy(t, "z", 0);
    test_eq( (int)t[0], 'a' );
    test_pass("c1990:strncpy");
}

TEST_CASE(string, strcat_strncat) {
    char d[16] = "ab";
    char e[16] = "ab";
    char* r;
    char* r2;

    r = strcat(d, "cd");
    test_true( r == d );
    test_str_eq( d, "abcd" );
    strcat(d, "");
    test_str_eq( d, "abcd" );
    test_pass("c1990:strcat");

    r2 = strncat(e, "cdef", 2);
    test_true( r2 == e );
    test_str_eq( e, "abcd" );
    test_eq( (int)e[4], 0 );
    strncat(e, "xyz", 10);
    test_str_eq( e, "abcdxyz" );
    strncat(e, "ignored", 0);
    test_str_eq( e, "abcdxyz" );
    test_pass("c1990:strncat");
}

TEST_CASE(string, strcmp_strncmp) {
    test_eq( strcmp("abc", "abc"), 0 );
    test_lt( strcmp("abc", "abd"), 0 );
    test_gt( strcmp("abd", "abc"), 0 );
    test_lt( strcmp("ab",  "abc"), 0 );
    test_gt( strcmp("abc", "ab" ), 0 );
    test_eq( strcmp("", ""), 0 );
    test_pass("c1990:strcmp");

    test_eq( strncmp("abc", "abd", 2), 0 );
    test_lt( strncmp("abc", "abd", 3), 0 );
    test_eq( strncmp("abc", "zzz", 0), 0 );
    test_lt( strncmp("ab", "abc", 3), 0 );
    test_pass("c1990:strncmp");
}

TEST_CASE(string, strcoll_strxfrm) {
    size_t need;
    size_t n;
    char dummy;
    char buf[64];
    char b2[64];

    test_eq( strcoll("abc", "abc"), 0 );
    test_lt( strcoll("abc", "abd"), 0 );
    test_gt( strcoll("abd", "abc"), 0 );
    test_pass("c1990:strcoll");

    need = strxfrm(&dummy, "abc", 0);
    test_ge( need, (size_t)3 );

    n = strxfrm(buf, "abc", sizeof buf);
    test_eq( n, need );
    test_lt( n, (size_t)sizeof buf );
    test_eq( (int)buf[n], 0 );

    strxfrm(b2, "abd", sizeof b2);
    test_lt( strcmp(buf, b2), 0 );

    test_eq( strxfrm(b2, "abc", 0), need );
    test_pass("c1990:strxfrm");
}

TEST_CASE(string, strchr_strrchr) {
    char       mut[] = "hello";
    const char cst[] = "hello";
    char* f;
    char* b;
    const char* c;

    f = strchr(mut, 'l');
    test_true( f == mut + 2 );
    *f = 'L';
    test_str_eq( mut, "heLlo" );

    c = strchr(cst, 'h');
    test_true( c == cst );
    test_true( strchr(cst, 'z') == NULL );
    test_true( strchr(cst,  '\0') == cst + 5 );
    test_pass("c1990:strchr");

    b = strrchr(mut, 'l');
    test_true( b == mut + 3 );
    test_true( strrchr(cst, 'z') == NULL );
    test_true( strrchr(cst, '\0') == cst + 5 );
    test_pass("c1990:strrchr");
}

TEST_CASE(string, strspn_strcspn_strpbrk) {
    char       mut[] = "a,b;c";
    const char cst[] = "a,b;c";
    char* p;
    const char* q;

    test_eq( strspn("abcde", "abc"), (size_t)3 );
    test_eq( strspn("xabc",  "abc"), (size_t)0 );
    test_eq( strspn("aaa",   "a"  ), (size_t)3 );
    test_eq( strspn("abc",   ""   ), (size_t)0 );
    test_eq( strspn("",      "abc"), (size_t)0 );
    test_pass("c1990:strspn");

    test_eq( strcspn("abcde", "dc"), (size_t)2 );
    test_eq( strcspn("abc",   "a" ), (size_t)0 );
    test_eq( strcspn("abc",   "z" ), (size_t)3 );
    test_eq( strcspn("abc",   ""  ), (size_t)3 );
    test_eq( strcspn("",      "a" ), (size_t)0 );
    test_pass("c1990:strcspn");

    p = strpbrk(mut, ";,");
    test_true( p == mut + 1 );
    *p = '|';
    test_str_eq( mut, "a|b;c" );
    q = strpbrk(cst, "c");
    test_true( q == cst + 4 );
    test_true( strpbrk(cst, "xyz") == NULL );
    test_true( strpbrk(cst, "") == NULL );
    test_pass("c1990:strpbrk");
}

TEST_CASE(string, strstr_find) {
    char       mut[] = "abcabc";
    const char cst[] = "abcabc";
    char* p;
    const char* q;

    p = strstr(mut, "bc");
    test_true( p == mut + 1 );
    *p = 'B';
    test_str_eq( mut, "aBcabc" );

    q = strstr(cst, "cab");
    test_true( q == cst + 2 );
    test_true( strstr(cst, "")   == cst );
    test_true( strstr(cst, "zz") == NULL );
    test_true( strstr(cst, "abcabcabc") == NULL );
    test_pass("c1990:strstr");
}

TEST_CASE(string, strtok_split) {

    char buf[] = "a,b;;c";
    char b2[]  = "  one|two";
    char b3[]  = "x=1";
    char only[]= ",,,";
    char* t;
    char* u;
    char* v;
    char* w;

    t = strtok(buf, ",;");
    test_str_eq( t, "a" );
    t = strtok(NULL, ",;");
    test_str_eq( t, "b" );
    t = strtok(NULL, ",;");
    test_str_eq( t, "c" );
    t = strtok(NULL, ",;");
    test_true( t == NULL );

    u = strtok(b2, " ");
    test_str_eq( u, "one|two" );
    v = strtok(b3, "=");
    test_str_eq( v, "x" );
    v = strtok(NULL, "=");
    test_str_eq( v, "1" );

    w = strtok(only, ",");
    test_true( w == NULL );
    {
        char empty[] = "";
        test_true( strtok(empty, ",") == NULL );
    }
    test_pass("c1990:strtok");
}

TEST_CASE(string, strlen_strerror) {
    char buf[8] = "ab";
    char m1[128];
    char m2[128];
    char* e0;
    char* e1;
    char* e2;
    int diff;

    test_eq( strlen(""), (size_t)0 );
    test_eq( strlen("abc"), (size_t)3 );
    test_eq( strlen("a\0bc"), (size_t)1 );

    test_eq( strlen(buf), (size_t)2 );
    buf[2] = 'c';
    buf[3] = '\0';
    test_eq( strlen(buf), (size_t)3 );
    test_pass("c1990:strlen");

    e0 = strerror(0);
    test_ptr( e0 );
    test_gt( strlen(e0), (size_t)0 );

    e1 = strerror(EDOM);
    test_ptr( e1 );
    strncpy(m1, e1, sizeof m1 - 1);
    m1[sizeof m1 - 1] = '\0';
    test_gt( strlen(m1), (size_t)0 );

    e2 = strerror(ERANGE);
    test_ptr( e2 );
    strncpy(m2, e2, sizeof m2 - 1);
    m2[sizeof m2 - 1] = '\0';
    test_gt( strlen(m2), (size_t)0 );

    diff = strcmp(m1, m2);
    test_ne( diff, 0 );
    test_pass("c1990:strerror");
}


TEST_CASE(string, annex_k_bounds_checked) {
#if defined(__STDC_LIB_EXT1__)
    {
        char dst[16];
        errno_t e;
        rsize_t r = (rsize_t)sizeof dst;
        constraint_handler_t old_h = set_constraint_handler_s(ignore_handler_s);
        e = (errno_t)0;
        test_true( e == 0 && r == sizeof dst );

        test_true( strcpy_s(dst, sizeof dst, "abc") == 0 );
        test_true( strcmp(dst, "abc") == 0 );
        test_true( strcpy_s(dst, 2, "abc") != 0 );

        test_true( strncpy_s(dst, sizeof dst, "abcdef", 3) == 0 );
        test_true( strcmp(dst, "abc") == 0 );

        test_true( strcat_s(dst, sizeof dst, "de") == 0 );
        test_true( strcmp(dst, "abcde") == 0 );
        test_true( strncat_s(dst, sizeof dst, "fgh", 1) == 0 );
        test_true( strcmp(dst, "abcdef") == 0 );

        test_eq( (long)strnlen_s("abc", 16), 3L );
        test_eq( (long)strnlen_s("abc", 2), 2L );
        test_eq( (long)strnlen_s(NULL, 5), 0L );

        {
            char text[] = "a,b";
            rsize_t max = sizeof text;
            char* ctx = NULL;
            char* tok = strtok_s(text, &max, ",", &ctx);
            test_true( tok != NULL && strcmp(tok, "a") == 0 );
            tok = strtok_s(NULL, &max, ",", &ctx);
            test_true( tok != NULL && strcmp(tok, "b") == 0 );
        }

        {
            char buf[64];
            test_true( strerror_s(buf, sizeof buf, 0) == 0 );
            test_true( strlen(buf) > 0 );
            test_true( strerrorlen_s(0) > 0 );
        }

        {
            char a[8], b[8];
            memset(a, 'x', sizeof a);
            test_true( memcpy_s(b, sizeof b, a, sizeof a) == 0 );
            test_true( memcmp(a, b, sizeof a) == 0 );
            test_true( memcpy_s(b, 2, a, sizeof a) != 0 );
            test_true( memmove_s(b, sizeof b, b, 4) == 0 );
#if !defined(__WATCOMC__)
            test_true( memset_s(b, sizeof b, 'y', 4) == 0 );
            test_eq( (int)b[0], (int)'y' );
#else
            TEST_SKIP_N(2);
#endif
        }
        set_constraint_handler_s(old_h);
    }
    test_pass("c2011:opt:errno_t");
    test_pass("c2011:opt:rsize_t");
    test_pass("c2011:opt:strcpy_s");
    test_pass("c2011:opt:strncpy_s");
    test_pass("c2011:opt:strcat_s");
    test_pass("c2011:opt:strncat_s");
    test_pass("c2011:opt:strnlen_s");
    test_pass("c2011:opt:strtok_s");
    test_pass("c2011:opt:strerror_s");
    test_pass("c2011:opt:strerrorlen_s");
    test_pass("c2011:opt:memcpy_s");
    test_pass("c2011:opt:memmove_s");
    test_pass("c2011:opt:memset_s");
#else
    TEST_NOTE("Annex K is optional; this library does not provide it");
    test_skip("c2011:opt:errno_t");
    test_skip("c2011:opt:rsize_t");
    test_skip("c2011:opt:strcpy_s");
    test_skip("c2011:opt:strncpy_s");
    test_skip("c2011:opt:strcat_s");
    test_skip("c2011:opt:strncat_s");
    test_skip("c2011:opt:strnlen_s");
    test_skip("c2011:opt:strtok_s");
    test_skip("c2011:opt:strerror_s");
    test_skip("c2011:opt:strerrorlen_s");
    test_skip("c2011:opt:memcpy_s");
    test_skip("c2011:opt:memmove_s");
    test_skip("c2011:opt:memset_s");
#endif
}

TEST_CASE(string, c23_additions) {
#if defined(__STDC_VERSION_STRING_H__)
    test_true( __STDC_VERSION_STRING_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STRING_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_STRING_H__");
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L && defined(_CCW_TARGET_C)
    {
        char secret[8];
        memcpy(secret, "hunter2", 8);
        memset_explicit(secret, 0, sizeof secret);
        test_eq( (int)secret[0], 0 );
        test_eq( (int)secret[6], 0 );
    }
    test_pass("c2023:memset_explicit");
#else
    TEST_NOTE("memset_explicit is C23; not available in this configuration");
    test_skip("c2023:memset_explicit");
#endif

#if TEST_TARGET_C >= 2023 && (defined(_CCW_TARGET_C) || !defined(__GLIBC__) || defined(_POSIX_C_SOURCE) || defined(_DEFAULT_SOURCE))
    {
        char* d = strdup("abc");
        if (d) {
            test_true( strcmp(d, "abc") == 0 );
            free(d);
            test_pass("c2023:strdup");
        } else {
            TEST_NOTE("strdup returned NULL");
            TEST_SKIP1();
            test_skip("c2023:strdup");
        }
    }
#if !defined(__WATCOMC__)
    {
        char* d = strndup("abcdef", 3);
        if (d) {
            test_true( strcmp(d, "abc") == 0 );
            free(d);
            test_pass("c2023:strndup");
        } else {
            TEST_NOTE("strndup returned NULL");
            TEST_SKIP1();
            test_skip("c2023:strndup");
        }
    }
#else
    TEST_NOTE("Open Watcom has no strndup");
    test_skip("c2023:strndup");
#endif
#else
    test_skip("c2023:strdup");
    test_skip("c2023:strndup");
#endif
}

TEST_SUITE(string)
    TEST_ADD(size_t_null_defined)
    TEST_ADD(memchr_fn)
    TEST_ADD(bulk_memory_boundaries)
    TEST_ADD(bulk_string_boundaries)
    TEST_ADD(memccpy_c23)
    TEST_ADD(strnlen_c23)
    TEST_ADD(memcpy_memmove)
    TEST_ADD(memset_memcmp)
    TEST_ADD(strcpy_strncpy)
    TEST_ADD(strcat_strncat)
    TEST_ADD(strcmp_strncmp)
    TEST_ADD(strcoll_strxfrm)
    TEST_ADD(strchr_strrchr)
    TEST_ADD(strspn_strcspn_strpbrk)
    TEST_ADD(strstr_find)
    TEST_ADD(strtok_split)
    TEST_ADD(strlen_strerror)
    TEST_ADD(annex_k_bounds_checked)
    TEST_ADD(c23_additions)
TEST_SUITE_END(string)
