#include <ccwrap_misc/ccwrap_test.hpp>

#include <algorithm>
#include <iostream>
#include <array>


CCWRAP_TEST_SUITE(array) {

	template<class T>
	void array_test_1(T& t) {
	    using namespace std;
	    T   t0;
	    typedef typename T::value_type  value_t;
	    value_t vtype = 0;
	    size_t  idx = 0;
	    for (typename T::iterator it = t0.begin(); it != t0.end(); ++it) {
	        *it = value_t(idx);
	        ++idx;
	    }
	    ccwrap_test_eq(t0[0], vtype);
	    ccwrap_test_eq(t0.size(), idx);
	    ccwrap_test_eq(t0.max_size(), t0.size());
	    ccwrap_test_eq(t0.front(), 0);
	    ccwrap_test_eq(t0.back(), value_t(idx-1));
	    idx = 0;
	    for (size_t i = 0; i < t0.size(); ++i) {
	        ccwrap_test_eq(t0[i], i);
	        ccwrap_test_eq(t0.at(i), i);
	        t0[i] = value_t(i);
	        t0.at(i) = value_t(i);
	    }
	    T const& ct0 = t0;
	    for (size_t i = 0; i < ct0.size(); ++i) {
	        ccwrap_test_eq(ct0[i], i);
	        ccwrap_test_eq(ct0.at(i), i);
	    }
	    T   t1 = T();
	    T   t2(t1);
	    T   t3 = t0;
	    ccwrap_test_eq(t1.empty(), false);
	    ccwrap_test_assert(t1 == t2);
	    ccwrap_test_assert(t0 == t3);

	    t2 = t3;
	    ccwrap_test_assert(t0 == t2);
	    swap(t1, t0);
	    ccwrap_test_assert(t1 == t3);
	    t2.fill(0);
	    ccwrap_test_assert(t2 == t0);

	    idx = 0;
	    for (typename T::iterator it = t2.begin(); it != t2.end(); ++it) {
	        *it = value_t(idx);
	        ++idx;
	    }
	    ccwrap_test_assert(t1 == t2);
	    idx = 0;
	    for (typename T::const_iterator it = t1.begin(); it != t1.end(); ++it, ++idx) {
	        ccwrap_test_eq(*it, t2[idx]);
	    }
	    idx = 0;
	    for (typename T::const_iterator it = t1.cbegin(); it != t1.cend(); ++it, ++idx) {
	        ccwrap_test_eq(*it, t2[idx]);
	    }

	    t3.fill(0);
	    idx = 0;
	    for (typename T::reverse_iterator it = t3.rbegin(); it != t3.rend(); ++it) {
	        *it = value_t(idx);
	        ++idx;
	    }
	    std::reverse(t2.begin(), t2.end());
	    ccwrap_test_assert(t2 == t3);
	    idx = 0;
	    for (typename T::const_reverse_iterator it = t2.rbegin(); it != t2.rend(); ++it, ++idx) {
	        ccwrap_test_eq(t1[idx], *it);
	    }
	    t = t3;
	}

	struct smp_arrays {
	    std::array<char,127>    charArray_;
	    std::array<int,1000>    intArray_;
	    std::array<float,100>   fltArray_;
	};



	CCWRAP_TEST(check) {
	    smp_arrays c;
	    array_test_1(c.charArray_);
	    array_test_1(c.intArray_);
	    array_test_1(c.fltArray_);
	}

}
