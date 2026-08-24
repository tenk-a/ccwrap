#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <atomic>
#if _TST_HAS_OS_THREADS
#include <thread>
#endif
#include <memory>
#include <type_traits>
#include <cstddef>

namespace {

struct Point {
    int x;
    int y;
};

inline bool operator==(Point const& a, Point const& b) { return a.x == b.x && a.y == b.y; }

}

#if _TST_ATOMIC_LIB_CXX20
struct FlagWaitThenCount {
    STD::atomic_flag* h; STD::atomic<int>* woken;
    FlagWaitThenCount(STD::atomic_flag& a, STD::atomic<int>& b) : h(&a), woken(&b) {}
    void operator()() const { h->wait(false); woken->fetch_add(1); }
};

struct FlagWaitThenSet {
    STD::atomic_flag* g; STD::atomic<bool>* done;
    FlagWaitThenSet(STD::atomic_flag& a, STD::atomic<bool>& b) : g(&a), done(&b) {}
    void operator()() const { g->wait(false); done->store(true); }
};

struct WaitThenCopy {
    STD::atomic<int>* v; STD::atomic<int>* seen;
    WaitThenCopy(STD::atomic<int>& a, STD::atomic<int>& b) : v(&a), seen(&b) {}
    void operator()() const { v->wait(0); seen->store(v->load()); }
};
struct WaitThenCount {
    STD::atomic<int>* w; STD::atomic<int>* woken; bool with_order;
    WaitThenCount(STD::atomic<int>& a, STD::atomic<int>& b, bool o) : w(&a), woken(&b), with_order(o) {}
    void operator()() const {
        if (with_order) w->wait(0, STD::memory_order_seq_cst); else w->wait(0);
        woken->fetch_add(1);
    }
};
#endif

TEST_CASE(atomic, member_types_and_traits) {
    typedef STD::atomic<int> AI;

#if _TST_ATOMIC_LIB_CXX17
    static_assert((STD::is_same<AI::value_type, int>::value), "");
    static_assert((STD::is_same<AI::difference_type, int>::value), "");
    static_assert((STD::is_same<STD::atomic<bool>::value_type, bool>::value), "");
    static_assert((STD::is_same<STD::atomic<Point>::value_type, Point>::value), "");
    static_assert((STD::is_same<STD::atomic<int*>::value_type, int*>::value), "");
    static_assert((STD::is_same<STD::atomic<int*>::difference_type, STD::ptrdiff_t>::value), "");
#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_atomic_float)
    static_assert((STD::is_same<STD::atomic<double>::difference_type, double>::value), "");
#endif
#endif
    test_true( 1 );
    test_pass("cxx11:atomic<T>::value_type");
    test_true( 1 );
    test_pass("cxx11:atomic<T>::difference_type");

#if _TST_ATOMIC_LIB_CXX17
    static_assert(AI::is_always_lock_free, "");
    test_true( AI::is_always_lock_free );
    test_pass("cxx17:atomic<int>::is_always_lock_free");
#else
    TEST_SKIP1();
    test_skip("cxx17:atomic<int>::is_always_lock_free");
#endif
#if _TST_ATOMIC_LIB_CXX17
    TEST_SKIP_WAT();
    TEST_SKIP_VC090("vc8/9 take <atomic> from libcxx03 too -- same generic fallback");
    test_true( STD::atomic<bool>::is_always_lock_free );
    test_pass("cxx17:atomic<bool>::is_always_lock_free");
#else
    TEST_SKIP1();
    test_skip("cxx17:atomic<bool>::is_always_lock_free");
#endif
#if _TST_ATOMIC_LIB_CXX17
    test_true( STD::atomic<int*>::is_always_lock_free );
    test_pass("cxx17:atomic<int*>::is_always_lock_free");
#else
    TEST_SKIP1();
    test_skip("cxx17:atomic<int*>::is_always_lock_free");
#endif

#if _TST_HAS_CXX11_LIB_MEMBERS && !defined(__WATCOMC__) && !(defined(_MSC_VER) && _MSC_VER < 1900)
    static_assert(!STD::is_copy_constructible<AI>::value, "");
    static_assert(!STD::is_copy_constructible<STD::atomic<Point> >::value, "");
    static_assert(!STD::is_copy_assignable<AI>::value, "");
    static_assert(!STD::is_copy_assignable<STD::atomic<Point> >::value, "");
#endif
    test_true( 1 );
    test_pass("cxx11:atomic<T> is not copyable");

    AI d;
#if TEST_TARGET_CXX >= 2020
    int dv = d.load();
    test_eq( dv, 0 );
    test_pass("cxx11:atomic<int> default ctor");
#else
    TEST_NOTE("value-initialisation by the default ctor is C++20 (P0883)");
    TEST_SKIP1();
    test_skip("cxx11:atomic<int> default ctor");
#endif

    AI a(7);
    int av = a.load();
    test_eq( av, 7 );
    test_pass("cxx11:atomic<int>(T)");

    test_true( a.is_lock_free() );
    test_pass("cxx11:atomic<int>::is_lock_free");
}

