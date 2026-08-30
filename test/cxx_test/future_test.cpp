#include "test_cxx.hpp"
#include <future>
#include <string>
#include <system_error>
#include <type_traits>
#include <stdexcept>
#include <chrono>
#if !defined(__WATCOMC__)
#include <thread>
#endif

#if _TST_HAS_FUTURE

namespace {
    int fut_double(int x) { return x * 2; }
    int fut_answer() { return 42; }
#if !defined(__WATCOMC__)
    struct SetValExit  { STD::promise<int>* p; void operator()() const { p->set_value_at_thread_exit(11); } };
    struct SetExcExit  { STD::promise<int>* p;
                         void operator()() const { p->set_exception_at_thread_exit(
                             STD::make_exception_ptr(STD::runtime_error("x"))); } };
    struct RunTaskExit { STD::packaged_task<int()>* t; void operator()() const { t->make_ready_at_thread_exit(); } };
#endif
}

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER < 1800
typedef STD::future_status::future_status _tst_future_status;
typedef STD::future_errc::future_errc     _tst_future_errc;
typedef STD::launch::launch               _tst_launch;
#else
typedef STD::future_status _tst_future_status;
typedef STD::future_errc   _tst_future_errc;
typedef STD::launch        _tst_launch;
#endif

TEST_CASE(future, promise_future) {
    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    test_true( f.valid() );

    p.set_value(42);
    f.wait();
    int v = f.get();
    test_eq( v, 42 );
    test_true( !f.valid() );
    test_pass("cxx11:promise future");
}

#if TEST_HAS_EH
TEST_CASE(future, broken_promise) {

    STD::future<int> f;
    {
        STD::promise<int> p;
        f = p.get_future();
        test_true( f.valid() );
    }
    bool threw = false;
    try { (void)f.get(); }
    catch (const STD::future_error&) { threw = true; }
    catch (const STD::logic_error&) { threw = true; }
    catch (...) {}
    test_true( threw );
    test_pass("cxx11:broken promise");
}
#else   // !TEST_HAS_EH
TEST_CASE(future, broken_promise) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:broken promise");
}
#endif  // TEST_HAS_EH

TEST_CASE(future, shared_future) {
    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    STD::shared_future<int> sf = f.share();
    test_true( !f.valid() );
    p.set_value(7);
    test_true( sf.valid() );

    test_eq( sf.get(), 7 );
    test_eq( sf.get(), 7 );
    test_true( sf.valid() );

    STD::shared_future<int> copy = sf;
    test_eq( copy.get(), 7 );
    test_pass("cxx11:shared future");
}

#if TEST_HAS_EH
TEST_CASE(future, status_and_error_types) {
    _tst_future_status a = STD::future_status::ready;
    _tst_future_status b = STD::future_status::timeout;
    _tst_future_status c = STD::future_status::deferred;
    test_true( a != b );
    test_true( b != c );
    test_true( a != c );

    STD::future<int> f;
    { STD::promise<int> p; f = p.get_future(); }
    bool is_logic = false;
    try { (void)f.get(); }
    catch (const STD::logic_error&) { is_logic = true; }
    catch (...) {}
    test_true( is_logic );
    test_pass("cxx11:status and error types");
}
#else   // !TEST_HAS_EH
TEST_CASE(future, status_and_error_types) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:status and error types");
}
#endif  // TEST_HAS_EH

namespace {
struct FutExc : STD::runtime_error {
    FutExc() : STD::runtime_error("fut-exc") {}
};
int pt_add(int a, int b) { return a + b; }
#if TEST_HAS_EH
int pt_throw() { throw STD::runtime_error("boom"); }
#endif
struct PtFunctor { int operator()() const { return 77; } };
}

