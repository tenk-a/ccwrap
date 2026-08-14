#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011 && _TST_HAS_REMOVED_CXX26
#include <codecvt>

#if _TST_HAS_CHAR16_32
typedef char32_t      ccw_cvt_elem;
#else
typedef unsigned long ccw_cvt_elem;
#endif

TEST_CASE(codecvt, utf8_out_and_in) {
    STD::codecvt_utf8<ccw_cvt_elem> cv;
    STD::codecvt_utf8<ccw_cvt_elem>::state_type cst = STD::codecvt_utf8<ccw_cvt_elem>::state_type();

    ccw_cvt_elem src[3] = { 0x41, 0x3042, 0x1F600 };
    char buf[16];
    const ccw_cvt_elem* from_next;
    char* to_next;

    STD::codecvt_base::result r = cv.out(cst, src, src + 3, from_next, buf, buf + 16, to_next);
    test_true( r == STD::codecvt_base::ok );
    test_eq( (int)(to_next - buf), 1 + 3 + 4 );
    test_eq( (int)(unsigned char)buf[0], 0x41 );
    test_eq( (int)(unsigned char)buf[1], 0xE3 );
    test_eq( (int)(unsigned char)buf[4], 0xF0 );
    test_pass("cxx03:codecvt_utf8::out");

    ccw_cvt_elem out[3];
    const char* from_next2;
    ccw_cvt_elem* to_next2;
    STD::codecvt_base::result r2 = cv.in(cst, buf, to_next, from_next2, out, out + 3, to_next2);
    test_true( r2 == STD::codecvt_base::ok );
    test_eq( (unsigned long)out[0], 0x41UL );
    test_eq( (unsigned long)out[1], 0x3042UL );
    test_eq( (unsigned long)out[2], 0x1F600UL );
    test_pass("cxx03:codecvt_utf8::in");

    test_lim( cv.max_length(), 4, 6 );
    test_pass("cxx03:codecvt_utf8::max_length");
    test_eq( cv.encoding(), 0 );
    test_pass("cxx03:codecvt_utf8::encoding");
}
#else
TEST_CASE(codecvt, utf8_out_and_in) {
    TEST_NOTE("codecvt_utf8 was removed in C++26 and this library honours the removal");
    test_skip("cxx03:codecvt_utf8::out");
    test_skip("cxx03:codecvt_utf8::in");
    test_skip("cxx03:codecvt_utf8::max_length");
    test_skip("cxx03:codecvt_utf8::encoding");
}
#endif

#if TEST_TARGET_CXX >= 2011 && _TST_HAS_REMOVED_CXX26
#include <codecvt>
#include <locale>
#include <cwchar>
#include <cstring>

typedef STD::codecvt<char16_t, char, STD::mbstate_t> cvt16_base;
typedef STD::codecvt<char32_t, char, STD::mbstate_t> cvt32_base;

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1900
#  define _TST_CODECVT_LENGTH_COUNTS_CHARS 1
#else
#  define _TST_CODECVT_LENGTH_COUNTS_CHARS 0
#endif

TEST_CASE(codecvt, codecvt_mode_enum) {

    test_eq( (int)STD::little_endian,   1 );
    test_eq( (int)STD::generate_header, 2 );
    test_eq( (int)STD::consume_header,  4 );
    test_pass("cxx11:codecvt_mode flag values");

    STD::codecvt_mode m = STD::codecvt_mode(STD::consume_header | STD::generate_header);
    test_eq( (int)m, 6 );
    test_true( (m & STD::consume_header) != 0 );
    test_true( (m & STD::little_endian) == 0 );
    test_pass("cxx11:codecvt_mode bitwise combine");

    STD::codecvt_mode zero = STD::codecvt_mode();
    test_eq( (int)zero, 0 );
    test_pass("cxx11:codecvt_mode value-init");

#if _tst_cplusplus >= 201103L
    test_eq( (int)STD::codecvt_mode::little_endian, (int)STD::little_endian );
    test_pass("cxx11:codecvt_mode enumerator scope");
#else
    TEST_SKIP1();
    test_skip("cxx11:codecvt_mode enumerator scope");
#endif
}