TEST_CASE(atomic, int_load_store_exchange) {
    STD::atomic<int> a(1);

    a.store(5);
    test_true( 1 );
    test_pass("cxx11:atomic<int>::store");
    int v1 = a.load();
    test_eq( v1, 5 );
    test_pass("cxx11:atomic<int>::load");

    a.store(6, STD::memory_order_release);
    test_true( 1 );
    test_pass("cxx11:atomic<int>::store (memory_order)");
    int v2 = a.load(STD::memory_order_acquire);
    test_eq( v2, 6 );
    test_pass("cxx11:atomic<int>::load (memory_order)");

    int assigned = (a = 11);
    test_eq( assigned, 11 );
    test_pass("cxx11:atomic<int>::operator=(T)");

    int conv = a;
    test_eq( conv, 11 );
    test_pass("cxx11:atomic<int>::operator T");

    int old = a.exchange(20);
    test_eq( old, 11 );
    int now = a.load();
    test_eq( now, 20 );
    test_pass("cxx11:atomic<int>::exchange");

    int old2 = a.exchange(21, STD::memory_order_acq_rel);
    test_eq( old2, 20 );
    int now2 = a.load(STD::memory_order_relaxed);
    test_eq( now2, 21 );
    test_pass("cxx11:atomic<int>::exchange (memory_order)");
}

TEST_CASE(atomic, int_compare_exchange) {
    STD::atomic<int> a(10);

    int e1 = 10;
    bool ok1 = a.compare_exchange_strong(e1, 11);
    test_true( ok1 );
    test_eq( e1, 10 );
    int v1 = a.load();
    test_eq( v1, 11 );
    test_pass("cxx11:atomic<int>::compare_exchange_strong");

    int e2 = 99;
    bool no = a.compare_exchange_strong(e2, 12);
    test_true( !no );
    test_eq( e2, 11 );
    int v2 = a.load();
    test_eq( v2, 11 );
    test_pass("cxx11:atomic<int>::compare_exchange_strong (fail refreshes expected)");

    int e3 = 11;
    bool ok3 = a.compare_exchange_strong(e3, 13, STD::memory_order_seq_cst);
    test_true( ok3 );
    test_pass("cxx11:atomic<int>::compare_exchange_strong (3-arg)");

    int e4 = 13;
    bool ok4 = a.compare_exchange_strong(e4, 14,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok4 );
    int v4 = a.load();
    test_eq( v4, 14 );
    test_pass("cxx11:atomic<int>::compare_exchange_strong (4-arg)");

    int e5 = 14;
    while (!a.compare_exchange_weak(e5, 15)) { }
    int v5 = a.load();
    test_eq( v5, 15 );
    test_pass("cxx11:atomic<int>::compare_exchange_weak");

    int e6 = 15;
    while (!a.compare_exchange_weak(e6, 16, STD::memory_order_seq_cst)) { }
    int v6 = a.load();
    test_eq( v6, 16 );
    test_pass("cxx11:atomic<int>::compare_exchange_weak (3-arg)");

    int e7 = 16;
    while (!a.compare_exchange_weak(e7, 17,
               STD::memory_order_acq_rel, STD::memory_order_relaxed)) { }
    int v7 = a.load();
    test_eq( v7, 17 );
    test_pass("cxx11:atomic<int>::compare_exchange_weak (4-arg)");

    int e8 = 0;
    bool nw = a.compare_exchange_weak(e8, 18);
    test_true( !nw );
    test_eq( e8, 17 );
    test_pass("cxx11:atomic<int>::compare_exchange_weak (fail refreshes expected)");
}

TEST_CASE(atomic, int_fetch_ops) {
    STD::atomic<int> a(0);

    int f1 = a.fetch_add(5);
    test_eq( f1, 0 );
    test_pass("cxx11:atomic<int>::fetch_add");
    int f2 = a.fetch_add(5, STD::memory_order_relaxed);
    test_eq( f2, 5 );
    int v1 = a.load();
    test_eq( v1, 10 );
    test_pass("cxx11:atomic<int>::fetch_add (memory_order)");

    int f3 = a.fetch_sub(3);
    test_eq( f3, 10 );
    test_pass("cxx11:atomic<int>::fetch_sub");
    int f4 = a.fetch_sub(3, STD::memory_order_acq_rel);
    test_eq( f4, 7 );
    int v2 = a.load();
    test_eq( v2, 4 );
    test_pass("cxx11:atomic<int>::fetch_sub (memory_order)");

    a.store(0xF0);
    int g1 = a.fetch_and(0x3C);
    test_eq( g1, 0xF0 );
    test_pass("cxx11:atomic<int>::fetch_and");
    int g2 = a.fetch_and(0x10, STD::memory_order_seq_cst);
    test_eq( g2, 0x30 );
    int v3 = a.load();
    test_eq( v3, 0x10 );
    test_pass("cxx11:atomic<int>::fetch_and (memory_order)");

    int h1 = a.fetch_or(0x01);
    test_eq( h1, 0x10 );
    test_pass("cxx11:atomic<int>::fetch_or");
    int h2 = a.fetch_or(0x02, STD::memory_order_release);
    test_eq( h2, 0x11 );
    int v4 = a.load();
    test_eq( v4, 0x13 );
    test_pass("cxx11:atomic<int>::fetch_or (memory_order)");

    int x1 = a.fetch_xor(0x03);
    test_eq( x1, 0x13 );
    test_pass("cxx11:atomic<int>::fetch_xor");
    int x2 = a.fetch_xor(0xFF, STD::memory_order_acquire);
    test_eq( x2, 0x10 );
    int v5 = a.load();
    test_eq( v5, 0xEF );
    test_pass("cxx11:atomic<int>::fetch_xor (memory_order)");
}

