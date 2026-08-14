#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2017
#include <variant>
#include <string>
#include <type_traits>
#include <vector>

TEST_CASE(variant, construct_index) {
    STD::variant<int, STD::string> v;
    test_true( !v.valueless_by_exception() );
    test_eq( STD::get<0>(v), 0 );

    STD::variant<int, STD::string> a = 42;
    STD::variant<int, STD::string> b = STD::string("hi");
    STD::variant<int, STD::string> c(a);
    test_eq( STD::get<int>(c), 42 );
    test_pass("cxx17:variant construction");

    test_eq( (int)v.index(), 0 );
    test_eq( (int)a.index(), 0 );
    test_eq( (int)b.index(), 1 );
    c = b;
    test_eq( (int)c.index(), 1 );
    test_true( STD::variant_npos == (STD::size_t)-1 );
    test_pass("cxx17:variant index");
}

TEST_CASE(variant, access) {
    STD::variant<int, STD::string> v = 42;
    test_true( STD::holds_alternative<int>(v) );
    test_true( !STD::holds_alternative<STD::string>(v) );
    test_pass("cxx17:holds_alternative");

    test_eq( STD::get<int>(v), 42 );
    test_eq( STD::get<0>(v), 42 );
    STD::get<int>(v) = 7;
    test_eq( STD::get<0>(v), 7 );

    const STD::variant<int, STD::string>& cr = v;
    test_eq( STD::get<int>(cr), 7 );
    test_eq( STD::get<0>(cr), 7 );

    v = STD::string("hello");
    test_eq( STD::get<STD::string>(v), STD::string("hello") );
    test_eq( STD::get<1>(v), STD::string("hello") );
    test_throw( (void)STD::get<int>(v) );
    test_pass("cxx17:get");

    test_true( STD::get_if<int>(&v) == 0 );
    test_ptr( STD::get_if<STD::string>(&v) );
    test_eq( *STD::get_if<STD::string>(&v), STD::string("hello") );
    test_true( STD::get_if<int>((STD::variant<int, STD::string>*)0) == 0 );
    test_pass("cxx17:get_if");

    {   const STD::variant<int, STD::string>& cv = v;
        test_true( STD::get_if<int>(&cv) == 0 );
        test_ptr( (void*)STD::get_if<STD::string>(&cv) );
        test_eq( *STD::get_if<STD::string>(&cv), STD::string("hello") );
        test_true( STD::get_if<STD::string>((const STD::variant<int, STD::string>*)0) == 0 ); }
    test_pass("cxx17:get_if<I>(const variant*)");
}

TEST_CASE(variant, emplace_traits) {
    STD::variant<int, STD::string> v;
    int& ri = v.emplace<int>(5);
    test_eq( ri, 5 );
    test_eq( (int)v.index(), 0 );
    STD::string& rs = v.emplace<STD::string>(3, 'q');
    test_eq( rs, STD::string("qqq") );
    test_eq( (int)v.index(), 1 );
    test_pass("cxx17:variant emplace");

    typedef STD::variant<char, int, double> V3;
    test_eq( (int)STD::variant_size<V3>::value, 3 );
    test_true( (STD::is_same<STD::variant_alternative<0, V3>::type, char>::value) );
    test_true( (STD::is_same<STD::variant_alternative<2, V3>::type, double>::value) );
    test_pass("cxx17:variant traits");
}

TEST_CASE(variant, monostate_swap_compare) {
    STD::variant<STD::monostate, int> m;
    test_eq( (int)m.index(), 0 );
    test_true( STD::holds_alternative<STD::monostate>(m) );

    test_true( STD::monostate() <= STD::monostate() );
    test_true( !(STD::monostate() < STD::monostate()) );

    STD::hash<STD::monostate> hm;
    test_eq( hm(STD::monostate()), hm(STD::monostate()) );
    test_true( hm(STD::monostate()) != 0 );
    test_pass("cxx17:monostate");

    STD::variant<int, double> a = 1, b = 1, c = 2;
    test_true( a == b );
    test_true( a != c );

    test_true( a <  c );
    test_true( c >  a );
    test_true( a <= b );
    test_true( a >= b );

    STD::variant<int, double> di = 100;
    STD::variant<int, double> dd = 1.0;
    test_true( di < dd );
    test_true( dd > di );
    test_pass("cxx17:comparison");

    STD::variant<int, STD::string> x = 1, y = STD::string("z");
    x.swap(y);
    test_eq( (int)x.index(), 1 );
    test_eq( (int)y.index(), 0 );
    test_eq( STD::get<STD::string>(x), STD::string("z") );
    test_pass("cxx17:swap");
}

