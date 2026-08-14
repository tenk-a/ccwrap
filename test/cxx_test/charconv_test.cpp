#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <charconv>
#include <cstring>

TEST_CASE(charconv, to_chars_int) {
    char buf[64];
    STD::to_chars_result r = STD::to_chars(buf, buf + sizeof(buf), 12345);
    *r.ptr = 0;
    test_eq( STD::strcmp(buf, "12345"), 0 );
    test_true( r.ec == STD::errc() );

    r = STD::to_chars(buf, buf + sizeof(buf), -42);
    *r.ptr = 0;
    test_eq( STD::strcmp(buf, "-42"), 0 );

    r = STD::to_chars(buf, buf + sizeof(buf), 255, 16);
    *r.ptr = 0;
    test_eq( STD::strcmp(buf, "ff"), 0 );
    r = STD::to_chars(buf, buf + sizeof(buf), 10, 2);
    *r.ptr = 0;
    test_eq( STD::strcmp(buf, "1010"), 0 );
    r = STD::to_chars(buf, buf + sizeof(buf), 35, 36);
    *r.ptr = 0;
    test_eq( STD::strcmp(buf, "z"), 0 );

    char tiny[2];
    STD::to_chars_result e = STD::to_chars(tiny, tiny + 1, 100000);
    test_true( e.ec != STD::errc() );
    test_pass("cxx17:to chars int");
}

TEST_CASE(charconv, from_chars_int) {
    const char* s = "12345rest";
    int v = 0;
    STD::from_chars_result r = STD::from_chars(s, s + STD::strlen(s), v);
    test_eq( v, 12345 );
    test_true( r.ptr == s + 5 );
    test_true( r.ec == STD::errc() );

    const char* h = "ff";
    int hv = 0;
    STD::from_chars(h, h + 2, hv, 16);
    test_eq( hv, 255 );

    const char* bad = "xyz";
    int bv = 7;
    STD::from_chars_result br = STD::from_chars(bad, bad + 3, bv);
    test_true( br.ec != STD::errc() );
    test_true( br.ptr == bad );
    test_pass("cxx17:from chars int");
}

TEST_CASE(charconv, roundtrip_and_float) {
    for (int i = -1000; i <= 1000; i += 137) {
        char buf[32];
        STD::to_chars_result w = STD::to_chars(buf, buf + sizeof(buf), i);
        int back = 0;
        STD::from_chars(buf, w.ptr, back);
        test_eq( back, i );
    }

    char fb[64];
    STD::to_chars_result fw = STD::to_chars(fb, fb + sizeof(fb), 3.5);
    *fw.ptr = 0;
    double d = 0;
    STD::from_chars(fb, fw.ptr, d);
    test_true( d == 3.5 );

    const char* fs = "2.25";
    double fv = 0;
    STD::from_chars(fs, fs + 4, fv);
    test_true( fv == 2.25 );

    char pb[64];
    STD::to_chars_result pw = STD::to_chars(pb, pb + sizeof(pb), 1.5, STD::chars_format::general);
    *pw.ptr = 0;
    double pd = 0;
    STD::from_chars(pb, pw.ptr, pd);
    test_true( pd == 1.5 );
    test_pass("cxx17:roundtrip and float");
}

#if TEST_TARGET_CXX >= 2026
TEST_CASE(charconv, result_operator_bool) {
    char buf[8];
    STD::to_chars_result ok = STD::to_chars(buf, buf + 8, 123);
    test_true( (bool)ok );
    test_true( ok.ec == STD::errc() );

    STD::to_chars_result full = STD::to_chars(buf, buf + 1, 123456);
    test_true( !(bool)full );

    int v = 0;
    STD::from_chars_result good = STD::from_chars(buf, buf + 3, v);
    (void)good;
    STD::from_chars_result bad;
    bad.ptr = buf; bad.ec = STD::errc::invalid_argument;
    test_true( !(bool)bad );
}
#else
TEST_CASE_SKIP(charconv, result_operator_bool)
#endif
#endif

