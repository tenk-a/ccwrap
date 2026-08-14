#include "test_cxx.hpp"

#include <cinttypes>

TEST_CASE(cinttypes, imaxdiv) {
    STD_NS imaxdiv_t __d = STD_NS imaxdiv((STD_NS intmax_t)7, (STD_NS intmax_t)2);
    test_eq( (long)__d.quot, 3L );
    test_eq( (long)__d.rem,  1L );

    STD_NS imaxdiv_t __n = STD_NS imaxdiv((STD_NS intmax_t)-7, (STD_NS intmax_t)2);
    test_eq( (long)__n.quot, -3L );
    test_eq( (long)__n.rem,  -1L );
    test_pass("cxx11:imaxdiv (basic)");
}

#if TEST_TARGET_CXX >= 2011
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cwchar>
#include <string>
#include <type_traits>

using _ccw::string_view;

template <class T>
static STD::string ci_fmt(char const* f, T v) {
    char buf[64];
    STD::snprintf(buf, sizeof buf, f, v);
    return STD::string(buf);
}

template <class T>
static T ci_scan(char const* text, char const* f) {
    T v = T(0);
    STD::sscanf(text, f, &v);
    return v;
}

#define CHK_PRI(ST, UT, Pd, Pi, Po, Pu, Px, PX)                     \
    do {                                                            \
        ST const sv = (ST)-42;                                      \
        UT const uv = (UT)0xABu;                                    \
        test_str_eq( ci_fmt("%" Pd, sv), "-42" );                   \
        test_str_eq( ci_fmt("%" Pi, sv), "-42" );                   \
        test_str_eq( ci_fmt("%" Po, uv), "253" );                   \
        test_str_eq( ci_fmt("%" Pu, uv), "171" );                   \
        test_str_eq( ci_fmt("%" Px, uv), "ab" );                    \
        test_str_eq( ci_fmt("%" PX, uv), "AB" );                    \
    } while (0)

#define CHK_SCN(ST, UT, Sd, Si, So, Su, Sx)                         \
    do {                                                            \
        ST const sd = ci_scan<ST>("-42", "%" Sd);                   \
        test_eq( (long long)sd, -42LL );                            \
        ST const si = ci_scan<ST>("-42", "%" Si);                   \
        test_eq( (long long)si, -42LL );                            \
        UT const so = ci_scan<UT>("253", "%" So);                   \
        test_eq( (unsigned long long)so, 171ULL );                  \
        UT const su = ci_scan<UT>("171", "%" Su);                   \
        test_eq( (unsigned long long)su, 171ULL );                  \
        UT const sx = ci_scan<UT>("ab", "%" Sx);                    \
        test_eq( (unsigned long long)sx, 171ULL );                  \
    } while (0)

