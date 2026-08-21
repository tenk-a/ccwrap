#include "test_cxx.hpp"
#include <mutex>
#include <chrono>
#include <type_traits>

namespace {
    struct OnceSet  { int* p; void operator()() const { ++*p; } };
    struct OnceBump { int* p; void operator()() const { ++*p; } };
}

TEST_CASE(mutex, plain_mutex) {
    STD::mutex m;
    m.lock();
    m.unlock();
    test_true( true );
    test_pass("cxx11:mutex");

    bool got = m.try_lock();
    test_true( got );
    test_pass("cxx11:mutex::try_lock");
    m.unlock();
}

TEST_CASE(mutex, recursive_mutex) {
    STD::recursive_mutex m;
    m.lock();
    m.lock();
    bool got = m.try_lock();
    test_true( got );
    test_pass("cxx11:recursive_mutex::try_lock");
    m.unlock();
    m.unlock();
    m.unlock();
    test_true( true );
    test_pass("cxx11:recursive_mutex");
}

TEST_CASE(mutex, timed_mutex) {
    STD::timed_mutex m;
    m.lock();
    m.unlock();
    bool got = m.try_lock();
    test_true( got );
    test_pass("cxx11:timed_mutex");
    m.unlock();

    bool f = m.try_lock_for(STD::chrono::milliseconds(5));
    test_true( f );
    test_pass("cxx11:timed_mutex::try_lock_for");
    m.unlock();

    STD::chrono::steady_clock::time_point soon = STD::chrono::steady_clock::now()
        + STD::chrono::milliseconds(5);
    bool u = m.try_lock_until(soon);
    test_true( u );
    test_pass("cxx11:timed_mutex::try_lock_until");
    m.unlock();

    {
        STD::unique_lock<STD::timed_mutex> lk(m);
        test_true( lk.owns_lock() );
        test_pass("cxx11:unique_lock(timed_mutex)");
    }
}

TEST_CASE(mutex, recursive_timed_mutex) {
    STD::recursive_timed_mutex m;
    m.lock();
    m.lock();
    bool got = m.try_lock_for(STD::chrono::milliseconds(5));
    test_true( got );
    test_pass("cxx11:recursive_timed_mutex::try_lock_for");
    m.unlock();
    m.unlock();
    m.unlock();
    test_true( true );
    test_pass("cxx11:recursive_timed_mutex");
}

TEST_CASE(mutex, lock_guard) {
    STD::mutex m;
    {
        STD::lock_guard<STD::mutex> g(m);
        test_true( true );
    }
    bool got = m.try_lock();
    test_true( got );
    test_pass("cxx11:lock_guard");
    m.unlock();

    m.lock();
    {
        STD::lock_guard<STD::mutex> g(m, STD::adopt_lock);
        test_true( true );
    }
    bool got2 = m.try_lock();
    test_true( got2 );
    test_pass("cxx11:adopt_lock");
    m.unlock();
}

