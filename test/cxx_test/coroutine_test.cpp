#include "test_cxx.hpp"

TEST_CASE_SKIP(coroutine, unimplemented)

#if _tst_cplusplus >= 202002L
#include <coroutine>
#include <exception>
#include <functional>
#include <type_traits>
#include <utility>
#include <compare>

namespace {

struct Task {
    struct promise_type {
        int steps;
        int final_value;
        promise_type() : steps(0), final_value(0) {}

        Task get_return_object() {
            return Task(STD::coroutine_handle<promise_type>::from_promise(*this));
        }
        STD::suspend_always initial_suspend() noexcept { return {}; }
        STD::suspend_always final_suspend()   noexcept { return {}; }
        void return_value(int v) { final_value = v; }
        void unhandled_exception() { STD::terminate(); }
    };

    typedef STD::coroutine_handle<promise_type> handle_type;

    explicit Task(handle_type h) : h_(h) {}
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    ~Task() { if (h_) h_.destroy(); }

    handle_type handle() const { return h_; }

    handle_type release() { handle_type h = h_; h_ = handle_type(); return h; }

    handle_type h_;
};

Task make_task(int* trace) {
    *trace = 1;
    co_await STD::suspend_always{};
    *trace = 2;
    co_await STD::suspend_always{};
    *trace = 3;
    co_return 99;
}

struct EagerTask {
    struct promise_type {
        int value;
        promise_type() : value(0) {}
        EagerTask get_return_object() {
            return EagerTask(STD::coroutine_handle<promise_type>::from_promise(*this));
        }
        STD::suspend_never  initial_suspend() noexcept { return {}; }
        STD::suspend_always final_suspend()   noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { STD::terminate(); }
    };
    typedef STD::coroutine_handle<promise_type> handle_type;

    explicit EagerTask(handle_type h) : h_(h) {}
    EagerTask(const EagerTask&) = delete;
    EagerTask& operator=(const EagerTask&) = delete;
    ~EagerTask() { if (h_) h_.destroy(); }

    handle_type h_;
};

EagerTask make_eager(int* trace) {
    *trace = 1;
    co_await STD::suspend_always{};
    *trace = 2;
    co_await STD::suspend_never{};
    *trace = 3;
    co_return;
}

struct ReadyAwaiter {
    bool await_ready() const noexcept { return true; }
    void await_suspend(STD::coroutine_handle<>) const noexcept {}
    int  await_resume() const noexcept { return 5; }
};

Task awaiting_task(int* out) {
    int v = co_await ReadyAwaiter{};
    *out = v;
    co_return v;
}

}

TEST_CASE(coroutine, coroutine_traits) {

    test_true( (STD::is_same<STD::coroutine_traits<Task>::promise_type,
                             Task::promise_type>::value) );
    test_true( (STD::is_same<STD::coroutine_traits<Task, int*>::promise_type,
                             Task::promise_type>::value) );
    test_true( (STD::is_same<STD::coroutine_traits<EagerTask, int*>::promise_type,
                             EagerTask::promise_type>::value) );

    test_true( (STD::is_same<
                    STD::coroutine_traits<decltype(make_task(nullptr)), int*>::promise_type,
                    Task::promise_type>::value) );
    test_pass("cxx20:coroutine_traits");
}

TEST_CASE(coroutine, handle_default_and_null) {

    STD::coroutine_handle<> h0;
    STD::coroutine_handle<> h1(nullptr);
    STD::coroutine_handle<Task::promise_type> hp0;
    STD::coroutine_handle<Task::promise_type> hp1 = nullptr;

    test_true( !h0 );
    test_true( !hp0 );
    test_pass("cxx20:coroutine_handle-default-ctor");
    test_true( !h1 );
    test_true( !hp1 );
    test_pass("cxx20:coroutine_handle-nullptr-ctor");

    test_true( static_cast<bool>(h0) == false );
    test_true( !static_cast<bool>(hp1) );
    test_pass("cxx20:coroutine_handle::operator bool");

    test_true( h0.address() == nullptr );
    test_true( h0 == h1 );
    test_true( hp0 == hp1 );
    test_pass("cxx20:coroutine_handle::address(null)");

    int trace = 0;
    Task t = make_task(&trace);
    STD::coroutine_handle<Task::promise_type> h = t.handle();
    test_true( !!h );
    h = nullptr;
    test_true( !h );
    test_true( h.address() == nullptr );
    test_pass("cxx20:coroutine_handle::operator=(nullptr_t)");

    test_true( t.handle() != h0 );
    test_pass("cxx20:coroutine_handle::operator!=");
}

