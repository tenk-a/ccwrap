/**
 *  @file   test.hpp
 *  @brief  ccwrap_test to test wrap
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @date   2020
 *  @license Boost Software License Version 1.0
 */
#ifndef __TEST_HPP_INCLUDED
#define __TEST_HPP_INCLUDED

#include "ccwrap_test.hpp"

#define TEST_SUITE(name)                CCWRAP_TEST_SUITE(name)
#define TEST(name)                      CCWRAP_TEST(name)
#define TEST_RUN()                      CCWRAP_TEST_RUN()
#define test_assert(p)                  ccwrap_test_assert(p)
#define test(p)                         ccwrap_test(p)
#define test_ptr(p)                     ccwrap_test_ptr(p)
#define test_ptr0(p)                    ccwrap_test_ptr0(p)
#define test_range(x,a,b)               ccwrap_test_range(x,a,b)
#define test_eq(l,r)                    ccwrap_test_eq(l,r)
#define test_ne(l,r)                    ccwrap_test_ne(l,r)
#define test_lt(l,r)                    ccwrap_test_lt(l,r)
#define test_le(l,r)                    ccwrap_test_le(l,r)
#define test_gt(l,r)                    ccwrap_test_gt(l,r)
#define test_ge(l,r)                    ccwrap_test_ge(l,r)
#define test_throw(x)                   ccwrap_test_throw(x)
#define test_no_throw(x)                ccwrap_test_no_throw(x)
#define TEST_PRINTF(...)                CCWRAP_TEST_PRINTF(...)

#endif
