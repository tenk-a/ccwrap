#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <atomic>
#if _TST_HAS_OS_THREADS
#include <thread>
#endif
#include <type_traits>
#include <cstddef>
#include <cstdint>

namespace {
template <class _Want, class _Got>
bool same_type(const _Got&) { return STD::is_same<_Want, _Got>::value; }

struct RefWaiter {
    int*              shared;
    STD::atomic<int>* seen;
    RefWaiter(int* a, STD::atomic<int>* b) : shared(a), seen(b) {}
    void operator()() const {
        STD::atomic_ref<int> w(*shared);
        w.wait(0);
        seen->store(w.load());
    }
};

struct FlagWaiter {
    STD::atomic_flag*  g;
    STD::atomic<bool>* done;
    FlagWaiter(STD::atomic_flag* a, STD::atomic<bool>* b) : g(a), done(b) {}
    void operator()() const {
        STD::atomic_flag_wait(g, false);
        done->store(true);
    }
};

struct Waiter {
    STD::atomic<int>* v;
    STD::atomic<int>* seen;
    Waiter(STD::atomic<int>* a, STD::atomic<int>* b) : v(a), seen(b) {}
    void operator()() const {
        STD::atomic_wait(v, 0);
        seen->store(STD::atomic_load(v));
    }
};

struct Publisher {
    int*              data;
    STD::atomic<int>* ready;
    Publisher(int* d, STD::atomic<int>* r) : data(d), ready(r) {}
    void operator()() const {
        *data = 7;
        STD::atomic_thread_fence(STD::memory_order_release);
        ready->store(1, STD::memory_order_relaxed);
    }
};
}

TEST_CASE(atomic, memory_order_enumerators) {
#if _TST_HAS_CXX11_LIB_MEMBERS && !defined(__WATCOMC__)
    static_assert(STD::is_enum<STD::memory_order>::value, "");
#endif

#if _TST_ATOMIC_LIB_CXX20
    test_true( STD::memory_order_relaxed == STD::memory_order::relaxed );
    test_true( STD::memory_order_consume == STD::memory_order::consume );
    test_true( STD::memory_order_acquire == STD::memory_order::acquire );
    test_true( STD::memory_order_release == STD::memory_order::release );
    test_true( STD::memory_order_acq_rel == STD::memory_order::acq_rel );
    test_true( STD::memory_order_seq_cst == STD::memory_order::seq_cst );
    test_pass("cxx20:memory_order scoped enumerators");
#else
    TEST_SKIP_N(6);
    test_skip("cxx20:memory_order scoped enumerators");
#endif

    test_true( STD::memory_order_relaxed != STD::memory_order_consume );
    test_true( STD::memory_order_consume != STD::memory_order_acquire );
    test_true( STD::memory_order_acquire != STD::memory_order_release );
    test_true( STD::memory_order_release != STD::memory_order_acq_rel );
    test_true( STD::memory_order_acq_rel != STD::memory_order_seq_cst );
    test_true( STD::memory_order_seq_cst != STD::memory_order_relaxed );
    test_pass("cxx11:memory_order distinct values");
}

TEST_CASE(atomic, memory_order_on_every_operation) {

    STD::atomic<int> a(1);

    int v0 = a.load(STD::memory_order_relaxed);
    test_eq( v0, 1 );
    int v1 = a.load(STD::memory_order_consume);
    test_eq( v1, 1 );
    int v2 = a.load(STD::memory_order_acquire);
    test_eq( v2, 1 );
    int v3 = a.load(STD::memory_order_seq_cst);
    test_eq( v3, 1 );
    test_pass("cxx11:atomic::load accepts every memory_order");

    a.store(2, STD::memory_order_relaxed);
    a.store(3, STD::memory_order_release);
    a.store(4, STD::memory_order_seq_cst);
    int v4 = a.load();
    test_eq( v4, 4 );
    test_pass("cxx11:atomic::store accepts every memory_order");

    int x0 = a.exchange(5, STD::memory_order_relaxed);
    test_eq( x0, 4 );
    int x1 = a.fetch_add(1, STD::memory_order_acquire);
    test_eq( x1, 5 );
    int x2 = a.fetch_add(1, STD::memory_order_release);
    test_eq( x2, 6 );
    int x3 = a.fetch_add(1, STD::memory_order_acq_rel);
    test_eq( x3, 7 );
    int x4 = a.fetch_add(1, STD::memory_order_seq_cst);
    test_eq( x4, 8 );
    int v5 = a.load();
    test_eq( v5, 9 );
    test_pass("cxx11:atomic RMW accepts every memory_order");

    int e1 = 9;
    bool ok1 = a.compare_exchange_strong(e1, 10,
                   STD::memory_order_acq_rel, STD::memory_order_consume);
    test_true( ok1 );
    int e2 = 10;
    bool ok2 = a.compare_exchange_strong(e2, 11,
                   STD::memory_order_release, STD::memory_order_relaxed);
    test_true( ok2 );
    int e3 = 11;
    bool ok3 = a.compare_exchange_strong(e3, 12,
                   STD::memory_order_seq_cst, STD::memory_order_acquire);
    test_true( ok3 );
    int e4 = 12;
#if _TST_ATOMIC_LIB_CXX17
    bool ok4 = a.compare_exchange_strong(e4, 13,
                   STD::memory_order_relaxed, STD::memory_order_seq_cst);
#else
    bool ok4 = a.compare_exchange_strong(e4, 13,
                   STD::memory_order_seq_cst, STD::memory_order_relaxed);
#endif
    test_true( ok4 );
    int v6 = a.load();
    test_eq( v6, 13 );
    test_pass("cxx11:compare_exchange accepts distinct success+failure orders");
}

