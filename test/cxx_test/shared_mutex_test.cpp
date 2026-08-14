#include "test_cxx.hpp"
#if TEST_TARGET_CXX >= 2017
#include <shared_mutex>
#include <mutex>
#include <chrono>
#include <type_traits>

TEST_CASE(shared_mutex, exclusive) {
    STD::shared_mutex m;
    m.lock();
    m.unlock();
    bool got = m.try_lock();
    test_true( got );
    m.unlock();

    {
        STD::unique_lock<STD::shared_mutex> u(m);
        test_true( u.owns_lock() );
    }
    {
        STD::lock_guard<STD::shared_mutex> g(m);
        test_true( true );
    }
    test_pass("cxx17:exclusive");
}

TEST_CASE(shared_mutex, shared) {
    STD::shared_mutex m;
    m.lock_shared();
    bool got = m.try_lock_shared();
    test_true( got );
    m.unlock_shared();
    m.unlock_shared();
    test_true( true );
    test_pass("cxx17:shared");
}

TEST_CASE(shared_mutex, shared_lock_wrapper) {
    STD::shared_mutex m;

    {
        STD::shared_lock<STD::shared_mutex> s(m);
        test_true( s.owns_lock() );
        test_true( s.mutex() == &m );
    }

    STD::shared_lock<STD::shared_mutex> empty;
    test_true( !empty.owns_lock() );
    test_true( empty.mutex() == 0 );

    STD::shared_lock<STD::shared_mutex> d(m, STD::defer_lock);
    test_true( !d.owns_lock() );
    d.lock();
    test_true( d.owns_lock() );
    d.unlock();
    test_true( !d.owns_lock() );

    m.lock_shared();
    {
        STD::shared_lock<STD::shared_mutex> a(m, STD::adopt_lock);
        test_true( a.owns_lock() );
    }

    STD::shared_lock<STD::shared_mutex> t(m, STD::try_to_lock);
    test_true( t.owns_lock() );
    t.unlock();

    STD::shared_lock<STD::shared_mutex> tl(m, STD::defer_lock);
    test_true( tl.try_lock() );
    test_true( tl.owns_lock() );

    test_true( (bool)tl );
    tl.unlock();
    test_true( !tl );

    STD::shared_lock<STD::shared_mutex> rl(m);
    STD::shared_mutex* raw = rl.release();
    test_true( raw == &m );
    test_true( !rl.owns_lock() );
    m.unlock_shared();

    STD::shared_lock<STD::shared_mutex> b(m);
    STD::shared_lock<STD::shared_mutex> e2;
    b.swap(e2);
    test_true( !b.owns_lock() );
    test_true( e2.owns_lock() && e2.mutex() == &m );
    e2.unlock();
    test_pass("cxx14:shared lock wrapper");
}

TEST_CASE(shared_mutex, shared_lock_timed) {

    STD::shared_timed_mutex m;
    STD::shared_lock<STD::shared_timed_mutex> s(m, STD::defer_lock);
    test_true( s.try_lock_for(STD::chrono::milliseconds(10)) );
    test_true( s.owns_lock() );
    s.unlock();
    test_true( s.try_lock_until(STD::chrono::steady_clock::now() + STD::chrono::milliseconds(10)) );
    test_true( s.owns_lock() );
    s.unlock();
    test_pass("cxx14:shared lock timed");
}

TEST_CASE(shared_mutex, shared_timed_mutex) {
    STD::shared_timed_mutex m;
    m.lock();
    m.unlock();
    m.lock_shared();
    m.unlock_shared();
    bool ex = m.try_lock();
    test_true( ex );
    m.unlock();
    bool sh = m.try_lock_shared();
    test_true( sh );
    m.unlock_shared();
    test_pass("cxx14:shared timed mutex");
}