#if TEST_TARGET_CXX >= 2017
#include <charconv>
#include <cstring>
#include <string>
#include <limits>
#include <system_error>
#include <type_traits>

namespace {

template <class _Want, class _Got>
bool same_type(const _Got&) { return STD::is_same<_Want, _Got>::value; }

template <class T>
STD::string tc(T v) {
    char buf[64];
    STD::to_chars_result r = STD::to_chars(buf, buf + sizeof buf, v);
    return (r.ec == STD::errc()) ? STD::string(buf, r.ptr) : STD::string("<err>");
}

template <class T>
STD::string tcb(T v, int base) {
    char buf[80];
    STD::to_chars_result r = STD::to_chars(buf, buf + sizeof buf, v, base);
    return (r.ec == STD::errc()) ? STD::string(buf, r.ptr) : STD::string("<err>");
}

template <class T>
T fc(const char* s) {
    T v = T();
    const char* last = s + STD::strlen(s);
    STD::from_chars_result r = STD::from_chars(s, last, v);
    return (r.ec == STD::errc() && r.ptr == last) ? v : T();
}

template <class T>
T fcb(const char* s, int base) {
    T v = T();
    const char* last = s + STD::strlen(s);
    STD::from_chars_result r = STD::from_chars(s, last, v, base);
    return (r.ec == STD::errc() && r.ptr == last) ? v : T();
}

template <class F>
STD::string tcf(F v) {
    char buf[64];
    STD::to_chars_result r = STD::to_chars(buf, buf + sizeof buf, v);
    return (r.ec == STD::errc()) ? STD::string(buf, r.ptr) : STD::string("<err>");
}

template <class F>
STD::string tcf(F v, STD::chars_format fmt) {
    char buf[64];
    STD::to_chars_result r = STD::to_chars(buf, buf + sizeof buf, v, fmt);
    return (r.ec == STD::errc()) ? STD::string(buf, r.ptr) : STD::string("<err>");
}

template <class F>
STD::string tcf(F v, STD::chars_format fmt, int prec) {
    char buf[128];
    STD::to_chars_result r = STD::to_chars(buf, buf + sizeof buf, v, fmt, prec);
    return (r.ec == STD::errc()) ? STD::string(buf, r.ptr) : STD::string("<err>");
}

template <class F>
F fcf(const char* s) {
    F v = F(-1);
    const char* last = s + STD::strlen(s);
    STD::from_chars_result r = STD::from_chars(s, last, v);
    return (r.ec == STD::errc() && r.ptr == last) ? v : F(-1);
}

template <class F>
F fcf(const char* s, STD::chars_format fmt) {
    F v = F(-1);
    const char* last = s + STD::strlen(s);
    STD::from_chars_result r = STD::from_chars(s, last, v, fmt);
    return (r.ec == STD::errc() && r.ptr == last) ? v : F(-1);
}

}

TEST_CASE(charconv, chars_format_enum) {
    typedef STD::chars_format cf;

    test_true( cf::general == (cf::fixed | cf::scientific) );
    test_true( cf::scientific != cf::fixed );
    test_true( cf::hex != cf::general );
    test_true( cf::hex != cf::fixed );
    test_true( cf::hex != cf::scientific );

    test_true( (cf::general & cf::fixed) == cf::fixed );
    test_true( (cf::fixed & cf::scientific) == cf() );
    test_true( (cf::fixed ^ cf::fixed) == cf() );
    test_true( (cf::general & ~cf::fixed) == cf::scientific );

    cf f = cf::fixed;
    f |= cf::scientific;
    test_true( f == cf::general );
    f &= cf::fixed;
    test_true( f == cf::fixed );
    f ^= cf::fixed;
    test_true( f == cf() );
    test_pass("cxx17:chars format enum");
}