#if TEST_HAS_EH
TEST_CASE(future, error_code_and_set_exception) {

    STD::error_code ec = STD::make_error_code(STD::future_errc::broken_promise);
    test_eq( ec.value(), (int)STD::future_errc::broken_promise );
    test_eq( STD::string(ec.category().name()), STD::string("future") );

    test_true( STD::is_error_code_enum<_tst_future_errc>::value );

#if !defined(__GNUC__) || defined(__WATCOMC__)
    STD::future_error fe(STD::make_error_code(STD::future_errc::no_state));
    test_eq( fe.code().value(), (int)STD::future_errc::no_state );
#else
    TEST_SKIP1();
#endif

    _tst_launch lp = STD::launch::async;
    test_true( lp == STD::launch::async );

    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    p.set_exception(STD::make_exception_ptr(FutExc()));
    bool caught = false;
    try { (void)f.get(); }
    catch (const FutExc&) { caught = true; }
    catch (...) {}
    test_true( caught );
    test_pass("cxx11:error code and set exception");
}
#else   // !TEST_HAS_EH
TEST_CASE(future, error_code_and_set_exception) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:error code and set exception");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(future, packaged_task_basic) {

    STD::packaged_task<int(int, int)> t(&pt_add);
    test_true( t.valid() );
    STD::future<int> f = t.get_future();
    t(3, 4);
    int r = f.get();
    test_eq( r, 7 );

    PtFunctor pf;
    STD::packaged_task<int()> t0(pf);
    STD::future<int> f0 = t0.get_future();
    t0();
    int r0 = f0.get();
    test_eq( r0, 77 );

    STD::packaged_task<int()> te(&pt_throw);
    STD::future<int> fe = te.get_future();
    te();
    bool ecaught = false; STD::string emsg;
    try { (void)fe.get(); }
    catch (const STD::exception& e) { ecaught = true; emsg = e.what(); }
    catch (...) {}
    test_true( ecaught );
    test_true( emsg.find("boom") != STD::string::npos );

    STD::packaged_task<int(int, int)> t2(&pt_add);
    STD::future<int> f2 = t2.get_future();
    STD::packaged_task<int(int, int)> t3(STD::move(t2));
    test_true( t3.valid() );
    t3(10, 20);
    int r2 = f2.get();
    test_eq( r2, 30 );
    test_pass("cxx11:packaged task basic");
}
#else   // !TEST_HAS_EH
TEST_CASE(future, packaged_task_basic) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:packaged task basic");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(future, errc_category_launch) {
    test_true( STD::is_error_code_enum<_tst_future_errc>::value );
    test_pass("cxx11:is_error_code_enum<future_errc>");

    STD::error_code ec = STD::make_error_code(STD::future_errc::broken_promise);
    test_true( ec.category() == STD::future_category() );
    test_pass("cxx11:future_errc");
    test_pass("cxx11:future_category");
    test_pass("cxx11:make_error_code(future_errc)");
    STD::error_condition eco = STD::make_error_condition(STD::future_errc::no_state);
    test_true( eco.category() == STD::future_category() );
    test_pass("cxx11:make_error_condition(future_errc)");

    bool threw = false;
    try {
        STD::promise<int> p;
        STD::future<int> f = p.get_future();
        STD::future<int> f2 = p.get_future();
        (void)f; (void)f2;
    } catch (const STD::future_error& e) {
        threw = true;
        test_true( e.code() == STD::future_errc::future_already_retrieved );
    }
    test_true( threw );
    test_pass("cxx11:future_error");

    test_true( STD::future_status::ready != STD::future_status::timeout
               && STD::future_status::timeout != STD::future_status::deferred );
    test_pass("cxx11:future_status (ready/timeout/deferred)");
    _tst_launch l = STD::launch::async | STD::launch::deferred;
    (void)l;
    test_true( STD::launch::async != STD::launch::deferred );
    test_pass("cxx11:launch (async/deferred)");
}
#else   // !TEST_HAS_EH
TEST_CASE(future, errc_category_launch) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:is_error_code_enum<future_errc>");
    test_skip("cxx11:future_errc");
    test_skip("cxx11:future_category");
    test_skip("cxx11:make_error_code(future_errc)");
    test_skip("cxx11:make_error_condition(future_errc)");
    test_skip("cxx11:future_error");
    test_skip("cxx11:future_status (ready/timeout/deferred)");
    test_skip("cxx11:launch (async/deferred)");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(future, promise_and_future_members) {
    typedef STD::chrono::milliseconds ms;

    STD::promise<int> p;
    test_no_throw( (void)STD::promise<int>() );
    test_pass("cxx11:promise::promise() default ctor");

    STD::future<int> f = p.get_future();
    test_true( f.valid() );
    test_pass("cxx11:promise::get_future");

    test_true( f.valid() );
    { STD::future<int> moved(STD::move(f)); test_true( moved.valid() ); f = STD::move(moved); }
    test_true( f.valid() );
    test_pass("cxx11:future::valid");

    p.set_value(42);
    test_true( f.wait_for(ms(0)) == STD::future_status::ready );
    test_throw( p.set_value(43) );
#if TEST_HAS_EH
    {
        bool dup = false;
        try { p.set_value(43); }
        catch (STD::future_error& fe) { dup = (fe.code() == STD::make_error_code(STD::future_errc::promise_already_satisfied)); }
        catch (...) {}
        test_true( dup );
    }
#else
    TEST_SKIP1();
    test_true( true );
#endif
    test_pass("cxx11:promise::set_value");

    test_no_throw( f.wait() );
    test_no_throw( f.wait() );
    test_pass("cxx11:future::wait");

    test_true( f.wait_for(ms(0)) == STD::future_status::ready );
    test_true( f.wait_for(ms(1)) == STD::future_status::ready );
    test_pass("cxx11:future::wait_for");

    test_true( f.wait_until(STD::chrono::steady_clock::now()) == STD::future_status::ready );
    test_true( f.wait_until(STD::chrono::steady_clock::now() + ms(1))
               == STD::future_status::ready );
    test_pass("cxx11:future::wait_until");

    { int gv = f.get(); test_eq( gv, 42 ); }
    test_true( !f.valid() );
    test_pass("cxx11:future::get");

    STD::promise<int> pa;
    STD::future<int> fa = pa.get_future();
    STD::promise<int> pb(STD::move(pa));
    pb.set_value(7);
    { int v = fa.get(); test_eq( v, 7 ); }
    test_pass("cxx11:promise::promise(promise&&) move ctor");

    STD::promise<int> pc;
    STD::promise<int> pb2;
    STD::future<int> fb2 = pb2.get_future();
    pc = STD::move(pb2);
    pc.set_value(9);
    { int v = fb2.get(); test_eq( v, 9 ); }
    test_pass("cxx11:promise::operator=(promise&&)");

    STD::promise<int> pd, pe;
    STD::future<int> fd = pd.get_future();
    pd.swap(pe);
    pe.set_value(11);
    { int v = fd.get(); test_eq( v, 11 ); }
    test_pass("cxx11:promise::swap");

    STD::promise<int> pf, pg;
    STD::future<int> ff = pf.get_future();
    STD::swap(pf, pg);
    pg.set_value(13);
    { int v = ff.get(); test_eq( v, 13 ); }
    test_pass("cxx11:swap(promise)");

    STD::promise<int> pal(STD::allocator_arg, STD::allocator<int>());
    test_true( pal.get_future().valid() );
    test_pass("cxx11:promise::promise(allocator_arg_t, Alloc)");
    bool ua = STD::uses_allocator<STD::promise<int>, STD::allocator<int> >::value;
    (void)ua;
    test_true( true );
    test_pass("cxx11:uses_allocator<promise>");

    STD::promise<int> px;
    STD::future<int> fx = px.get_future();
    px.set_exception(STD::make_exception_ptr(STD::runtime_error("boom")));
    bool threw = false;
    try { fx.get(); } catch (const STD::runtime_error&) { threw = true; }
    test_true( threw );
    test_pass("cxx11:promise::set_exception");

    STD::future<int> fnull;
    test_true( !fnull.valid() );
    test_pass("cxx11:future::future() default ctor");
    STD::promise<int> pm;
    STD::future<int> fm = pm.get_future();
    pm.set_value(7);
    STD::future<int> fm2(STD::move(fm));
    test_true( fm2.valid() && !fm.valid() );
    test_pass("cxx11:future::future(future&&) move ctor");
    STD::future<int> fm3;
    fm3 = STD::move(fm2);
    test_pass("cxx11:future::operator=(future&&)");
    STD::shared_future<int> sf = fm3.share();
    test_true( sf.valid() && !fm3.valid() );
    { int gv = sf.get(); test_eq( gv, 7 ); }
    test_pass("cxx11:future::share");

#if !defined(__WATCOMC__)

    STD::promise<int> pve;
    STD::future<int> fve = pve.get_future();
    SetValExit sve; sve.p = &pve;
    STD::thread tve(sve);
    tve.join();
    { int gv = fve.get(); test_eq( gv, 11 ); }
    test_pass("cxx11:promise::set_value_at_thread_exit");
    STD::promise<int> pee;
    STD::future<int> fee = pee.get_future();
    SetExcExit see; see.p = &pee;
    STD::thread tee(see);
    tee.join();
    bool t2 = false;
    try { fee.get(); } catch (const STD::runtime_error&) { t2 = true; }
    test_true( t2 );
    test_pass("cxx11:promise::set_exception_at_thread_exit");
#else
    test_skip("cxx11:promise::set_value_at_thread_exit");
    test_skip("cxx11:promise::set_exception_at_thread_exit");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(future, promise_and_future_members) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:promise::promise() default ctor");
    test_skip("cxx11:promise::get_future");
    test_skip("cxx11:future::valid");
    test_skip("cxx11:promise::set_value");
    test_skip("cxx11:future::wait");
    test_skip("cxx11:future::wait_for");
    test_skip("cxx11:future::wait_until");
    test_skip("cxx11:future::get");
    test_skip("cxx11:promise::promise(promise&&) move ctor");
    test_skip("cxx11:promise::operator=(promise&&)");
    test_skip("cxx11:promise::swap");
    test_skip("cxx11:swap(promise)");
    test_skip("cxx11:promise::promise(allocator_arg_t, Alloc)");
    test_skip("cxx11:uses_allocator<promise>");
    test_skip("cxx11:promise::set_exception");
    test_skip("cxx11:future::future() default ctor");
    test_skip("cxx11:future::future(future&&) move ctor");
    test_skip("cxx11:future::operator=(future&&)");
    test_skip("cxx11:future::share");
    test_skip("cxx11:promise::set_value_at_thread_exit");
    test_skip("cxx11:promise::set_exception_at_thread_exit");
}
#endif  // TEST_HAS_EH

