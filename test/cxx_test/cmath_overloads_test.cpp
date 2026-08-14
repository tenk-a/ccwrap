#include "test_cxx.hpp"
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <stdlib.h>
#include <type_traits>

TEST_CASE(cmath, float_overloads_std_cxx11) {
    test_true( STD_NS abs(-2.5f) == 2.5f );
    test_true( STD_NS abs(-2.5L) == 2.5L );
    test_true( STD_NS fabs(-2.5f) == 2.5f );
    test_true( STD_NS fabs(-2.5L) == 2.5L );
    test_true( STD_NS sqrt(9.0f) == 3.0f );
    test_true( STD_NS sqrt(9.0L) == 3.0L );
    test_true( STD_NS pow(2.0f, 3.0f) == 8.0f );
    test_true( STD_NS pow(2.0L, 3.0L) == 8.0L );
    test_true( STD_NS fmod(5.0f, 3.0f) == 2.0f );
    test_true( STD_NS ceil(1.2f) == 2.0f );
    test_true( STD_NS floor(1.8f) == 1.0f );
    test_pass("cxx03:cmath float/long double overloads (C++98 set)");

    test_true( STD_NS hypot(3.0f, 4.0f) == 5.0f );
    test_true( STD_NS hypot(3.0L, 4.0L) == 5.0L );
    test_true( STD_NS round(1.5f) == 2.0f );
    test_true( STD_NS round(1.5L) == 2.0L );
    test_true( STD_NS trunc(1.9f) == 1.0f );
    test_true( STD_NS trunc(1.9L) == 1.0L );
    test_true( STD_NS cbrt(8.0f) == 2.0f );
    test_true( STD_NS fmax(1.0f, 2.0f) == 2.0f );
    test_true( STD_NS fmin(1.0f, 2.0f) == 1.0f );
    test_true( STD_NS fdim(3.0f, 1.0f) == 2.0f );
    test_true( STD_NS copysign(1.0f, -2.0f) == -1.0f );
    test_true( STD_NS log2(8.0f) == 3.0f );
    test_true( STD_NS exp2(3.0f) == 8.0f );
    test_true( STD_NS nearbyint(1.5f) == 2.0f || STD_NS nearbyint(1.5f) == 1.0f );
    test_true( STD_NS rint(2.0f) == 2.0f );
    test_true( STD_NS scalbn(1.0f, 3) == 8.0f );
    test_true( STD_NS fma(2.0f, 3.0f, 1.0f) == 7.0f );
    test_pass("cxx11:cmath float/long double overloads (C99 set)");
}

TEST_CASE(cmath, float_overloads_return_type_cxx11) {
    test_true( tst_type_is<float>(STD_NS sqrt(1.0f)) );
    test_true( tst_type_is<long double>(STD_NS sqrt(1.0L)) );
    test_true( tst_type_is<float>(STD_NS abs(1.0f)) );
    test_true( tst_type_is<float>(STD_NS hypot(1.0f, 1.0f)) );
    test_true( tst_type_is<float>(STD_NS round(1.0f)) );
#if !_TST_NO_GLOBAL_MATH_INT
    test_true( tst_type_is<double>(STD_NS sqrt(1)) );
    test_true( tst_type_is<double>(STD_NS pow(2, 3)) );
#else
    TEST_SKIP_N(2);
#endif
    test_pass("cxx11:cmath overloads keep the argument type");
}

TEST_CASE(cmath, float_overloads_global_cxx03) {
    test_true( ::abs(-2.5f) == 2.5f );
    test_true( ::abs(-2.5) == 2.5 );
    test_true( ::abs(-2.5L) == 2.5L );
    test_true( ::abs(-3) == 3 );
    test_pass("cxx03:global abs (float / double / long double)");

    test_true( ::fabs(-2.5f) == 2.5f );
    test_true( ::sqrt(9.0f) == 3.0f );
    test_true( ::pow(2.0f, 3.0f) == 8.0f );
    test_true( ::ceil(1.2f) == 2.0f );
    test_true( ::floor(1.8f) == 1.0f );
    test_true( ::fmod(5.0f, 3.0f) == 2.0f );
    test_true( ::sin(0.0f) == 0.0f );
    test_true( ::log(1.0f) == 0.0f );
    test_pass("cxx03:<math.h> puts the float overloads in the global namespace");
}

TEST_CASE(cmath, float_overloads_global_cxx11) {
    test_true( ::hypot(3.0f, 4.0f) == 5.0f );
    test_true( ::round(1.5f) == 2.0f );
    test_true( ::trunc(1.9f) == 1.0f );
    test_true( ::cbrt(8.0f) == 2.0f );
    test_true( ::fmax(1.0f, 2.0f) == 2.0f );
    test_true( ::fmin(1.0f, 2.0f) == 1.0f );
    test_true( ::log2(8.0f) == 3.0f );
    test_true( ::exp2(3.0f) == 8.0f );
    test_pass("cxx11:<math.h> puts the C99 float overloads in the global namespace");
}

TEST_CASE(cmath, integer_promotion_overloads_cxx11) {
#if !_TST_NO_GLOBAL_MATH_INT
    test_true( STD_NS sqrt(9) == 3.0 );
    test_true( STD_NS pow(2, 3) == 8.0 );
    test_true( STD_NS fmod(5, 3) == 2.0 );
    test_true( STD_NS hypot(3, 4) == 5.0 );
#else
    TEST_SKIP_N(4);
#endif
#if !defined(_MSC_VER) || _MSC_VER >= 1700
    test_true( ::sqrt(9) == 3.0 );
    test_true( ::pow(2, 3) == 8.0 );
#else
    TEST_NOTE("pre-VS2012 <math.h> has no integral overloads in the global namespace;"
              " ccwrap cannot add them (native <cmath> imports ::sqrt into std, where"
              " the integral overloads already are)");
    TEST_SKIP_N(2);
#endif
    test_pass("cxx11:cmath integral arguments promote to double");
}
