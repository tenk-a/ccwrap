#include <ccwrap_misc/test.hpp>

#include <algorithm>
#include <iostream>
//#include <array>
#include "ccwrap_misc/array.hpp"


TEST_SUITE(array) {

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
	    test_eq(t0[0], vtype);
	    test_eq(t0.size(), idx);
	    test_eq(t0.max_size(), t0.size());
	    test_eq(t0.front(), 0);
	    test_eq(t0.back(), value_t(idx-1));
	    idx = 0;
	    for (size_t i = 0; i < t0.size(); ++i) {
	        test_eq(t0[i], i);
	        test_eq(t0.at(i), i);
	        t0[i] = value_t(i);
	        t0.at(i) = value_t(i);
	    }
	    T const& ct0 = t0;
	    for (size_t i = 0; i < ct0.size(); ++i) {
	        test_eq(ct0[i], i);
	        test_eq(ct0.at(i), i);
	    }

	    T   t1;
	    t1.fill(0);
	    T   t2(t1);
	    test_eq(t1.empty(), false);
	    test_true(t1 == t2);
	    test_false(t1 != t2);
	    test_false(t1 < t2);
	    test_true(t1 >= t2);
	    test_false(t1 > t2);
	    test_true(t1 <= t2);

	    T   t3 = t0;
	    test_true(t0 == t3);
	    test_false(t0 != t3);
	    test_false(t0 < t3);
	    test_true(t0 >= t3);
	    test_false(t0 > t3);
	    test_true(t0 <= t3);

	    t2 = t3;
	    test_true(t0 == t2);
	    std::swap(t1, t0);
	    test_true(t1 == t3);
	    t2.fill(0);
	    test_true(t2 == t0);

	    idx = 0;
	    for (typename T::iterator it = t2.begin(); it != t2.end(); ++it) {
	        *it = value_t(idx);
	        ++idx;
	    }
	    test_true(t1 == t2);
	    idx = 0;
	    for (typename T::const_iterator it = t1.begin(); it != t1.end(); ++it, ++idx) {
	        test_eq(*it, t2[idx]);
	    }
	    idx = 0;
	    for (typename T::const_iterator it = t1.cbegin(); it != t1.cend(); ++it, ++idx) {
	        test_eq(*it, t2[idx]);
	    }

	    t3.fill(0);
	    idx = 0;
	    for (typename T::reverse_iterator it = t3.rbegin(); it != t3.rend(); ++it) {
	        *it = value_t(idx);
	        ++idx;
	    }
	    std::reverse(t2.begin(), t2.end());
	    test_true(t2 == t3);
	    idx = 0;
		T const& ct2 = t2;
	    for (typename T::const_reverse_iterator it = ct2.rbegin(); it != ct2.rend(); ++it, ++idx) {
	        test_eq(t1[idx], *it);
	    }
	    t = t3;
	}

	struct smp_arrays {
	    std::array<char,127>    charArray_;
	    std::array<int,1000>    intArray_;
	    std::array<float,100>   fltArray_;
	};

	TEST_CASE(array, check) {
	    smp_arrays c;
	    array_test_1(c.charArray_);
	    array_test_1(c.intArray_);
        array_test_1(c.fltArray_);
	}
}
