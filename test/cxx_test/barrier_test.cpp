#include "test_cxx.hpp"
#if TEST_TARGET_CXX >= 2020
#include <barrier>
#endif

TEST_CASE(barrier, header_compiles) {
#if TEST_TARGET_CXX >= 2020
    test_true( true );
    test_pass("cxx20:barrier(header)");
#else
    TEST_NOTE("<barrier> is C++20 and the build is below C++20");
    test_skip("cxx20:barrier(header)");
#endif
}

#if TEST_TARGET_CXX >= 2020
#include <barrier>
#include <thread>
#include <atomic>
#include <vector>
#include <utility>
#include <cstddef>
#include <type_traits>

#if _tst_cplusplus >= 201103L
#  define _NOEXCEPT_TEST noexcept
#else
#  define _NOEXCEPT_TEST
#endif

namespace {

STD::atomic<int> g_phases(0);

struct Completion {
    void operator()() _NOEXCEPT_TEST { g_phases.fetch_add(1); }
};

struct PhaseWorker {
    STD::barrier<Completion>* b;
    STD::atomic<int>*         work;
    PhaseWorker(STD::barrier<Completion>* bb, STD::atomic<int>* w) : b(bb), work(w) {}
    void operator()() const {
        for (int k = 0; k < 4; ++k) { work->fetch_add(1); b->arrive_and_wait(); }
    }
};

struct TokenWorker {
    STD::barrier<>*   b;
    STD::atomic<int>* hits;
    TokenWorker(STD::barrier<>* bb, STD::atomic<int>* h) : b(bb), hits(h) {}
    void operator()() const {
        hits->fetch_add(1); b->arrive_and_wait();
        hits->fetch_add(1); b->arrive_and_wait();
    }
};

struct Dropper {
    STD::barrier<Completion>* b;
    explicit Dropper(STD::barrier<Completion>* bb) : b(bb) {}
    void operator()() const { b->arrive_and_drop(); }
};

struct LamCompletion {
    STD::atomic<int>* lam;
    LamCompletion() : lam(0) {}
    explicit LamCompletion(STD::atomic<int>* l) : lam(l) {}
    void operator()() _NOEXCEPT_TEST { lam->fetch_add(1); }
};

}

TEST_CASE(barrier, arrive_and_wait_threads) {

    g_phases.store(0);
    STD::atomic<int> work(0);

    STD::barrier<Completion> b(3, Completion());

    STD::thread t0((PhaseWorker(&b, &work)));
    STD::thread t1((PhaseWorker(&b, &work)));
    STD::thread t2((PhaseWorker(&b, &work)));
    t0.join();
    t1.join();
    t2.join();

    int phases = g_phases.load();
    int done   = work.load();
    test_eq( phases, 4 );
    test_pass("cxx20:barrier::arrive_and_wait");
    test_eq( done, 12 );
    test_pass("cxx20:barrier::completion_function");
}

TEST_CASE(barrier, arrive_and_wait_token) {

    g_phases.store(0);
    STD::barrier<Completion> b(1, Completion());

    STD::barrier<Completion>::arrival_token tok = b.arrive();
    b.wait(STD::move(tok));
    int n1 = g_phases.load();
    test_eq( n1, 1 );
    test_pass("cxx20:barrier::arrive");

    STD::barrier<Completion>::arrival_token tok2 = b.arrive();
    b.wait(STD::move(tok2));
    int n2 = g_phases.load();
    test_eq( n2, 2 );
    test_pass("cxx20:barrier::wait");

    STD::barrier<Completion> b2(3, Completion());
    STD::barrier<Completion>::arrival_token t3 = b2.arrive(3);
    b2.wait(STD::move(t3));
    int n3 = g_phases.load();
    test_eq( n3, 3 );
    test_pass("cxx20:barrier::arrive(n)");

    STD::barrier<Completion> b3(3, Completion());
    STD::barrier<Completion>::arrival_token ta = b3.arrive(2);
    int mid = g_phases.load();
    test_eq( mid, 3 );
    STD::barrier<Completion>::arrival_token tb = b3.arrive(1);
    b3.wait(STD::move(ta));
    b3.wait(STD::move(tb));
    int n4 = g_phases.load();
    test_eq( n4, 4 );
    test_pass("cxx20:barrier::arrival_token");
}