TEST_CASE(codecvt, codecvt_utf8_in) {

    STD::codecvt_utf8<char32_t> cvt(0);
    STD::mbstate_t st = STD::mbstate_t();

    const char ext[] = "\x41\xC3\xA9\xE6\x97\xA5\xF0\x9F\x98\x80";
    const int  extn  = 10;
    char32_t   buf[8];
    const char* fnext = 0;
    char32_t*   tnext = 0;

    STD::codecvt_base::result r =
        cvt.in(st, ext, ext + extn, fnext, buf, buf + 8, tnext);
    test_true( r == STD::codecvt_base::ok );
    test_true( fnext == ext + extn );
    test_eq( (int)(tnext - buf), 4 );
    test_eq( (unsigned long)buf[0], 0x41ul );
    test_eq( (unsigned long)buf[1], 0xE9ul );
    test_eq( (unsigned long)buf[2], 0x65E5ul );
    test_eq( (unsigned long)buf[3], 0x1F600ul );
    test_pass("cxx11:codecvt_utf8::in (UTF-8 decode)");

    STD::mbstate_t st2 = STD::mbstate_t();
    STD::codecvt_base::result r2 =
        cvt.in(st2, ext, ext + 2, fnext, buf, buf + 8, tnext);
    test_true( r2 != STD::codecvt_base::error );
    test_eq( (int)(fnext - ext), 1 );
    test_eq( (int)(tnext - buf), 1 );
    test_eq( (unsigned long)buf[0], 0x41ul );
    test_pass("cxx11:codecvt_utf8::in truncated input -> partial");

    STD::mbstate_t st3 = STD::mbstate_t();
    STD::codecvt_base::result r3 =
        cvt.in(st3, ext, ext + extn, fnext, buf, buf + 1, tnext);
    test_true( r3 != STD::codecvt_base::error );
    test_eq( (int)(fnext - ext), 1 );
    test_eq( (int)(tnext - buf), 1 );
    test_pass("cxx11:codecvt_utf8::in dest-limited");
}

TEST_CASE(codecvt, codecvt_utf8_out) {

    STD::codecvt_utf8<char32_t> cvt(0);
    STD::mbstate_t st = STD::mbstate_t();

    const char32_t src[] = { 0x41, 0xE9, 0x65E5, 0x1F600 };
    char            obuf[24];
    const char32_t* fnext = 0;
    char*           tnext = 0;

    STD::codecvt_base::result r =
        cvt.out(st, src, src + 4, fnext, obuf, obuf + 24, tnext);
    test_true( r == STD::codecvt_base::ok );
    test_true( fnext == src + 4 );
    test_eq( (int)(tnext - obuf), 1 + 2 + 3 + 4 );
    test_eq( (int)(unsigned char)obuf[0], 0x41 );
    test_eq( (int)(unsigned char)obuf[1], 0xC3 );
    test_eq( (int)(unsigned char)obuf[2], 0xA9 );
    test_eq( (int)(unsigned char)obuf[3], 0xE6 );
    test_eq( (int)(unsigned char)obuf[6], 0xF0 );
    test_eq( (int)(unsigned char)obuf[9], 0x80 );
    test_pass("cxx11:codecvt_utf8::out (UTF-8 encode)");

    STD::mbstate_t st2 = STD::mbstate_t();
    STD::codecvt_base::result r2 =
        cvt.out(st2, src, src + 4, fnext, obuf, obuf + 2, tnext);
    test_true( r2 != STD::codecvt_base::error );
    test_eq( (int)(fnext - src), 1 );
    test_eq( (int)(tnext - obuf), 1 );
    test_eq( (int)(unsigned char)obuf[0], 0x41 );
    test_pass("cxx11:codecvt_utf8::out dest-limited");

    STD::mbstate_t st3 = STD::mbstate_t();
    cvt.out(st3, src, src + 4, fnext, obuf, obuf + 24, tnext);
    STD::mbstate_t st4 = STD::mbstate_t();
    char32_t back[8];
    const char* bfn = 0;
    char32_t*   btn = 0;
    cvt.in(st4, obuf, tnext, bfn, back, back + 8, btn);
    test_eq( (int)(btn - back), 4 );
    test_eq( (unsigned long)back[3], 0x1F600ul );
    test_pass("cxx11:codecvt_utf8 round-trip out then in");
}

