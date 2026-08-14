#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <optional>
#include <string>
#include <type_traits>

TEST_CASE(optional, construct) {
    STD::optional<int> a;
    test_true( !a.has_value() );
    test_true( !a );

    STD::optional<int> b(5);
    test_true( b.has_value() );
    test_true( (bool)b );
    test_eq( *b, 5 );

    STD::optional<int> n(STD::nullopt);
    test_true( !n.has_value() );

    STD::optional<int> c(b);
    test_eq( *c, 5 );

    STD::optional<STD::string> ip(STD::in_place, 3, 'x');
    test_eq( *ip, STD::string("xxx") );
    STD::optional<STD::string> ip0(STD::in_place);
    test_eq( *ip0, STD::string() );
    test_pass("cxx17:optional construction");
}

TEST_CASE(optional, assign_observe) {
    STD::optional<int> o;
    o = 7;
    test_eq( *o, 7 );
    test_eq( o.value(), 7 );
    test_eq( *o.operator->(), 7 );
    *o = 8;
    test_eq( o.value(), 8 );

    STD::optional<int> o2;
    o2 = o;
    test_eq( o2.value(), 8 );
    o2 = STD::nullopt;
    test_true( !o2.has_value() );
    test_eq( o2.value_or(99), 99 );
    test_eq( o.value_or(99), 8 );
    test_pass("cxx17:optional assignment");

    const STD::optional<int>& cr = o;
    test_eq( *cr, 8 );
    test_eq( cr.value(), 8 );
    test_eq( *cr.operator->(), 8 );

    o.reset();
    test_true( !o.has_value() );
    test_throw( (void)o.value() );
    test_pass("cxx17:optional observers");
}

TEST_CASE(optional, modify) {
    STD::optional<STD::string> s;
    STD::string& r = s.emplace(4, 'z');
    test_eq( r, STD::string("zzzz") );
    test_eq( *s, STD::string("zzzz") );
    s.emplace();
    test_eq( *s, STD::string() );
    test_pass("cxx17:optional::emplace");

    STD::optional<int> x = 5, y = 9;
    x.swap(y);
    test_eq( *x, 9 );
    test_eq( *y, 5 );
    test_pass("cxx17:optional::swap");

    STD::optional<int> mo = STD::make_optional(42);
    test_eq( *mo, 42 );
    test_pass("cxx17:optional make_optional");
}

TEST_CASE(optional, compare) {
    STD::optional<int> a = 1, b = 1, c = 2, none;

    test_true( a == b );
    test_true( a != c );
    test_true( a <  c );
    test_true( c >  a );
    test_true( a <= b );
    test_true( a >= b );

    test_true( none == STD::nullopt );
    test_true( STD::nullopt == none );
    test_true( a != STD::nullopt );
    test_true( none < a );
    test_true( a > STD::nullopt );
    test_true( none <= a );
    test_true( a >= STD::nullopt );

    test_true( a == 1 );
    test_true( 1 == a );
    test_true( a != 2 );
    test_true( a < 2 );
    test_true( 0 < a );
    test_true( a <= 1 );
    test_true( a >= 1 );
    test_true( none < 5 );
    test_pass("cxx17:optional comparisons");
}

TEST_CASE(optional, hash) {
    STD::optional<int> a(42);
    STD::optional<int> b(42);
    STD::optional<int> e;
    STD::hash<STD::optional<int> > h;
    STD::size_t ha = h(a), hb = h(b);
    test_eq( ha, hb );
    test_eq( ha, STD::hash<int>()(42) );
    STD::optional<int> e2;
    test_eq( h(e), h(e2) );
    test_pass("cxx17:optional hash");
}

namespace {
struct OptCounter {
    static int live;
    int v;
    OptCounter() : v(0) { ++live; }
    explicit OptCounter(int x) : v(x) { ++live; }
    OptCounter(const OptCounter& o) : v(o.v) { ++live; }
    OptCounter& operator=(const OptCounter& o) { v = o.v; return *this; }
    ~OptCounter() { --live; }
};
int OptCounter::live = 0;
}

