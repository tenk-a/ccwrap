#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2020
#include <compare>
#include <execution>
#include <version>

TEST_CASE(compare, orderings) {
    STD::strong_ordering so = STD::strong_ordering::less;
    test_true( so < 0 );
    test_true( so != 0 );
    test_true( !(so == 0) );
    test_true( !(so > 0) );
    test_pass("cxx20:strong_ordering (vs 0)");

    STD::weak_ordering wo = STD::weak_ordering::greater;
    test_true( wo > 0 );
    test_pass("cxx20:weak_ordering (vs 0)");
    test_true( STD::is_gt(wo) );
    test_pass("cxx20:is_gt (weak_ordering)");
    test_true( STD::is_gteq(STD::weak_ordering::equivalent) );
    test_pass("cxx20:is_gteq (weak_ordering)");

    STD::partial_ordering un = STD::partial_ordering::unordered;
    test_true( !(un < 0) && !(un == 0) && !(un > 0) );
    test_true( un != 0 );
    test_pass("cxx20:partial_ordering::unordered");
}

TEST_CASE(compare, conversions) {
    STD::partial_ordering pe = STD::strong_ordering::equal;
    test_true( pe == 0 );
    test_pass("cxx20:strong_ordering to partial_ordering");
    test_true( STD::is_eq(pe) );
    test_pass("cxx20:is_eq (partial)");
    test_true( STD::is_lt(STD::strong_ordering::less) );
    test_pass("cxx20:is_lt (strong_ordering)");

    STD::weak_ordering we = STD::strong_ordering::greater;
    test_true( we > 0 );
    test_pass("cxx20:strong_ordering to weak_ordering");
}

#if _TST_HAS_EXECUTION_POLICY
TEST_CASE(execution, policies) {
    test_true( STD::is_execution_policy<STD::execution::sequenced_policy>::value );
    test_true( !STD::is_execution_policy<int>::value );
    test_pass("cxx20:is_execution_policy");
    (void)STD::execution::seq;
    (void)STD::execution::par;
    (void)STD::execution::par_unseq;
    (void)STD::execution::unseq;
    test_true( 1 );
    test_pass("cxx20:execution policy objects");
}
#else
TEST_CASE(execution, policies) {
    TEST_NOTE("no execution policies here (libc++ keeps its PSTL behind _LIBCPP_ENABLE_EXPERIMENTAL)");
    test_skip("cxx20:is_execution_policy");
    test_skip("cxx20:execution policy objects");
}
#endif

#if _TST_TEST_HAS(<debugging>) && _TST_HAS_OS_THREADS
#include <debugging>
TEST_CASE(debugging, present_and_breakpoint) {
    bool p = STD::is_debugger_present();
    test_true( p == false || p == true );
    test_pass("cxx26:is_debugger_present");
    STD::breakpoint_if_debugging();
    test_true( 1 );
    test_pass("cxx26:breakpoint_if_debugging");
}
#endif
#endif

#if _tst_cplusplus >= 202002L
#include <compare>
#include <string>
#include <type_traits>
#include <limits>
#include <cmath>

namespace {

struct Strong {
    int v;
    auto operator<=>(const Strong&) const = default;
    bool operator==(const Strong&) const = default;
};

struct Weak {
    int v;
    STD::weak_ordering operator<=>(const Weak& o) const {
        return (v < 0 ? -1 : v > 0 ? 1 : 0) <=> (o.v < 0 ? -1 : o.v > 0 ? 1 : 0);
    }
    bool operator==(const Weak& o) const { return (*this <=> o) == 0; }
};

struct OnlyLt {
    int v;
    friend bool operator==(OnlyLt a, OnlyLt b) { return a.v == b.v; }
    friend bool operator< (OnlyLt a, OnlyLt b) { return a.v <  b.v; }
};

struct None { int v; };

template <class T> struct names_a_type { static const bool value = true; };

template <class T> concept has_3way_result   = requires { typename STD::compare_three_way_result<T>::type; };
template <class T> concept has_strong_order  = requires (T a, T b) { STD::strong_order(a, b); };
template <class T> concept has_weak_order    = requires (T a, T b) { STD::weak_order(a, b); };
template <class T> concept has_partial_order = requires (T a, T b) { STD::partial_order(a, b); };
template <class T> concept has_strong_fb     = requires (T a, T b) { STD::compare_strong_order_fallback(a, b); };
template <class T> concept has_weak_fb       = requires (T a, T b) { STD::compare_weak_order_fallback(a, b); };
template <class T> concept has_partial_fb    = requires (T a, T b) { STD::compare_partial_order_fallback(a, b); };

}

