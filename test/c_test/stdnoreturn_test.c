#include "c_test.h"

#if TEST_TARGET_C >= 2011
#include <stdnoreturn.h>
#include <stdlib.h>

#if defined(noreturn)
noreturn static void ccw_stop_macro(void);
static void ccw_stop_macro(void) { exit(99); }
#endif

_Noreturn static void ccw_stop_keyword(void);
static void ccw_stop_keyword(void) { exit(99); }

TEST_CASE(stdnoreturn, macro_c2011) {
#if defined(noreturn)
    void (*fp)(void) = ccw_stop_macro;
    test_ptr( (void*)fp );
    test_true( fp != ccw_stop_keyword );
    test_pass("c2011:noreturn");
#else
    TEST_SKIP1();
    TEST_SKIP1();
    TEST_NOTE("noreturn macro is obsolescent in C23 and this library no longer defines it");
    test_skip("c2011:noreturn");
#endif
}

TEST_CASE(stdnoreturn, keyword_c2011) {
    void (*fp)(void) = ccw_stop_keyword;
    test_ptr( (void*)fp );
    test_pass("c2011:_Noreturn");
}

#else
TEST_CASE(stdnoreturn, macro_c2011) {
    test_skip("c2011:noreturn");
}
TEST_CASE(stdnoreturn, keyword_c2011) {
    test_skip("c2011:_Noreturn");
}
#endif

TEST_SUITE(stdnoreturn)
    TEST_ADD(macro_c2011)
    TEST_ADD(keyword_c2011)
TEST_SUITE_END(stdnoreturn)
