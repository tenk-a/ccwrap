#include "test_cxx.hpp"

TEST_CASE_SKIP(concepts, unimplemented)

#if _tst_cplusplus >= 202002L
#include <concepts>
#include <string>
#include <memory>
#include <utility>
#include <type_traits>

namespace {

struct Base {};
struct Derived : Base {};
struct Unrelated {};

struct NoDefault {
    explicit NoDefault(int) {}
};

struct NoMove {
    NoMove() = default;
    NoMove(const NoMove&) = delete;
    NoMove(NoMove&&) = delete;
    NoMove& operator=(NoMove&&) = delete;
};

struct NoEq { int v; };

struct EqOnly {
    int v;
    bool operator==(const EqOnly&) const = default;
};

struct Full {
    int v = 0;
    auto operator<=>(const Full&) const = default;
    bool operator==(const Full&) const = default;
};

struct AdlSwap {
    int v = 0;
    friend void swap(AdlSwap& a, AdlSwap& b) noexcept { int t = a.v; a.v = b.v; b.v = t; }
};

struct MemberOnly {
    int v = 0;
    MemberOnly() = default;
    MemberOnly(const MemberOnly&) = default;
    MemberOnly& operator=(const MemberOnly&) = default;
};

struct NoDtor {
    ~NoDtor() = delete;
};

}

TEST_CASE(concepts, core_language_concepts) {

    static_assert( STD::same_as<int, int> );
    static_assert( !STD::same_as<int, const int> );
    test_true(  (STD::same_as<int, int>) );
    test_true(  (STD::same_as<STD::string, STD::basic_string<char> >) );
    test_true( !(STD::same_as<int, const int>) );
    test_true( !(STD::same_as<int, unsigned int>) );
    test_pass("cxx20:same_as");
    test_true( !(STD::same_as<int, long>) );

    static_assert( STD::derived_from<Derived, Base> );
    static_assert( !STD::derived_from<Base, Derived> );
    test_true(  (STD::derived_from<Derived, Base>) );
    test_true(  (STD::derived_from<Base, Base>) );
    test_true( !(STD::derived_from<Base, Derived>) );
    test_true( !(STD::derived_from<Unrelated, Base>) );
    test_true( !(STD::derived_from<int, Base>) );
    test_pass("cxx20:derived_from");

    static_assert( STD::convertible_to<int, double> );
    static_assert( !STD::convertible_to<int, STD::string> );
    test_true(  (STD::convertible_to<int, double>) );
    test_true(  (STD::convertible_to<Derived*, Base*>) );
    test_true(  (STD::convertible_to<const char*, STD::string>) );
    test_true( !(STD::convertible_to<int, STD::string>) );
    test_true( !(STD::convertible_to<Base*, Derived*>) );
    test_true( !(STD::convertible_to<void*, int*>) );
    test_pass("cxx20:convertible_to");

    static_assert( STD::common_reference_with<int&, const int&> );
    test_true(  (STD::common_reference_with<int&, const int&>) );
    test_true(  (STD::common_reference_with<int, double>) );
    test_true(  (STD::common_reference_with<Derived&, Base&>) );
    test_true( !(STD::common_reference_with<int, STD::string>) );
    test_true( !(STD::common_reference_with<Base, Unrelated>) );
    test_pass("cxx20:common_reference_with");

    static_assert( STD::common_with<int, double> );
    test_true(  (STD::common_with<int, double>) );
    test_true(  (STD::common_with<int, int>) );
    test_true(  (STD::common_with<Derived*, Base*>) );
    test_true( !(STD::common_with<int, STD::string>) );
    test_true( !(STD::common_with<Base, Unrelated>) );
    test_pass("cxx20:common_with");
}