TEST_CASE(cinttypes, imaxdiv_t_and_imaxdiv) {

    STD_NS imaxdiv_t d = STD_NS imaxdiv((STD_NS intmax_t)17, (STD_NS intmax_t)5);

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<decltype(d.quot), STD_NS intmax_t>::value) );
    test_true( (STD::is_same<decltype(d.rem),  STD_NS intmax_t>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif
    STD_NS imaxdiv_t cp = d;
    test_eq( (long long)cp.quot, 3LL );
    test_eq( (long long)cp.rem,  2LL );
    test_pass("cxx11:imaxdiv_t");

    test_eq( (long long)d.quot, 3LL );
    test_eq( (long long)d.rem,  2LL );

    STD_NS imaxdiv_t a = STD_NS imaxdiv((STD_NS intmax_t)-17, (STD_NS intmax_t)5);
    test_eq( (long long)a.quot, -3LL );
    test_eq( (long long)a.rem,  -2LL );
    test_eq( (long long)(a.quot * 5 + a.rem), -17LL );

    STD_NS imaxdiv_t b = STD_NS imaxdiv((STD_NS intmax_t)17, (STD_NS intmax_t)-5);
    test_eq( (long long)b.quot, -3LL );
    test_eq( (long long)b.rem,   2LL );
    test_eq( (long long)(b.quot * -5 + b.rem), 17LL );

    STD_NS imaxdiv_t c = STD_NS imaxdiv((STD_NS intmax_t)-17, (STD_NS intmax_t)-5);
    test_eq( (long long)c.quot, 3LL );
    test_eq( (long long)c.rem, -2LL );

    STD_NS imaxdiv_t w = STD_NS imaxdiv((STD_NS intmax_t)9223372036854775807LL, (STD_NS intmax_t)1000);
    test_eq( (long long)w.quot, 9223372036854775LL );
    test_eq( (long long)w.rem,  807LL );
    test_pass("cxx11:imaxdiv");
}

TEST_CASE(cinttypes, imaxabs) {
    test_eq( (long long)STD_NS imaxabs((STD_NS intmax_t)0),   0LL );
    test_eq( (long long)STD_NS imaxabs((STD_NS intmax_t)7),   7LL );
    test_eq( (long long)STD_NS imaxabs((STD_NS intmax_t)-7),  7LL );
    test_eq( (long long)STD_NS imaxabs((STD_NS intmax_t)-1234567890123LL), 1234567890123LL );
    test_eq( (long long)STD_NS imaxabs(INTMAX_MAX), (long long)INTMAX_MAX );

    STD_NS intmax_t r = STD_NS imaxabs((STD_NS intmax_t)-1);
#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<decltype(STD_NS imaxabs((STD_NS intmax_t)0)),
                             STD_NS intmax_t>::value) );
#else
    TEST_SKIP1();
#endif
    test_eq( (long long)r, 1LL );
    test_pass("cxx11:imaxabs");
}

TEST_CASE(cinttypes, strtoimax_strtoumax) {
    char* end = 0;
    char const* s = "  -12345xyz";
    STD_NS intmax_t v = STD_NS strtoimax(s, &end, 10);
    test_eq( (long long)v, -12345LL );
    test_str_eq( end, "xyz" );

    char const* h = "0x1Fzz";
    end = 0;
    STD_NS intmax_t hv = STD_NS strtoimax(h, &end, 16);
    test_eq( (long long)hv, 31LL );
    test_eq( (int)(end - h), 4 );
    test_eq( (long long)STD_NS strtoimax("7f", 0, 16), 127LL );

    test_eq( (long long)STD_NS strtoimax("0755", 0, 0), 493LL );
    test_eq( (long long)STD_NS strtoimax("0X10", 0, 0), 16LL );
    test_eq( (long long)STD_NS strtoimax("-99", 0, 0), -99LL );

    test_eq( (long long)STD_NS strtoimax("9223372036854775807", 0, 10),
             (long long)INTMAX_MAX );

    char const* bad = "zz";
    end = 0;
    STD_NS intmax_t bv = STD_NS strtoimax(bad, &end, 10);
    test_eq( (long long)bv, 0LL );
    test_true( end == bad );
    test_pass("cxx11:strtoimax");

    char* uend = 0;
    STD_NS uintmax_t u = STD_NS strtoumax("18446744073709551615", &uend, 10);
    test_eq( (unsigned long long)u, (unsigned long long)UINTMAX_MAX );
    test_str_eq( uend, "" );
    test_eq( (unsigned long long)STD_NS strtoumax("ff", 0, 16), 255ULL );
    test_eq( (unsigned long long)STD_NS strtoumax("101", 0, 2), 5ULL );
    test_eq( (unsigned long long)STD_NS strtoumax("  +42 ", 0, 10), 42ULL );
    test_pass("cxx11:strtoumax");
}