TEST_CASE(optional, ctors_assign_dtor) {
    {
        STD::optional<STD::string> a(STD::string("moved"));
        STD::optional<STD::string> b(STD::move(a));
        test_true( b.has_value() );
        test_eq( *b, STD::string("moved") );
        test_pass("cxx17:optional::optional(optional&&) (move)");
    }
    {
        STD::optional<int> a(5);
        STD::optional<long> b(a);
        test_true( b.has_value() );
        test_eq( *b, 5L );
        test_pass("cxx17:optional::optional(const optional<U>&) (converting)");
    }
    {
        STD::optional<int> a(6);
        STD::optional<long> b(STD::move(a));
        test_eq( *b, 6L );
        test_pass("cxx17:optional::optional(optional<U>&&) (converting)");
    }
    {
        STD::optional<STD::string> a(STD::string("src"));
        STD::optional<STD::string> b;
        b = STD::move(a);
        test_eq( *b, STD::string("src") );
        test_pass("cxx17:optional::operator=(optional&&)");
    }
    {
        STD::optional<int> a(7);
        STD::optional<long> b;
        b = a;
        test_eq( *b, 7L );
        test_pass("cxx17:optional::operator=(const optional<U>&)");
    }
    {
        STD::optional<int> a(8);
        STD::optional<long> b;
        b = STD::move(a);
        test_eq( *b, 8L );
        test_pass("cxx17:optional::operator=(optional<U>&&)");
    }
    {
        int before = OptCounter::live;
        {
            STD::optional<OptCounter> o(OptCounter(3));
            test_eq( OptCounter::live, before + 1 );
        }
        test_eq( OptCounter::live, before );
        test_pass("cxx17:optional::~optional");
    }
    {
        STD::optional<int> a(1), b(2);
        STD::swap(a, b);
        test_eq( *a, 2 );
        test_eq( *b, 1 );
        test_pass("cxx17:swap(optional) (non-member)");
    }
    {
        STD::optional<STD::string> s = STD::make_optional<STD::string>((STD::size_t)3, 'x');
        test_eq( *s, STD::string("xxx") );
        test_pass("cxx17:make_optional(in_place, Args...)");
    }
}

#if _tst_cplusplus >= 201103L
#include <vector>
TEST_CASE(optional, init_list_forms) {
    {
        STD::optional<STD::vector<int> > o(STD::in_place, { 1, 2, 3 });
        test_eq( o->size(), 3u );
        test_eq( (*o)[2], 3 );
        test_pass("cxx17:optional::optional(in_place_t, initializer_list, Args...)");
    }
    {
        STD::optional<STD::vector<int> > o;
        o.emplace({ 4, 5 });
        test_eq( o->size(), 2u );
        test_eq( (*o)[0], 4 );
        test_pass("cxx17:optional::emplace(initializer_list, Args...)");
    }
}
#else
TEST_CASE_SKIP(optional, init_list_forms)
#endif

TEST_CASE(optional, spaceship) {
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    STD::optional<int> a(1), b(2), e;
    test_true( (a <=> a) == 0 );
    test_true( (a <=> b) <  0 );
    test_true( (e <=> a) <  0 );
    test_true( (e <=> e) == 0 );
    test_pass("cxx20:operator<=>(optional, optional)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>(optional, optional)");
#endif
}

TEST_CASE(optional, monadic) {
#if defined(__cpp_lib_optional) && __cpp_lib_optional >= 202110L
    struct Half { STD::optional<int> operator()(int x) const { return STD::optional<int>(x / 2); } };
    struct Dbl  { int operator()(int x) const { return x * 2; } };
    struct Zero { STD::optional<int> operator()() const { return STD::optional<int>(0); } };

    STD::optional<int> a(10), e;
    test_eq( *a.and_then(Half()), 5 );
    test_true( !e.and_then(Half()).has_value() );
    test_pass("cxx23:optional::and_then");

    test_eq( *a.transform(Dbl()), 20 );
    test_true( !e.transform(Dbl()).has_value() );
    test_pass("cxx23:optional::transform");

    test_eq( *e.or_else(Zero()), 0 );
    test_eq( *a.or_else(Zero()), 10 );
    test_pass("cxx23:optional::or_else");
#else
    test_skip("cxx23:optional::and_then");
    test_skip("cxx23:optional::transform");
    test_skip("cxx23:optional::or_else");
#endif
}
#endif


