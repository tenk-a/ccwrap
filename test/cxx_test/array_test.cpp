#include "test_cxx.hpp"

#include <array>
#include <cstring>
#include <type_traits>

TEST_CASE(array, capacity_access) {
    STD::array<int, 5> a = { 1, 2, 3, 4, 5 };
    const STD::array<int, 5>& cr = a;

    test_eq( a.size(), 5u );
    test_pass("cxx11:array::size");
    test_eq( a.max_size(), 5u );
    test_pass("cxx11:array::max_size");
    test_true( !a.empty() );
    test_pass("cxx11:array::empty");

    test_eq( a[0], 1 );
    test_eq( cr[4], 5 );
    test_pass("cxx11:array::operator[]");
    test_eq( a.at(2), 3 );
    test_eq( cr.at(2), 3 );
    a[2] = 30;
    test_eq( a.at(2), 30 );
    a[2] = 3;
    test_throw( (void)a.at(99) );
    test_throw( (void)cr.at(99) );
    test_pass("cxx11:array::at");

    test_eq( a.front(), 1 );
    test_eq( cr.front(), 1 );
    test_pass("cxx11:array::front");
    test_eq( a.back(), 5 );
    test_eq( cr.back(), 5 );
    test_pass("cxx11:array::back");

    test_eq( *a.data(), 1 );
    test_eq( *cr.data(), 1 );
    test_pass("cxx11:array::data");
}

TEST_CASE(array, iterators) {
    STD::array<int, 4> a = { 10, 20, 30, 40 };
    const STD::array<int, 4>& cr = a;

    int sum = 0;
    for (STD::array<int, 4>::iterator it = a.begin(); it != a.end(); ++it) sum += *it;
    test_eq( sum, 100 );
    test_pass("cxx11:array::begin");

    sum = 0;
    for (STD::array<int, 4>::const_iterator it = cr.begin(); it != cr.end(); ++it) sum += *it;
    test_eq( sum, 100 );
    test_pass("cxx11:array::end");

    sum = 0;
    for (STD::array<int, 4>::const_iterator it = a.cbegin(); it != a.cend(); ++it) sum += *it;
    test_eq( sum, 100 );
    test_pass("cxx11:array::cbegin");

    test_eq( *a.rbegin(), 40 );
    test_eq( *cr.rbegin(), 40 );
    test_pass("cxx11:array::rbegin");
    test_eq( *a.crbegin(), 40 );
    test_pass("cxx11:array::crbegin");
    int first_rev = *(a.rend() - 1);
    test_eq( first_rev, 10 );
    test_pass("cxx11:array::rend");
    test_eq( *(cr.crend() - 1), 10 );
    test_pass("cxx11:array::crend");
}

TEST_CASE(array, modify_compare) {
    STD::array<int, 3> a = { 1, 2, 3 };
    a.fill(7);
    test_eq( a[0], 7 );
    test_eq( a[2], 7 );
    test_pass("cxx11:array::fill");

    STD::array<int, 3> x = { 1, 2, 3 };
    STD::array<int, 3> y = { 4, 5, 6 };
    x.swap(y);
    test_eq( x[0], 4 );
    test_eq( y[0], 1 );
    test_pass("cxx11:array::swap");
    using STD::swap;
    swap(x, y);
    test_eq( x[0], 1 );
    STD::swap(x, y);
    test_eq( x[0], 4 );
    test_pass("cxx11:swap");

    STD::array<int, 3> p = { 1, 2, 3 };
    STD::array<int, 3> q = { 1, 2, 3 };
    STD::array<int, 3> r = { 1, 2, 4 };
    test_true( p == q );
    test_true( p != r );
    test_pass("cxx11:operator==");
    test_true( p <  r );
    test_true( r >  p );
    test_true( p <= q );
    test_true( p >= q );
    test_pass("cxx11:operator<");

    STD::array<char, 6> s = { 'h', 'e', 'l', 'l', 'o', '\0' };
    test_eq( STD::strcmp(s.data(), "hello"), 0 );
    test_pass("cxx11:array::data#char");

    STD::array<int, 0> e;
    test_true( e.empty() );
    test_eq( e.size(), 0u );
    test_pass("cxx11:array#zero_size");
}