TEST_CASE(cinttypes, wcstoimax_wcstoumax) {
    wchar_t* end = 0;
    wchar_t const* s = L"  -987654321abc";
    STD_NS intmax_t v = STD_NS wcstoimax(s, &end, 10);
    test_eq( (long long)v, -987654321LL );
    test_true( end != 0 && *end == L'a' );
    test_eq( (int)(end - s), 12 );

    test_eq( (long long)STD_NS wcstoimax(L"7f", 0, 16), 127LL );
    test_eq( (long long)STD_NS wcstoimax(L"0755", 0, 0), 493LL );
    test_eq( (long long)STD_NS wcstoimax(L"9223372036854775807", 0, 10),
             (long long)INTMAX_MAX );

    wchar_t const* bad = L"zz";
    end = 0;
    STD_NS intmax_t bv = STD_NS wcstoimax(bad, &end, 10);
    test_eq( (long long)bv, 0LL );
    test_true( end == bad );
    test_pass("cxx11:wcstoimax");

    wchar_t* uend = 0;
    STD_NS uintmax_t u = STD_NS wcstoumax(L"18446744073709551615", &uend, 10);
    test_eq( (unsigned long long)u, (unsigned long long)UINTMAX_MAX );
    test_true( uend != 0 && *uend == L'\0' );
    test_eq( (unsigned long long)STD_NS wcstoumax(L"ff", 0, 16), 255ULL );
    test_eq( (unsigned long long)STD_NS wcstoumax(L"101", 0, 2), 5ULL );
    test_pass("cxx11:wcstoumax");
}

TEST_CASE(cinttypes, pri_exact_width) {
    CHK_PRI(STD_NS int8_t,  STD_NS uint8_t,  PRId8,  PRIi8,  PRIo8,  PRIu8,  PRIx8,  PRIX8);
    CHK_PRI(STD_NS int16_t, STD_NS uint16_t, PRId16, PRIi16, PRIo16, PRIu16, PRIx16, PRIX16);
    CHK_PRI(STD_NS int32_t, STD_NS uint32_t, PRId32, PRIi32, PRIo32, PRIu32, PRIx32, PRIX32);
    CHK_PRI(STD_NS int64_t, STD_NS uint64_t, PRId64, PRIi64, PRIo64, PRIu64, PRIx64, PRIX64);

    STD_NS int32_t v = 5;
    test_str_eq( ci_fmt("%05" PRId32, v), "00005" );
    STD_NS uint64_t big = 12345678901234567890ull;
    test_str_eq( ci_fmt("%" PRIu64, big), "12345678901234567890" );
    STD_NS int64_t neg = -9223372036854775807LL - 1;
    test_str_eq( ci_fmt("%" PRId64, neg), "-9223372036854775808" );
    test_pass("cxx11:PRI* exact-width");
}

TEST_CASE(cinttypes, pri_least_width) {
    CHK_PRI(STD_NS int_least8_t,  STD_NS uint_least8_t,
            PRIdLEAST8,  PRIiLEAST8,  PRIoLEAST8,  PRIuLEAST8,  PRIxLEAST8,  PRIXLEAST8);
    CHK_PRI(STD_NS int_least16_t, STD_NS uint_least16_t,
            PRIdLEAST16, PRIiLEAST16, PRIoLEAST16, PRIuLEAST16, PRIxLEAST16, PRIXLEAST16);
    CHK_PRI(STD_NS int_least32_t, STD_NS uint_least32_t,
            PRIdLEAST32, PRIiLEAST32, PRIoLEAST32, PRIuLEAST32, PRIxLEAST32, PRIXLEAST32);
    CHK_PRI(STD_NS int_least64_t, STD_NS uint_least64_t,
            PRIdLEAST64, PRIiLEAST64, PRIoLEAST64, PRIuLEAST64, PRIxLEAST64, PRIXLEAST64);
    test_pass("cxx11:PRI* least-width");
}

TEST_CASE(cinttypes, pri_fast_width) {
    CHK_PRI(STD_NS int_fast8_t,  STD_NS uint_fast8_t,
            PRIdFAST8,  PRIiFAST8,  PRIoFAST8,  PRIuFAST8,  PRIxFAST8,  PRIXFAST8);
    CHK_PRI(STD_NS int_fast16_t, STD_NS uint_fast16_t,
            PRIdFAST16, PRIiFAST16, PRIoFAST16, PRIuFAST16, PRIxFAST16, PRIXFAST16);
    CHK_PRI(STD_NS int_fast32_t, STD_NS uint_fast32_t,
            PRIdFAST32, PRIiFAST32, PRIoFAST32, PRIuFAST32, PRIxFAST32, PRIXFAST32);
    CHK_PRI(STD_NS int_fast64_t, STD_NS uint_fast64_t,
            PRIdFAST64, PRIiFAST64, PRIoFAST64, PRIuFAST64, PRIxFAST64, PRIXFAST64);
    test_pass("cxx11:PRI* fast-width");
}