TEST_CASE(mutex, unique_lock) {
    STD::mutex m;

    STD::unique_lock<STD::mutex> u(m);
    test_true( u.owns_lock() );
    test_pass("cxx11:unique_lock::owns_lock");
    test_true( u.mutex() == &m );
    test_pass("cxx11:unique_lock::mutex");
    u.unlock();
    test_true( !u.owns_lock() );
    test_pass("cxx11:unique_lock::unlock");
    u.lock();
    test_true( u.owns_lock() );
    test_pass("cxx11:unique_lock::lock");
    u.unlock();

    STD::unique_lock<STD::mutex> empty;
    test_true( !empty.owns_lock() );
    test_true( empty.mutex() == 0 );
    test_pass("cxx11:unique_lock");

    STD::unique_lock<STD::mutex> d(m, STD::defer_lock);
    test_true( !d.owns_lock() );
    d.lock();
    test_true( d.owns_lock() );
    d.unlock();
    test_pass("cxx11:defer_lock");

    m.lock();
    STD::unique_lock<STD::mutex> a(m, STD::adopt_lock);
    test_true( a.owns_lock() );
    a.unlock();
    test_pass("cxx11:unique_lock(adopt_lock)");

    STD::unique_lock<STD::mutex> t(m, STD::try_to_lock);
    test_true( t.owns_lock() );
    t.unlock();
    test_pass("cxx11:try_to_lock");

    STD::unique_lock<STD::mutex> tl(m, STD::defer_lock);
    bool ok = tl.try_lock();
    test_true( ok );
    test_true( tl.owns_lock() );
    test_pass("cxx11:unique_lock::try_lock");

    STD::mutex* raw = tl.release();
    test_true( raw == &m );
    test_true( !tl.owns_lock() );
    test_pass("cxx11:unique_lock::release");
    m.unlock();

    STD::unique_lock<STD::mutex> b(m);
    test_true( (bool)b );
    b.unlock();
    test_true( !b );
    test_pass("cxx11:unique_lock::operator_bool");
    b.lock();

    STD::unique_lock<STD::mutex> e2;
    b.swap(e2);
    test_true( !b.owns_lock() );
    test_true( e2.owns_lock() );
    test_true( e2.mutex() == &m );
    test_pass("cxx11:unique_lock::swap");

#if !defined(__WATCOMC__)
    STD::swap(b, e2);
    test_true( b.owns_lock() );
    test_true( !e2.owns_lock() );
    e2.swap(b);
    test_pass("cxx11:swap(unique_lock)");
#endif
    if (e2.owns_lock()) e2.unlock();
}

TEST_CASE(mutex, unique_lock_timed) {

    STD::timed_mutex tm;
    STD::unique_lock<STD::timed_mutex> u(tm, STD::defer_lock);
    bool got = u.try_lock_for(STD::chrono::milliseconds(10));
    test_true( got );
    test_true( u.owns_lock() );
    test_pass("cxx11:unique_lock::try_lock_for");
    u.unlock();

    bool got2 = u.try_lock_until(STD::chrono::steady_clock::now() + STD::chrono::milliseconds(10));
    test_true( got2 );
    test_true( u.owns_lock() );
    test_pass("cxx11:unique_lock::try_lock_until");
    u.unlock();
}

TEST_CASE(mutex, scoped_lock) {

    STD::mutex a;
#if TEST_TARGET_CXX >= 2017
    {
        STD::scoped_lock<STD::mutex> g(a);
        test_true( true );
    }
#else
    TEST_SKIP_N(1);
#endif
    bool ga = a.try_lock();
    test_true( ga );
    test_pass("cxx17:scoped_lock");
    a.unlock();
}

namespace {
int g_once_calls = 0;
void bump_once() { ++g_once_calls; }
}

TEST_CASE(mutex, once_flag) {
    STD::once_flag f;
    STD::call_once(f, bump_once);
    STD::call_once(f, bump_once);
    STD::call_once(f, bump_once);
    test_eq( g_once_calls, 1 );
    test_pass("cxx11:call_once");

    STD::once_flag f2;
    int local = 0;
    OnceSet s; s.p = &local;
    STD::call_once(f2, s);
    STD::call_once(f2, s);
    test_eq( local, 1 );
    test_pass("cxx11:once_flag");
}

