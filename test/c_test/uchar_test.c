#include "c_test.h"

#include <uchar.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

static int ccw_uchar_utf8_locale(void) {
    static const char* names[] = { ".UTF-8", ".utf8", "C.UTF-8", "en_US.UTF-8", ".65001", 0 };
    int i;
    for (i = 0; names[i] != 0; ++i)
        if (setlocale(LC_CTYPE, names[i]) != 0) return 1;
    return 0;
}

TEST_CASE(uchar, types_and_macros) {
    mbstate_t st;
    size_t z = sizeof(char32_t);
    test_gt( z, (size_t)0 );
    test_pass("c2011:size_t");
    memset(&st, 0, sizeof st);
    test_gt( sizeof(mbstate_t), (size_t)0 );
    test_pass("c2011:mbstate_t");

    test_ge( sizeof(char16_t), (size_t)2 );
    test_pass("c2011:char16_t");
    test_ge( sizeof(char32_t), (size_t)4 );
    test_pass("c2011:char32_t");

#ifdef __STDC_VERSION_UCHAR_H__
    test_ge( (long)__STDC_VERSION_UCHAR_H__, 202311L );
    test_pass("c2023:__STDC_VERSION_UCHAR_H__");
#else
    TEST_SKIP1(); TEST_NOTE("__STDC_VERSION_UCHAR_H__ not defined");
    test_skip("c2023:__STDC_VERSION_UCHAR_H__");
#endif
}

TEST_CASE(uchar, mbrtoc8_c8rtomb) {
#if defined(__CCW_HAS_CHAR8_T)
    mbstate_t st;
    char8_t c8;
    size_t r, w, wn, we, w0, w1, w3;
    char buf[8];
    int c;

    ccw_uchar_utf8_locale();
    memset(&st, 0, sizeof st);
    c8 = 0;
    r = mbrtoc8(&c8, "A", 1, &st);
    test_eq( r, (size_t)1 );
    test_eq( (int)c8, (int)'A' );

    memset(&st, 0, sizeof st);
    r = mbrtoc8(&c8, "A", 0, &st);
    test_eq( r, (size_t)-2 );

    memset(&st, 0, sizeof st);
    c8 = 0xEE;
    r = mbrtoc8(&c8, "\0", 1, &st);
    test_eq( r, (size_t)0 );
    test_eq( (int)c8, 0 );

    {
        char two[3];
        char8_t u0, u1;
        size_t k0, k1;
        two[0] = (char)0xC3; two[1] = (char)0xA9; two[2] = 0;
        memset(&st, 0, sizeof st);
        u0 = 0; k0 = mbrtoc8(&u0, two, 2, &st);
        test_eq( k0, (size_t)2 );
        test_eq( (int)u0, 0xC3 );
        u1 = 0; k1 = mbrtoc8(&u1, two + 2, 1, &st);
        test_eq( k1, (size_t)-3 );
        test_eq( (int)u1, 0xA9 );
    }

    {
        char bad[2];
        size_t e;
        bad[0] = (char)0x80; bad[1] = 0;
        memset(&st, 0, sizeof st);
        e = mbrtoc8(&c8, bad, 1, &st);
        test_eq( e, (size_t)-1 );
    }
    test_pass("c2023:mbrtoc8");

    memset(&st, 0, sizeof st);
    memset(buf, 0, sizeof buf);
    w = c8rtomb(buf, (char8_t)'Z', &st);
    test_eq( w, (size_t)1 );
    test_eq( (int)buf[0], (int)'Z' );

    memset(&st, 0, sizeof st);
    wn = c8rtomb(NULL, (char8_t)0, &st);
    test_eq( wn, (size_t)1 );

    memset(&st, 0, sizeof st);
    we = c8rtomb(buf, (char8_t)0x80, &st);
    test_eq( we, (size_t)-1 );

    memset(&st, 0, sizeof st);
    memset(buf, 0, sizeof buf);
    w0 = c8rtomb(buf, (char8_t)0xE2, &st);  test_eq( w0, (size_t)0 );
    w1 = c8rtomb(buf, (char8_t)0x82, &st);  test_eq( w1, (size_t)0 );
    w3 = c8rtomb(buf, (char8_t)0xAC, &st);  test_eq( w3, (size_t)3 );
    test_eq( (int)(unsigned char)buf[0], 0xE2 );
    test_eq( (int)(unsigned char)buf[2], 0xAC );

    for (c = 1; c < 128; ++c) {
        char in[2];
        char8_t u;
        char back[2];
        size_t kf, kb;
        in[0] = (char)c; in[1] = 0;
        memset(&st, 0, sizeof st);
        u = 0;
        kf = mbrtoc8(&u, in, 1, &st);
        test_eq( kf, (size_t)1 );
        test_eq( (int)u, c );
        memset(&st, 0, sizeof st);
        back[0] = 0; back[1] = 0;
        kb = c8rtomb(back, u, &st);
        test_eq( kb, (size_t)1 );
        test_eq( (int)back[0], c );
    }
    test_pass("c2023:c8rtomb");
    {
        char8_t u = 'u';
        test_eq(u, 'u');
    }
    test_pass("c2023:char8_t");
#else
    TEST_SKIP_CASE();
#endif
}