TEST_CASE(barrier, arrive_and_drop) {

    g_phases.store(0);
    STD::barrier<Completion> b(2, Completion());

    STD::thread helper((Dropper(&b)));

    b.arrive_and_wait();
    helper.join();
    int after1 = g_phases.load();
    test_eq( after1, 1 );
    test_pass("cxx20:barrier::arrive_and_drop");

    b.arrive_and_wait();
    int after2 = g_phases.load();
    test_eq( after2, 2 );

    b.arrive_and_wait();
    int after3 = g_phases.load();
    test_eq( after3, 3 );
    test_pass("cxx20:barrier::arrive_and_drop(persists)");
}

TEST_CASE(barrier, default_completion_and_max) {

    STD::atomic<int> hits(0);
    {
        STD::barrier<> b(2);
        STD::thread t((TokenWorker(&b, &hits)));
        b.arrive_and_wait();
        b.arrive_and_wait();
        t.join();
    }
    int n = hits.load();
    test_eq( n, 2 );
    test_pass("cxx20:barrier(default_completion)");

    STD::atomic<int> lam(0);
    {
        LamCompletion fn(&lam);
        STD::barrier<LamCompletion> b(1, fn);
        b.arrive_and_wait();
        b.arrive_and_wait();
    }
    int ln = lam.load();
    test_eq( ln, 2 );
    test_pass("cxx20:barrier(lambda_completion)");

    STD::ptrdiff_t m0 = (STD::barrier<>::max)();
    STD::ptrdiff_t m1 = (STD::barrier<Completion>::max)();
    test_gt( m0, STD::ptrdiff_t(0) );
    test_ge( m0, STD::ptrdiff_t(4) );
    test_gt( m1, STD::ptrdiff_t(0) );
    test_pass("cxx20:barrier::max");

    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::barrier<> >::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible move assignment as assignable (vc bugs A26)");
    test_true( !STD::is_move_assignable<STD::barrier<> >::value );
    test_pass("cxx20:barrier::operator=");
}
#else
TEST_CASE_SKIP(barrier, arrive_and_wait_threads)
TEST_CASE_SKIP(barrier, arrive_and_wait_token)
TEST_CASE_SKIP(barrier, arrive_and_drop)
TEST_CASE_SKIP(barrier, default_completion_and_max)
#endif


#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
TEST_CASE(barrier, lifetime_and_phases_cxx20) {
    TEST_SKIP_WAT("C++03 deduction does not check access, so a private copy ctor is not seen");
    test_true( !STD::is_copy_constructible<STD::barrier<> >::value );
    test_pass("cxx20:barrier copy constructor is deleted");

    {
        STD::atomic<int> hits(0);
        {
            STD::barrier<> b(1);
            b.arrive_and_wait();
            test_eq( hits.load(), 0 );
        }
        test_true( true );
    }
    test_pass("cxx20:barrier::~barrier");

    {
        STD::barrier<> z(0);
        test_true( (STD::barrier<>::max)() >= 0 );
        (void)z;
    }
    test_pass("cxx20:barrier zero expected count");

    {
        STD::atomic<int> phases(0);
        LamCompletion fn(&phases);
        STD::barrier<LamCompletion> b(1, fn);
        b.arrive_and_wait();
        b.arrive_and_wait();
        b.arrive_and_wait();
        test_eq( phases.load(), 3 );
    }
    test_pass("cxx20:barrier repeated phases");
}
#else
TEST_CASE(barrier, lifetime_and_phases_cxx20) {
    test_skip("cxx20:barrier copy constructor is deleted");
    test_skip("cxx20:barrier::~barrier");
    test_skip("cxx20:barrier zero expected count");
    test_skip("cxx20:barrier repeated phases");
}
#endif