TEST_CASE(mutex, destructors_and_noncopyable) {
    for (int i = 0; i < 64; ++i) {
        STD::mutex tmp;
        tmp.lock();
        tmp.unlock();
    }
    {
        STD::mutex after;
        test_true( after.try_lock() );
        after.unlock();
    }
    test_true( STD::is_destructible<STD::mutex>::value );
    test_pass("cxx11:mutex::~mutex");

    for (int i = 0; i < 64; ++i) {
        STD::recursive_mutex tmp;
        tmp.lock();
        tmp.lock();
        tmp.unlock();
        tmp.unlock();
    }
    {
        STD::recursive_mutex after;
        test_true( after.try_lock() );
        after.unlock();
    }
    test_true( STD::is_destructible<STD::recursive_mutex>::value );
    test_pass("cxx11:recursive_mutex::~recursive_mutex");

    {
        STD::timed_mutex tmp;
        tmp.lock();
        tmp.unlock();
    }
    test_true( STD::is_destructible<STD::timed_mutex>::value );
    test_pass("cxx11:timed_mutex::~timed_mutex");

    {
        STD::recursive_timed_mutex tmp;
        tmp.lock();
        tmp.unlock();
    }
    test_true( STD::is_destructible<STD::recursive_timed_mutex>::value );
    test_pass("cxx11:recursive_timed_mutex::~recursive_timed_mutex");

#if !defined(__WATCOMC__)
    TEST_SKIP_VC120("MSVC <= 12 misreports a deleted/inaccessible copy constructor (vc bugs A26/A27)");
    test_true( !STD::is_copy_constructible<STD::mutex>::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::mutex>::value );
    test_pass("cxx11:mutex::mutex(const mutex&) = delete");

    TEST_SKIP_VC120("MSVC <= 12 misreports a deleted/inaccessible copy constructor (vc bugs A26/A27)");
    test_true( !STD::is_copy_constructible<STD::recursive_mutex>::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::recursive_mutex>::value );
    test_pass("cxx11:recursive_mutex::recursive_mutex(const recursive_mutex&) = delete");

    TEST_SKIP_VC120("MSVC <= 12 misreports a deleted/inaccessible copy constructor (vc bugs A26/A27)");
    test_true( !STD::is_copy_constructible<STD::once_flag>::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::once_flag>::value );
    test_pass("cxx11:once_flag::once_flag(const once_flag&) = delete");

#if TEST_TARGET_CXX >= 2017
    TEST_SKIP_VC110("VS2012 or older cannot see that a copy constructor is inaccessible (vc bugs A26/A27)");
    test_true(( !STD::is_copy_constructible<STD::scoped_lock<STD::mutex> >::value ));
    test_pass("cxx17:scoped_lock::scoped_lock(const scoped_lock&) = delete");
#else
    TEST_NOTE("scoped_lock is C++17");
    test_skip("cxx17:scoped_lock::scoped_lock(const scoped_lock&) = delete");
#endif
#else
    TEST_NOTE("C++03 has no deleted functions, so the trait cannot see the intent");
    test_skip("cxx11:mutex::mutex(const mutex&) = delete");
    test_skip("cxx11:recursive_mutex::recursive_mutex(const recursive_mutex&) = delete");
    test_skip("cxx11:once_flag::once_flag(const once_flag&) = delete");
    test_skip("cxx17:scoped_lock::scoped_lock(const scoped_lock&) = delete");
#endif
}

TEST_CASE(mutex, guard_destructors) {
    STD::mutex m;
    {
        STD::lock_guard<STD::mutex> g(m);
        test_true( true );
    }
    test_true( m.try_lock() );
    m.unlock();
    test_true( STD::is_destructible<STD::lock_guard<STD::mutex> >::value );
    test_pass("cxx11:lock_guard::~lock_guard");

    {
        STD::unique_lock<STD::mutex> lk(m);
        test_true( lk.owns_lock() );
    }
    test_true( m.try_lock() );
    m.unlock();
    {
        STD::unique_lock<STD::mutex> lk(m, STD::defer_lock);
        test_true( !lk.owns_lock() );
    }
    test_true( m.try_lock() );
    m.unlock();
    {
        STD::unique_lock<STD::mutex> lk(m);
        lk.unlock();
        test_true( !lk.owns_lock() );
    }
    test_true( m.try_lock() );
    m.unlock();
    TEST_NOTE("the not-owning case cannot show a STILL-locked mutex single-threaded");
    test_pass("cxx11:unique_lock::~unique_lock");

    STD::mutex a, b;
#if TEST_TARGET_CXX >= 2017
    {
        STD::scoped_lock<STD::mutex, STD::mutex> g(a, b);
        test_true( true );
    }
#else
    TEST_SKIP_N(1);
#endif
    test_true( a.try_lock() );
    test_true( b.try_lock() );
    a.unlock();
    b.unlock();
    test_pass("cxx17:scoped_lock::~scoped_lock");
}

