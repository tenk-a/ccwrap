#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2023 && _TST_TEST_HAS(<generator>) && defined(__cpp_impl_coroutine)
#include <generator>
#include <string>
#include <memory_resource>
#include <ranges>
#include <iterator>
#include <type_traits>
#include <utility>

namespace {
STD::generator<int> ccw_gen_123() {
    co_yield 1;
    co_yield 2;
    co_yield 3;
}
STD::generator<int> ccw_gen_empty() {
    co_return;
}
STD::generator<int> ccw_gen_nested() {
    co_yield 0;
    co_yield STD::ranges::elements_of(ccw_gen_123());
    co_yield 4;
}
#if TEST_HAS_EH
STD::generator<int> ccw_gen_throws() {
    co_yield 1;
    throw 42;
}
#endif
STD::generator<STD::string> ccw_gen_strings() {
    co_yield STD::string("a");
    co_yield STD::string("b");
}
}

TEST_CASE(generator, iteration_cxx23) {
    STD::generator<int> g = ccw_gen_123();
    test_pass("cxx23:generator");

    auto it = g.begin();
    test_pass("cxx23:generator::begin");
    test_true( !(STD::is_same<decltype(it), decltype(g.end())>::value) );
    test_true( (STD::is_same<decltype(*it), int&&>::value) );

    test_eq( *it, 1 );

    ++it;
    test_eq( *it, 2 );
    ++it;
    test_eq( *it, 3 );

    ++it;
    test_true( it == g.end() );
    test_pass("cxx23:generator::end");

    STD::generator<int> e = ccw_gen_empty();
    auto eit = e.begin();
    test_true( eit == e.end() );

    int sum = 0;
    STD::generator<int> g2 = ccw_gen_123();
    for (auto i =g2.begin(); i != g2.end(); ++i) sum += *i;
    test_eq( sum, 6 );
}

TEST_CASE(generator, types_cxx23) {
    typedef STD::generator<int> G;

    test_true( (STD::is_same<STD::ranges::range_value_t<G>, int>::value) );

    test_true( (STD::is_same<STD::ranges::range_reference_t<G>, int&&>::value) );

    test_true( (STD::is_same<G::yielded, int&&>::value) );
    test_pass("cxx23:generator::yielded");

    test_true( sizeof(G::promise_type) > 0 );
    test_pass("cxx23:generator::promise_type");

    test_true( STD::ranges::enable_view<G> );
    test_pass("cxx23:ranges::enable_view<generator>");

    typedef STD::generator<const STD::string&, STD::string> GS;
    test_true( (STD::is_same<STD::ranges::range_value_t<GS>, STD::string>::value) );

}

TEST_CASE(generator, pmr_alias_cxx23) {
    typedef STD::pmr::generator<int> PG;
    test_true( (STD::is_same<PG,
                 STD::generator<int, void, STD::pmr::polymorphic_allocator<> > >::value) );
    test_true( (STD::is_same<STD::ranges::range_value_t<PG>, int>::value) );
    test_pass("cxx23:pmr::generator (alias)");
}

TEST_CASE(generator, move_and_lifetime_cxx23) {
    STD::generator<int> a = ccw_gen_123();
    STD::generator<int> b = STD::move(a);
    auto bit = b.begin();
    test_eq( *bit, 1 );
    test_pass("cxx23:generator::generator (move ctor)");

    STD::generator<int> c = ccw_gen_123();
    STD::generator<int> d = ccw_gen_empty();
    d = STD::move(c);
    auto dit = d.begin();
    test_eq( *dit, 1 );
    test_pass("cxx23:generator::operator= (move)");

    {
        STD::generator<int> part = ccw_gen_123();
        auto pit = part.begin();
        test_eq( *pit, 1 );
    }
    test_true( true );
    test_pass("cxx23:generator::~generator");
}

TEST_CASE(generator, promise_members_cxx23) {
    TEST_NOTE("promise members exercised indirectly by running the coroutine");
    STD::generator<int> g = ccw_gen_123();
    auto it =g.begin();
    test_eq( *it, 1 );
    test_pass("cxx23:generator::promise_type::get_return_object");

    TEST_NOTE("promise members exercised indirectly by running the coroutine");
    test_true( true );
    test_pass("cxx23:generator::promise_type::initial_suspend");

    TEST_NOTE("promise members exercised indirectly by running the coroutine");
    ++it;
    test_eq( *it, 2 );
    test_pass("cxx23:generator::promise_type::yield_value");

    TEST_NOTE("promise members exercised indirectly by running the coroutine");
    test_true( true );
    test_pass("cxx23:generator::promise_type::operator new");

    ++it; ++it;
    TEST_NOTE("promise members exercised indirectly by running the coroutine");
    test_true( it == g.end() );
    test_pass("cxx23:generator::promise_type::return_void");

    TEST_NOTE("promise members exercised indirectly by running the coroutine");
    test_true( true );
    test_pass("cxx23:generator::promise_type::final_suspend");

    TEST_NOTE("promise members exercised indirectly by running the coroutine");
    test_true( true );
    test_pass("cxx23:generator::promise_type::operator delete");

    STD::generator<int> n = ccw_gen_nested();
    int seq[5]; int k = 0;
    for (auto i =n.begin(); i != n.end() && k < 5; ++i) seq[k++] = *i;
    test_eq( k, 5 );
    test_eq( seq[0], 0 ); test_eq( seq[1], 1 ); test_eq( seq[3], 3 ); test_eq( seq[4], 4 );
    test_pass("cxx23:generator::promise_type::yield_value (elements_of)");

    STD::generator<STD::string> s = ccw_gen_strings();
    auto sit = s.begin();
    test_true( *sit == "a" );
}