TEST_CASE(atomic, kill_dependency) {

    int v = 42;
    int r = STD::kill_dependency(v);
    test_eq( r, 42 );
    test_pass("cxx11:kill_dependency (int)");

    STD::atomic<int*> p(&v);
    int* q = STD::kill_dependency(p.load(STD::memory_order_consume));
    test_eq( *q, 42 );
    test_pass("cxx11:kill_dependency (pointer via consume load)");

    double d = STD::kill_dependency(1.5);
    test_eq( d, 1.5 );
    test_pass("cxx11:kill_dependency (double)");

#if _tst_cplusplus >= 201103L
    static_assert((STD::is_same<decltype(STD::kill_dependency(1.5)), double>::value), "");
    static_assert((STD::is_same<decltype(STD::kill_dependency(1)), int>::value), "");
#else

    test_true( same_type<double>(STD::kill_dependency(1.5)) );
    test_true( same_type<int>(STD::kill_dependency(1)) );
    test_pass("cxx11:kill_dependency return type (decay)");
#endif
}

#if _TST_HAS_OS_THREADS
TEST_CASE(atomic, fences) {
    STD::atomic<int> a(0);

    a.store(1, STD::memory_order_relaxed);
    test_no_throw( STD::atomic_thread_fence(STD::memory_order_release) );
    test_no_throw( STD::atomic_thread_fence(STD::memory_order_acquire) );
    test_no_throw( STD::atomic_thread_fence(STD::memory_order_acq_rel) );
    test_no_throw( STD::atomic_thread_fence(STD::memory_order_seq_cst) );
    test_no_throw( STD::atomic_thread_fence(STD::memory_order_relaxed) );
    test_no_throw( STD::atomic_thread_fence(STD::memory_order_consume) );
    test_pass("cxx11:atomic_thread_fence");

    test_no_throw( STD::atomic_signal_fence(STD::memory_order_release) );
    test_no_throw( STD::atomic_signal_fence(STD::memory_order_acquire) );
    test_no_throw( STD::atomic_signal_fence(STD::memory_order_acq_rel) );
    test_no_throw( STD::atomic_signal_fence(STD::memory_order_seq_cst) );
    test_no_throw( STD::atomic_signal_fence(STD::memory_order_relaxed) );
    test_pass("cxx11:atomic_signal_fence");

    int v = a.load(STD::memory_order_relaxed);
    test_eq( v, 1 );
    test_pass("cxx11:atomic_thread_fence relaxed load");

    int data = 0;
    STD::atomic<int> ready(0);
    STD::thread t(Publisher(&data, &ready));
    t.join();
    STD::atomic_thread_fence(STD::memory_order_acquire);
    int rd = ready.load(STD::memory_order_relaxed);
    test_eq( rd, 1 );
    test_eq( data, 7 );
    test_pass("cxx11:atomic_thread_fence release/acquire hand-off");
}
#else
TEST_CASE_SKIP(atomic, fences)
#endif

TEST_CASE(atomic, lock_free_macros) {

    int m[] = {
        ATOMIC_BOOL_LOCK_FREE,
        ATOMIC_CHAR_LOCK_FREE,
        ATOMIC_CHAR8_T_LOCK_FREE,
        ATOMIC_CHAR16_T_LOCK_FREE,
        ATOMIC_CHAR32_T_LOCK_FREE,
        ATOMIC_WCHAR_T_LOCK_FREE,
        ATOMIC_SHORT_LOCK_FREE,
        ATOMIC_INT_LOCK_FREE,
        ATOMIC_LONG_LOCK_FREE,
        ATOMIC_LLONG_LOCK_FREE,
        ATOMIC_POINTER_LOCK_FREE
    };
    for (STD::size_t i = 0; i < sizeof m / sizeof m[0]; ++i)
        test_lim( m[i], 0, 2 );
    test_pass("cxx11:ATOMIC_*_LOCK_FREE macros");

#if _TST_ATOMIC_LIB_CXX17
#if ATOMIC_INT_LOCK_FREE == 2
    test_true( STD::atomic<int>::is_always_lock_free );
#else
    test_true( !STD::atomic<int>::is_always_lock_free );
#endif
#if ATOMIC_POINTER_LOCK_FREE == 2
    test_true( STD::atomic<void*>::is_always_lock_free );
#else
    test_true( !STD::atomic<void*>::is_always_lock_free );
#endif
#if ATOMIC_BOOL_LOCK_FREE == 2
    test_true( STD::atomic<bool>::is_always_lock_free );
#else
    test_true( !STD::atomic<bool>::is_always_lock_free );
#endif
    test_pass("cxx11:ATOMIC_*_LOCK_FREE agree with is_always_lock_free");
#else
    TEST_SKIP_N(3);
    test_skip("cxx11:ATOMIC_*_LOCK_FREE agree with is_always_lock_free");
#endif
}

TEST_CASE(atomic, flag_init_and_var_init) {

    STD::atomic_flag fg;
#if !_TST_ATOMIC_LIB_CXX20
    fg.clear();
#endif
    bool prev = fg.test_and_set();
    test_true( !prev );
    test_pass("cxx11:atomic_flag::test_and_set");

    fg.clear();
#if _TST_ATOMIC_LIB_CXX20
    bool t = fg.test();
    test_true( !t );
    test_pass("cxx20:atomic_flag::test");
#else
    TEST_SKIP1();
    test_skip("cxx20:atomic_flag::test");
#endif

#if _TST_HAS_INIT_LIST
    STD::atomic<int> a = ATOMIC_VAR_INIT(5);
#else
    STD::atomic<int> a(5);
#endif
    int v = a.load();
    test_eq( v, 5 );
    test_pass("cxx11:ATOMIC_VAR_INIT");
}