TEST_CASE(array, tuple_interface) {
    typedef STD::array<int, 5>    A5;
    typedef STD::array<char, 1>   A1;
    typedef STD::array<double, 3> AD3;

    STD::size_t n5 = STD::tuple_size<A5>::value;
    STD::size_t n1 = STD::tuple_size<A1>::value;
    test_eq( n5, 5u );
    test_eq( n1, 1u );
    test_pass("cxx11:tuple_size");

    bool e0 = STD::is_same<STD::tuple_element<0, AD3>::type, double>::value;
    bool e2 = STD::is_same<STD::tuple_element<2, AD3>::type, double>::value;
    test_true( e0 );
    test_true( e2 );
    test_pass("cxx11:tuple_element");
}

TEST_CASE(array, member_typedefs) {
    typedef STD::array<int, 4> A;
    A a = { 1, 2, 3, 4 };
    const A& cr = a;

    A::reference r = a[0];
    r = 10;
    test_eq( a[0], 10 );
    test_pass("cxx11:array::reference");

    A::pointer p = a.data();
    test_eq( *p, 10 );
    test_pass("cxx11:array::pointer");

    A::const_pointer cp = cr.data();
    test_eq( *cp, 10 );
    test_pass("cxx11:array::const_pointer");

    A::size_type n = a.size();
    test_eq( n, (STD::size_t)4 );
    test_pass("cxx11:array::size_type");

    A::difference_type d = a.end() - a.begin();
    test_eq( (long)d, 4L );
    test_pass("cxx11:array::difference_type");

    int sum = 0;
    for (A::const_iterator it = cr.begin(); it != cr.end(); ++it) sum += *it;
    test_eq( sum, 19 );
    test_pass("cxx11:array::const_iterator");

    int rsum = 0;
    for (A::const_reverse_iterator it = cr.rbegin(); it != cr.rend(); ++it) rsum += *it;
    test_eq( rsum, 19 );
    test_pass("cxx11:array::const_reverse_iterator");
}

TEST_CASE(array, cend_and_free_end) {
    STD::array<int, 3> a = { 1, 2, 3 };
    test_eq( (long)(a.cend() - a.cbegin()), 3L );
    test_pass("cxx11:array::cend");

    test_eq( (long)(STD::end(a) - STD::begin(a)), 3L );
    test_pass("cxx11:end");
}

#if TEST_TARGET_CXX >= 2011
#include <array>
#include <string>
#include <tuple>
#include <utility>
#include <type_traits>
#include <iterator>
#include <cstddef>

using _ccw::string_view;

namespace {

struct Pt {
    int x, y;
    bool operator==(const Pt& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Pt& o) const { return !(*this == o); }
    bool operator<(const Pt& o) const { return x != o.x ? x < o.x : y < o.y; }
};

inline Pt pt(int x, int y) { Pt p = {x, y}; return p; }

}

TEST_CASE(array, basic_access) {
    STD::array<int, 5> a = {1, 2, 3, 4, 5};

    test_eq( a.size(), STD::size_t(5) );
    test_pass("cxx11:array::size#chk");
    test_eq( a.max_size(), STD::size_t(5) );
    test_pass("cxx11:array::max_size#chk");
    test_true( !a.empty() );
    test_pass("cxx11:array::empty#chk");

    test_eq( a[0], 1 );
    test_eq( a[4], 5 );
    a[2] = 30;
    test_eq( a[2], 30 );
    test_pass("cxx11:array::operator[]#chk");

    test_eq( a.at(1), 2 );
    a.at(1) = 20;
    test_eq( a.at(1), 20 );
    test_throw( (void)a.at(5) );
    test_throw( (void)a.at(99) );
    test_pass("cxx11:array::at#chk");

    test_eq( a.front(), 1 );
    test_eq( a.back(), 5 );
    a.front() = 10;
    a.back()  = 50;
    test_eq( a[0], 10 );
    test_eq( a[4], 50 );
    test_pass("cxx11:array::front#chk");
    test_pass("cxx11:array::back#chk");

    int* p = a.data();
    test_ptr( p );
    test_eq( p[0], 10 );
    test_eq( &a[3] - p, STD::ptrdiff_t(3) );
    test_pass("cxx11:array::data#chk");

    const STD::array<int, 5>& ca = a;
    test_eq( ca[0], 10 );
    test_eq( ca.at(0), 10 );
    test_eq( ca.front(), 10 );
    test_eq( ca.back(), 50 );
    test_ptr( ca.data() );
    test_pass("cxx11:array#const_access");

    STD::array<int, 0> z;
    test_true( z.empty() );
    test_eq( z.size(), STD::size_t(0) );
    test_true( z.begin() == z.end() );
    test_pass("cxx11:array#zero_size_chk");
}

