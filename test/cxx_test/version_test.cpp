#include "test_cxx.hpp"
#include <version>

TEST_CASE(version, contract) {
    test_true( true );
#if defined(_CCW_LIBCXX03)
    test_true( _CCW_LIBCXX03 == 1 );
    test_pass("cxx20:<version> is a pure macro header");
#else
    TEST_SKIP1();
    test_skip("cxx20:<version> is a pure macro header");
#endif
}

TEST_CASE(version, complete_feature_macros) {
#if TEST_TARGET_CXX >= 2017
    test_true( __cpp_lib_as_const   == 201510L );
    test_true( __cpp_lib_clamp      == 201603L );
    test_true( __cpp_lib_gcd_lcm    == 201606L );
    test_true( __cpp_lib_invoke     == 201411L );
    test_true( __cpp_lib_scoped_lock == 201703L );
    test_true( __cpp_lib_shared_mutex == 201505L );
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
#if TEST_TARGET_CXX >= 2020
    test_true( __cpp_lib_bit_cast   == 201806L );
    test_true( __cpp_lib_bitops     == 201907L );
    test_true( __cpp_lib_endian     == 201907L );
    test_true( __cpp_lib_math_constants == 201907L );
    test_true( __cpp_lib_semaphore  == 201907L );
    test_true( __cpp_lib_remove_cvref == 201711L );
    test_true( __cpp_lib_atomic_lock_free_type_aliases == 201907L );
    test_true( __cpp_lib_atomic_flag_test == 201907L );
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
#if TEST_TARGET_CXX >= 2023 && defined(__cpp_lib_stdatomic_h)
    test_true( __cpp_lib_stdatomic_h == 202011L );
#else
    TEST_SKIP1();
#endif
#if TEST_TARGET_CXX >= 2014 && _TST_HAS_RVALUE_REF
    test_true( __cpp_lib_make_unique == 201304L );
    test_pass("cxx20:__cpp_lib_* feature-test macros");
#else
    TEST_SKIP1();
    test_skip("cxx20:__cpp_lib_* feature-test macros");
#endif
}

TEST_CASE(version, subset_macros_absent) {

#if defined(_CCW_LIBCXX03)
    test_true( true );
#  ifndef __cpp_lib_optional
    test_true( true );
#  else
    test_true( false );
#  endif
#  ifndef __cpp_lib_variant
    test_true( true );
#  else
    test_true( false );
#  endif
#  ifndef __cpp_lib_filesystem
    test_true( true );
#  else
    test_true( false );
#  endif
    test_pass("cxx20:unimplemented feature macros absent");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:unimplemented feature macros absent");
#endif
}

TEST_CASE(version, era_feature_macros) {

    test_skip("cxx11:__cpp_lib_* macro (library feature-test present)");

#if defined(__cpp_lib_integer_sequence) && (__cpp_lib_integer_sequence) >= 201304L
    test_true( (__cpp_lib_integer_sequence) >= 201304L );
    test_pass("cxx14:__cpp_lib_* macro (library feature-test present)");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx14:__cpp_lib_* macro (library feature-test present)");
#endif

#if defined(__cpp_lib_optional) && (__cpp_lib_optional) >= 201606L
    test_true( (__cpp_lib_optional) >= 201606L );
    test_pass("cxx17:__cpp_lib_* macro (library feature-test present)");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_* macro (library feature-test present)");
#endif

#if defined(__cpp_lib_to_underlying) && (__cpp_lib_to_underlying) >= 202102L
    test_true( (__cpp_lib_to_underlying) >= 202102L );
    test_pass("cxx23:__cpp_lib_* macro (library feature-test present)");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_* macro (library feature-test present)");
#endif

#if defined(__cpp_lib_ratio) && __cpp_lib_ratio >= 202306L
    test_true( __cpp_lib_ratio >= 202306L );
    test_pass("cxx26:__cpp_lib_* macro (library feature-test present)");
#else
    test_skip("cxx26:__cpp_lib_* macro (library feature-test present)");
#endif
}
