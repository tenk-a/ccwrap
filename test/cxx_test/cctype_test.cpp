#include "test_cxx.hpp"

#include <cctype>

TEST_CASE(cctype, classify) {
    test_true(  STD_NS isdigit('5') );  test_true( !STD_NS isdigit('a') );
    test_true(  STD_NS isalpha('a') );  test_true(  STD_NS isalpha('Z') );  test_true( !STD_NS isalpha('5') );
    test_true(  STD_NS isalnum('9') );  test_true(  STD_NS isalnum('x') );
    test_true(  STD_NS isspace(' ') );  test_true(  STD_NS isspace('\t') ); test_true( !STD_NS isspace('x') );
    test_true(  STD_NS isupper('A') );  test_true( !STD_NS isupper('a') );
    test_true(  STD_NS islower('a') );  test_true( !STD_NS islower('A') );
    test_true(  STD_NS isxdigit('f') ); test_true( !STD_NS isxdigit('g') );
    test_true(  STD_NS ispunct('!') );  test_true( !STD_NS ispunct('a') );
    test_true(  STD_NS iscntrl('\n') ); test_true( !STD_NS iscntrl('a') );
    test_true(  STD_NS isprint('a') );  test_true( !STD_NS isprint('\n') );
    test_true(  STD_NS isgraph('a') );  test_true( !STD_NS isgraph(' ') );
    test_pass("cxx03:<cctype> classifiers reachable (smoke)");
}

TEST_CASE(cctype, convert) {
    test_eq( STD_NS toupper('a'), int('A') );
    test_eq( STD_NS toupper('Z'), int('Z') );
    test_eq( STD_NS toupper('5'), int('5') );
    test_eq( STD_NS tolower('A'), int('a') );
    test_eq( STD_NS tolower('a'), int('a') );
    test_eq( STD_NS tolower('9'), int('9') );
    test_pass("cxx03:<cctype> converters reachable (smoke)");
}

#include <cctype>
#include <cstdio>

TEST_CASE(cctype, alnum_alpha_digit_xdigit) {
    test_true(  !!STD_NS isalnum('a') );
    test_true(  !!STD_NS isalnum('Z') );
    test_true(  !!STD_NS isalnum('7') );
    test_true(  !STD_NS isalnum('+') );
    test_true(  !STD_NS isalnum(' ') );
    test_true(  !STD_NS isalnum(EOF) );
    test_pass("cxx03:isalnum");

    test_true(  !!STD_NS isalpha('a') );
    test_true(  !!STD_NS isalpha('Q') );
    test_true(  !STD_NS isalpha('0') );
    test_true(  !STD_NS isalpha('_') );
    test_true(  !STD_NS isalpha(EOF) );
    test_pass("cxx03:isalpha");

    test_true(  !!STD_NS isdigit('0') );
    test_true(  !!STD_NS isdigit('9') );
    test_true(  !STD_NS isdigit('a') );
    test_true(  !STD_NS isdigit('/') );
    test_true(  !STD_NS isdigit(EOF) );
    test_pass("cxx03:isdigit");

    test_true(  !!STD_NS isxdigit('0') );
    test_true(  !!STD_NS isxdigit('9') );
    test_true(  !!STD_NS isxdigit('a') );
    test_true(  !!STD_NS isxdigit('F') );
    test_true(  !STD_NS isxdigit('g') );
    test_true(  !STD_NS isxdigit('G') );
    test_true(  !STD_NS isxdigit(EOF) );

    bool ok = true;
    for (int c = '0'; c <= '9'; ++c)
        ok = ok && STD_NS isdigit(c) && STD_NS isalnum(c) && STD_NS isxdigit(c) && !STD_NS isalpha(c);
    test_true( ok );
    test_pass("cxx03:isxdigit");
}