TEST_CASE(concepts, arithmetic_concepts) {

    static_assert( STD::integral<int> );
    static_assert( !STD::integral<double> );
    test_true(  STD::integral<int> );
    test_true(  STD::integral<bool> );
    test_true(  STD::integral<char> );
    test_true(  STD::integral<unsigned long long> );
    test_true( !STD::integral<double> );
    test_true( !STD::integral<STD::string> );
    test_true( !STD::integral<int*> );
    test_pass("cxx20:integral");

    static_assert( STD::signed_integral<int> );
    static_assert( !STD::signed_integral<unsigned> );
    test_true(  STD::signed_integral<int> );
    test_true(  STD::signed_integral<signed char> );
    test_true(  STD::signed_integral<long long> );
    test_true( !STD::signed_integral<unsigned> );
    test_true( !STD::signed_integral<bool> );
    test_true( !STD::signed_integral<double> );
    test_pass("cxx20:signed_integral");

    static_assert( STD::unsigned_integral<unsigned> );
    static_assert( !STD::unsigned_integral<int> );
    test_true(  STD::unsigned_integral<unsigned> );
    test_true(  STD::unsigned_integral<bool> );
    test_true(  STD::unsigned_integral<unsigned char> );
    test_true( !STD::unsigned_integral<int> );
    test_true( !STD::unsigned_integral<double> );

    test_true( STD::signed_integral<int> != STD::unsigned_integral<int> );
    test_pass("cxx20:unsigned_integral");

    static_assert( STD::floating_point<double> );
    static_assert( !STD::floating_point<int> );
    test_true(  STD::floating_point<float> );
    test_true(  STD::floating_point<double> );
    test_true(  STD::floating_point<long double> );
    test_true( !STD::floating_point<int> );
    test_true( !STD::floating_point<STD::string> );
    test_pass("cxx20:floating_point");
}

TEST_CASE(concepts, assignable_and_swappable) {

    static_assert( STD::assignable_from<int&, int> );
    static_assert( !STD::assignable_from<int, int> );
    test_true(  (STD::assignable_from<int&, int>) );
    test_true(  (STD::assignable_from<int&, double>) );
    test_true(  (STD::assignable_from<STD::string&, const char*>) );
    test_true( !(STD::assignable_from<int, int>) );
    test_true( !(STD::assignable_from<const int&, int>) );
    test_true( !(STD::assignable_from<int&, STD::string>) );
    test_pass("cxx20:assignable_from");

    static_assert( STD::swappable<int> );
    static_assert( !STD::swappable<NoMove> );
    test_true(  STD::swappable<int> );
    test_true(  STD::swappable<STD::string> );
    test_true(  STD::swappable<AdlSwap> );
    test_true(  STD::swappable<STD::unique_ptr<int> > );
    test_true( !STD::swappable<NoMove> );
    test_pass("cxx20:swappable");

    static_assert( STD::swappable_with<int&, int&> );
    test_true(  (STD::swappable_with<int&, int&>) );
    test_true(  (STD::swappable_with<AdlSwap&, AdlSwap&>) );
    test_true( !(STD::swappable_with<int&, double&>) );
    test_true( !(STD::swappable_with<int&, STD::string&>) );
    test_pass("cxx20:swappable_with");
}

TEST_CASE(concepts, object_lifetime_concepts) {

    static_assert( STD::destructible<int> );
    static_assert( !STD::destructible<void> );
    test_true(  STD::destructible<int> );
    test_true(  STD::destructible<STD::string> );
    test_true(  STD::destructible<NoMove> );
    test_true( !STD::destructible<void> );
    test_true( !STD::destructible<int()> );
    test_true( !STD::destructible<NoDtor> );
    test_pass("cxx20:destructible");

    static_assert( STD::constructible_from<int, int> );
    static_assert( !STD::constructible_from<NoDefault> );
    test_true(  (STD::constructible_from<int, int>) );
    test_true(  (STD::constructible_from<int, double>) );
    test_true(  (STD::constructible_from<STD::string, const char*>) );
    test_true(  (STD::constructible_from<NoDefault, int>) );
    test_true( !(STD::constructible_from<NoDefault>) );
    test_true( !(STD::constructible_from<int, STD::string>) );
    test_pass("cxx20:constructible_from");

    static_assert( STD::default_initializable<int> );
    static_assert( !STD::default_initializable<NoDefault> );
    test_true(  STD::default_initializable<int> );
    test_true(  STD::default_initializable<STD::string> );
    test_true(  STD::default_initializable<Full> );
    test_true( !STD::default_initializable<NoDefault> );
    test_true( !STD::default_initializable<int&> );
    test_pass("cxx20:default_initializable");

    static_assert( STD::move_constructible<int> );
    static_assert( !STD::move_constructible<NoMove> );
    test_true(  STD::move_constructible<int> );
    test_true(  STD::move_constructible<STD::string> );
    test_true(  (STD::move_constructible<STD::unique_ptr<int> >) );
    test_true( !STD::move_constructible<NoMove> );
    test_pass("cxx20:move_constructible");

    static_assert( STD::copy_constructible<int> );
    static_assert( !STD::copy_constructible<STD::unique_ptr<int> > );
    test_true(  STD::copy_constructible<int> );
    test_true(  STD::copy_constructible<STD::string> );
    test_true( !(STD::copy_constructible<STD::unique_ptr<int> >) );
    test_true( !STD::copy_constructible<NoMove> );
    test_pass("cxx20:copy_constructible");
}

