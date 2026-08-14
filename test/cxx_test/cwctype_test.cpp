#include "test_cxx.hpp"
#include <cwctype>

TEST_CASE(cwctype, header_compiles) { test_true( true );  test_pass("cxx03:<cwctype> header compiles"); }

#include <cwctype>
#include <cwchar>
#include <clocale>
#include <string>

TEST_CASE(cwctype, types_and_weof) {

    STD_NS wint_t w = STD_NS wint_t(L'a');
    test_eq( (long)w, (long)L'a' );
    test_ge( sizeof(STD_NS wint_t), sizeof(wchar_t) );
    test_pass("cxx03:wint_t");

    STD_NS wint_t eof = WEOF;
    test_ne( (long)eof, (long)L'a' );
    test_ne( (long)eof, 0L );
    test_pass("cxx03:WEOF");

    STD_NS wctype_t   ty = STD_NS wctype("alpha");
    test_ne( (long)ty, 0L );
    test_pass("cxx03:wctype_t");

    STD_NS wctrans_t  tr = STD_NS wctrans("toupper");
    test_ne( (long)tr, 0L );
    test_pass("cxx03:wctrans_t");
}

TEST_CASE(cwctype, classification) {
    test_true( STD_NS iswalnum(L'a') && STD_NS iswalnum(L'Z') && STD_NS iswalnum(L'0') );
    test_true( !STD_NS iswalnum(L'+') && !STD_NS iswalnum(L' ') );
    test_pass("cxx03:iswalnum");

    test_true( STD_NS iswalpha(L'a') && STD_NS iswalpha(L'Z') );
    test_true( !STD_NS iswalpha(L'0') && !STD_NS iswalpha(L'_') );
    test_pass("cxx03:iswalpha");

    test_true( STD_NS iswdigit(L'0') && STD_NS iswdigit(L'9') );
    test_true( !STD_NS iswdigit(L'a') && !STD_NS iswdigit(L' ') );
    test_pass("cxx03:iswdigit");

    test_true( STD_NS iswxdigit(L'0') && STD_NS iswxdigit(L'f') && STD_NS iswxdigit(L'F') );
    test_true( !STD_NS iswxdigit(L'g') && !STD_NS iswxdigit(L'G') );
    test_pass("cxx03:iswxdigit");

    test_true( STD_NS iswlower(L'a') && !STD_NS iswlower(L'A') && !STD_NS iswlower(L'0') );
    test_pass("cxx03:iswlower");

    test_true( STD_NS iswupper(L'A') && !STD_NS iswupper(L'a') && !STD_NS iswupper(L'0') );
    test_pass("cxx03:iswupper");

    test_true( STD_NS iswspace(L' ')  && STD_NS iswspace(L'\t') && STD_NS iswspace(L'\n') );
    test_true( STD_NS iswspace(L'\v') && STD_NS iswspace(L'\f') && STD_NS iswspace(L'\r') );
    test_true( !STD_NS iswspace(L'a') );
    test_pass("cxx03:iswspace");

    test_true( STD_NS iswblank(L' ') && STD_NS iswblank(L'\t') );
    test_true( !STD_NS iswblank(L'\n') && !STD_NS iswblank(L'a') );
    test_pass("cxx11:iswblank");

    test_true( STD_NS iswcntrl(L'\n') && STD_NS iswcntrl(L'\0') );
    test_true( !STD_NS iswcntrl(L' ') && !STD_NS iswcntrl(L'a') );
    test_pass("cxx03:iswcntrl");

    test_true( STD_NS iswprint(L' ') && STD_NS iswprint(L'a') && STD_NS iswprint(L'~') );
    test_true( !STD_NS iswprint(L'\n') );
    test_pass("cxx03:iswprint");

    test_true( STD_NS iswgraph(L'a') && STD_NS iswgraph(L'!') );
    test_true( !STD_NS iswgraph(L' ') && !STD_NS iswgraph(L'\n') );
    test_pass("cxx03:iswgraph");

    test_true( STD_NS iswpunct(L'!') && STD_NS iswpunct(L',') );
    test_true( !STD_NS iswpunct(L'a') && !STD_NS iswpunct(L'0') && !STD_NS iswpunct(L' ') );
    test_pass("cxx03:iswpunct");

    for (int c = 0; c < 128; ++c) {
        STD_NS wint_t x = STD_NS wint_t(c);
        bool alnum = STD_NS iswalnum(x) != 0;
        bool alpha = STD_NS iswalpha(x) != 0;
        bool digit = STD_NS iswdigit(x) != 0;
        bool graph = STD_NS iswgraph(x) != 0;
        bool print = STD_NS iswprint(x) != 0;
        bool punct = STD_NS iswpunct(x) != 0;
        bool space = STD_NS iswspace(x) != 0;
        bool cntrl = STD_NS iswcntrl(x) != 0;
        test_eq( alnum, alpha || digit );
        test_eq( graph, alnum || punct );
        if (c == '	') TEST_SKIP_VC110("MSVC before VS2013 calls TAB printing");
        test_eq( print, graph || c == ' ' );
        if (c == '	') TEST_SKIP_VC110("MSVC before VS2013 calls TAB printing");
        test_true( !(cntrl && print) );
        test_true( !(space && graph) );
    }

    test_true( !STD_NS iswalnum(WEOF) && !STD_NS iswalpha(WEOF) && !STD_NS iswdigit(WEOF) );
    test_true( !STD_NS iswspace(WEOF) && !STD_NS iswprint(WEOF) && !STD_NS iswpunct(WEOF) );
    test_pass("cxx03:classification partition invariants (+ WEOF)");
}