TEST_CASE(uchar, mbrtoc32_c32rtomb) {
    mbstate_t st;
    char32_t c32;
    size_t r, w, w3, w4, we;
    char buf[8];
    int utf8;

    ccw_uchar_utf8_locale();
    {
        char probe[3];
        probe[0]=(char)0xE2; probe[1]=(char)0x82; probe[2]=(char)0xAC;
        memset(&st, 0, sizeof st);
        utf8 = (mbrtoc32(&c32, probe, 3, &st) == (size_t)3);
    }

    memset(&st, 0, sizeof st);
    c32 = 0;
    r = mbrtoc32(&c32, "A", 1, &st);
    test_eq( r, (size_t)1 );
    test_eq( (long)c32, 65L );

    memset(&st, 0, sizeof st);
    r = mbrtoc32(&c32, "A", 0, &st);
    test_eq( r, (size_t)-2 );

    memset(&st, 0, sizeof st);
    c32 = 0xFFFF;
    r = mbrtoc32(&c32, "\0", 1, &st);
    test_eq( r, (size_t)0 );
    test_eq( (long)c32, 0L );

    if (!utf8) {
        TEST_NOTE("the locale's multibyte encoding is not UTF-8 here");
        TEST_SKIP_N(4);
    } else {
        char b[4];
        char b4[5];
        size_t k, k4;
        b[0]=(char)0xE2; b[1]=(char)0x82; b[2]=(char)0xAC; b[3]=0;
        memset(&st, 0, sizeof st);
        k = mbrtoc32(&c32, b, 3, &st);
        test_eq( k, (size_t)3 );
        test_eq( (long)c32, 0x20ACL );

        b4[0]=(char)0xF0; b4[1]=(char)0x9F; b4[2]=(char)0x98; b4[3]=(char)0x80; b4[4]=0;
        memset(&st, 0, sizeof st);
        k4 = mbrtoc32(&c32, b4, 4, &st);
        test_eq( k4, (size_t)4 );
        test_eq( (long)c32, 0x1F600L );
    }

    {
        char b[2];
        size_t e;
        b[0]=(char)0x80; b[1]=0;
        memset(&st, 0, sizeof st);
        e = mbrtoc32(&c32, b, 1, &st);
        test_eq( e, (size_t)-1 );
    }
    test_pass("c2011:mbrtoc32");

    memset(&st, 0, sizeof st); memset(buf, 0, sizeof buf);
    w = c32rtomb(buf, (char32_t)'Z', &st);
    test_eq( w, (size_t)1 );
    test_eq( (int)buf[0], (int)'Z' );

    if (!utf8) {
        TEST_NOTE("the locale's multibyte encoding is not UTF-8 here");
        TEST_SKIP_N(5);
    } else {
        memset(&st, 0, sizeof st); memset(buf, 0, sizeof buf);
        w3 = c32rtomb(buf, (char32_t)0x20AC, &st);
        test_eq( w3, (size_t)3 );
        test_eq( (int)(unsigned char)buf[0], 0xE2 );

        memset(&st, 0, sizeof st); memset(buf, 0, sizeof buf);
        w4 = c32rtomb(buf, (char32_t)0x1F600, &st);
        test_eq( w4, (size_t)4 );
        test_eq( (int)(unsigned char)buf[0], 0xF0 );
        test_eq( (int)(unsigned char)buf[3], 0x80 );
    }

    memset(&st, 0, sizeof st);
    we = c32rtomb(buf, (char32_t)0xD800, &st);
    test_eq( we, (size_t)-1 );
    test_pass("c2011:c32rtomb");
}

