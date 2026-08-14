#include "test_cxx.hpp"
#include <condition_variable>

TEST_CASE(condition_variable, header_compiles) { test_true( true ); test_pass("cxx11:condition_variable(header)"); }

#include <mutex>
#include <chrono>
#include <type_traits>
namespace {
struct cv_true  { bool operator()() const { return true; } };
struct cv_false { bool operator()() const { return false; } };
}
#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
typedef STD::cv_status::cv_status _tst_cv_status;
#else
typedef STD::cv_status _tst_cv_status;
#endif

#if defined(__WATCOMC__)
#  define _TST_CV_NO_TIMEOUT  STD::no_timeout
#else
#  define _TST_CV_NO_TIMEOUT  STD::cv_status::no_timeout
#endif

namespace {
template <class Cv, class Lock, class Clock, class Dur>
_tst_cv_status ccw_cv_wait_until(Cv& cv, Lock& lk,
                                 const STD::chrono::time_point<Clock, Dur>& deadline) {
    _tst_cv_status st = cv.wait_until(lk, deadline);
    while (st == _TST_CV_NO_TIMEOUT && Clock::now() < deadline)
        st = cv.wait_until(lk, deadline);
    return st;
}
template <class Cv, class Lock, class Rep, class Period>
_tst_cv_status ccw_cv_wait_for(Cv& cv, Lock& lk,
                               const STD::chrono::duration<Rep, Period>& d) {
    typedef STD::chrono::steady_clock sc;
    sc::time_point deadline = sc::now() + d;
    _tst_cv_status st = cv.wait_for(lk, d);
    while (st == _TST_CV_NO_TIMEOUT && sc::now() < deadline)
        st = cv.wait_for(lk, deadline - sc::now());
    return st;
}
}

TEST_CASE(condition_variable, timed_wait_single_thread) {

#if defined(__WATCOMC__)
    const _tst_cv_status TIMED_OUT = STD::timeout;
    const _tst_cv_status NOT_TIMED_OUT = STD::no_timeout;
#else
    const _tst_cv_status TIMED_OUT = STD::cv_status::timeout;
    const _tst_cv_status NOT_TIMED_OUT = STD::cv_status::no_timeout;
#endif
    STD::condition_variable cv;
    STD::mutex m;
    STD::unique_lock<STD::mutex> lk(m);

    _tst_cv_status s = ccw_cv_wait_for(cv, lk, STD::chrono::milliseconds(1));
    test_true( s == TIMED_OUT );
    test_pass("cxx11:condition_variable::wait_for");
    test_true( TIMED_OUT != NOT_TIMED_OUT );
    test_pass("cxx11:cv_status");

    test_true(  cv.wait_for(lk, STD::chrono::milliseconds(1), cv_true()) );

    test_true( !cv.wait_for(lk, STD::chrono::milliseconds(1), cv_false()) );
    test_pass("cxx11:condition_variable::wait_for(pred)");

    STD::chrono::steady_clock::time_point past = STD::chrono::steady_clock::now();
    _tst_cv_status s2 = ccw_cv_wait_until(cv, lk, past);
    test_true( s2 == TIMED_OUT );
    test_pass("cxx11:condition_variable::wait_until");
    test_true( !cv.wait_until(lk, past, cv_false()) );
    test_pass("cxx11:condition_variable::wait_until(pred)");

    STD::condition_variable_any cva;
    STD::mutex m2;
    STD::unique_lock<STD::mutex> lk2(m2);
    test_true(  cva.wait_for(lk2, STD::chrono::milliseconds(1), cv_true()) );
    test_true( !cva.wait_for(lk2, STD::chrono::milliseconds(1), cv_false()) );
    test_pass("cxx11:condition_variable_any::wait_for(pred)");
}

#if _tst_cplusplus >= 201103L
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <chrono>
#include <stop_token>
#include <type_traits>

namespace {

#if TEST_TARGET_CXX >= 2014
using namespace STD::chrono_literals;
#endif

const STD::chrono::seconds kLong(5);

struct Box {
    STD::mutex              mtx;
    STD::condition_variable cv;
    bool                    ready;
    int                     value;
    Box() : ready(false), value(0) {}

    void produce(int v) {
        {
            STD::lock_guard<STD::mutex> lk(mtx);
            value = v;
            ready = true;
        }
        cv.notify_one();
    }
};

}