TEST_CASE(array, non_trivial_elements) {
    STD::array<STD::string, 3> s = {"one", "two", "three"};
    test_eq( s.size(), STD::size_t(3) );
    test_str_eq( s[0].c_str(), "one" );
    test_str_eq( s.back().c_str(), "three" );
    test_str_eq( s.at(1).c_str(), "two" );
    test_pass("cxx11:array#nontrivial_elem");

    STD::array<STD::string, 3> t = s;
    test_true( t == s );
    t[1] = "TWO";
    test_true( t != s );
    test_pass("cxx11:array#copy");

    STD::array<Pt, 2> p = {{ {1, 2}, {3, 4} }};
    test_eq( p[1].x, 3 );
    test_true( p.front() == pt(1, 2) );
    test_true( p.back()  == pt(3, 4) );
    test_pass("cxx11:array#aggregate_elem");
}

TEST_CASE(array, iterators_chk) {
    STD::array<int, 4> a = {1, 2, 3, 4};

    int sum = 0;
    for (STD::array<int, 4>::iterator it = a.begin(); it != a.end(); ++it)
        sum += *it;
    test_eq( sum, 10 );
    test_eq( a.end() - a.begin(), STD::ptrdiff_t(4) );

    *a.begin() = 100;
    test_eq( a[0], 100 );
    a[0] = 1;
    test_pass("cxx11:array::begin#chk");

    const STD::array<int, 4>& ca = a;
    test_eq( *ca.begin(), 1 );
    test_eq( ca.end() - ca.begin(), STD::ptrdiff_t(4) );
    test_pass("cxx11:array::end#chk");

    test_eq( *a.cbegin(), 1 );
    test_eq( a.cend() - a.cbegin(), STD::ptrdiff_t(4) );
    test_pass("cxx11:array::cbegin#chk");

    test_eq( *a.rbegin(), 4 );
    test_eq( *(a.rend() - 1), 1 );
    test_eq( a.rend() - a.rbegin(), STD::ptrdiff_t(4) );
    *a.rbegin() = 40;
    test_eq( a[3], 40 );
    a[3] = 4;
    test_pass("cxx11:array::rbegin#chk");

    test_eq( *ca.rbegin(), 4 );
    test_eq( ca.rend() - ca.rbegin(), STD::ptrdiff_t(4) );
    test_pass("cxx11:array::rend#chk");

    test_eq( *a.crbegin(), 4 );
    test_eq( a.crend() - a.crbegin(), STD::ptrdiff_t(4) );
    test_pass("cxx11:array::crbegin#chk");

    int seq = 0;
    for (STD::array<int, 4>::const_reverse_iterator it = a.crbegin(); it != a.crend(); ++it)
        seq = seq * 10 + *it;
    test_eq( seq, 4321 );
    test_pass("cxx11:array::crend#chk");

    test_eq( STD::distance(STD::begin(a), STD::end(a)), STD::ptrdiff_t(4) );
    test_pass("cxx11:begin");

    STD::array<STD::string, 2> s = {"a", "b"};
    test_str_eq( s.begin()->c_str(), "a" );
    test_str_eq( s.rbegin()->c_str(), "b" );
    test_pass("cxx11:array#iter_deref");
}

