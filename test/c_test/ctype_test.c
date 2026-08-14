#include "c_test.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>

TEST_CASE(ctype, is_alnum_alpha_digit_xdigit) {
    int c, ok;

    test_true(  isalnum('a') ); test_true( isalnum('Z') ); test_true( isalnum('7') );
    test_true( !isalnum('+') ); test_true( !isalnum(' ') ); test_true( !isalnum(EOF) );
    test_pass("c1990:isalnum");

    test_true(  isalpha('a') ); test_true( isalpha('Q') );
    test_true( !isalpha('0') ); test_true( !isalpha('_') ); test_true( !isalpha(EOF) );
    test_pass("c1990:isalpha");

    test_true(  isdigit('0') ); test_true( isdigit('9') );
    test_true( !isdigit('a') ); test_true( !isdigit('/') ); test_true( !isdigit(EOF) );
    test_pass("c1990:isdigit");

    test_true(  isxdigit('0') ); test_true( isxdigit('a') ); test_true( isxdigit('F') );
    test_true( !isxdigit('g') ); test_true( !isxdigit('G') ); test_true( !isxdigit(EOF) );
    test_pass("c1990:isxdigit");

    ok = 1;
    for (c = '0'; c <= '9'; ++c)
        ok = ok && isdigit(c) && isalnum(c) && isxdigit(c) && !isalpha(c);
    test_true( ok );

    ok = 1;
    for (c = 0; c <= UCHAR_MAX; ++c) {
        int alnum = isalnum(c) != 0;
        int alpha = isalpha(c) != 0;
        int digit = isdigit(c) != 0;
        int graph = isgraph(c) != 0;
        int punct = ispunct(c) != 0;
        ok = ok && (alnum == (alpha || digit))
                && (graph == (alnum || punct));
    }
    test_true( ok );
    test_true( (isalnum(0) != 0) == ((isalpha(0) != 0) || (isdigit(0) != 0)) );
    test_true( (isalnum(UCHAR_MAX) != 0) ==
               ((isalpha(UCHAR_MAX) != 0) || (isdigit(UCHAR_MAX) != 0)) );
    test_pass("c1990:digit-class consistency property");
}

TEST_CASE(ctype, is_space_blank) {
    test_true(  isspace(' ')  ); test_true( isspace('\t') );
    test_true(  isspace('\n') ); test_true( isspace('\r') );
    test_true( !isspace('a') ); test_true( !isspace(EOF) );
    test_pass("c1990:isspace");

    test_true(  isblank(' ')  ); test_true( isblank('\t') );
    test_true( !isblank('\n') ); test_true( !isblank('a') );
    test_pass("c1999:isblank");
}

TEST_CASE(ctype, is_cntrl_print_graph_punct) {
    int c, printable, partition;

    test_true(  iscntrl('\n') ); test_true( iscntrl(0x00) ); test_true( iscntrl(0x7F) );
    test_true( !iscntrl(' ') ); test_true( !iscntrl(EOF) );
    test_pass("c1990:iscntrl");

    test_true(  isprint(' ') ); test_true( isprint('~') );
    test_true( !isprint('\n') );
    test_pass("c1990:isprint");

    test_true( !isgraph(' ') ); test_true( isgraph('!') ); test_true( !isgraph('\t') );
    test_pass("c1990:isgraph");

    test_true(  ispunct('!') ); test_true( ispunct('~') );
    test_true( !ispunct('a') ); test_true( !ispunct('0') ); test_true( !ispunct(' ') );
    test_pass("c1990:ispunct");

    printable = 1; partition = 1;
    for (c = 0x20; c < 0x7F; ++c) {
        printable = printable && isprint(c) && !iscntrl(c);
        partition = partition
                 && ((!!isgraph(c)) == (c != ' '))
                 && ((!!isgraph(c)) == (!!isalnum(c) || !!ispunct(c)));
    }
    test_true( printable );
    test_true( partition );
    test_pass("c1990:ctype printable-range partition property");
}

TEST_CASE(ctype, is_upper_lower) {
    test_true(  isupper('A') ); test_true( !isupper('a') ); test_true( !isupper(EOF) );
    test_pass("c1990:isupper");
    test_true(  islower('a') ); test_true( !islower('A') ); test_true( !islower(EOF) );
    test_pass("c1990:islower");
}

TEST_CASE(ctype, toupper_tolower) {
    int c, roundtrip;

    test_eq( toupper('a'), (int)'A' );
    test_eq( toupper('A'), (int)'A' );
    test_eq( toupper('5'), (int)'5' );
    test_eq( toupper(EOF), EOF );
    test_pass("c1990:toupper");

    test_eq( tolower('Z'), (int)'z' );
    test_eq( tolower('a'), (int)'a' );
    test_eq( tolower('5'), (int)'5' );
    test_eq( tolower(EOF), EOF );
    test_pass("c1990:tolower");

    roundtrip = 1;
    for (c = 'a'; c <= 'z'; ++c) {
        int u = toupper(c);
        int l = tolower(u);
        roundtrip = roundtrip && isupper(u) && islower(l)
                 && (l == c) && (u == c - 'a' + 'A');
    }
    test_true( roundtrip );

    roundtrip = 1;
    for (c = 0; c <= UCHAR_MAX; ++c) {
        int u = toupper(c);
        int l = tolower(c);
        roundtrip = roundtrip
                 && (islower(c) ? (isupper(u) != 0) : (u == c))
                 && (isupper(c) ? (islower(l) != 0) : (l == c));
    }
    test_true( roundtrip );
    test_eq( toupper(0), 0 );
    test_eq( tolower(0), 0 );
    test_lim( toupper(UCHAR_MAX), 0, UCHAR_MAX );
    test_lim( tolower(UCHAR_MAX), 0, UCHAR_MAX );
    test_pass("c1990:case-conversion round-trip property");
}

TEST_SUITE(ctype)
    TEST_ADD(is_alnum_alpha_digit_xdigit)
    TEST_ADD(is_space_blank)
    TEST_ADD(is_cntrl_print_graph_punct)
    TEST_ADD(is_upper_lower)
    TEST_ADD(toupper_tolower)
TEST_SUITE_END(ctype)