TEST_CASE(condition_variable, cv_status_enum) {

    test_true( STD::cv_status::no_timeout != STD::cv_status::timeout );
    test_pass("cxx11:cv_status::enumerators");
    test_true( (STD::is_enum<_tst_cv_status>::value) );

    test_true( (STD::is_same<STD::underlying_type<_tst_cv_status>::type,
                             STD::underlying_type<_tst_cv_status>::type>::value) );
    test_pass("cxx11:cv_status::scoped_enum");

    _tst_cv_status s = STD::cv_status::no_timeout;
    test_true( s == STD::cv_status::no_timeout );
    s = STD::cv_status::timeout;
    test_true( s == STD::cv_status::timeout );
    test_pass("cxx11:cv_status::assignment");
}

TEST_CASE(condition_variable, ctor_dtor_native_handle) {

    {
        STD::condition_variable cv;

        cv.notify_one();
        cv.notify_all();
    }

    test_true( !STD::is_copy_constructible<STD::condition_variable>::value );
    test_true( !STD::is_move_constructible<STD::condition_variable>::value );
    test_pass("cxx11:condition_variable");

    TEST_SKIP1();
    TEST_SKIP1();
}

TEST_CASE(condition_variable, gap_fill) {
#if !defined(_MSC_VER) && !defined(__WATCOMC__)
    STD::condition_variable cv;
    STD::condition_variable::native_handle_type h = cv.native_handle();
    test_true( h == cv.native_handle() );
    test_true( (STD::is_same<STD::condition_variable::native_handle_type,
                             decltype(cv.native_handle())>::value) );
    test_pass("cxx11:condition_variable::native_handle");
#else
    test_skip("cxx11:condition_variable::native_handle");
#endif

    STD::condition_variable_any cva;
    cva.notify_one();
    test_true( true );
    test_pass("cxx11:condition_variable_any::notify_one");
    cva.notify_all();
    test_true( true );
    test_pass("cxx11:condition_variable_any::notify_all");
}

TEST_CASE(condition_variable, wait_and_notify_one) {
    Box box;

    STD::thread producer([&box] { box.produce(42); });

    {
        STD::unique_lock<STD::mutex> lk(box.mtx);

        while (!box.ready)
            box.cv.wait(lk);
        test_eq( box.value, 42 );
        test_pass("cxx11:condition_variable::notify_one");
        test_true( box.ready );
        test_true( lk.owns_lock() );
        test_pass("cxx11:condition_variable::wait");
    }

    producer.join();
}

TEST_CASE(condition_variable, wait_with_predicate_and_notify_all) {
    Box box;

    int seen1 = 0, seen2 = 0;
    STD::thread w1([&box, &seen1] {
        STD::unique_lock<STD::mutex> lk(box.mtx);
        box.cv.wait(lk, [&box] { return box.ready; });
        seen1 = box.value;
    });
    STD::thread w2([&box, &seen2] {
        STD::unique_lock<STD::mutex> lk(box.mtx);
        box.cv.wait(lk, [&box] { return box.ready; });
        seen2 = box.value;
    });

    {
        STD::lock_guard<STD::mutex> lk(box.mtx);
        box.value = 7;
        box.ready = true;
    }
    box.cv.notify_all();

    w1.join();
    w2.join();
    test_eq( seen1, 7 );
    test_eq( seen2, 7 );
    test_pass("cxx11:condition_variable::notify_all");

    {
        STD::unique_lock<STD::mutex> lk(box.mtx);
        bool r = box.cv.wait_for(lk, STD::chrono::milliseconds(0),
                                 [&box] { return box.ready; });
        test_true( r );
        test_pass("cxx11:condition_variable::wait(pred)");
    }
}

TEST_CASE(condition_variable, wait_for) {
    Box box;

    STD::thread producer([&box] { box.produce(11); });
    {
        STD::unique_lock<STD::mutex> lk(box.mtx);

        bool ok = box.cv.wait_for(lk, kLong, [&box] { return box.ready; });
        test_true( ok );
        test_eq( box.value, 11 );
        test_pass("cxx11:condition_variable::wait_for(pred,notify)");
    }
    producer.join();

    {
        STD::condition_variable cv;
        STD::mutex              m;
        STD::unique_lock<STD::mutex> lk(m);

        _tst_cv_status st = ccw_cv_wait_for(cv, lk, STD::chrono::milliseconds(20));
        test_true( st == STD::cv_status::timeout );
        test_true( lk.owns_lock() );
        test_pass("cxx11:condition_variable::wait_for(timeout)");

        bool ok = cv.wait_for(lk, STD::chrono::milliseconds(20),
                              [] { return false; });
        test_true( !ok );
        test_pass("cxx11:condition_variable::wait_for(pred,false)");
    }
}