TEST_CASE(shared_mutex, shared_timed_mutex_timed) {
    STD::shared_timed_mutex m;

    bool ex = m.try_lock_for(STD::chrono::milliseconds(5));
    test_true( ex );
    m.unlock();

    STD::chrono::steady_clock::time_point soon = STD::chrono::steady_clock::now()
        + STD::chrono::milliseconds(5);
    bool exu = m.try_lock_until(soon);
    test_true( exu );
    m.unlock();

    bool sh = m.try_lock_shared_for(STD::chrono::milliseconds(5));
    test_true( sh );
    bool sh2 = m.try_lock_shared_for(STD::chrono::milliseconds(5));
    test_true( sh2 );
    m.unlock_shared();
    m.unlock_shared();

    bool shu = m.try_lock_shared_until(soon + STD::chrono::milliseconds(5));
    test_true( shu );
    m.unlock_shared();
    test_pass("cxx14:shared timed mutex timed");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(shared_mutex, shared_mutex_members) {
    STD::shared_mutex m;
    m.lock();          test_true( true );  test_pass("cxx17:shared_mutex::lock");
    m.unlock();        test_true( true );  test_pass("cxx17:shared_mutex::unlock");
    test_true( m.try_lock() );             test_pass("cxx17:shared_mutex::try_lock");
    m.unlock();
    m.lock_shared();   test_true( true );  test_pass("cxx17:shared_mutex::lock_shared");
    m.unlock_shared(); test_true( true );  test_pass("cxx17:shared_mutex::unlock_shared");
    test_true( m.try_lock_shared() );      test_pass("cxx17:shared_mutex::try_lock_shared");
    m.unlock_shared();
#if !defined(_MSC_VER) && _TST_HAS_MUTEX_NATIVE_HANDLE
    (void)m.native_handle();  test_true( true );  test_pass("cxx17:shared_mutex::native_handle");
    test_true( tst_type_is<STD::shared_mutex::native_handle_type>(m.native_handle()) );
    test_pass("cxx17:shared_mutex::native_handle_type");
#else
    test_skip("cxx17:shared_mutex::native_handle");
    test_skip("cxx17:shared_mutex::native_handle_type");
#endif
}
#else
TEST_CASE(shared_mutex, shared_mutex_members) {
    test_skip("cxx17:shared_mutex::lock"); test_skip("cxx17:shared_mutex::unlock");
    test_skip("cxx17:shared_mutex::try_lock"); test_skip("cxx17:shared_mutex::lock_shared");
    test_skip("cxx17:shared_mutex::unlock_shared"); test_skip("cxx17:shared_mutex::try_lock_shared");
    test_skip("cxx17:shared_mutex::native_handle");
    test_skip("cxx17:shared_mutex::native_handle_type");
}
#endif

TEST_CASE(shared_mutex, shared_timed_mutex_members) {
    STD::shared_timed_mutex m;
    STD::chrono::milliseconds d(2);
    STD::chrono::steady_clock::time_point tp = STD::chrono::steady_clock::now() + d;

    m.lock();   test_true( true );  test_pass("cxx14:shared_timed_mutex::lock");
    m.unlock(); test_true( true );  test_pass("cxx14:shared_timed_mutex::unlock");
    test_true( m.try_lock() );              test_pass("cxx14:shared_timed_mutex::try_lock");
    m.unlock();
    test_true( m.try_lock_for(d) );         test_pass("cxx14:shared_timed_mutex::try_lock_for");
    m.unlock();
    test_true( m.try_lock_until(tp) );      test_pass("cxx14:shared_timed_mutex::try_lock_until");
    m.unlock();

    m.lock_shared();   test_true( true );  test_pass("cxx14:shared_timed_mutex::lock_shared");
    m.unlock_shared(); test_true( true );  test_pass("cxx14:shared_timed_mutex::unlock_shared");
    test_true( m.try_lock_shared() );       test_pass("cxx14:shared_timed_mutex::try_lock_shared");
    m.unlock_shared();
    test_true( m.try_lock_shared_for(d) );  test_pass("cxx14:shared_timed_mutex::try_lock_shared_for");
    m.unlock_shared();
    STD::chrono::steady_clock::time_point tp2 = STD::chrono::steady_clock::now() + d;
    test_true( m.try_lock_shared_until(tp2) ); test_pass("cxx14:shared_timed_mutex::try_lock_shared_until");
    m.unlock_shared();
}

TEST_CASE(shared_mutex, shared_lock_members) {
    STD::shared_mutex sm;

    test_true( (STD::is_same<STD::shared_lock<STD::shared_mutex>::mutex_type,
                             STD::shared_mutex>::value) );
    test_pass("cxx14:shared_lock::mutex_type");

    STD::shared_lock<STD::shared_mutex> s0;
    test_true( !s0.owns_lock() && s0.mutex() == 0 );
    test_pass("cxx14:shared_lock default ctor");
    test_pass("cxx14:shared_lock::owns_lock");
    test_pass("cxx14:shared_lock::mutex");

    STD::shared_lock<STD::shared_mutex> s1(sm);
    test_true( s1.owns_lock() );
    test_pass("cxx14:shared_lock(mutex)");
    test_true( (bool)s1 );
    test_pass("cxx14:shared_lock::operator bool");

#if _TST_HAS_RVALUE_REF
    STD::shared_lock<STD::shared_mutex> s2(STD::move(s1));
    test_true( s2.owns_lock() && !s1.owns_lock() );
    test_pass("cxx14:shared_lock(shared_lock&&) move ctor");
    STD::shared_lock<STD::shared_mutex> s3;
    s3 = STD::move(s2);
    test_true( s3.owns_lock() && !s2.owns_lock() );
    test_pass("cxx14:shared_lock::operator=(shared_lock&&)");
#else
    STD::shared_lock<STD::shared_mutex> s3;
    s3.swap(s1);
    test_true( s3.owns_lock() && !s1.owns_lock() );
    test_skip("cxx14:shared_lock(shared_lock&&) move ctor");
    test_skip("cxx14:shared_lock::operator=(shared_lock&&)");
#endif
    s3.unlock();
    test_true( !s3.owns_lock() );
    test_pass("cxx14:shared_lock::unlock");

    STD::shared_lock<STD::shared_mutex> sd(sm, STD::defer_lock);
    test_true( !sd.owns_lock() );
    test_pass("cxx14:shared_lock(mutex, defer_lock)");
    sd.lock();
    test_true( sd.owns_lock() );
    test_pass("cxx14:shared_lock::lock");
    STD::shared_mutex* raw = sd.release();
    test_true( raw == &sm && !sd.owns_lock() );
    test_pass("cxx14:shared_lock::release");
    sm.unlock_shared();

    STD::shared_lock<STD::shared_mutex> st(sm, STD::try_to_lock);
    test_true( st.owns_lock() );
    test_pass("cxx14:shared_lock(mutex, try_to_lock)");
    st.unlock();
    STD::shared_lock<STD::shared_mutex> stl(sm, STD::defer_lock);
    test_true( stl.try_lock() );
    test_pass("cxx14:shared_lock::try_lock");
    stl.unlock();
    sm.lock_shared();
    STD::shared_lock<STD::shared_mutex> sa(sm, STD::adopt_lock);
    test_true( sa.owns_lock() );
    test_pass("cxx14:shared_lock(mutex, adopt_lock)");
    sa.unlock();

    STD::shared_lock<STD::shared_mutex> x(sm), y;
    x.swap(y);
    test_true( y.owns_lock() && !x.owns_lock() );
    test_pass("cxx14:shared_lock::swap");
    STD::swap(x, y);
    test_true( x.owns_lock() && !y.owns_lock() );
    test_pass("cxx14:swap(shared_lock, shared_lock)");
    x.unlock();

    STD::shared_timed_mutex stm;
    STD::chrono::milliseconds d(2);
    STD::shared_lock<STD::shared_timed_mutex> td(stm, d);
    test_true( td.owns_lock() );
    td.unlock();
    test_pass("cxx14:shared_lock(mutex, duration)");
    STD::shared_lock<STD::shared_timed_mutex> tp(stm,
        STD::chrono::steady_clock::now() + d);
    test_true( tp.owns_lock() );
    tp.unlock();
    test_pass("cxx14:shared_lock(mutex, time_point)");
    STD::shared_lock<STD::shared_timed_mutex> tf(stm, STD::defer_lock);
    test_true( tf.try_lock_for(d) );
    tf.unlock();
    test_pass("cxx14:shared_lock::try_lock_for");
    test_true( tf.try_lock_until(STD::chrono::steady_clock::now() + d) );
    tf.unlock();
    test_pass("cxx14:shared_lock::try_lock_until");
}

TEST_CASE(shared_mutex, feature_test_macros) {
#if defined(__cpp_lib_shared_timed_mutex) && (__cpp_lib_shared_timed_mutex) >= 201402L
    test_true( (__cpp_lib_shared_timed_mutex) >= 201402L );
    test_pass("cxx14:__cpp_lib_shared_timed_mutex=201402L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx14:__cpp_lib_shared_timed_mutex=201402L");
#endif
#if defined(__cpp_lib_shared_mutex) && (__cpp_lib_shared_mutex) >= 201505L
    test_true( (__cpp_lib_shared_mutex) >= 201505L );
    test_pass("cxx17:__cpp_lib_shared_mutex=201505L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_shared_mutex=201505L");
#endif
}
#else
TEST_CASE(shared_mutex, exclusive) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx17:exclusive");
}
TEST_CASE(shared_mutex, shared) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx17:shared");
}
TEST_CASE(shared_mutex, shared_lock_wrapper) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx14:shared lock wrapper");
}
TEST_CASE(shared_mutex, shared_lock_timed) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx14:shared lock timed");
}
TEST_CASE(shared_mutex, shared_timed_mutex) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx14:shared timed mutex");
}
TEST_CASE(shared_mutex, shared_timed_mutex_timed) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx14:shared timed mutex timed");
}
TEST_CASE(shared_mutex, shared_mutex_members) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx17:shared_mutex::lock");
    test_skip("cxx17:shared_mutex::unlock");
    test_skip("cxx17:shared_mutex::try_lock");
    test_skip("cxx17:shared_mutex::lock_shared");
    test_skip("cxx17:shared_mutex::unlock_shared");
    test_skip("cxx17:shared_mutex::try_lock_shared");
    test_skip("cxx17:shared_mutex::native_handle");
    test_skip("cxx17:shared_mutex::native_handle_type");
}
TEST_CASE(shared_mutex, shared_timed_mutex_members) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx14:shared_timed_mutex::lock");
    test_skip("cxx14:shared_timed_mutex::unlock");
    test_skip("cxx14:shared_timed_mutex::try_lock");
    test_skip("cxx14:shared_timed_mutex::try_lock_for");
    test_skip("cxx14:shared_timed_mutex::try_lock_until");
    test_skip("cxx14:shared_timed_mutex::lock_shared");
    test_skip("cxx14:shared_timed_mutex::unlock_shared");
    test_skip("cxx14:shared_timed_mutex::try_lock_shared");
    test_skip("cxx14:shared_timed_mutex::try_lock_shared_for");
    test_skip("cxx14:shared_timed_mutex::try_lock_shared_until");
}
TEST_CASE(shared_mutex, shared_lock_members) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx14:shared_lock::mutex_type");
    test_skip("cxx14:shared_lock default ctor");
    test_skip("cxx14:shared_lock::owns_lock");
    test_skip("cxx14:shared_lock::mutex");
    test_skip("cxx14:shared_lock(mutex)");
    test_skip("cxx14:shared_lock::operator bool");
    test_skip("cxx14:shared_lock(shared_lock&&) move ctor");
    test_skip("cxx14:shared_lock::operator=(shared_lock&&)");
    test_skip("cxx14:shared_lock::unlock");
    test_skip("cxx14:shared_lock(mutex, defer_lock)");
    test_skip("cxx14:shared_lock::lock");
    test_skip("cxx14:shared_lock::release");
    test_skip("cxx14:shared_lock(mutex, try_to_lock)");
    test_skip("cxx14:shared_lock::try_lock");
    test_skip("cxx14:shared_lock(mutex, adopt_lock)");
    test_skip("cxx14:shared_lock::swap");
    test_skip("cxx14:swap(shared_lock, shared_lock)");
    test_skip("cxx14:shared_lock(mutex, duration)");
    test_skip("cxx14:shared_lock(mutex, time_point)");
    test_skip("cxx14:shared_lock::try_lock_for");
    test_skip("cxx14:shared_lock::try_lock_until");
}
TEST_CASE(shared_mutex, feature_test_macros) {
    TEST_NOTE("<shared_mutex> is C++17");
    test_skip("cxx14:__cpp_lib_shared_timed_mutex=201402L");
    test_skip("cxx17:__cpp_lib_shared_mutex=201505L");
}
#endif