TEST_CASE(cwctype, wctype_iswctype) {

    static const char* names[] = {
        "alnum", "alpha", "blank", "cntrl", "digit", "graph",
        "lower", "print", "punct", "space", "upper", "xdigit",
    };
    for (int i = 0; i < 12; ++i) {
        STD_NS wctype_t t = STD_NS wctype(names[i]);
        if (i == 2) TEST_SKIP_VC110("MSVC before VS2013 has no wctype(\"blank\")");
        test_ne( (long)t, 0L );
    }

    STD_NS wctype_t none = STD_NS wctype("no-such-class");
    test_eq( (long)none, 0L );
    test_pass("cxx03:wctype");

    STD_NS wctype_t alpha  = STD_NS wctype("alpha");
    STD_NS wctype_t digit  = STD_NS wctype("digit");
    STD_NS wctype_t space  = STD_NS wctype("space");
    STD_NS wctype_t upper  = STD_NS wctype("upper");
    STD_NS wctype_t xdigit = STD_NS wctype("xdigit");

    test_true( STD_NS iswctype(L'a', alpha) != 0 );
    test_true( STD_NS iswctype(L'1', alpha) == 0 );
    test_true( STD_NS iswctype(L'7', digit) != 0 );
    test_true( STD_NS iswctype(L' ', space) != 0 );
    test_true( STD_NS iswctype(L'A', upper) != 0 );
    test_true( STD_NS iswctype(L'e', xdigit) != 0 );

    for (int c = 0; c < 128; ++c) {
        STD_NS wint_t x = STD_NS wint_t(c);
        bool via_class = STD_NS iswctype(x, alpha) != 0;
        bool direct    = STD_NS iswalpha(x) != 0;
        test_eq( via_class, direct );
    }
    test_pass("cxx03:iswctype");
}

TEST_CASE(cwctype, towlower_towupper) {
    test_eq( (long)STD_NS towlower(L'A'), (long)L'a' );
    test_eq( (long)STD_NS towlower(L'a'), (long)L'a' );
    test_eq( (long)STD_NS towlower(L'0'), (long)L'0' );

    test_eq( (long)STD_NS towlower(WEOF), (long)WEOF );
    test_pass("cxx03:towlower");

    test_eq( (long)STD_NS towupper(L'a'), (long)L'A' );
    test_eq( (long)STD_NS towupper(L'A'), (long)L'A' );
    test_eq( (long)STD_NS towupper(L'%'), (long)L'%' );

    for (int c = 'a'; c <= 'z'; ++c) {
        STD_NS wint_t up = STD_NS towupper(STD_NS wint_t(c));
        test_true( STD_NS iswupper(up) != 0 );
        test_eq( (long)STD_NS towlower(up), (long)c );
    }

    test_eq( (long)STD_NS towupper(WEOF), (long)WEOF );
    test_pass("cxx03:towupper");
}

TEST_CASE(cwctype, wctrans_towctrans) {
    STD_NS wctrans_t up = STD_NS wctrans("toupper");
    STD_NS wctrans_t lo = STD_NS wctrans("tolower");
    test_ne( (long)up, 0L );
    test_ne( (long)lo, 0L );

    STD_NS wctrans_t none = STD_NS wctrans("no-such-trans");
    test_eq( (long)none, 0L );
    test_pass("cxx03:wctrans");

    test_eq( (long)STD_NS towctrans(L'a', up), (long)L'A' );
    test_eq( (long)STD_NS towctrans(L'A', lo), (long)L'a' );
    test_eq( (long)STD_NS towctrans(L'7', up), (long)L'7' );

    for (int c = 0; c < 128; ++c) {
        STD_NS wint_t x = STD_NS wint_t(c);
        test_eq( (long)STD_NS towctrans(x, up), (long)STD_NS towupper(x) );
        test_eq( (long)STD_NS towctrans(x, lo), (long)STD_NS towlower(x) );
    }
    test_pass("cxx03:towctrans");
}