TEST_CASE(condition_variable, wait_until) {
    typedef STD::chrono::steady_clock clock;
    Box box;

    STD::thread producer([&box] { box.produce(13); });
    {
        STD::unique_lock<STD::mutex> lk(box.mtx);
        bool ok = box.cv.wait_until(lk, clock::now() + kLong,
                                    [&box] { return box.ready; });
        test_true( ok );
        test_eq( box.value, 13 );
        test_pass("cxx11:condition_variable::wait_until(pred,notify)");
    }
    producer.join();

    {
        STD::condition_variable cv;
        STD::mutex              m;
        STD::unique_lock<STD::mutex> lk(m);

        clock::time_point deadline = clock::now() + STD::chrono::milliseconds(20);
        _tst_cv_status st = ccw_cv_wait_until(cv, lk, deadline);
        test_true( st == STD::cv_status::timeout );
        test_true( clock::now() >= deadline );
        test_pass("cxx11:condition_variable::wait_until(timeout)");

        bool ok = cv.wait_until(lk, clock::now() + STD::chrono::milliseconds(20),
                                [] { return false; });
        test_true( !ok );
        test_pass("cxx11:condition_variable::wait_until(pred,false)");

        bool ok2 = cv.wait_until(lk, clock::now() - STD::chrono::seconds(1),
                                 [] { return true; });
        test_true( ok2 );
        test_pass("cxx11:condition_variable::wait_until(past)");
    }

    {
        STD::condition_variable cv;
        STD::mutex              m;
        STD::unique_lock<STD::mutex> lk(m);
        _tst_cv_status st = ccw_cv_wait_until(
            cv, lk, STD::chrono::system_clock::now() + STD::chrono::milliseconds(20));
        test_true( st == STD::cv_status::timeout );
        test_pass("cxx11:condition_variable::wait_until(system_clock)");
    }
}

TEST_CASE(condition_variable, notify_all_at_thread_exit) {
    STD::mutex              m;
    STD::condition_variable cv;
    bool                    done = false;

    STD::thread t([&m, &cv, &done] {
        STD::unique_lock<STD::mutex> lk(m);
        done = true;

        STD::notify_all_at_thread_exit(cv, STD::move(lk));

    });

    {
        STD::unique_lock<STD::mutex> lk(m);
        bool ok = cv.wait_for(lk, kLong, [&done] { return done; });
        test_true( ok );
    }

    t.join();
    test_true( done );
    test_pass("cxx11:notify_all_at_thread_exit");
}

TEST_CASE(condition_variable, any_ctor_notify) {
    {
        STD::condition_variable_any cv;
        cv.notify_one();
        cv.notify_all();
    }
    test_true( !STD::is_copy_constructible<STD::condition_variable_any>::value );
    test_true( !STD::is_move_constructible<STD::condition_variable_any>::value );
    test_pass("cxx11:condition_variable_any");
}

TEST_CASE(condition_variable, any_wait_and_predicate) {

    STD::mutex                  m;
    STD::condition_variable_any cv;
    bool                        ready = false;
    int                         value = 0;

    STD::thread producer([&] {
        {
            STD::lock_guard<STD::mutex> lk(m);
            value = 21;
            ready = true;
        }
        cv.notify_all();
    });

    {
        STD::unique_lock<STD::mutex> lk(m);
        while (!ready)
            cv.wait(lk);
        test_eq( value, 21 );
        test_pass("cxx11:condition_variable_any::wait");
    }
    producer.join();

#if TEST_TARGET_CXX >= 2017
    STD::shared_mutex           sm;
    STD::condition_variable_any cv2;
    bool                        flag = false;

    STD::thread t2([&] {
        {
            STD::lock_guard<STD::shared_mutex> lk(sm);
            flag = true;
        }
        cv2.notify_one();
    });
    {
        STD::unique_lock<STD::shared_mutex> lk(sm);
        cv2.wait(lk, [&flag] { return flag; });
        test_true( flag );
        test_pass("cxx11:condition_variable_any::wait(pred)");
    }
    t2.join();
#else
    TEST_NOTE("condition_variable_any over a shared_mutex needs <shared_mutex> (C++17)");
    test_skip("cxx11:condition_variable_any::wait(pred)");
#endif
}