TEST_CASE(mutex, timed_mutex_try_lock) {
    STD::timed_mutex m;
    bool first = m.try_lock();
    test_true( first );
    m.unlock();
    bool again = m.try_lock();
    test_true( again );
    m.unlock();
    test_pass("cxx11:timed_mutex::try_lock");
}

TEST_CASE(mutex, once_flag_default_ctor) {
    STD::once_flag f;
    int calls = 0;
    OnceBump b; b.p = &calls;
    STD::call_once(f, b);
    test_eq( calls, 1 );

    STD::once_flag g;
    STD::call_once(g, b);
    test_eq( calls, 2 );
    STD::call_once(f, b);
    test_eq( calls, 2 );
    test_pass("cxx11:once_flag::once_flag()");
}

TEST_CASE(mutex, mutex_type_members) {
    STD::mutex m;
    m.lock();    test_true( true );  test_pass("cxx11:mutex::lock");
    m.unlock();  test_true( true );  test_pass("cxx11:mutex::unlock");
#if !defined(_MSC_VER)
    (void)m.native_handle();  test_true( true );  test_pass("cxx11:mutex::native_handle");
#else
    test_skip("cxx11:mutex::native_handle");
#endif

    STD::recursive_mutex rm;
    rm.lock(); rm.lock();  test_true( true );  test_pass("cxx11:recursive_mutex::lock");
    rm.unlock(); rm.unlock();  test_true( true );  test_pass("cxx11:recursive_mutex::unlock");
#if !defined(_MSC_VER)
    (void)rm.native_handle();  test_true( true );  test_pass("cxx11:recursive_mutex::native_handle");
#else
    test_skip("cxx11:recursive_mutex::native_handle");
#endif

    STD::timed_mutex tm;
    tm.lock();    test_true( true );  test_pass("cxx11:timed_mutex::lock");
    tm.unlock();  test_true( true );  test_pass("cxx11:timed_mutex::unlock");
#if !defined(_MSC_VER) && _TST_HAS_MUTEX_NATIVE_HANDLE
    (void)tm.native_handle();  test_true( true );  test_pass("cxx11:timed_mutex::native_handle");
#else
    test_skip("cxx11:timed_mutex::native_handle");
#endif

    STD::recursive_timed_mutex rtm;
    rtm.lock();  test_true( true );  test_pass("cxx11:recursive_timed_mutex::lock");
    test_true( rtm.try_lock() );
    test_pass("cxx11:recursive_timed_mutex::try_lock");
    test_true( rtm.try_lock_until(STD::chrono::steady_clock::now()
                                  + STD::chrono::milliseconds(1)) );
    test_pass("cxx11:recursive_timed_mutex::try_lock_until");
    rtm.unlock(); rtm.unlock(); rtm.unlock();  test_true( true );  test_pass("cxx11:recursive_timed_mutex::unlock");
#if !defined(_MSC_VER) && _TST_HAS_MUTEX_NATIVE_HANDLE
    (void)rtm.native_handle();  test_true( true );  test_pass("cxx11:recursive_timed_mutex::native_handle");
#else
    test_skip("cxx11:recursive_timed_mutex::native_handle");
#endif
}