TEST_CASE(cctype, space_blank_cntrl_print_graph_punct) {
    test_true(  !!STD_NS isspace(' ')  );
    test_true(  !!STD_NS isspace('\t') );
    test_true(  !!STD_NS isspace('\n') );
    test_true(  !!STD_NS isspace('\v') );
    test_true(  !!STD_NS isspace('\f') );
    test_true(  !!STD_NS isspace('\r') );
    test_true(  !STD_NS isspace('a') );
    test_true(  !STD_NS isspace(EOF) );
    test_pass("cxx03:isspace");

    test_true(  !!STD_NS isblank(' ')  );
    test_true(  !!STD_NS isblank('\t') );
    test_true(  !STD_NS isblank('\n') );
    test_true(  !STD_NS isblank('a') );
    test_true(  !STD_NS isblank(EOF) );
    test_pass("cxx11:isblank");

    test_true(  !!STD_NS iscntrl('\n') );
    test_true(  !!STD_NS iscntrl('\t') );
    test_true(  !!STD_NS iscntrl(0x00) );
    test_true(  !!STD_NS iscntrl(0x7F) );
    test_true(  !STD_NS iscntrl(' ') );
    test_true(  !STD_NS iscntrl('a') );
    test_true(  !STD_NS iscntrl(EOF) );
    test_pass("cxx03:iscntrl");

    test_true(  !!STD_NS isprint(' ') );
    test_true(  !!STD_NS isprint('a') );
    test_true(  !!STD_NS isprint('~') );
    test_true(  !STD_NS isprint('\n') );
    test_true(  !STD_NS isprint(EOF) );
    test_pass("cxx03:isprint");

    test_true(  !STD_NS isgraph(' ') );
    test_true(  !!STD_NS isgraph('a') );
    test_true(  !!STD_NS isgraph('!') );
    test_true(  !STD_NS isgraph('\t') );
    test_true(  !STD_NS isgraph(EOF) );
    test_pass("cxx03:isgraph");

    test_true(  !!STD_NS ispunct('!') );
    test_true(  !!STD_NS ispunct(',') );
    test_true(  !!STD_NS ispunct('~') );
    test_true(  !STD_NS ispunct('a') );
    test_true(  !STD_NS ispunct('0') );
    test_true(  !STD_NS ispunct(' ') );
    test_true(  !STD_NS ispunct(EOF) );

    bool printable = true, partition = true;
    for (int c = 0x20; c < 0x7F; ++c) {
        printable = printable && STD_NS isprint(c) && !STD_NS iscntrl(c);
        partition = partition
                 && ((!!STD_NS isgraph(c)) == (c != ' '))
                 && ((!!STD_NS isgraph(c)) == (!!STD_NS isalnum(c) || !!STD_NS ispunct(c)));
    }
    test_true( printable );
    test_true( partition );
    test_pass("cxx03:ispunct");
}

TEST_CASE(cctype, upper_lower_and_conversion) {
    test_true(  !!STD_NS isupper('A') );
    test_true(  !!STD_NS isupper('Z') );
    test_true(  !STD_NS isupper('a') );
    test_true(  !STD_NS isupper('0') );
    test_true(  !STD_NS isupper(EOF) );
    test_pass("cxx03:isupper");

    test_true(  !!STD_NS islower('a') );
    test_true(  !!STD_NS islower('z') );
    test_true(  !STD_NS islower('A') );
    test_true(  !STD_NS islower('0') );
    test_true(  !STD_NS islower(EOF) );
    test_pass("cxx03:islower");

    test_eq( STD_NS tolower('A'), (int)'a' );
    test_eq( STD_NS tolower('Z'), (int)'z' );
    test_eq( STD_NS tolower('a'), (int)'a' );
    test_eq( STD_NS tolower('5'), (int)'5' );
    test_eq( STD_NS tolower(EOF), EOF );
    test_pass("cxx03:tolower");

    test_eq( STD_NS toupper('a'), (int)'A' );
    test_eq( STD_NS toupper('z'), (int)'Z' );
    test_eq( STD_NS toupper('A'), (int)'A' );
    test_eq( STD_NS toupper('5'), (int)'5' );
    test_eq( STD_NS toupper(EOF), EOF );

    bool roundtrip = true;
    for (int c = 'a'; c <= 'z'; ++c) {
        int u = STD_NS toupper(c);
        int l = STD_NS tolower(u);
        roundtrip = roundtrip && STD_NS isupper(u) && STD_NS islower(l)
                 && (l == c) && (u == c - 'a' + 'A');
    }
    test_true( roundtrip );
    test_pass("cxx03:toupper");
}