TEST_CASE(atomic, type_aliases_builtin) {
#if !(defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900)
    static_assert((STD::is_same<STD::atomic_bool,     STD::atomic<bool> >::value), "");
    static_assert((STD::is_same<STD::atomic_char,     STD::atomic<char> >::value), "");
    static_assert((STD::is_same<STD::atomic_schar,    STD::atomic<signed char> >::value), "");
    static_assert((STD::is_same<STD::atomic_uchar,    STD::atomic<unsigned char> >::value), "");
    static_assert((STD::is_same<STD::atomic_short,    STD::atomic<short> >::value), "");
    static_assert((STD::is_same<STD::atomic_ushort,   STD::atomic<unsigned short> >::value), "");
    static_assert((STD::is_same<STD::atomic_int,      STD::atomic<int> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint,     STD::atomic<unsigned int> >::value), "");
    static_assert((STD::is_same<STD::atomic_long,     STD::atomic<long> >::value), "");
    static_assert((STD::is_same<STD::atomic_ulong,    STD::atomic<unsigned long> >::value), "");
    static_assert((STD::is_same<STD::atomic_llong,    STD::atomic<long long> >::value), "");
    static_assert((STD::is_same<STD::atomic_ullong,   STD::atomic<unsigned long long> >::value), "");
    static_assert((STD::is_same<STD::atomic_char16_t, STD::atomic<char16_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_char32_t, STD::atomic<char32_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_wchar_t,  STD::atomic<wchar_t> >::value), "");
#endif
#if _tst_cplusplus >= 202002L
    static_assert((STD::is_same<STD::atomic_char8_t,  STD::atomic<char8_t> >::value), "");
#endif
    test_true( 1 );
    test_pass("cxx11:atomic_* builtin type aliases");

#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    STD::atomic_int i; i.store(1);
#else
    STD::atomic_int i(1);
#endif
    int io = i.fetch_add(1);
    test_eq( io, 1 );
    int iv = i.load();
    test_eq( iv, 2 );
    test_pass("cxx11:atomic_int usable");

#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    STD::atomic_bool b; b.store(false);
#else
    STD::atomic_bool b(false);
#endif
    bool bo = b.exchange(true);
    test_true( !bo );
    bool bv = b.load();
    test_true( bv );
    test_pass("cxx11:atomic_bool usable");

#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    STD::atomic_ullong u; u.store(0);
#else
    STD::atomic_ullong u(0);
#endif
    unsigned long long uo = u.fetch_add(3);
    test_eq( uo, 0ull );
    unsigned long long uv = u.load();
    test_eq( uv, 3ull );
    test_pass("cxx11:atomic_ullong usable");

#if _TST_HAS_CHAR8_NAME
#  if _TST_HAS_CHAR8_T
    STD::atomic_char8_t c8(u8'a');
#  else
    STD::atomic_char8_t c8((char8_t)'a');
#  endif
    char8_t c8v = c8.load();
    test_eq( (int)c8v, (int)'a' );
    test_pass("cxx20:atomic_char8_t usable");
#else
    TEST_SKIP1();
    test_skip("cxx20:atomic_char8_t usable");
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    STD::atomic_wchar_t wc; wc.store(L'z');
#else
    STD::atomic_wchar_t wc(L'z');
#endif
    wchar_t wcv = wc.load();
    test_eq( (int)wcv, (int)L'z' );
    test_pass("cxx11:atomic_wchar_t usable");
}

TEST_CASE(atomic, type_aliases_stdint) {
    static_assert((STD::is_same<STD::atomic_int8_t,   STD::atomic<STD::int8_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint8_t,  STD::atomic<STD::uint8_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int16_t,  STD::atomic<STD::int16_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint16_t, STD::atomic<STD::uint16_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int32_t,  STD::atomic<STD::int32_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint32_t, STD::atomic<STD::uint32_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int64_t,  STD::atomic<STD::int64_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint64_t, STD::atomic<STD::uint64_t> >::value), "");

    static_assert((STD::is_same<STD::atomic_int_least8_t,   STD::atomic<STD::int_least8_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_least8_t,  STD::atomic<STD::uint_least8_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int_least16_t,  STD::atomic<STD::int_least16_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_least16_t, STD::atomic<STD::uint_least16_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int_least32_t,  STD::atomic<STD::int_least32_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_least32_t, STD::atomic<STD::uint_least32_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int_least64_t,  STD::atomic<STD::int_least64_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_least64_t, STD::atomic<STD::uint_least64_t> >::value), "");

    static_assert((STD::is_same<STD::atomic_int_fast8_t,   STD::atomic<STD::int_fast8_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_fast8_t,  STD::atomic<STD::uint_fast8_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int_fast16_t,  STD::atomic<STD::int_fast16_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_fast16_t, STD::atomic<STD::uint_fast16_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int_fast32_t,  STD::atomic<STD::int_fast32_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_fast32_t, STD::atomic<STD::uint_fast32_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_int_fast64_t,  STD::atomic<STD::int_fast64_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uint_fast64_t, STD::atomic<STD::uint_fast64_t> >::value), "");

    static_assert((STD::is_same<STD::atomic_intptr_t,  STD::atomic<STD::intptr_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uintptr_t, STD::atomic<STD::uintptr_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_size_t,    STD::atomic<STD::size_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_ptrdiff_t, STD::atomic<STD::ptrdiff_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_intmax_t,  STD::atomic<STD::intmax_t> >::value), "");
    static_assert((STD::is_same<STD::atomic_uintmax_t, STD::atomic<STD::uintmax_t> >::value), "");
    test_true( 1 );
    test_pass("cxx11:atomic_* stdint type aliases");

    STD::atomic_size_t s(0);
    STD::size_t so = s.fetch_add(3);
    test_eq( so, STD::size_t(0) );
    STD::size_t sv = s.load();
    test_eq( sv, STD::size_t(3) );
    test_pass("cxx11:atomic_size_t usable");

    STD::atomic_ptrdiff_t d(10);
    STD::ptrdiff_t dobs = d.fetch_sub(4);
    test_eq( dobs, STD::ptrdiff_t(10) );
    STD::ptrdiff_t dv = d.load();
    test_eq( dv, STD::ptrdiff_t(6) );
    test_pass("cxx11:atomic_ptrdiff_t usable");

    STD::atomic_int32_t i32(0x0F);
    STD::int32_t i32o = i32.fetch_or(0xF0);
    test_eq( i32o, STD::int32_t(0x0F) );
    STD::int32_t i32v = i32.load();
    test_eq( i32v, STD::int32_t(0xFF) );
    test_pass("cxx11:atomic_int32_t usable");

    STD::atomic_uint64_t u64(0);
    unsigned long long u64o = u64.fetch_add(1ull << 40);
    test_eq( u64o, 0ull );
    unsigned long long u64v = u64.load();
    test_eq( u64v, 1ull << 40 );
    test_pass("cxx11:atomic_uint64_t usable");

    STD::atomic_int_fast16_t f16(1);
    STD::int_fast16_t f16v = ++f16;
    test_eq( (long)f16v, 2L );
    test_pass("cxx11:atomic_int_fast16_t usable");

    STD::atomic_uintmax_t um(7);
    STD::uintmax_t umv = um.load();
    test_eq( umv, STD::uintmax_t(7) );
    test_pass("cxx11:atomic_uintmax_t usable");
}

