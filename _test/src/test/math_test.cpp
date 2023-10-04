#include <_ccwrap/ccwrap_test.hpp>

#define __STDC_LIMIT_MACROS
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <cmath>
#include <limits>

CCWRAP_TEST_SUITE(math) {

    using namespace std;

    template<class T>
    void test_math() {
        T   zero = T(0);
        T   one  = T(1);
        T   minusOne = T(-1);
        T   qnan = numeric_limits<T>::quiet_NaN();
        T   inf  = numeric_limits<T>::infinity();

        ccwrap_test_assert(!isnan(zero));
        ccwrap_test_assert(!isnan(one));
        ccwrap_test_assert(!isnan(minusOne));
        ccwrap_test_assert(!isnan(inf));
        ccwrap_test_assert(!isnan(-inf));
        ccwrap_test_assert(isnan(qnan));
        ccwrap_test_assert(isnan(numeric_limits<T>::signaling_NaN()));

        ccwrap_test_assert(!isinf(zero));
        ccwrap_test_assert(!isinf(one));
        ccwrap_test_assert(!isinf(minusOne));
        ccwrap_test_assert(isinf(inf));
        ccwrap_test_assert(isinf(-inf));
        ccwrap_test_assert(!isinf(qnan));
        ccwrap_test_assert(!isinf(numeric_limits<T>::signaling_NaN()));

        ccwrap_test_assert(isfinite(zero));
        ccwrap_test_assert(isfinite(one));
        ccwrap_test_assert(isfinite(minusOne));
        ccwrap_test_assert(!isfinite(inf));
        ccwrap_test_assert(!isfinite(-inf));
        ccwrap_test_assert(!isfinite(qnan));
        ccwrap_test_assert(!isfinite(numeric_limits<T>::signaling_NaN()));

        ccwrap_test_assert(isfinite(zero));
        ccwrap_test_assert(isfinite(one));
        ccwrap_test_assert(isfinite(minusOne));
        ccwrap_test_assert(!isfinite(inf));
        ccwrap_test_assert(!isfinite(-inf));
        ccwrap_test_assert(!isfinite(qnan));
        ccwrap_test_assert(!isfinite(numeric_limits<T>::signaling_NaN()));

        ccwrap_test_assert(!signbit(zero));
        ccwrap_test_assert(!signbit(one));
        ccwrap_test_assert(signbit(minusOne));
        ccwrap_test_assert(!signbit(inf));
        ccwrap_test_assert(signbit(-inf));
        ccwrap_test_assert(!signbit(qnan));
        ccwrap_test_assert(!signbit(numeric_limits<T>::signaling_NaN()));

        ccwrap_test_assert(!isunordered(zero, one));
        ccwrap_test_assert(!isunordered(one, minusOne));
        ccwrap_test_assert(!isunordered(minusOne, inf));
        ccwrap_test_assert(!isunordered(inf, -inf));
        ccwrap_test_assert(isunordered(zero, qnan));
        ccwrap_test_assert(isunordered(qnan, zero));
        ccwrap_test_assert(isunordered(qnan, qnan));
        ccwrap_test_assert(isunordered(zero, numeric_limits<T>::signaling_NaN()));
        ccwrap_test_assert(isunordered(numeric_limits<T>::signaling_NaN(), zero));
        ccwrap_test_assert(isunordered(numeric_limits<T>::signaling_NaN(), qnan));

        ccwrap_test_assert(!isgreater(zero,zero));
        ccwrap_test_assert(!isgreater(zero,one));
        ccwrap_test_assert( isgreater(one,zero));
        ccwrap_test_assert(!isgreater(one,one));
        ccwrap_test_assert(!isgreater(qnan,qnan));
        ccwrap_test_assert(!isgreater(qnan,one));
        ccwrap_test_assert(!isgreater(one,qnan));
        ccwrap_test_assert(!isgreater(inf,inf));
        ccwrap_test_assert( isgreater(inf,one));
        ccwrap_test_assert(!isgreater(one,inf));

        ccwrap_test_assert( isgreaterequal(zero,zero));
        ccwrap_test_assert(!isgreaterequal(zero,one));
        ccwrap_test_assert( isgreaterequal(one,zero));
        ccwrap_test_assert( isgreaterequal(one,one));
        ccwrap_test_assert(!isgreaterequal(qnan,qnan));
        ccwrap_test_assert(!isgreaterequal(qnan,one));
        ccwrap_test_assert(!isgreaterequal(one,qnan));
        ccwrap_test_assert( isgreaterequal(inf,inf));
        ccwrap_test_assert( isgreaterequal(inf,one));
        ccwrap_test_assert(!isgreaterequal(one,inf));

        ccwrap_test_assert(!isless(zero,zero));
        ccwrap_test_assert( isless(zero,one));
        ccwrap_test_assert(!isless(one,zero));
        ccwrap_test_assert(!isless(one,one));
        ccwrap_test_assert(!isless(qnan,qnan));
        ccwrap_test_assert(!isless(qnan,one));
        ccwrap_test_assert(!isless(one,qnan));
        ccwrap_test_assert(!isless(inf,inf));
        ccwrap_test_assert(!isless(inf,one));
        ccwrap_test_assert( isless(one,inf));

        ccwrap_test_assert( islessequal(zero,zero));
        ccwrap_test_assert( islessequal(zero,one));
        ccwrap_test_assert(!islessequal(one,zero));
        ccwrap_test_assert( islessequal(one,one));
        ccwrap_test_assert(!islessequal(qnan,qnan));
        ccwrap_test_assert(!islessequal(qnan,one));
        ccwrap_test_assert(!islessequal(one,qnan));
        ccwrap_test_assert( islessequal(inf,inf));
        ccwrap_test_assert(!islessequal(inf,one));
        ccwrap_test_assert( islessequal(one,inf));
    }

    CCWRAP_TEST(double) {
        test_math<double>();
    }

    CCWRAP_TEST(float) {
        test_math<float>();
    }

    CCWRAP_TEST(long_double) {
        test_math<long double>();
    }

}   // CCWRAP_TEST_SUITE(math)