TEST_CASE(atomic, int_operators) {
    STD::atomic<int> a(0);

    int pre = ++a;
    test_eq( pre, 1 );
    test_pass("cxx11:atomic<int>::operator++ (pre)");
    int post = a++;
    test_eq( post, 1 );
    int v1 = a.load();
    test_eq( v1, 2 );
    test_pass("cxx11:atomic<int>::operator++ (post)");

    int predec = --a;
    test_eq( predec, 1 );
    test_pass("cxx11:atomic<int>::operator-- (pre)");
    int postdec = a--;
    test_eq( postdec, 1 );
    int v2 = a.load();
    test_eq( v2, 0 );
    test_pass("cxx11:atomic<int>::operator-- (post)");

    int pa = (a += 10);
    test_eq( pa, 10 );
    test_pass("cxx11:atomic<int>::operator+=");
    int ps = (a -= 4);
    test_eq( ps, 6 );
    test_pass("cxx11:atomic<int>::operator-=");

    a.store(0xF0);
    int an = (a &= 0x3C);
    test_eq( an, 0x30 );
    test_pass("cxx11:atomic<int>::operator&=");
    int orr = (a |= 0x0F);
    test_eq( orr, 0x3F );
    test_pass("cxx11:atomic<int>::operator|=");
    int xr = (a ^= 0xFF);
    test_eq( xr, 0xC0 );
    int v3 = a.load();
    test_eq( v3, 0xC0 );
    test_pass("cxx11:atomic<int>::operator^=");

    STD::atomic<unsigned long long> u(0);
    unsigned long long uf = u.fetch_add(1ull << 40);
    test_eq( uf, 0ull );
    unsigned long long uv = u.load();
    test_eq( uv, 1ull << 40 );
    test_pass("cxx11:atomic<unsigned long long>::fetch_add");
    unsigned long long ux = (u ^= (1ull << 40));
    test_eq( ux, 0ull );
    test_pass("cxx11:atomic<unsigned long long>::operator^=");
}

TEST_CASE(atomic, bool_specialization) {
    STD::atomic<bool> b(false);
    test_true( b.is_lock_free() );
    test_pass("cxx11:atomic<bool>::is_lock_free");

    b.store(true);
    test_true( 1 );
    test_pass("cxx11:atomic<bool>::store");
    bool v = b.load();
    test_true( v );
    test_pass("cxx11:atomic<bool>::load");

    b.store(false, STD::memory_order_release);
    test_true( 1 );
    test_pass("cxx11:atomic<bool>::store (memory_order)");
    bool v2 = b.load(STD::memory_order_acquire);
    test_true( !v2 );
    test_pass("cxx11:atomic<bool>::load (memory_order)");

    bool old = b.exchange(true, STD::memory_order_seq_cst);
    test_true( !old );
    bool now = b.load();
    test_true( now );
    test_pass("cxx11:atomic<bool>::exchange");

    bool e = true;
    bool ok = b.compare_exchange_strong(e, false);
    test_true( ok );
    test_pass("cxx11:atomic<bool>::compare_exchange_strong");
    bool cur = b;
    test_true( !cur );
    test_pass("cxx11:atomic<bool>::operator bool");

    bool e2 = false;
    while (!b.compare_exchange_weak(e2, true,
               STD::memory_order_acq_rel, STD::memory_order_acquire)) { }
    bool cur2 = b.load();
    test_true( cur2 );
    test_pass("cxx11:atomic<bool>::compare_exchange_weak");

    bool assigned = (b = false);
    test_true( !assigned );
    bool cur3 = b.load();
    test_true( !cur3 );
    test_pass("cxx11:atomic<bool>::operator=(bool)");
}

TEST_CASE(atomic, floating_specialization) {
    STD::atomic<double> d(0.0);
    double d1 = d.exchange(2.25, STD::memory_order_acq_rel);
    test_eq( d1, 0.0 );
    test_pass("cxx20:atomic<double>::exchange");

    double e = 2.25;
    bool ok = d.compare_exchange_strong(e, 4.5, STD::memory_order_seq_cst);
    test_true( ok );
    double dv = d.load(STD::memory_order_relaxed);
    test_eq( dv, 4.5 );
    test_pass("cxx20:atomic<double>::compare_exchange_strong");

    double e2 = 4.5;
    while (!d.compare_exchange_weak(e2, 8.0)) { }
    double dv2 = d.load();
    test_eq( dv2, 8.0 );
    test_pass("cxx20:atomic<double>::compare_exchange_weak");

    STD::atomic<float> f(1.0f);
    f.store(3.5f);
    float fv = f.load();
    test_eq( fv, 3.5f );
    test_pass("cxx11:atomic<float>::load()");
    test_pass("cxx11:atomic<float>::store(value)");
}