TEST_CASE(codecvt, codecvt_utf8_properties) {
    STD::codecvt_utf8<char32_t> cvt(0);

    test_eq( cvt.encoding(), 0 );
    test_true( !cvt.always_noconv() );
    test_lim( cvt.max_length(), 1, 8 );
    test_pass("cxx11:codecvt_utf8 conversion properties");

    STD::mbstate_t st = STD::mbstate_t();
    const char ext[] = "\x41\xC3\xA9\xE6\x97\xA5";
#if _TST_CODECVT_LENGTH_COUNTS_CHARS
    TEST_SKIP_VC("native codecvt length() counts characters, not external chars");
#endif
    test_eq( cvt.length(st, ext, ext + 6, 3), 6 );
    STD::mbstate_t st2 = STD::mbstate_t();
#if _TST_CODECVT_LENGTH_COUNTS_CHARS
    TEST_SKIP_VC("native codecvt length() counts characters, not external chars");
#endif
    test_eq( cvt.length(st2, ext, ext + 6, 2), 3 );
    STD::mbstate_t st3 = STD::mbstate_t();
    test_eq( cvt.length(st3, ext, ext + 6, 1), 1 );
    STD::mbstate_t st4 = STD::mbstate_t();
    test_eq( cvt.length(st4, ext, ext + 6, 0), 0 );
    test_pass("cxx03:codecvt_utf8::length");

    STD::locale loc(STD::locale::classic(), new STD::codecvt_utf8<char32_t>);
    test_true( STD::has_facet<cvt32_base>(loc) );
    test_pass("cxx11:codecvt_utf8 as installed locale facet");

    STD::codecvt_utf8<char16_t> c16(0);
    test_true( !c16.always_noconv() );
    STD::codecvt_utf8<wchar_t> cw(0);
    test_eq( cw.encoding(), 0 );
    test_pass("cxx11:codecvt_utf8 char16_t and wchar_t elem");
}

TEST_CASE(codecvt, codecvt_utf8_maxcode_and_mode) {

    STD::codecvt_utf8<char32_t, 0x7F> ascii(0);
    STD::mbstate_t st = STD::mbstate_t();
    const char32_t hi[] = { 0xE9 };
    char obuf[8];
    const char32_t* fnext = 0;
    char* tnext = 0;
    STD::codecvt_base::result r = ascii.out(st, hi, hi + 1, fnext, obuf, obuf + 8, tnext);
    test_true( r == STD::codecvt_base::error );

    STD::mbstate_t st2 = STD::mbstate_t();
    const char32_t lo[] = { 0x41 };
    STD::codecvt_base::result r2 = ascii.out(st2, lo, lo + 1, fnext, obuf, obuf + 8, tnext);
    test_true( r2 == STD::codecvt_base::ok );
    test_eq( (int)(tnext - obuf), 1 );
    test_pass("cxx11:codecvt_utf8 Maxcode template param");

    STD::codecvt_utf8<char32_t, 0x10FFFF, STD::generate_header> gen(0);
    STD::mbstate_t st3 = STD::mbstate_t();
    STD::codecvt_base::result r3 = gen.out(st3, lo, lo + 1, fnext, obuf, obuf + 8, tnext);
    test_true( r3 == STD::codecvt_base::ok );
    test_eq( (int)(tnext - obuf), 4 );
    test_eq( (int)(unsigned char)obuf[0], 0xEF );
    test_eq( (int)(unsigned char)obuf[1], 0xBB );
    test_eq( (int)(unsigned char)obuf[2], 0xBF );
    test_eq( (int)(unsigned char)obuf[3], 0x41 );
    test_pass("cxx11:codecvt_utf8 generate_header BOM out");

    STD::codecvt_utf8<char32_t, 0x10FFFF, STD::consume_header> con(0);
    STD::mbstate_t st4 = STD::mbstate_t();
    const char withbom[] = "\xEF\xBB\xBF\x41\x42";
    char32_t ibuf[8];
    const char* ifn = 0;
    char32_t*   itn = 0;
    STD::codecvt_base::result r4 = con.in(st4, withbom, withbom + 5, ifn, ibuf, ibuf + 8, itn);
    test_true( r4 == STD::codecvt_base::ok );
    test_eq( (int)(itn - ibuf), 2 );
    test_eq( (unsigned long)ibuf[0], 0x41ul );
    test_eq( (unsigned long)ibuf[1], 0x42ul );
    test_pass("cxx11:codecvt_utf8 consume_header BOM in");
}

