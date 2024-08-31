#include <ccwrap_misc/test.hpp>

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

TEST_SUITE(math) {

    using namespace std;

    template<class T>
    void test_math() {
        T   zero = T(0);
        T   one  = T(1);
        T   minusOne = T(-1);
        T   qnan = numeric_limits<T>::quiet_NaN();
        T   inf  = numeric_limits<T>::infinity();

        test_true(!isnan(zero));
        test_true(!isnan(one));
        test_true(!isnan(minusOne));
        test_true(!isnan(inf));
        test_true(!isnan(-inf));
        test_true(isnan(qnan));
        test_true(isnan(numeric_limits<T>::signaling_NaN()));

        test_true(!isinf(zero));
        test_true(!isinf(one));
        test_true(!isinf(minusOne));
        test_true(isinf(inf));
        test_true(isinf(-inf));
        test_true(!isinf(qnan));
        test_true(!isinf(numeric_limits<T>::signaling_NaN()));

        test_true(isfinite(zero));
        test_true(isfinite(one));
        test_true(isfinite(minusOne));
        test_true(!isfinite(inf));
        test_true(!isfinite(-inf));
        test_true(!isfinite(qnan));
        test_true(!isfinite(numeric_limits<T>::signaling_NaN()));

        test_true(isfinite(zero));
        test_true(isfinite(one));
        test_true(isfinite(minusOne));
        test_true(!isfinite(inf));
        test_true(!isfinite(-inf));
        test_true(!isfinite(qnan));
        test_true(!isfinite(numeric_limits<T>::signaling_NaN()));

        test_true(!signbit(zero));
        test_true(!signbit(one));
        test_true(signbit(minusOne));
        test_true(!signbit(inf));
        test_true(signbit(-inf));
        test_true(!signbit(qnan));
        test_true(!signbit(numeric_limits<T>::signaling_NaN()));

	 #if 1 //!defined(__WATCOMC__)
        test_true(!isunordered(zero, one));
        test_true(!isunordered(one, minusOne));
        test_true(!isunordered(minusOne, inf));
        test_true(!isunordered(inf, -inf));
        test_true(isunordered(zero, qnan));
        test_true(isunordered(qnan, zero));
        test_true(isunordered(qnan, qnan));
        test_true(isunordered(zero, numeric_limits<T>::signaling_NaN()));
        test_true(isunordered(numeric_limits<T>::signaling_NaN(), zero));
        test_true(isunordered(numeric_limits<T>::signaling_NaN(), qnan));

        test_true(!isgreater(zero,zero));
        test_true(!isgreater(zero,one));
        test_true( isgreater(one,zero));
        test_true(!isgreater(one,one));
        test_true(!isgreater(qnan,qnan));
        test_true(!isgreater(qnan,one));
        test_true(!isgreater(one,qnan));
        test_true(!isgreater(inf,inf));
        test_true( isgreater(inf,one));
        test_true(!isgreater(one,inf));

        test_true( isgreaterequal(zero,zero));
        test_true(!isgreaterequal(zero,one));
        test_true( isgreaterequal(one,zero));
        test_true( isgreaterequal(one,one));
        test_true(!isgreaterequal(qnan,qnan));
        test_true(!isgreaterequal(qnan,one));
        test_true(!isgreaterequal(one,qnan));
        test_true( isgreaterequal(inf,inf));
        test_true( isgreaterequal(inf,one));
        test_true(!isgreaterequal(one,inf));

        test_true(!isless(zero,zero));
        test_true( isless(zero,one));
        test_true(!isless(one,zero));
        test_true(!isless(one,one));
        test_true(!isless(qnan,qnan));
        test_true(!isless(qnan,one));
        test_true(!isless(one,qnan));
        test_true(!isless(inf,inf));
        test_true(!isless(inf,one));
        test_true( isless(one,inf));

        test_true( islessequal(zero,zero));
        test_true( islessequal(zero,one));
        test_true(!islessequal(one,zero));
        test_true( islessequal(one,one));
        test_true(!islessequal(qnan,qnan));
        test_true(!islessequal(qnan,one));
        test_true(!islessequal(one,qnan));
        test_true( islessequal(inf,inf));
        test_true(!islessequal(inf,one));
        test_true( islessequal(one,inf));
	 #endif
    }

	TEST_CASE(math_test, double) {
	    test_math<double>();
	}

	TEST_CASE(math_test, float) {
	    test_math<float>();
	}

	TEST_CASE(math_test, long_double) {
	    test_math<long double>();
	}

}
