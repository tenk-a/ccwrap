#include "test_cxx.hpp"
#include <csignal>

TEST_CASE(csignal, header_compiles) { test_true( true );  test_pass("cxx03:<csignal> header compiles"); }

#include <csignal>
#include <type_traits>

namespace {

typedef void (*sig_handler_t)(int);

volatile STD_NS sig_atomic_t g_caught = 0;
volatile STD_NS sig_atomic_t g_count  = 0;

extern "C" {
static void chk_handler(int sig) {

    g_caught = sig;
    g_count  = g_count + 1;
    STD_NS signal(sig, chk_handler);
}
}

}

TEST_CASE(csignal, sig_atomic_t) {

    test_true( STD::is_integral<STD_NS sig_atomic_t>::value );
    test_true( sizeof(STD_NS sig_atomic_t) >= 1 );

    volatile STD_NS sig_atomic_t v = 0;
    v = 42;
    long got = (long)v;
    test_eq( got, 42L );
    test_pass("cxx03:sig_atomic_t");
}

TEST_CASE(csignal, handler_macros) {

    sig_handler_t h = &chk_handler;

    sig_handler_t dfl = SIG_DFL;
    test_true( h != dfl );
    test_pass("cxx03:SIG_DFL");

    sig_handler_t ign = SIG_IGN;
    test_true( h != ign );
    test_true( dfl != ign );
    test_pass("cxx03:SIG_IGN");

    sig_handler_t err = SIG_ERR;
    test_true( h != err );
    test_true( dfl != err );
    test_true( ign != err );
    test_pass("cxx03:SIG_ERR");
}

TEST_CASE(csignal, signal_numbers) {

    switch (SIGINT) {
    case SIGINT: test_true(true); break;
    default:     test_true(false); break;
    }

    test_gt( SIGABRT, 0 );
    test_ne( SIGABRT, SIGFPE );
    test_pass("cxx03:SIGABRT");

    test_gt( SIGFPE, 0 );
    test_ne( SIGFPE, SIGILL );
    test_pass("cxx03:SIGFPE");

    test_gt( SIGILL, 0 );
    test_ne( SIGILL, SIGINT );
    test_pass("cxx03:SIGILL");

    test_gt( SIGINT, 0 );
    test_ne( SIGINT, SIGSEGV );
    test_pass("cxx03:SIGINT");

    test_gt( SIGSEGV, 0 );
    test_ne( SIGSEGV, SIGTERM );
    test_pass("cxx03:SIGSEGV");

    test_gt( SIGTERM, 0 );
    test_ne( SIGTERM, SIGABRT );
    test_pass("cxx03:SIGTERM");
}

TEST_CASE(csignal, signal_and_raise) {
    g_caught = 0;
    g_count  = 0;

    sig_handler_t prev = STD_NS signal(SIGINT, chk_handler);

    int rc = STD_NS raise(SIGINT);
    test_eq( rc, 0 );

    int caught = (int)g_caught;
    test_eq( caught, SIGINT );
    int count = (int)g_count;
    test_eq( count, 1 );

    STD_NS raise(SIGINT);
    caught = (int)g_caught;
    count  = (int)g_count;
    test_eq( caught, SIGINT );
    test_eq( count, 2 );
    test_pass("cxx03:raise");

    test_true( prev != SIG_ERR );
    sig_handler_t back = STD_NS signal(SIGINT, SIG_DFL);
    test_true( back != SIG_ERR );
    test_true( back == &chk_handler );
    test_pass("cxx03:signal");
}

TEST_CASE(csignal, sig_ign) {
    g_caught = 0;

    sig_handler_t prev = STD_NS signal(SIGTERM, SIG_IGN);
    test_true( prev != SIG_ERR );

    int rc = STD_NS raise(SIGTERM);
    test_eq( rc, 0 );
    int caught = (int)g_caught;
    test_eq( caught, 0 );

    sig_handler_t back = STD_NS signal(SIGTERM, SIG_DFL);
    test_true( back != SIG_ERR );
    test_true( back == SIG_IGN );

    sig_handler_t cur = STD_NS signal(SIGTERM, SIG_DFL);
    test_true( cur == SIG_DFL );
    test_pass("cxx03:signal SIG_IGN behaviour");
}