TEST_CASE(cinttypes, pri_max_and_ptr) {
    CHK_PRI(STD_NS intmax_t, STD_NS uintmax_t,
            PRIdMAX, PRIiMAX, PRIoMAX, PRIuMAX, PRIxMAX, PRIXMAX);

    STD_NS intmax_t im = INTMAX_MAX;
    test_str_eq( ci_fmt("%" PRIdMAX, im), "9223372036854775807" );
    test_pass("cxx11:PRI* MAX");

    CHK_PRI(STD_NS intptr_t, STD_NS uintptr_t,
            PRIdPTR, PRIiPTR, PRIoPTR, PRIuPTR, PRIxPTR, PRIXPTR);

    int x = 0;
    STD_NS uintptr_t up = (STD_NS uintptr_t)(void*)&x;
    STD::string txt = ci_fmt("%" PRIxPTR, up);
    STD_NS uintptr_t back = ci_scan<STD_NS uintptr_t>(txt.c_str(), "%" SCNxPTR);
    test_true( (void*)back == (void*)&x );
    test_pass("cxx11:PRI* PTR");
}

#if defined(_MSC_VER) && _MSC_VER < 1900
#  define _TST_SCN8_OVERRUNS 1
#else
#  define _TST_SCN8_OVERRUNS 0
#endif

TEST_CASE(cinttypes, scn_exact_width) {
#if _TST_SCN8_OVERRUNS
    TEST_NOTE("pre-UCRT MSVC writes an int through the 1-byte object for %hh");
    TEST_SKIP_N(5);
#else
    CHK_SCN(STD_NS int8_t,  STD_NS uint8_t,  SCNd8,  SCNi8,  SCNo8,  SCNu8,  SCNx8);
#endif
    CHK_SCN(STD_NS int16_t, STD_NS uint16_t, SCNd16, SCNi16, SCNo16, SCNu16, SCNx16);
    CHK_SCN(STD_NS int32_t, STD_NS uint32_t, SCNd32, SCNi32, SCNo32, SCNu32, SCNx32);
    CHK_SCN(STD_NS int64_t, STD_NS uint64_t, SCNd64, SCNi64, SCNo64, SCNu64, SCNx64);

    STD_NS int32_t hex = ci_scan<STD_NS int32_t>("0x1f", "%" SCNi32);
    test_eq( (long long)hex, 31LL );
    STD_NS int32_t oct = ci_scan<STD_NS int32_t>("0755", "%" SCNi32);
    test_eq( (long long)oct, 493LL );
    STD_NS int32_t dec = ci_scan<STD_NS int32_t>("0755", "%" SCNd32);
    test_eq( (long long)dec, 755LL );

    STD_NS uint64_t u64 = ci_scan<STD_NS uint64_t>("18446744073709551615", "%" SCNu64);
    test_eq( (unsigned long long)u64, 18446744073709551615ULL );
    STD_NS int64_t i64 = ci_scan<STD_NS int64_t>("-9223372036854775808", "%" SCNd64);
    test_eq( (long long)i64, -9223372036854775807LL - 1 );
    test_pass("cxx11:SCN* exact-width");
}