TEST_CASE(atomic, nonmember_load_store_exchange) {
    STD::atomic<int> a(1);

    bool lf = STD::atomic_is_lock_free(&a);
    test_true( lf );
    test_pass("cxx11:atomic_is_lock_free");

    STD::atomic_store(&a, 5);
    test_true( 1 );
    test_pass("cxx11:atomic_store");
    int v1 = STD::atomic_load(&a);
    test_eq( v1, 5 );
    test_pass("cxx11:atomic_load");

    STD::atomic_store_explicit(&a, 6, STD::memory_order_release);
    test_true( 1 );
    test_pass("cxx11:atomic_store_explicit");
    int v2 = STD::atomic_load_explicit(&a, STD::memory_order_acquire);
    test_eq( v2, 6 );
    test_pass("cxx11:atomic_load_explicit");

    int old = STD::atomic_exchange(&a, 7);
    test_eq( old, 6 );
    test_pass("cxx11:atomic_exchange");
    int old2 = STD::atomic_exchange_explicit(&a, 8, STD::memory_order_acq_rel);
    test_eq( old2, 7 );
    int v3 = STD::atomic_load(&a);
    test_eq( v3, 8 );
    test_pass("cxx11:atomic_exchange_explicit");

    static int arr[4] = { 0, 1, 2, 3 };
    STD::atomic<int*> p(arr);
    STD::atomic_store(&p, arr + 1);
    test_true( 1 );
    test_pass("cxx11:atomic_store (ptr)");
    int* l = STD::atomic_load(&p);
    test_eq( l - arr, STD::ptrdiff_t(1) );
    test_pass("cxx11:atomic_load (ptr)");
    int* pold = STD::atomic_exchange_explicit(&p, arr + 2, STD::memory_order_acq_rel);
    test_eq( pold - arr, STD::ptrdiff_t(1) );
    test_pass("cxx11:atomic_exchange_explicit (ptr)");
    test_true( STD::atomic_is_lock_free(&p) );
    test_pass("cxx11:atomic_is_lock_free (ptr)");
}

TEST_CASE(atomic, nonmember_compare_exchange) {
    STD::atomic<int> a(10);

    int e1 = 10;
    bool ok1 = STD::atomic_compare_exchange_strong(&a, &e1, 11);
    test_true( ok1 );
    test_eq( e1, 10 );
    int v1 = STD::atomic_load(&a);
    test_eq( v1, 11 );
    test_pass("cxx11:atomic_compare_exchange_strong");

    int e2 = 0;
    bool no = STD::atomic_compare_exchange_strong(&a, &e2, 12);
    test_true( !no );
    test_eq( e2, 11 );
    int v2 = STD::atomic_load(&a);
    test_eq( v2, 11 );
    test_pass("cxx11:atomic_compare_exchange_strong (fail refreshes expected)");

    int e3 = 11;
    bool ok3 = STD::atomic_compare_exchange_strong_explicit(&a, &e3, 12,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok3 );
    int v3 = STD::atomic_load(&a);
    test_eq( v3, 12 );
    test_pass("cxx11:atomic_compare_exchange_strong_explicit");

    int e4 = 12;
    while (!STD::atomic_compare_exchange_weak(&a, &e4, 13)) { }
    int v4 = STD::atomic_load(&a);
    test_eq( v4, 13 );
    test_pass("cxx11:atomic_compare_exchange_weak");

    int e5 = 13;
    while (!STD::atomic_compare_exchange_weak_explicit(&a, &e5, 14,
               STD::memory_order_seq_cst, STD::memory_order_relaxed)) { }
    int v5 = STD::atomic_load(&a);
    test_eq( v5, 14 );
    test_pass("cxx11:atomic_compare_exchange_weak_explicit");
}