TEST_CASE(charconv, result_structs) {

    char buf[8];
    STD::to_chars_result tr = STD::to_chars(buf, buf + sizeof buf, 4711);
    test_true( tr.ec == STD::errc() );
    test_eq( (int)(tr.ptr - buf), 4 );
    test_eq( STD::string(buf, tr.ptr), "4711" );
    test_true( same_type<char*>(tr.ptr) );
    test_true( same_type<STD::errc>(tr.ec) );

    const char src[] = "123abc";
    int v = 0;
    STD::from_chars_result fr = STD::from_chars(src, src + 6, v);
    test_true( fr.ec == STD::errc() );
    test_eq( v, 123 );
    test_eq( (int)(fr.ptr - src), 3 );
    test_true( same_type<const char*>(fr.ptr) );
    test_true( same_type<STD::errc>(fr.ec) );

    STD::to_chars_result tr2 = STD::to_chars(buf, buf + sizeof buf, 4711);
#if TEST_TARGET_CXX >= 2020
    test_true( tr == tr2 );
#else
    (void)tr2;
    TEST_SKIP1();
#endif

#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 202306L

    test_true( bool(tr) );
    test_true( bool(fr) );
    char tiny[1];
    STD::to_chars_result bad = STD::to_chars(tiny, tiny + 1, 12345);
    test_true( !bad );
    int dummy = 0;
    STD::from_chars_result badf = STD::from_chars(src + 3, src + 6, dummy);
    test_true( !badf );
#endif
    test_pass("cxx17:result structs");
}

TEST_CASE(charconv, to_chars_integral_default_base) {

    test_eq( tc(char(65)),                        "65" );
    test_eq( tc((signed char)(-12)),              "-12" );
    test_eq( tc((unsigned char)(200)),            "200" );
    test_eq( tc((short)(-32768)),                 "-32768" );
    test_eq( tc((unsigned short)(65535)),         "65535" );
    test_eq( tc(0),                               "0" );
    test_eq( tc(-2147483647 - 1),                 "-2147483648" );
    test_eq( tc(4294967295u),                     "4294967295" );
    test_eq( tc(-1234567L),                       "-1234567" );
    test_eq( tc(4000000000UL),                    "4000000000" );
    test_eq( tc(-9223372036854775807LL - 1),      "-9223372036854775808" );
    test_eq( tc(18446744073709551615ULL),         "18446744073709551615" );
    test_pass("cxx17:to chars integral default base");
}

TEST_CASE(charconv, to_chars_integral_with_base) {

    test_eq( tcb(char(65), 16),                   "41" );
    test_eq( tcb((signed char)(-15), 16),         "-f" );
    test_eq( tcb((unsigned char)(255), 2),        "11111111" );
    test_eq( tcb((short)(-8), 8),                 "-10" );
    test_eq( tcb((unsigned short)(4096), 16),     "1000" );
    test_eq( tcb(255, 16),                        "ff" );
    test_eq( tcb(255u, 8),                        "377" );
    test_eq( tcb(-10L, 2),                        "-1010" );
    test_eq( tcb(35UL, 36),                       "z" );
    test_eq( tcb(-255LL, 16),                     "-ff" );
    test_eq( tcb(18446744073709551615ULL, 16),    "ffffffffffffffff" );

    test_eq( tcb(1, 2),                           "1" );
    test_eq( tcb(0, 36),                          "0" );
    test_eq( tcb(1295, 36),                       "zz" );
    test_eq( tcb(123, 10),                        "123" );

    test_eq( tcb(0xABCDEF, 16),                   "abcdef" );
    test_pass("cxx17:to chars integral with base");
}