TEST_CASE(codecvt, codecvt_utf16_in_out) {

    STD::codecvt_utf16<char32_t> be(0);
    STD::mbstate_t st = STD::mbstate_t();

    const char ext[] = "\x00\x41\x65\xE5\xD8\x3D\xDE\x00";
    char32_t    buf[8];
    const char* fnext = 0;
    char32_t*   tnext = 0;

    STD::codecvt_base::result r = be.in(st, ext, ext + 8, fnext, buf, buf + 8, tnext);
    test_true( r == STD::codecvt_base::ok );
    test_eq( (int)(tnext - buf), 3 );
    test_eq( (unsigned long)buf[0], 0x41ul );
    test_eq( (unsigned long)buf[1], 0x65E5ul );
    test_eq( (unsigned long)buf[2], 0x1F600ul );
    test_pass("cxx11:codecvt_utf16::in (big endian)");

    STD::mbstate_t st2 = STD::mbstate_t();
    char obuf[16];
    const char32_t* ofn = 0;
    char*           otn = 0;
    STD::codecvt_base::result r2 = be.out(st2, buf, buf + 3, ofn, obuf, obuf + 16, otn);
    test_true( r2 == STD::codecvt_base::ok );
    test_eq( (int)(otn - obuf), 8 );
    test_eq( (int)(unsigned char)obuf[0], 0x00 );
    test_eq( (int)(unsigned char)obuf[1], 0x41 );
    test_eq( (int)(unsigned char)obuf[4], 0xD8 );
    test_pass("cxx11:codecvt_utf16::out (big endian)");

    STD::codecvt_utf16<char32_t, 0x10FFFF, STD::little_endian> le(0);
    STD::mbstate_t st3 = STD::mbstate_t();
    const char lext[] = "\x41\x00\xE5\x65";
    char32_t   lbuf[4];
    const char* lfn = 0;
    char32_t*   ltn = 0;
    STD::codecvt_base::result r3 = le.in(st3, lext, lext + 4, lfn, lbuf, lbuf + 4, ltn);
    test_true( r3 == STD::codecvt_base::ok );
    test_eq( (int)(ltn - lbuf), 2 );
    test_eq( (unsigned long)lbuf[0], 0x41ul );
    test_eq( (unsigned long)lbuf[1], 0x65E5ul );
    test_pass("cxx11:codecvt_utf16 little_endian mode");

    test_eq( be.encoding(), 0 );
    test_true( !be.always_noconv() );
    test_lim( be.max_length(), 1, 16 );
    test_pass("cxx11:codecvt_utf16 conversion properties");

    STD::mbstate_t st4 = STD::mbstate_t();
#if _TST_CODECVT_LENGTH_COUNTS_CHARS
    TEST_SKIP_VC("native codecvt length() counts characters, not external chars");
#endif
    test_eq( be.length(st4, ext, ext + 8, 3), 8 );
    STD::mbstate_t st5 = STD::mbstate_t();
#if _TST_CODECVT_LENGTH_COUNTS_CHARS
    TEST_SKIP_VC("native codecvt length() counts characters, not external chars");
#endif
    test_eq( be.length(st5, ext, ext + 8, 1), 2 );
    test_pass("cxx03:codecvt_utf16::length");

    STD::mbstate_t st6 = STD::mbstate_t();
    STD::codecvt_base::result r6 = be.in(st6, ext, ext + 6, fnext, buf, buf + 8, tnext);
    test_true( r6 != STD::codecvt_base::error );
    test_eq( (int)(fnext - ext), 4 );
    test_eq( (int)(tnext - buf), 2 );
    test_eq( (unsigned long)buf[1], 0x65E5ul );
    test_pass("cxx11:codecvt_utf16::in half surrogate -> partial");

    STD::mbstate_t st7 = STD::mbstate_t();
    STD::codecvt_base::result r7 = be.in(st7, ext, ext + 8, fnext, buf, buf + 1, tnext);
    test_true( r7 != STD::codecvt_base::error );
    test_eq( (int)(fnext - ext), 2 );
    test_eq( (int)(tnext - buf), 1 );
    test_pass("cxx11:codecvt_utf16::in dest-limited");
}

