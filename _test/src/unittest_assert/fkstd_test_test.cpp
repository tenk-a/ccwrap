//#include <_ccwrap/ccwrap_common.h>
#include <_ccwrap/ccwrap_test.hpp>

CCWRAP_TEST_SUITE(ccwrap_test) {

    bool foo() {return true;}
    bool bar() { throw "error"; return false;}

    CCWRAP_TEST(ok) {
        ccwrap_test_assert(true);
        ccwrap_test_assert(foo());
        ccwrap_test_ptr("test");
        ccwrap_test_ptr0("test");
        ccwrap_test_ptr0(NULL);
        int x = 10;
        ccwrap_test_range(x,1,20);
        std::string a = "abc", b = "def";
        ccwrap_test_range(a, "012", "xyz");
        static char const tbl[] = "0123456789abcdefghij";
        ccwrap_test_range(&tbl[1], &tbl[0], &tbl[19]);

        ccwrap_test_eq(x, 10);
        ccwrap_test_ne(x, 11);
        ccwrap_test_lt(x, 11);
        ccwrap_test_le(x, 11);
        ccwrap_test_le(x, 10);
        ccwrap_test_gt(x,  5);
        ccwrap_test_ge(x,  5);
        ccwrap_test_ge(x, 10);

        ccwrap_test_eq(a, "abc");
        ccwrap_test_ne(a, b);
        ccwrap_test_lt(a, b);
        ccwrap_test_le(a, b);
        ccwrap_test_le(a, "abc");
        ccwrap_test_gt(b, "012");
        ccwrap_test_ge(b, "012");
        ccwrap_test_ge(b, "def");

        ccwrap_test_throw(bar());
        ccwrap_test_no_throw(foo());
    }

    CCWRAP_TEST(ng) {
        using namespace __ccwrap;
        CCWRAP_TEST_PRINTF("---- !!!! Purposely error !!!! ---- BEGIN\n");
        ccwrap_test_assert(false);
        ccwrap_test_ptr(NULL);
        ccwrap_test_ptr0((char const*)1);
        int x = 10;
        ccwrap_test_range(x,1,9);
        std::string a = "abc", b = "def";
        ccwrap_test_range(a, b, "xyz");
        static char const tbl[] = "0123456789abcdefghij";
        ccwrap_test_range(&tbl[0], &tbl[1], &tbl[19]);

        ccwrap_test_eq(x, 11);
        ccwrap_test_ne(x, 10);
        ccwrap_test_lt(x, 10);
        ccwrap_test_lt(x,  9);
        ccwrap_test_le(x,  9);
        ccwrap_test_gt(x, 10);
        ccwrap_test_gt(x, 11);
        ccwrap_test_ge(x, 11);

        ccwrap_test_throw(foo());
        ccwrap_test_no_throw(throw "1");
        CCWRAP_TEST_PRINTF("---- !!!! Purposely error !!!! ---- END\n");
    }
}