TEST_CASE(atomic, nonmember_fetch_ops) {
    STD::atomic<int> a(0);

    int f1 = STD::atomic_fetch_add(&a, 5);
    test_eq( f1, 0 );
    test_pass("cxx11:atomic_fetch_add");
    int f2 = STD::atomic_fetch_add_explicit(&a, 5, STD::memory_order_relaxed);
    test_eq( f2, 5 );
    int v0 = STD::atomic_load(&a);
    test_eq( v0, 10 );
    test_pass("cxx11:atomic_fetch_add_explicit");

    {   int buf[8];
        STD::atomic<int*> p(buf);
        int* q = STD::atomic_fetch_add_explicit(&p, (STD::ptrdiff_t)2,
                                                STD::memory_order_relaxed);
        test_true( q == buf );
        test_true( STD::atomic_load(&p) == buf + 2 );
        STD::atomic_fetch_add_explicit(&p, (STD::ptrdiff_t)0,
                                       STD::memory_order_relaxed);
        test_true( STD::atomic_load(&p) == buf + 2 ); }
    test_pass("cxx11:atomic_fetch_add_explicit(atomic<T*>*, ptrdiff_t, memory_order)");

    int f3 = STD::atomic_fetch_sub(&a, 3);
    test_eq( f3, 10 );
    test_pass("cxx11:atomic_fetch_sub");
    int f4 = STD::atomic_fetch_sub_explicit(&a, 3, STD::memory_order_acq_rel);
    test_eq( f4, 7 );
    int v1 = STD::atomic_load(&a);
    test_eq( v1, 4 );
    test_pass("cxx11:atomic_fetch_sub_explicit");

    STD::atomic_store(&a, 0xF0);
    int g1 = STD::atomic_fetch_and(&a, 0x3C);
    test_eq( g1, 0xF0 );
    test_pass("cxx11:atomic_fetch_and");
    int g2 = STD::atomic_fetch_and_explicit(&a, 0x10, STD::memory_order_seq_cst);
    test_eq( g2, 0x30 );
    int v2 = STD::atomic_load(&a);
    test_eq( v2, 0x10 );
    test_pass("cxx11:atomic_fetch_and_explicit");

    int h1 = STD::atomic_fetch_or(&a, 0x01);
    test_eq( h1, 0x10 );
    test_pass("cxx11:atomic_fetch_or");
    int h2 = STD::atomic_fetch_or_explicit(&a, 0x02, STD::memory_order_release);
    test_eq( h2, 0x11 );
    int v3 = STD::atomic_load(&a);
    test_eq( v3, 0x13 );
    test_pass("cxx11:atomic_fetch_or_explicit");

    int x1 = STD::atomic_fetch_xor(&a, 0x03);
    test_eq( x1, 0x13 );
    test_pass("cxx11:atomic_fetch_xor");
    int x2 = STD::atomic_fetch_xor_explicit(&a, 0xFF, STD::memory_order_acquire);
    test_eq( x2, 0x10 );
    int v4 = STD::atomic_load(&a);
    test_eq( v4, 0xEF );
    test_pass("cxx11:atomic_fetch_xor_explicit");

    static int arr[4] = { 0, 1, 2, 3 };
    STD::atomic<int*> p(arr);
    int* pa = STD::atomic_fetch_add(&p, 2);
    test_eq( pa - arr, STD::ptrdiff_t(0) );
    test_pass("cxx11:atomic_fetch_add (ptr)");
    int* ps = STD::atomic_fetch_sub_explicit(&p, 1, STD::memory_order_relaxed);
    test_eq( ps - arr, STD::ptrdiff_t(2) );
    int* cur = STD::atomic_load(&p);
    test_eq( cur - arr, STD::ptrdiff_t(1) );
    test_pass("cxx11:atomic_fetch_sub_explicit (ptr)");
}

#if _TST_HAS_OS_THREADS
TEST_CASE(atomic, nonmember_wait_notify) {
    STD::atomic<int> a(1);
    test_no_throw( STD::atomic_wait(&a, 0) );
    test_no_throw( STD::atomic_wait_explicit(&a, 0, STD::memory_order_acquire) );
    test_pass("cxx20:atomic_wait (nonmember, non-blocking)");
    test_no_throw( STD::atomic_notify_one(&a) );
    test_no_throw( STD::atomic_notify_all(&a) );
    test_pass("cxx20:atomic_notify (nonmember, non-blocking)");

    STD::atomic<int> v(0);
    STD::atomic<int> seen(-1);
    STD::thread t(Waiter(&v, &seen));
    STD::atomic_store(&v, 42);
    STD::atomic_notify_all(&v);
    t.join();
    int s = seen.load();
    test_eq( s, 42 );
    test_pass("cxx20:atomic_wait + atomic_notify_all threaded (nonmember)");
}
#else
TEST_CASE_SKIP(atomic, nonmember_wait_notify)
#endif

