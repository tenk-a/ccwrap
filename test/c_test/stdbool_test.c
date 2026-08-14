#include "c_test.h"

#include <stdbool.h>

TEST_CASE(stdbool, bool_true_false) {
    bool t = true;
    bool f = false;

    test_true( true );
    test_eq( (int)true, 1 );
    test_pass("c1999:true");

    test_true( !false );
    test_eq( (int)false, 0 );
    test_pass("c1999:false");

    test_true( t );
    test_true( !f );
    test_true( sizeof(bool) >= 1 );

    t = 5;
    TEST_SKIP_WAT("_Bool does not normalise a nonzero store to 1");
    TEST_SKIP_VC("_Bool does not normalise a nonzero store to 1");
    test_eq( (int)t, 1 );
    t = 0;
    test_eq( (int)t, 0 );
    test_pass("c1999:bool");
}

TEST_CASE(stdbool, compat_macro) {
#ifdef __bool_true_false_are_defined
    test_eq( (int)__bool_true_false_are_defined, 1 );
    test_pass("c1999:__bool_true_false_are_defined");
#else
    TEST_SKIP1();
    TEST_NOTE("__bool_true_false_are_defined not defined");
    test_skip("c1999:__bool_true_false_are_defined");
#endif
}

TEST_SUITE(stdbool)
    TEST_ADD(bool_true_false)
    TEST_ADD(compat_macro)
TEST_SUITE_END(stdbool)
