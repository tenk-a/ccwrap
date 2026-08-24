#include "test_cxx.hpp"

#include <atomic>
#include <cstddef>
#include <type_traits>
#include <cstdint>
#include <memory>

#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1700) || \
    defined(__cpp_lib_atomic_min_max)
#  define _TST_ATOMIC_MIN_MAX 1
#else
#  define _TST_ATOMIC_MIN_MAX 0
#endif

TEST_CASE(atomic, load_store) {
    STD::atomic<int> a(0);
    test_eq( a.load(), 0 );
    test_true( a.is_lock_free() );

    a.store(5);
    test_eq( a.load(), 5 );
    test_eq( (int)a, 5 );
    a = 8;
    test_eq( a.load(), 8 );

    int prev = a.exchange(100);
    test_eq( prev, 8 );
    test_eq( a.load(), 100 );
}

TEST_CASE(atomic, arithmetic) {
    STD::atomic<int> a(5);
    int p;
    p = a.fetch_add(3);  test_eq( p, 5 );  test_eq( a.load(), 8 );
    p = a.fetch_sub(2);  test_eq( p, 8 );  test_eq( a.load(), 6 );

    int r;
    r = ++a;             test_eq( r, 7 );  test_eq( a.load(), 7 );
    r = a++;             test_eq( r, 7 );  test_eq( a.load(), 8 );
    r = --a;             test_eq( r, 7 );  test_eq( a.load(), 7 );
    r = a--;             test_eq( r, 7 );  test_eq( a.load(), 6 );
    r = (a += 4);        test_eq( r, 10 ); test_eq( a.load(), 10 );
    r = (a -= 3);        test_eq( r, 7 );  test_eq( a.load(), 7 );
}

TEST_CASE(atomic, bitwise) {
    STD::atomic<int> a(0x0F);
    int p;
    p = a.fetch_or(0xF0);   test_eq( p, 0x0F ); test_eq( a.load(), 0xFF );
    p = a.fetch_and(0x3C);  test_eq( p, 0xFF ); test_eq( a.load(), 0x3C );
    p = a.fetch_xor(0x0F);  test_eq( p, 0x3C ); test_eq( a.load(), 0x33 );

    int r;
    r = (a |= 0xC0);   test_eq( a.load(), 0xF3 );
    r = (a &= 0xF0);   test_eq( a.load(), 0xF0 );
    r = (a ^= 0xFF);   test_eq( a.load(), 0x0F );
    (void)r;
}

TEST_CASE(atomic, cas_flag_fence) {
    STD::atomic<int> a(10);
    int expected = 10;
    test_true( a.compare_exchange_strong(expected, 20) );
    test_eq( a.load(), 20 );
    expected = 10;
    test_true( !a.compare_exchange_strong(expected, 30) );
    test_eq( expected, 20 );
    expected = 20;
    test_true( a.compare_exchange_weak(expected, 40) );
    test_eq( a.load(), 40 );

    STD::atomic_flag f;
    f.clear();
    test_true( !f.test_and_set() );
    test_true(  f.test_and_set() );
    f.clear();
    test_true( !f.test_and_set() );

    STD::atomic_thread_fence(STD::memory_order_seq_cst);
    STD::atomic_signal_fence(STD::memory_order_seq_cst);

#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    STD::atomic_int      ai;  ai.store(1);    test_eq( ai.load(), 1 );
    STD::atomic_uint     au;  au.store(2u);   test_eq( (int)au.load(), 2 );
    STD::atomic_bool     ab;  ab.store(true); test_true( ab.load() );
#else
    STD::atomic_int      ai(1);   test_eq( ai.load(), 1 );
    STD::atomic_uint     au(2u);  test_eq( (int)au.load(), 2 );
    STD::atomic_bool     ab(true);test_true( ab.load() );
#endif
    STD::atomic_size_t   as(3u);  test_eq( (int)as.load(), 3 );
    STD::atomic_ptrdiff_t ap(4);  test_eq( (int)ap.load(), 4 );
}

TEST_CASE(atomic, member_types_and_new_ops) {

#if _TST_ATOMIC_LIB_CXX17
    test_true( (STD::is_same<STD::atomic<int>::value_type, int>::value) );
    test_true( (STD::is_same<STD::atomic<int>::difference_type, int>::value) );

    test_true( STD::atomic<int>::is_always_lock_free );
#else
    TEST_SKIP_N(3);
#endif

    STD::atomic<int> a(5);
    int v = a;
    test_eq( v, 5 );

#if TEST_TARGET_CXX >= 2026 && _TST_ATOMIC_MIN_MAX
    STD::atomic<int> m(10);
    int prev = m.fetch_max(20);
    test_eq( prev, 10 );
    test_eq( m.load(), 20 );
    prev = m.fetch_max(15);
    test_eq( prev, 20 );
    test_eq( m.load(), 20 );
    test_pass("cxx26:atomic<int>::fetch_max");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx26:atomic<int>::fetch_max");
#endif
#if TEST_TARGET_CXX >= 2026 && _TST_ATOMIC_MIN_MAX
    prev = m.fetch_min(5);
    test_eq( prev, 20 );
    test_eq( m.load(), 5 );
    test_pass("cxx26:atomic<int>::fetch_min");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx26:atomic<int>::fetch_min");
#endif

    STD::atomic<int> w(1);
#if _TST_ATOMIC_LIB_CXX20
    w.wait(0);
    w.notify_one();
    w.notify_all();
#endif
    test_true( true );

#if TEST_TARGET_CXX >= 2020
    test_true( ATOMIC_CHAR8_T_LOCK_FREE >= 0 && ATOMIC_CHAR8_T_LOCK_FREE <= 2 );
#else
    TEST_SKIP1();
#endif
}

