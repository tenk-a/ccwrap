#include "c_test.h"

#include <wctype.h>
#include <wchar.h>

TEST_CASE(wctype, types_and_weof) {
    wint_t     w = (wint_t)L'a';
    wint_t     eof;
    wctype_t   ty;
    wctrans_t  tr;

    test_eq( (long)w, (long)L'a' );
    test_ge( sizeof(wint_t), sizeof(wchar_t) );

    eof = WEOF;
    test_ne( (long)eof, (long)L'a' );
    test_pass("c1995:WEOF");

    test_true( sizeof(wint_t) >= sizeof(wchar_t) );
    test_pass("c1995:wint_t");

    ty = wctype("alpha");
    test_ne( (long)ty, 0L );
    test_pass("c1995:wctype_t");

    tr = wctrans("toupper");
    test_ne( (long)tr, 0L );
    test_pass("c1995:wctrans_t");
}

TEST_CASE(wctype, classification) {
    int c;

    test_true( iswalnum(L'a') && iswalnum(L'Z') && iswalnum(L'0') && !iswalnum(L'+') );
    test_pass("c1995:iswalnum");
    test_true( iswalpha(L'a') && iswalpha(L'Z') && !iswalpha(L'0') && !iswalpha(L'_') );
    test_pass("c1995:iswalpha");
    test_true( iswdigit(L'0') && iswdigit(L'9') && !iswdigit(L'a') );
    test_pass("c1995:iswdigit");
    test_true( iswxdigit(L'0') && iswxdigit(L'f') && iswxdigit(L'F') && !iswxdigit(L'g') );
    test_pass("c1995:iswxdigit");
    test_true( iswlower(L'a') && !iswlower(L'A') );
    test_pass("c1995:iswlower");
    test_true( iswupper(L'A') && !iswupper(L'a') );
    test_pass("c1995:iswupper");
    test_true( iswspace(L' ') && iswspace(L'\t') && iswspace(L'\n') && !iswspace(L'a') );
    test_pass("c1995:iswspace");

    test_true( iswblank(L' ') && iswblank(L'\t') && !iswblank(L'\n') );
    test_pass("c1999:iswblank");

    test_true( iswcntrl(L'\n') && iswcntrl(L'\0') && !iswcntrl(L' ') );
    test_pass("c1995:iswcntrl");
    test_true( iswprint(L' ') && iswprint(L'a') && iswprint(L'~') && !iswprint(L'\n') );
    test_pass("c1995:iswprint");
    test_true( iswgraph(L'a') && iswgraph(L'!') && !iswgraph(L' ') );
    test_pass("c1995:iswgraph");
    test_true( iswpunct(L'!') && iswpunct(L',') && !iswpunct(L'a') && !iswpunct(L'0') );
    test_pass("c1995:iswpunct");

    for (c = 0; c < 128; ++c) {
        wint_t x = (wint_t)c;
        int alnum = iswalnum(x) != 0;
        int alpha = iswalpha(x) != 0;
        int digit = iswdigit(x) != 0;
        int graph = iswgraph(x) != 0;
        int print = iswprint(x) != 0;
        int punct = iswpunct(x) != 0;
        int space = iswspace(x) != 0;
        int cntrl = iswcntrl(x) != 0;
        test_eq( alnum, alpha || digit );
        test_eq( graph, alnum || punct );
        test_eq( print, graph || c == ' ' );
        test_true( !(cntrl && print) );
        test_true( !(space && graph) );
    }

    test_true( !iswalnum(WEOF) && !iswalpha(WEOF) && !iswdigit(WEOF) );
    test_true( !iswspace(WEOF) && !iswprint(WEOF) && !iswpunct(WEOF) );
    test_pass("c1995:isw* WEOF + partition invariants");
}

TEST_CASE(wctype, class_lookup) {
    static const char* names[12] = {
        "alnum", "alpha", "blank", "cntrl", "digit", "graph",
        "lower", "print", "punct", "space", "upper", "xdigit"
    };
    wctype_t none, alpha, digit, space, upper, xdigit;
    int i, c;

    for (i = 0; i < 12; ++i) {
        wctype_t t = wctype(names[i]);
        test_ne( (long)t, 0L );
    }

    none = wctype("no-such-class");
    test_eq( (long)none, 0L );

    alpha  = wctype("alpha");
    digit  = wctype("digit");
    space  = wctype("space");
    upper  = wctype("upper");
    xdigit = wctype("xdigit");

    test_true( iswctype(L'a', alpha) != 0 );
    test_true( iswctype(L'1', alpha) == 0 );
    test_true( iswctype(L'7', digit) != 0 );
    test_true( iswctype(L' ', space) != 0 );
    test_true( iswctype(L'A', upper) != 0 );
    test_true( iswctype(L'e', xdigit) != 0 );

    for (c = 0; c < 128; ++c) {
        wint_t x = (wint_t)c;
        int via_class = iswctype(x, alpha) != 0;
        int direct    = iswalpha(x) != 0;
        test_eq( via_class, direct );
    }
    test_pass("c1995:wctype");
    test_true( iswctype(L'a', wctype("alpha")) != 0 );
    test_pass("c1995:iswctype");
}

TEST_CASE(wctype, towlower_towupper) {
    int c;

    test_eq( (long)towlower(L'A'), (long)L'a' );
    test_eq( (long)towlower(L'a'), (long)L'a' );
    test_eq( (long)towlower(L'0'), (long)L'0' );
    test_eq( (long)towupper(L'a'), (long)L'A' );
    test_eq( (long)towupper(L'A'), (long)L'A' );
    test_eq( (long)towupper(L'%'), (long)L'%' );

    for (c = 'a'; c <= 'z'; ++c) {
        wint_t up = towupper((wint_t)c);
        test_true( iswupper(up) != 0 );
        test_eq( (long)towlower(up), (long)c );
    }

    test_eq( (long)towlower(WEOF), (long)WEOF );
    test_eq( (long)towupper(WEOF), (long)WEOF );
    test_pass("c1995:towlower");
    test_true( towupper(L'a') == (long)L'A' );
    test_pass("c1995:towupper");
}

TEST_CASE(wctype, wctrans_lookup) {
    wctrans_t up = wctrans("toupper");
    wctrans_t lo = wctrans("tolower");
    wctrans_t none;
    int c;

    test_ne( (long)up, 0L );
    test_ne( (long)lo, 0L );

    test_eq( (long)towctrans(L'a', up), (long)L'A' );
    test_eq( (long)towctrans(L'A', lo), (long)L'a' );
    test_eq( (long)towctrans(L'7', up), (long)L'7' );

    for (c = 0; c < 128; ++c) {
        wint_t x = (wint_t)c;
        test_eq( (long)towctrans(x, up), (long)towupper(x) );
        test_eq( (long)towctrans(x, lo), (long)towlower(x) );
    }

    none = wctrans("no-such-trans");
    test_eq( (long)none, 0L );
    test_pass("c1995:wctrans");
    test_true( towctrans(L'a', wctrans("toupper")) == (long)L'A' );
    test_pass("c1995:towctrans");
}

TEST_SUITE(wctype)
    TEST_ADD(types_and_weof)
    TEST_ADD(classification)
    TEST_ADD(class_lookup)
    TEST_ADD(towlower_towupper)
    TEST_ADD(wctrans_lookup)
TEST_SUITE_END(wctype)