#if _TST_ATOMIC_LIB_CXX20
TEST_CASE(atomic, floating_specialization_cxx20) {
    STD::atomic<float> f(1.0f);

    float o1 = f.fetch_add(0.5f);
    test_eq( o1, 1.0f );
    float v1 = f.load();
    test_eq( v1, 1.5f );
    test_pass("cxx20:atomic<float>::fetch_add");

    float o2 = f.fetch_sub(0.5f, STD::memory_order_relaxed);
    test_eq( o2, 1.5f );
    float v2 = f.load();
    test_eq( v2, 1.0f );
    test_pass("cxx20:atomic<float>::fetch_sub");

    float p = (f += 2.0f);
    test_eq( p, 3.0f );
    test_pass("cxx20:atomic<float>::operator+=");
    float m = (f -= 1.0f);
    test_eq( m, 2.0f );
    test_pass("cxx20:atomic<float>::operator-=");

    STD::atomic<double> d(8.0);
    double da = d.fetch_add(0.5, STD::memory_order_acq_rel);
    test_eq( da, 8.0 );
    test_pass("cxx20:atomic<double>::fetch_add");
    double ds = d.fetch_sub(0.5);
    test_eq( ds, 8.5 );
    double dv3 = d.load();
    test_eq( dv3, 8.0 );
    test_pass("cxx20:atomic<double>::fetch_sub");

    double q = (d += 1.5);
    test_eq( q, 9.5 );
    double r = (d -= 0.5);
    test_eq( r, 9.0 );
    test_pass("cxx20:atomic<double>::operator+=(value)");
    test_pass("cxx20:atomic<double>::operator-=(value)");

    double conv = d;
    test_eq( conv, 9.0 );
    d.store(2.5);
    test_eq( (double)d, 2.5 );
    test_pass("cxx20:atomic<double>::operator T");
}
#else
TEST_CASE_SKIP(atomic, floating_specialization_cxx20)
#endif