TEST_CASE(atomic, nonmember_and_explicit) {
    STD::atomic<int> a(1);
    test_true( STD::atomic_is_lock_free(&a) );

    STD::atomic_store(&a, 5);
    test_eq( STD::atomic_load(&a), 5 );
    STD::atomic_store_explicit(&a, 6, STD::memory_order_release);
    test_eq( STD::atomic_load_explicit(&a, STD::memory_order_acquire), 6 );

    int old1 = STD::atomic_exchange(&a, 7);
    test_eq( old1, 6 );
    int old2 = STD::atomic_exchange_explicit(&a, 8, STD::memory_order_acq_rel);
    test_eq( old2, 7 );
    test_eq( STD::atomic_load(&a), 8 );

    STD::atomic<int> b(10);
    int e1 = 10;
    test_true( STD::atomic_compare_exchange_strong(&b, &e1, 11) );
    test_eq( STD::atomic_load(&b), 11 );
    int e2 = 0;
    test_true( !STD::atomic_compare_exchange_strong(&b, &e2, 12) );
    test_eq( e2, 11 );
    int e3 = 11;
    test_true( STD::atomic_compare_exchange_strong_explicit(&b, &e3, 12,
                   STD::memory_order_acq_rel, STD::memory_order_acquire) );
    test_eq( STD::atomic_load(&b), 12 );
    int e4 = 12;
    while (!STD::atomic_compare_exchange_weak(&b, &e4, 13)) {}
    test_eq( STD::atomic_load(&b), 13 );
    int e5 = 13;
    while (!STD::atomic_compare_exchange_weak_explicit(&b, &e5, 14,
               STD::memory_order_seq_cst, STD::memory_order_relaxed)) {}
    test_eq( STD::atomic_load(&b), 14 );

    STD::atomic<int> c(0);
    int f1 = STD::atomic_fetch_add(&c, 5);              test_eq( f1, 0 );
    int f2 = STD::atomic_fetch_add_explicit(&c, 5, STD::memory_order_relaxed); test_eq( f2, 5 );
    test_eq( STD::atomic_load(&c), 10 );
    int f3 = STD::atomic_fetch_sub(&c, 3);              test_eq( f3, 10 );
    int f4 = STD::atomic_fetch_sub_explicit(&c, 3, STD::memory_order_acq_rel); test_eq( f4, 7 );
    test_eq( STD::atomic_load(&c), 4 );

    STD::atomic_store(&c, 0xF0);
    int g1 = STD::atomic_fetch_and(&c, 0x3C);           test_eq( g1, 0xF0 );
    int g2 = STD::atomic_fetch_and_explicit(&c, 0x10, STD::memory_order_seq_cst); test_eq( g2, 0x30 );
    test_eq( STD::atomic_load(&c), 0x10 );
    int h1 = STD::atomic_fetch_or(&c, 0x01);            test_eq( h1, 0x10 );
    int h2 = STD::atomic_fetch_or_explicit(&c, 0x02, STD::memory_order_release); test_eq( h2, 0x11 );
    test_eq( STD::atomic_load(&c), 0x13 );
    int x1 = STD::atomic_fetch_xor(&c, 0x03);           test_eq( x1, 0x13 );
    int x2 = STD::atomic_fetch_xor_explicit(&c, 0xFF, STD::memory_order_acquire); test_eq( x2, 0x10 );
    test_eq( STD::atomic_load(&c), 0xEF );

    static int arr[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    STD::atomic<int*> p(arr);
    STD::atomic_store(&p, arr + 1);
    test_eq( STD::atomic_load(&p) - arr, STD::ptrdiff_t(1) );
    int* pold = STD::atomic_exchange_explicit(&p, arr + 2, STD::memory_order_acq_rel);
    test_eq( pold - arr, STD::ptrdiff_t(1) );
    int* pa = STD::atomic_fetch_add(&p, 3);
    test_eq( pa - arr, STD::ptrdiff_t(2) );
    test_eq( STD::atomic_load(&p) - arr, STD::ptrdiff_t(5) );
    int* ps = STD::atomic_fetch_sub_explicit(&p, 2, STD::memory_order_relaxed);
    test_eq( ps - arr, STD::ptrdiff_t(5) );
    test_eq( STD::atomic_load(&p) - arr, STD::ptrdiff_t(3) );
    test_true( STD::atomic_is_lock_free(&p) );
}

#if _TST_HAS_ATOMIC_SHARED_PTR && TEST_TARGET_CXX >= 2020
TEST_CASE(atomic, shared_ptr_atomic) {
    typedef STD::atomic<STD::shared_ptr<int> > AS;
#if _TST_ATOMIC_LIB_CXX17
    test_true( (STD::is_same<AS::value_type, STD::shared_ptr<int> >::value) );
#else
    TEST_SKIP1();
#endif

    AS a;
    STD::shared_ptr<int> e0 = a.load();
    test_true( !e0 );

    STD::shared_ptr<int> p1(new int(1));
    STD::shared_ptr<int> p2(new int(2));

    AS b(p1);
    STD::shared_ptr<int> l1 = b.load();
    test_eq( *l1, 1 );
    TEST_SKIP_VC120("no atomic<shared_ptr> specialisation; the generic atomic<T> answers is_lock_free() from sizeof");
    test_true( !b.is_lock_free() );

    b.store(p2, STD::memory_order_release);
    STD::shared_ptr<int> l2 = b.load(STD::memory_order_acquire);
    test_eq( *l2, 2 );

    b = p1;
    STD::shared_ptr<int> l3 = b.load();
    test_eq( *l3, 1 );

    STD::shared_ptr<int> old = b.exchange(p2, STD::memory_order_acq_rel);
    test_eq( *old, 1 );

    STD::shared_ptr<int> exp = p2;
    bool ok = b.compare_exchange_strong(exp, p1);
    test_true( ok );
    STD::shared_ptr<int> l4 = b.load();
    test_eq( *l4, 1 );

    STD::shared_ptr<int> exp2 = p1;
    bool ok2 = b.compare_exchange_strong(exp2, p2,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok2 );

    STD::shared_ptr<int> exp3 = p2;
    while (!b.compare_exchange_weak(exp3, p1)) exp3 = p2;
    STD::shared_ptr<int> l5 = b.load();
    test_eq( *l5, 1 );

    STD::shared_ptr<int> junk(new int(99));
    STD::shared_ptr<int> expBad = junk;
    bool no = b.compare_exchange_strong(expBad, p2, STD::memory_order_seq_cst);
    test_true( !no );
    test_eq( *expBad, 1 );

#if _TST_ATOMIC_LIB_CXX20
    test_no_throw( b.wait(STD::shared_ptr<int>()) );
    test_no_throw( b.wait(STD::shared_ptr<int>(), STD::memory_order_acquire) );
    test_no_throw( b.notify_one() );
    test_no_throw( b.notify_all() );
#else
    TEST_SKIP_N(4);
#endif
}
#else
TEST_CASE(atomic, shared_ptr_atomic) {
    TEST_NOTE("atomic<shared_ptr> (C++20 P0718) is not implemented by this library");
    TEST_SKIP1();
}
#endif

#if _TST_HAS_ATOMIC_SHARED_PTR && TEST_TARGET_CXX >= 2020
TEST_CASE(atomic, weak_ptr_atomic) {
    typedef STD::atomic<STD::weak_ptr<int> > AW;
#if _TST_ATOMIC_LIB_CXX17
    test_true( (STD::is_same<AW::value_type, STD::weak_ptr<int> >::value) );
#else
    TEST_SKIP1();
#endif

    STD::shared_ptr<int> s1(new int(1));
    STD::shared_ptr<int> s2(new int(2));
    STD::weak_ptr<int> w1(s1);
    STD::weak_ptr<int> w2(s2);

    AW a;
    STD::weak_ptr<int> e0 = a.load();
    test_true( e0.expired() );

    AW b(w1);
    STD::shared_ptr<int> l1 = b.load().lock();
    test_eq( *l1, 1 );
    TEST_SKIP_VC120("no atomic<weak_ptr> specialisation; the generic atomic<T> answers is_lock_free() from sizeof");
    test_true( !b.is_lock_free() );

    b.store(w2, STD::memory_order_release);
    STD::shared_ptr<int> l2 = b.load(STD::memory_order_acquire).lock();
    test_eq( *l2, 2 );

    b = w1;
    STD::shared_ptr<int> l3 = b.load().lock();
    test_eq( *l3, 1 );

    STD::weak_ptr<int> old = b.exchange(w2, STD::memory_order_acq_rel);
    STD::shared_ptr<int> ol = old.lock();
    test_eq( *ol, 1 );

    STD::weak_ptr<int> exp = w2;
    bool ok = b.compare_exchange_strong(exp, w1);
    test_true( ok );
    STD::shared_ptr<int> l4 = b.load().lock();
    test_eq( *l4, 1 );

    STD::weak_ptr<int> exp2 = w1;
    while (!b.compare_exchange_weak(exp2, w2,
               STD::memory_order_seq_cst, STD::memory_order_relaxed)) exp2 = w1;
    STD::shared_ptr<int> l5 = b.load().lock();
    test_eq( *l5, 2 );

    STD::weak_ptr<int> expBad;
    bool no = b.compare_exchange_strong(expBad, w1, STD::memory_order_seq_cst);
    test_true( !no );
    STD::shared_ptr<int> refreshed = expBad.lock();
    test_eq( *refreshed, 2 );

#if _TST_ATOMIC_LIB_CXX20
    test_no_throw( b.wait(STD::weak_ptr<int>()) );
    test_no_throw( b.wait(STD::weak_ptr<int>(), STD::memory_order_acquire) );
    test_no_throw( b.notify_one() );
    test_no_throw( b.notify_all() );
#else
    TEST_SKIP_N(4);
#endif
}
#else
TEST_CASE(atomic, weak_ptr_atomic) {
    TEST_NOTE("atomic<weak_ptr> (C++20 P0718) is not implemented by this library");
    TEST_SKIP1();
}
#endif

TEST_CASE(atomic, aliases_and_wait_notify) {

#if _TST_ATOMIC_LIB_CXX17
    test_true( (STD::is_same<STD::atomic_char16_t::value_type, char16_t>::value) );
    test_true( (STD::is_same<STD::atomic_char32_t::value_type, char32_t>::value) );
    test_true( (STD::is_same<STD::atomic_int_least8_t::value_type,   STD::int_least8_t>::value) );
    test_true( (STD::is_same<STD::atomic_uint_least16_t::value_type, STD::uint_least16_t>::value) );
    test_true( (STD::is_same<STD::atomic_int_least32_t::value_type,  STD::int_least32_t>::value) );
    test_true( (STD::is_same<STD::atomic_uint_least64_t::value_type, STD::uint_least64_t>::value) );
    test_true( (STD::is_same<STD::atomic_int_fast8_t::value_type,    STD::int_fast8_t>::value) );
    test_true( (STD::is_same<STD::atomic_uint_fast32_t::value_type,  STD::uint_fast32_t>::value) );
#else
    TEST_SKIP_N(8);
#endif
    { STD::atomic_int_least8_t z(7); test_eq( (int)z.load(), 7 ); }

    STD::atomic_flag f;
    f.clear();
#if _TST_ATOMIC_LIB_CXX20
    test_true( !f.test() );
    f.test_and_set();
    test_true( f.test() );
    f.wait(false);
    f.notify_one();
    f.notify_all();
#else
    TEST_SKIP_N(2);
#endif
    test_true( true );

    STD::atomic<int> a(1);
#if _TST_ATOMIC_LIB_CXX20
    a.wait(0);
#endif
#if _TST_ATOMIC_LIB_CXX20
    STD::atomic_wait(&a, 0);
    STD::atomic_wait_explicit(&a, 0, STD::memory_order_acquire);
    STD::atomic_notify_one(&a);
    STD::atomic_notify_all(&a);
#endif
    test_true( true );

    STD::atomic_flag g;
    g.clear();
    STD::atomic_flag_test_and_set(&g);
#if _TST_ATOMIC_LIB_CXX20
    test_true( STD::atomic_flag_test(&g) );
    test_true( STD::atomic_flag_test_explicit(&g, STD::memory_order_acquire) );
    STD::atomic_flag_wait(&g, false);
    STD::atomic_flag_notify_one(&g);
    STD::atomic_flag_notify_all(&g);
#else
    TEST_SKIP_N(2);
#endif
    test_true( true );

#if TEST_TARGET_CXX >= 2026 && _TST_ATOMIC_MIN_MAX
    STD::atomic<int> m(10);
    int p1 = STD::atomic_fetch_max(&m, 20);          test_eq( p1, 10 ); test_eq( m.load(), 20 );
    int p2 = STD::atomic_fetch_max(&m, 15);          test_eq( p2, 20 ); test_eq( m.load(), 20 );
    test_pass("cxx26:atomic_fetch_max (nonmember)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx26:atomic_fetch_max (nonmember)");
#endif
#if TEST_TARGET_CXX >= 2026 && _TST_ATOMIC_MIN_MAX
    int p3 = STD::atomic_fetch_min_explicit(&m, 5, STD::memory_order_acq_rel);
    test_eq( p3, 20 ); test_eq( m.load(), 5 );
    test_pass("cxx26:atomic_fetch_min (nonmember)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx26:atomic_fetch_min (nonmember)");
#endif
}

#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
#  define _TST_ATOMIC_SAME(alias, T)   TEST_SKIP1()
#else
#  define _TST_ATOMIC_SAME(alias, T)   \
       test_true(( STD::is_same<STD::alias, STD::atomic<T> >::value ))
#endif

#if _TST_ATOMIC_LIB_CXX17
#  define _TST_ATOMIC_VT(alias, T)   test_true(( STD::is_same<STD::alias::value_type, T>::value ))
#else
#  define _TST_ATOMIC_VT(alias, T)   TEST_SKIP1()
#endif

#define _TST_ATOMIC_ALIAS(alias, T, row)                                  \
    do {                                                                  \
        _TST_ATOMIC_VT(alias, T);                                         \
        _TST_ATOMIC_SAME(alias, T);                                       \
        STD::alias _v;                                                    \
        _v.store((T)1);                                                   \
        test_true( _v.load() == (T)1 );                                   \
        test_pass(row);                                                   \
    } while (0)

#if TEST_TARGET_CXX >= 2020
TEST_CASE(atomic, atomic_ref_int) {
    typedef STD::atomic_ref<int> AR;
    test_true( (STD::is_same<AR::value_type, int>::value) );
    test_true( (STD::is_same<AR::difference_type, int>::value) );
    test_true( AR::required_alignment >= sizeof(int) || AR::required_alignment >= 1 );
    test_true( AR::is_always_lock_free );

    int obj = 1;
    AR r(obj);
    AR r2(r);
    test_true( r.is_lock_free() );

    r.store(5);
    test_eq( obj, 5 );
    test_eq( r2.load(), 5 );

    r.store(6, STD::memory_order_release);
    test_eq( r.load(STD::memory_order_acquire), 6 );

    int conv = r;
    test_eq( conv, 6 );
    int assigned = (r = 7);
    test_eq( assigned, 7 );
    test_eq( obj, 7 );

    int old = r.exchange(8);
    test_eq( old, 7 );
    test_eq( obj, 8 );

    int e1 = 8;
    test_true( r.compare_exchange_strong(e1, 20) );
    test_eq( obj, 20 );
    int e2 = 0;
    test_true( !r.compare_exchange_strong(e2, 30) );
    test_eq( e2, 20 );
    int e3 = 20;
    test_true( r.compare_exchange_weak(e3, 21,
                   STD::memory_order_seq_cst, STD::memory_order_relaxed) );
    test_eq( obj, 21 );

    obj = 0;
    int a1 = r.fetch_add(5);   test_eq( a1, 0 );  test_eq( obj, 5 );
    int a2 = r.fetch_sub(2);   test_eq( a2, 5 );  test_eq( obj, 3 );
    obj = 0xF0;
    int a3 = r.fetch_and(0x3C); test_eq( a3, 0xF0 ); test_eq( obj, 0x30 );
    int a4 = r.fetch_or(0x01);  test_eq( a4, 0x30 ); test_eq( obj, 0x31 );
    int a5 = r.fetch_xor(0x11); test_eq( a5, 0x31 ); test_eq( obj, 0x20 );
    obj = 5;
    int pp = ++r; test_eq( pp, 6 ); test_eq( obj, 6 );
    int qq = r++; test_eq( qq, 6 ); test_eq( obj, 7 );

    obj = 42;
    r.wait(0);
    r.notify_one();
    r.notify_all();
    test_true( true );
}

TEST_CASE(atomic, atomic_ref_primary) {

    typedef STD::atomic_ref<double> AR;
    test_true( (STD::is_same<AR::value_type, double>::value) );
    test_pass("cxx20:atomic_ref<double>::value_type");
#if defined(__WATCOMC__)
    test_true( !AR::is_always_lock_free );
    test_pass("cxx20:atomic_ref<double>::is_always_lock_free");
#else
    TEST_SKIP1();
    test_skip("cxx20:atomic_ref<double>::is_always_lock_free");
#endif

    double obj = 1.5;
    AR r(obj);
#if defined(__WATCOMC__)
    test_true( !r.is_lock_free() );
    test_pass("cxx20:atomic_ref<double>::is_lock_free");
#else
    (void)r.is_lock_free();
    TEST_SKIP1();
    test_skip("cxx20:atomic_ref<double>::is_lock_free");
#endif
    r.store(2.5);
    test_eq( obj, 2.5 );
    test_eq( r.load(), 2.5 );
    test_pass("cxx20:atomic_ref<double>::store");
    double old = r.exchange(3.5);
    test_eq( old, 2.5 );
    test_pass("cxx20:atomic_ref<double>::exchange");
    double e = 3.5;
    test_true( r.compare_exchange_strong(e, 4.5) );
    test_eq( obj, 4.5 );
    test_pass("cxx20:atomic_ref<double>::compare_exchange_strong");
    double bad = 0.0;
    test_true( !r.compare_exchange_strong(bad, 9.0) );
    test_eq( bad, 4.5 );
    test_pass("cxx20:atomic_ref<double> failing CAS refreshes expected");
}

TEST_CASE(atomic, atomic_init_and_ref_minmax) {

#if 1
    STD::atomic<int> a;
    STD::atomic_init(&a, 5);
    test_eq( a.load(), 5 );
    test_pass("cxx11:atomic_init (free, deprecated in C++20)");
#else
    test_skip("cxx11:atomic_init (free, deprecated in C++20)");
#endif

#if defined(__cpp_lib_atomic_min_max)
    int obj = 10;
    STD::atomic_ref<int> r(obj);
    r.fetch_max(20);
    test_eq( r.load(), 20 );
    test_pass("cxx26:atomic_ref<int>::fetch_max");
    r.fetch_min(5);
    test_eq( r.load(), 5 );
    test_pass("cxx26:atomic_ref<int>::fetch_min");
#else
    test_skip("cxx26:atomic_ref<int>::fetch_max");
    test_skip("cxx26:atomic_ref<int>::fetch_min");
#endif
}

#else
TEST_CASE(atomic, atomic_ref_int) {
    TEST_NOTE("atomic_ref is C++20");
    TEST_SKIP1();
}
TEST_CASE(atomic, atomic_ref_primary) {
    TEST_NOTE("atomic_ref is C++20");
    test_skip("cxx20:atomic_ref<double>::value_type");
    test_skip("cxx20:atomic_ref<double>::is_always_lock_free");
    test_skip("cxx20:atomic_ref<double>::is_lock_free");
    test_skip("cxx20:atomic_ref<double>::store");
    test_skip("cxx20:atomic_ref<double>::exchange");
    test_skip("cxx20:atomic_ref<double>::compare_exchange_strong");
    test_skip("cxx20:atomic_ref<double> failing CAS refreshes expected");
}
TEST_CASE(atomic, atomic_init_and_ref_minmax) {
    TEST_NOTE("atomic_ref is C++20");
    test_skip("cxx11:atomic_init (free, deprecated in C++20)");
    test_skip("cxx26:atomic_ref<int>::fetch_max");
    test_skip("cxx26:atomic_ref<int>::fetch_min");
}
#endif

TEST_CASE(atomic, named_aliases_cxx11) {
    _TST_ATOMIC_ALIAS(atomic_char,  char,           "cxx11:atomic_char");
    _TST_ATOMIC_ALIAS(atomic_schar, signed char,    "cxx11:atomic_schar");
    _TST_ATOMIC_ALIAS(atomic_uchar, unsigned char,  "cxx11:atomic_uchar");
    _TST_ATOMIC_ALIAS(atomic_short, short,          "cxx11:atomic_short");
    _TST_ATOMIC_ALIAS(atomic_ushort, unsigned short, "cxx11:atomic_ushort");
    _TST_ATOMIC_ALIAS(atomic_uint,  unsigned int,   "cxx11:atomic_uint");
    _TST_ATOMIC_ALIAS(atomic_long,  long,           "cxx11:atomic_long");
    _TST_ATOMIC_ALIAS(atomic_ulong, unsigned long,  "cxx11:atomic_ulong");
    _TST_ATOMIC_ALIAS(atomic_llong, long long,      "cxx11:atomic_llong");
}

TEST_CASE(atomic, named_aliases_fixed_width_cxx11) {
#if _TST_HAS_CHAR16_32
    _TST_ATOMIC_ALIAS(atomic_char16_t, char16_t, "cxx11:atomic_char16_t");
    _TST_ATOMIC_ALIAS(atomic_char32_t, char32_t, "cxx11:atomic_char32_t");
#else
    TEST_NOTE("char16_t / char32_t are not distinct types here");
    test_skip("cxx11:atomic_char16_t");
    test_skip("cxx11:atomic_char32_t");
#endif
    _TST_ATOMIC_ALIAS(atomic_int8_t,   STD::int8_t,   "cxx11:atomic_int8_t");
    _TST_ATOMIC_ALIAS(atomic_int16_t,  STD::int16_t,  "cxx11:atomic_int16_t");
    _TST_ATOMIC_ALIAS(atomic_int64_t,  STD::int64_t,  "cxx11:atomic_int64_t");
    _TST_ATOMIC_ALIAS(atomic_uint8_t,  STD::uint8_t,  "cxx11:atomic_uint8_t");
    _TST_ATOMIC_ALIAS(atomic_uint16_t, STD::uint16_t, "cxx11:atomic_uint16_t");
    _TST_ATOMIC_ALIAS(atomic_uint32_t, STD::uint32_t, "cxx11:atomic_uint32_t");

    _TST_ATOMIC_ALIAS(atomic_int_least8_t,    STD::int_least8_t,    "cxx11:atomic_int_least8_t");
    _TST_ATOMIC_ALIAS(atomic_int_least16_t,   STD::int_least16_t,   "cxx11:atomic_int_least16_t");
    _TST_ATOMIC_ALIAS(atomic_int_least32_t,   STD::int_least32_t,   "cxx11:atomic_int_least32_t");
    _TST_ATOMIC_ALIAS(atomic_int_least64_t,   STD::int_least64_t,   "cxx11:atomic_int_least64_t");
    _TST_ATOMIC_ALIAS(atomic_uint_least8_t,   STD::uint_least8_t,   "cxx11:atomic_uint_least8_t");
    _TST_ATOMIC_ALIAS(atomic_uint_least16_t,  STD::uint_least16_t,  "cxx11:atomic_uint_least16_t");
    _TST_ATOMIC_ALIAS(atomic_uint_least32_t,  STD::uint_least32_t,  "cxx11:atomic_uint_least32_t");
    _TST_ATOMIC_ALIAS(atomic_uint_least64_t,  STD::uint_least64_t,  "cxx11:atomic_uint_least64_t");

    _TST_ATOMIC_ALIAS(atomic_int_fast8_t,     STD::int_fast8_t,     "cxx11:atomic_int_fast8_t");
    _TST_ATOMIC_ALIAS(atomic_int_fast32_t,    STD::int_fast32_t,    "cxx11:atomic_int_fast32_t");
    _TST_ATOMIC_ALIAS(atomic_int_fast64_t,    STD::int_fast64_t,    "cxx11:atomic_int_fast64_t");
    _TST_ATOMIC_ALIAS(atomic_uint_fast8_t,    STD::uint_fast8_t,    "cxx11:atomic_uint_fast8_t");
    _TST_ATOMIC_ALIAS(atomic_uint_fast16_t,   STD::uint_fast16_t,   "cxx11:atomic_uint_fast16_t");
    _TST_ATOMIC_ALIAS(atomic_uint_fast32_t,   STD::uint_fast32_t,   "cxx11:atomic_uint_fast32_t");
    _TST_ATOMIC_ALIAS(atomic_uint_fast64_t,   STD::uint_fast64_t,   "cxx11:atomic_uint_fast64_t");

    _TST_ATOMIC_ALIAS(atomic_intmax_t,  STD::intmax_t,  "cxx11:atomic_intmax_t");
    _TST_ATOMIC_ALIAS(atomic_intptr_t,  STD::intptr_t,  "cxx11:atomic_intptr_t");
    _TST_ATOMIC_ALIAS(atomic_uintptr_t, STD::uintptr_t, "cxx11:atomic_uintptr_t");
}

TEST_CASE(atomic, memory_order_constants_cxx20) {
#if _TST_ATOMIC_LIB_CXX20
    test_true( STD::memory_order_relaxed == STD::memory_order::relaxed );
    STD::atomic<int> a(0);
    a.store(1, STD::memory_order_relaxed);
    test_eq( a.load(STD::memory_order_relaxed), 1 );
    test_pass("cxx20:memory_order_relaxed");

    test_true( STD::memory_order_acquire == STD::memory_order::acquire );
    test_eq( a.load(STD::memory_order_acquire), 1 );
    test_pass("cxx20:memory_order_acquire");

    test_true( STD::memory_order_release == STD::memory_order::release );
    a.store(2, STD::memory_order_release);
    test_pass("cxx20:memory_order_release");

    test_true( STD::memory_order_acq_rel == STD::memory_order::acq_rel );
    int prev = a.exchange(3, STD::memory_order_acq_rel);
    test_eq( prev, 2 );
    test_pass("cxx20:memory_order_acq_rel");

    test_true( STD::memory_order_seq_cst == STD::memory_order::seq_cst );
    test_eq( a.load(STD::memory_order_seq_cst), 3 );
    test_true( STD::memory_order_relaxed != STD::memory_order_seq_cst );
    test_true( STD::memory_order_acquire != STD::memory_order_release );
    test_pass("cxx20:memory_order_seq_cst");
#else
    TEST_NOTE("memory_order is not a scoped enum here, so there is nothing to compare");
    test_skip("cxx20:memory_order_relaxed");
    test_skip("cxx20:memory_order_acquire");
    test_skip("cxx20:memory_order_release");
    test_skip("cxx20:memory_order_acq_rel");
    test_skip("cxx20:memory_order_seq_cst");
#endif
}

TEST_CASE(atomic, free_wait_notify_cxx20) {
#if _TST_ATOMIC_LIB_CXX20
    STD::atomic<int> a(1);
    STD::atomic_wait_explicit(&a, 0, STD::memory_order_seq_cst);
    test_true( a.load() == 1 );
    test_pass("cxx20:atomic_wait_explicit");
    STD::atomic_notify_one(&a);
    test_true( a.load() == 1 );
    test_pass("cxx20:atomic_notify_one");

    STD::atomic_flag f;
    f.clear();
    STD::atomic_flag_wait_explicit(&f, true, STD::memory_order_seq_cst);
    test_true( !f.test() );
    test_pass("cxx20:atomic_flag_wait_explicit");
    STD::atomic_flag_notify_one(&f);
    test_true( !f.test() );
    test_pass("cxx20:atomic_flag_notify_one");
    STD::atomic_flag_notify_all(&f);
    test_true( !f.test() );
    test_pass("cxx20:atomic_flag_notify_all");

    STD::atomic_signed_lock_free s;
    s.store(1);
    test_eq( (long)s.load(), 1L );
    test_true( STD::atomic_signed_lock_free::is_always_lock_free );
    test_true( STD::is_signed<STD::atomic_signed_lock_free::value_type>::value );
    test_pass("cxx20:atomic_signed_lock_free");

    STD::atomic_unsigned_lock_free u;
    u.store(1u);
    test_eq( (long)u.load(), 1L );
    test_true( STD::atomic_unsigned_lock_free::is_always_lock_free );
    test_true( !STD::is_signed<STD::atomic_unsigned_lock_free::value_type>::value );
    test_pass("cxx20:atomic_unsigned_lock_free");
#else
    TEST_NOTE("the wait / notify family is C++20; not in this library");
    test_skip("cxx20:atomic_wait_explicit");
    test_skip("cxx20:atomic_notify_one");
    test_skip("cxx20:atomic_flag_wait_explicit");
    test_skip("cxx20:atomic_flag_notify_one");
    test_skip("cxx20:atomic_flag_notify_all");
    test_skip("cxx20:atomic_signed_lock_free");
    test_skip("cxx20:atomic_unsigned_lock_free");
#endif
}

TEST_CASE(atomic, atomic_ref_members_cxx20) {
#if _TST_ATOMIC_LIB_CXX20
    int v = 5;
    STD::atomic_ref<int> r(v);

    test_true(( STD::is_same<STD::atomic_ref<int>::value_type, int>::value ));
    r.store(6);
    test_eq( v, 6 );
    test_pass("cxx20:atomic_ref<T>::value_type");

    test_true( STD::atomic_ref<int>::required_alignment >= alignof(int) );
    test_true( (STD::size_t)STD::atomic_ref<int>::required_alignment > 0 );
    test_pass("cxx20:atomic_ref<T>::required_alignment");

    static_assert(STD::atomic_ref<int>::is_always_lock_free
                  || !STD::atomic_ref<int>::is_always_lock_free, "constant");
    test_eq( (int)STD::atomic_ref<int>::is_always_lock_free, (int)r.is_lock_free() );
    test_pass("cxx20:atomic_ref<T>::is_always_lock_free");

#if defined(__cpp_lib_atomic_ref) && (__cpp_lib_atomic_ref) >= 202502L
    test_true( r.address() == &v );
    test_pass("cxx26:atomic_ref<T>::address");
#else
    TEST_NOTE("atomic_ref::address is C++26 (P2835); not in this library");
    test_skip("cxx26:atomic_ref<T>::address");
#endif
#else
    TEST_NOTE("atomic_ref is C++20; not in this library");
    test_skip("cxx20:atomic_ref<T>::value_type");
    test_skip("cxx20:atomic_ref<T>::required_alignment");
    test_skip("cxx20:atomic_ref<T>::is_always_lock_free");
    test_skip("cxx26:atomic_ref<T>::address");
#endif
}

TEST_CASE(atomic, minmax_and_store_ops_cxx26) {
#if defined(__cpp_lib_atomic_min_max)
    STD::atomic<int> a(3);
    test_eq( STD::atomic_fetch_max_explicit(&a, 5, STD::memory_order_seq_cst), 3 );
    test_eq( a.load(), 5 );
    test_eq( STD::atomic_fetch_max_explicit(&a, 1, STD::memory_order_seq_cst), 5 );
    test_eq( a.load(), 5 );
    test_pass("cxx26:atomic_fetch_max_explicit");
    test_eq( STD::atomic_fetch_min_explicit(&a, 2, STD::memory_order_seq_cst), 5 );
    test_eq( a.load(), 2 );
    test_pass("cxx26:atomic_fetch_min_explicit");
#else
    TEST_NOTE("the atomic min / max operations are C++26 (P0493); not in this library");
    test_skip("cxx26:atomic_fetch_max_explicit");
    test_skip("cxx26:atomic_fetch_min_explicit");
#endif

#if defined(_CCW_HAS_ATOMIC_STORE_OPS)
    STD::atomic<int> b(1);
    STD::atomic_store_add(&b, 2);            test_eq( b.load(), 3 );
    test_pass("cxx26:atomic_store_add");
    STD::atomic_store_add_explicit(&b, 1, STD::memory_order_seq_cst);
    test_pass("cxx26:atomic_store_add_explicit");
    STD::atomic_store_sub(&b, 1);            test_pass("cxx26:atomic_store_sub");
    STD::atomic_store_sub_explicit(&b, 1, STD::memory_order_seq_cst);
    test_pass("cxx26:atomic_store_sub_explicit");
    STD::atomic_store_and(&b, 3);            test_pass("cxx26:atomic_store_and");
    STD::atomic_store_and_explicit(&b, 3, STD::memory_order_seq_cst);
    test_pass("cxx26:atomic_store_and_explicit");
    STD::atomic_store_or(&b, 4);             test_pass("cxx26:atomic_store_or");
    STD::atomic_store_or_explicit(&b, 4, STD::memory_order_seq_cst);
    test_pass("cxx26:atomic_store_or_explicit");
    STD::atomic_store_xor(&b, 1);            test_pass("cxx26:atomic_store_xor");
    STD::atomic_store_xor_explicit(&b, 1, STD::memory_order_seq_cst);
    test_pass("cxx26:atomic_store_xor_explicit");
    STD::atomic_store_max(&b, 9);            test_pass("cxx26:atomic_store_max");
    STD::atomic_store_max_explicit(&b, 9, STD::memory_order_seq_cst);
    test_pass("cxx26:atomic_store_max_explicit");
    STD::atomic_store_min(&b, 0);            test_pass("cxx26:atomic_store_min");
    STD::atomic_store_min_explicit(&b, 0, STD::memory_order_seq_cst);
    test_pass("cxx26:atomic_store_min_explicit");
#else
    TEST_NOTE("no atomic_store_* read-modify-writes in any library here");
    test_skip("cxx26:atomic_store_add");
    test_skip("cxx26:atomic_store_add_explicit");
    test_skip("cxx26:atomic_store_sub");
    test_skip("cxx26:atomic_store_sub_explicit");
    test_skip("cxx26:atomic_store_and");
    test_skip("cxx26:atomic_store_and_explicit");
    test_skip("cxx26:atomic_store_or");
    test_skip("cxx26:atomic_store_or_explicit");
    test_skip("cxx26:atomic_store_xor");
    test_skip("cxx26:atomic_store_xor_explicit");
    test_skip("cxx26:atomic_store_max");
    test_skip("cxx26:atomic_store_max_explicit");
    test_skip("cxx26:atomic_store_min");
    test_skip("cxx26:atomic_store_min_explicit");
#endif
}