TEST_CASE(compare, strong_ordering_constants) {

    test_true( STD::strong_ordering::less    <  0 );
    test_true( STD::strong_ordering::less    <= 0 );
    test_true( STD::strong_ordering::less    != 0 );
    test_true( !(STD::strong_ordering::less  == 0) );
    test_pass("cxx20:strong_ordering::less");

    test_true( STD::strong_ordering::equal      == 0 );
    test_true( STD::strong_ordering::equal      <= 0 );
    test_true( STD::strong_ordering::equal      >= 0 );
    test_true( STD::strong_ordering::equivalent == 0 );
    test_pass("cxx20:strong_ordering::equal");

    test_true( STD::strong_ordering::greater >  0 );
    test_true( STD::strong_ordering::greater >= 0 );
    test_true( STD::strong_ordering::greater != 0 );
    test_pass("cxx20:strong_ordering::greater");

    test_true( STD::strong_ordering::equal == STD::strong_ordering::equivalent );
    test_true( STD::strong_ordering::less  != STD::strong_ordering::greater );
    test_pass("cxx20:strong_ordering equality of constants");

    test_true( STD::strong_ordering::less == STD::strong_ordering::less );
    test_true( STD::strong_ordering::less != STD::strong_ordering::equal );
    test_pass("cxx20:strong_ordering operator==");

    test_true( (STD::strong_ordering::less    <=> 0) <  0 );
    test_true( (STD::strong_ordering::equal   <=> 0) == 0 );
    test_true( (STD::strong_ordering::greater <=> 0) >  0 );
    test_true( (0 <=> STD::strong_ordering::less)    >  0 );
    test_true( (0 <=> STD::strong_ordering::greater)  < 0 );
    test_pass("cxx20:strong_ordering operator<=>");

    test_true( 0 > STD::strong_ordering::less );
    test_true( 0 < STD::strong_ordering::greater );
    test_pass("cxx20:strong_ordering reversed relation");
}

TEST_CASE(compare, weak_ordering_constants) {

    test_true( STD::weak_ordering::less       <  0 );
    test_true( STD::weak_ordering::equivalent == 0 );
    test_true( STD::weak_ordering::greater    >  0 );
    test_true( STD::weak_ordering::less       != STD::weak_ordering::greater );
    test_true( STD::weak_ordering::equivalent == STD::weak_ordering::equivalent );
    test_pass("cxx20:weak_ordering constants");

    test_true( (STD::weak_ordering::less    <=> 0) <  0 );
    test_true( (STD::weak_ordering::greater <=> 0) >  0 );
    test_true( (0 <=> STD::weak_ordering::less)    >  0 );
    test_pass("cxx20:weak_ordering operator<=>");

    STD::weak_ordering w1 = STD::strong_ordering::less;
    STD::weak_ordering w2 = STD::strong_ordering::equal;
    STD::weak_ordering w3 = STD::strong_ordering::greater;
    test_true( w1 == STD::weak_ordering::less );
    test_true( w2 == STD::weak_ordering::equivalent );
    test_true( w3 == STD::weak_ordering::greater );
    test_pass("cxx20:weak_ordering from strong_ordering");
    test_true( (STD::is_convertible<STD::strong_ordering, STD::weak_ordering>::value) );
    test_true( !(STD::is_convertible<STD::weak_ordering, STD::strong_ordering>::value) );
    test_pass("cxx20:weak_ordering convertibility");
}