#if TEST_TARGET_CXX >= 2017
#if TEST_HAS_EH
TEST_CASE(optional, core_observers_cxx17) {
    STD::optional<int> e;
    STD::optional<int> v(42);

    test_true( !e.has_value() );
    test_true( v.has_value() );
    test_true( (bool)v == v.has_value() );
    test_pass("cxx17:optional::has_value");

    test_eq( *v, 42 );
    *v = 7;
    test_eq( *v, 7 );
    const STD::optional<int>& cv = v;
    test_eq( *cv, 7 );
    test_pass("cxx17:optional::operator*");

    STD::optional<STD::string> s(STD::string("abc"));
    test_eq( s->size(), (STD::size_t)3 );
    s->append("d");
    test_true( *s == "abcd" );
    test_pass("cxx17:optional::operator->");

    v.reset();
    test_true( !v.has_value() );
    v.reset();
    test_true( !v.has_value() );
    test_pass("cxx17:optional::reset");

    test_eq( e.value_or(9), 9 );
    STD::optional<int> f(3);
    test_eq( f.value_or(9), 3 );
    test_pass("cxx17:optional::value_or");

    test_true(( STD::is_same<STD::optional<int>::value_type, int>::value ));
    test_true(( STD::is_same<STD::optional<STD::string>::value_type, STD::string>::value ));
    test_pass("cxx17:optional::value_type");

    STD::optional<int> g(5);
    g = STD::nullopt;
    test_true( !g.has_value() );
    const STD::nullopt_t* nullopt_p = &STD::nullopt;
    test_true( nullopt_p != 0 );
    test_pass("cxx17:nullopt");

    STD::optional<int> h(STD::nullopt);
    test_true( !h.has_value() );
    test_pass("cxx17:nullopt_t");

    bool caught = false;
    try { (void)e.value(); }
    catch (const STD::bad_optional_access& ex) { caught = (ex.what() != 0); }
    test_true( caught );
    test_pass("cxx17:bad_optional_access");
}
#else   // !TEST_HAS_EH
TEST_CASE(optional, core_observers_cxx17) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx17:optional::has_value");
    test_skip("cxx17:optional::operator*");
    test_skip("cxx17:optional::operator->");
    test_skip("cxx17:optional::reset");
    test_skip("cxx17:optional::value_or");
    test_skip("cxx17:optional::value_type");
    test_skip("cxx17:nullopt");
    test_skip("cxx17:nullopt_t");
    test_skip("cxx17:bad_optional_access");
}
#endif  // TEST_HAS_EH

#if defined(__cpp_lib_optional_range_support)
TEST_CASE(optional, range_support_cxx26) {
    STD::optional<int> e;
    STD::optional<int> v(42);

    test_true( e.begin() == e.end() );
    test_true( v.begin() != v.end() );
    test_pass("cxx26:optional::begin");

    test_eq( (int)(v.end() - v.begin()), 1 );
    test_eq( (int)(e.end() - e.begin()), 0 );
    test_pass("cxx26:optional::end");

    STD::optional<int>::iterator it = v.begin();
    *it = 7;
    test_eq( *v, 7 );
    test_pass("cxx26:optional::iterator");

    const STD::optional<int>& cv = v;
    STD::optional<int>::const_iterator ci = cv.begin();
    test_eq( *ci, 7 );
    test_pass("cxx26:optional::const_iterator");
}
#else
TEST_CASE(optional, range_support_cxx26) {
    TEST_NOTE("optional as a range is C++26 (P3168); this library does not have it yet");
    test_skip("cxx26:optional::begin");
    test_skip("cxx26:optional::end");
    test_skip("cxx26:optional::iterator");
    test_skip("cxx26:optional::const_iterator");
}
#endif
#else
TEST_CASE(optional, core_observers_cxx17) {
    test_skip("cxx17:optional::has_value");
    test_skip("cxx17:optional::operator*");
    test_skip("cxx17:optional::operator->");
    test_skip("cxx17:optional::reset");
    test_skip("cxx17:optional::value_or");
    test_skip("cxx17:optional::value_type");
    test_skip("cxx17:nullopt");
    test_skip("cxx17:nullopt_t");
    test_skip("cxx17:bad_optional_access");
}
TEST_CASE(optional, range_support_cxx26) {
    test_skip("cxx26:optional::begin");
    test_skip("cxx26:optional::end");
    test_skip("cxx26:optional::iterator");
    test_skip("cxx26:optional::const_iterator");
}
#endif

