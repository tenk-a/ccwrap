#include "test_cxx.hpp"
#include <cassert>

TEST_CASE(cassert, header_compiles) { test_true( true );     test_pass("cxx03:<cassert> header compiles");
}

#include <cassert>
#include <string>

TEST_CASE(cassert, macro_exists) {
#ifdef assert
    test_true( true );
#else
    test_true( false );
#endif

    test_no_throw( assert(1 == 1) );
    test_no_throw( assert(true) );

    int x = 10;
    int* p = &x;
    test_no_throw( assert(p != nullptr) );
    test_no_throw( assert(*p == 10) );

    STD::string s = "abc";
    test_no_throw( assert(s.size() == 3) );
    test_no_throw( assert(!s.empty()) );

    test_no_throw( assert(s.size()) );
    test_no_throw( assert(p) );

    for (int i = 1; i <= 3; ++i)
        assert(i > 0);
    if (x == 10)
        assert(x % 2 == 0);
    test_true( true );

    test_no_throw( assert((x = 10, x == 10)) );
    test_eq( x, 10 );
    test_pass("cxx03:assert");
}

TEST_CASE(cassert, ndebug) {

    int n = 0;
#ifdef NDEBUG
    assert((++n, true));
    test_eq( n, 0 );
#else
    assert((++n, true));
    test_eq( n, 1 );
#endif

    test_no_throw( assert(n >= 0) );
    test_pass("cxx03:NDEBUG disables assert");
}

TEST_CASE(cassert, is_a_macro_not_a_function) {
#if defined(assert)
    test_true( true );
#else
    test_true( false );
#endif

    static_assert(sizeof(long) >= sizeof(int), "long is at least as wide as int");
#if _TST_HAS_STATIC_ASSERT1
    static_assert(true);
#endif
    test_true( true );
    test_pass("cxx03:assert is a macro");
}


TEST_CASE(cassert, variadic_macro_cxx26) {
#if defined(__cpp_lib_variadic_assert)
    {
        int a = 1, b = 1;
        assert(a == b, "values differ");
    }
    test_true( true );
    test_pass("cxx26:assert variadic macro");
#else
    TEST_NOTE("the variadic assert is C++26 (P2264); not available here");
    test_skip("cxx26:assert variadic macro");
#endif

#if defined(NDEBUG)
    assert(false);
    test_true( true );
    test_pass("cxx26:assert variadic macro with NDEBUG");
#else
    TEST_NOTE("this translation unit is built with assertions enabled");
    test_skip("cxx26:assert variadic macro with NDEBUG");
#endif
}