TEST_CASE(charconv, from_chars_integral_default_base) {

    test_eq( (int)fc<char>("65"),                 65 );
    test_eq( (int)fc<signed char>("-12"),         -12 );
    test_eq( (int)fc<unsigned char>("200"),       200 );
    test_eq( (int)fc<short>("-32768"),            -32768 );
    test_eq( (int)fc<unsigned short>("65535"),    65535 );
    test_eq( fc<int>("-2147483648"),              -2147483647 - 1 );
    test_eq( fc<unsigned int>("4294967295"),      4294967295u );
    test_eq( fc<long>("-1234567"),                -1234567L );
    test_eq( fc<unsigned long>("4000000000"),     4000000000UL );
    test_eq( fc<long long>("-9223372036854775808"), -9223372036854775807LL - 1 );
    test_eq( fc<unsigned long long>("18446744073709551615"), 18446744073709551615ULL );

    int v = 7;
    const char* p = "+5";
    STD::from_chars_result r = STD::from_chars(p, p + 2, v);
    test_true( r.ec == STD::errc::invalid_argument );
    test_eq( v, 7 );
    test_pass("cxx17:from chars integral default base");
}

TEST_CASE(charconv, from_chars_integral_with_base) {

    test_eq( (int)fcb<char>("41", 16),                65 );
    test_eq( (int)fcb<signed char>("-f", 16),         -15 );
    test_eq( (int)fcb<unsigned char>("11111111", 2),  255 );
    test_eq( (int)fcb<short>("-10", 8),               -8 );
    test_eq( (int)fcb<unsigned short>("1000", 16),    4096 );
    test_eq( fcb<int>("ff", 16),                      255 );
    test_eq( fcb<unsigned int>("377", 8),             255u );
    test_eq( fcb<long>("-1010", 2),                   -10L );
    test_eq( fcb<unsigned long>("z", 36),             35UL );
    test_eq( fcb<long long>("-ff", 16),               -255LL );
    test_eq( fcb<unsigned long long>("ffffffffffffffff", 16), 18446744073709551615ULL );

    test_eq( fcb<int>("ABCDEF", 16),                  0xABCDEF );
    test_eq( fcb<int>("zz", 36),                      1295 );

    const char* s = "0x1f";
    int v = -1;
    STD::from_chars_result r = STD::from_chars(s, s + 4, v, 16);
    test_true( r.ec == STD::errc() );
    test_eq( v, 0 );
    test_eq( (int)(r.ptr - s), 1 );
    test_pass("cxx17:from chars integral with base");
}

TEST_CASE(charconv, to_chars_floating_plain) {

    test_eq( tcf(1.5f),        "1.5" );
    test_eq( tcf(1.5),         "1.5" );
    test_eq( tcf(1.5L),        "1.5" );

    test_eq( tcf(0.0),         "0" );
    TEST_SKIP_WAT();
    test_eq( tcf(-0.0),        "-0" );
    test_eq( tcf(-2.25),       "-2.25" );
    test_eq( tcf(0.5f),        "0.5" );
    test_eq( tcf(100.0L),      "100" );

    char buf[64];
    double d = 0.1;
    STD::to_chars_result r = STD::to_chars(buf, buf + sizeof buf, d);
    test_true( r.ec == STD::errc() );
    double back = 0.0;
    STD::from_chars(buf, r.ptr, back);
    test_eq( back, d );
    test_pass("cxx17:to chars floating plain");
}

TEST_CASE(charconv, to_chars_floating_format) {
    typedef STD::chars_format cf;

    test_eq( tcf(1.5f, cf::fixed),        "1.5" );
    test_eq( tcf(1.5,  cf::fixed),        "1.5" );
    test_eq( tcf(1.5L, cf::fixed),        "1.5" );

    test_eq( tcf(1.5f, cf::scientific),   "1.5e+00" );
    test_eq( tcf(1.5,  cf::scientific),   "1.5e+00" );
    test_eq( tcf(1.5L, cf::scientific),   "1.5e+00" );

    test_eq( tcf(1.5f, cf::general),      "1.5" );
    test_eq( tcf(1.5,  cf::general),      "1.5" );
    test_eq( tcf(1.5L, cf::general),      "1.5" );

    test_eq( tcf(1500.0, cf::fixed),      "1500" );
    test_eq( tcf(1500.0, cf::scientific), "1.5e+03" );

    test_eq( tcf(0.5,  cf::hex),          "1p-1" );
    test_eq( tcf(1.0,  cf::hex),          "1p+0" );

    double back = 0.0;
    STD::string h = tcf(0.1, cf::hex);
    STD::from_chars(h.data(), h.data() + h.size(), back, cf::hex);
    test_eq( back, 0.1 );
    test_pass("cxx17:to chars floating format");
}

