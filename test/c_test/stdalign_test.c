#include "c_test.h"

#include <stdalign.h>
#include <stddef.h>

TEST_CASE(stdalign, macros_and_keywords) {
#if defined(__alignof_is_defined) && __alignof_is_defined
    test_eq( (int)__alignof_is_defined, 1 );
    test_pass("c2011:__alignof_is_defined");

#if defined(__alignas_is_defined)
    test_eq( (int)__alignas_is_defined, 1 );
    test_pass("c2011:__alignas_is_defined");
#else
    TEST_SKIP1();
    TEST_NOTE("__alignas_is_defined not defined");
    test_skip("c2011:__alignas_is_defined");
#endif

    test_true( alignof(char) == 1 );
    test_true( alignof(double) >= alignof(char) );
    test_pass("c2011:alignof");
    test_true( _Alignof(int) >= 1 );
    test_true( alignof(double) == _Alignof(double) );
    test_pass("c2011:_Alignof");

    {
        alignas(16) char buf[16];
        TEST_SKIP_WAT("alignas on a local is not honoured (C)");
        test_true( ((size_t)(void*)buf & 15u) == 0 );
    }
    test_pass("c2011:alignas");
    {
        _Alignas(8) char b2[8];
        TEST_SKIP_WAT("_Alignas on a local is not honoured (C)");
        test_true( ((size_t)(void*)b2 & 7u) == 0 );
    }
    test_pass("c2011:_Alignas");
#else
    TEST_SKIP1(); TEST_NOTE("_Alignof/_Alignas not available on this C front-end");
    test_true( 1 ); test_pass("c2011:__alignof_is_defined");
    TEST_SKIP1(); test_true( 1 ); test_pass("c2011:__alignas_is_defined");
    TEST_SKIP1(); test_true( 1 ); test_pass("c2011:alignof");
    TEST_SKIP1(); test_true( 1 ); test_pass("c2011:_Alignof");
    TEST_SKIP1(); test_true( 1 ); test_pass("c2011:alignas");
    TEST_SKIP1(); test_true( 1 ); test_pass("c2011:_Alignas");
#endif
}


TEST_CASE(stdalign, version_macro_c2023) {
#if defined(__STDC_VERSION_STDALIGN_H__)
    test_true( __STDC_VERSION_STDALIGN_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_STDALIGN_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_STDALIGN_H__");
#endif
}

TEST_SUITE(stdalign)
    TEST_ADD(macros_and_keywords)
    TEST_ADD(version_macro_c2023)
TEST_SUITE_END(stdalign)
