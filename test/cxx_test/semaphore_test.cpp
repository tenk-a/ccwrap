#include "test_cxx.hpp"
#if TEST_TARGET_CXX >= 2020
#include <semaphore>
#include <chrono>
#include <thread>
#include <type_traits>

TEST_CASE(semaphore, counting) {
    STD::counting_semaphore<4> s(2);
    test_true( s.try_acquire() );
    test_pass("cxx20:counting_semaphore::counting_semaphore(ptrdiff_t) ctor");
    test_true( s.try_acquire() );
    test_true( !s.try_acquire() );
    test_pass("cxx20:counting_semaphore::try_acquire");

    s.release();
    test_true( s.try_acquire() );
    test_true( !s.try_acquire() );
    test_pass("cxx20:counting_semaphore::release");

    s.release(2);
    s.acquire();
    s.acquire();
    test_true( !s.try_acquire() );
    test_pass("cxx20:counting_semaphore::acquire");

    test_eq( (long)STD::counting_semaphore<4>::max(), 4L );
    test_pass("cxx20:counting_semaphore::max");
}

TEST_CASE(semaphore, binary) {
    STD::binary_semaphore s(1);
    test_true( s.try_acquire() );
    test_true( !s.try_acquire() );
    s.release();
    test_true( s.try_acquire() );
    test_pass("cxx20:binary_semaphore::try_acquire");

    test_eq( (long)STD::binary_semaphore::max(), 1L );
    test_pass("cxx20:binary_semaphore::max");

    STD::binary_semaphore empty(0);
    test_true( !empty.try_acquire() );
    empty.release();
    s.release();
    test_true( empty.try_acquire() );
    test_pass("cxx20:binary_semaphore::release");
}

TEST_CASE(semaphore, timed_acquire) {

    STD::counting_semaphore<4> s(1);
    test_true( s.try_acquire_for(STD::chrono::milliseconds(1)) );

    test_true( !s.try_acquire_for(STD::chrono::milliseconds(1)) );
    test_pass("cxx20:counting_semaphore::try_acquire_for");

    s.release();
    STD::chrono::steady_clock::time_point soon = STD::chrono::steady_clock::now()
        + STD::chrono::milliseconds(1);
    test_true( s.try_acquire_until(soon) );
    STD::chrono::steady_clock::time_point past = STD::chrono::steady_clock::now();
    test_true( !s.try_acquire_until(past) );
    test_pass("cxx20:counting_semaphore::try_acquire_until");
}

namespace {

struct Acquirer {
    STD::binary_semaphore* sem;
    int*                   got;
    Acquirer(STD::binary_semaphore* s, int* g) : sem(s), got(g) {}
    void operator()() const { sem->acquire(); *got = 1; }
};
}

TEST_CASE(semaphore, blocking_acquire_across_threads) {

    STD::binary_semaphore sem(0);
    int got = 0;
    STD::thread t((Acquirer(&sem, &got)));

    test_eq( got, 0 );
    sem.release();
    t.join();
    test_eq( got, 1 );

    test_true( !sem.try_acquire() );
    test_pass("cxx20:binary_semaphore::acquire(blocking)");
}

TEST_CASE(semaphore, max_and_types) {

    test_true( STD::counting_semaphore<4>::max() >= 4 );
    test_true( STD::counting_semaphore<1>::max() >= 1 );
    test_true( STD::binary_semaphore::max() >= 1 );
    test_pass("cxx20:counting_semaphore::max(least)");

    test_true( STD::counting_semaphore<>::max() > 1000 );
    test_pass("cxx20:counting_semaphore");

    test_true( (STD::is_same<STD::binary_semaphore, STD::counting_semaphore<1> >::value) );
    test_pass("cxx20:binary_semaphore");

    STD::counting_semaphore<2> a(0);
    test_true( !a.try_acquire() );
    a.release();
    test_true( a.try_acquire() );
    test_pass("cxx20:counting_semaphore(noncopyable)");

    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::counting_semaphore<2> >::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible move assignment as assignable (vc bugs A26)");
    test_true( !STD::is_move_assignable<STD::counting_semaphore<2> >::value );
    test_pass("cxx20:counting_semaphore::operator=");
}

TEST_CASE(semaphore, timed_acquire_gives_up) {

    STD::binary_semaphore empty(0);
    STD::chrono::steady_clock::time_point t0 = STD::chrono::steady_clock::now();
    bool ok = empty.try_acquire_for(STD::chrono::milliseconds(30));
    STD::chrono::steady_clock::time_point t1 = STD::chrono::steady_clock::now();
    test_true( !ok );
    long long ms = (long long)STD::chrono::duration_cast<STD::chrono::milliseconds>(t1 - t0).count();
    test_true( ms >= 10 );
    test_pass("cxx20:binary_semaphore::try_acquire_for");

    STD::binary_semaphore ready(1);
    test_true( ready.try_acquire_for(STD::chrono::milliseconds(30)) );
    test_pass("cxx20:binary_semaphore::try_acquire_for(ready)");

    STD::binary_semaphore empty2(0);
    test_true( !empty2.try_acquire_until(STD::chrono::steady_clock::now()
                                         + STD::chrono::milliseconds(20)) );
    STD::binary_semaphore ready2(1);
    test_true( ready2.try_acquire_until(STD::chrono::steady_clock::now()
                                        + STD::chrono::milliseconds(20)) );
    test_pass("cxx20:binary_semaphore::try_acquire_until");
}

TEST_CASE(semaphore, feature_test_macros) {
#if defined(__cpp_lib_semaphore) && (__cpp_lib_semaphore) >= 201907L
    test_true( (__cpp_lib_semaphore) >= 201907L );
    test_pass("cxx20:__cpp_lib_semaphore=201907L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_semaphore=201907L");
#endif
}
#else
TEST_CASE(semaphore, counting) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:counting_semaphore::counting_semaphore(ptrdiff_t) ctor");
    test_skip("cxx20:counting_semaphore::try_acquire");
    test_skip("cxx20:counting_semaphore::release");
    test_skip("cxx20:counting_semaphore::acquire");
    test_skip("cxx20:counting_semaphore::max");
}
TEST_CASE(semaphore, binary) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:binary_semaphore::try_acquire");
    test_skip("cxx20:binary_semaphore::max");
    test_skip("cxx20:binary_semaphore::release");
}
TEST_CASE(semaphore, timed_acquire) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:counting_semaphore::try_acquire_for");
    test_skip("cxx20:counting_semaphore::try_acquire_until");
}
TEST_CASE(semaphore, blocking_acquire_across_threads) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:binary_semaphore::acquire(blocking)");
}
TEST_CASE(semaphore, max_and_types) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:counting_semaphore::max(least)");
    test_skip("cxx20:counting_semaphore");
    test_skip("cxx20:binary_semaphore");
    test_skip("cxx20:counting_semaphore(noncopyable)");
    test_skip("cxx20:counting_semaphore::operator=");
}
TEST_CASE(semaphore, timed_acquire_gives_up) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:binary_semaphore::try_acquire_for");
    test_skip("cxx20:binary_semaphore::try_acquire_for(ready)");
    test_skip("cxx20:binary_semaphore::try_acquire_until");
}
TEST_CASE(semaphore, feature_test_macros) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:__cpp_lib_semaphore=201907L");
}
#endif