TEST_CASE(variant, relational_operators_cxx17) {
    typedef STD::variant<int, double> V;
    V one = 1, one2 = 1, two = 2;
    V big_int = 100;
    V small_dbl = 1.0;

    test_true( one == one2 );
    test_true( !(one == two) );
    test_true( !(big_int == small_dbl) );
    test_pass("cxx17:operator==(variant,variant)");

    test_true( one != two );
    test_true( !(one != one2) );
    test_true( big_int != small_dbl );
    test_pass("cxx17:operator!=(variant,variant)");

    test_true( one < two );
    test_true( !(two < one) );
    test_true( !(one < one2) );
    test_true( big_int < small_dbl );
    test_pass("cxx17:operator<(variant,variant)");

    test_true( one <= two );
    test_true( one <= one2 );
    test_true( !(two <= one) );
    test_pass("cxx17:operator<=(variant,variant)");

    test_true( two > one );
    test_true( !(one > two) );
    test_true( !(one > one2) );
    test_true( small_dbl > big_int );
    test_pass("cxx17:operator>(variant,variant)");

    test_true( two >= one );
    test_true( one >= one2 );
    test_true( !(one >= two) );
    test_pass("cxx17:operator>=(variant,variant)");
}

TEST_CASE(variant, special_members_cxx17) {
    typedef STD::variant<int, STD::string> V;

    test_eq( (int)STD::variant_size<V>::value, 2 );
    test_true(( STD::is_same<STD::variant_alternative<0, V>::type, int>::value ));
    test_true(( STD::is_same<STD::variant_alternative<1, V>::type, STD::string>::value ));
    test_true( sizeof(V) >= sizeof(STD::string) );
    test_pass("cxx17:variant");

    V d;
    test_eq( (int)d.index(), 0 );
    test_eq( STD::get<0>(d), 0 );
    test_true( !d.valueless_by_exception() );
    test_pass("cxx17:variant::variant()");

    V src = STD::string("hello");
    V copy(src);
    test_eq( (int)copy.index(), 1 );
    test_eq( STD::get<1>(copy), STD::string("hello") );
    test_eq( STD::get<1>(src), STD::string("hello") );
    test_true( &STD::get<1>(copy) != &STD::get<1>(src) );
    test_pass("cxx17:variant::variant(const variant&)");

    V from_int = 7;
    test_eq( (int)from_int.index(), 0 );
    test_eq( STD::get<0>(from_int), 7 );
    V from_literal = "abc";
    test_eq( (int)from_literal.index(), 1 );
    test_eq( STD::get<1>(from_literal), STD::string("abc") );
    test_pass("cxx17:variant::variant(T&&)");

    V target = 1;
    target = src;
    test_eq( (int)target.index(), 1 );
    test_eq( STD::get<1>(target), STD::string("hello") );
    target = from_int;
    test_eq( (int)target.index(), 0 );
    test_eq( STD::get<0>(target), 7 );
    test_pass("cxx17:variant::operator=(const variant&)");

    V conv = 1;
    conv = STD::string("z");
    test_eq( (int)conv.index(), 1 );
    test_eq( STD::get<1>(conv), STD::string("z") );
    conv = 5;
    test_eq( (int)conv.index(), 0 );
    test_eq( STD::get<0>(conv), 5 );
    test_pass("cxx17:variant::operator=(T&&)");
}

namespace {
    struct VarVisit {
        int operator()(int x) const { return x; }
        int operator()(const STD::string& s) const { return (int)s.size(); }
        int operator()(const STD::vector<int>& v) const { return (int)v.size(); }
    };
}

