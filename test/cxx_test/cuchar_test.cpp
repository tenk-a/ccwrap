#include "test_cxx.hpp"
#include <cuchar>

TEST_CASE(cuchar, header_compiles) { test_true( true );  test_pass("cxx11:<cuchar> header compiles"); }

#if defined(__CCW_HAS_CHAR8_T)
#include <cstring>

TEST_CASE(cuchar, mbrtoc8_c8rtomb) {
    STD_NS mbstate_t st;
    STD_NS size_t r;

    STD::memset(&st, 0, sizeof st);
    char8_t c8 = 0;
    r = ::mbrtoc8(&c8, "A", 1, &st);
    test_eq( r, STD_NS size_t(1) );
    test_eq( (int)c8, (int)'A' );

    STD::memset(&st, 0, sizeof st);
    r = ::mbrtoc8(&c8, "A", 0, &st);
    test_eq( r, STD_NS size_t(-2) );

    STD::memset(&st, 0, sizeof st);
    c8 = 0xEE;
    r = ::mbrtoc8(&c8, "\0", 1, &st);
    test_eq( r, STD_NS size_t(0) );
    test_eq( (int)c8, 0 );

    STD::memset(&st, 0, sizeof st);
    r = ::mbrtoc8(NULL, NULL, 0, &st);
    test_eq( r, STD_NS size_t(0) );

    {
        char two[3]; two[0] = (char)0xC3; two[1] = (char)0xA9; two[2] = 0;
        STD::memset(&st, 0, sizeof st);
        char8_t u0 = 0; STD_NS size_t k0 = ::mbrtoc8(&u0, two, 2, &st);
        test_eq( k0, STD_NS size_t(2) );
        test_eq( (int)u0, 0xC3 );
        char8_t u1 = 0; STD_NS size_t k1 = ::mbrtoc8(&u1, two + 2, 1, &st);
        test_eq( k1, STD_NS size_t(-3) );
        test_eq( (int)u1, 0xA9 );
    }

    {
        char bad[2]; bad[0] = (char)0x80; bad[1] = 0;
        STD::memset(&st, 0, sizeof st);
        STD_NS size_t e = ::mbrtoc8(&c8, bad, 1, &st);
        test_eq( e, STD_NS size_t(-1) );
    }
    test_pass("cxx20:mbrtoc8");

    STD::memset(&st, 0, sizeof st);
    char buf[8];
    STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w = ::c8rtomb(buf, (char8_t)'Z', &st);
    test_eq( w, STD_NS size_t(1) );
    test_eq( (int)buf[0], (int)'Z' );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t wn = ::c8rtomb(NULL, (char8_t)0, &st);
    test_eq( wn, STD_NS size_t(1) );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t we = ::c8rtomb(buf, (char8_t)0x80, &st);
    test_eq( we, STD_NS size_t(-1) );

    STD::memset(&st, 0, sizeof st);
    STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w0 = ::c8rtomb(buf, (char8_t)0xE2, &st);
    test_eq( w0, STD_NS size_t(0) );
    STD_NS size_t w1 = ::c8rtomb(buf, (char8_t)0x82, &st);
    test_eq( w1, STD_NS size_t(0) );
    STD_NS size_t w3 = ::c8rtomb(buf, (char8_t)0xAC, &st);
    test_eq( w3, STD_NS size_t(3) );
    test_eq( (int)(unsigned char)buf[0], 0xE2 );
    test_eq( (int)(unsigned char)buf[1], 0x82 );
    test_eq( (int)(unsigned char)buf[2], 0xAC );
    test_pass("cxx20:c8rtomb");

    for (int c = 1; c < 128; ++c) {
        char in[2]; in[0] = (char)c; in[1] = 0;
        STD::memset(&st, 0, sizeof st);
        char8_t u = 0;
        STD_NS size_t kf = ::mbrtoc8(&u, in, 1, &st);
        test_eq( kf, STD_NS size_t(1) );
        test_eq( (int)u, c );

        STD::memset(&st, 0, sizeof st);
        char back[2]; back[0] = 0; back[1] = 0;
        STD_NS size_t kb = ::c8rtomb(back, u, &st);
        test_eq( kb, STD_NS size_t(1) );
        test_eq( (int)back[0], c );
    }
    test_pass("cxx20:char8_t");
}

