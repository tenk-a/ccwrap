#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <chrono>
#include <sstream>
#if defined(__cpp_lib_formatters)
#include <format>
#endif

namespace {

struct IncOnce   { int* p; IncOnce(int* q) : p(q) {} void operator()() const { ++(*p); } };
struct ThreadFn  { int* p; int n; ThreadFn(int* p_, int n_) : p(p_), n(n_) {}
                   void operator()() const { for (int i = 0; i < n; ++i) ++(*p); } };
struct ThreadFn1 { int* p; ThreadFn1(int* p_) : p(p_) {} void operator()(int add) const { *p += add; } };
struct MutexInc  { int* p; STD::mutex* m; int n; MutexInc(int* q, STD::mutex* mm, int nn) : p(q), m(mm), n(nn) {}
                   void operator()() const { for (int i = 0; i < n; ++i) { STD::lock_guard<STD::mutex> g(*m); ++(*p); } } };
struct AtomicInc { STD::atomic<int>* a; int n; AtomicInc(STD::atomic<int>* aa, int nn) : a(aa), n(nn) {}
                   void operator()() const { for (int i = 0; i < n; ++i) ++(*a); } };
#if TEST_TARGET_CXX >= 2017
struct ShMxInc   { int* p; STD::shared_mutex* m; int n; ShMxInc(int* q, STD::shared_mutex* mm, int nn) : p(q), m(mm), n(nn) {}
                   void operator()() const { for (int i = 0; i < n; ++i) { STD::lock_guard<STD::shared_mutex> g(*m); ++(*p); } } };
#endif
struct CvConsumer { STD::mutex* m; STD::condition_variable* cv; bool* ready; int* out;
    CvConsumer(STD::mutex* mm, STD::condition_variable* c, bool* r, int* o) : m(mm), cv(c), ready(r), out(o) {}
    void operator()() const {
        STD::unique_lock<STD::mutex> lk(*m);
        while (!*ready) cv->wait(lk);
        *out = 99;
    } };
#if _TST_HAS_FUTURE
struct PromiseProducer { STD::promise<int>* p; PromiseProducer(STD::promise<int>* q) : p(q) {}
                         void operator()() const { p->set_value(123); } };
#endif

}

TEST_CASE(mutex, lock_types) {
    STD::mutex m;
    { STD::lock_guard<STD::mutex> g(m); }

    STD::unique_lock<STD::mutex> ul(m);
    test_true( ul.owns_lock() );
    ul.unlock();
    test_true( !ul.owns_lock() );
    test_pass("cxx11:unique_lock");

    test_true( m.try_lock() );
    m.unlock();
    test_pass("cxx11:mutex::try_lock");

#if TEST_TARGET_CXX >= 2017
    { STD::scoped_lock<STD::mutex> sl(m); }
#endif
}

