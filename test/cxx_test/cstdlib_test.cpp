#include "test_cxx.hpp"

#include <cstdlib>

TEST_CASE(cstdlib, convert) {
    test_eq( STD_NS atoi("123"), 123 );
    test_eq( STD_NS atoi("-45"), -45 );
    test_eq( STD_NS atol("100000"), 100000L );
    test_true( STD_NS atof("3.5") == 3.5 );

    char* end = 0;
    test_eq( STD_NS strtol("ff", &end, 16), 255L );
    test_eq( STD_NS strtol("777", &end, 8), 511L );
    test_eq( STD_NS strtol("12abc", &end, 10), 12L );
    test_eq( *end, 'a' );
    test_eq( STD_NS strtoul("4000000000", &end, 10), 4000000000UL );
    test_true( STD_NS strtod("2.25", &end) == 2.25 );
    test_pass("cxx03:<cstdlib> string-to-number reachable (smoke)");
}

TEST_CASE(cstdlib, math_util) {
    test_eq( STD_NS abs(-7), 7 );
    test_eq( STD_NS abs(7), 7 );
    test_eq( STD_NS labs(-8L), 8L );

    STD_NS div_t d = STD_NS div(17, 5);
    test_eq( d.quot, 3 );
    test_eq( d.rem, 2 );
    STD_NS ldiv_t ld = STD_NS ldiv(100L, 7L);
    test_eq( ld.quot, 14L );
    test_eq( ld.rem, 2L );
    test_pass("cxx03:<cstdlib> integer math reachable (smoke)");
}

static int cmp_int(const void* a, const void* b) {
    int x = *(const int*)a, y = *(const int*)b;
    return (x > y) - (x < y);
}

TEST_CASE(cstdlib, alloc_qsort_bsearch) {
    int* p = (int*)STD_NS malloc(5 * sizeof(int));
    test_ptr( p );
    p[0] = 3; p[1] = 1; p[2] = 4; p[3] = 1; p[4] = 5;

    STD_NS qsort(p, 5, sizeof(int), cmp_int);
    test_eq( p[0], 1 );
    test_eq( p[4], 5 );

    int key = 4;
    int* found = (int*)STD_NS bsearch(&key, p, 5, sizeof(int), cmp_int);
    test_ptr( found );
    test_eq( *found, 4 );

    p = (int*)STD_NS realloc(p, 8 * sizeof(int));
    test_ptr( p );
    STD_NS free(p);

    int* z = (int*)STD_NS calloc(4, sizeof(int));
    test_ptr( z );
    test_eq( z[0], 0 );
    test_eq( z[3], 0 );
    STD_NS free(z);
    test_pass("cxx03:<cstdlib> alloc and search reachable (smoke)");
}

TEST_CASE(cstdlib, rand) {
    STD_NS srand(12345);
    int a = STD_NS rand();
    test_true( a >= 0 && a <= RAND_MAX );
    test_pass("cxx03:rand (basic)");
    STD_NS srand(12345);
    int b = STD_NS rand();
    test_eq( a, b );
    test_pass("cxx03:srand (basic)");
}

#if TEST_TARGET_CXX >= 2011
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cerrno>
#include <climits>
#include <cwchar>
#include <type_traits>

using _ccw::string_view;

namespace {

extern "C" {
static int cmp_int_c(const void* a, const void* b) {
    int x = *static_cast<const int*>(a);
    int y = *static_cast<const int*>(b);
    return (x > y) - (x < y);
}
}

int cmp_int_cxx(const void* a, const void* b) {
    return cmp_int_c(a, b);
}

int g_handler_marker = 0;

extern "C" {
static void on_exit_c(void)            { g_handler_marker |= 1; }
static void on_quick_exit_c(void)      { g_handler_marker |= 4; }
}
void           on_exit_cxx(void)       { g_handler_marker |= 2; }
void           on_quick_exit_cxx(void) { g_handler_marker |= 8; }

}