TEST_CASE(coroutine, from_promise_address_conversion) {
    int trace = 0;
    Task t = make_task(&trace);
    STD::coroutine_handle<Task::promise_type> h = t.handle();

    Task::promise_type& p = h.promise();
    test_eq( p.steps, 0 );
    test_pass("cxx20:coroutine_handle::promise");

    STD::coroutine_handle<Task::promise_type> h2 =
        STD::coroutine_handle<Task::promise_type>::from_promise(p);
    test_true( h2 == h );
    test_pass("cxx20:coroutine_handle::from_promise");

    void* addr = h.address();
    test_true( addr != nullptr );
    test_pass("cxx20:coroutine_handle::address");
    STD::coroutine_handle<Task::promise_type> h3 =
        STD::coroutine_handle<Task::promise_type>::from_address(addr);
    test_true( h3 == h );

    STD::coroutine_handle<> hv = STD::coroutine_handle<>::from_address(addr);
    test_true( hv.address() == addr );
    test_pass("cxx20:coroutine_handle::from_address");

    STD::coroutine_handle<> erased = h;
    test_true( erased.address() == h.address() );
    test_true( erased == hv );
    test_pass("cxx20:coroutine_handle-conversion-to-void-handle");

    TEST_SKIP1();
    test_true( (STD::is_same<decltype(h.promise()), Task::promise_type&>::value) );
    test_pass("cxx20:coroutine_handle::promise-returns-ref");
}

TEST_CASE(coroutine, resume_done_destroy) {
    int  trace = 0;
    Task t = make_task(&trace);
    STD::coroutine_handle<Task::promise_type> h = t.handle();

    test_eq( trace, 0 );
    test_true( !h.done() );

    h.resume();
    test_eq( trace, 1 );
    test_true( !h.done() );
    test_pass("cxx20:coroutine_handle::resume");

    h();
    test_eq( trace, 2 );
    test_true( !h.done() );
    test_pass("cxx20:coroutine_handle::operator()");

    h.resume();
    test_eq( trace, 3 );
    test_true( h.done() );
    test_eq( h.promise().final_value, 99 );
    test_pass("cxx20:coroutine_handle::done");

    test_true( !!h );

    int   trace2 = 0;
    Task  t2 = make_task(&trace2);
    STD::coroutine_handle<Task::promise_type> h2 = t2.release();
    h2.resume();
    test_eq( trace2, 1 );
    h2.destroy();
    test_eq( trace2, 1 );
    test_pass("cxx20:coroutine_handle::destroy");
}

TEST_CASE(coroutine, suspend_never_and_always) {

    STD::suspend_never  never;
    STD::suspend_always always;

    test_true(  never.await_ready() );
    test_pass("cxx20:suspend_never::await_ready");
    test_true( !always.await_ready() );
    test_pass("cxx20:suspend_always::await_ready");

    test_no_throw( never.await_suspend(STD::coroutine_handle<>()) );
    test_pass("cxx20:suspend_never::await_suspend");
    test_no_throw( never.await_resume() );
    test_pass("cxx20:suspend_never::await_resume");
    test_no_throw( always.await_suspend(STD::coroutine_handle<>()) );
    test_pass("cxx20:suspend_always::await_suspend");
    test_no_throw( always.await_resume() );
    test_pass("cxx20:suspend_always::await_resume");

    test_true( (STD::is_same<decltype(never.await_resume()),  void>::value) );
    test_true( (STD::is_same<decltype(never.await_suspend(STD::coroutine_handle<>())),
                             void>::value) );
    test_true( STD::is_empty<STD::suspend_never>::value );
    test_pass("cxx20:suspend_never-signatures-and-empty");
    test_true( (STD::is_same<decltype(always.await_resume()), void>::value) );
    test_true( (STD::is_same<decltype(always.await_suspend(STD::coroutine_handle<>())),
                             void>::value) );
    test_true( STD::is_empty<STD::suspend_always>::value );
    test_pass("cxx20:suspend_always-signatures-and-empty");

    int trace = 0;
    EagerTask e = make_eager(&trace);
    test_eq( trace, 1 );
    test_true( !e.h_.done() );
    test_pass("cxx20:suspend_never-as-initial_suspend");

    e.h_.resume();
    test_eq( trace, 3 );
    test_true( e.h_.done() );
    test_pass("cxx20:suspend_never-co_await-runs-through");

    int  out = 0;
    Task at = awaiting_task(&out);
    at.handle().resume();
    test_eq( out, 5 );
    test_true( at.handle().done() );
    test_pass("cxx20:await_resume-yields-value");
}

