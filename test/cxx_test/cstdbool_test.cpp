#include "test_cxx.hpp"

#include <cstdbool>

TEST_CASE(cstdbool, bool_true_false) {
    bool __t = true;
    bool __f = false;
    test_true( __t );
    test_true( !__f );
    test_true( true );
    test_true( !false );
    test_eq( (int)true, 1 );
    test_eq( (int)false, 0 );
    test_true( sizeof(bool) >= 1 );
    test_pass("cxx11:__bool_true_false_are_defined");
}

#include <cstdbool>

TEST_CASE(cstdbool, compat_header) {
#if defined(__bool_true_false_are_defined)
    test_eq( (int)__bool_true_false_are_defined, 1 );
#else
    TEST_SKIP1();
#endif

    bool t = true;
    bool f = false;
    test_true( t );
    test_true( !f );
    test_true( t != f );
    test_eq( (int)t, 1 );
    test_eq( (int)f, 0 );
    test_true( sizeof(bool) >= 1 );
    test_pass("cxx11:<cstdbool>");
}