TEST_CASE(cuchar, mbrtoc32_c32rtomb_ccw) {
    STD_NS mbstate_t st;
    STD_NS size_t r;

    STD::memset(&st, 0, sizeof st);
    char32_t c32 = 0;
    r = ::mbrtoc32(&c32, "A", 1, &st);
    test_eq( r, STD_NS size_t(1) );
    test_eq( (long)c32, 65L );

    STD::memset(&st, 0, sizeof st);
    r = ::mbrtoc32(&c32, "A", 0, &st);
    test_eq( r, STD_NS size_t(-2) );

    STD::memset(&st, 0, sizeof st);
    c32 = 0xFFFF;
    r = ::mbrtoc32(&c32, "\0", 1, &st);
    test_eq( r, STD_NS size_t(0) );
    test_eq( (long)c32, 0L );

    { char b[4]; b[0]=(char)0xE2; b[1]=(char)0x82; b[2]=(char)0xAC; b[3]=0;
      STD::memset(&st, 0, sizeof st);
      STD_NS size_t k = ::mbrtoc32(&c32, b, 3, &st);
      test_eq( k, STD_NS size_t(3) );
      test_eq( (long)c32, 0x20ACL ); }

    { char b[5]; b[0]=(char)0xF0; b[1]=(char)0x9F; b[2]=(char)0x98; b[3]=(char)0x80; b[4]=0;
      STD::memset(&st, 0, sizeof st);
      STD_NS size_t k = ::mbrtoc32(&c32, b, 4, &st);
      test_eq( k, STD_NS size_t(4) );
      test_eq( (long)c32, 0x1F600L ); }

    { char b[2]; b[0]=(char)0x80; b[1]=0;
      STD::memset(&st, 0, sizeof st);
      STD_NS size_t e = ::mbrtoc32(&c32, b, 1, &st);
      test_eq( e, STD_NS size_t(-1) ); }
    test_pass("cxx11:mbrtoc32");

    char buf[8];
    STD::memset(&st, 0, sizeof st); STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w = ::c32rtomb(buf, (char32_t)'Z', &st);
    test_eq( w, STD_NS size_t(1) );
    test_eq( (int)buf[0], (int)'Z' );

    STD::memset(&st, 0, sizeof st); STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w3 = ::c32rtomb(buf, (char32_t)0x20AC, &st);
    test_eq( w3, STD_NS size_t(3) );
    test_eq( (int)(unsigned char)buf[0], 0xE2 );
    test_eq( (int)(unsigned char)buf[2], 0xAC );

    STD::memset(&st, 0, sizeof st); STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w4 = ::c32rtomb(buf, (char32_t)0x1F600, &st);
    test_eq( w4, STD_NS size_t(4) );
    test_eq( (int)(unsigned char)buf[0], 0xF0 );
    test_eq( (int)(unsigned char)buf[3], 0x80 );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t we = ::c32rtomb(buf, (char32_t)0xD800, &st);
    test_eq( we, STD_NS size_t(-1) );
    test_pass("cxx11:c32rtomb");
}

