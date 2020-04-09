#include <array>
#include <iostream>
#include <algorithm>
#include "test_check.hpp"

template<class T>
void array_test_1(T& t) {
    using namespace std;
    T   t0;
    typename T::value_type  vtype = 0;
    size_t idx = 0;
    for (typename T::iterator it = t0.begin(); it != t0.end(); ++it) {
        *it = idx++;
    }
    TEST_EQ(t0[0], vtype);
    TEST_EQ(t0.size(), idx);
    TEST_EQ(t0.max_size(), t0.size());
    TEST_EQ(t0.front(), 0);
    TEST_EQ(t0.back(), idx-1);
    idx = 0;
    for (size_t i = 0; i < t0.size(); ++i) {
        TEST_EQ(t0[i], i);
        TEST_EQ(t0.at(i), i);
        t0[i] = i;
        t0.at(i) = i;
    }
    T const& ct0 = t0;
    for (size_t i = 0; i < ct0.size(); ++i) {
        TEST_EQ(ct0[i], i);
        TEST_EQ(ct0.at(i), i);
    }
    T   t1 = T();
    T   t2(t1);
    T   t3 = t0;
    TEST_EQ(t1.empty(), false);
    TEST_CHECK(t1 == t2);
    TEST_CHECK(t0 == t3);

    t2 = t3;
    TEST_CHECK(t0 == t2);
    swap(t1, t0);
    TEST_CHECK(t1 == t3);
    t2.fill(0);
    TEST_CHECK(t2 == t0);

    idx = 0;
    for (typename T::iterator it = t2.begin(); it != t2.end(); ++it) {
        *it = idx++;
    }
    TEST_CHECK(t1 == t2);
    idx = 0;
    for (typename T::const_iterator it = t1.begin(); it != t1.end(); ++it) {
        TEST_EQ(*it, t2[idx++]);
    }
    idx = 0;
    for (typename T::const_iterator it = t1.cbegin(); it != t1.cend(); ++it) {
        TEST_EQ(*it, t2[idx++]);
    }

    t3.fill(0);
    idx = 0;
    for (typename T::reverse_iterator it = t3.rbegin(); it != t3.rend(); ++it) {
        *it = idx++;
    }
    std::reverse(t2.begin(), t2.end());
    TEST_CHECK(t2 == t3);
    idx = 0;
    for (typename T::const_reverse_iterator it = t2.rbegin(); it != t2.rend(); ++it) {
        TEST_EQ(t1[idx++], *it);
    }
    t = t3;
}

struct smp_arrays {
    std::array<char,127>    charArray_;
    std::array<int,1000>    intArray_;
};

int array_test() {
    smp_arrays c;
    array_test_1(c.charArray_);
    array_test_1(c.intArray_);
    return 0;
}