TEST_CASE(mutex, unique_lock_ctors) {
    STD::mutex m;
    STD::unique_lock<STD::mutex> u0;
    test_true( !u0.owns_lock() && u0.mutex() == 0 );
    test_pass("cxx11:unique_lock default ctor");

    STD::unique_lock<STD::mutex> u1(m);
    test_true( u1.owns_lock() );
    test_pass("cxx11:unique_lock(mutex)");

#if _TST_HAS_RVALUE_REF
    STD::unique_lock<STD::mutex> u2(STD::move(u1));
    test_true( u2.owns_lock() && !u1.owns_lock() );
    test_pass("cxx11:unique_lock(unique_lock&&) move ctor");
    STD::unique_lock<STD::mutex> u3;
    u3 = STD::move(u2);
    test_true( u3.owns_lock() && !u2.owns_lock() );
    test_pass("cxx11:unique_lock::operator=(unique_lock&&)");
    u3.unlock();
#else
    STD::unique_lock<STD::mutex> u3;
    u3.swap(u1);
    test_true( u3.owns_lock() && !u1.owns_lock() );
    test_skip("cxx11:unique_lock(unique_lock&&) move ctor");
    test_skip("cxx11:unique_lock::operator=(unique_lock&&)");
    u3.unlock();
#endif

    STD::unique_lock<STD::mutex> ud(m, STD::defer_lock);
    test_true( !ud.owns_lock() );
    ud.lock();
    test_true( ud.owns_lock() );
    ud.unlock();
    test_pass("cxx11:unique_lock(defer_lock)");

    STD::unique_lock<STD::mutex> ut(m, STD::try_to_lock);
    test_true( ut.owns_lock() );
    ut.unlock();
    test_pass("cxx11:unique_lock(try_to_lock)");

    STD::timed_mutex tm;
    STD::unique_lock<STD::timed_mutex> utd(tm, STD::chrono::milliseconds(2));
    test_true( utd.owns_lock() );
    utd.unlock();
    test_pass("cxx11:unique_lock(mutex, duration)");
    STD::unique_lock<STD::timed_mutex> utp(tm, STD::chrono::steady_clock::now()
                                           + STD::chrono::milliseconds(2));
    test_true( utp.owns_lock() );
    test_pass("cxx11:unique_lock(mutex, time_point)");
}