TEST_CASE(cuchar, mbrtoc16_c16rtomb_ccw) {
    STD_NS mbstate_t st;
    STD_NS size_t r;

    STD::memset(&st, 0, sizeof st);
    char16_t c16 = 0;
    r = ::mbrtoc16(&c16, "A", 1, &st);
    test_eq( r, STD_NS size_t(1) );
    test_eq( (long)c16, 65L );

    { char b[4]; b[0]=(char)0xE2; b[1]=(char)0x82; b[2]=(char)0xAC; b[3]=0;
      STD::memset(&st, 0, sizeof st);
      STD_NS size_t k = ::mbrtoc16(&c16, b, 3, &st);
      test_eq( k, STD_NS size_t(3) );
      test_eq( (long)c16, 0x20ACL ); }

    { char b[5]; b[0]=(char)0xF0; b[1]=(char)0x9F; b[2]=(char)0x98; b[3]=(char)0x80; b[4]=0;
      STD::memset(&st, 0, sizeof st);
      char16_t hi = 0; STD_NS size_t k0 = ::mbrtoc16(&hi, b, 4, &st);
      test_eq( k0, STD_NS size_t(4) );
      test_eq( (long)hi, 0xD83DL );
      char16_t lo = 0; STD_NS size_t k1 = ::mbrtoc16(&lo, b + 4, 1, &st);
      test_eq( k1, STD_NS size_t(-3) );
      test_eq( (long)lo, 0xDE00L ); }
    test_pass("cxx11:mbrtoc16");

    char buf[8];
    STD::memset(&st, 0, sizeof st); STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w = ::c16rtomb(buf, (char16_t)'Z', &st);
    test_eq( w, STD_NS size_t(1) );
    test_eq( (int)buf[0], (int)'Z' );

    STD::memset(&st, 0, sizeof st); STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w3 = ::c16rtomb(buf, (char16_t)0x20AC, &st);
    test_eq( w3, STD_NS size_t(3) );
    test_eq( (int)(unsigned char)buf[0], 0xE2 );

    STD::memset(&st, 0, sizeof st); STD::memset(buf, 0, sizeof buf);
    STD_NS size_t wh = ::c16rtomb(buf, (char16_t)0xD83D, &st);
    test_eq( wh, STD_NS size_t(0) );
    STD_NS size_t wl = ::c16rtomb(buf, (char16_t)0xDE00, &st);
    test_eq( wl, STD_NS size_t(4) );
    test_eq( (int)(unsigned char)buf[0], 0xF0 );
    test_eq( (int)(unsigned char)buf[3], 0x80 );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t we = ::c16rtomb(buf, (char16_t)0xDE00, &st);
    test_eq( we, STD_NS size_t(-1) );
    test_pass("cxx11:c16rtomb");
}
#else
TEST_CASE_SKIP(cuchar, mbrtoc8_c8rtomb)
TEST_CASE_SKIP(cuchar, mbrtoc32_c32rtomb_ccw)
TEST_CASE_SKIP(cuchar, mbrtoc16_c16rtomb_ccw)
#endif

#if TEST_TARGET_CXX >= 2011
#include <cuchar>
#include <climits>
#include <clocale>
#include <cstring>
#include <string>
#include <type_traits>

TEST_CASE(cuchar, types_and_macros) {

    STD_NS mbstate_t st;
    STD::memset(&st, 0, sizeof st);
    test_gt( sizeof(STD_NS mbstate_t), STD_NS size_t(0) );

    STD_NS size_t n = sizeof(STD_NS mbstate_t);
    test_gt( n, STD_NS size_t(0) );
    test_pass("cxx11:mbstate_t");

    test_true( STD::is_unsigned<STD_NS size_t>::value );
    test_pass("cxx11:size_t");

#ifdef __STDC_VERSION_UCHAR_H__
    test_ge( (long)__STDC_VERSION_UCHAR_H__, 202311L );
    test_pass("cxx26:__STDC_VERSION_UCHAR_H__");
#else
    TEST_SKIP1();
    TEST_NOTE("__STDC_VERSION_UCHAR_H__ not defined by this toolchain's <cuchar>");
    test_skip("cxx26:__STDC_VERSION_UCHAR_H__");
#endif

    test_ge( sizeof(char16_t), STD_NS size_t(2) );
    test_pass("cxx11:char16_t");

    test_ge( sizeof(char32_t), STD_NS size_t(4) );
    test_pass("cxx11:char32_t");
}

TEST_CASE(cuchar, mbrtoc16_c16rtomb) {
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    STD::setlocale(LC_ALL, "C");

    STD_NS mbstate_t st;
    STD::memset(&st, 0, sizeof st);
    char16_t c16 = 0;
    STD_NS size_t r = ::mbrtoc16(&c16, "A", 1, &st);
    test_eq( r, STD_NS size_t(1) );
    test_eq( (int)c16, (int)(char16_t)'A' );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t inc = ::mbrtoc16(&c16, "A", 0, &st);
    test_eq( inc, STD_NS size_t(-2) );

    STD::memset(&st, 0, sizeof st);
    c16 = 0xFFFF;
    STD_NS size_t z = ::mbrtoc16(&c16, "\0", 1, &st);
    test_eq( z, STD_NS size_t(0) );
    test_eq( (int)c16, 0 );

    STD::memset(&st, 0, sizeof st);
    STD_NS size_t nul = ::mbrtoc16(NULL, NULL, 0, &st);
    test_eq( nul, STD_NS size_t(0) );

    const char src[] = "cu16";
    char16_t out[8] = {0};
    STD::memset(&st, 0, sizeof st);
    const char* p = src;
    STD_NS size_t left = STD::strlen(src);
    int i = 0;
    while (left) {
        STD_NS size_t k = ::mbrtoc16(&out[i], p, left, &st);
        test_lim( (double)(long long)k, 1.0, (double)left );
        p += k; left -= k; ++i;
    }
    test_eq( i, 4 );
    test_eq( (int)out[0], (int)(char16_t)'c' );
    test_eq( (int)out[3], (int)(char16_t)'6' );
    test_pass("cxx11:mbrtoc16");

    STD::memset(&st, 0, sizeof st);
    char buf[MB_LEN_MAX + 1];
    STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w = ::c16rtomb(buf, (char16_t)'Z', &st);
    test_eq( w, STD_NS size_t(1) );
    test_eq( (int)buf[0], (int)'Z' );
    test_pass("cxx11:c16rtomb");

    STD::setlocale(LC_ALL, saved.c_str());
}

