// #include <_ccwrap/ccwrap_common.h>
#include <_ccwrap/ccwrap_test.hpp>
#include <stdio.h>

#include "assert_ex_test.hh"

void assert_ex_test();

int main()
{
    std::printf("CCWRAP 'test' & 'assert_ex' test.\n");
    CCWRAP_TEST_RUN();
    assert_ex_test();
    return 0;
}
