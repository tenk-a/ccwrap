#include "c_test.h"

#include <assert.h>
#include <string.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
static_assert(sizeof(int) >= 2, "int is at least 16 bits");
static_assert(sizeof(char) == 1, "char is exactly 1 byte");
#endif

TEST_CASE(assert, macro_exists) {
#ifdef assert
    test_true( 1 );
#else
    test_fail();
#endif
    test_pass("c1990:assert(macro exists)");
}

TEST_CASE(assert, true_conditions_do_not_abort) {
    int x = 10;
    int* p = &x;
    const char* s = "abc";
    int i;

    assert(1 == 1);
    assert(x == 10);
    assert(p != NULL);
    assert(*p == 10);
    assert(strlen(s) == 3);

    assert(x);
    assert(p);
    assert(strlen(s));

    for (i = 1; i <= 3; ++i)
        assert(i > 0);
    if (x == 10)
        assert(x % 2 == 0);

    assert((x = 10, x == 10));
    test_eq( x, 10 );

    test_true( 1 );
    test_pass("c1990:assert(scalar expression, no abort when true)");
}

TEST_CASE(assert, ndebug) {

    int n = 0;
#ifdef NDEBUG
    assert((++n, 1));
    test_eq( n, 0 );
#else
    assert((++n, 1));
    test_eq( n, 1 );
#endif
    assert(n >= 0);
    test_true( 1 );
    test_pass("c1990:NDEBUG (compile-time disable of assert)");
}

TEST_CASE(assert, static_assert_block_scope) {
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    static_assert(sizeof(long) >= sizeof(int), "long is at least as wide as int");
    static_assert(sizeof(void*) >= sizeof(int), "a pointer holds at least an int here");

    _Static_assert(sizeof(char) == 1, "_Static_assert is the C11 keyword");
    test_true( 1 );
    test_pass("c2011:_Static_assert");
#ifdef static_assert
    test_true( 1 );
#else
    test_true( 1 );
#endif
    test_pass("c2011:static_assert");
#else
    TEST_NOTE("_Static_assert is a C11 keyword; ccwrap cannot add it to a C90/C99 front end");
    test_skip("c2011:_Static_assert");
    test_skip("c2011:static_assert");
#endif
}


TEST_CASE(assert, version_macro_c2023) {
#if defined(__STDC_VERSION_ASSERT_H__)
    test_true( __STDC_VERSION_ASSERT_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_ASSERT_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_ASSERT_H__");
#endif
}

TEST_SUITE(assert)
    TEST_ADD(macro_exists)
    TEST_ADD(true_conditions_do_not_abort)
    TEST_ADD(ndebug)
    TEST_ADD(static_assert_block_scope)
    TEST_ADD(version_macro_c2023)
TEST_SUITE_END(assert)