TEST_CASE(variant, ctors_emplace) {
    typedef STD::variant<int, STD::string, STD::vector<int> > V;

    V a(STD::string("moved"));
    V b(STD::move(a));
    test_true( b.index() == 1 && STD::get<1>(b) == "moved" );
    test_pass("cxx17:variant::variant(variant&&) (move)");
    V c;
    c = STD::move(b);
    test_true( c.index() == 1 && STD::get<1>(c) == "moved" );
    test_pass("cxx17:variant::operator=(variant&&)");

#if _TST_HAS_VAR_TEMPLATE && _TST_HAS_INIT_LIST

    V vi(STD::in_place_index<0>, 5);
    test_true( vi.index() == 0 && STD::get<0>(vi) == 5 );
    test_pass("cxx17:variant::variant(in_place_index_t<I>, Args...)");
    V vil(STD::in_place_index<2>, { 1, 2, 3 });
    test_true( vil.index() == 2 && STD::get<2>(vil).size() == 3 );
    test_pass("cxx17:variant::variant(in_place_index_t<I>, initializer_list, Args...)");

    V vt(STD::in_place_type<STD::string>, "hi");
    test_true( vt.index() == 1 && STD::get<1>(vt) == "hi" );
    test_pass("cxx17:variant::variant(in_place_type_t<T>, Args...)");
    V vtl(STD::in_place_type<STD::vector<int> >, { 4, 5 });
    test_true( vtl.index() == 2 && STD::get<2>(vtl).size() == 2 );
    test_pass("cxx17:variant::variant(in_place_type_t<T>, initializer_list, Args...)");

    V e;
    e.emplace<0>(9);
    test_true( STD::get<0>(e) == 9 );
    test_pass("cxx17:variant::emplace<I>(Args...)");
    e.emplace<2>({ 1, 2, 3 });
    test_true( STD::get<2>(e).size() == 3 );
    test_pass("cxx17:variant::emplace<I>(initializer_list, Args...)");
    e.emplace<STD::vector<int> >({ 4, 5, 6, 7 });
    test_true( STD::get<2>(e).size() == 4 );
    test_pass("cxx17:variant::emplace<T>(initializer_list, Args...)");
#else
    test_skip("cxx17:variant::variant(in_place_index_t<I>, Args...)");
    test_skip("cxx17:variant::variant(in_place_index_t<I>, initializer_list, Args...)");
    test_skip("cxx17:variant::variant(in_place_type_t<T>, Args...)");
    test_skip("cxx17:variant::variant(in_place_type_t<T>, initializer_list, Args...)");
    test_skip("cxx17:variant::emplace<I>(Args...)");
    test_skip("cxx17:variant::emplace<I>(initializer_list, Args...)");
    test_skip("cxx17:variant::emplace<T>(initializer_list, Args...)");
#endif

    { V d(STD::string("bye")); (void)d; }
    test_true( true );
    test_pass("cxx17:variant::~variant");

#if !defined(__WATCOMC__)
    STD::in_place_index_t<0> ti; (void)ti;
    STD::in_place_type_t<int> tt; (void)tt;
    test_true( true );
    test_pass("cxx17:in_place_index_t");
#else
    TEST_SKIP1();
    test_skip("cxx17:in_place_index_t");
#endif
    test_pass("cxx17:in_place_type_t");
}

TEST_CASE(variant, access_visit_traits) {
    typedef STD::variant<int, STD::string, STD::vector<int> > V;
    V v(STD::string("abcd"));

#if _TST_HAS_ALIAS_TEMPLATE && !defined(__WATCOMC__) && _TST_HAS_CXX17_LIB_MEMBERS
    test_true( STD::get_if<0>(&v) == 0 );
    test_true( STD::get_if<1>(&v) != 0 && *STD::get_if<1>(&v) == "abcd" );
    test_pass("cxx17:get_if<I>(variant*)");
#else
    TEST_SKIP_N(2);
    test_skip("cxx17:get_if<I>(variant*)");
#endif

#if _TST_HAS_ALIAS_TEMPLATE && !defined(__WATCOMC__) && _TST_HAS_CXX17_LIB_MEMBERS
    test_eq( STD::visit(VarVisit(), v), 4 );
    test_pass("cxx17:visit");
#else
    TEST_SKIP1();
    test_skip("cxx17:visit");
#endif

    V a(1), b(STD::string("x"));
    STD::swap(a, b);
    test_true( a.index() == 1 && b.index() == 0 );
    test_pass("cxx17:swap(variant) (non-member)");

#if !defined(__WATCOMC__)
    typedef STD::variant<int, STD::string> HV;
    HV h1(42), h2(42);
    test_true( STD::hash<HV>()(h1) == STD::hash<HV>()(h2) );
    test_pass("cxx17:hash<variant>");
#else
    TEST_SKIP1();
    test_skip("cxx17:hash<variant>");
#endif

#if _TST_HAS_VAR_TEMPLATE
    int vs = (int)STD::variant_size_v<V>;
    test_eq( vs, 3 );
    test_pass("cxx17:variant_size_v");
    test_true(( STD::is_same<STD::variant_alternative_t<1, V>, STD::string>::value ));
    test_pass("cxx17:variant_alternative_t");
#else
    test_skip("cxx17:variant_size_v");
    test_skip("cxx17:variant_alternative_t");
#endif

#if TEST_TARGET_CXX >= 2020 && !defined(__WATCOMC__) && _TST_HAS_CXX20_LIB_MEMBERS

    long r = STD::visit<long>(VarVisit(), v);
    test_eq( r, 4L );
    test_pass("cxx20:visit<R>");
#else
    test_skip("cxx20:visit<R>");
#endif

    test_skip("cxx26:variant::visit (member)");
}