TEST_CASE(atomic, pointer_specialization) {
    static int arr[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    STD::atomic<int*> p(arr);
    test_true( p.is_lock_free() );
    test_pass("cxx11:atomic<int*>::is_lock_free");

    int* l = p.load();
    test_true( l == arr );
    test_pass("cxx11:atomic<int*>::load");

    p.store(arr + 1, STD::memory_order_release);
    int* l2 = p.load(STD::memory_order_acquire);
    test_eq( l2 - arr, STD::ptrdiff_t(1) );
    test_pass("cxx11:atomic<int*>::store (memory_order)");

    int* old = p.exchange(arr + 2);
    test_eq( old - arr, STD::ptrdiff_t(1) );
    test_pass("cxx11:atomic<int*>::exchange");
    int* old2 = p.exchange(arr + 2, STD::memory_order_acq_rel);
    test_eq( old2 - arr, STD::ptrdiff_t(2) );
    test_pass("cxx11:atomic<int*>::exchange (memory_order)");

    int* e = arr + 2;
    bool ok = p.compare_exchange_strong(e, arr + 3);
    test_true( ok );
    test_pass("cxx11:atomic<int*>::compare_exchange_strong");
    int* e2 = arr + 3;
    bool ok2 = p.compare_exchange_strong(e2, arr + 4,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok2 );
    test_pass("cxx11:atomic<int*>::compare_exchange_strong (orders)");
    int* e3 = arr + 4;
    while (!p.compare_exchange_weak(e3, arr + 4, STD::memory_order_seq_cst)) { }
    int* cur = p.load();
    test_eq( cur - arr, STD::ptrdiff_t(4) );
    test_pass("cxx11:atomic<int*>::compare_exchange_weak");

    int* fa = p.fetch_add(2);
    test_eq( fa - arr, STD::ptrdiff_t(4) );
    int* v1 = p.load();
    test_eq( v1 - arr, STD::ptrdiff_t(6) );
    test_pass("cxx11:atomic<int*>::fetch_add");

    int* fs = p.fetch_sub(1, STD::memory_order_relaxed);
    test_eq( fs - arr, STD::ptrdiff_t(6) );
    test_pass("cxx11:atomic<int*>::fetch_sub");

    int* inc = ++p;
    test_eq( inc - arr, STD::ptrdiff_t(6) );
    test_pass("cxx11:atomic<int*>::operator++ (pre)");
    int* pst = p++;
    test_eq( pst - arr, STD::ptrdiff_t(6) );
    test_pass("cxx11:atomic<int*>::operator++ (post)");
    int* dec = --p;
    test_eq( dec - arr, STD::ptrdiff_t(6) );
    test_pass("cxx11:atomic<int*>::operator-- (pre)");
    int* pdc = p--;
    test_eq( pdc - arr, STD::ptrdiff_t(6) );
    int* v2 = p.load();
    test_eq( v2 - arr, STD::ptrdiff_t(5) );
    test_pass("cxx11:atomic<int*>::operator-- (post)");

    int* pe = (p += 2);
    test_eq( pe - arr, STD::ptrdiff_t(7) );
    test_pass("cxx11:atomic<int*>::operator+=");
    int* me = (p -= 3);
    test_eq( me - arr, STD::ptrdiff_t(4) );
    test_pass("cxx11:atomic<int*>::operator-=");

    int* conv = p;
    test_eq( conv - arr, STD::ptrdiff_t(4) );
    test_pass("cxx11:atomic<int*>::operator T");

    int* assigned = (p = arr);
    test_true( assigned == arr );
    test_pass("cxx11:atomic<int*>::operator=(T*)");
}

TEST_CASE(atomic, primary_template_struct) {
    typedef STD::atomic<Point> AP;

    AP a;
#if TEST_TARGET_CXX >= 2020
    Point zero = a.load();
    TEST_SKIP_WAT("Open Watcom does not value-initialise an aggregate: T() and a "
                  "mem-initialiser v_() both leave the members as they were (A37)");
    test_eq( zero.x, 0 );
    TEST_SKIP_WAT("Open Watcom does not value-initialise an aggregate: T() and a "
                  "mem-initialiser v_() both leave the members as they were (A37)");
    test_eq( zero.y, 0 );
    test_pass("cxx11:atomic<struct> default ctor (value-init)");
#else
    TEST_NOTE("value-initialisation by the default ctor is C++20 (P0883)");
    TEST_SKIP_N(2);
    test_skip("cxx11:atomic<struct> default ctor (value-init)");
#endif

    Point p12 = { 1, 2 };
    a.store(p12);
    test_true( 1 );
    test_pass("cxx11:atomic<struct>::store");
    Point v = a.load();
    test_eq( v.x, 1 );
    test_eq( v.y, 2 );
    test_pass("cxx11:atomic<struct>::load");

    Point p34 = { 3, 4 };
    a.store(p34, STD::memory_order_release);
    test_true( 1 );
    test_pass("cxx11:atomic<struct>::store (memory_order)");
    Point v2 = a.load(STD::memory_order_acquire);
    test_true( v2 == p34 );
    test_pass("cxx11:atomic<struct>::load (memory_order)");

    Point p56 = { 5, 6 };
    Point old = a.exchange(p56, STD::memory_order_acq_rel);
    test_true( old == p34 );
    test_pass("cxx11:atomic<struct>::exchange");

    Point exp = p56;
    Point p78 = { 7, 8 };
    bool ok = a.compare_exchange_strong(exp, p78);
    test_true( ok );
    Point v3 = a.load();
    test_true( v3 == p78 );
    test_pass("cxx11:atomic<struct>::compare_exchange_strong");

    Point bad = { 0, 0 };
    bool no = a.compare_exchange_strong(bad, p12, STD::memory_order_seq_cst);
    test_true( !no );
    test_true( bad == p78 );
    test_pass("cxx11:atomic<struct> failing CAS refreshes expected");

    Point exp2 = p78;
    Point p910 = { 9, 10 };
    bool ok2 = a.compare_exchange_strong(exp2, p910,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok2 );
    test_pass("cxx11:atomic<struct>::compare_exchange_strong (orders)");

    Point exp3 = p910;
    while (!a.compare_exchange_weak(exp3, p12,
               STD::memory_order_release, STD::memory_order_relaxed)) { }
    Point v4 = a.load();
    test_true( v4 == p12 );
    test_pass("cxx11:atomic<struct>::compare_exchange_weak");

    AP b(p34);
    Point bv = b;
    test_true( bv == p34 );
    test_pass("cxx11:atomic<struct>(T) + operator T");

    Point assigned = (b = p56);
    TEST_SKIP_VC_RANGE(1600, 1900, "vc10..vc12 atomic<T>::operator=(T) returns the previous value for a non-lock-free T");
    test_true( assigned == p56 );
    Point bv2 = b.load();
    test_true( bv2 == p56 );
    test_pass("cxx11:atomic<struct>::operator=(T)");

    bool lf = a.is_lock_free();
#if _TST_ATOMIC_LIB_CXX17
    test_true( AP::is_always_lock_free ? lf : true );
#else
    test_true( lf || !lf );
#endif
    test_pass("cxx11:atomic<struct>::is_lock_free");
}

#if _TST_ATOMIC_LIB_CXX20
#if _TST_HAS_OS_THREADS
TEST_CASE(atomic, wait_notify) {

    STD::atomic<int> a(1);
    test_no_throw( a.wait(0) );
    test_no_throw( a.wait(0, STD::memory_order_acquire) );
    test_pass("cxx20:atomic<int>::wait (non-blocking)");
    test_no_throw( a.notify_one() );
    test_no_throw( a.notify_all() );
    test_pass("cxx20:atomic<int>::notify (non-blocking)");

    STD::atomic<int> v(0);
    STD::atomic<int> seen(-1);
    STD::thread t((WaitThenCopy(v, seen)));
    v.store(42);
    v.notify_one();
    t.join();
    int s = seen.load();
    test_eq( s, 42 );
    test_pass("cxx20:atomic<int>::wait + notify_one (threaded)");

    STD::atomic<int> w(0);
    STD::atomic<int> woken(0);
    STD::thread t1((WaitThenCount(w, woken, true)));
    STD::thread t2((WaitThenCount(w, woken, false)));
    w.store(1);
    w.notify_all();
    t1.join();
    t2.join();
    int n = woken.load();
    test_eq( n, 2 );
    test_pass("cxx20:atomic<int>::notify_all (threaded)");
}
#else
TEST_CASE_SKIP(atomic, wait_notify)
#endif
#else
TEST_CASE_SKIP(atomic, wait_notify)
#endif

TEST_CASE(atomic, flag_basic) {

#if _TST_HAS_CXX11_LIB_MEMBERS && !defined(__WATCOMC__) && !(defined(_MSC_VER) && _MSC_VER < 1900)
    static_assert(!STD::is_copy_constructible<STD::atomic_flag>::value, "");
    static_assert(!STD::is_copy_assignable<STD::atomic_flag>::value, "");
#endif
    test_true( 1 );
    test_pass("cxx11:atomic_flag is not copyable");

#if TEST_TARGET_CXX >= 2020
    STD::atomic_flag f;
#else
    STD::atomic_flag f = ATOMIC_FLAG_INIT;
#endif
    bool prev = f.test_and_set();
    test_true( !prev );
    bool prev1 = f.test_and_set();
    test_true( prev1 );
    test_pass("cxx11:atomic_flag::test_and_set");

    f.clear();
    bool prev2 = f.test_and_set(STD::memory_order_acq_rel);
    test_true( !prev2 );
    bool prev3 = f.test_and_set(STD::memory_order_acquire);
    test_true( prev3 );
    test_pass("cxx11:atomic_flag::test_and_set (memory_order)");

    f.clear(STD::memory_order_release);
    bool prev4 = f.test_and_set(STD::memory_order_relaxed);
    test_true( !prev4 );
    test_pass("cxx11:atomic_flag::clear (memory_order)");

    f.clear();
    bool prev5 = f.test_and_set();
    test_true( !prev5 );
    test_pass("cxx11:atomic_flag::clear");
}

#if _TST_ATOMIC_LIB_CXX20
TEST_CASE(atomic, flag_basic_cxx20) {

    STD::atomic_flag f;
    bool t0 = f.test();
    test_true( !t0 );
    test_pass("cxx20:atomic_flag::test");

    f.test_and_set();
    bool t1 = f.test();
    test_true( t1 );
    bool t2 = f.test(STD::memory_order_acquire);
    test_true( t2 );
    test_pass("cxx20:atomic_flag::test(memory_order)");

    f.clear();
    bool t3 = f.test(STD::memory_order_relaxed);
    test_true( !t3 );
    test_pass("cxx20:atomic_flag::test() (after clear)");
}
#else
TEST_CASE_SKIP(atomic, flag_basic_cxx20)
#endif

#if _TST_ATOMIC_LIB_CXX20
#if _TST_HAS_OS_THREADS
TEST_CASE(atomic, flag_wait_notify) {

    STD::atomic_flag fl;
    fl.test_and_set();
    test_no_throw( fl.wait(false) );
    test_no_throw( fl.wait(false, STD::memory_order_acquire) );
    test_pass("cxx20:atomic_flag::wait (non-blocking)");
    test_no_throw( fl.notify_one() );
    test_no_throw( fl.notify_all() );
    test_pass("cxx20:atomic_flag::notify (non-blocking)");

    STD::atomic_flag g;
    STD::atomic<bool> done(false);
    STD::thread t((FlagWaitThenSet(g, done)));
    g.test_and_set();
    g.notify_all();
    t.join();
    bool d = done.load();
    test_true( d );
    test_pass("cxx20:atomic_flag::wait + notify_all (threaded)");

    STD::atomic_flag h;
    STD::atomic<int> woken(0);
    STD::thread t1((FlagWaitThenCount(h, woken)));
    h.test_and_set();
    h.notify_one();
    t1.join();
    int n = woken.load();
    test_eq( n, 1 );
    test_pass("cxx20:atomic_flag::notify_one (threaded)");
}
#else
TEST_CASE_SKIP(atomic, flag_wait_notify)
#endif
#else
TEST_CASE_SKIP(atomic, flag_wait_notify)
#endif

#if _TST_ATOMIC_LIB_CXX20 && _TST_HAS_ATOMIC_SHARED_PTR && TEST_TARGET_CXX >= 2020
TEST_CASE(atomic, shared_ptr_specialization) {
    typedef STD::atomic<STD::shared_ptr<int> > AS;
    static_assert((STD::is_same<AS::value_type, STD::shared_ptr<int> >::value), "");
    test_true( 1 );
    test_pass("cxx20:atomic<shared_ptr>::value_type");

    AS a;
    STD::shared_ptr<int> e0 = a.load();
    test_true( !e0 );
    test_pass("cxx20:atomic<shared_ptr> default ctor + load");

    STD::shared_ptr<int> p1(new int(1));
    STD::shared_ptr<int> p2(new int(2));

    AS b(p1);
    STD::shared_ptr<int> l1 = b.load();
    test_eq( *l1, 1 );
    test_pass("cxx20:atomic<shared_ptr>(shared_ptr)");
    bool lf = b.is_lock_free();
    test_true( lf || !lf );
    test_pass("cxx20:atomic<shared_ptr>::is_lock_free");

    b.store(p2, STD::memory_order_release);
    STD::shared_ptr<int> l2 = b.load(STD::memory_order_acquire);
    test_eq( *l2, 2 );
    test_pass("cxx20:atomic<shared_ptr>::store");

    b = p1;
    STD::shared_ptr<int> l3 = b.load();
    test_eq( *l3, 1 );
    test_pass("cxx20:atomic<shared_ptr>::operator=");

    STD::shared_ptr<int> old = b.exchange(p2, STD::memory_order_acq_rel);
    test_eq( *old, 1 );
    test_pass("cxx20:atomic<shared_ptr>::exchange");

    STD::shared_ptr<int> exp = p2;
    bool ok = b.compare_exchange_strong(exp, p1);
    test_true( ok );
    STD::shared_ptr<int> l4 = b.load();
    test_eq( *l4, 1 );
    test_pass("cxx20:atomic<shared_ptr>::compare_exchange_strong");

    STD::shared_ptr<int> exp2 = p1;
    bool ok2 = b.compare_exchange_strong(exp2, p2,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok2 );
    test_pass("cxx20:atomic<shared_ptr>::compare_exchange_strong (orders)");

    STD::shared_ptr<int> exp3 = p2;
    while (!b.compare_exchange_weak(exp3, p1)) exp3 = p2;
    STD::shared_ptr<int> l5 = b.load();
    test_eq( *l5, 1 );
    test_pass("cxx20:atomic<shared_ptr>::compare_exchange_weak");

    STD::shared_ptr<int> exp4 = p1;
    while (!b.compare_exchange_weak(exp4, p2,
               STD::memory_order_seq_cst, STD::memory_order_relaxed)) exp4 = p1;
    STD::shared_ptr<int> l6 = b.load();
    test_eq( *l6, 2 );
    test_pass("cxx20:atomic<shared_ptr>::compare_exchange_weak (orders)");

    STD::shared_ptr<int> junk(new int(99));
    STD::shared_ptr<int> expBad = junk;
    bool no = b.compare_exchange_strong(expBad, p1, STD::memory_order_seq_cst);
    test_true( !no );
    test_eq( *expBad, 2 );
    test_pass("cxx20:atomic<shared_ptr> failing CAS refreshes expected");

    test_no_throw( b.wait(STD::shared_ptr<int>()) );
    test_no_throw( b.wait(STD::shared_ptr<int>(), STD::memory_order_acquire) );
    test_pass("cxx20:atomic<shared_ptr>::wait");
    test_no_throw( b.notify_one() );
    test_no_throw( b.notify_all() );
    test_pass("cxx20:atomic<shared_ptr>::notify");
}
#else
TEST_CASE_SKIP(atomic, shared_ptr_specialization)
#endif

#if _TST_ATOMIC_LIB_CXX20 && _TST_HAS_ATOMIC_SHARED_PTR && TEST_TARGET_CXX >= 2020
TEST_CASE(atomic, weak_ptr_specialization) {
    typedef STD::atomic<STD::weak_ptr<int> > AW;
    static_assert((STD::is_same<AW::value_type, STD::weak_ptr<int> >::value), "");
    test_true( 1 );
    test_pass("cxx20:atomic<weak_ptr>::value_type");

    STD::shared_ptr<int> s1(new int(1));
    STD::shared_ptr<int> s2(new int(2));
    STD::weak_ptr<int> w1(s1);
    STD::weak_ptr<int> w2(s2);

    AW a;
    STD::weak_ptr<int> e0 = a.load();
    test_true( e0.expired() );
    test_pass("cxx20:atomic<weak_ptr> default ctor + load");

    AW b(w1);
    STD::shared_ptr<int> l1 = b.load().lock();
    test_eq( *l1, 1 );
    test_pass("cxx20:atomic<weak_ptr>(weak_ptr)");
    bool lf = b.is_lock_free();
    test_true( lf || !lf );
    test_pass("cxx20:atomic<weak_ptr>::is_lock_free");

    b.store(w2, STD::memory_order_release);
    STD::shared_ptr<int> l2 = b.load(STD::memory_order_acquire).lock();
    test_eq( *l2, 2 );
    test_pass("cxx20:atomic<weak_ptr>::store");

    b = w1;
    STD::shared_ptr<int> l3 = b.load().lock();
    test_eq( *l3, 1 );
    test_pass("cxx20:atomic<weak_ptr>::operator=");

    STD::weak_ptr<int> old = b.exchange(w2, STD::memory_order_acq_rel);
    STD::shared_ptr<int> ol = old.lock();
    test_eq( *ol, 1 );
    test_pass("cxx20:atomic<weak_ptr>::exchange");

    STD::weak_ptr<int> exp = w2;
    bool ok = b.compare_exchange_strong(exp, w1);
    test_true( ok );
    test_pass("cxx20:atomic<weak_ptr>::compare_exchange_strong");

    STD::weak_ptr<int> exp2 = w1;
    bool ok2 = b.compare_exchange_strong(exp2, w2,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok2 );
    test_pass("cxx20:atomic<weak_ptr>::compare_exchange_strong (orders)");

    STD::weak_ptr<int> exp3 = w2;
    while (!b.compare_exchange_weak(exp3, w1)) exp3 = w2;
    STD::shared_ptr<int> l4 = b.load().lock();
    test_eq( *l4, 1 );
    test_pass("cxx20:atomic<weak_ptr>::compare_exchange_weak");

    STD::weak_ptr<int> exp4 = w1;
    while (!b.compare_exchange_weak(exp4, w2,
               STD::memory_order_seq_cst, STD::memory_order_relaxed)) exp4 = w1;
    STD::shared_ptr<int> l5 = b.load().lock();
    test_eq( *l5, 2 );
    test_pass("cxx20:atomic<weak_ptr>::compare_exchange_weak (orders)");

    STD::weak_ptr<int> expBad;
    bool no = b.compare_exchange_strong(expBad, w1, STD::memory_order_seq_cst);
    test_true( !no );
    STD::shared_ptr<int> refreshed = expBad.lock();
    test_eq( *refreshed, 2 );
    test_pass("cxx20:atomic<weak_ptr> failing CAS refreshes expected");

    test_no_throw( b.wait(STD::weak_ptr<int>()) );
    test_no_throw( b.wait(STD::weak_ptr<int>(), STD::memory_order_acquire) );
    test_pass("cxx20:atomic<weak_ptr>::wait");
    test_no_throw( b.notify_one() );
    test_no_throw( b.notify_all() );
    test_pass("cxx20:atomic<weak_ptr>::notify");
}
#else
TEST_CASE_SKIP(atomic, weak_ptr_specialization)
#endif

#else
TEST_CASE_SKIP(atomic, member_types_and_traits)
TEST_CASE_SKIP(atomic, int_load_store_exchange)
TEST_CASE_SKIP(atomic, int_compare_exchange)
TEST_CASE_SKIP(atomic, int_fetch_ops)
TEST_CASE_SKIP(atomic, int_operators)
TEST_CASE_SKIP(atomic, bool_specialization)
TEST_CASE_SKIP(atomic, floating_specialization)
TEST_CASE_SKIP(atomic, floating_specialization_cxx20)
TEST_CASE_SKIP(atomic, pointer_specialization)
TEST_CASE_SKIP(atomic, primary_template_struct)
TEST_CASE_SKIP(atomic, wait_notify)
TEST_CASE_SKIP(atomic, flag_basic)
TEST_CASE_SKIP(atomic, flag_basic_cxx20)
TEST_CASE_SKIP(atomic, flag_wait_notify)
TEST_CASE_SKIP(atomic, shared_ptr_specialization)
TEST_CASE_SKIP(atomic, weak_ptr_specialization)
#endif

TEST_CASE(atomic, constexpr_operations_cxx26) {
#if defined(__cpp_lib_constexpr_atomic) && (__cpp_lib_constexpr_atomic) >= 202411L
    struct K {
        static constexpr float ld() { STD::atomic<float> f(2.5f); return f.load(); }
        static constexpr float st() { STD::atomic<float> f(1.0f); f.store(3.5f); return f.load(); }
        static constexpr double add() { STD::atomic<double> d(1.0); d += 1.5; return d.load(); }
        static constexpr double sub() { STD::atomic<double> d(4.0); d -= 1.5; return d.load(); }
        static constexpr bool mo() { STD::atomic_flag f; f.test_and_set();
                                     return f.test(STD::memory_order_acquire); }
        static constexpr bool cleared() { STD::atomic_flag f; f.test_and_set(); f.clear();
                                          return f.test(); }
    };
    static_assert(K::ld() == 2.5f, "constexpr atomic<float>::load");
    test_eq( K::ld(), 2.5f );
    test_pass("cxx26:constexpr atomic<float>::load()");

    static_assert(K::st() == 3.5f, "constexpr atomic<float>::store");
    test_eq( K::st(), 3.5f );
    test_pass("cxx26:constexpr atomic<float>::store(value)");

    static_assert(K::add() == 2.5, "constexpr atomic<double>::operator+=");
    test_eq( K::add(), 2.5 );
    test_pass("cxx26:constexpr atomic<double>::operator+=(value)");

    static_assert(K::sub() == 2.5, "constexpr atomic<double>::operator-=");
    test_eq( K::sub(), 2.5 );
    test_pass("cxx26:constexpr atomic<double>::operator-=(value)");

    static_assert(K::mo(), "constexpr atomic_flag::test(memory_order)");
    test_true( K::mo() );
    test_pass("cxx26:constexpr atomic_flag::test(memory_order)");

    static_assert(!K::cleared(), "constexpr atomic_flag::test after clear");
    test_true( !K::cleared() );
    test_pass("cxx26:constexpr atomic_flag::test() (after clear)");
#else
    TEST_NOTE("constexpr atomic is C++26 (P3309R3, __cpp_lib_constexpr_atomic)");
    TEST_SKIP_N(6);
    test_skip("cxx26:constexpr atomic<float>::load()");
    test_skip("cxx26:constexpr atomic<float>::store(value)");
    test_skip("cxx26:constexpr atomic<double>::operator+=(value)");
    test_skip("cxx26:constexpr atomic<double>::operator-=(value)");
    test_skip("cxx26:constexpr atomic_flag::test(memory_order)");
    test_skip("cxx26:constexpr atomic_flag::test() (after clear)");
#endif
}