TEST_CASE(compare, partial_ordering_constants) {

    test_true( STD::partial_ordering::less       <  0 );
    test_true( STD::partial_ordering::equivalent == 0 );
    test_true( STD::partial_ordering::greater    >  0 );
    test_pass("cxx20:partial_ordering constants");

    test_true( !(STD::partial_ordering::unordered <  0) );
    test_true( !(STD::partial_ordering::unordered == 0) );
    test_true( !(STD::partial_ordering::unordered >  0) );
    test_true( !(STD::partial_ordering::unordered <= 0) );
    test_true( !(STD::partial_ordering::unordered >= 0) );
    test_true(   STD::partial_ordering::unordered != 0 );
    test_pass("cxx20:partial_ordering::unordered vs 0");

    test_true( STD::partial_ordering::unordered == STD::partial_ordering::unordered );
    test_true( STD::partial_ordering::unordered != STD::partial_ordering::equivalent );
    test_pass("cxx20:partial_ordering::unordered equality");

    test_true( (STD::partial_ordering::unordered <=> 0) == STD::partial_ordering::unordered );
    test_true( (STD::partial_ordering::less      <=> 0) <  0 );
    test_true( (0 <=> STD::partial_ordering::greater)   <  0 );
    test_pass("cxx20:partial_ordering operator<=>");

    STD::partial_ordering p1 = STD::strong_ordering::less;
    STD::partial_ordering p2 = STD::weak_ordering::greater;
    STD::partial_ordering p3 = STD::strong_ordering::equivalent;
    test_true( p1 == STD::partial_ordering::less );
    test_true( p3 == STD::partial_ordering::equivalent );
    test_pass("cxx20:partial_ordering from strong_ordering");
    test_true( p2 == STD::partial_ordering::greater );
    test_pass("cxx20:partial_ordering from weak_ordering");
    test_true( (STD::is_convertible<STD::strong_ordering, STD::partial_ordering>::value) );
    test_true( (STD::is_convertible<STD::weak_ordering,   STD::partial_ordering>::value) );
    test_true( !(STD::is_convertible<STD::partial_ordering, STD::weak_ordering>::value) );
    test_pass("cxx20:partial_ordering convertibility");

    double nan = STD::numeric_limits<double>::quiet_NaN();
    test_true( (1.0 <=> nan) == STD::partial_ordering::unordered );
    test_true( (1.0 <=> 2.0) == STD::partial_ordering::less );
    test_true( (STD::is_same<decltype(1.0 <=> 2.0), STD::partial_ordering>::value) );
    test_true( (STD::is_same<decltype(1 <=> 2), STD::strong_ordering>::value) );
    test_pass("cxx20:floating-point operator<=>");
}

TEST_CASE(compare, is_eq_family) {

    test_true(  STD::is_eq  (STD::partial_ordering::equivalent) );
    test_true( !STD::is_eq  (STD::partial_ordering::less) );
    test_true( !STD::is_eq  (STD::partial_ordering::unordered) );
    test_pass("cxx20:is_eq");

    test_true(  STD::is_neq (STD::partial_ordering::less) );
    test_true(  STD::is_neq (STD::partial_ordering::unordered) );
    test_true( !STD::is_neq (STD::partial_ordering::equivalent) );
    test_pass("cxx20:is_neq");

    test_true(  STD::is_lt  (STD::partial_ordering::less) );
    test_true( !STD::is_lt  (STD::partial_ordering::equivalent) );
    test_true( !STD::is_lt  (STD::partial_ordering::unordered) );
    test_pass("cxx20:is_lt");

    test_true(  STD::is_lteq(STD::partial_ordering::less) );
    test_true(  STD::is_lteq(STD::partial_ordering::equivalent) );
    test_true( !STD::is_lteq(STD::partial_ordering::greater) );
    test_true( !STD::is_lteq(STD::partial_ordering::unordered) );
    test_pass("cxx20:is_lteq");

    test_true(  STD::is_gt  (STD::partial_ordering::greater) );
    test_true( !STD::is_gt  (STD::partial_ordering::equivalent) );
    test_true( !STD::is_gt  (STD::partial_ordering::unordered) );
    test_pass("cxx20:is_gt");

    test_true(  STD::is_gteq(STD::partial_ordering::greater) );
    test_true(  STD::is_gteq(STD::partial_ordering::equivalent) );
    test_true( !STD::is_gteq(STD::partial_ordering::less) );
    test_true( !STD::is_gteq(STD::partial_ordering::unordered) );
    test_pass("cxx20:is_gteq");

    test_true(  STD::is_lt  (STD::strong_ordering::less) );
    test_true(  STD::is_eq  (STD::strong_ordering::equal) );
    test_pass("cxx20:is_* from strong_ordering");
    test_true(  STD::is_gteq(STD::weak_ordering::equivalent) );
    test_pass("cxx20:is_* from weak_ordering");

    test_true(  STD::is_lt(1 <=> 2) );
    test_true(  STD::is_gt(STD::string("b") <=> STD::string("a")) );
    test_true( STD::is_eq(Strong{3} <=> Strong{3}) );
    test_pass("cxx20:is_* from <=>");
}