#if _TST_HAS_OS_THREADS
TEST_CASE(atomic, nonmember_flag_ops) {

    STD::atomic_flag fg;
#if !_TST_ATOMIC_LIB_CXX20
    STD::atomic_flag_clear(&fg);
#endif

    bool t0 = STD::atomic_flag_test(&fg);
    test_true( !t0 );
    test_pass("cxx20:atomic_flag_test");

    bool p0 = STD::atomic_flag_test_and_set(&fg);
    test_true( !p0 );
    test_pass("cxx11:atomic_flag_test_and_set");
    bool t1 = STD::atomic_flag_test_explicit(&fg, STD::memory_order_acquire);
    test_true( t1 );
    test_pass("cxx20:atomic_flag_test_explicit");

    bool p1 = STD::atomic_flag_test_and_set_explicit(&fg, STD::memory_order_acq_rel);
    test_true( p1 );
    test_pass("cxx11:atomic_flag_test_and_set_explicit");

    STD::atomic_flag_clear(&fg);
    bool t2 = STD::atomic_flag_test(&fg);
    test_true( !t2 );
    test_pass("cxx11:atomic_flag_clear");

    STD::atomic_flag_test_and_set(&fg);
    STD::atomic_flag_clear_explicit(&fg, STD::memory_order_release);
    bool t3 = STD::atomic_flag_test_explicit(&fg, STD::memory_order_relaxed);
    test_true( !t3 );
    test_pass("cxx11:atomic_flag_clear_explicit");

    STD::atomic_flag_test_and_set(&fg);
    test_no_throw( STD::atomic_flag_wait(&fg, false) );
    test_no_throw( STD::atomic_flag_wait_explicit(&fg, false, STD::memory_order_acquire) );
    test_pass("cxx20:atomic_flag_wait (nonmember)");
    test_no_throw( STD::atomic_flag_notify_one(&fg) );
    test_no_throw( STD::atomic_flag_notify_all(&fg) );
    test_pass("cxx20:atomic_flag_notify (nonmember)");

    STD::atomic_flag g;
#if !_TST_ATOMIC_LIB_CXX20
    STD::atomic_flag_clear(&g);
#endif
    STD::atomic<bool> done(false);
    STD::thread th(FlagWaiter(&g, &done));
    STD::atomic_flag_test_and_set(&g);
    STD::atomic_flag_notify_all(&g);
    th.join();
    bool d = done.load();
    test_true( d );
    test_pass("cxx20:atomic_flag threaded hand-off (nonmember)");
}
#else
TEST_CASE_SKIP(atomic, nonmember_flag_ops)
#endif

TEST_CASE(atomic, atomic_ref_types_and_basics) {
    typedef STD::atomic_ref<int> AR;

    static_assert((STD::is_same<AR::value_type, int>::value), "");
    test_true( 1 );
    test_pass("cxx20:atomic_ref<int>::value_type");
    static_assert((STD::is_same<AR::difference_type, int>::value), "");
    test_true( 1 );
    test_pass("cxx20:atomic_ref<int>::difference_type");
    static_assert(AR::required_alignment >= alignof(int), "");
    test_true( 1 );
    test_pass("cxx20:atomic_ref<int>::required_alignment");
    static_assert(AR::is_always_lock_free, "");
    test_true( AR::is_always_lock_free );
    test_pass("cxx20:atomic_ref<int>::is_always_lock_free");

#if _TST_HAS_CXX11_LIB_MEMBERS && !defined(__WATCOMC__) && !(defined(_MSC_VER) && _MSC_VER < 1900)
    static_assert(STD::is_copy_constructible<AR>::value, "");
    static_assert(!STD::is_copy_assignable<AR>::value, "");
#endif
    test_true( 1 );
    test_pass("cxx20:atomic_ref<int> is copy-constructible, not copy-assignable");

    #if _tst_cplusplus >= 201103L

    #if _tst_cplusplus >= 201103L

    alignas(AR::required_alignment) int obj = 1;

    #else

    int obj = 1;

    #endif

    #else

    int obj = 1;

    #endif
    AR r(obj);
    AR r2(r);
    test_true( r.is_lock_free() );
    test_pass("cxx20:atomic_ref<int>(T&) + copy ctor + is_lock_free");

    r.store(5);
    test_eq( obj, 5 );
    test_pass("cxx20:atomic_ref<int>::store");
    int l = r2.load();
    test_eq( l, 5 );
    test_pass("cxx20:atomic_ref<int>::load");

    r.store(6, STD::memory_order_release);
    test_true( 1 );
    test_pass("cxx20:atomic_ref<int>::store (memory_order)");
    int l2 = r.load(STD::memory_order_acquire);
    test_eq( l2, 6 );
    test_pass("cxx20:atomic_ref<int>::load (memory_order)");

    int conv = r;
    test_eq( conv, 6 );
    test_pass("cxx20:atomic_ref<int>::operator T");

    int assigned = (r = 7);
    test_eq( assigned, 7 );
    test_eq( obj, 7 );
    test_pass("cxx20:atomic_ref<int>::operator=(T)");

    int old = r.exchange(8);
    test_eq( old, 7 );
    int old2 = r.exchange(9, STD::memory_order_acq_rel);
    test_eq( old2, 8 );
    test_eq( obj, 9 );
    test_pass("cxx20:atomic_ref<int>::exchange");
}

TEST_CASE(atomic, atomic_ref_compare_exchange) {
    #if _tst_cplusplus >= 201103L
    #if _tst_cplusplus >= 201103L
    alignas(STD::atomic_ref<int>::required_alignment) int obj = 10;
    #else
    int obj = 10;
    #endif
    #else
    int obj = 10;
    #endif
    STD::atomic_ref<int> r(obj);

    int e1 = 10;
    bool ok1 = r.compare_exchange_strong(e1, 11);
    test_true( ok1 );
    test_eq( e1, 10 );
    test_eq( obj, 11 );
    test_pass("cxx20:atomic_ref<int>::compare_exchange_strong");

    int e2 = 0;
    bool no = r.compare_exchange_strong(e2, 99, STD::memory_order_seq_cst);
    test_true( !no );
    test_eq( e2, 11 );
    test_eq( obj, 11 );
    test_pass("cxx20:atomic_ref<int>::compare_exchange_strong (fail refreshes expected)");

    int e3 = 11;
    bool ok3 = r.compare_exchange_strong(e3, 12, STD::memory_order_relaxed);
    test_true( ok3 );
    test_pass("cxx20:atomic_ref<int>::compare_exchange_strong (3-arg)");

    int e4 = 12;
    bool ok4 = r.compare_exchange_strong(e4, 13,
                   STD::memory_order_acq_rel, STD::memory_order_acquire);
    test_true( ok4 );
    test_eq( obj, 13 );
    test_pass("cxx20:atomic_ref<int>::compare_exchange_strong (4-arg)");

    int e5 = 13;
    while (!r.compare_exchange_weak(e5, 14)) { }
    test_eq( obj, 14 );
    test_pass("cxx20:atomic_ref<int>::compare_exchange_weak");

    int e6 = 14;
    while (!r.compare_exchange_weak(e6, 15, STD::memory_order_seq_cst)) { }
    test_eq( obj, 15 );
    test_pass("cxx20:atomic_ref<int>::compare_exchange_weak (3-arg)");

    int e7 = 15;
    while (!r.compare_exchange_weak(e7, 16,
               STD::memory_order_acq_rel, STD::memory_order_relaxed)) { }
    test_eq( obj, 16 );
    test_pass("cxx20:atomic_ref<int>::compare_exchange_weak (4-arg)");
}