TEST_CASE(charconv, to_chars_floating_precision) {
#if defined(_MSC_VER) && !defined(__cpp_lib_to_chars)
    TEST_NOTE("MSVC declares the precision-taking floating to_chars deleted until it implements it");
    TEST_SKIP_N(5);
    test_skip("cxx17:to chars floating precision");
#else
    typedef STD::chars_format cf;

    test_eq( tcf(1.5f, cf::fixed, 3),      "1.500" );
    test_eq( tcf(1.5,  cf::fixed, 3),      "1.500" );
    test_eq( tcf(1.5L, cf::fixed, 3),      "1.500" );

    test_eq( tcf(1.5,  cf::fixed, 0),      "2" );
    TEST_SKIP_WAT();
    TEST_SKIP_VC141("MSVC's printf rounds a tie away from zero through VS2017");
    test_eq( tcf(1.25, cf::fixed, 1),      "1.2" );

    test_eq( tcf(1.5,  cf::scientific, 2), "1.50e+00" );
    test_eq( tcf(1.5f, cf::scientific, 0), "2e+00" );

    test_eq( tcf(1.0 / 3.0, cf::general, 5), "0.33333" );
    test_eq( tcf(1.5,       cf::general, 8), "1.5" );
    test_eq( tcf(1234567.0, cf::general, 3), "1.23e+06" );

    test_eq( tcf(1.0, cf::hex, 2),           "1.00p+0" );
    test_pass("cxx17:to chars floating precision");
#endif
}

TEST_CASE(charconv, from_chars_floating) {

    test_eq( fcf<float>("1.5"),        1.5f );
    test_eq( fcf<double>("1.5"),       1.5 );
#if _TST_HAS_FROM_CHARS_LONG_DOUBLE
    test_eq( fcf<long double>("1.5"),  1.5L );
#else
    TEST_SKIP1();
#endif

    test_eq( fcf<double>("-2.25"),     -2.25 );
    test_eq( fcf<double>("1.5e2"),     150.0 );
    test_eq( fcf<double>("1.5E-2"),    0.015 );
    test_eq( fcf<double>("0"),         0.0 );
    test_eq( fcf<float>("3.5e1"),      35.0f );
#if _TST_HAS_FROM_CHARS_LONG_DOUBLE
    test_eq( fcf<long double>("100"),  100.0L );
#else
    TEST_SKIP1();
#endif

    const char* p = "+1.5";
    double d = 9.0;
    STD::from_chars_result r = STD::from_chars(p, p + 4, d);
    test_true( r.ec == STD::errc::invalid_argument );
    test_eq( d, 9.0 );
    test_pass("cxx17:from chars floating");
}

