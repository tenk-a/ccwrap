#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <latch>
#include <semaphore>
#include <barrier>
#include <type_traits>

TEST_CASE(latch, count_down) {
    STD::latch lt(2);
    test_true( !lt.try_wait() );
    lt.count_down();
    lt.count_down();
    test_true( lt.try_wait() );
    test_pass("cxx20:latch count_down");
    lt.wait();
    test_true( 1 );
    test_pass("cxx20:latch wait");
}

TEST_CASE(semaphore, counting_and_binary) {
    STD::counting_semaphore<4> sem(2);
    test_true(  sem.try_acquire() );
    test_true(  sem.try_acquire() );
    test_true( !sem.try_acquire() );
    sem.release();
    test_true(  sem.try_acquire() );
    test_pass("cxx20:counting_semaphore");

    STD::binary_semaphore bs(0);
    test_true( !bs.try_acquire() );
    bs.release();
    test_true(  bs.try_acquire() );
    test_pass("cxx20:binary_semaphore");
}

namespace {
static int g_barrier_phases = 0;
struct BarrierDone {
    void operator()() noexcept { ++g_barrier_phases; }
};
}

TEST_CASE(barrier, arrive_and_wait) {
    g_barrier_phases = 0;
    STD::barrier<BarrierDone> bar(1, BarrierDone());
    bar.arrive_and_wait();
    test_eq( g_barrier_phases, 1 );
    bar.arrive_and_wait();
    test_eq( g_barrier_phases, 2 );
    STD::barrier<> plain(1);
    test_no_throw( plain.arrive_and_wait() );
    test_pass("cxx20:barrier arrive_and_wait");
}

TEST_CASE(latch, member_operations) {

    STD::latch lt(3);
    test_true( !lt.try_wait() );
    test_pass("cxx20:latch::latch(ptrdiff_t) ctor");

    test_true( !lt.try_wait() );
    test_pass("cxx20:latch::try_wait");

    lt.count_down(2);
    test_true( !lt.try_wait() );
    lt.count_down(1);
    test_true( lt.try_wait() );
    test_pass("cxx20:latch::count_down(update)");

    STD::latch one(1);
    one.arrive_and_wait();
    test_true( one.try_wait() );
    test_pass("cxx20:latch::arrive_and_wait");

    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible copy assignment as assignable (vc bugs A26)");
    test_true( !STD::is_copy_assignable<STD::latch>::value );
    TEST_SKIP_VC120("MSVC <= 12 reports an inaccessible move assignment as assignable (vc bugs A26)");
    test_true( !STD::is_move_assignable<STD::latch>::value );
    test_pass("cxx20:latch::operator=");
}
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(latch, max_and_ctor) {
    test_true( STD::latch::max() > 0 );
    test_pass("cxx20:latch::max");

    STD::latch __l(2);
    test_true( !__l.try_wait() );
    __l.count_down();
    test_true( !__l.try_wait() );
    __l.count_down();
    test_true( __l.try_wait() );
    test_pass("cxx20:latch ctor");
}
#else
TEST_CASE(latch, max_and_ctor) {
    TEST_NOTE("this is a C++20 header and the build is below C++20");
    test_skip("cxx20:latch::max");
    test_skip("cxx20:latch ctor");
}
#endif

TEST_CASE(latch, feature_test_macros) {
#if defined(__cpp_lib_latch)
    test_true( __cpp_lib_latch > 0L );
    test_pass("cxx20:__cpp_lib_latch");
#else
    test_skip("cxx20:__cpp_lib_latch");
#endif
}


#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
TEST_CASE(latch, lifetime_cxx20) {
    TEST_SKIP_WAT("C++03 deduction does not check access, so a private copy ctor is not seen");
    test_true( !STD::is_copy_constructible<STD::latch>::value );
    test_pass("cxx20:latch::latch(const latch&) = delete");

    {
        STD::latch done(1);
        done.count_down();
        test_true( done.try_wait() );
    }
    {
        STD::latch unfinished(2);
        unfinished.count_down();
        test_true( !unfinished.try_wait() );
    }
    test_true( true );
    test_pass("cxx20:latch::~latch");
}
#else
TEST_CASE(latch, lifetime_cxx20) {
    test_skip("cxx20:latch::latch(const latch&) = delete");
    test_skip("cxx20:latch::~latch");
}
#endif