TEST_CASE(codecvt, codecvt_utf8_utf16) {

    STD::codecvt_utf8_utf16<char16_t> cvt(0);
    STD::mbstate_t st = STD::mbstate_t();

    const char ext[] = "\x41\xE6\x97\xA5\xF0\x9F\x98\x80";
    char16_t    buf[8];
    const char* fnext = 0;
    char16_t*   tnext = 0;

    STD::codecvt_base::result r = cvt.in(st, ext, ext + 8, fnext, buf, buf + 8, tnext);
    test_true( r == STD::codecvt_base::ok );
    test_eq( (int)(tnext - buf), 4 );
    test_eq( (unsigned)buf[0], 0x41u );
    test_eq( (unsigned)buf[1], 0x65E5u );
    test_eq( (unsigned)buf[2], 0xD83Du );
    test_eq( (unsigned)buf[3], 0xDE00u );
    test_pass("cxx03:codecvt_utf8_utf16::in");

    STD::mbstate_t st2 = STD::mbstate_t();
    char            obuf[16];
    const char16_t* ofn = 0;
    char*           otn = 0;
    STD::codecvt_base::result r2 = cvt.out(st2, buf, buf + 4, ofn, obuf, obuf + 16, otn);
    test_true( r2 == STD::codecvt_base::ok );
    test_eq( (int)(otn - obuf), 8 );
    test_eq( (int)STD::memcmp(obuf, ext, 8), 0 );
    test_pass("cxx03:codecvt_utf8_utf16::out");

    test_eq( cvt.encoding(), 0 );
    test_true( !cvt.always_noconv() );
    test_lim( cvt.max_length(), 1, 8 );
    test_pass("cxx11:codecvt_utf8_utf16 conversion properties");

    STD::mbstate_t stt = STD::mbstate_t();
    STD::codecvt_base::result r3 = cvt.in(stt, ext, ext + 3, fnext, buf, buf + 8, tnext);
    test_true( r3 != STD::codecvt_base::error );
    test_eq( (int)(fnext - ext), 1 );
    test_eq( (int)(tnext - buf), 1 );
    test_eq( (unsigned)buf[0], 0x41u );
    test_pass("cxx11:codecvt_utf8_utf16::in truncated -> partial");

    STD::mbstate_t st3 = STD::mbstate_t();
#if _TST_CODECVT_LENGTH_COUNTS_CHARS
    TEST_SKIP_VC("native codecvt length() counts characters, not external chars");
#endif
    test_eq( cvt.length(st3, ext, ext + 8, 4), 8 );
    STD::mbstate_t st4 = STD::mbstate_t();
#if _TST_CODECVT_LENGTH_COUNTS_CHARS
    TEST_SKIP_VC("native codecvt length() counts characters, not external chars");
#endif
    test_eq( cvt.length(st4, ext, ext + 8, 2), 4 );
    test_pass("cxx03:codecvt_utf8_utf16::length");

    STD::codecvt_utf8_utf16<wchar_t> cw(0);
    test_eq( cw.encoding(), 0 );
    STD::mbstate_t st5 = STD::mbstate_t();
    wchar_t     wbuf[8];
    const char* wfn = 0;
    wchar_t*    wtn = 0;
    STD::codecvt_base::result rw = cw.in(st5, ext, ext + 4, wfn, wbuf, wbuf + 8, wtn);
    test_true( rw == STD::codecvt_base::ok );
    test_eq( (int)(wtn - wbuf), 2 );
    test_eq( (unsigned long)wbuf[1], 0x65E5ul );
    test_pass("cxx11:codecvt_utf8_utf16 wchar_t elem");
}