TEST_CASE(charconv, from_chars_floating_format) {
    typedef STD::chars_format cf;

    test_eq( fcf<float>("1.5", cf::fixed),           1.5f );
    test_eq( fcf<double>("1.5", cf::fixed),          1.5 );
#if _TST_HAS_FROM_CHARS_LONG_DOUBLE
    test_eq( fcf<long double>("1.5", cf::fixed),     1.5L );
#else
    TEST_SKIP1();
#endif

    test_eq( fcf<double>("1.5e2", cf::scientific),   150.0 );
    test_eq( fcf<double>("1.5e2", cf::general),      150.0 );
    test_eq( fcf<double>("1.5", cf::general),        1.5 );

    test_eq( fcf<double>("1p-1", cf::hex),           0.5 );
    test_eq( fcf<double>("1.8p+1", cf::hex),         3.0 );
    test_eq( fcf<float>("1p+2", cf::hex),            4.0f );

    const char* s = "1.5e2";
    double d = 0.0;
    STD::from_chars_result r = STD::from_chars(s, s + 5, d, cf::fixed);
    test_true( r.ec == STD::errc() );
    test_eq( d, 1.5 );
    test_eq( (int)(r.ptr - s), 3 );

    double d2 = 8.0;
    STD::from_chars_result r2 = STD::from_chars(s, s + 3, d2, cf::scientific);
    test_true( r2.ec == STD::errc::invalid_argument );
    test_true( r2.ptr == s );
    test_eq( d2, 8.0 );
    test_pass("cxx17:from chars floating format");
}

TEST_CASE(charconv, errors) {

    char tiny[2];
    STD::to_chars_result r = STD::to_chars(tiny, tiny + 2, 12345);
    test_true( r.ec == STD::errc::value_too_large );
    test_true( r.ptr == tiny + 2 );

    STD::to_chars_result rf = STD::to_chars(tiny, tiny + 2, 1.25);
    test_true( rf.ec == STD::errc::value_too_large );
    test_true( rf.ptr == tiny + 2 );

    STD::to_chars_result re = STD::to_chars(tiny, tiny, 1);
    test_true( re.ec == STD::errc::value_too_large );

    const char junk[] = "zzz";
    int v = 42;
    STD::from_chars_result fr = STD::from_chars(junk, junk + 3, v);
    test_true( fr.ec == STD::errc::invalid_argument );
    test_true( fr.ptr == junk );
    test_eq( v, 42 );

    const char two[] = "2";
    int b2 = 7;
    STD::from_chars_result fr2 = STD::from_chars(two, two + 1, b2, 2);
    test_true( fr2.ec == STD::errc::invalid_argument );
    test_eq( b2, 7 );

    STD::from_chars_result fr3 = STD::from_chars(junk, junk, v);
    test_true( fr3.ec == STD::errc::invalid_argument );

    const char big[] = "99999999999999999999";
    int ov = 3;
    STD::from_chars_result fr4 = STD::from_chars(big, big + 20, ov);
    test_true( fr4.ec == STD::errc::result_out_of_range );
    test_true( fr4.ptr == big + 20 );
    test_eq( ov, 3 );

    const char neg[] = "-1";
    unsigned uv = 5;
    STD::from_chars_result fr5 = STD::from_chars(neg, neg + 2, uv);
    test_true( fr5.ec == STD::errc::invalid_argument );
    test_eq( uv, 5u );
    test_pass("cxx17:errors");
}

TEST_CASE(charconv, round_trip) {

    const long long ints[] = { 0, 1, -1, 12345, -12345, 2147483647LL, -2147483648LL,
                               9223372036854775807LL };
    const int bases[] = { 2, 8, 10, 16, 36 };
    for (int i = 0; i < 8; ++i) {
        for (int b = 0; b < 5; ++b) {
            char buf[80];
            STD::to_chars_result tr = STD::to_chars(buf, buf + sizeof buf, ints[i], bases[b]);
            test_true( tr.ec == STD::errc() );
            long long back = 0;
            STD::from_chars_result fr = STD::from_chars(buf, tr.ptr, back, bases[b]);
            test_true( fr.ec == STD::errc() && fr.ptr == tr.ptr );
            test_eq( back, ints[i] );
        }
    }

    const STD::chars_format fmts[] = { STD::chars_format::general,
                                       STD::chars_format::fixed,
                                       STD::chars_format::scientific,
                                       STD::chars_format::hex };
    const double ds[] = { 0.0, 1.0, -1.0, 0.1, 1e10, -2.5e-7, 3.14159265358979 };
    for (int i = 0; i < 7; ++i) {
        for (int f = 0; f < 4; ++f) {
            char buf[128];
            STD::to_chars_result tr = STD::to_chars(buf, buf + sizeof buf, ds[i], fmts[f]);
            test_true( tr.ec == STD::errc() );
            double back = -12345.0;
            STD::from_chars_result fr = STD::from_chars(buf, tr.ptr, back, fmts[f]);
            test_true( fr.ec == STD::errc() && fr.ptr == tr.ptr );
            test_eq( back, ds[i] );
        }
    }

    char fbuf[64];
    float f = 3.14159274f;
    STD::to_chars_result tr = STD::to_chars(fbuf, fbuf + sizeof fbuf, f);
    float fback = 0.0f;
    STD::from_chars(fbuf, tr.ptr, fback);
    test_eq( fback, f );
    test_pass("cxx17:round trip");
}