TEST_CASE(uchar, mbrtoc16_c16rtomb) {
    mbstate_t st;
    char16_t c16;
    size_t r, w, w3, wh, wl, we;
    char buf[8];
    int utf8;

    ccw_uchar_utf8_locale();
    {
        char probe[3];
        probe[0]=(char)0xE2; probe[1]=(char)0x82; probe[2]=(char)0xAC;
        memset(&st, 0, sizeof st);
        utf8 = (mbrtoc16(&c16, probe, 3, &st) == (size_t)3);
    }

    memset(&st, 0, sizeof st);
    c16 = 0;
    r = mbrtoc16(&c16, "A", 1, &st);
    test_eq( r, (size_t)1 );
    test_eq( (long)c16, 65L );

    if (!utf8) {
        TEST_NOTE("the locale's multibyte encoding is not UTF-8 here");
        TEST_SKIP_N(6);
    } else {
        char b[4];
        char b4[5];
        char16_t hi, lo;
        size_t k, k0, k1;
        b[0]=(char)0xE2; b[1]=(char)0x82; b[2]=(char)0xAC; b[3]=0;
        memset(&st, 0, sizeof st);
        k = mbrtoc16(&c16, b, 3, &st);
        test_eq( k, (size_t)3 );
        test_eq( (long)c16, 0x20ACL );

        b4[0]=(char)0xF0; b4[1]=(char)0x9F; b4[2]=(char)0x98; b4[3]=(char)0x80; b4[4]=0;
        memset(&st, 0, sizeof st);
        hi = 0; k0 = mbrtoc16(&hi, b4, 4, &st);
        test_eq( k0, (size_t)4 );
        test_eq( (long)hi, 0xD83DL );
        lo = 0; k1 = mbrtoc16(&lo, b4 + 4, 1, &st);
        test_eq( k1, (size_t)-3 );
        test_eq( (long)lo, 0xDE00L );
    }
    test_pass("c2011:mbrtoc16");

    memset(&st, 0, sizeof st); memset(buf, 0, sizeof buf);
    w = c16rtomb(buf, (char16_t)'Z', &st);
    test_eq( w, (size_t)1 );
    test_eq( (int)buf[0], (int)'Z' );

    if (!utf8) {
        TEST_NOTE("the locale's multibyte encoding is not UTF-8 here");
        TEST_SKIP_N(6);
    } else {
        memset(&st, 0, sizeof st); memset(buf, 0, sizeof buf);
        w3 = c16rtomb(buf, (char16_t)0x20AC, &st);
        test_eq( w3, (size_t)3 );
        test_eq( (int)(unsigned char)buf[0], 0xE2 );

        memset(&st, 0, sizeof st); memset(buf, 0, sizeof buf);
        wh = c16rtomb(buf, (char16_t)0xD83D, &st);  test_eq( wh, (size_t)0 );
        wl = c16rtomb(buf, (char16_t)0xDE00, &st);  test_eq( wl, (size_t)4 );
        test_eq( (int)(unsigned char)buf[0], 0xF0 );
        test_eq( (int)(unsigned char)buf[3], 0x80 );
    }

    memset(&st, 0, sizeof st);
    we = c16rtomb(buf, (char16_t)0xDE00, &st);
    test_eq( we, (size_t)-1 );
    test_pass("c2011:c16rtomb");
}


#if TEST_TARGET_C >= 2011
TEST_CASE(uchar, mbrtoc16_partial_results) {
    mbstate_t st;
    char16_t c16;
    char32_t c32;
    size_t r;
    static const char emoji[] = "\xF0\x9F\x98\x80";

    ccw_uchar_utf8_locale();
    memset(&st, 0, sizeof st);
    r = mbrtoc16(&c16, emoji, 4, &st);
    if (r == (size_t)-1) {
        TEST_NOTE("the C locale here cannot decode UTF-8; surrogate pair not reachable");
        TEST_SKIP_N(2);
    } else {
        test_eq( (long)r, 4L );
        r = mbrtoc16(&c16, emoji, 4, &st);
        test_eq( (long)r, (long)(size_t)-3 );
    }
    test_pass("c2011:mbrtoc16 returns (size_t)-3 for the trailing surrogate");

    memset(&st, 0, sizeof st);
    r = mbrtoc32(&c32, emoji, 2, &st);
    if (r == (size_t)-1) {
        TEST_NOTE("the C locale here cannot decode UTF-8; incomplete case not reachable");
        TEST_SKIP1();
    } else {
        test_eq( (long)r, (long)(size_t)-2 );
    }
    test_pass("c2011:mbrtoc32/mbrtoc16 return (size_t)-2 for an incomplete sequence");

    memset(&st, 0, sizeof st);
    r = mbrtoc16(NULL, NULL, 0, &st);
    test_eq( (long)r, 0L );
    test_true( mbsinit(&st) != 0 );
    test_pass("c2011:mbrtoc16(NULL, NULL, 0, ps) resets the conversion state");
}
#else
TEST_CASE(uchar, mbrtoc16_partial_results) {
    test_skip("c2011:mbrtoc16 returns (size_t)-3 for the trailing surrogate");
    test_skip("c2011:mbrtoc32/mbrtoc16 return (size_t)-2 for an incomplete sequence");
    test_skip("c2011:mbrtoc16(NULL, NULL, 0, ps) resets the conversion state");
}
#endif

TEST_SUITE(uchar)
    TEST_ADD(types_and_macros)
    TEST_ADD(mbrtoc8_c8rtomb)
    TEST_ADD(mbrtoc32_c32rtomb)
    TEST_ADD(mbrtoc16_c16rtomb)
    TEST_ADD(mbrtoc16_partial_results)
TEST_SUITE_END(uchar)