TEST_CASE(condition_variable, any_wait_for_until) {
    typedef STD::chrono::steady_clock clock;

    STD::mutex                  m;
    STD::condition_variable_any cv;
    bool                        ready = false;

    STD::thread producer([&] {
        {
            STD::lock_guard<STD::mutex> lk(m);
            ready = true;
        }
        cv.notify_all();
    });
    {
        STD::unique_lock<STD::mutex> lk(m);
        bool ok = cv.wait_for(lk, kLong, [&ready] { return ready; });
        test_true( ok );
        test_pass("cxx11:condition_variable_any::wait_for");

        bool ok2 = cv.wait_until(lk, clock::now() + kLong, [&ready] { return ready; });
        test_true( ok2 );
        test_pass("cxx11:condition_variable_any::wait_until(pred)");
    }
    producer.join();

    {
        STD::mutex                  m2;
        STD::condition_variable_any cv2;
        STD::unique_lock<STD::mutex> lk(m2);

        _tst_cv_status st = ccw_cv_wait_for(cv2, lk, STD::chrono::milliseconds(20));
        test_true( st == STD::cv_status::timeout );
        test_pass("cxx11:condition_variable_any::wait_for(timeout)");

        _tst_cv_status st2 = ccw_cv_wait_until(cv2, lk, clock::now() + STD::chrono::milliseconds(20));
        test_true( st2 == STD::cv_status::timeout );
        test_pass("cxx11:condition_variable_any::wait_until(timeout)");

        bool ok = cv2.wait_for(lk, STD::chrono::milliseconds(20), [] { return false; });
        test_true( !ok );
        test_pass("cxx11:condition_variable_any::wait_for(false)");

        bool ok2 = cv2.wait_until(lk, clock::now() + STD::chrono::milliseconds(20),
                                  [] { return false; });
        test_true( !ok2 );
        test_pass("cxx11:condition_variable_any::wait_until(false)");
    }
}

#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_jthread)
TEST_CASE(condition_variable, any_wait_with_stop_token) {
    typedef STD::chrono::steady_clock clock;

    {
        STD::mutex                  m;
        STD::condition_variable_any cv;
        bool                        ready = false;
        STD::stop_source            src;

        STD::thread producer([&] {
            {
                STD::lock_guard<STD::mutex> lk(m);
                ready = true;
            }
            cv.notify_all();
        });
        {
            STD::unique_lock<STD::mutex> lk(m);
            bool ok = cv.wait(lk, src.get_token(), [&ready] { return ready; });
            test_true( ok );
            test_pass("cxx20:condition_variable_any::wait(stop_token)");
            bool ok2 = cv.wait_for(lk, src.get_token(), kLong,
                                   [&ready] { return ready; });
            test_true( ok2 );
            test_pass("cxx20:condition_variable_any::wait_for(stop_token)");
            bool ok3 = cv.wait_until(lk, src.get_token(), clock::now() + kLong,
                                     [&ready] { return ready; });
            test_true( ok3 );
            test_pass("cxx20:condition_variable_any::wait_until(stop_token)");
        }
        producer.join();
        test_true( !src.stop_requested() );
    }

    {
        STD::mutex                  m;
        STD::condition_variable_any cv;
        STD::stop_source            src;

        STD::thread stopper([&src] { src.request_stop(); });
        {
            STD::unique_lock<STD::mutex> lk(m);
            bool ok = cv.wait(lk, src.get_token(), [] { return false; });
            test_true( !ok );
            test_true( lk.owns_lock() );
            test_pass("cxx20:condition_variable_any::wait(stop_token,stopped)");
        }
        stopper.join();
        test_true( src.stop_requested() );

        STD::unique_lock<STD::mutex> lk(m);
        bool ok2 = cv.wait_for(lk, src.get_token(), kLong, [] { return false; });
        test_true( !ok2 );
        test_pass("cxx20:condition_variable_any::wait_for(stop_token,stopped)");
        bool ok3 = cv.wait_until(lk, src.get_token(), clock::now() + kLong,
                                 [] { return false; });
        test_true( !ok3 );
        test_pass("cxx20:condition_variable_any::wait_until(stop_token,stopped)");
    }
}
#else
TEST_CASE(condition_variable, any_wait_with_stop_token) {
    TEST_NOTE("the stop_token overloads of condition_variable_any need jthread support");
    test_skip("cxx20:condition_variable_any::wait(stop_token)");
    test_skip("cxx20:condition_variable_any::wait_for(stop_token)");
    test_skip("cxx20:condition_variable_any::wait_until(stop_token)");
    test_skip("cxx20:condition_variable_any::wait(stop_token,stopped)");
    test_skip("cxx20:condition_variable_any::wait_for(stop_token,stopped)");
    test_skip("cxx20:condition_variable_any::wait_until(stop_token,stopped)");
}
#endif
#else
TEST_CASE_SKIP(condition_variable, cv_status_enum)
TEST_CASE_SKIP(condition_variable, ctor_dtor_native_handle)
TEST_CASE_SKIP(condition_variable, wait_and_notify_one)
TEST_CASE_SKIP(condition_variable, wait_with_predicate_and_notify_all)
TEST_CASE_SKIP(condition_variable, wait_for)
TEST_CASE_SKIP(condition_variable, wait_until)
TEST_CASE_SKIP(condition_variable, notify_all_at_thread_exit)
TEST_CASE_SKIP(condition_variable, any_ctor_notify)
TEST_CASE_SKIP(condition_variable, any_wait_and_predicate)
TEST_CASE_SKIP(condition_variable, any_wait_for_until)
TEST_CASE_SKIP(condition_variable, any_wait_with_stop_token)
#endif


