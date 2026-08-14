#include "test_cxx.hpp"

#include <cstring>

TEST_CASE(cstring, copy_concat) {
    char buf[32];
    STD_NS strcpy(buf, "hello");
    test_eq( STD_NS strlen(buf), 5u );
    STD_NS strncpy(buf, "abcdef", 3);
    buf[3] = 0;
    test_eq( STD_NS strcmp(buf, "abc"), 0 );

    STD_NS strcpy(buf, "foo");
    STD_NS strcat(buf, "bar");
    test_eq( STD_NS strcmp(buf, "foobar"), 0 );
    STD_NS strncat(buf, "!!!", 2);
    test_eq( STD_NS strcmp(buf, "foobar!!"), 0 );
    test_pass("cxx03:<cstring> copy and concat reachable (smoke)");
}

TEST_CASE(cstring, compare_search) {
    test_eq( STD_NS strcmp("abc", "abc"), 0 );
    test_true( STD_NS strcmp("abc", "abd") < 0 );
    test_eq( STD_NS strncmp("abcxx", "abcyy", 3), 0 );

    const char* s = "hello world";
    test_true( STD_NS strchr(s, 'w') == s + 6 );
    test_true( STD_NS strrchr(s, 'o') == s + 7 );
    test_true( STD_NS strstr(s, "wor") == s + 6 );
    test_eq( STD_NS strspn("aabbc", "ab"), 4u );
    test_eq( STD_NS strcspn("abcde", "cd"), 2u );
    const char* t = "abcde";
    test_true( STD_NS strpbrk(t, "dc") == t + 2 );

    char tok[] = "a,b,c";
    char* p = STD_NS strtok(tok, ",");
    test_eq( STD_NS strcmp(p, "a"), 0 );
    p = STD_NS strtok(0, ",");
    test_eq( STD_NS strcmp(p, "b"), 0 );
    test_pass("cxx03:<cstring> compare and search reachable (smoke)");
}

TEST_CASE(cstring, memory) {
    char a[8], b[8];
    STD_NS memset(a, 'x', 8);
    test_eq( a[0], 'x' );
    test_eq( a[7], 'x' );
    STD_NS memcpy(b, a, 8);
    test_eq( STD_NS memcmp(a, b, 8), 0 );
    STD_NS memset(b, 0, 8);
    test_true( STD_NS memcmp(a, b, 8) != 0 );

    char ov[8] = { '0','1','2','3','4','5','6','7' };
    STD_NS memmove(ov + 2, ov, 4);
    test_eq( ov[2], '0' );
    test_eq( ov[5], '3' );

    test_true( STD_NS memchr(a, 'x', 8) == a );
    test_true( STD_NS memchr(b, 'x', 8) == 0 );

    test_true( STD_NS strerror(0) != 0 );
    test_pass("cxx03:<cstring> memory ops reachable (smoke)");
}

#if TEST_TARGET_CXX >= 2026 && _TST_HAS_STD_C23_LIB
TEST_CASE(cstring, memccpy) {
    char dst[8];
    STD_NS memset(dst, 0, sizeof dst);
    void* p = STD_NS memccpy(dst, "abcdef", 'c', sizeof dst);
    test_true( p != 0 );
    test_true( STD_NS memcmp(dst, "abc", 3) == 0 );

    test_eq( (int)((char*)p - dst), 3 );

    STD_NS memset(dst, 0, sizeof dst);
    void* q = STD_NS memccpy(dst, "xy", 'z', sizeof dst);
    test_true( q == 0 );
    test_true( STD_NS memcmp(dst, "xy", 2) == 0 );
    test_pass("cxx26:memccpy");
}
#else
TEST_CASE_SKIP(cstring, memccpy)
#endif

#if TEST_TARGET_CXX >= 2011
#include <cstring>
#include <cstddef>
#include <cerrno>
#include <type_traits>

using _ccw::string_view;

TEST_CASE(cstring, types_and_macros) {

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<STD_NS size_t, decltype(STD_NS strlen(""))>::value) );
#else
    TEST_SKIP1();
#endif
    test_true( (STD::is_unsigned<STD_NS size_t>::value) );
    test_true( sizeof(STD_NS size_t) >= sizeof(unsigned) );
    test_pass("cxx03:size_t");

    const char* p = NULL;
    test_true( p == 0 );
    test_true( STD_NS strchr("abc", 'z') == NULL );
    test_pass("cxx03:NULL");
}