TEST_CASE(cstdlib, types_and_members) {

    test_true( (STD::is_unsigned<STD_NS size_t>::value) );
#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<STD_NS size_t, decltype(sizeof(int))>::value) );
    test_pass("cxx03:size_t");
#else
    TEST_SKIP1();
    test_skip("cxx03:size_t");
#endif

    STD_NS div_t   d  = STD_NS div(7, 2);
    test_eq( d.quot, 3 );
    test_eq( d.rem,  1 );
    test_pass("cxx03:div_t");

    STD_NS ldiv_t  ld = STD_NS ldiv(7L, 2L);
    test_eq( ld.quot, 3L );
    test_eq( ld.rem,  1L );
    test_pass("cxx03:ldiv_t");

    STD_NS lldiv_t lld = STD_NS lldiv(7LL, 2LL);
    test_eq( lld.quot, 3LL );
    test_eq( lld.rem,  1LL );
    TEST_NOTE("lldiv_t is C++11");
    test_pass("cxx11:lldiv_t");

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<int,       decltype(STD_NS div_t().quot)>::value) );
    test_true( (STD::is_same<int,       decltype(STD_NS div_t().rem )>::value) );
    test_true( (STD::is_same<long,      decltype(STD_NS ldiv_t().quot)>::value) );
    test_true( (STD::is_same<long,      decltype(STD_NS ldiv_t().rem )>::value) );
    test_true( (STD::is_same<long long, decltype(STD_NS lldiv_t().quot)>::value) );
    test_true( (STD::is_same<long long, decltype(STD_NS lldiv_t().rem )>::value) );
    test_pass("cxx11:div_t ldiv_t lldiv_t quot,rem member-type property");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:div_t ldiv_t lldiv_t quot,rem member-type property");
#endif
}

TEST_CASE(cstdlib, macros) {

    void* p = NULL;
    test_true( p == 0 );
    test_pass("cxx03:NULL");

    test_eq( EXIT_SUCCESS, 0 );
    test_pass("cxx03:EXIT_SUCCESS");

    test_ne( EXIT_FAILURE, EXIT_SUCCESS );
    test_pass("cxx03:EXIT_FAILURE");

    test_ge( (long)RAND_MAX, 32767L );
    test_pass("cxx03:RAND_MAX");

    test_ge( (STD_NS size_t)MB_CUR_MAX, (STD_NS size_t)1 );
    test_le( (STD_NS size_t)MB_CUR_MAX, (STD_NS size_t)MB_LEN_MAX );
    test_pass("cxx03:MB_CUR_MAX");
}

TEST_CASE(cstdlib, start_and_termination) {

    void (*p_abort)()      = &STD_NS abort;
    test_ptr( (void*)p_abort );
    test_pass("cxx03:abort");

    void (*p_exit)(int)    = &STD_NS exit;
    test_ptr( (void*)p_exit );
    test_pass("cxx03:exit");

    int r1 = STD_NS atexit(&on_exit_c);
    int r2 = STD_NS atexit(&on_exit_cxx);
    test_eq( r1, 0 );
    test_eq( r2, 0 );
    test_pass("cxx03:atexit");

    void (*p__Exit)(int)   = &STD_NS _Exit;
    test_ptr( (void*)p__Exit );
    test_pass("cxx11:_Exit");

    void (*p_quick)(int)   = &STD_NS quick_exit;
    test_ptr( (void*)p_quick );
    test_pass("cxx11:quick_exit");

    int r3 = STD_NS at_quick_exit(&on_quick_exit_c);
    int r4 = STD_NS at_quick_exit(&on_quick_exit_cxx);
    test_eq( r3, 0 );
    test_eq( r4, 0 );

    test_eq( g_handler_marker, 0 );
    test_pass("cxx11:at_quick_exit");
}

TEST_CASE(cstdlib, environment) {

    const char* none = STD_NS getenv("__CCW_NO_SUCH_ENV_VAR_37__");
    test_true( none == NULL );

    const char* path = STD_NS getenv("PATH");
    if (path)
        test_gt( STD::strlen(path), (STD_NS size_t)0 );
    test_pass("cxx03:getenv");

    int has_shell = STD_NS system(NULL);
    test_ne( has_shell, 0 );
    test_pass("cxx03:system");
}