TEST_CASE(concepts, comparison_concepts) {

    static_assert( STD::equality_comparable<int> );
    static_assert( !STD::equality_comparable<NoEq> );
    test_true(  STD::equality_comparable<int> );
    test_true(  STD::equality_comparable<STD::string> );
    test_true(  STD::equality_comparable<EqOnly> );
    test_true(  STD::equality_comparable<Full> );
    test_true( !STD::equality_comparable<NoEq> );
    test_pass("cxx20:equality_comparable");

    static_assert( STD::equality_comparable_with<int, long> );
    test_true(  (STD::equality_comparable_with<int, int>) );
    test_true(  (STD::equality_comparable_with<int, double>) );
    test_true(  (STD::equality_comparable_with<STD::string, const char*>) );
    test_true( !(STD::equality_comparable_with<int, STD::string>) );
    test_true( !(STD::equality_comparable_with<EqOnly, Full>) );
    test_pass("cxx20:equality_comparable_with");

    static_assert( STD::totally_ordered<int> );
    static_assert( !STD::totally_ordered<EqOnly> );
    test_true(  STD::totally_ordered<int> );
    test_true(  STD::totally_ordered<double> );
    test_true(  STD::totally_ordered<STD::string> );
    test_true(  STD::totally_ordered<Full> );
    test_true( !STD::totally_ordered<EqOnly> );
    test_true( !STD::totally_ordered<NoEq> );
    test_pass("cxx20:totally_ordered");

    static_assert( STD::totally_ordered_with<int, double> );
    test_true(  (STD::totally_ordered_with<int, int>) );
    test_true(  (STD::totally_ordered_with<int, double>) );
    test_true(  (STD::totally_ordered_with<STD::string, const char*>) );
    test_true( !(STD::totally_ordered_with<int, STD::string>) );
    test_true( !(STD::totally_ordered_with<Full, EqOnly>) );
    test_pass("cxx20:totally_ordered_with");
}

TEST_CASE(concepts, object_concepts) {

    static_assert( STD::movable<int> );
    static_assert( !STD::movable<NoMove> );
    test_true(  STD::movable<int> );
    test_true(  STD::movable<STD::string> );
    test_true(  (STD::movable<STD::unique_ptr<int> >) );
    test_true( !STD::movable<NoMove> );
    test_true( !STD::movable<int&> );
    test_pass("cxx20:movable");

    static_assert( STD::copyable<int> );
    static_assert( !STD::copyable<STD::unique_ptr<int> > );
    test_true(  STD::copyable<int> );
    test_true(  STD::copyable<STD::string> );
    test_true(  STD::copyable<Full> );
    test_true( !(STD::copyable<STD::unique_ptr<int> >) );
    test_pass("cxx20:copyable");

    static_assert( STD::semiregular<int> );
    static_assert( !STD::semiregular<NoDefault> );
    test_true(  STD::semiregular<int> );
    test_true(  STD::semiregular<STD::string> );
    test_true(  STD::semiregular<NoEq> );
    test_true( !STD::semiregular<NoDefault> );
    test_true( !(STD::semiregular<STD::unique_ptr<int> >) );
    test_pass("cxx20:semiregular");

    static_assert( STD::regular<int> );
    static_assert( !STD::regular<NoEq> );
    test_true(  STD::regular<int> );
    test_true(  STD::regular<STD::string> );
    test_true(  STD::regular<Full> );
    test_true(  STD::regular<EqOnly> );
    test_true( !STD::regular<NoEq> );
    test_true( !STD::regular<NoDefault> );
    test_pass("cxx20:regular");
}

