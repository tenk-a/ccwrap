#include "c_test.h"

#include <setjmp.h>

static jmp_buf      g_env;
static volatile int g_reached;

static void leaf_jump(int v) {
    g_reached = v;
    longjmp(g_env, v);
    g_reached = -1;
}

static int run_jump(int v) {
    volatile int stage = 0;
    int r = setjmp(g_env);
    if (r == 0) {
        stage = 1;
        leaf_jump(v);
        stage = 2;
    }
    return (stage == 1) ? r : -1000;
}

static int run_jump_zero(void) {
    int r = setjmp(g_env);
    if (r == 0)
        longjmp(g_env, 0);
    return r;
}

static int run_jump_loop(int n) {
    volatile int count = 0;
    int r = setjmp(g_env);
    if (r != 0)
        count = r;
    if (r < n)
        longjmp(g_env, r + 1);
    return count;
}

TEST_CASE(setjmp, direct_return_is_zero) {
    jmp_buf env;
    int r = setjmp(env);
    test_eq( r, 0 );
    test_pass("c1990:setjmp(jmp_buf) direct return is 0");
}

TEST_CASE(setjmp, delivers_the_value) {
    int r, reached, r2, reached2;

    g_reached = 0;
    r = run_jump(7);
    test_eq( r, 7 );
    reached = g_reached;
    test_eq( reached, 7 );

    g_reached = 0;
    r2 = run_jump(1);
    test_eq( r2, 1 );
    reached2 = g_reached;
    test_eq( reached2, 1 );
    test_pass("c1990:longjmp(jmp_buf,int) delivers the value to setjmp");
}

TEST_CASE(setjmp, zero_becomes_one) {
    int r = run_jump_zero();
    test_eq( r, 1 );
    test_ne( r, 0 );
    test_pass("c1990:longjmp value 0 is replaced by 1");
}

TEST_CASE(setjmp, buf_can_be_reused) {
    int last = run_jump_loop(3);
    jmp_buf a;
    test_true( sizeof(jmp_buf) > 0 );
    test_true( sizeof(a) == sizeof(jmp_buf) );
    test_pass("c1990:jmp_buf");
    test_eq( last, 3 );
    test_pass("c1990:jmp_buf reuse");
}


TEST_CASE(setjmp, version_macro_c2023) {
#if defined(__STDC_VERSION_SETJMP_H__)
    test_true( __STDC_VERSION_SETJMP_H__ >= 202311L );
    test_pass("c2023:__STDC_VERSION_SETJMP_H__");
#else
    TEST_NOTE("C23 header version macro not defined by this library");
    test_skip("c2023:__STDC_VERSION_SETJMP_H__");
#endif
}

TEST_SUITE(setjmp)
    TEST_ADD(direct_return_is_zero)
    TEST_ADD(delivers_the_value)
    TEST_ADD(zero_becomes_one)
    TEST_ADD(buf_can_be_reused)
    TEST_ADD(version_macro_c2023)
TEST_SUITE_END(setjmp)