TEST_CASE(coroutine, noop_coroutine) {

    STD::noop_coroutine_handle n = STD::noop_coroutine();
    test_true( (STD::is_same<STD::noop_coroutine_handle,
                             STD::coroutine_handle<STD::noop_coroutine_promise> >::value) );
    test_pass("cxx20:noop_coroutine");

    test_true( !!n );
    test_true( !n.done() );
    test_true( n.address() != nullptr );
    test_pass("cxx20:noop_coroutine_handle-observers");

    test_no_throw( n.resume() );
    test_no_throw( n() );
    test_no_throw( n.destroy() );
    test_true( !n.done() );
    test_pass("cxx20:noop_coroutine_handle-noop-ops");

    STD::noop_coroutine_promise& p = n.promise();
    (void)p;
    test_true( (STD::is_same<decltype(n.promise()),
                             STD::noop_coroutine_promise&>::value) );
    test_true( STD::is_empty<STD::noop_coroutine_promise>::value );
    test_pass("cxx20:noop_coroutine_promise");

    STD::noop_coroutine_handle n2 = STD::noop_coroutine();
    test_true( n2 == n );
    test_true( n2.address() == n.address() );
    test_pass("cxx20:noop_coroutine-identity");

    STD::coroutine_handle<> erased = n;
    test_true( erased.address() == n.address() );
    test_true( !erased.done() );
    test_pass("cxx20:noop_coroutine-conversion-to-void-handle");
}

TEST_CASE(coroutine, comparison_and_hash) {
    int  trace1 = 0, trace2 = 0;
    Task a = make_task(&trace1);
    Task b = make_task(&trace2);

    STD::coroutine_handle<> ha = a.handle();
    STD::coroutine_handle<> hb = b.handle();
    STD::coroutine_handle<> ha2 = a.handle();
    STD::coroutine_handle<> hnull;

    test_true(   ha == ha2 );
    test_true( !(ha == hb) );
    test_true(   ha != hb );
    test_pass("cxx20:coroutine_handle::operator==");

    STD::strong_ordering c = (ha <=> ha2);
    test_true( c == STD::strong_ordering::equal );
    STD::strong_ordering d = (ha <=> hb);
    test_true( d != STD::strong_ordering::equal );

    test_true( (ha < hb) != (hb < ha) );
    test_true( !(ha < ha2) && !(ha2 < ha) );

    test_true( hnull < ha );
    test_true( hnull <= ha );
    test_true( ha >= hnull );
    test_true( ha > hnull );
    test_pass("cxx20:coroutine_handle::operator<=>");

    STD::hash<STD::coroutine_handle<> > h;
    test_eq( h(ha), h(ha2) );
    test_true( h(ha) != h(hb) );
    test_pass("cxx20:hash<coroutine_handle<>>");

    STD::hash<STD::coroutine_handle<Task::promise_type> > hp;
    test_eq( hp(a.handle()), hp(a.handle()) );
    test_true( hp(a.handle()) != hp(b.handle()) );
    test_pass("cxx20:hash<coroutine_handle<P>>");
}
#else
TEST_CASE_SKIP(coroutine, coroutine_traits)
TEST_CASE_SKIP(coroutine, handle_default_and_null)
TEST_CASE_SKIP(coroutine, from_promise_address_conversion)
TEST_CASE_SKIP(coroutine, resume_done_destroy)
TEST_CASE_SKIP(coroutine, suspend_never_and_always)
TEST_CASE_SKIP(coroutine, noop_coroutine)
TEST_CASE_SKIP(coroutine, comparison_and_hash)
#endif
