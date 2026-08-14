#include "c_test.h"

#include <signal.h>

typedef void (*sig_handler_t)(int);

static volatile sig_atomic_t g_caught = 0;
static volatile sig_atomic_t g_count  = 0;

static void chk_handler(int sig) {

    g_caught = sig;
    g_count  = g_count + 1;
    signal(sig, chk_handler);
}

TEST_CASE(signal, sig_atomic_type) {
    volatile sig_atomic_t v = 0;
    long got;
    test_true( sizeof(sig_atomic_t) >= 1 );
    v = 42;
    got = (long)v;
    test_eq( got, 42L );
    test_pass("c1990:sig_atomic_t");
}

TEST_CASE(signal, handler_macros) {
    sig_handler_t dfl = SIG_DFL;
    sig_handler_t ign = SIG_IGN;
    sig_handler_t err = SIG_ERR;
    sig_handler_t h   = &chk_handler;

    test_true( dfl != ign ); test_true( dfl != err ); test_true( h != dfl );
    test_pass("c1990:SIG_DFL");
    test_true( ign != err ); test_true( h != ign );
    test_pass("c1990:SIG_IGN");
    test_true( h != err );
    test_pass("c1990:SIG_ERR");
}

TEST_CASE(signal, signal_numbers) {
    int sigs[6];
    int i, j;
    sigs[0] = SIGABRT; sigs[1] = SIGFPE;  sigs[2] = SIGILL;
    sigs[3] = SIGINT;  sigs[4] = SIGSEGV; sigs[5] = SIGTERM;

    for (i = 0; i < 6; ++i) {
        test_gt( sigs[i], 0 );
        for (j = i + 1; j < 6; ++j)
            test_ne( sigs[i], sigs[j] );
    }
    test_gt( SIGABRT, 0 ); test_pass("c1990:SIGABRT");
    test_gt( SIGFPE,  0 ); test_pass("c1990:SIGFPE");
    test_gt( SIGILL,  0 ); test_pass("c1990:SIGILL");

    switch (SIGINT) { case SIGINT: test_true(1); break; default: test_true(0); break; }
    test_pass("c1990:SIGINT");
    test_gt( SIGSEGV, 0 ); test_pass("c1990:SIGSEGV");
    test_gt( SIGTERM, 0 ); test_pass("c1990:SIGTERM");
}

TEST_CASE(signal, install_and_raise) {
    sig_handler_t prev, back;
    int rc, caught, count;

    g_caught = 0;
    g_count  = 0;

    prev = signal(SIGINT, chk_handler);
    test_true( prev != SIG_ERR );

    rc = raise(SIGINT);
    test_eq( rc, 0 );
    caught = (int)g_caught;
    test_eq( caught, SIGINT );
    count = (int)g_count;
    test_eq( count, 1 );
    raise(SIGINT);
    count  = (int)g_count;
    test_eq( count, 2 );
    test_pass("c1990:raise(int)");

    back = signal(SIGINT, SIG_DFL);
    test_true( prev != SIG_ERR );
    test_true( back != SIG_ERR );
    test_true( back == &chk_handler );
    test_pass("c1990:signal(int,handler)");
}

TEST_CASE(signal, ignored_signal) {
    sig_handler_t prev, back;
    int rc, caught;

    g_caught = 0;

    prev = signal(SIGTERM, SIG_IGN);
    test_true( prev != SIG_ERR );

    rc = raise(SIGTERM);
    test_eq( rc, 0 );
    caught = (int)g_caught;
    test_eq( caught, 0 );

    back = signal(SIGTERM, SIG_DFL);
    test_true( back != SIG_ERR );
    test_true( back == SIG_IGN );
    test_pass("c1990:signal SIG_IGN behaviour");
}


TEST_CASE(signal, restore_default_c1990) {
    void (*prev)(int);
    void (*back)(int);

    prev = signal(SIGABRT, SIG_IGN);
    test_true( prev != SIG_ERR );

    back = signal(SIGABRT, SIG_DFL);
    test_true( back == SIG_IGN );

    back = signal(SIGABRT, SIG_DFL);
    test_true( back == SIG_DFL );
    test_pass("c1990:signal(sig, SIG_DFL) restores the default disposition");
}

TEST_SUITE(signal)
    TEST_ADD(sig_atomic_type)
    TEST_ADD(handler_macros)
    TEST_ADD(signal_numbers)
    TEST_ADD(install_and_raise)
    TEST_ADD(ignored_signal)
    TEST_ADD(restore_default_c1990)
TEST_SUITE_END(signal)