TEST_CASE(atomic, atomic_ref_fetch_and_operators) {
    #if _tst_cplusplus >= 201103L
    #if _tst_cplusplus >= 201103L
    alignas(STD::atomic_ref<int>::required_alignment) int obj = 0;
    #else
    int obj = 0;
    #endif
    #else
    int obj = 0;
    #endif
    STD::atomic_ref<int> r(obj);

    int a1 = r.fetch_add(5);
    test_eq( a1, 0 );
    int a2 = r.fetch_add(5, STD::memory_order_relaxed);
    test_eq( a2, 5 );
    test_pass("cxx20:atomic_ref<int>::fetch_add");
    int s1 = r.fetch_sub(3);
    test_eq( s1, 10 );
    int s2 = r.fetch_sub(3, STD::memory_order_acq_rel);
    test_eq( s2, 7 );
    test_eq( obj, 4 );
    test_pass("cxx20:atomic_ref<int>::fetch_sub");

    r.store(0xF0);
    int n1 = r.fetch_and(0x3C);
    test_eq( n1, 0xF0 );
    int n2 = r.fetch_and(0x10, STD::memory_order_seq_cst);
    test_eq( n2, 0x30 );
    test_pass("cxx20:atomic_ref<int>::fetch_and");
    int o1 = r.fetch_or(0x01);
    test_eq( o1, 0x10 );
    int o2 = r.fetch_or(0x02, STD::memory_order_release);
    test_eq( o2, 0x11 );
    test_pass("cxx20:atomic_ref<int>::fetch_or");
    int x1 = r.fetch_xor(0x03);
    test_eq( x1, 0x13 );
    int x2 = r.fetch_xor(0xFF, STD::memory_order_acquire);
    test_eq( x2, 0x10 );
    test_eq( obj, 0xEF );
    test_pass("cxx20:atomic_ref<int>::fetch_xor");

    r.store(0);
    int pre = ++r;
    test_eq( pre, 1 );
    test_pass("cxx20:atomic_ref<int>::operator++ (pre)");
    int post = r++;
    test_eq( post, 1 );
    test_pass("cxx20:atomic_ref<int>::operator++ (post)");
    int dec = --r;
    test_eq( dec, 1 );
    test_pass("cxx20:atomic_ref<int>::operator-- (pre)");
    int pdec = r--;
    test_eq( pdec, 1 );
    test_eq( obj, 0 );
    test_pass("cxx20:atomic_ref<int>::operator-- (post)");

    int pa = (r += 10);
    test_eq( pa, 10 );
    test_pass("cxx20:atomic_ref<int>::operator+=");
    int ps = (r -= 4);
    test_eq( ps, 6 );
    test_pass("cxx20:atomic_ref<int>::operator-=");

    r.store(0xF0);
    int ba = (r &= 0x3C);
    test_eq( ba, 0x30 );
    test_pass("cxx20:atomic_ref<int>::operator&=");
    int bo = (r |= 0x0F);
    test_eq( bo, 0x3F );
    test_pass("cxx20:atomic_ref<int>::operator|=");
    int bx = (r ^= 0xFF);
    test_eq( bx, 0xC0 );
    test_eq( obj, 0xC0 );
    test_pass("cxx20:atomic_ref<int>::operator^=");
}

struct AtRefPair { int a; int b; };