TEST_CASE(cstdlib, allocation) {

    void* p = STD_NS malloc(32);
    test_ptr( p );
    STD::memset(p, 0x5A, 32);
    test_eq( (int)*static_cast<unsigned char*>(p), 0x5A );
    test_pass("cxx03:malloc");

    unsigned char* c = static_cast<unsigned char*>(STD_NS calloc(8, sizeof(unsigned char)));
    test_ptr( c );
    int sum = 0;
    for (int i = 0; i < 8; ++i) sum += c[i];
    test_eq( sum, 0 );
    STD_NS free(c);

    int* ints = static_cast<int*>(STD_NS calloc(4, sizeof(int)));
    test_ptr( ints );
    test_eq( ints[3], 0 );
    STD_NS free(ints);
    test_pass("cxx03:calloc");

    void* q = STD_NS realloc(p, 64);
    test_ptr( q );
    test_eq( (int)*static_cast<unsigned char*>(q), 0x5A );
    STD_NS free(q);

    void* r = STD_NS realloc(NULL, 16);
    test_ptr( r );
    STD_NS free(r);
    test_pass("cxx03:realloc");

    test_no_throw( STD_NS free(NULL) );

    void* z = STD_NS malloc(0);
    STD_NS free(z);
    test_pass("cxx03:free");
}

TEST_CASE(cstdlib, ato_family) {
    test_eq( STD_NS atoi("42"), 42 );
    test_eq( STD_NS atoi("  -7abc"), -7 );
    test_eq( STD_NS atoi("abc"), 0 );
    test_eq( STD_NS atoi("+3"), 3 );
    test_pass("cxx03:atoi");

    test_eq( STD_NS atol("123456"), 123456L );
    test_eq( STD_NS atol("-1"), -1L );
    test_pass("cxx03:atol");

    double d = STD_NS atof("3.5");
    test_true( d > 3.49 && d < 3.51 );
    double e = STD_NS atof("-1e2");
    test_true( e > -100.01 && e < -99.99 );
    test_eq( STD_NS atof("xyz"), 0.0 );
    test_pass("cxx03:atof");

    test_eq( STD_NS atoll("9007199254740993"), 9007199254740993LL );
    test_eq( STD_NS atoll("-2"), -2LL );
    test_pass("cxx11:atoll");
}

TEST_CASE(cstdlib, strto_integer) {
    char* end = NULL;

    long l = STD_NS strtol("  123rest", &end, 10);
    test_eq( l, 123L );
    test_str_eq( end, "rest" );

    test_eq( STD_NS strtol("ff", NULL, 16), 255L );
    test_eq( STD_NS strtol("0x1f", NULL, 16), 31L );
    test_eq( STD_NS strtol("0x10", NULL, 0), 16L );
    test_eq( STD_NS strtol("010", NULL, 0), 8L );
    test_eq( STD_NS strtol("10", NULL, 0), 10L );
    test_eq( STD_NS strtol("-101", NULL, 2), -5L );

    const char* junk = "zz";
    long z = STD_NS strtol(junk, &end, 10);
    test_eq( z, 0L );
    test_true( end == junk );

    errno = 0;
    long big = STD_NS strtol("99999999999999999999", NULL, 10);
    test_eq( big, LONG_MAX );
    test_eq( errno, ERANGE );

    errno = 0;
    long small = STD_NS strtol("-99999999999999999999", NULL, 10);
    test_eq( small, LONG_MIN );
    test_eq( errno, ERANGE );
    TEST_NOTE("strtol/strtoul are C++03; strtoll/strtoull are C++11");
    test_pass("cxx11:strtol");

    errno = 0;
    long long ll = STD_NS strtoll("-9000000000", &end, 10);
    test_eq( ll, -9000000000LL );
    test_eq( (int)*end, 0 );
    test_eq( errno, 0 );

    errno = 0;
    long long llbig = STD_NS strtoll("99999999999999999999999", NULL, 10);
    test_eq( llbig, LLONG_MAX );
    test_eq( errno, ERANGE );
    test_pass("cxx11:strtoll");

    errno = 0;
    unsigned long ul = STD_NS strtoul("4294967295", &end, 10);
    test_eq( ul, 4294967295UL );
    test_eq( (int)*end, 0 );

    errno = 0;
    unsigned long ulbig = STD_NS strtoul("999999999999999999999", NULL, 10);
    test_eq( ulbig, ULONG_MAX );
    test_eq( errno, ERANGE );
    test_pass("cxx11:strtoul");

    errno = 0;
    unsigned long long ull = STD_NS strtoull("18446744073709551615", NULL, 10);
    test_eq( ull, ULLONG_MAX );
    test_eq( errno, 0 );

    errno = 0;
    unsigned long long ullbig = STD_NS strtoull("99999999999999999999999999", NULL, 10);
    test_eq( ullbig, ULLONG_MAX );
    test_eq( errno, ERANGE );
    errno = 0;
    test_pass("cxx11:strtoull");
}