TEST_CASE(compare, common_comparison_category) {
    typedef STD::strong_ordering  S;
    typedef STD::weak_ordering    W;
    typedef STD::partial_ordering P;

    test_true( (STD::is_same<STD::common_comparison_category_t<S, S>, S>::value) );
    test_true( (STD::is_same<STD::common_comparison_category_t<S, W>, W>::value) );
    test_true( (STD::is_same<STD::common_comparison_category_t<W, S>, W>::value) );
    test_true( (STD::is_same<STD::common_comparison_category_t<S, P>, P>::value) );
    test_true( (STD::is_same<STD::common_comparison_category_t<W, P>, P>::value) );
    test_true( (STD::is_same<STD::common_comparison_category_t<S, W, P>, P>::value) );
    test_true( (STD::is_same<STD::common_comparison_category_t<S, int>, void>::value) );
    test_true( (STD::is_same<STD::common_comparison_category_t<>, S>::value) );
    test_pass("cxx20:common_comparison_category_t");

    test_true( (STD::is_same<STD::common_comparison_category<S, W>::type, W>::value) );
    test_true( (STD::is_same<STD::common_comparison_category<P>::type, P>::value) );
    test_pass("cxx20:common_comparison_category::type");

    static_assert(STD::is_same_v<STD::common_comparison_category_t<S, W, P>, P>);
    static_assert(STD::is_same_v<STD::common_comparison_category<S, S>::type, S>);
}

TEST_CASE(compare, compare_three_way_result) {
    typedef STD::strong_ordering  S;
    typedef STD::partial_ordering P;
    typedef STD::weak_ordering    W;

    test_true( (STD::is_same<STD::compare_three_way_result_t<int>, S>::value) );
    test_true( (STD::is_same<STD::compare_three_way_result_t<int, long>, S>::value) );
    test_true( (STD::is_same<STD::compare_three_way_result_t<double>, P>::value) );
    test_true( (STD::is_same<STD::compare_three_way_result_t<int, double>, P>::value) );
    test_true( (STD::is_same<STD::compare_three_way_result_t<STD::string>, S>::value) );
    test_true( (STD::is_same<STD::compare_three_way_result_t<Strong>, S>::value) );
    test_true( (STD::is_same<STD::compare_three_way_result_t<Weak>, W>::value) );
    test_pass("cxx20:compare_three_way_result_t");

    test_true( (STD::is_same<STD::compare_three_way_result<int>::type, S>::value) );
    test_true( (STD::is_same<STD::compare_three_way_result<double, double>::type, P>::value) );
    test_pass("cxx20:compare_three_way_result::type");

    test_true(  has_3way_result<int> );
    test_true( !has_3way_result<None> );
    test_pass("cxx20:compare_three_way_result SFINAE");

    static_assert(STD::is_same_v<STD::compare_three_way_result_t<int>, S>);
}

