#include "test_cxx.hpp"
#include <ciso646>

TEST_CASE(ciso646, alternative_tokens) {

    test_true( true and true );
    test_true( true or false );
    test_true( not false );
    test_true( (1 == 1) and not (1 == 2) );

    test_eq( 0x0F bitand 0x03, 0x03 );
    test_eq( 0x0C bitor  0x03, 0x0F );
    test_eq( 0x0F xor    0x0A, 0x05 );
    test_eq( (compl 0) & 0xFF, 0xFF );

    test_true( 1 not_eq 2 );
    int x = 0x0F;
    x and_eq 0x03;
    test_eq( x, 0x03 );
    x or_eq 0x08;
    test_eq( x, 0x0B );
    x xor_eq 0x0F;
    test_eq( x, 0x04 );
    test_pass("cxx03:<ciso646> alternative-token macros");
}