TEST_CASE(cstdlib, strto_floating) {
    char* end = NULL;

    double d = STD_NS strtod("2.5e1xyz", &end);
    test_true( d > 24.99 && d < 25.01 );
    test_str_eq( end, "xyz" );

    const char* junk = "abc";
    double z = STD_NS strtod(junk, &end);
    test_eq( z, 0.0 );
    test_true( end == junk );

    errno = 0;
    double over = STD_NS strtod("1e400", NULL);
    test_eq( errno, ERANGE );
    test_gt( over, 1e300 );
    errno = 0;
    TEST_NOTE("strtod is C++03; strtof/strtold are C++11");
    test_pass("cxx11:strtod");

    float f = STD_NS strtof("-0.5", &end);
    test_true( f > -0.51f && f < -0.49f );
    test_eq( (int)*end, 0 );
    test_pass("cxx11:strtof");

    long double ld = STD_NS strtold("1.25", &end);
    test_true( ld > 1.24L && ld < 1.26L );
    test_eq( (int)*end, 0 );
    test_pass("cxx11:strtold");
}

TEST_CASE(cstdlib, rand_srand) {

    STD_NS srand(1u);
    int a1 = STD_NS rand();
    int a2 = STD_NS rand();
    test_lim( a1, 0, RAND_MAX );
    test_lim( a2, 0, RAND_MAX );
    test_pass("cxx03:rand");

    STD_NS srand(1u);
    int b1 = STD_NS rand();
    int b2 = STD_NS rand();
    test_eq( b1, a1 );
    test_eq( b2, a2 );

    STD_NS srand(12345u);
    int c1 = STD_NS rand();
    test_lim( c1, 0, RAND_MAX );
    test_pass("cxx03:srand");
}

TEST_CASE(cstdlib, bsearch_qsort) {
    int v[] = { 5, 3, 9, 1, 7 };
    const STD_NS size_t n = sizeof v / sizeof v[0];

    STD_NS qsort(v, n, sizeof v[0], &cmp_int_c);
    test_eq( v[0], 1 );
    test_eq( v[1], 3 );
    test_eq( v[2], 5 );
    test_eq( v[3], 7 );
    test_eq( v[4], 9 );

    int w[] = { 4, 2, 8, 6 };
    STD_NS qsort(w, 4, sizeof w[0], &cmp_int_cxx);
    test_eq( w[0], 2 );
    test_eq( w[3], 8 );
    test_pass("cxx03:qsort");

    int key = 7;
    void* f1 = STD_NS bsearch(&key, v, n, sizeof v[0], &cmp_int_c);
    test_true( f1 == (void*)(v + 3) );

    void* f2 = STD_NS bsearch(&key, v, n, sizeof v[0], &cmp_int_cxx);
    test_true( f2 == (void*)(v + 3) );

    const int* cv = v;
    const void* f3 = STD_NS bsearch(&key, cv, n, sizeof v[0], &cmp_int_c);
    test_true( f3 == (const void*)(v + 3) );

    const void* f4 = STD_NS bsearch(&key, cv, n, sizeof v[0], &cmp_int_cxx);
    test_true( f4 == (const void*)(v + 3) );

    int missing = 6;
    test_true( STD_NS bsearch(&missing, v, n, sizeof v[0], &cmp_int_c) == NULL );

    test_true( STD_NS bsearch(&key, v, 0, sizeof v[0], &cmp_int_c) == NULL );
    test_pass("cxx03:bsearch");
}