TEST_CASE(compare, three_way_comparable_concepts) {

    test_true(  STD::three_way_comparable<int> );
    test_true(  STD::three_way_comparable<double> );
    test_true(  STD::three_way_comparable<STD::string> );
    test_true(  STD::three_way_comparable<Strong> );
    test_true(  STD::three_way_comparable<Weak> );
    test_true( !STD::three_way_comparable<None> );
    test_true( !STD::three_way_comparable<OnlyLt> );
    test_pass("cxx20:three_way_comparable");

    test_true(  (STD::three_way_comparable<int, STD::strong_ordering>) );
    test_true(  (STD::three_way_comparable<int, STD::weak_ordering>) );
    test_true(  (STD::three_way_comparable<int, STD::partial_ordering>) );
    test_true( !(STD::three_way_comparable<double, STD::strong_ordering>) );
    test_true(  (STD::three_way_comparable<double, STD::partial_ordering>) );
    test_true( !(STD::three_way_comparable<Weak, STD::strong_ordering>) );
    test_true(  (STD::three_way_comparable<Weak, STD::weak_ordering>) );
    test_pass("cxx20:three_way_comparable (with category)");

    test_true(  (STD::three_way_comparable_with<int, int>) );
    test_true(  (STD::three_way_comparable_with<int, long>) );
    test_true(  (STD::three_way_comparable_with<int, double>) );
    test_true( !(STD::three_way_comparable_with<int, STD::string>) );
    test_true( !(STD::three_way_comparable_with<Strong, Weak>) );
    test_true(  (STD::three_way_comparable_with<int, double, STD::partial_ordering>) );
    test_true( !(STD::three_way_comparable_with<int, double, STD::strong_ordering>) );
    test_true(  (STD::three_way_comparable_with<int, long, STD::strong_ordering>) );
    test_pass("cxx20:three_way_comparable_with");

    static_assert(STD::three_way_comparable<int, STD::strong_ordering>);
    static_assert(!STD::three_way_comparable<None>);
    static_assert(STD::three_way_comparable_with<int, long>);
}

TEST_CASE(compare, compare_three_way_object) {
    STD::compare_three_way cmp;

    test_true( cmp(1, 2)   <  0 );
    test_true( cmp(2, 2)   == 0 );
    test_true( cmp(3, 2)   >  0 );
    test_true( (STD::is_same<decltype(cmp(1, 2)), STD::strong_ordering>::value) );
    test_pass("cxx20:compare_three_way operator()");

    test_true( cmp(1, 2L)  <  0 );
    test_true( cmp(1, 1.5) <  0 );
    test_true( (STD::is_same<decltype(cmp(1, 1.5)), STD::partial_ordering>::value) );
    test_pass("cxx20:compare_three_way (heterogeneous)");

    double nan = STD::numeric_limits<double>::quiet_NaN();
    test_true( cmp(1.0, nan) == STD::partial_ordering::unordered );
    test_pass("cxx20:compare_three_way (unordered)");

    test_true( cmp(STD::string("a"), STD::string("b")) < 0 );
    test_true( cmp(Strong{1}, Strong{2}) < 0 );
    test_true( cmp(Weak{5}, Weak{7}) == 0 );
    test_true( (STD::is_same<decltype(cmp(Weak{1}, Weak{2})), STD::weak_ordering>::value) );
    test_pass("cxx20:compare_three_way (user types)");

    int arr[3] = { 0, 1, 2 };
    test_true( cmp(arr + 0, arr + 1) < 0 );
    test_pass("cxx20:compare_three_way (pointers)");

    test_true( (names_a_type<STD::compare_three_way::is_transparent>::value) );
    test_pass("cxx20:compare_three_way::is_transparent");
    static_assert(names_a_type<STD::compare_three_way::is_transparent>::value);

    STD::compare_three_way cmp2 = cmp;
    test_true( cmp2(0, 0) == 0 );
    test_pass("cxx20:compare_three_way copyable");
}

