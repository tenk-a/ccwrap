#include "c_test.h"

#include <locale.h>
#include <string.h>
#include <limits.h>

TEST_CASE(locale, lc_macros) {
    int cats[6];
    int i, j;
    void* np = NULL;
    test_true( np == NULL );             test_pass("c1990:NULL");
    cats[0] = LC_ALL;      cats[1] = LC_COLLATE; cats[2] = LC_CTYPE;
    cats[3] = LC_MONETARY; cats[4] = LC_NUMERIC; cats[5] = LC_TIME;

    for (i = 0; i < 6; ++i)
        for (j = i + 1; j < 6; ++j)
            test_ne( cats[i], cats[j] );
    test_true( LC_ALL == cats[0] );      test_pass("c1990:LC_ALL");
    test_true( LC_COLLATE == cats[1] );  test_pass("c1990:LC_COLLATE");
    test_true( LC_CTYPE == cats[2] );    test_pass("c1990:LC_CTYPE");
    test_true( LC_MONETARY == cats[3] ); test_pass("c1990:LC_MONETARY");
    test_true( LC_NUMERIC == cats[4] );  test_pass("c1990:LC_NUMERIC");
    test_true( LC_TIME == cats[5] );     test_pass("c1990:LC_TIME");
}

TEST_CASE(locale, set_and_query) {
    char saved[128];
    const char* s0 = setlocale(LC_ALL, NULL);
    const char* c;
    const char* nat;
    const char* bad;

    test_ptr( s0 );
    if (!s0) {
        TEST_NOTE("setlocale(LC_ALL, NULL) returned NULL; avoid dereferencing it");
        TEST_SKIP_N(11);
        return;
    }
    strcpy(saved, s0);

    c = setlocale(LC_ALL, "C");
    test_ptr( c );
    test_str_eq( c, "C" );
    test_str_eq( setlocale(LC_ALL, NULL), "C" );

    test_str_eq( setlocale(LC_NUMERIC, "C"), "C" );
    test_str_eq( setlocale(LC_CTYPE, NULL), "C" );
    test_ptr( setlocale(LC_COLLATE,  "C") );
    test_ptr( setlocale(LC_MONETARY, "C") );
    test_ptr( setlocale(LC_TIME,     "C") );

    nat = setlocale(LC_ALL, "");
    test_true( nat == nat );

    setlocale(LC_ALL, "C");
    bad = setlocale(LC_ALL, "no-such-locale-@@@");
    test_true( bad == NULL );
    test_str_eq( setlocale(LC_ALL, NULL), "C" );

    setlocale(LC_ALL, saved);
    test_pass("c1990:setlocale");
}

TEST_CASE(locale, localeconv_c_locale) {
    char saved[128];
    const char* current;
    struct lconv* lc;

    current = setlocale(LC_ALL, NULL);
    if (!current) {
        TEST_NOTE("setlocale(LC_ALL, NULL) returned NULL; locale state cannot be saved");
        TEST_SKIP_CASE();
        return;
    }
    strcpy(saved, current);
    setlocale(LC_ALL, "C");

    lc = localeconv();
    test_ptr( lc );

    test_str_eq( lc->decimal_point, "." );
    test_str_eq( lc->thousands_sep, "" );
    test_str_eq( lc->grouping, "" );

    test_str_eq( lc->int_curr_symbol,   "" );
    test_str_eq( lc->currency_symbol,   "" );
    test_str_eq( lc->mon_decimal_point, "" );
    test_str_eq( lc->mon_thousands_sep, "" );
    test_str_eq( lc->mon_grouping,      "" );
    test_str_eq( lc->positive_sign,     "" );
    test_str_eq( lc->negative_sign,     "" );

    {
        int mark = (int)(char)CHAR_MAX;
        test_eq( (int)lc->int_frac_digits, mark );
        test_eq( (int)lc->frac_digits,     mark );
        test_eq( (int)lc->p_cs_precedes,   mark );
        test_eq( (int)lc->p_sep_by_space,  mark );
        test_eq( (int)lc->n_cs_precedes,   mark );
        test_eq( (int)lc->n_sep_by_space,  mark );
        test_eq( (int)lc->p_sign_posn,     mark );
        test_eq( (int)lc->n_sign_posn,     mark );
    }

    setlocale(LC_ALL, saved);
    test_pass("c1990:localeconv (C-locale numeric & monetary fields)");
}