TEST_CASE(cuchar, mbrtoc32_c32rtomb) {
    STD::string saved(STD::setlocale(LC_ALL, NULL));
    STD::setlocale(LC_ALL, "C");

    STD_NS mbstate_t st;
    STD::memset(&st, 0, sizeof st);
    char32_t c32 = 0;
    STD_NS size_t r = ::mbrtoc32(&c32, "q", 1, &st);
    test_eq( r, STD_NS size_t(1) );
    test_eq( (int)c32, (int)(char32_t)'q' );

    STD::memset(&st, 0, sizeof st);
    test_eq( ::mbrtoc32(&c32, "q", 0, &st), STD_NS size_t(-2) );

    STD::memset(&st, 0, sizeof st);
    c32 = 0xFFFFFFFFu;
    test_eq( ::mbrtoc32(&c32, "\0", 1, &st), STD_NS size_t(0) );
    test_eq( (int)c32, 0 );
    test_pass("cxx11:mbrtoc32");

    STD::memset(&st, 0, sizeof st);
    char buf[MB_LEN_MAX + 1];
    STD::memset(buf, 0, sizeof buf);
    STD_NS size_t w = ::c32rtomb(buf, (char32_t)'q', &st);
    test_eq( w, STD_NS size_t(1) );
    test_eq( (int)buf[0], (int)'q' );
    test_pass("cxx11:c32rtomb");

    for (int c = 'a'; c <= 'z'; ++c) {
        char in[2]; in[0] = (char)c; in[1] = 0;
        STD::memset(&st, 0, sizeof st);
        char32_t u = 0;
        STD_NS size_t k = ::mbrtoc32(&u, in, 1, &st);
        test_eq( k, STD_NS size_t(1) );
        test_eq( (int)u, c );

        STD::memset(&st, 0, sizeof st);
        char back[MB_LEN_MAX + 1] = {0};
        STD_NS size_t m = ::c32rtomb(back, u, &st);
        test_eq( m, STD_NS size_t(1) );
        test_eq( (int)back[0], c );
    }
    test_pass("cxx11:round-trip property");

    STD::setlocale(LC_ALL, saved.c_str());
}
#else
TEST_CASE_SKIP(cuchar, types_and_macros)
TEST_CASE_SKIP(cuchar, mbrtoc16_c16rtomb)
TEST_CASE_SKIP(cuchar, mbrtoc32_c32rtomb)
#endif


TEST_CASE(cuchar, encoding_macros_cxx11) {
#if defined(__STDC_UTF_16__)
    test_eq( (long)__STDC_UTF_16__, 1L );
    test_pass("cxx11:__STDC_UTF_16__");
#else
    TEST_NOTE("__STDC_UTF_16__ is not defined: char16_t need not be UTF-16 here");
    test_skip("cxx11:__STDC_UTF_16__");
#endif
#if defined(__STDC_UTF_32__)
    test_eq( (long)__STDC_UTF_32__, 1L );
    test_pass("cxx11:__STDC_UTF_32__");
#else
    TEST_NOTE("__STDC_UTF_32__ is not defined: char32_t need not be UTF-32 here");
    test_skip("cxx11:__STDC_UTF_32__");
#endif

#if defined(__STDC_VERSION_UCHAR_H__)
    test_true( __STDC_VERSION_UCHAR_H__ >= 202311L );
    test_pass("cxx26:__STDC_VERSION_UCHAR_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("cxx26:__STDC_VERSION_UCHAR_H__");
#endif
}
