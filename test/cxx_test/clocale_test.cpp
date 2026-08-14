#include "test_cxx.hpp"

#include <clocale>
#include <cstring>
#include <climits>

TEST_CASE(clocale, setlocale_and_lconv) {

    const char* __n = STD_NS setlocale(LC_ALL, "C");
    test_true( __n != 0 );
    test_pass("cxx03:setlocale (basic)");

    STD_NS lconv* __lc = STD_NS localeconv();
    test_true( __lc != 0 );
    test_pass("cxx03:localeconv (basic)");

    test_true( STD::strcmp(__lc->decimal_point, ".") == 0 );
    test_true( __lc->thousands_sep != 0 );
    test_true( __lc->grouping != 0 );
    test_true( __lc->int_curr_symbol != 0 );
    test_true( __lc->currency_symbol != 0 );
    test_true( __lc->mon_decimal_point != 0 );
    test_true( __lc->mon_thousands_sep != 0 );
    test_true( __lc->mon_grouping != 0 );
    test_true( __lc->positive_sign != 0 );
    test_true( __lc->negative_sign != 0 );

    char __cm = (char)CHAR_MAX;
    test_eq( __lc->int_frac_digits, __cm );
    test_eq( __lc->frac_digits, __cm );
    test_eq( __lc->p_cs_precedes, __cm );
    test_eq( __lc->p_sep_by_space, __cm );
    test_eq( __lc->n_cs_precedes, __cm );
    test_eq( __lc->n_sep_by_space, __cm );
    test_eq( __lc->p_sign_posn, __cm );
    test_eq( __lc->n_sign_posn, __cm );
    test_pass("cxx03:struct lconv (basic)");

    int __cats[6] = { LC_ALL, LC_COLLATE, LC_CTYPE, LC_MONETARY, LC_NUMERIC, LC_TIME };
    for (int __i = 0; __i < 6; ++__i)
        for (int __j = __i + 1; __j < 6; ++__j)
            test_true( __cats[__i] != __cats[__j] );
    test_pass("cxx03:LC_* category macros (basic)");
}

#if TEST_TARGET_CXX >= 2011
#include <clocale>

#include <locale.h>
#include <climits>
#include <cstring>
#include <string>

using _ccw::string_view;

TEST_CASE(clocale, lc_macros) {

    const int all = LC_ALL;
    test_eq( all, LC_ALL );
    test_ne( LC_ALL, LC_COLLATE );
    test_pass("cxx03:LC_ALL");

    test_ne( LC_COLLATE, LC_CTYPE );
    test_pass("cxx03:LC_COLLATE");

    test_ne( LC_CTYPE, LC_MONETARY );
    test_pass("cxx03:LC_CTYPE");

    test_ne( LC_MONETARY, LC_NUMERIC );
    test_pass("cxx03:LC_MONETARY");

    test_ne( LC_NUMERIC, LC_TIME );
    test_pass("cxx03:LC_NUMERIC");

    test_ne( LC_TIME, LC_ALL );
    test_pass("cxx03:LC_TIME");
}

TEST_CASE(clocale, setlocale) {
    const char* saved0 = STD_NS setlocale(LC_ALL, NULL);
    test_ptr( saved0 );
    STD::string saved(saved0);

    const char* c = STD_NS setlocale(LC_ALL, "C");
    test_ptr( c );
    test_str_eq( c, "C" );

    const char* q = STD_NS setlocale(LC_ALL, NULL);
    test_str_eq( q, "C" );

    const char* num = STD_NS setlocale(LC_NUMERIC, "C");
    test_ptr( num );
    test_str_eq( num, "C" );
    test_str_eq( STD_NS setlocale(LC_CTYPE, NULL), "C" );
    test_ptr( STD_NS setlocale(LC_COLLATE,  "C") );
    test_ptr( STD_NS setlocale(LC_MONETARY, "C") );
    test_ptr( STD_NS setlocale(LC_TIME,     "C") );

    const char* nat = STD_NS setlocale(LC_ALL, "");
    test_true( nat == NULL || STD::strlen(nat) >= 0 );

    STD_NS setlocale(LC_ALL, "C");
    const char* bad = STD_NS setlocale(LC_ALL, "no-such-locale-@@@");
    test_true( bad == NULL );
    test_str_eq( STD_NS setlocale(LC_ALL, NULL), "C" );

    STD_NS setlocale(LC_ALL, saved.c_str());
    test_pass("cxx03:setlocale");
}

TEST_CASE(clocale, localeconv_c_locale) {
    STD::string saved(STD_NS setlocale(LC_ALL, NULL));
    STD_NS setlocale(LC_ALL, "C");

    STD_NS lconv* lc = STD_NS localeconv();
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

    test_lim( (int)lc->int_frac_digits, (int)SCHAR_MAX, (int)CHAR_MAX );
    test_lim( (int)lc->frac_digits,     (int)SCHAR_MAX, (int)CHAR_MAX );
    test_lim( (int)lc->p_cs_precedes,   (int)SCHAR_MAX, (int)CHAR_MAX );
    test_lim( (int)lc->p_sep_by_space,  (int)SCHAR_MAX, (int)CHAR_MAX );
    test_lim( (int)lc->n_cs_precedes,   (int)SCHAR_MAX, (int)CHAR_MAX );
    test_lim( (int)lc->n_sep_by_space,  (int)SCHAR_MAX, (int)CHAR_MAX );
    test_lim( (int)lc->p_sign_posn,     (int)SCHAR_MAX, (int)CHAR_MAX );
    test_lim( (int)lc->n_sign_posn,     (int)SCHAR_MAX, (int)CHAR_MAX );

    STD_NS setlocale(LC_ALL, saved.c_str());
    test_pass("cxx03:localeconv (C-locale numeric & monetary fields)");
}