TEST_CASE(charconv, result_equality) {
#if !defined(__WATCOMC__) && TEST_TARGET_CXX >= 2020
    char buf[16];
    STD::to_chars_result a = STD::to_chars(buf, buf + sizeof buf, 42);
    STD::to_chars_result b = a;
    test_true( a == b );
    STD::to_chars_result c = STD::to_chars(buf, buf + sizeof buf, 4);
    test_true( !(a == c) );
    test_pass("cxx20:to_chars_result::operator==");

    const char* s = "123";
    int v = 0;
    STD::from_chars_result fa = STD::from_chars(s, s + 3, v);
    STD::from_chars_result fb = fa;
    test_true( fa == fb );
    test_pass("cxx20:from_chars_result::operator==");
#else
    test_skip("cxx20:to_chars_result::operator==");
    test_skip("cxx20:from_chars_result::operator==");
#endif
}
#else
TEST_CASE_SKIP(charconv, chars_format_enum)
TEST_CASE_SKIP(charconv, result_equality)
TEST_CASE_SKIP(charconv, result_structs)
TEST_CASE_SKIP(charconv, to_chars_integral_default_base)
TEST_CASE_SKIP(charconv, to_chars_integral_with_base)
TEST_CASE_SKIP(charconv, from_chars_integral_default_base)
TEST_CASE_SKIP(charconv, from_chars_integral_with_base)
TEST_CASE_SKIP(charconv, to_chars_floating_plain)
TEST_CASE_SKIP(charconv, to_chars_floating_format)
TEST_CASE_SKIP(charconv, to_chars_floating_precision)
TEST_CASE_SKIP(charconv, from_chars_floating)
TEST_CASE_SKIP(charconv, from_chars_floating_format)
TEST_CASE_SKIP(charconv, errors)
TEST_CASE_SKIP(charconv, round_trip)
#endif


#if TEST_TARGET_CXX >= 2017
TEST_CASE(charconv, synopsis_names_cxx17) {
    STD::to_chars_result (*tc)(char*, char*, int, int) = &STD::to_chars;
    test_ptr( (void*)tc );
    char buf[8];
    STD::to_chars_result r = tc(buf, buf + sizeof buf, 255, 16);
    test_true( r.ec == STD::errc() );
    test_true( STD::strncmp(buf, "ff", 2) == 0 );
    test_pass("cxx17:to chars integral with base");

    STD::from_chars_result (*fc)(const char*, const char*, int&, int) = &STD::from_chars;
    test_ptr( (void*)fc );
    int got = 0;
    const char* s = "ff";
    STD::from_chars_result q = fc(s, s + 2, got, 16);
    test_true( q.ec == STD::errc() );
    test_eq( got, 255 );
    test_pass("cxx17:from chars integral with base");

    STD::chars_format f = STD::chars_format::scientific;
    test_true( f != STD::chars_format::fixed );
    test_true( (STD::chars_format::fixed | STD::chars_format::scientific) ==
               STD::chars_format::general );
    test_pass("cxx17:chars format enum");
}
#else
TEST_CASE(charconv, synopsis_names_cxx17) {
    test_skip("cxx17:to chars integral with base");
    test_skip("cxx17:from chars integral with base");
    test_skip("cxx17:chars format enum");
}
#endif