TEST_CASE(cinttypes, scn_least_width) {
#if _TST_SCN8_OVERRUNS
    TEST_NOTE("pre-UCRT MSVC writes an int through the 1-byte object for %hh");
    TEST_SKIP_N(5);
#else
    CHK_SCN(STD_NS int_least8_t,  STD_NS uint_least8_t,
            SCNdLEAST8,  SCNiLEAST8,  SCNoLEAST8,  SCNuLEAST8,  SCNxLEAST8);
#endif
    CHK_SCN(STD_NS int_least16_t, STD_NS uint_least16_t,
            SCNdLEAST16, SCNiLEAST16, SCNoLEAST16, SCNuLEAST16, SCNxLEAST16);
    CHK_SCN(STD_NS int_least32_t, STD_NS uint_least32_t,
            SCNdLEAST32, SCNiLEAST32, SCNoLEAST32, SCNuLEAST32, SCNxLEAST32);
    CHK_SCN(STD_NS int_least64_t, STD_NS uint_least64_t,
            SCNdLEAST64, SCNiLEAST64, SCNoLEAST64, SCNuLEAST64, SCNxLEAST64);
    test_pass("cxx11:SCN* least-width");
}

#define CHK_SCN_KNOWN_BAD(ST, UT, Sd, Si, So, Su, Sx)               \
    do {                                                            \
        ST const sd = ci_scan<ST>("-42", "%" Sd);                   \
        TEST_SKIP1(); test_eq( (long long)sd, -42LL );               \
        ST const si = ci_scan<ST>("-42", "%" Si);                   \
        TEST_SKIP1(); test_eq( (long long)si, -42LL );               \
        UT const so = ci_scan<UT>("253", "%" So);                   \
        TEST_SKIP1(); test_eq( (unsigned long long)so, 171ULL );     \
        UT const su = ci_scan<UT>("171", "%" Su);                   \
        TEST_SKIP1(); test_eq( (unsigned long long)su, 171ULL );     \
        UT const sx = ci_scan<UT>("ab", "%" Sx);                    \
        TEST_SKIP1(); test_eq( (unsigned long long)sx, 171ULL );     \
    } while (0)

TEST_CASE(cinttypes, scn_fast_width) {
#if _TST_SCN8_OVERRUNS
    TEST_NOTE("pre-UCRT MSVC writes an int through the 1-byte object for %hh");
    TEST_SKIP_N(5);
#else
    CHK_SCN(STD_NS int_fast8_t,  STD_NS uint_fast8_t,
            SCNdFAST8,  SCNiFAST8,  SCNoFAST8,  SCNuFAST8,  SCNxFAST8);
#endif

    CHK_SCN_KNOWN_BAD(STD_NS int_fast16_t, STD_NS uint_fast16_t,
            SCNdFAST16, SCNiFAST16, SCNoFAST16, SCNuFAST16, SCNxFAST16);

    CHK_SCN(STD_NS int_fast32_t, STD_NS uint_fast32_t,
            SCNdFAST32, SCNiFAST32, SCNoFAST32, SCNuFAST32, SCNxFAST32);
    CHK_SCN(STD_NS int_fast64_t, STD_NS uint_fast64_t,
            SCNdFAST64, SCNiFAST64, SCNoFAST64, SCNuFAST64, SCNxFAST64);
    test_pass("cxx11:SCN* fast-width");
}

TEST_CASE(cinttypes, scn_max_and_ptr) {
    CHK_SCN(STD_NS intmax_t, STD_NS uintmax_t,
            SCNdMAX, SCNiMAX, SCNoMAX, SCNuMAX, SCNxMAX);

    STD_NS intmax_t im = INTMAX_MIN;
    STD::string imtxt = ci_fmt("%" PRIdMAX, im);
    STD_NS intmax_t imback = ci_scan<STD_NS intmax_t>(imtxt.c_str(), "%" SCNdMAX);
    test_eq( (long long)imback, (long long)INTMAX_MIN );

    STD_NS uintmax_t um = UINTMAX_MAX;
    STD::string umtxt = ci_fmt("%" PRIuMAX, um);
    STD_NS uintmax_t umback = ci_scan<STD_NS uintmax_t>(umtxt.c_str(), "%" SCNuMAX);
    test_eq( (unsigned long long)umback, (unsigned long long)UINTMAX_MAX );
    test_pass("cxx11:SCN* MAX");

    CHK_SCN(STD_NS intptr_t, STD_NS uintptr_t,
            SCNdPTR, SCNiPTR, SCNoPTR, SCNuPTR, SCNxPTR);
    test_pass("cxx11:SCN* PTR");
}
#else
TEST_CASE_SKIP(cinttypes, imaxdiv_t_and_imaxdiv)
TEST_CASE_SKIP(cinttypes, imaxabs)
TEST_CASE_SKIP(cinttypes, strtoimax_strtoumax)
TEST_CASE_SKIP(cinttypes, wcstoimax_wcstoumax)
TEST_CASE_SKIP(cinttypes, pri_exact_width)
TEST_CASE_SKIP(cinttypes, pri_least_width)
TEST_CASE_SKIP(cinttypes, pri_fast_width)
TEST_CASE_SKIP(cinttypes, pri_max_and_ptr)
TEST_CASE_SKIP(cinttypes, scn_exact_width)
TEST_CASE_SKIP(cinttypes, scn_least_width)
TEST_CASE_SKIP(cinttypes, scn_fast_width)
TEST_CASE_SKIP(cinttypes, scn_max_and_ptr)
#endif