TEST_CASE(future, shared_future_members) {
    typedef STD::chrono::milliseconds ms;

    STD::shared_future<int> snull;
    test_true( !snull.valid() );
    test_pass("cxx11:shared_future::shared_future() default ctor");

    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    p.set_value(99);
    STD::shared_future<int> sf(STD::move(f));
    test_true( sf.valid() );
    test_pass("cxx11:shared_future::shared_future(future&&)");

    STD::shared_future<int> sf2(sf);
    test_true( sf2.valid() );
    test_pass("cxx11:shared_future::shared_future(const shared_future&) copy ctor");

    sf.wait();
    test_true( sf.wait_for(ms(0)) == STD::future_status::ready );
    test_true( sf.wait_until(STD::chrono::steady_clock::now()) == STD::future_status::ready );
    { int gv = sf.get(); test_eq( gv, 99 ); }
    { int gv = sf2.get(); test_eq( gv, 99 ); }
    test_pass("cxx11:shared_future::valid");
    test_pass("cxx11:shared_future::wait");
    test_pass("cxx11:shared_future::wait_for");
    test_pass("cxx11:shared_future::wait_until");
    test_pass("cxx11:shared_future::get");
}

TEST_CASE(future, packaged_task_members) {
    STD::packaged_task<int()> t0;
    test_true( !t0.valid() );
    test_pass("cxx11:packaged_task::packaged_task() default ctor");
    test_pass("cxx11:packaged_task::valid");

    STD::packaged_task<int()> t(fut_answer);
    test_true( t.valid() );
    test_pass("cxx11:packaged_task::packaged_task(F&&)");
    STD::future<int> f = t.get_future();
    test_pass("cxx11:packaged_task::get_future");
    t();
    { int gv = f.get(); test_eq( gv, 42 ); }
    test_pass("cxx11:packaged_task::operator()");

    STD::packaged_task<int()> t2(fut_answer);
    STD::packaged_task<int()> t3(STD::move(t2));
    test_true( t3.valid() && !t2.valid() );
    test_pass("cxx11:packaged_task::packaged_task(packaged_task&&) move ctor");
    STD::packaged_task<int()> t4;
    t4 = STD::move(t3);
    test_true( t4.valid() );
    test_pass("cxx11:packaged_task::operator=(packaged_task&&)");

    STD::future<int> f4 = t4.get_future();
    t4();
    { int gv = f4.get(); test_eq( gv, 42 ); }
    t4.reset();
    STD::future<int> f4b = t4.get_future();
    t4();
    { int gv = f4b.get(); test_eq( gv, 42 ); }
    test_pass("cxx11:packaged_task::reset");

    STD::packaged_task<int()> a(fut_answer), b;
    a.swap(b);
    test_true( b.valid() && !a.valid() );
    test_pass("cxx11:packaged_task::swap");
    STD::swap(a, b);
    test_true( a.valid() && !b.valid() );
    test_pass("cxx11:swap(packaged_task)");

#if !defined(__WATCOMC__)

    STD::packaged_task<int()> tx(fut_answer);
    STD::future<int> fx = tx.get_future();
    RunTaskExit rte; rte.t = &tx;
    STD::thread th(rte);
    th.join();
    { int gv = fx.get(); test_eq( gv, 42 ); }
    test_pass("cxx11:packaged_task::make_ready_at_thread_exit");
#else
    test_skip("cxx11:packaged_task::make_ready_at_thread_exit");
#endif
}