TEST_CASE(atomic, atomic_ref_other_types) {

    #if _tst_cplusplus >= 201103L
    alignas(STD::atomic_ref<double>::required_alignment) double d = 1.0;
    #else
    double d = 1.0;
    #endif
    STD::atomic_ref<double> rd(d);
    static_assert((STD::is_same<STD::atomic_ref<double>::difference_type, double>::value), "");

    double f1 = rd.fetch_add(0.5);
    test_eq( f1, 1.0 );
    test_pass("cxx20:atomic_ref<double>::fetch_add");
    double f2 = rd.fetch_sub(0.25, STD::memory_order_relaxed);
    test_eq( f2, 1.5 );
    test_pass("cxx20:atomic_ref<double>::fetch_sub");
    double pp = (rd += 0.75);
    test_eq( pp, 2.0 );
    test_pass("cxx20:atomic_ref<double>::operator+=");
    double mm = (rd -= 1.0);
    test_eq( mm, 1.0 );
    test_eq( d, 1.0 );
    test_pass("cxx20:atomic_ref<double>::operator-=");

    static int arr[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
#if _tst_cplusplus >= 201103L
    alignas(STD::atomic_ref<int*>::required_alignment) int* ptr = arr;
#else
    int* ptr = arr;
#endif
    STD::atomic_ref<int*> rp(ptr);
    static_assert((STD::is_same<STD::atomic_ref<int*>::difference_type, STD::ptrdiff_t>::value), "");

    int* pa = rp.fetch_add(2);
    test_eq( pa - arr, STD::ptrdiff_t(0) );
    test_pass("cxx20:atomic_ref<int*>::fetch_add");
    int* pb = rp.fetch_sub(1, STD::memory_order_acq_rel);
    test_eq( pb - arr, STD::ptrdiff_t(2) );
    test_pass("cxx20:atomic_ref<int*>::fetch_sub");
    int* inc = ++rp;
    test_eq( inc - arr, STD::ptrdiff_t(2) );
    test_pass("cxx20:atomic_ref<int*>::operator++ (pre)");
    int* pst = rp++;
    test_eq( pst - arr, STD::ptrdiff_t(2) );
    test_pass("cxx20:atomic_ref<int*>::operator++ (post)");
    int* dcr = --rp;
    test_eq( dcr - arr, STD::ptrdiff_t(2) );
    test_pass("cxx20:atomic_ref<int*>::operator-- (pre)");
    int* pdc = rp--;
    test_eq( pdc - arr, STD::ptrdiff_t(2) );
    test_pass("cxx20:atomic_ref<int*>::operator-- (post)");
    int* pe = (rp += 3);
    test_eq( pe - arr, STD::ptrdiff_t(4) );
    test_pass("cxx20:atomic_ref<int*>::operator+=");
    int* me = (rp -= 2);
    test_eq( me - arr, STD::ptrdiff_t(2) );
    test_eq( ptr - arr, STD::ptrdiff_t(2) );
    test_pass("cxx20:atomic_ref<int*>::operator-=");

    typedef AtRefPair Pair;
#if _tst_cplusplus >= 201103L
    alignas(STD::atomic_ref<Pair>::required_alignment) Pair pr = { 1, 2 };
#else
    Pair pr = { 1, 2 };
#endif
    STD::atomic_ref<Pair> rs(pr);
    static_assert((STD::is_same<STD::atomic_ref<Pair>::value_type, Pair>::value), "");
    test_true( 1 );
    test_pass("cxx20:atomic_ref<T> traits (other types)");

    Pair got = rs.load(STD::memory_order_acquire);
    test_eq( got.a, 1 );
    test_eq( got.b, 2 );
    test_pass("cxx20:atomic_ref<struct>::load");

    Pair nv = { 3, 4 };
    Pair old = rs.exchange(nv);
    test_eq( old.b, 2 );
    test_pass("cxx20:atomic_ref<struct>::exchange");

    Pair exp = { 3, 4 };
    Pair des = { 5, 6 };
    bool ok = rs.compare_exchange_strong(exp, des);
    test_true( ok );
    test_eq( pr.a, 5 );
    test_eq( pr.b, 6 );
    test_pass("cxx20:atomic_ref<struct>::compare_exchange_strong");
}

#if _TST_HAS_OS_THREADS
TEST_CASE(atomic, atomic_ref_wait_notify) {
    #if _tst_cplusplus >= 201103L
    #if _tst_cplusplus >= 201103L
    alignas(STD::atomic_ref<int>::required_alignment) int obj = 1;
    #else
    int obj = 1;
    #endif
    #else
    int obj = 1;
    #endif
    STD::atomic_ref<int> r(obj);
    test_no_throw( r.wait(0) );
    test_no_throw( r.wait(0, STD::memory_order_acquire) );
    test_pass("cxx20:atomic_ref<int>::wait (non-blocking)");
    test_no_throw( r.notify_one() );
    test_no_throw( r.notify_all() );
    test_pass("cxx20:atomic_ref<int>::notify (non-blocking)");

    #if _tst_cplusplus >= 201103L

    #if _tst_cplusplus >= 201103L

    alignas(STD::atomic_ref<int>::required_alignment) int shared = 0;

    #else

    int shared = 0;

    #endif

    #else

    int shared = 0;

    #endif
    STD::atomic_ref<int> rr(shared);
    STD::atomic<int> seen(-1);
    STD::thread t(RefWaiter(&shared, &seen));
    rr.store(42);
    rr.notify_all();
    t.join();
    int s = seen.load();
    test_eq( s, 42 );
    test_pass("cxx20:atomic_ref<int>::wait + notify_all threaded");
}
#else
TEST_CASE_SKIP(atomic, atomic_ref_wait_notify)
#endif

#else
TEST_CASE_SKIP(atomic, memory_order_enumerators)
TEST_CASE_SKIP(atomic, memory_order_on_every_operation)
TEST_CASE_SKIP(atomic, kill_dependency)
TEST_CASE_SKIP(atomic, fences)
TEST_CASE_SKIP(atomic, lock_free_macros)
TEST_CASE_SKIP(atomic, flag_init_and_var_init)
TEST_CASE_SKIP(atomic, type_aliases_builtin)
TEST_CASE_SKIP(atomic, type_aliases_stdint)
TEST_CASE_SKIP(atomic, nonmember_load_store_exchange)
TEST_CASE_SKIP(atomic, nonmember_compare_exchange)
TEST_CASE_SKIP(atomic, nonmember_fetch_ops)
TEST_CASE_SKIP(atomic, nonmember_wait_notify)
TEST_CASE_SKIP(atomic, nonmember_flag_ops)
TEST_CASE_SKIP(atomic, atomic_ref_types_and_basics)
TEST_CASE_SKIP(atomic, atomic_ref_compare_exchange)
TEST_CASE_SKIP(atomic, atomic_ref_fetch_and_operators)
TEST_CASE_SKIP(atomic, atomic_ref_other_types)
TEST_CASE_SKIP(atomic, atomic_ref_wait_notify)
#endif
