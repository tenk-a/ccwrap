//#include <_ccwrap/ccwrap_common.h>
#include <ccwrap_misc/test.hpp>

TEST_SUITE(ccwrap_test_check) {}

bool foo() {return true;}
bool bar() { throw "error"; return false;}

TEST_CASE(ccwrap_test_check, ok) {
    test_true(true);
    test_true(foo());
    test_ptr("test");
    test_ptr0("test");
    test_ptr0(NULL);
    int x = 10;
    test_lim(x,1,20);
    std::string a = "abc", b = "def";
    test_lim(a, "012", "xyz");
    static char const tbl[] = "0123456789abcdefghij";
    test_lim(&tbl[1], &tbl[0], &tbl[19]);

    test_eq(x, 10);
    test_ne(x, 11);
    test_lt(x, 11);
    test_le(x, 11);
    test_le(x, 10);
    test_gt(x,  5);
    test_ge(x,  5);
    test_ge(x, 10);

    test_eq(a, "abc");
    test_ne(a, b);
    test_lt(a, b);
    test_le(a, b);
    test_le(a, "abc");
    test_gt(b, "012");
    test_ge(b, "012");
    test_ge(b, "def");

    test_throw(bar());
    test_no_throw(foo());
}

TEST_CASE(ccwrap_test_check, ng) {
    using namespace __ccwrap;
    CCWRAP_TEST_PRINTF("---- !!!! Purposely error !!!! ---- BEGIN\n");
    test_true(false);
    test_ptr(NULL);
    test_ptr0((char const*)1);
    int x = 10;
    test_lim(x,1,9);
    std::string a = "abc", b = "def";
    test_lim(a, b, "xyz");
    static char const tbl[] = "0123456789abcdefghij";
    test_lim(&tbl[0], &tbl[1], &tbl[19]);

    test_eq(x, 11);
    test_ne(x, 10);
    test_lt(x, 10);
    test_lt(x,  9);
    test_le(x,  9);
    test_gt(x, 10);
    test_gt(x, 11);
    test_ge(x, 11);

    test_throw(foo());
    test_no_throw(throw "1");
    CCWRAP_TEST_PRINTF("---- !!!! Purposely error !!!! ---- END\n");
}