TEST_CASE(compare, strong_weak_partial_order) {

    test_true( STD::strong_order(1, 2) < 0 );
    test_true( STD::strong_order(2, 2) == 0 );
    test_true( (STD::is_same<decltype(STD::strong_order(1, 2)), STD::strong_ordering>::value) );
    test_pass("cxx20:strong_order");

    test_true( STD::strong_order(1.0, 2.0) < 0 );
    test_true( STD::strong_order(-0.0, 0.0) < 0 );
    test_true( (-0.0 <=> 0.0) == 0 );
    test_true( (STD::is_same<decltype(STD::strong_order(1.0, 2.0)), STD::strong_ordering>::value) );
    double nan = STD::numeric_limits<double>::quiet_NaN();
    test_true( STD::strong_order(1.0, nan) < 0 );
    test_true( STD::strong_order(1.0f, 2.0f) < 0 );
    test_true( STD::strong_order(Strong{1}, Strong{2}) < 0 );
    test_pass("cxx20:strong_order (floating totalOrder)");

    test_true( STD::weak_order(1, 2) < 0 );
    test_true( STD::weak_order(1.0, 2.0) < 0 );
    test_true( STD::weak_order(-0.0, 0.0) == 0 );
    test_true( (STD::is_same<decltype(STD::weak_order(1.0, 2.0)), STD::weak_ordering>::value) );
    test_true( STD::weak_order(Weak{5}, Weak{7}) == 0 );
    test_pass("cxx20:weak_order");

    test_true( STD::partial_order(1, 2) < 0 );
    test_true( STD::partial_order(1.0, 2.0) < 0 );
    test_true( STD::partial_order(1.0, nan) == STD::partial_ordering::unordered );
    test_true( (STD::is_same<decltype(STD::partial_order(1.0, 2.0)), STD::partial_ordering>::value) );
    test_true( STD::partial_order(STD::string("a"), STD::string("b")) < 0 );
    test_pass("cxx20:partial_order");

    test_true( !has_strong_order<OnlyLt> );
    test_true( !has_weak_order<OnlyLt> );
    test_true( !has_partial_order<OnlyLt> );
    test_true(  has_strong_order<int> );
    test_true(  has_weak_order<double> );
    test_true(  has_partial_order<double> );
    test_pass("cxx20:order CPO SFINAE");
}

TEST_CASE(compare, order_fallbacks) {

    test_true( STD::compare_strong_order_fallback(1, 2) < 0 );
    test_true( STD::compare_weak_order_fallback(1, 2) < 0 );
    test_true( STD::compare_partial_order_fallback(1, 2) < 0 );
    test_pass("cxx20:compare order fallbacks (with <=>)");

    OnlyLt a{1}, b{2}, c{1};
    test_true( STD::compare_strong_order_fallback(a, b) < 0 );
    test_true( STD::compare_strong_order_fallback(b, a) > 0 );
    test_true( STD::compare_strong_order_fallback(a, c) == 0 );
    test_true( (STD::is_same<decltype(STD::compare_strong_order_fallback(a, b)),
                             STD::strong_ordering>::value) );
    test_pass("cxx20:compare_strong_order_fallback");

    test_true( STD::compare_weak_order_fallback(a, b) < 0 );
    test_true( STD::compare_weak_order_fallback(a, c) == 0 );
    test_true( (STD::is_same<decltype(STD::compare_weak_order_fallback(a, b)),
                             STD::weak_ordering>::value) );
    test_pass("cxx20:compare_weak_order_fallback");

    test_true( STD::compare_partial_order_fallback(a, b) < 0 );
    test_true( STD::compare_partial_order_fallback(a, c) == 0 );
    test_true( (STD::is_same<decltype(STD::compare_partial_order_fallback(a, b)),
                             STD::partial_ordering>::value) );
    test_pass("cxx20:compare_partial_order_fallback");

    test_true( STD::compare_strong_order_fallback(-0.0, 0.0) < 0 );
    test_pass("cxx20:compare_strong_order_fallback (floating)");
    double nan = STD::numeric_limits<double>::quiet_NaN();
    test_true( STD::compare_partial_order_fallback(1.0, nan) == STD::partial_ordering::unordered );
    test_pass("cxx20:compare_partial_order_fallback (unordered)");

    test_true(  has_strong_fb<OnlyLt> );
    test_true(  has_weak_fb<OnlyLt> );
    test_true(  has_partial_fb<OnlyLt> );

    test_true( !has_strong_fb<None> );
    test_true( !has_weak_fb<None> );
    test_true( !has_partial_fb<None> );
    test_pass("cxx20:order fallback SFINAE");

    const auto& cpo = STD::compare_strong_order_fallback;
    test_true( cpo(1, 2) < 0 );
    test_pass("cxx20:order fallback CPO object");
}