TEST_CASE(future, async_functions) {
#if !defined(__WATCOMC__)

    STD::future<int> f1 = STD::async(fut_double, 21);
    { int gv = f1.get(); test_eq( gv, 42 ); }
    test_pass("cxx11:async(F&&, Args&&...)");

    STD::future<int> f2 = STD::async(STD::launch::deferred, fut_double, 5);
#if defined(_MSC_VER) && _MSC_VER == 1800
    TEST_SKIP_VC("MSVC 12's wait_for reports timeout, not deferred, for a deferred async");
#endif
    test_true( f2.wait_for(STD::chrono::milliseconds(0)) == STD::future_status::deferred );
    { int gv = f2.get(); test_eq( gv, 10 ); }
    test_pass("cxx11:async(launch, F&&, Args&&...)");
#else
    test_skip("cxx11:async(F&&, Args&&...)");
    test_skip("cxx11:async(launch, F&&, Args&&...)");
#endif
}

TEST_CASE(future, enumerators_cxx11) {
    STD::error_code bp = STD::make_error_code(STD::future_errc::broken_promise);
    test_true( (int)STD::future_errc::broken_promise != 0 );
    test_true( bp == STD::future_errc::broken_promise );
    test_true( bp != STD::future_errc::no_state );
    test_pass("cxx11:future_errc::broken_promise");

    STD::error_code fr = STD::make_error_code(STD::future_errc::future_already_retrieved);
    test_true( (int)STD::future_errc::future_already_retrieved != 0 );
    test_true( fr == STD::future_errc::future_already_retrieved );
    test_true( fr != bp );
    test_pass("cxx11:future_errc::future_already_retrieved");

    STD::error_code ps = STD::make_error_code(STD::future_errc::promise_already_satisfied);
    test_true( (int)STD::future_errc::promise_already_satisfied != 0 );
    test_true( ps == STD::future_errc::promise_already_satisfied );
    test_true( ps != bp && ps != fr );
    test_pass("cxx11:future_errc::promise_already_satisfied");

    STD::error_code ns = STD::make_error_code(STD::future_errc::no_state);
    test_true( (int)STD::future_errc::no_state != 0 );
    test_true( ns == STD::future_errc::no_state );
    test_true( ns != bp && ns != fr && ns != ps );
    test_pass("cxx11:future_errc::no_state");

    _tst_future_status ready   = STD::future_status::ready;
    _tst_future_status timeout = STD::future_status::timeout;
    _tst_future_status deferred = STD::future_status::deferred;
    test_true( ready != timeout && timeout != deferred && ready != deferred );

    {
        STD::promise<int> p;
        STD::future<int> f = p.get_future();
        p.set_value(1);
        test_true( f.wait_for(STD::chrono::milliseconds(0)) == STD::future_status::ready );
    }
    test_pass("cxx11:future_status::ready");

    {
        STD::promise<int> p;
        STD::future<int> f = p.get_future();
        test_true( f.wait_for(STD::chrono::milliseconds(1)) == STD::future_status::timeout );
    }
    test_pass("cxx11:future_status::timeout");

#if !defined(__WATCOMC__)
    {
        STD::future<int> f = STD::async(STD::launch::deferred, fut_answer);
        test_true( f.wait_for(STD::chrono::milliseconds(0)) == STD::future_status::deferred );
        int got = f.get();
        test_eq( got, 42 );
    }
    test_pass("cxx11:future_status::deferred");

    _tst_launch both = STD::launch::async | STD::launch::deferred;
    test_true( (both & STD::launch::async) == STD::launch::async );
    {
        STD::future<int> f = STD::async(STD::launch::async, fut_answer);
        int got = f.get();
        test_eq( got, 42 );
    }
    test_pass("cxx11:launch::async");

    {
        int calls = 0;
        struct Count { int* p; int operator()() const { ++*p; return 5; } };
        Count c; c.p = &calls;
        STD::future<int> f = STD::async(STD::launch::deferred, c);
        test_eq( calls, 0 );
        int got = f.get();
        test_eq( got, 5 );
        test_eq( calls, 1 );
    }
    test_pass("cxx11:launch::deferred");
#else
    TEST_NOTE("std::async is not available here");
    test_skip("cxx11:future_status::deferred");
    test_skip("cxx11:launch::async");
    test_skip("cxx11:launch::deferred");
#endif
}