TEST_CASE(codecvt, facet_lifetime) {

    {
        STD::locale loc(STD::locale::classic(), new STD::codecvt_utf8<char16_t>(0));
        test_true( STD::has_facet<cvt16_base>(loc) );
    }
    test_pass("cxx11:codecvt facet refs=0 locale-owned");
    {
        STD::codecvt_utf16<char16_t> keep(1);
        STD::locale loc(STD::locale::classic(), &keep);
        test_true( STD::has_facet<cvt16_base>(loc) );
    }
    test_pass("cxx11:codecvt facet refs=1 stack-safe");
    {
        STD::locale loc(STD::locale::classic(), new STD::codecvt_utf8_utf16<char16_t>(0));
        const cvt16_base& f = STD::use_facet<cvt16_base>(loc);
        test_eq( f.encoding(), 0 );
    }
    test_pass("cxx11:use_facet on codecvt_utf8_utf16");

    {
        STD::codecvt_utf8<char32_t>       a(0);
        STD::codecvt_utf16<char32_t>      b(0);
        STD::codecvt_utf8_utf16<char16_t> c(0);
        test_true( !a.always_noconv() && !b.always_noconv() && !c.always_noconv() );
    }
    test_true( true );
    test_pass("cxx11:codecvt facets automatic destruction");
}

TEST_CASE(codecvt, more_facet_members) {
    STD::codecvt_utf8<char32_t> u8(0);

    test_true( !u8.always_noconv() );
    test_pass("cxx11:codecvt_utf8::always_noconv");

    STD::mbstate_t st = STD::mbstate_t();
    char ub[8];
    char* un = 0;
    STD::codecvt_base::result ur = u8.unshift(st, ub, ub + 8, un);
    test_true( ur == STD::codecvt_base::noconv || ur == STD::codecvt_base::ok );
    test_eq( (int)(un - ub), 0 );
    test_pass("cxx03:codecvt_utf8::unshift");

    STD::codecvt_utf16<char32_t> u16(0);
    test_true( u16.max_length() > 0 );
    test_pass("cxx03:codecvt_utf16::max_length");

    STD::mbstate_t st2 = STD::mbstate_t();
    char32_t src[2] = { 0x41, 0x42 };
    char small[2];
    const char32_t* fn = 0;
    char* tn = 0;
    STD::codecvt_base::result r = u16.out(st2, src, src + 2, fn, small, small + 2, tn);

    test_true( r == STD::codecvt_base::partial || r == STD::codecvt_base::ok );
    test_true( fn < src + 2 );
    test_true( (long)(tn - small) <= 2 );
    test_pass("cxx11:codecvt_utf16::out dest-limited");

    STD::codecvt_utf8_utf16<char16_t> uu(0);
    test_true( uu.max_length() > 0 );
    test_pass("cxx03:codecvt_utf8_utf16::max_length");
}
#else
TEST_CASE_SKIP(codecvt, codecvt_mode_enum)
TEST_CASE_SKIP(codecvt, codecvt_utf8_in)
TEST_CASE_SKIP(codecvt, codecvt_utf8_out)
TEST_CASE_SKIP(codecvt, codecvt_utf8_properties)
TEST_CASE_SKIP(codecvt, codecvt_utf8_maxcode_and_mode)
TEST_CASE_SKIP(codecvt, codecvt_utf16_in_out)
TEST_CASE_SKIP(codecvt, codecvt_utf8_utf16)
TEST_CASE_SKIP(codecvt, facet_lifetime)
TEST_CASE_SKIP(codecvt, more_facet_members)
#endif