TEST_CASE(compare, type_order_cxx26) {
#if defined(__cpp_lib_type_order)
    test_true( STD::type_order<int, int>::value == STD::strong_ordering::equal );
    test_true( (STD::type_order<int, long>::value < 0) !=
               (STD::type_order<long, int>::value < 0) );
    test_pass("cxx26:type_order");

    test_true( STD::type_order_v<char, char> == STD::strong_ordering::equal );
    test_true( STD::type_order_v<int, long> == STD::type_order<int, long>::value );
    test_pass("cxx26:type_order_v");
#else
    TEST_NOTE("type_order is C++26 (P2830); no library here defines it yet");
    test_skip("cxx26:type_order");
    test_skip("cxx26:type_order_v");
#endif
}
#else
TEST_CASE_SKIP(compare, strong_ordering_constants)
TEST_CASE_SKIP(compare, weak_ordering_constants)
TEST_CASE_SKIP(compare, partial_ordering_constants)
TEST_CASE_SKIP(compare, is_eq_family)
TEST_CASE_SKIP(compare, common_comparison_category)
TEST_CASE_SKIP(compare, compare_three_way_result)
TEST_CASE_SKIP(compare, three_way_comparable_concepts)
TEST_CASE_SKIP(compare, compare_three_way_object)
TEST_CASE_SKIP(compare, strong_weak_partial_order)
TEST_CASE_SKIP(compare, order_fallbacks)
TEST_CASE(compare, type_order_cxx26) {
    test_skip("cxx26:type_order");
    test_skip("cxx26:type_order_v");
}
#endif


#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_three_way_comparison)
TEST_CASE(compare, ordering_constants_named_cxx20) {
    test_true( STD::partial_ordering::less < 0 );
    test_true( !(STD::partial_ordering::less == 0) );
    test_true( (1 <=> 2) == STD::partial_ordering::less );
    test_pass("cxx20:partial_ordering::less");

    test_true( STD::partial_ordering::equivalent == 0 );
    test_true( !(STD::partial_ordering::equivalent < 0) );
    test_pass("cxx20:partial_ordering::equivalent");

    test_true( STD::partial_ordering::greater > 0 );
    test_true( (2 <=> 1) == STD::partial_ordering::greater );
    test_pass("cxx20:partial_ordering::greater");

    test_true( !(STD::partial_ordering::unordered == 0) );
    test_true( !(STD::partial_ordering::unordered < 0) );
    test_true( !(STD::partial_ordering::unordered > 0) );

    test_true( STD::weak_ordering::less < 0 );
    test_true( STD::partial_ordering(STD::weak_ordering::less) ==
               STD::partial_ordering::less );
    test_pass("cxx20:weak_ordering::less");

    test_true( STD::weak_ordering::equivalent == 0 );
    test_pass("cxx20:weak_ordering::equivalent");

    test_true( STD::weak_ordering::greater > 0 );
    test_pass("cxx20:weak_ordering::greater");
}
#else
TEST_CASE(compare, ordering_constants_named_cxx20) {
    test_skip("cxx20:partial_ordering::less");
    test_skip("cxx20:partial_ordering::equivalent");
    test_skip("cxx20:partial_ordering::greater");
    test_skip("cxx20:weak_ordering::less");
    test_skip("cxx20:weak_ordering::equivalent");
    test_skip("cxx20:weak_ordering::greater");
}
#endif
