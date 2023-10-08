#include <../ccwrap/ccwrap_misc/ccwrap_test.hpp>
#include <algorithm>
#include <memory>
#if __CCWRAP__ == 2
#include <ccwrap/ccwrap_misc/unique_ptr_cxx03.hpp>
#endif

CCWRAP_TEST_SUITE(unique_ptr_cxx03) {

    CCWRAP_TEST(check) {
     #if __CCWRAP__ == 2
        using namespace ccwrap;
     #else
        using namespace std;
     #endif
        unique_ptr<int> p0;
        ccwrap_test_assert(p0.get() == nullptr);
        ccwrap_test_assert(p0 == nullptr);
        //ccwrap_test_assert(p0 >= nullptr);
        //ccwrap_test_assert(p0 <= nullptr);
        ccwrap_test_assert(nullptr == p0);
        //ccwrap_test_assert(nullptr <= p0);
        //ccwrap_test_assert(nullptr >= p0);
        unique_ptr<int>     p1(new int);
        ccwrap_test_assert(p1.get() != nullptr);
        ccwrap_test_assert(p1 != nullptr);
        ccwrap_test_assert(nullptr != p1);
        ccwrap_test_assert(p1 != p0);
        ccwrap_test_assert(p1 >  p0);
        ccwrap_test_assert(p1 >= p0);
        ccwrap_test_assert(p0 <  p1);
        ccwrap_test_assert(p0 <= p1);

        *p1 = 1; ccwrap_test_eq(*p1, 1);
        p1.reset(new int);
        ccwrap_test_assert(p1.get() != nullptr);
        *p1 = 2; ccwrap_test_eq(*p1, 2);
        int* p1b = p1.release();
        ccwrap_test_assert(p1.get() == nullptr);
        delete p1b;
        //unique_ptr<char[]>    p2(new char[1024]);
        //ccwrap_test_assert(p2.get() != nullptr);
        struct St {
            int a;
            float b;
        };
        unique_ptr<St>  stp(new St);
        stp->a = 10;
        stp->b = 100.0f;
    }
}