#if TEST_TARGET_CXX >= 2017
#if TEST_HAS_EH
TEST_CASE(optional, members_cxx17) {
    typedef STD::optional<STD::string> OS;

    test_true(( STD::is_same<OS::value_type, STD::string>::value ));
    test_true( sizeof(OS) > sizeof(STD::string) );
    test_pass("cxx17:optional");

    STD::optional<int> d;
    test_true( !d.has_value() );
    test_true( !d );
    test_pass("cxx17:optional::optional()");

    STD::optional<int> n(STD::nullopt);
    test_true( !n.has_value() );
    test_true( n == d );
    test_pass("cxx17:optional::optional(nullopt_t)");

    OS src(STD::string("hello"));
    OS copy(src);
    test_true( copy.has_value() );
    test_eq( *copy, STD::string("hello") );
    test_true( &*copy != &*src );
    OS empty_src;
    OS empty_copy(empty_src);
    test_true( !empty_copy.has_value() );
    test_pass("cxx17:optional::optional(const optional&)");

    OS from_literal("abc");
    test_true( from_literal.has_value() );
    test_eq( *from_literal, STD::string("abc") );
    STD::optional<long> widened(42);
    test_eq( *widened, 42L );
    test_pass("cxx17:optional::optional(U&&)");

    OS built(STD::in_place, 3, 'x');
    test_eq( *built, STD::string("xxx") );
    OS defaulted(STD::in_place);
    test_true( defaulted.has_value() );
    test_eq( *defaulted, STD::string() );
    test_pass("cxx17:optional::optional(in_place_t,Args...)");

    OS o(STD::string("gone"));
    o = STD::nullopt;
    test_true( !o.has_value() );
    o = STD::nullopt;
    test_true( !o.has_value() );
    test_pass("cxx17:optional::operator=(nullopt_t)");

    OS a(STD::string("a")), b(STD::string("b")), e;
    a = b;    test_eq( *a, STD::string("b") );
    a = e;    test_true( !a.has_value() );
    a = b;    test_eq( *a, STD::string("b") );
    e = OS(); test_true( !e.has_value() );
    test_pass("cxx17:optional::operator=(const optional&)");

    STD::optional<int> v;
    v = 7;
    test_eq( *v, 7 );
    v = 8;
    test_eq( *v, 8 );
    test_true( v.has_value() );
    test_pass("cxx17:optional::operator=(U&&)");

    test_true( (bool)v == v.has_value() );
    test_true( (bool)e == e.has_value() );
    if (v) test_true( true ); else test_true( false );
    test_pass("cxx17:optional::operator bool");

    test_eq( v.value(), 8 );
    const STD::optional<int>& cv = v;
    test_eq( cv.value(), 8 );
    v.value() = 9;
    test_eq( *v, 9 );
    STD::optional<int> gone;
    test_throw( (void)gone.value() );
    bool caught = false;
    try { (void)gone.value(); }
    catch (const STD::bad_optional_access&) { caught = true; }
    catch (...) {}
    test_true( caught );
    test_pass("cxx17:optional::value");
}
#else   // !TEST_HAS_EH
TEST_CASE(optional, members_cxx17) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx17:optional");
    test_skip("cxx17:optional::optional()");
    test_skip("cxx17:optional::optional(nullopt_t)");
    test_skip("cxx17:optional::optional(const optional&)");
    test_skip("cxx17:optional::optional(U&&)");
    test_skip("cxx17:optional::optional(in_place_t,Args...)");
    test_skip("cxx17:optional::operator=(nullopt_t)");
    test_skip("cxx17:optional::operator=(const optional&)");
    test_skip("cxx17:optional::operator=(U&&)");
    test_skip("cxx17:optional::operator bool");
    test_skip("cxx17:optional::value");
}
#endif  // TEST_HAS_EH

