#include "test_cxx.hpp"
#include <cstdalign>

TEST_CASE(cstdalign, header_compiles) { test_true( true );     test_pass("cxx11:<cstdalign> header compiles");
}

#if TEST_TARGET_CXX >= 2011
#include <cstdalign>

TEST_CASE(cstdalign, compat_header) {

#if defined(__alignas_is_defined)
    test_eq( (int)__alignas_is_defined, 1 );
#else
    TEST_SKIP1();
#endif
#if defined(__alignof_is_defined)
    test_eq( (int)__alignof_is_defined, 1 );
    test_pass("cxx11:__alignof_is_defined");
#else
    TEST_SKIP1();
    test_skip("cxx11:__alignof_is_defined");
#endif

    test_true( alignof(char) == 1 );
    test_true( alignof(double) >= alignof(char) );

#if !defined(__WATCOMC__)
    alignas(16) char buf[16];
    test_true( ((STD::size_t)(void*)buf & 15u) == 0 );

    struct alignas(8) S { char c; };
    test_true( alignof(S) == 8 );
    test_true( sizeof(S) == 8 );
    test_pass("cxx11:__alignas_is_defined");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:__alignas_is_defined");
#endif
}
#else
TEST_CASE_SKIP(cstdalign, compat_header)
#endif
