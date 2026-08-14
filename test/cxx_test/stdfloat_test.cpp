#include "test_cxx.hpp"
#if _TST_TEST_HAS(<stdfloat>)
#include <stdfloat>
#endif

TEST_CASE(stdfloat, fixed_width_types) {
    test_true( true );

#if defined(__STDCPP_FLOAT16_T__) && _TST_TEST_HAS(<stdfloat>)
    test_eq( sizeof(STD::float16_t), (STD::size_t)2 );
    test_pass("cxx23:float16_t");
#else
    TEST_SKIP1();
    test_skip("cxx23:float16_t");
#endif

#if defined(__STDCPP_FLOAT32_T__) && _TST_TEST_HAS(<stdfloat>)
    test_eq( sizeof(STD::float32_t), (STD::size_t)4 );
    test_pass("cxx23:float32_t");
#else
    TEST_SKIP1();
    test_skip("cxx23:float32_t");
#endif

#if defined(__STDCPP_FLOAT64_T__) && _TST_TEST_HAS(<stdfloat>)
    test_eq( sizeof(STD::float64_t), (STD::size_t)8 );
    test_pass("cxx23:float64_t");
#else
    TEST_SKIP1();
    test_skip("cxx23:float64_t");
#endif

#if defined(__STDCPP_FLOAT128_T__) && _TST_TEST_HAS(<stdfloat>)
    test_eq( sizeof(STD::float128_t), (STD::size_t)16 );
    test_pass("cxx23:float128_t");
#else
    TEST_SKIP1();
    test_skip("cxx23:float128_t");
#endif

#if defined(__STDCPP_BFLOAT16_T__) && _TST_TEST_HAS(<stdfloat>)
    test_eq( sizeof(STD::bfloat16_t), (STD::size_t)2 );
    test_pass("cxx23:bfloat16_t");
#else
    TEST_SKIP1();
    test_skip("cxx23:bfloat16_t");
#endif
}

TEST_CASE(stdfloat, predefined_macros) {
#if defined(__STDCPP_FLOAT16_T__)
    test_eq( (int)__STDCPP_FLOAT16_T__, 1 );
    test_pass("cxx23:__STDCPP_FLOAT16_T__");
#else
    test_skip("cxx23:__STDCPP_FLOAT16_T__");
#endif
#if defined(__STDCPP_FLOAT32_T__)
    test_eq( (int)__STDCPP_FLOAT32_T__, 1 );
    test_pass("cxx23:__STDCPP_FLOAT32_T__");
#else
    test_skip("cxx23:__STDCPP_FLOAT32_T__");
#endif
#if defined(__STDCPP_FLOAT64_T__)
    test_eq( (int)__STDCPP_FLOAT64_T__, 1 );
    test_pass("cxx23:__STDCPP_FLOAT64_T__");
#else
    test_skip("cxx23:__STDCPP_FLOAT64_T__");
#endif
#if defined(__STDCPP_FLOAT128_T__)
    test_eq( (int)__STDCPP_FLOAT128_T__, 1 );
    test_pass("cxx23:__STDCPP_FLOAT128_T__");
#else
    test_skip("cxx23:__STDCPP_FLOAT128_T__");
#endif
#if defined(__STDCPP_BFLOAT16_T__)
    test_eq( (int)__STDCPP_BFLOAT16_T__, 1 );
    test_pass("cxx23:__STDCPP_BFLOAT16_T__");
#else
    test_skip("cxx23:__STDCPP_BFLOAT16_T__");
#endif
}