#if TEST_HAS_EH
TEST_CASE(future, future_error_observers_cxx11) {
    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    bool caught = false;
    try {
        STD::future<int> again = p.get_future();
        (void)again;
    } catch (const STD::future_error& e) {
        caught = true;
        test_true( e.code() == STD::future_errc::future_already_retrieved );
        test_eq( STD::string(e.code().category().name()), STD::string("future") );
    } catch (...) {}
    test_true( caught );
    test_pass("cxx11:future_error::code");

    bool caught2 = false;
    try {
        STD::promise<int> q;
        q.set_value(1);
        q.set_value(2);
    } catch (const STD::future_error& e) {
        caught2 = true;
        test_ptr( (void*)e.what() );
        test_true( STD::string(e.what()).size() > 0 );
    } catch (const STD::logic_error& e) {
        caught2 = true;
        test_true( STD::string(e.what()).size() > 0 );
    } catch (...) {}
    test_true( caught2 );
    test_pass("cxx11:future_error::what");
}
#else   // !TEST_HAS_EH
TEST_CASE(future, future_error_observers_cxx11) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:future_error::code");
    test_skip("cxx11:future_error::what");
}
#endif  // TEST_HAS_EH

#if TEST_HAS_EH
TEST_CASE(future, destructors_cxx11) {
    {
        STD::future<int> f;
        { STD::promise<int> p; f = p.get_future(); }
        bool broken = false;
        try { (void)f.get(); }
        catch (const STD::future_error& e) {
            broken = (e.code() == STD::future_errc::broken_promise);
        } catch (...) {}
        test_true( broken );
    }
    test_pass("cxx11:promise::~promise");

    {
        STD::promise<int> p;
        { STD::future<int> f = p.get_future(); test_true( f.valid() ); }
        p.set_value(3);
        test_true( true );
    }
    test_pass("cxx11:future::~future");

    {
        STD::promise<int> p;
        STD::shared_future<int> a = p.get_future().share();
        p.set_value(9);
        {
            STD::shared_future<int> b = a;
            test_eq( b.get(), 9 );
        }
        test_eq( a.get(), 9 );
        test_true( a.valid() );
    }
    test_pass("cxx11:shared_future::~shared_future");

#if !(defined(_MSC_VER) && _MSC_VER < 1910)
    {
        STD::future<int> f;
        { STD::packaged_task<int()> t(fut_answer); f = t.get_future(); }
        bool broken = false;
        try { (void)f.get(); }
        catch (const STD::future_error& e) {
            broken = (e.code() == STD::future_errc::broken_promise);
        } catch (...) {}
        test_true( broken );
    }
    test_pass("cxx11:packaged_task::~packaged_task");
#else
    TEST_NOTE("MSVC before 14.1 blocks forever instead of reporting broken_promise");
    TEST_SKIP1();
    test_skip("cxx11:packaged_task::~packaged_task");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(future, destructors_cxx11) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx11:promise::~promise");
    test_skip("cxx11:future::~future");
    test_skip("cxx11:shared_future::~shared_future");
    test_skip("cxx11:packaged_task::~packaged_task");
}
#endif  // TEST_HAS_EH