TEST_CASE(variant, cxx20_spaceship) {
#if defined(__cpp_lib_three_way_comparison)
    STD::variant<int, STD::string> l(1), h(2);
    test_true( (l <=> h) < 0 && (l <=> l) == 0 );
    test_pass("cxx20:operator<=>(variant, variant)");
    STD::monostate m1, m2;
    test_true( (m1 <=> m2) == 0 );
    test_pass("cxx20:monostate operator<=>");
#else
    test_skip("cxx20:operator<=>(variant, variant)");
    test_skip("cxx20:monostate operator<=>");
#endif
}
#endif


#if TEST_TARGET_CXX >= 2017
#if TEST_HAS_EH
namespace {
struct CcwThrowOnCopy {
    CcwThrowOnCopy() {}
    CcwThrowOnCopy(const CcwThrowOnCopy&) { throw 7; }
    CcwThrowOnCopy& operator=(const CcwThrowOnCopy&) { throw 7; return *this; }
};
}
#endif

#if TEST_HAS_EH
TEST_CASE(variant, helper_types_cxx17) {
    typedef STD::variant<int, STD::string, double> V;

    test_eq( (int)STD::variant_size<V>::value, 3 );
    test_eq( (int)STD::variant_size<const V>::value, 3 );
    test_pass("cxx17:variant_size");

    test_true(( STD::is_same<STD::variant_alternative<0, V>::type, int>::value ));
    test_true(( STD::is_same<STD::variant_alternative<1, V>::type, STD::string>::value ));
    test_true(( STD::is_same<STD::variant_alternative<2, V>::type, double>::value ));
    test_pass("cxx17:variant_alternative");

    test_true( STD::variant_npos == (STD::size_t)-1 );
    test_pass("cxx17:variant_npos");

    STD::variant<int, CcwThrowOnCopy> w;
    test_true( !w.valueless_by_exception() );
    CcwThrowOnCopy boom;
    bool threw = false;
    try { w = boom; } catch (int) { threw = true; }
    test_true( threw );
    test_true( w.valueless_by_exception() );
    test_true( w.index() == STD::variant_npos );
    test_pass("cxx17:variant::valueless_by_exception");

    V v(42);
    bool caught = false;
    try { (void)STD::get<STD::string>(v); }
    catch (const STD::bad_variant_access& e) { caught = (e.what() != 0); }
    test_true( caught );
    test_pass("cxx17:bad_variant_access");
}
#else   // !TEST_HAS_EH
TEST_CASE(variant, helper_types_cxx17) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx17:variant_size");
    test_skip("cxx17:variant_alternative");
    test_skip("cxx17:variant_npos");
    test_skip("cxx17:variant::valueless_by_exception");
    test_skip("cxx17:bad_variant_access");
}
#endif  // TEST_HAS_EH
#else
TEST_CASE(variant, helper_types_cxx17) {
    test_skip("cxx17:variant_size");
    test_skip("cxx17:variant_alternative");
    test_skip("cxx17:variant_npos");
    test_skip("cxx17:variant::valueless_by_exception");
    test_skip("cxx17:bad_variant_access");
}
#endif

TEST_CASE(variant, feature_test_macros) {
#if defined(__cpp_lib_variant) && (__cpp_lib_variant) >= 201606L
    test_true( (__cpp_lib_variant) >= 201606L );
    test_pass("cxx17:__cpp_lib_variant=201606L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_variant=201606L");
#endif
#if defined(__cpp_lib_variant) && (__cpp_lib_variant) >= 202106L
    test_true( (__cpp_lib_variant) >= 202106L );
    test_pass("cxx23:__cpp_lib_variant=202106L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_variant=202106L");
#endif
#if defined(__cpp_lib_variant) && (__cpp_lib_variant) >= 202306L
    test_true( (__cpp_lib_variant) >= 202306L );
    test_pass("cxx26:__cpp_lib_variant=202306L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_variant=202306L");
#endif
}