TEST_CASE(locale, lconv_member_types) {
    struct lconv* lc = localeconv();
    char* s;
    char  n;
    test_ptr( lc );
    s = lc->decimal_point;
    test_ptr( s );
    n = lc->frac_digits;
    test_true( n == n );
    test_pass("c1990:struct lconv");
}


TEST_CASE(locale, setlocale_forms) {
    char* p;
    char saved[64];

    p = setlocale(LC_ALL, NULL);
    test_ptr( p );
    if (!p) {
        TEST_NOTE("setlocale query returned NULL; avoid strlen/strncpy on it");
        TEST_SKIP_N(7);
        return;
    }
    test_true( strlen(p) > 0 );
    strncpy(saved, p, sizeof saved - 1);
    saved[sizeof saved - 1] = '\0';
    p = setlocale(LC_NUMERIC, NULL);
    test_ptr( p );
    test_pass("c1990:setlocale(category, NULL) query form");

    p = setlocale(LC_ALL, "C");
    test_ptr( p );
    test_true( strcmp(setlocale(LC_NUMERIC, NULL), "C") == 0 );
    test_true( localeconv()->decimal_point[0] == '.' );
    test_pass("c1990:setlocale(LC_ALL, 'C') resets to the C locale");

    p = setlocale(LC_ALL, "");
    if (p) {
        test_ptr( setlocale(LC_ALL, NULL) );
    } else {
        TEST_NOTE("the native locale is not available here; setlocale returned NULL");
        TEST_SKIP1();
    }
    setlocale(LC_ALL, "C");
    test_true( strcmp(setlocale(LC_NUMERIC, NULL), "C") == 0 );
    test_pass("c1990:setlocale(LC_ALL, '') implementation-defined native locale");
}

TEST_CASE(locale, lconv_international) {
#if !defined(__WATCOMC__) && !defined(_MSC_VER) && TEST_TARGET_C >= 1999
    struct lconv* lc;
    setlocale(LC_ALL, "C");
    lc = localeconv();
    test_ptr( lc );

    test_eq( (int)lc->int_p_cs_precedes, (int)CHAR_MAX );
    test_pass("c1999:struct lconv int_p_cs_precedes");
    test_eq( (int)lc->int_n_cs_precedes, (int)CHAR_MAX );
    test_pass("c1999:struct lconv int_n_cs_precedes");
    test_eq( (int)lc->int_p_sep_by_space, (int)CHAR_MAX );
    test_pass("c1999:struct lconv int_p_sep_by_space");
    test_eq( (int)lc->int_n_sep_by_space, (int)CHAR_MAX );
    test_pass("c1999:struct lconv int_n_sep_by_space");
    test_eq( (int)lc->int_p_sign_posn, (int)CHAR_MAX );
    test_pass("c1999:struct lconv int_p_sign_posn");
    test_eq( (int)lc->int_n_sign_posn, (int)CHAR_MAX );
    test_pass("c1999:struct lconv int_n_sign_posn");
#else
    TEST_NOTE("this struct lconv has no C99 int_* members (Open Watcom / MSVC)");
    TEST_SKIP_N(7);
    test_skip("c1999:struct lconv int_p_cs_precedes");
    test_skip("c1999:struct lconv int_n_cs_precedes");
    test_skip("c1999:struct lconv int_p_sep_by_space");
    test_skip("c1999:struct lconv int_n_sep_by_space");
    test_skip("c1999:struct lconv int_p_sign_posn");
    test_skip("c1999:struct lconv int_n_sign_posn");
#endif
}

TEST_SUITE(locale)
    TEST_ADD(lc_macros)
    TEST_ADD(set_and_query)
    TEST_ADD(localeconv_c_locale)
    TEST_ADD(lconv_member_types)
    TEST_ADD(setlocale_forms)
    TEST_ADD(lconv_international)
TEST_SUITE_END(locale)