TEST_CASE(cstring, memcpy_memmove) {
    char dst[8];
    const char src[] = "abcdefg";

    void* r = STD_NS memcpy(dst, src, sizeof src);
    test_true( r == (void*)dst );
    test_str_eq( dst, "abcdefg" );

    char z[] = "xy";
    STD_NS memcpy(z, "AB", 0);
    test_str_eq( z, "xy" );
    test_pass("cxx03:memcpy");

    char buf[] = "abcdef";
    void* r2 = STD_NS memmove(buf, buf + 1, 6);
    test_true( r2 == (void*)buf );
    test_str_eq( buf, "bcdef" );

    char buf2[] = "abcdef";
    STD_NS memmove(buf2 + 1, buf2, 5);
    test_str_eq( buf2, "aabcde" );
    test_pass("cxx03:memmove");
}

TEST_CASE(cstring, memset_memcmp) {

    char b[6];
    void* r = STD_NS memset(b, 'x', 5);
    b[5] = '\0';
    test_true( r == (void*)b );
    test_str_eq( b, "xxxxx" );

    STD_NS memset(b, 0, sizeof b);
    test_eq( (int)b[0], 0 );
    test_eq( (int)b[4], 0 );
    test_pass("cxx03:memset");

    test_eq( STD_NS memcmp("abc", "abc", 3), 0 );
    test_lt( STD_NS memcmp("abc", "abd", 3), 0 );
    test_gt( STD_NS memcmp("abd", "abc", 3), 0 );
    test_eq( STD_NS memcmp("abX", "abY", 2), 0 );
    test_eq( STD_NS memcmp("", "", 0), 0 );

    const unsigned char hi[1] = { 0x80 };
    const unsigned char lo[1] = { 0x01 };
    test_gt( STD_NS memcmp(hi, lo, 1), 0 );
    test_pass("cxx03:memcmp");
}