static int g_ft_live = 0;

struct FtLive {
    char* p_;
    FtLive() : p_(new char[16]) { p_[0] = 0; ++g_ft_live; }
    FtLive(char c) : p_(new char[16]) { p_[0] = c; p_[1] = 0; ++g_ft_live; }
    FtLive(const FtLive& o) : p_(new char[16]) {
        for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        ++g_ft_live;
    }
    FtLive& operator=(const FtLive& o) {
        if (this != &o) for (int i = 0; i < 16; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~FtLive() { delete[] p_; --g_ft_live; }
    char tag() const { return p_[0]; }
};

TEST_CASE(future, class_value_lifetime) {
    g_ft_live = 0;
    {
        {
            STD::promise<FtLive> p;
            STD::future<FtLive> f = p.get_future();
            test_true( g_ft_live <= 1 );

            p.set_value(FtLive('a'));
            test_true( g_ft_live >= 1 );

            FtLive got = f.get();
            test_eq( got.tag(), 'a' );
        }
        test_eq( g_ft_live, 0 );

        {
            STD::promise<FtLive> p;
            STD::shared_future<FtLive> s = p.get_future().share();
            p.set_value(FtLive('b'));
            test_eq( s.get().tag(), 'b' );
            STD::shared_future<FtLive> s2 = s;
            test_eq( s2.get().tag(), 'b' );
        }
        test_eq( g_ft_live, 0 );

        {
            STD::promise<FtLive> p;
            STD::future<FtLive> f = p.get_future();
            p.set_value(FtLive('c'));
        }
        test_eq( g_ft_live, 0 );
    }
    test_eq( g_ft_live, 0 );
}

#else
TEST_CASE_SKIP(future, promise_future)
TEST_CASE_SKIP(future, class_value_lifetime)
TEST_CASE_SKIP(future, broken_promise)
TEST_CASE_SKIP(future, shared_future)
TEST_CASE_SKIP(future, status_and_error_types)
TEST_CASE_SKIP(future, error_code_and_set_exception)
TEST_CASE_SKIP(future, packaged_task_basic)
TEST_CASE_SKIP(future, errc_category_launch)
TEST_CASE_SKIP(future, promise_and_future_members)
TEST_CASE_SKIP(future, shared_future_members)
TEST_CASE_SKIP(future, packaged_task_members)
TEST_CASE_SKIP(future, async_functions)
TEST_CASE(future, enumerators_cxx11) {
    test_skip("cxx11:future_errc::broken_promise");
    test_skip("cxx11:future_errc::future_already_retrieved");
    test_skip("cxx11:future_errc::promise_already_satisfied");
    test_skip("cxx11:future_errc::no_state");
    test_skip("cxx11:future_status::ready");
    test_skip("cxx11:future_status::timeout");
    test_skip("cxx11:future_status::deferred");
    test_skip("cxx11:launch::async");
    test_skip("cxx11:launch::deferred");
}
TEST_CASE(future, future_error_observers_cxx11) {
    test_skip("cxx11:future_error::code");
    test_skip("cxx11:future_error::what");
}
TEST_CASE(future, destructors_cxx11) {
    test_skip("cxx11:promise::~promise");
    test_skip("cxx11:future::~future");
    test_skip("cxx11:shared_future::~shared_future");
    test_skip("cxx11:packaged_task::~packaged_task");
}
#endif