#if TEST_TARGET_CXX >= 2011
namespace {
bool ccw_round_trip_max(const char* pri, const char* scn, STD_NS intmax_t v) {
    char buf[64];
    STD_NS intmax_t back = 0;
    if (STD::snprintf(buf, sizeof buf, pri, v) <= 0) return false;
    if (STD::sscanf(buf, scn, &back) != 1) return false;
    return back == v;
}
}

TEST_CASE(cinttypes, intmax_format_macros_cxx11) {
    char buf[64];
    STD_NS intmax_t v = 1234567;
    STD_NS uintmax_t u = 7654321u;

    test_true( ccw_round_trip_max("%" PRIdMAX, "%" SCNdMAX, v) );
    test_pass("cxx11:PRIdMAX");
    test_pass("cxx11:SCNdMAX");

    test_true( ccw_round_trip_max("%" PRIiMAX, "%" SCNiMAX, v) );
    test_pass("cxx11:PRIiMAX");
    test_pass("cxx11:SCNiMAX");

    STD::snprintf(buf, sizeof buf, "%" PRIuMAX, u);
    test_true( STD::strcmp(buf, "7654321") == 0 );
    {
        STD_NS uintmax_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNuMAX, &back), 1 );
        test_true( back == u );
    }
    test_pass("cxx11:PRIuMAX");
    test_pass("cxx11:SCNuMAX");

    STD::snprintf(buf, sizeof buf, "%" PRIoMAX, (STD_NS uintmax_t)8);
    test_true( STD::strcmp(buf, "10") == 0 );
    {
        STD_NS uintmax_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNoMAX, &back), 1 );
        test_true( back == (STD_NS uintmax_t)8 );
    }
    test_pass("cxx11:PRIoMAX");
    test_pass("cxx11:SCNoMAX");

    STD::snprintf(buf, sizeof buf, "%" PRIxMAX, (STD_NS uintmax_t)255);
    test_true( STD::strcmp(buf, "ff") == 0 );
    {
        STD_NS uintmax_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNxMAX, &back), 1 );
        test_true( back == (STD_NS uintmax_t)255 );
    }
    test_pass("cxx11:PRIxMAX");
    test_pass("cxx11:SCNxMAX");

    STD::snprintf(buf, sizeof buf, "%" PRIXMAX, (STD_NS uintmax_t)255);
    test_true( STD::strcmp(buf, "FF") == 0 );
    test_pass("cxx11:PRIXMAX");
}