TEST_CASE(cstdlib, abs_and_div) {

    test_eq( STD_NS abs(-5), 5 );
    test_eq( STD_NS abs(5), 5 );
    test_eq( STD_NS abs(0), 0 );
    test_eq( STD_NS abs(-5L), 5L );
#if !defined(__WATCOMC__)
    test_eq( STD_NS abs(-5LL), 5LL );
#else
    TEST_SKIP1();
#endif
#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<int,       decltype(STD_NS abs(-1))>::value) );
    test_true( (STD::is_same<long,      decltype(STD_NS abs(-1L))>::value) );
    test_true( (STD::is_same<long long, decltype(STD_NS abs(-1LL))>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
    TEST_NOTE("abs/labs/div/ldiv are C++03; long long overloads + llabs/lldiv are C++11");
    test_pass("cxx11:abs");

    test_eq( STD_NS labs(-123456L), 123456L );
    test_pass("cxx11:labs");

    test_eq( STD_NS llabs(-9000000000LL), 9000000000LL );
    test_pass("cxx11:llabs");

    STD_NS div_t d = STD_NS div(-7, 2);
    test_eq( d.quot, -3 );
    test_eq( d.rem, -1 );
    test_eq( d.quot * 2 + d.rem, -7 );

    STD_NS ldiv_t  d2 = STD_NS div(7L, 2L);
    test_eq( d2.quot, 3L );
#if !defined(__WATCOMC__)
    STD_NS lldiv_t d3 = STD_NS div(7LL, 2LL);
    test_eq( d3.quot, 3LL );
#else
    TEST_SKIP1();
#endif
#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<STD_NS ldiv_t,  decltype(STD_NS div(1L, 1L))>::value) );
    test_true( (STD::is_same<STD_NS lldiv_t, decltype(STD_NS div(1LL, 1LL))>::value) );
    test_pass("cxx11:div");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:div");
#endif

    STD_NS ldiv_t ld = STD_NS ldiv(-7L, 2L);
    test_eq( ld.quot, -3L );
    test_eq( ld.rem, -1L );
    test_pass("cxx11:ldiv");

    STD_NS lldiv_t lld = STD_NS lldiv(9000000001LL, 3LL);
    test_eq( lld.quot, 3000000000LL );
    test_eq( lld.rem, 1LL );
    test_pass("cxx11:lldiv");
}

TEST_CASE(cstdlib, multibyte) {

    int mb1 = STD_NS mblen("a", MB_CUR_MAX);
    test_eq( mb1, 1 );
    int mb0 = STD_NS mblen("", 1);
    test_eq( mb0, 0 );
    STD_NS mblen(NULL, 0);
    test_pass("cxx03:mblen");

    wchar_t wc = 0;
    int used = STD_NS mbtowc(&wc, "A", MB_CUR_MAX);
    test_eq( used, 1 );
    test_eq( (int)wc, (int)L'A' );
    STD_NS mbtowc(NULL, NULL, 0);
    test_pass("cxx03:mbtowc");

    char mb[8];
    int n = STD_NS wctomb(mb, L'Z');
    test_eq( n, 1 );
    test_eq( (int)mb[0], 'Z' );
    test_pass("cxx03:wctomb");

    STD_NS size_t need = STD_NS mbstowcs(NULL, "abc", 0);
    test_eq( need, (STD_NS size_t)3 );

    wchar_t wbuf[8];
    STD_NS size_t got = STD_NS mbstowcs(wbuf, "abc", 8);
    test_eq( got, (STD_NS size_t)3 );
    test_eq( (int)wbuf[0], (int)L'a' );
    test_eq( (int)wbuf[2], (int)L'c' );
    test_eq( (int)wbuf[3], 0 );
    test_pass("cxx03:mbstowcs");

    STD_NS size_t need2 = STD_NS wcstombs(NULL, L"abc", 0);
    test_eq( need2, (STD_NS size_t)3 );

    char cbuf[8];
    STD_NS size_t got2 = STD_NS wcstombs(cbuf, L"abc", 8);
    test_eq( got2, (STD_NS size_t)3 );
    test_str_eq( cbuf, "abc" );
    test_pass("cxx03:wcstombs");
}
#else
TEST_CASE_SKIP(cstdlib, types_and_members)
TEST_CASE_SKIP(cstdlib, macros)
TEST_CASE_SKIP(cstdlib, start_and_termination)
TEST_CASE_SKIP(cstdlib, environment)
TEST_CASE_SKIP(cstdlib, allocation)
TEST_CASE_SKIP(cstdlib, ato_family)
TEST_CASE_SKIP(cstdlib, strto_integer)
TEST_CASE_SKIP(cstdlib, strto_floating)
TEST_CASE_SKIP(cstdlib, rand_srand)
TEST_CASE_SKIP(cstdlib, bsearch_qsort)
TEST_CASE_SKIP(cstdlib, abs_and_div)
TEST_CASE_SKIP(cstdlib, multibyte)
#endif

TEST_CASE(cstdlib, gap_fill) {

    test_skip("cxx03:gets");

#if TEST_TARGET_CXX >= 2017 && !defined(_MSC_VER) && !defined(__WATCOMC__) && !defined(__MINGW32__)
    void* p = STD_NS aligned_alloc(64, 128);
    test_ptr( p );
    test_true( ((STD_NS size_t)p % 64) == 0 );
    STD_NS free(p);
    test_pass("cxx17:aligned_alloc");
#else
    test_skip("cxx17:aligned_alloc");
#endif
}


TEST_CASE(cstdlib, c23_additions_cxx26) {
    TEST_NOTE("C++26/C23 <cstdlib> additions; no library here defines them");
    test_skip("cxx26:free_sized");
    test_skip("cxx26:free_aligned_sized");
    test_skip("cxx26:memalignment");
    test_skip("cxx26:strfromd");
    test_skip("cxx26:strfromf");
    test_skip("cxx26:strfroml");
}


TEST_CASE(cstdlib, div_result_members_cxx03) {
    STD_NS div_t d = STD_NS div(17, 5);
    test_eq( d.quot, 3 );
    test_pass("cxx03:div_t::quot");
    test_eq( d.rem, 2 );
    test_pass("cxx03:div_t::rem");
    test_eq( d.quot * 5 + d.rem, 17 );
    {
        STD_NS div_t n = STD_NS div(-17, 5);
        test_eq( n.quot, -3 );
        test_eq( n.rem, -2 );
    }

    STD_NS ldiv_t l = STD_NS ldiv(17L, 5L);
    test_eq( l.quot, 3L );
    test_pass("cxx03:ldiv_t::quot");
    test_eq( l.rem, 2L );
    test_pass("cxx03:ldiv_t::rem");
    test_eq( l.quot * 5L + l.rem, 17L );
}

#if TEST_TARGET_CXX >= 2011
TEST_CASE(cstdlib, lldiv_result_members_cxx11) {
    STD_NS lldiv_t l = STD_NS lldiv(17LL, 5LL);
    test_true( l.quot == 3LL );
    test_pass("cxx11:lldiv_t::quot");
    test_true( l.rem == 2LL );
    test_pass("cxx11:lldiv_t::rem");
    test_true( l.quot * 5LL + l.rem == 17LL );
    {
        STD_NS lldiv_t big = STD_NS lldiv(9223372036854775807LL, 1000LL);
        test_true( big.quot == 9223372036854775LL );
        test_true( big.rem == 807LL );
    }
}
#else
TEST_CASE(cstdlib, lldiv_result_members_cxx11) {
    test_skip("cxx11:lldiv_t::quot");
    test_skip("cxx11:lldiv_t::rem");
}
#endif