TEST_CASE(clocale, lconv_member_types) {

    STD_NS lconv* lc = STD_NS localeconv();
    char* s = lc->decimal_point;
    test_ptr( s );
    char  n = lc->frac_digits;
    test_true( n == n );

    test_eq( sizeof(STD_NS lconv), sizeof(lconv) );
    test_pass("cxx03:struct lconv member types");
}
#else
TEST_CASE_SKIP(clocale, lc_macros)
TEST_CASE_SKIP(clocale, setlocale)
TEST_CASE_SKIP(clocale, localeconv_c_locale)
TEST_CASE_SKIP(clocale, lconv_member_types)
#endif

TEST_CASE(clocale, null_macro) {
    void* p = NULL;
    test_true( p == 0 );
    char* cp = NULL;
    test_true( cp == 0 );
    test_pass("cxx03:NULL");
}


TEST_CASE(clocale, lconv_members_cxx03) {
    STD_NS setlocale(LC_ALL, "C");
    STD_NS lconv* lc = STD_NS localeconv();
    test_ptr( lc );

    test_true( STD::strcmp(lc->decimal_point, ".") == 0 );
    test_pass("cxx03:lconv::decimal_point");
    test_true( STD::strcmp(lc->thousands_sep, "") == 0 );
    test_pass("cxx03:lconv::thousands_sep");
    test_true( STD::strcmp(lc->grouping, "") == 0 );
    test_pass("cxx03:lconv::grouping");

    test_true( STD::strcmp(lc->int_curr_symbol, "") == 0 );
    test_pass("cxx03:lconv::int_curr_symbol");
    test_true( STD::strcmp(lc->currency_symbol, "") == 0 );
    test_pass("cxx03:lconv::currency_symbol");
    test_true( STD::strcmp(lc->mon_decimal_point, "") == 0 );
    test_pass("cxx03:lconv::mon_decimal_point");
    test_true( STD::strcmp(lc->mon_thousands_sep, "") == 0 );
    test_pass("cxx03:lconv::mon_thousands_sep");
    test_true( STD::strcmp(lc->mon_grouping, "") == 0 );
    test_pass("cxx03:lconv::mon_grouping");
    test_true( STD::strcmp(lc->positive_sign, "") == 0 );
    test_pass("cxx03:lconv::positive_sign");
    test_true( STD::strcmp(lc->negative_sign, "") == 0 );
    test_pass("cxx03:lconv::negative_sign");

    test_eq( (int)lc->int_frac_digits, (int)CHAR_MAX );
    test_pass("cxx03:lconv::int_frac_digits");
    test_eq( (int)lc->frac_digits, (int)CHAR_MAX );
    test_pass("cxx03:lconv::frac_digits");
    test_eq( (int)lc->p_cs_precedes, (int)CHAR_MAX );
    test_pass("cxx03:lconv::p_cs_precedes");
    test_eq( (int)lc->p_sep_by_space, (int)CHAR_MAX );
    test_pass("cxx03:lconv::p_sep_by_space");
    test_eq( (int)lc->n_cs_precedes, (int)CHAR_MAX );
    test_pass("cxx03:lconv::n_cs_precedes");
    test_eq( (int)lc->n_sep_by_space, (int)CHAR_MAX );
    test_pass("cxx03:lconv::n_sep_by_space");
    test_eq( (int)lc->p_sign_posn, (int)CHAR_MAX );
    test_pass("cxx03:lconv::p_sign_posn");
    test_eq( (int)lc->n_sign_posn, (int)CHAR_MAX );
    test_pass("cxx03:lconv::n_sign_posn");
}

TEST_CASE(clocale, lconv_international_members_cxx11) {
#if defined(_MSC_VER) || defined(__WATCOMC__) || defined(__MINGW32__)
    TEST_NOTE("this struct lconv has no C99 int_* members (MSVC / mingw UCRT / Open Watcom)");
    TEST_SKIP_N(7);
    test_skip("cxx11:lconv::int_p_cs_precedes");
    test_skip("cxx11:lconv::int_n_cs_precedes");
    test_skip("cxx11:lconv::int_p_sep_by_space");
    test_skip("cxx11:lconv::int_n_sep_by_space");
    test_skip("cxx11:lconv::int_p_sign_posn");
    test_skip("cxx11:lconv::int_n_sign_posn");
#else
    STD_NS setlocale(LC_ALL, "C");
    STD_NS lconv* lc = STD_NS localeconv();
    test_ptr( lc );

    test_eq( (int)lc->int_p_cs_precedes, (int)CHAR_MAX );
    test_pass("cxx11:lconv::int_p_cs_precedes");
    test_eq( (int)lc->int_n_cs_precedes, (int)CHAR_MAX );
    test_pass("cxx11:lconv::int_n_cs_precedes");
    test_eq( (int)lc->int_p_sep_by_space, (int)CHAR_MAX );
    test_pass("cxx11:lconv::int_p_sep_by_space");
    test_eq( (int)lc->int_n_sep_by_space, (int)CHAR_MAX );
    test_pass("cxx11:lconv::int_n_sep_by_space");
    test_eq( (int)lc->int_p_sign_posn, (int)CHAR_MAX );
    test_pass("cxx11:lconv::int_p_sign_posn");
    test_eq( (int)lc->int_n_sign_posn, (int)CHAR_MAX );
    test_pass("cxx11:lconv::int_n_sign_posn");
#endif
}
