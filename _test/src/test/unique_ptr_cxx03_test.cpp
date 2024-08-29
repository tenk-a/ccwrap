#include <../ccwrap/ccwrap_misc/test.hpp>
#include <algorithm>
#include <memory>
#if __CCWRAP__ == 2
#include <ccwrap/ccwrap_misc/unique_ptr_cxx03.hpp>
#endif

TEST_SUITE(unique_ptr_cxx03) {}

struct St {
    int   a;
    float b;
};

TEST_CASE(unique_ptr_cxx03, check) {
    using namespace std;
    unique_ptr<int> p0;
    test_true(p0.get() == nullptr);
    test_true(p0 == nullptr);
    //test_true(p0 >= nullptr);
    //test_true(p0 <= nullptr);
    test_true(nullptr == p0);
    //test_true(nullptr <= p0);
    //test_true(nullptr >= p0);
    unique_ptr<int>     p1(new int);
    test_true(p1.get() != nullptr);
    test_true(p1 != nullptr);
    test_true(nullptr != p1);
    test_true(p1 != p0);
    test_true(p1 >  p0);
    test_true(p1 >= p0);
    test_true(p0 <  p1);
    test_true(p0 <= p1);

    *p1 = 1; test_eq(*p1, 1);
    p1.reset(new int);
    test_true(p1.get() != nullptr);
    *p1 = 2; test_eq(*p1, 2);
    int* p1b = p1.release();
    test_true(p1.get() == nullptr);
    delete p1b;
    //unique_ptr<char[]>    p2(new char[1024]);
    //test_true(p2.get() != nullptr);
    unique_ptr<St>  stp(new St);
    stp->a = 10;
    stp->b = 100.0f;
}