TEST_CASE(optional, relational_operators_cxx17) {
    STD::optional<int> one(1), one2(1), two(2), none, none2;

    test_true( one == one2 );
    test_true( none == none2 );
    test_true( !(one == two) );
    test_true( !(one == none) );
    test_pass("cxx17:operator==(optional,optional)");

    test_true( one != two );
    test_true( one != none );
    test_true( !(none != none2) );
    test_pass("cxx17:operator!=(optional,optional)");

    test_true( one < two );
    test_true( none < one );
    test_true( !(one < none) );
    test_true( !(none < none2) );
    test_pass("cxx17:operator<(optional,optional)");

    test_true( one <= two );
    test_true( one <= one2 );
    test_true( none <= none2 );
    test_true( !(one <= none) );
    test_pass("cxx17:operator<=(optional,optional)");

    test_true( two > one );
    test_true( one > none );
    test_true( !(none > none2) );
    test_pass("cxx17:operator>(optional,optional)");

    test_true( two >= one );
    test_true( one >= one2 );
    test_true( none >= none2 );
    test_true( !(none >= one) );
    test_pass("cxx17:operator>=(optional,optional)");
}

TEST_CASE(optional, range_traits_cxx26) {
#if defined(__cpp_lib_optional_range_support) && (__cpp_lib_optional_range_support) >= 202406L
    test_true( STD::ranges::enable_view<STD::optional<int> > );
    test_true( STD::ranges::view<STD::optional<int> > );
    test_pass("cxx26:ranges::enable_view<optional<T>>");

    test_true( STD::format_kind<STD::optional<int> > == STD::range_format::disabled );
    test_pass("cxx26:format_kind<optional<T>>");
#else
    TEST_NOTE("optional is not a range here (P3168 needs __cpp_lib_optional_range_support)");
    test_skip("cxx26:ranges::enable_view<optional<T>>");
    test_skip("cxx26:format_kind<optional<T>>");
#endif
}
#else
TEST_CASE(optional, members_cxx17) {
    test_skip("cxx17:optional");
    test_skip("cxx17:optional::optional()");
    test_skip("cxx17:optional::optional(nullopt_t)");
    test_skip("cxx17:optional::optional(const optional&)");
    test_skip("cxx17:optional::optional(U&&)");
    test_skip("cxx17:optional::optional(in_place_t,Args...)");
    test_skip("cxx17:optional::operator=(nullopt_t)");
    test_skip("cxx17:optional::operator=(const optional&)");
    test_skip("cxx17:optional::operator=(U&&)");
    test_skip("cxx17:optional::operator bool");
    test_skip("cxx17:optional::value");
}
TEST_CASE(optional, relational_operators_cxx17) {
    test_skip("cxx17:operator==(optional,optional)");
    test_skip("cxx17:operator!=(optional,optional)");
    test_skip("cxx17:operator<(optional,optional)");
    test_skip("cxx17:operator<=(optional,optional)");
    test_skip("cxx17:operator>(optional,optional)");
    test_skip("cxx17:operator>=(optional,optional)");
}
TEST_CASE(optional, range_traits_cxx26) {
    test_skip("cxx26:ranges::enable_view<optional<T>>");
    test_skip("cxx26:format_kind<optional<T>>");
}
#endif

TEST_CASE(optional, feature_test_macros) {
#if defined(__cpp_lib_optional) && (__cpp_lib_optional) >= 201606L
    test_true( (__cpp_lib_optional) >= 201606L );
    test_pass("cxx17:__cpp_lib_optional=201606L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_optional=201606L");
#endif
#if defined(__cpp_lib_optional) && (__cpp_lib_optional) >= 202106L
    test_true( (__cpp_lib_optional) >= 202106L );
    test_pass("cxx23:__cpp_lib_optional=202106L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_optional=202106L");
#endif
#if defined(__cpp_lib_optional) && (__cpp_lib_optional) >= 202110L
    test_true( (__cpp_lib_optional) >= 202110L );
    test_pass("cxx23:__cpp_lib_optional=202110L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_optional=202110L");
#endif
#if defined(__cpp_lib_optional_range_support) && (__cpp_lib_optional_range_support) >= 202406L
    test_true( (__cpp_lib_optional_range_support) >= 202406L );
    test_pass("cxx26:__cpp_lib_optional_range_support=202406L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_optional_range_support=202406L");
#endif
}