TEST_CASE(array, member_types) {
    typedef STD::array<Pt, 3> A;

    test_true(( STD::is_same<A::value_type, Pt>::value ));
    test_true(( STD::is_same<A::reference, Pt&>::value ));
    test_true(( STD::is_same<A::const_reference, const Pt&>::value ));
    test_true(( STD::is_same<A::size_type, STD::size_t>::value ));
    test_true(( STD::is_same<A::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx11:array::value_type");

    test_true(( STD::is_same<A::const_iterator,
                             STD::array<Pt, 3>::const_iterator>::value ));
#if _tst_cplusplus >= 201103L
    test_true(( STD::is_same<decltype(STD::declval<A&>().begin()), A::iterator>::value ));
    test_true(( STD::is_same<decltype(STD::declval<const A&>().begin()),
                             A::const_iterator>::value ));
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif
    test_true(( STD::is_same<STD::iterator_traits<A::iterator>::value_type, Pt>::value ));
    test_true(( STD::is_same<STD::iterator_traits<A::const_iterator>::value_type, Pt>::value ));
    test_true(( STD::is_same<STD::iterator_traits<A::iterator>::reference, Pt&>::value ));
    test_true(( STD::is_same<STD::iterator_traits<A::const_iterator>::reference,
                             const Pt&>::value ));
    test_pass("cxx11:array::iterator");

    test_true(( STD::is_same<A::reverse_iterator,
                             STD::reverse_iterator<A::iterator> >::value ));
    test_true(( STD::is_same<A::const_reverse_iterator,
                             STD::reverse_iterator<A::const_iterator> >::value ));
#if _tst_cplusplus >= 201103L
    test_true(( STD::is_same<decltype(STD::declval<A&>().rbegin()),
                             A::reverse_iterator>::value ));
    test_true(( STD::is_same<decltype(STD::declval<A&>().crbegin()),
                             A::const_reverse_iterator>::value ));
    test_pass("cxx11:array::reverse_iterator");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:array::reverse_iterator");
#endif

    test_true(( STD::is_same<STD::array<STD::string, 2>::value_type, STD::string>::value ));
    test_true(( STD::is_same<STD::array<STD::string, 2>::const_reference,
                             const STD::string&>::value ));
    test_pass("cxx11:array::const_reference");
}

TEST_CASE(array, fill_and_swap) {
    STD::array<int, 4> a;
    a.fill(7);
    test_eq( a[0], 7 );
    test_eq( a[3], 7 );
    test_pass("cxx11:array::fill#chk");

    STD::array<int, 4> b = {1, 2, 3, 4};
    a.swap(b);
    test_eq( a[0], 1 );
    test_eq( b[0], 7 );
    test_pass("cxx11:array::swap#chk");

    STD::swap(a, b);
    test_eq( a[0], 7 );
    test_eq( b[3], 4 );
    test_pass("cxx11:swap#chk");

    STD::array<STD::string, 2> s = {"x", "y"};
    s.fill("z");
    test_str_eq( s[0].c_str(), "z" );
    test_str_eq( s[1].c_str(), "z" );
    test_pass("cxx11:array::fill#string");

    STD::array<STD::string, 2> t = {"p", "q"};
    STD::swap(s, t);
    test_str_eq( s[0].c_str(), "p" );
    test_str_eq( t[0].c_str(), "z" );
    test_pass("cxx11:swap#string");

    STD::array<int, 0> z0, z1;
    test_no_throw( z0.swap(z1) );
    test_pass("cxx11:array::swap#empty");
}

TEST_CASE(array, comparison) {
    STD::array<int, 3> a = {1, 2, 3};
    STD::array<int, 3> b = {1, 2, 3};
    STD::array<int, 3> c = {1, 2, 4};

    test_true( a == b );
    test_true( !(a == c) );
    test_pass("cxx11:operator==#chk");
    test_true( a != c );
    test_true( !(a != b) );
    test_pass("cxx11:operator!=");

    test_true( a <  c );
    test_true( a <= b );
    test_true( a <= c );
    test_pass("cxx11:operator<#chk");
    test_true( c >  a );
    test_pass("cxx11:operator>");
    test_true( c >= a );
    test_true( b >= a );
    test_pass("cxx11:operator>=");
    test_true( !(c < a) );
    test_true( !(a > c) );
    test_pass("cxx11:operator<=");

#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    test_true( (a <=> b) == 0 );
    test_true( (a <=> c) <  0 );
    test_true( (c <=> a) >  0 );
    test_true( STD::is_eq(a <=> b) );
    test_true( STD::is_lt(a <=> c) );
    test_pass("cxx20:operator<=>");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>");
#endif

    STD::array<STD::string, 2> s1 = {"aa", "bb"};
    STD::array<STD::string, 2> s2 = {"aa", "bc"};
    test_true( s1 != s2 );
    test_true( s1 < s2 );
    test_pass("cxx11:operator<#string");

    STD::array<Pt, 2> p1 = {{ {1, 1}, {2, 2} }};
    STD::array<Pt, 2> p2 = {{ {1, 1}, {2, 3} }};
    test_true( p1 == p1 );
    test_true( p1 != p2 );
    test_true( p1 < p2 );
    test_pass("cxx11:operator==#aggregate");
}

TEST_CASE(array, tuple_interface_chk) {
    STD::array<int, 3> a = {1, 2, 3};

#if !defined(__WATCOMC__) && _TST_HAS_DECLTYPE
    test_eq( STD::get<0>(a), 1 );
    test_eq( STD::get<2>(a), 3 );
    STD::get<1>(a) = 20;
    test_eq( a[1], 20 );
    test_true(( STD::is_same<decltype(STD::get<0>(a)), int&>::value ));

    const STD::array<int, 3>& ca = a;
    test_eq( STD::get<0>(ca), 1 );
    test_true(( STD::is_same<decltype(STD::get<0>(ca)), const int&>::value ));

    STD::array<STD::string, 2> s = {"moved", "kept"};
    STD::string moved = STD::get<0>(STD::move(s));
    test_str_eq( moved.c_str(), "moved" );
    test_true(( STD::is_same<decltype(STD::get<0>(STD::move(a))), int&&>::value ));

    const STD::array<int, 3> cr = {7, 8, 9};
    test_eq( STD::get<2>(STD::move(cr)), 9 );
    TEST_SKIP_VC120("get(const array&&) is C++14; MSVC 12 has only the C++11 overloads");
    test_true(( STD::is_same<decltype(STD::get<0>(STD::move(cr))), const int&&>::value ));
    test_pass("cxx11:get");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:get");
#endif

    test_eq( (STD::tuple_size<STD::array<int, 3> >::value), STD::size_t(3) );
    test_eq( (STD::tuple_size<STD::array<STD::string, 7> >::value), STD::size_t(7) );
    test_eq( (STD::tuple_size<STD::array<Pt, 0> >::value), STD::size_t(0) );
    test_pass("cxx11:tuple_size#chk");
    test_true(( STD::is_same<STD::tuple_element<0, STD::array<int, 3> >::type, int>::value ));
    test_true(( STD::is_same<STD::tuple_element<1, STD::array<STD::string, 2> >::type,
                             STD::string>::value ));
    test_true(( STD::is_same<STD::tuple_element<0, const STD::array<Pt, 2> >::type,
                             const Pt>::value ));
    test_pass("cxx11:tuple_element#chk");

#if _tst_cplusplus >= 201703L
    STD::array<int, 3> sb = {4, 5, 6};
    auto& [x, y, z] = sb;
    test_eq( x, 4 );
    test_eq( y, 5 );
    test_eq( z, 6 );
    x = 40;
    test_eq( sb[0], 40 );
    test_pass("cxx17:array#structured_binding");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx17:array#structured_binding");
#endif
}

#if !defined(__WATCOMC__) && _TST_HAS_DECLTYPE && TEST_TARGET_CXX >= 2020
TEST_CASE(array, to_array) {
    int raw[3] = {1, 2, 3};
    auto a = STD::to_array(raw);
    test_true(( STD::is_same<decltype(a), STD::array<int, 3> >::value ));
    test_eq( a.size(), STD::size_t(3) );
    test_eq( a[2], 3 );
    a[0] = 9;
    test_eq( raw[0], 1 );
    test_pass("cxx20:to_array");

#if _TST_HAS_CXX17_LIB_MEMBERS
    auto b = STD::to_array({4, 5, 6, 7});
    test_true(( STD::is_same<decltype(b), STD::array<int, 4> >::value ));
    test_eq( b.size(), STD::size_t(4) );
    test_eq( b[3], 7 );
    test_pass("cxx20:to_array#rvalue");

    auto c = STD::to_array<long>({1, 2});
    test_true(( STD::is_same<decltype(c), STD::array<long, 2> >::value ));
    test_eq( c[1], 2L );
    test_pass("cxx20:to_array#explicit_type");
#else
    TEST_SKIP_N(3);
    test_skip("cxx20:to_array#rvalue");
    TEST_SKIP_N(2);
    test_skip("cxx20:to_array#explicit_type");
#endif

    const char cs[] = "hi";
    auto d = STD::to_array(cs);
    test_eq( d.size(), STD::size_t(3) );
    test_eq( d[0], 'h' );
    test_eq( d[2], '\0' );
    test_pass("cxx20:to_array#char_array");

#if _TST_HAS_CXX17_LIB_MEMBERS
    auto s = STD::to_array<STD::string>({"one", "two"});
    test_true(( STD::is_same<decltype(s), STD::array<STD::string, 2> >::value ));
    test_str_eq( s[0].c_str(), "one" );
    test_str_eq( s[1].c_str(), "two" );
    test_pass("cxx20:to_array#string");
#else
    TEST_SKIP_N(3);
    test_skip("cxx20:to_array#string");
#endif

    Pt praw[2] = {{1, 2}, {3, 4}};
    auto p = STD::to_array(praw);
    test_true(( STD::is_same<decltype(p), STD::array<Pt, 2> >::value ));
    test_true( p[1] == pt(3, 4) );
    test_pass("cxx20:to_array#aggregate");
}
#else
TEST_CASE_SKIP(array, to_array)
#endif

#if _TST_HAS_CTAD
TEST_CASE(array, aggregate_and_ctad) {

    STD::array<int, 3> a = {};
    test_eq( a[0], 0 );
    test_eq( a[2], 0 );
    test_pass("cxx11:array#value_init");

    STD::array<int, 3> b = {1};
    test_eq( b[0], 1 );
    test_eq( b[1], 0 );
    test_pass("cxx11:array#aggregate_init");

    STD::array c{1, 2, 3, 4};
    test_true(( STD::is_same<decltype(c), STD::array<int, 4> >::value ));
    test_eq( c.size(), STD::size_t(4) );
    test_pass("cxx17:array#ctad");

    STD::array<Pt, 2> p{{ {5, 6}, {7, 8} }};
    test_eq( p[0].y, 6 );
    test_pass("cxx11:array#brace_init");

    STD::array<STD::array<int, 2>, 2> m = {{ {{1, 2}}, {{3, 4}} }};
    test_eq( m[1][0], 3 );
    test_eq( STD::get<0>(STD::get<1>(m)), 3 );
    test_pass("cxx11:array#nested");
}
#else
TEST_CASE_SKIP(array, aggregate_and_ctad)
#endif

#else
TEST_CASE_SKIP(array, basic_access)
TEST_CASE_SKIP(array, non_trivial_elements)
TEST_CASE_SKIP(array, iterators_chk)
TEST_CASE_SKIP(array, member_types)
TEST_CASE_SKIP(array, fill_and_swap)
TEST_CASE_SKIP(array, comparison)
TEST_CASE_SKIP(array, tuple_interface_chk)
TEST_CASE_SKIP(array, to_array)
TEST_CASE_SKIP(array, aggregate_and_ctad)
#endif

static int g_arr_live = 0;

struct ArrLive {
    char* p_;
    ArrLive() : p_(new char[32]) { p_[0] = 0; ++g_arr_live; }
    ArrLive(char c) : p_(new char[32]) { p_[0] = c; p_[1] = 0; ++g_arr_live; }
    ArrLive(const ArrLive& o) : p_(new char[32]) {
        for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        ++g_arr_live;
    }
    ArrLive& operator=(const ArrLive& o) {
        if (this != &o) for (int i = 0; i < 32; ++i) p_[i] = o.p_[i];
        return *this;
    }
    ~ArrLive() { delete[] p_; --g_arr_live; }
    char tag() const { return p_[0]; }
    bool operator==(const ArrLive& o) const { return p_[0] == o.p_[0]; }
    bool operator< (const ArrLive& o) const { return p_[0] <  o.p_[0]; }
};

TEST_CASE(array, class_elements_lifetime) {
    g_arr_live = 0;
    {
        STD::array<ArrLive, 4> a;
        test_eq( g_arr_live, 4 );
        for (int i = 0; i < 4; ++i) a[i] = ArrLive((char)('a' + i));
        test_eq( g_arr_live, 4 );
        test_eq( a.front().tag(), 'a' );
        test_eq( a.back().tag(), 'd' );
        test_eq( a.at(2).tag(), 'c' );

        STD::array<ArrLive, 4> b(a);
        test_eq( g_arr_live, 8 );
        test_eq( b[3].tag(), 'd' );

        b.fill(ArrLive('z'));
        test_eq( g_arr_live, 8 );
        test_eq( b[0].tag(), 'z' );
        test_eq( b[3].tag(), 'z' );

        a.swap(b);
        test_eq( g_arr_live, 8 );
        test_eq( a[0].tag(), 'z' );
        test_eq( b[0].tag(), 'a' );

        b = a;
        test_eq( g_arr_live, 8 );
        test_eq( b[3].tag(), 'z' );

        int n = 0;
        for (STD::array<ArrLive, 4>::iterator it = a.begin(); it != a.end(); ++it) ++n;
        test_eq( n, 4 );
    }
    test_eq( g_arr_live, 0 );
}