TEST_CASE(mutex, call_once) {
    static STD::once_flag of;
    static int oc = 0;
    STD::call_once(of, IncOnce(&oc));
    STD::call_once(of, IncOnce(&oc));
    test_eq( oc, 1 );
    test_pass("cxx11:call_once");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(mutex, shared_mutex) {
    STD::shared_mutex sm;
    { STD::shared_lock<STD::shared_mutex> rl(sm); test_true( rl.owns_lock() ); }
    { STD::lock_guard<STD::shared_mutex> wl(sm); }
}
#endif

TEST_CASE(thread, join_and_args) {
    int counter = 0;
    STD::thread t(ThreadFn(&counter, 1000));
    test_true( t.joinable() );
    test_pass("cxx11:thread::joinable");
    t.join();
    test_eq( counter, 1000 );
    test_true( !t.joinable() );
    test_pass("cxx11:thread::join");

    int acc = 5;
    STD::thread t2(ThreadFn1(&acc), 37);
    t2.join();
    test_eq( acc, 42 );
    test_pass("cxx11:thread");

    test_true( STD::thread::hardware_concurrency() >= 1 );
    test_pass("cxx11:thread::hardware_concurrency");
    STD::this_thread::sleep_for(STD::chrono::milliseconds(1));
    test_true( 1 );
    test_pass("cxx11:this_thread::sleep_for");
    STD::this_thread::yield();
    test_true( 1 );
    test_pass("cxx11:this_thread::yield");
}

TEST_CASE(thread, mutual_exclusion) {
    int shared = 0;
    STD::mutex mm;
    STD::thread a(MutexInc(&shared, &mm, 5000));
    STD::thread b(MutexInc(&shared, &mm, 5000));
    a.join(); b.join();
    test_eq( shared, 10000 );
}

TEST_CASE(thread, condition_variable) {
    STD::mutex cm;
    STD::condition_variable cv;
    bool ready = false;
    int out = 0;
    STD::thread c(CvConsumer(&cm, &cv, &ready, &out));
    { STD::lock_guard<STD::mutex> g(cm); ready = true; }
    cv.notify_one();
    c.join();
    test_eq( out, 99 );
}

TEST_CASE(thread, atomic_increment) {
    STD::atomic<int> ac(0);
    STD::thread x(AtomicInc(&ac, 5000));
    STD::thread y(AtomicInc(&ac, 5000));
    x.join(); y.join();
    test_eq( ac.load(), 10000 );
}

#if _TST_HAS_FUTURE
TEST_CASE(thread, promise_future) {
    STD::promise<int> pr;
    STD::future<int> fut = pr.get_future();
    test_true( fut.valid() );
    test_pass("cxx11:promise::get_future");
    PromiseProducer prod(&pr);
    STD::thread pt(prod);
    int fv = fut.get();
    pt.join();
    test_eq( fv, 123 );
    test_true( !fut.valid() );
}
#else
TEST_CASE_SKIP(thread, promise_future)
#endif

#if TEST_TARGET_CXX >= 2017
TEST_CASE(thread, shared_mutex_exclusive_writes) {
    int shv = 0;
    STD::shared_mutex shm;
    STD::thread w1(ShMxInc(&shv, &shm, 5000));
    STD::thread w2(ShMxInc(&shv, &shm, 5000));
    w1.join(); w2.join();
    test_eq( shv, 10000 );
    { STD::shared_lock<STD::shared_mutex> rl(shm); test_eq( shv, 10000 ); }
}
#endif

TEST_CASE(thread, id_compare) {
    STD::thread::id a;
    STD::thread::id b = STD::this_thread::get_id();

    test_true( a <= a ); test_true( a >= a );
    test_true( !(a < a) ); test_true( !(a > a) );
    test_pass("cxx11:thread::id");

    test_true( (a < b) == (b > a) );
    test_true( (a <= b) == !(b < a) );
    test_true( (a >= b) == !(a < b) );
    test_true( (a != b) == !(a == b) );
    test_pass("cxx11:this_thread::get_id");
}

TEST_CASE(thread, thread_members) {
    STD::thread t0;
    test_true( !t0.joinable() );
    test_pass("cxx11:thread::thread() default ctor");

    int x = 0;
    STD::thread t1(ThreadFn1(&x), 5);
    test_true( t1.joinable() );
    test_pass("cxx11:thread::thread(F&&, Args&&...)");

    test_true( t1.get_id() != STD::thread::id() );
    test_pass("cxx11:thread::get_id");

    (void)t1.native_handle();
    test_true( true );
    test_pass("cxx11:thread::native_handle");

#if _TST_HAS_RVALUE_REF
    STD::thread t2(STD::move(t1));
    test_true( !t1.joinable() && t2.joinable() );
    test_pass("cxx11:thread::thread(thread&&) move ctor");

    STD::thread t3;
    t3 = STD::move(t2);
    test_true( !t2.joinable() && t3.joinable() );
    test_pass("cxx11:thread::operator=(thread&&)");
#else
    STD::thread t3;
    t3.swap(t1);
    test_true( !t1.joinable() && t3.joinable() );
    test_skip("cxx11:thread::thread(thread&&) move ctor");
    test_skip("cxx11:thread::operator=(thread&&)");
#endif

    STD::thread t4;
    t3.swap(t4);
    test_true( t4.joinable() && !t3.joinable() );
    test_pass("cxx11:thread::swap");
    STD::swap(t3, t4);
    test_true( t3.joinable() && !t4.joinable() );
    test_pass("cxx11:swap(thread&, thread&)");

    t3.join();
    test_eq( x, 5 );

    int y = 0;
    IncOnce inc(&y);
    STD::thread td(inc);
    td.detach();
    test_true( !td.joinable() );
    test_pass("cxx11:thread::detach");
}

TEST_CASE(thread, thread_id_ops) {
    STD::thread::id a = STD::this_thread::get_id();
    STD::thread::id b = a;
    test_true( a == b );
    STD::thread::id z;
    test_true( !(a == z) );
    test_pass("cxx11:thread::id::operator==");

    test_true( !(a < a) && (a <= a) && !(a > a) && (a >= a) );
    test_pass("cxx11:thread::id relational operators (< <= > >=, pre-C++20)");

    test_true( STD::hash<STD::thread::id>()(a) == STD::hash<STD::thread::id>()(b) );
    test_pass("cxx11:hash<thread::id>");

    STD::ostringstream os; os << a;
    test_true( !os.str().empty() );
    test_pass("cxx11:operator<<(thread::id)");

#if defined(__cpp_lib_three_way_comparison)
    test_true( (a <=> a) == 0 );
    test_pass("cxx20:thread::id::operator<=>");
#else
    test_skip("cxx20:thread::id::operator<=>");
#endif
}

TEST_CASE(thread, sleep_until) {
    STD::chrono::steady_clock::time_point t =
        STD::chrono::steady_clock::now() + STD::chrono::milliseconds(2);
    STD::this_thread::sleep_until(t);
    test_true( STD::chrono::steady_clock::now() >= t );
    test_pass("cxx11:this_thread::sleep_until");

#if defined(__cpp_lib_formatters)
    STD::string fs = STD::format("{}", STD::this_thread::get_id());
    test_true( !fs.empty() );
    test_pass("cxx23:formatter<thread::id>");
#else
    test_skip("cxx23:formatter<thread::id>");
#endif
}

#if defined(__cpp_lib_jthread)
TEST_CASE(thread, jthread_members) {
    STD::jthread j0;
    test_true( !j0.joinable() );
    test_pass("cxx20:jthread::jthread() default ctor");

    int x = 0;
    STD::jthread j1(ThreadFn1(&x), 5);
    test_true( j1.joinable() );
    test_pass("cxx20:jthread::jthread(F&&, Args&&...)");
    test_true( j1.joinable() );
    test_pass("cxx20:jthread::joinable");
    test_true( j1.get_id() != STD::jthread::id() );
    test_pass("cxx20:jthread::get_id");
    (void)j1.native_handle();
    test_pass("cxx20:jthread::native_handle");
    test_true( STD::jthread::hardware_concurrency() >= 1 );
    test_pass("cxx20:jthread::hardware_concurrency");
    STD::stop_source ss = j1.get_stop_source();
    test_true( ss.stop_possible() );
    test_pass("cxx20:jthread::get_stop_source");
    STD::stop_token stk = j1.get_stop_token();
    test_true( !stk.stop_requested() );
    test_pass("cxx20:jthread::get_stop_token");
    j1.join();
    test_eq( x, 5 );
    test_pass("cxx20:jthread::join");

    STD::jthread j2(ThreadFn1(&x), 1);
    STD::jthread j3(STD::move(j2));
    test_true( !j2.joinable() && j3.joinable() );
    test_pass("cxx20:jthread::jthread(jthread&&) move ctor");
    STD::jthread j4;
    j4 = STD::move(j3);
    test_true( !j3.joinable() && j4.joinable() );
    test_pass("cxx20:jthread::operator=(jthread&&)");
    STD::jthread j5;
    j4.swap(j5);
    test_true( j5.joinable() && !j4.joinable() );
    test_pass("cxx20:jthread::swap");
    STD::swap(j4, j5);
    test_true( j4.joinable() && !j5.joinable() );
    test_pass("cxx20:swap(jthread&, jthread&)");
    j4.join();

    STD::atomic<bool> ran(false);
    STD::jthread jr([&ran](STD::stop_token st) { while (!st.stop_requested()) {} ran = true; });
    test_true( jr.request_stop() );
    jr.join();
    test_true( ran.load() );
    test_pass("cxx20:jthread::request_stop");

    STD::jthread jx(ThreadFn1(&x), 0);
    jx.detach();
    test_true( !jx.joinable() );
    test_pass("cxx20:jthread::detach");

    {
        STD::atomic<bool> r2(false);
        STD::jthread jd([&r2](STD::stop_token st) { while (!st.stop_requested()) {} r2 = true; });
    }
    test_true( true );
    test_pass("cxx20:jthread::~jthread (request_stop + join)");
}
#else
TEST_CASE(thread, jthread_members) {
    test_skip("cxx20:jthread::jthread() default ctor"); test_skip("cxx20:jthread::jthread(F&&, Args&&...)");
    test_skip("cxx20:jthread::joinable"); test_skip("cxx20:jthread::get_id");
    test_skip("cxx20:jthread::native_handle"); test_skip("cxx20:jthread::hardware_concurrency");
    test_skip("cxx20:jthread::get_stop_source"); test_skip("cxx20:jthread::get_stop_token");
    test_skip("cxx20:jthread::join"); test_skip("cxx20:jthread::jthread(jthread&&) move ctor");
    test_skip("cxx20:jthread::operator=(jthread&&)"); test_skip("cxx20:jthread::swap");
    test_skip("cxx20:swap(jthread&, jthread&)"); test_skip("cxx20:jthread::request_stop");
    test_skip("cxx20:jthread::detach"); test_skip("cxx20:jthread::~jthread (request_stop + join)");
}
#endif
#endif


#if TEST_TARGET_CXX >= 2011
TEST_CASE(thread, native_handle_types_cxx11) {
#if !defined(_MSC_VER) && !defined(__WATCOMC__)
    STD::thread t;
    test_true(( STD::is_same<STD::thread::native_handle_type,
                             decltype(t.native_handle())>::value ));
    test_pass("cxx11:thread::native_handle_type");
#else
    TEST_NOTE("native_handle is optional and this implementation does not provide it");
    test_skip("cxx11:thread::native_handle_type");
#endif

#if defined(__cpp_lib_jthread) && !defined(_MSC_VER) && !defined(__WATCOMC__)
    STD::jthread j;
    test_true(( STD::is_same<STD::jthread::native_handle_type,
                             decltype(j.native_handle())>::value ));
    test_pass("cxx20:jthread::native_handle_type");
#else
    test_skip("cxx20:jthread::native_handle_type");
#endif

    TEST_NOTE("C++26 thread attribute hints (P2019); no library here defines them");
}
#else
TEST_CASE(thread, native_handle_types_cxx11) {
    test_skip("cxx11:thread::native_handle_type");
    test_skip("cxx20:jthread::native_handle_type");
}
#endif

TEST_CASE(thread, feature_test_macros) {
#if defined(__cpp_lib_jthread) && (__cpp_lib_jthread) >= 201911L
    test_true( (__cpp_lib_jthread) >= 201911L );
    test_pass("cxx20:__cpp_lib_jthread=201911L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_jthread=201911L");
#endif
}