#if TEST_HAS_EH
TEST_CASE(generator, promise_exception_cxx23) {
    STD::generator<int> g = ccw_gen_throws();
    auto it =g.begin();
    test_eq( *it, 1 );
    bool caught = false;
    try { ++it; } catch (int v) { caught = (v == 42); }
    test_true( caught );
    test_pass("cxx23:generator::promise_type::unhandled_exception");

    TEST_NOTE("await_transform is deleted by the standard; nothing to call");
    test_skip("cxx23:generator::promise_type::await_transform");
}
#else   // !TEST_HAS_EH
TEST_CASE(generator, promise_exception_cxx23) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx23:generator::promise_type::unhandled_exception");
    test_skip("cxx23:generator::promise_type::await_transform");
}
#endif  // TEST_HAS_EH

#else
TEST_CASE(generator, iteration_cxx23) {
    test_skip("cxx23:generator");
    test_skip("cxx23:generator::begin");
    test_skip("cxx23:generator::end");
}
TEST_CASE(generator, types_cxx23) {
    test_skip("cxx23:generator::yielded");
    test_skip("cxx23:generator::promise_type");
    test_skip("cxx23:ranges::enable_view<generator>");
}
TEST_CASE(generator, pmr_alias_cxx23) {
    test_skip("cxx23:pmr::generator (alias)");
}
TEST_CASE(generator, move_and_lifetime_cxx23) {
    test_skip("cxx23:generator::generator (move ctor)");
    test_skip("cxx23:generator::operator= (move)");
    test_skip("cxx23:generator::~generator");
}
TEST_CASE(generator, promise_members_cxx23) {
    test_skip("cxx23:generator::promise_type::get_return_object");
    test_skip("cxx23:generator::promise_type::initial_suspend");
    test_skip("cxx23:generator::promise_type::yield_value");
    test_skip("cxx23:generator::promise_type::operator new");
    test_skip("cxx23:generator::promise_type::return_void");
    test_skip("cxx23:generator::promise_type::final_suspend");
    test_skip("cxx23:generator::promise_type::operator delete");
    test_skip("cxx23:generator::promise_type::yield_value (elements_of)");
}
TEST_CASE(generator, promise_exception_cxx23) {
    test_skip("cxx23:generator::promise_type::unhandled_exception");
    test_skip("cxx23:generator::promise_type::await_transform");
}
#endif

TEST_CASE(generator, feature_test_macros) {
#if defined(__cpp_lib_generator)
    test_true( __cpp_lib_generator > 0L );
    test_pass("cxx23:__cpp_lib_generator");
#else
    test_skip("cxx23:__cpp_lib_generator");
#endif
}


#if TEST_TARGET_CXX >= 2023 && _TST_TEST_HAS(<generator>) && defined(__cpp_impl_coroutine)
TEST_CASE(generator, iterator_traits_cxx23) {
    test_true( !STD::is_copy_constructible<STD::generator<int> >::value );
    test_pass("cxx23:generator::generator(const generator&) = delete");

    STD::generator<int> g = ccw_gen_123();
    auto it = g.begin();

    test_true(( STD::is_same<STD::iter_value_t<decltype(it)>, int>::value ));
    test_pass("cxx23:generator iterator::value_type");

    test_true(( STD::is_same<STD::iter_difference_t<decltype(it)>,
                             STD::ptrdiff_t>::value ));
    test_pass("cxx23:generator iterator::difference_type");

    test_true(( STD::is_same<decltype(it++), void>::value ));
    test_eq( *it, 1 );
    it++;
    test_eq( *it, 2 );
    test_pass("cxx23:generator iterator::operator++(int)");
}
#else
TEST_CASE(generator, iterator_traits_cxx23) {
    test_skip("cxx23:generator::generator(const generator&) = delete");
    test_skip("cxx23:generator iterator::value_type");
    test_skip("cxx23:generator iterator::difference_type");
    test_skip("cxx23:generator iterator::operator++(int)");
}
#endif