TEST_CASE(cinttypes, intptr_format_macros_cxx11) {
#if defined(INTPTR_MAX)
    char buf[64];
    STD_NS intptr_t v = 4321;
    STD_NS uintptr_t u = 1234u;

    STD::snprintf(buf, sizeof buf, "%" PRIdPTR, v);
    test_true( STD::strcmp(buf, "4321") == 0 );
    {
        STD_NS intptr_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNdPTR, &back), 1 );
        test_true( back == v );
    }
    test_pass("cxx11:PRIdPTR");
    test_pass("cxx11:SCNdPTR");

    STD::snprintf(buf, sizeof buf, "%" PRIiPTR, v);
    test_true( STD::strcmp(buf, "4321") == 0 );
    {
        STD_NS intptr_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNiPTR, &back), 1 );
        test_true( back == v );
    }
    test_pass("cxx11:PRIiPTR");
    test_pass("cxx11:SCNiPTR");

    STD::snprintf(buf, sizeof buf, "%" PRIuPTR, u);
    test_true( STD::strcmp(buf, "1234") == 0 );
    {
        STD_NS uintptr_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNuPTR, &back), 1 );
        test_true( back == u );
    }
    test_pass("cxx11:PRIuPTR");
    test_pass("cxx11:SCNuPTR");

    STD::snprintf(buf, sizeof buf, "%" PRIoPTR, (STD_NS uintptr_t)8);
    test_true( STD::strcmp(buf, "10") == 0 );
    {
        STD_NS uintptr_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNoPTR, &back), 1 );
        test_true( back == (STD_NS uintptr_t)8 );
    }
    test_pass("cxx11:PRIoPTR");
    test_pass("cxx11:SCNoPTR");

    STD::snprintf(buf, sizeof buf, "%" PRIxPTR, (STD_NS uintptr_t)255);
    test_true( STD::strcmp(buf, "ff") == 0 );
    {
        STD_NS uintptr_t back = 0;
        test_eq( STD::sscanf(buf, "%" SCNxPTR, &back), 1 );
        test_true( back == (STD_NS uintptr_t)255 );
    }
    test_pass("cxx11:PRIxPTR");
    test_pass("cxx11:SCNxPTR");

    STD::snprintf(buf, sizeof buf, "%" PRIXPTR, (STD_NS uintptr_t)255);
    test_true( STD::strcmp(buf, "FF") == 0 );
    test_pass("cxx11:PRIXPTR");
#else
    TEST_NOTE("intptr_t is optional and absent here, so the PTR macros cannot be used");
    test_skip("cxx11:PRIdPTR");  test_skip("cxx11:SCNdPTR");
    test_skip("cxx11:PRIiPTR");  test_skip("cxx11:SCNiPTR");
    test_skip("cxx11:PRIuPTR");  test_skip("cxx11:SCNuPTR");
    test_skip("cxx11:PRIoPTR");  test_skip("cxx11:SCNoPTR");
    test_skip("cxx11:PRIxPTR");  test_skip("cxx11:SCNxPTR");
    test_skip("cxx11:PRIXPTR");
#endif
}

TEST_CASE(cinttypes, binary_format_macros_cxx26) {
#if defined(PRIbMAX)
    char buf[80];
    STD::snprintf(buf, sizeof buf, "%" PRIbMAX, (STD_NS uintmax_t)5);
    test_true( STD::strcmp(buf, "101") == 0 );
    test_pass("cxx26:PRIbMAX");
    STD::snprintf(buf, sizeof buf, "%" PRIBMAX, (STD_NS uintmax_t)5);
    test_true( STD::strcmp(buf, "101") == 0 );
    test_pass("cxx26:PRIBMAX");
    {
        STD_NS uintmax_t back = 0;
        test_eq( STD::sscanf("101", "%" SCNbMAX, &back), 1 );
        test_true( back == (STD_NS uintmax_t)5 );
    }
    test_pass("cxx26:SCNbMAX");
#else
    TEST_NOTE("the binary conversion macros are C23/C++26; not in this library");
    test_skip("cxx26:PRIbMAX");
    test_skip("cxx26:PRIBMAX");
    test_skip("cxx26:SCNbMAX");
#endif
#if defined(PRIbPTR)
    {
        char buf2[80];
        STD::snprintf(buf2, sizeof buf2, "%" PRIbPTR, (STD_NS uintptr_t)6);
        test_true( STD::strcmp(buf2, "110") == 0 );
        test_pass("cxx26:PRIbPTR");
        STD::snprintf(buf2, sizeof buf2, "%" PRIBPTR, (STD_NS uintptr_t)6);
        test_true( STD::strcmp(buf2, "110") == 0 );
        test_pass("cxx26:PRIBPTR");
        {
            STD_NS uintptr_t back = 0;
            test_eq( STD::sscanf("110", "%" SCNbPTR, &back), 1 );
            test_true( back == (STD_NS uintptr_t)6 );
        }
        test_pass("cxx26:SCNbPTR");
    }
#else
    test_skip("cxx26:PRIbPTR");
    test_skip("cxx26:PRIBPTR");
    test_skip("cxx26:SCNbPTR");
#endif
}