#if TEST_TARGET_CXX >= 2017
TEST_CASE(charconv, result_structs_named_cxx17) {
    char buf[16];

    STD::to_chars_result tr = STD::to_chars(buf, buf + sizeof buf, 12345);
    test_true( tr.ec == STD::errc() );
    test_eq( (int)(tr.ptr - buf), 5 );
    {
        char tiny[2];
        STD::to_chars_result over = STD::to_chars(tiny, tiny + 2, 12345);
        test_true( over.ec == STD::errc::value_too_large );
        test_true( over.ptr == tiny + 2 );
    }
    test_pass("cxx17:to_chars_result");

    const char* text = "678xyz";
    int v = 0;
    STD::from_chars_result fr = STD::from_chars(text, text + 6, v);
    test_true( fr.ec == STD::errc() );
    test_eq( v, 678 );
    test_true( fr.ptr == text + 3 );
    {
        const char* bad = "zz";
        int w = -1;
        STD::from_chars_result nf = STD::from_chars(bad, bad + 2, w);
        test_true( nf.ec == STD::errc::invalid_argument );
        test_true( nf.ptr == bad );
        test_eq( w, -1 );
    }
    test_pass("cxx17:from_chars_result");
}

TEST_CASE(charconv, constexpr_and_operator_bool) {
#if defined(__cpp_lib_constexpr_charconv)
    test_true( __cpp_lib_constexpr_charconv >= 202207L );
    test_pass("cxx23:__cpp_lib_constexpr_charconv");
    {
        char b[8] = {0};
        STD::to_chars_result r = STD::to_chars(b, b + 8, 42);
        test_true( r.ec == STD::errc() && b[0] == '4' && b[1] == '2' );
    }
    test_pass("cxx23:constexpr integral to_chars");
    {
        const char* s = "42";
        int v = 0;
        STD::from_chars_result r = STD::from_chars(s, s + 2, v);
        test_true( r.ec == STD::errc() && v == 42 );
    }
    test_pass("cxx23:constexpr integral from_chars");
#else
    TEST_NOTE("constexpr charconv is C++23; this library does not announce it");
    test_skip("cxx23:__cpp_lib_constexpr_charconv");
    test_skip("cxx23:constexpr integral to_chars");
    test_skip("cxx23:constexpr integral from_chars");
#endif

#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 202306L
    {
        char b[8];
        STD::to_chars_result r = STD::to_chars(b, b + 8, 7);
        test_true( (bool)r );
        char tiny[1];
        test_true( !(bool)STD::to_chars(tiny, tiny + 1, 12345) );
    }
    test_pass("cxx26:to_chars_result::operator bool");
    {
        const char* s = "7";
        int v = 0;
        test_true( (bool)STD::from_chars(s, s + 1, v) );
        const char* bad = "z";
        test_true( !(bool)STD::from_chars(bad, bad + 1, v) );
    }
    test_pass("cxx26:from_chars_result::operator bool");
#else
    TEST_NOTE("the result structs' operator bool is C++26 (P2497); not in this library");
    test_skip("cxx26:to_chars_result::operator bool");
    test_skip("cxx26:from_chars_result::operator bool");
#endif
}
#else
TEST_CASE(charconv, result_structs_named_cxx17) {
    test_skip("cxx17:to_chars_result");
    test_skip("cxx17:from_chars_result");
}
TEST_CASE(charconv, constexpr_and_operator_bool) {
    test_skip("cxx23:__cpp_lib_constexpr_charconv");
    test_skip("cxx23:constexpr integral to_chars");
    test_skip("cxx23:constexpr integral from_chars");
    test_skip("cxx26:to_chars_result::operator bool");
    test_skip("cxx26:from_chars_result::operator bool");
}
#endif