TEST_CASE(mutex, lock_guard_adopt_and_free_lock) {
    STD::mutex m;
    m.lock();
    {
        STD::lock_guard<STD::mutex> g(m, STD::adopt_lock);
        test_true( true );
    }
    test_pass("cxx11:lock_guard(adopt_lock)");

    STD::mutex a, b;
    STD::lock(a, b);
    test_true( true );
    a.unlock(); b.unlock();
    test_pass("cxx11:lock (variadic free function)");
    STD::mutex c, d;
    int r = STD::try_lock(c, d);
    test_eq( r, -1 );
    c.unlock(); d.unlock();
    test_pass("cxx11:try_lock (variadic free function)");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(mutex, scoped_lock_variants) {
    { STD::scoped_lock<> lk; test_true( true ); }
    test_pass("cxx17:scoped_lock (empty)");

    STD::mutex a, b;
    a.lock(); b.lock();
    { STD::scoped_lock<STD::mutex, STD::mutex> lk(STD::adopt_lock, a, b); test_true( true ); }
    test_pass("cxx17:scoped_lock(adopt_lock)");
}
#else
TEST_CASE(mutex, scoped_lock_variants) {
    test_skip("cxx17:scoped_lock (empty)");
    test_skip("cxx17:scoped_lock(adopt_lock)");
}
#endif


#if TEST_TARGET_CXX >= 2011
TEST_CASE(mutex, lock_tag_types_cxx11) {
    STD::mutex m;

    STD::unique_lock<STD::mutex> deferred(m, STD::defer_lock);
    test_true( !deferred.owns_lock() );
    const STD::defer_lock_t* defer_p = &STD::defer_lock;
    test_true( defer_p != 0 );
    test_pass("cxx11:defer_lock_t");

    STD::unique_lock<STD::mutex> attempted(m, STD::try_to_lock);
    test_true( attempted.owns_lock() );
    const STD::try_to_lock_t* try_p = &STD::try_to_lock;
    test_true( try_p != 0 );
    test_pass("cxx11:try_to_lock_t");
    attempted.unlock();

    m.lock();
    {
        STD::lock_guard<STD::mutex> adopted(m, STD::adopt_lock);
        const STD::adopt_lock_t* adopt_p = &STD::adopt_lock;
        test_true( adopt_p != 0 );
    }
    test_true( m.try_lock() );
    m.unlock();
    test_pass("cxx11:adopt_lock_t");
}

TEST_CASE(mutex, mutex_type_aliases_cxx11) {
    test_true(( STD::is_same<STD::lock_guard<STD::mutex>::mutex_type, STD::mutex>::value ));
    test_true(( STD::is_same<STD::lock_guard<STD::recursive_mutex>::mutex_type,
                             STD::recursive_mutex>::value ));
    test_pass("cxx11:lock_guard::mutex_type");

    test_true(( STD::is_same<STD::unique_lock<STD::mutex>::mutex_type, STD::mutex>::value ));
    test_true(( STD::is_same<STD::unique_lock<STD::timed_mutex>::mutex_type,
                             STD::timed_mutex>::value ));
    test_pass("cxx11:unique_lock::mutex_type");

#if defined(__cpp_lib_scoped_lock)
    test_true(( STD::is_same<STD::scoped_lock<STD::mutex>::mutex_type, STD::mutex>::value ));
    test_pass("cxx17:scoped_lock::mutex_type");
#else
    TEST_NOTE("scoped_lock's mutex_type exists only for the single-mutex specialisation");
    test_skip("cxx17:scoped_lock::mutex_type");
#endif
}

TEST_CASE(mutex, native_handle_types_cxx11) {
#if !defined(_MSC_VER) && !defined(__WATCOMC__) && _TST_HAS_DECLTYPE
    STD::mutex m;
    test_true(( STD::is_same<STD::mutex::native_handle_type,
                             decltype(m.native_handle())>::value ));
    test_pass("cxx11:mutex::native_handle_type");

    STD::recursive_mutex rm;
    test_true(( STD::is_same<STD::recursive_mutex::native_handle_type,
                             decltype(rm.native_handle())>::value ));
    test_pass("cxx11:recursive_mutex::native_handle_type");

# if _TST_HAS_MUTEX_NATIVE_HANDLE
    STD::timed_mutex tm;
    test_true(( STD::is_same<STD::timed_mutex::native_handle_type,
                             decltype(tm.native_handle())>::value ));
    test_pass("cxx11:timed_mutex::native_handle_type");

    STD::recursive_timed_mutex rtm;
    test_true(( STD::is_same<STD::recursive_timed_mutex::native_handle_type,
                             decltype(rtm.native_handle())>::value ));
    test_pass("cxx11:recursive_timed_mutex::native_handle_type");
# else
    TEST_NOTE("this library gives the timed mutexes no native_handle");
    test_skip("cxx11:timed_mutex::native_handle_type");
    test_skip("cxx11:recursive_timed_mutex::native_handle_type");
# endif
#else
    TEST_NOTE("native_handle is optional and this implementation does not provide it");
    test_skip("cxx11:mutex::native_handle_type");
    test_skip("cxx11:recursive_mutex::native_handle_type");
    test_skip("cxx11:timed_mutex::native_handle_type");
    test_skip("cxx11:recursive_timed_mutex::native_handle_type");
#endif
}
#else
TEST_CASE(mutex, lock_tag_types_cxx11) {
    test_skip("cxx11:defer_lock_t");
    test_skip("cxx11:try_to_lock_t");
    test_skip("cxx11:adopt_lock_t");
}
TEST_CASE(mutex, mutex_type_aliases_cxx11) {
    test_skip("cxx11:lock_guard::mutex_type");
    test_skip("cxx11:unique_lock::mutex_type");
    test_skip("cxx17:scoped_lock::mutex_type");
}
TEST_CASE(mutex, native_handle_types_cxx11) {
    test_skip("cxx11:mutex::native_handle_type");
    test_skip("cxx11:recursive_mutex::native_handle_type");
    test_skip("cxx11:timed_mutex::native_handle_type");
    test_skip("cxx11:recursive_timed_mutex::native_handle_type");
}
#endif

TEST_CASE(mutex, feature_test_macros) {
#if defined(__cpp_lib_scoped_lock)
    test_true( __cpp_lib_scoped_lock > 0L );
    test_pass("cxx17:__cpp_lib_scoped_lock");
#else
    test_skip("cxx17:__cpp_lib_scoped_lock");
#endif
}
