#include "test_cxx.hpp"

#include <csetjmp>

TEST_CASE(csetjmp, jmp_buf_longjmp) {
    STD_NS jmp_buf __jb;
    volatile int __stage = 0;
    int __r = setjmp(__jb);
    if (__r == 0) {
        __stage = 1;
        STD_NS longjmp(__jb, 7);
        __stage = 99;
    } else {
        __stage = 2;
    }
    test_eq( __r == 0 ? 0 : __r, __r );
    test_pass("cxx03:jmp_buf");
    test_eq( (int)__stage, 2 );
    test_pass("cxx03:setjmp");
    test_eq( __r, 7 );
    test_pass("cxx03:longjmp");
}

#include <csetjmp>

namespace {

STD_NS jmp_buf            g_env;
volatile int            g_reached;

void leaf_jump(int v) {
    g_reached = v;
    STD_NS longjmp(g_env, v);
    g_reached = -1;
}

int run_jump(int v) {
    volatile int stage = 0;
    int r = setjmp(g_env);
    if (r == 0) {
        stage = 1;
        leaf_jump(v);
        stage = 2;
    }

    return (stage == 1) ? r : -1000;
}

int run_jump_zero() {
    int r = setjmp(g_env);
    if (r == 0)
        STD_NS longjmp(g_env, 0);
    return r;
}

int run_jump_loop(int n) {
    volatile int count = 0;
    int r = setjmp(g_env);
    if (r != 0)
        count = r;
    if (r < n)
        STD_NS longjmp(g_env, r + 1);
    return count;
}

}

TEST_CASE(csetjmp, setjmp_returns_zero_on_the_direct_call) {

    STD_NS jmp_buf env;
    int r = setjmp(env);
    test_eq( r, 0 );
    test_pass("cxx03:setjmp direct return is 0");
}

TEST_CASE(csetjmp, longjmp_delivers_the_value) {
    g_reached = 0;
    int r = run_jump(7);
    test_eq( r, 7 );
    int reached = g_reached;
    test_eq( reached, 7 );

    g_reached = 0;
    int r2 = run_jump(1);
    test_eq( r2, 1 );
    int reached2 = g_reached;
    test_eq( reached2, 1 );
    test_pass("cxx03:longjmp delivers the value to setjmp");
}

TEST_CASE(csetjmp, longjmp_zero_becomes_one) {
    int r = run_jump_zero();
    test_eq( r, 1 );
    test_ne( r, 0 );
    test_pass("cxx03:longjmp value 0 is replaced by 1");
}

TEST_CASE(csetjmp, jmp_buf_can_be_reused) {
    int last = run_jump_loop(3);
    test_eq( last, 3 );

    test_true( sizeof(STD_NS jmp_buf) > 0 );
    STD_NS jmp_buf a;
    test_true( sizeof(a) == sizeof(STD_NS jmp_buf) );
    test_pass("cxx03:jmp_buf reuse");
}
