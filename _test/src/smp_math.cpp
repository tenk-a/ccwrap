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

using namespace std;

template<class T>
void test_math() {
	T	zero = T(0);
	T	one	 = T(1);
	T	minusOne = T(-1);
	T	qnan = numeric_limits<T>::quiet_NaN();
	T	inf  = numeric_limits<T>::infinity();

	assert(!isnan(zero));
	assert(!isnan(one));
	assert(!isnan(minusOne));
	assert(!isnan(inf));
	assert(!isnan(-inf));
	assert(isnan(qnan));
	assert(isnan(numeric_limits<T>::signaling_NaN()));

	assert(!isinf(zero));
	assert(!isinf(one));
	assert(!isinf(minusOne));
	assert(isinf(inf));
	assert(isinf(-inf));
	assert(!isinf(qnan));
	assert(!isinf(numeric_limits<T>::signaling_NaN()));

	assert(isfinite(zero));
	assert(isfinite(one));
	assert(isfinite(minusOne));
	assert(!isfinite(inf));
	assert(!isfinite(-inf));
	assert(!isfinite(qnan));
	assert(!isfinite(numeric_limits<T>::signaling_NaN()));

	assert(isfinite(zero));
	assert(isfinite(one));
	assert(isfinite(minusOne));
	assert(!isfinite(inf));
	assert(!isfinite(-inf));
	assert(!isfinite(qnan));
	assert(!isfinite(numeric_limits<T>::signaling_NaN()));

	assert(!signbit(zero));
	assert(!signbit(one));
	assert(signbit(minusOne));
	assert(!signbit(inf));
	assert(signbit(-inf));
	assert(!signbit(qnan));
	assert(!signbit(numeric_limits<T>::signaling_NaN()));

	assert(!isunordered(zero, one));
	assert(!isunordered(one, minusOne));
	assert(!isunordered(minusOne, inf));
	assert(!isunordered(inf, -inf));
	assert(isunordered(zero, qnan));
	assert(isunordered(qnan, zero));
	assert(isunordered(qnan, qnan));
	assert(isunordered(zero, numeric_limits<T>::signaling_NaN()));
	assert(isunordered(numeric_limits<T>::signaling_NaN(), zero));
	assert(isunordered(numeric_limits<T>::signaling_NaN(), qnan));

	assert(!isgreater(zero,zero));
	assert(!isgreater(zero,one));
	assert( isgreater(one,zero));
	assert(!isgreater(one,one));
	assert(!isgreater(qnan,qnan));
	assert(!isgreater(qnan,one));
	assert(!isgreater(one,qnan));
	assert(!isgreater(inf,inf));
	assert( isgreater(inf,one));
	assert(!isgreater(one,inf));

	assert( isgreaterequal(zero,zero));
	assert(!isgreaterequal(zero,one));
	assert( isgreaterequal(one,zero));
	assert( isgreaterequal(one,one));
	assert(!isgreaterequal(qnan,qnan));
	assert(!isgreaterequal(qnan,one));
	assert(!isgreaterequal(one,qnan));
	assert( isgreaterequal(inf,inf));
	assert( isgreaterequal(inf,one));
	assert(!isgreaterequal(one,inf));

	assert(!isless(zero,zero));
	assert( isless(zero,one));
	assert(!isless(one,zero));
	assert(!isless(one,one));
	assert(!isless(qnan,qnan));
	assert(!isless(qnan,one));
	assert(!isless(one,qnan));
	assert(!isless(inf,inf));
	assert(!isless(inf,one));
	assert( isless(one,inf));

	assert( islessequal(zero,zero));
	assert( islessequal(zero,one));
	assert(!islessequal(one,zero));
	assert( islessequal(one,one));
	assert(!islessequal(qnan,qnan));
	assert(!islessequal(qnan,one));
	assert(!islessequal(one,qnan));
	assert( islessequal(inf,inf));
	assert(!islessequal(inf,one));
	assert( islessequal(one,inf));
}

int main(int argc, char* argv[])
{
    std::printf("\n%s\n", argv[0]);
	test_math<double>();
	test_math<float>();
	test_math<long double>();
	return 0;
}
