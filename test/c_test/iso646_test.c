#include "c_test.h"

#include <iso646.h>

TEST_CASE(iso646, alternative_tokens) {
    int x;

    test_true( 1 and 1 ); test_true( (1 == 1) and not (1 == 2) );
    test_pass("c1995:and");
    test_true( 1 or 0 );
    test_pass("c1995:or");
    test_true( not 0 );
    test_pass("c1995:not");

    test_eq( 0x0F bitand 0x03, 0x03 );
    test_pass("c1995:bitand");
    test_eq( 0x0C bitor  0x03, 0x0F );
    test_pass("c1995:bitor");
    test_eq( 0x0F xor    0x0A, 0x05 );
    test_pass("c1995:xor");
    test_eq( (compl 0) & 0xFF, 0xFF );
    test_pass("c1995:compl");

    test_true( 1 not_eq 2 );
    test_pass("c1995:not_eq");
    x = 0x0F;
    x and_eq 0x03;
    test_eq( x, 0x03 );
    test_pass("c1995:and_eq");
    x or_eq 0x08;
    test_eq( x, 0x0B );
    test_pass("c1995:or_eq");
    x xor_eq 0x0F;
    test_eq( x, 0x04 );
    test_pass("c1995:xor_eq");
}

TEST_SUITE(iso646)
    TEST_ADD(alternative_tokens)
TEST_SUITE_END(iso646)