TEST_CASE(cstring, memchr_overloads) {
    char        mut[]  = "abcdef";
    const char  cst[]  = "abcdef";

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<void*,
        decltype(STD_NS memchr(static_cast<void*>(mut), 'c', 6))>::value) );
    test_true( (STD::is_same<const void*,
        decltype(STD_NS memchr(static_cast<const void*>(cst), 'c', 6))>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    void* p = STD_NS memchr(static_cast<void*>(mut), 'c', 6);
    test_true( p == (void*)(mut + 2) );
    *static_cast<char*>(p) = 'C';
    test_str_eq( mut, "abCdef" );

    const void* q = STD_NS memchr(static_cast<const void*>(cst), 'f', 6);
    test_true( q == (const void*)(cst + 5) );

    test_true( STD_NS memchr(cst, 'z', 6) == NULL );
    test_true( STD_NS memchr(cst, 'a', 0) == NULL );
    test_pass("cxx03:memchr");
}

TEST_CASE(cstring, strcpy_strncpy) {
    char d[8];
    char* r = STD_NS strcpy(d, "abc");
    test_true( r == d );
    test_str_eq( d, "abc" );
    test_pass("cxx03:strcpy");

    char p[6];
    STD_NS memset(p, 'Z', sizeof p);
    char* r2 = STD_NS strncpy(p, "ab", 5);
    test_true( r2 == p );
    test_eq( (int)p[0], 'a' );
    test_eq( (int)p[2], 0 );
    test_eq( (int)p[4], 0 );
    test_eq( (int)p[5], 'Z' );

    char t[4] = { 'Q', 'Q', 'Q', 'Q' };
    STD_NS strncpy(t, "abcdef", 3);
    test_eq( (int)t[0], 'a' );
    test_eq( (int)t[2], 'c' );
    test_eq( (int)t[3], 'Q' );
    test_pass("cxx03:strncpy");
}

TEST_CASE(cstring, strcat_strncat) {
    char d[16] = "ab";
    char* r = STD_NS strcat(d, "cd");
    test_true( r == d );
    test_str_eq( d, "abcd" );

    STD_NS strcat(d, "");
    test_str_eq( d, "abcd" );
    test_pass("cxx03:strcat");

    char e[16] = "ab";
    char* r2 = STD_NS strncat(e, "cdef", 2);
    test_true( r2 == e );
    test_str_eq( e, "abcd" );
    test_eq( (int)e[4], 0 );

    STD_NS strncat(e, "xyz", 10);
    test_str_eq( e, "abcdxyz" );
    test_pass("cxx03:strncat");
}

TEST_CASE(cstring, strcmp_strncmp) {
    test_eq( STD_NS strcmp("abc", "abc"), 0 );
    test_lt( STD_NS strcmp("abc", "abd"), 0 );
    test_gt( STD_NS strcmp("abd", "abc"), 0 );
    test_lt( STD_NS strcmp("ab",  "abc"), 0 );
    test_gt( STD_NS strcmp("abc", "ab" ), 0 );
    test_eq( STD_NS strcmp("", ""), 0 );
    test_pass("cxx03:strcmp");

    test_eq( STD_NS strncmp("abc", "abd", 2), 0 );
    test_lt( STD_NS strncmp("abc", "abd", 3), 0 );
    test_eq( STD_NS strncmp("abc", "zzz", 0), 0 );
    test_lt( STD_NS strncmp("ab", "abc", 3), 0 );
    test_pass("cxx03:strncmp");
}

TEST_CASE(cstring, strcoll_strxfrm) {

    test_eq( STD_NS strcoll("abc", "abc"), 0 );
    test_lt( STD_NS strcoll("abc", "abd"), 0 );
    test_gt( STD_NS strcoll("abd", "abc"), 0 );
    test_pass("cxx03:strcoll");

    STD_NS size_t need = STD_NS strxfrm(NULL, "abc", 0);
    test_ge( need, (STD_NS size_t)3 );

    char buf[64];
    STD_NS size_t n = STD_NS strxfrm(buf, "abc", sizeof buf);
    test_eq( n, need );
    test_lt( n, (STD_NS size_t)sizeof buf );
    test_eq( (int)buf[n], 0 );

    char b2[64];
    STD_NS strxfrm(b2, "abd", sizeof b2);
    test_lt( STD_NS strcmp(buf, b2), 0 );
    test_pass("cxx03:strxfrm");
}

TEST_CASE(cstring, strchr_strrchr_overloads) {
    char       mut[] = "hello";
    const char cst[] = "hello";

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<char*,       decltype(STD_NS strchr(mut, 'l'))>::value) );
    test_true( (STD::is_same<const char*, decltype(STD_NS strchr(cst, 'l'))>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    char* f = STD_NS strchr(mut, 'l');
    test_true( f == mut + 2 );
    *f = 'L';
    test_str_eq( mut, "heLlo" );

    const char* c = STD_NS strchr(cst, 'h');
    test_true( c == cst );
    test_true( STD_NS strchr(cst, 'z') == NULL );
    test_true( STD_NS strchr(cst,  '\0') == cst + 5 );
    test_pass("cxx03:strchr");

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<char*,       decltype(STD_NS strrchr(mut, 'l'))>::value) );
    test_true( (STD::is_same<const char*, decltype(STD_NS strrchr(cst, 'l'))>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    char* b = STD_NS strrchr(mut, 'l');
    test_true( b == mut + 3 );

    test_true( STD_NS strrchr(cst, 'z') == NULL );
    test_true( STD_NS strrchr(cst, '\0') == cst + 5 );
    test_pass("cxx03:strrchr");
}

TEST_CASE(cstring, strspn_strcspn_strpbrk) {
    test_eq( STD_NS strspn("abcde", "abc"), (STD_NS size_t)3 );
    test_eq( STD_NS strspn("xabc",  "abc"), (STD_NS size_t)0 );
    test_eq( STD_NS strspn("aaa",   "a"  ), (STD_NS size_t)3 );
    test_eq( STD_NS strspn("abc",   ""   ), (STD_NS size_t)0 );
    test_pass("cxx03:strspn");

    test_eq( STD_NS strcspn("abcde", "dc"), (STD_NS size_t)2 );
    test_eq( STD_NS strcspn("abc",   "a" ), (STD_NS size_t)0 );
    test_eq( STD_NS strcspn("abc",   "z" ), (STD_NS size_t)3 );
    test_pass("cxx03:strcspn");

    char       mut[] = "a,b;c";
    const char cst[] = "a,b;c";

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<char*,       decltype(STD_NS strpbrk(mut, ",;"))>::value) );
    test_true( (STD::is_same<const char*, decltype(STD_NS strpbrk(cst, ",;"))>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    char* p = STD_NS strpbrk(mut, ";,");
    test_true( p == mut + 1 );
    *p = '|';
    test_str_eq( mut, "a|b;c" );

    const char* q = STD_NS strpbrk(cst, "c");
    test_true( q == cst + 4 );
    test_true( STD_NS strpbrk(cst, "xyz") == NULL );
    test_pass("cxx03:strpbrk");
}

TEST_CASE(cstring, strstr_overloads) {
    char       mut[] = "abcabc";
    const char cst[] = "abcabc";

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<char*,       decltype(STD_NS strstr(mut, "bc"))>::value) );
    test_true( (STD::is_same<const char*, decltype(STD_NS strstr(cst, "bc"))>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    char* p = STD_NS strstr(mut, "bc");
    test_true( p == mut + 1 );
    *p = 'B';
    test_str_eq( mut, "aBcabc" );

    const char* q = STD_NS strstr(cst, "cab");
    test_true( q == cst + 2 );
    test_true( STD_NS strstr(cst, "")   == cst );
    test_true( STD_NS strstr(cst, "zz") == NULL );
    test_true( STD_NS strstr(cst, "abcabcabc") == NULL );
    test_pass("cxx03:strstr");
}

TEST_CASE(cstring, strtok) {

    char buf[] = "a,b;;c";
    char* t = STD_NS strtok(buf, ",;");
    test_str_eq( t, "a" );

    t = STD_NS strtok(NULL, ",;");
    test_str_eq( t, "b" );

    t = STD_NS strtok(NULL, ",;");
    test_str_eq( t, "c" );

    t = STD_NS strtok(NULL, ",;");
    test_true( t == NULL );

    char b2[] = "  one|two";
    char* u = STD_NS strtok(b2, " ");
    test_str_eq( u, "one|two" );
    char b3[] = "x=1";
    char* v = STD_NS strtok(b3, "=");
    test_str_eq( v, "x" );
    v = STD_NS strtok(NULL, "=");
    test_str_eq( v, "1" );

    char only[] = ",,,";
    char* w = STD_NS strtok(only, ",");
    test_true( w == NULL );
    test_pass("cxx03:strtok");
}

TEST_CASE(cstring, strlen_strerror) {
    test_eq( STD_NS strlen(""), (STD_NS size_t)0 );
    test_eq( STD_NS strlen("abc"), (STD_NS size_t)3 );

    char buf[8] = "ab";
    test_eq( STD_NS strlen(buf), (STD_NS size_t)2 );
    buf[2] = 'c';
    buf[3] = '\0';
    test_eq( STD_NS strlen(buf), (STD_NS size_t)3 );
    test_pass("cxx03:strlen");

    char* e0 = STD_NS strerror(0);
    test_ptr( e0 );
    test_gt( STD_NS strlen(e0), (STD_NS size_t)0 );

    char m1[128];
    char* e1 = STD_NS strerror(EDOM);
    test_ptr( e1 );
    STD_NS strncpy(m1, e1, sizeof m1 - 1);
    m1[sizeof m1 - 1] = '\0';
    test_gt( STD_NS strlen(m1), (STD_NS size_t)0 );

    char m2[128];
    char* e2 = STD_NS strerror(ERANGE);
    test_ptr( e2 );
    STD_NS strncpy(m2, e2, sizeof m2 - 1);
    m2[sizeof m2 - 1] = '\0';
    test_gt( STD_NS strlen(m2), (STD_NS size_t)0 );

    int diff = STD_NS strcmp(m1, m2);
    test_ne( diff, 0 );
    test_pass("cxx03:strerror");
}
#else
TEST_CASE_SKIP(cstring, types_and_macros)
TEST_CASE_SKIP(cstring, memcpy_memmove)
TEST_CASE_SKIP(cstring, memset_memcmp)
TEST_CASE_SKIP(cstring, memchr_overloads)
TEST_CASE_SKIP(cstring, strcpy_strncpy)
TEST_CASE_SKIP(cstring, strcat_strncat)
TEST_CASE_SKIP(cstring, strcmp_strncmp)
TEST_CASE_SKIP(cstring, strcoll_strxfrm)
TEST_CASE_SKIP(cstring, strchr_strrchr_overloads)
TEST_CASE_SKIP(cstring, strspn_strcspn_strpbrk)
TEST_CASE_SKIP(cstring, strstr_overloads)
TEST_CASE_SKIP(cstring, strtok)
TEST_CASE_SKIP(cstring, strlen_strerror)
#endif


TEST_CASE(cstring, c23_additions_cxx26) {
    TEST_NOTE("C++26 <cstring> additions; no library here puts these in std");
    test_skip("cxx26:memset_explicit");
    test_skip("cxx26:strdup");
    test_skip("cxx26:strndup");
}