#if TEST_TARGET_CXX >= 2011
TEST_CASE(condition_variable, native_handle_and_assign_cxx11) {
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::condition_variable>::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible move assignment as assignable (vc bugs A26)");
    test_true( !STD::is_move_assignable<STD::condition_variable>::value );
    test_pass("cxx11:condition_variable::operator=");

    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::condition_variable_any>::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible move assignment as assignable (vc bugs A26)");
    test_true( !STD::is_move_assignable<STD::condition_variable_any>::value );
    test_pass("cxx11:condition_variable_any::operator=");

#if !defined(_MSC_VER) && !defined(__WATCOMC__)
    STD::condition_variable cv;
    test_true(( STD::is_same<STD::condition_variable::native_handle_type,
                             decltype(cv.native_handle())>::value ));
    test_pass("cxx11:condition_variable::native_handle_type");
#else
    TEST_NOTE("native_handle is optional and this implementation does not provide it");
    test_skip("cxx11:condition_variable::native_handle_type");
#endif
}
#else
TEST_CASE(condition_variable, native_handle_and_assign_cxx11) {
    test_skip("cxx11:condition_variable::operator=");
    test_skip("cxx11:condition_variable_any::operator=");
    test_skip("cxx11:condition_variable::native_handle_type");
}
#endif


#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_jthread)
namespace {
struct CcwPredTrue  { bool operator()() const { return true; } };
struct CcwPredFalse { bool operator()() const { return false; } };
}

TEST_CASE(condition_variable, wait_with_stop_token_cxx20) {
    STD::condition_variable_any cv;
    STD::mutex m;
    STD::stop_source src;

    {
        STD::unique_lock<STD::mutex> lk(m);
        test_true( cv.wait(lk, src.get_token(), CcwPredTrue()) );
        test_true( lk.owns_lock() );
    }
    test_pass("cxx20:condition_variable_any::wait(stop_token)");

    src.request_stop();
    {
        STD::unique_lock<STD::mutex> lk(m);
        test_true( !cv.wait(lk, src.get_token(), CcwPredFalse()) );
        test_true( lk.owns_lock() );
    }
    test_pass("cxx20:condition_variable_any::wait(stop_token,stopped)");

    STD::chrono::milliseconds d(5);
    {
        STD::unique_lock<STD::mutex> lk(m);
        STD::stop_source fresh;
        test_true( cv.wait_for(lk, fresh.get_token(), d, CcwPredTrue()) );
    }
    test_pass("cxx20:condition_variable_any::wait_for(stop_token)");
    {
        STD::unique_lock<STD::mutex> lk(m);
        test_true( !cv.wait_for(lk, src.get_token(), d, CcwPredFalse()) );
    }
    test_pass("cxx20:condition_variable_any::wait_for(stop_token,stopped)");

    STD::chrono::steady_clock::time_point tp = STD::chrono::steady_clock::now() + d;
    {
        STD::unique_lock<STD::mutex> lk(m);
        STD::stop_source fresh;
        test_true( cv.wait_until(lk, fresh.get_token(), tp, CcwPredTrue()) );
    }
    test_pass("cxx20:condition_variable_any::wait_until(stop_token)");
    {
        STD::unique_lock<STD::mutex> lk(m);
        test_true( !cv.wait_until(lk, src.get_token(), tp, CcwPredFalse()) );
    }
    test_pass("cxx20:condition_variable_any::wait_until(stop_token,stopped)");
}
#else
TEST_CASE(condition_variable, wait_with_stop_token_cxx20) {
    TEST_NOTE("the stop_token overloads need <stop_token> and jthread support");
    test_skip("cxx20:condition_variable_any::wait(stop_token)");
    test_skip("cxx20:condition_variable_any::wait(stop_token,stopped)");
    test_skip("cxx20:condition_variable_any::wait_for(stop_token)");
    test_skip("cxx20:condition_variable_any::wait_for(stop_token,stopped)");
    test_skip("cxx20:condition_variable_any::wait_until(stop_token)");
    test_skip("cxx20:condition_variable_any::wait_until(stop_token,stopped)");
}
#endif