TEST_CASE(concepts, callable_concepts) {
    auto add     = [](int a, int b) { return a + b; };
    auto is_even = [](int a) { return a % 2 == 0; };
    auto less    = [](int a, int b) { return a < b; };
    auto same    = [](int a, int b) { return a == b; };
    auto to_int  = [](int a) { return a; };

    typedef decltype(add)     Add;
    typedef decltype(is_even) IsEven;
    typedef decltype(less)    Less;
    typedef decltype(same)    Same;
    typedef decltype(to_int)  ToInt;

    static_assert( STD::invocable<Add, int, int> );
    static_assert( !STD::invocable<Add, int> );
    test_true(  (STD::invocable<Add, int, int>) );
    test_true(  (STD::invocable<IsEven, int>) );
    test_true( !(STD::invocable<Add, int>) );
    test_true( !(STD::invocable<Add, STD::string, STD::string>) );
    test_true( !(STD::invocable<int, int>) );
    test_pass("cxx20:invocable");

    static_assert( STD::regular_invocable<Add, int, int> );
    test_true(  (STD::regular_invocable<Add, int, int>) );
    test_true(  (STD::regular_invocable<IsEven, int>) );
    test_true( !(STD::regular_invocable<Add, int>) );
    test_pass("cxx20:regular_invocable");

    static_assert( STD::predicate<IsEven, int> );
    static_assert( STD::predicate<Add, int, int> );
    test_true(  (STD::predicate<IsEven, int>) );
    test_true(  (STD::predicate<Less, int, int>) );
    test_true( !(STD::predicate<IsEven, STD::string>) );
    test_true( !(STD::predicate<IsEven, int, int>) );
    test_pass("cxx20:predicate");

    static_assert( STD::relation<Less, int, int> );
    test_true(  (STD::relation<Less, int, int>) );
    test_true(  (STD::relation<Same, int, int>) );
    test_true( !(STD::relation<IsEven, int, int>) );
    test_true( !(STD::relation<Less, STD::string, STD::string>) );
    test_pass("cxx20:relation");

    static_assert( STD::equivalence_relation<Same, int, int> );
    test_true(  (STD::equivalence_relation<Same, int, int>) );
    test_true(  (STD::equivalence_relation<Less, int, int>) );
    test_true( !(STD::equivalence_relation<IsEven, int, int>) );
    test_pass("cxx20:equivalence_relation");

    static_assert( STD::strict_weak_order<Less, int, int> );
    test_true(  (STD::strict_weak_order<Less, int, int>) );
    test_true(  (STD::strict_weak_order<Same, int, int>) );
    test_true( !(STD::strict_weak_order<IsEven, int, int>) );
    test_true( !(STD::strict_weak_order<ToInt, int, int>) );

    test_eq( add(2, 3), 5 );
    test_true( is_even(4) );
    test_true( less(1, 2) );
    test_pass("cxx20:strict_weak_order");
}

TEST_CASE(concepts, ranges_swap) {

    int a = 1, b = 2;
    STD::ranges::swap(a, b);
    test_eq( a, 2 );
    test_eq( b, 1 );

    test_pass("cxx20:ranges::swap");

    int x[3] = { 1, 2, 3 };
    int y[3] = { 4, 5, 6 };
    STD::ranges::swap(x, y);
    test_eq( x[0], 4 );
    test_eq( x[2], 6 );
    test_eq( y[0], 1 );
    test_eq( y[2], 3 );
    test_pass("cxx20:ranges::swap array");

    AdlSwap p{1}, q{2};
    STD::ranges::swap(p, q);
    test_eq( p.v, 2 );
    test_eq( q.v, 1 );
    test_pass("cxx20:ranges::swap adl");

    MemberOnly m, n;
    m.v = 1;
    n.v = 2;
    STD::ranges::swap(m, n);
    test_eq( m.v, 2 );
    test_eq( n.v, 1 );
    test_pass("cxx20:ranges::swap move fallback");

    STD::unique_ptr<int> u(new int(7)), v(new int(8));
    int* pu = u.get();
    int* pv = v.get();
    STD::ranges::swap(u, v);
    test_true( u.get() == pv );
    test_true( v.get() == pu );
    test_eq( *u, 8 );
    test_eq( *v, 7 );
    test_pass("cxx20:ranges::swap move-only");

    STD::string s1 = "left", s2 = "right";
    STD::ranges::swap(s1, s2);
    test_eq( s1, "right" );
    test_eq( s2, "left" );
    test_pass("cxx20:ranges::swap string");

    test_true( STD::swappable<AdlSwap> );
    test_true( STD::swappable<MemberOnly> );
    test_true( !STD::swappable<NoMove> );
    test_pass("cxx20:swappable via ranges::swap");
}
#else
TEST_CASE_SKIP(concepts, core_language_concepts)
TEST_CASE_SKIP(concepts, arithmetic_concepts)
TEST_CASE_SKIP(concepts, assignable_and_swappable)
TEST_CASE_SKIP(concepts, object_lifetime_concepts)
TEST_CASE_SKIP(concepts, comparison_concepts)
TEST_CASE_SKIP(concepts, object_concepts)
TEST_CASE_SKIP(concepts, callable_concepts)
TEST_CASE_SKIP(concepts, ranges_swap)
#endif