TEST_CASE(cinttypes, abs_div_overloads_cxx11) {
#if defined(__cpp_lib_cinttypes_abs_div) || (!defined(_MSC_VER) && !defined(__WATCOMC__))
    test_true( STD_NS abs((STD_NS intmax_t)-7) == (STD_NS intmax_t)7 );
    test_pass("cxx11:opt:abs");
    test_true( STD_NS div((STD_NS intmax_t)7, (STD_NS intmax_t)2).quot == 3 );
    test_true( STD_NS div((STD_NS intmax_t)7, (STD_NS intmax_t)2).rem  == 1 );
    test_pass("cxx11:opt:div");
#else
    TEST_NOTE("the intmax_t overloads of abs/div are optional and absent here");
    test_skip("cxx11:opt:abs");
    test_skip("cxx11:opt:div");
#endif
}
#else
TEST_CASE(cinttypes, intmax_format_macros_cxx11) {
    test_skip("cxx11:PRIdMAX"); test_skip("cxx11:SCNdMAX");
    test_skip("cxx11:PRIiMAX"); test_skip("cxx11:SCNiMAX");
    test_skip("cxx11:PRIuMAX"); test_skip("cxx11:SCNuMAX");
    test_skip("cxx11:PRIoMAX"); test_skip("cxx11:SCNoMAX");
    test_skip("cxx11:PRIxMAX"); test_skip("cxx11:SCNxMAX");
    test_skip("cxx11:PRIXMAX");
}
TEST_CASE(cinttypes, intptr_format_macros_cxx11) {
    test_skip("cxx11:PRIdPTR"); test_skip("cxx11:SCNdPTR");
    test_skip("cxx11:PRIiPTR"); test_skip("cxx11:SCNiPTR");
    test_skip("cxx11:PRIuPTR"); test_skip("cxx11:SCNuPTR");
    test_skip("cxx11:PRIoPTR"); test_skip("cxx11:SCNoPTR");
    test_skip("cxx11:PRIxPTR"); test_skip("cxx11:SCNxPTR");
    test_skip("cxx11:PRIXPTR");
}
TEST_CASE(cinttypes, binary_format_macros_cxx26) {
    test_skip("cxx26:PRIbMAX"); test_skip("cxx26:PRIBMAX"); test_skip("cxx26:SCNbMAX");
    test_skip("cxx26:PRIbPTR"); test_skip("cxx26:PRIBPTR"); test_skip("cxx26:SCNbPTR");
}
TEST_CASE(cinttypes, abs_div_overloads_cxx11) {
    test_skip("cxx11:opt:abs");
    test_skip("cxx11:opt:div");
}
#endif


TEST_CASE(cinttypes, imaxdiv_t_members_cxx11) {
    STD_NS imaxdiv_t d = STD_NS imaxdiv((STD_NS intmax_t)17, (STD_NS intmax_t)5);
    test_true( d.quot == (STD_NS intmax_t)3 );
    test_pass("cxx11:imaxdiv_t::quot");
    test_true( d.rem == (STD_NS intmax_t)2 );
    test_pass("cxx11:imaxdiv_t::rem");

    test_true( d.quot * (STD_NS intmax_t)5 + d.rem == (STD_NS intmax_t)17 );
    {
        STD_NS imaxdiv_t n = STD_NS imaxdiv((STD_NS intmax_t)-17, (STD_NS intmax_t)5);
        test_true( n.quot == (STD_NS intmax_t)-3 );
        test_true( n.rem == (STD_NS intmax_t)-2 );
    }
}
