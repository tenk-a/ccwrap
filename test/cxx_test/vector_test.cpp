#include "test_cxx.hpp"
#include <vector>
#include <stdexcept>
#include <string>
#include <functional>
#include <type_traits>
#include <iterator>
#if TEST_TARGET_CXX >= 2011
#include <array>
#endif

TEST_CASE(vector, construct_and_assign) {
    STD::vector<int> a;
    test_true( a.empty() );
    test_eq( a.size(), 0u );
    test_pass("cxx03:vector::vector()");

    STD::vector<int> b(4);
    test_eq( b.size(), 4u );
    test_eq( b[0], 0 );
    test_pass("cxx03:vector::vector(n)");

    STD::vector<int> c(3, 7);
    test_eq( c.size(), 3u );
    test_eq( c[0], 7 );
    test_eq( c[2], 7 );
    test_pass("cxx03:vector::vector(n,value)");

    int raw[] = { 1, 2, 3, 4, 5 };
    STD::vector<int> d(raw, raw + 5);
    test_eq( d.size(), 5u );
    test_eq( d[4], 5 );
    test_pass("cxx03:vector::vector(range)");

    STD::vector<int> e(d);
    test_true( e == d );
    test_pass("cxx03:vector::vector(const vector&)");

    STD::vector<int> f;
    f = d;
    test_true( f == d );
    f = f;
    test_eq( f.size(), 5u );
    test_pass("cxx03:vector::operator=(const vector&)");

    c.assign(2, 9);
    test_eq( c.size(), 2u );
    test_eq( c[1], 9 );
    test_pass("cxx03:vector::assign(n,value)");
    c.assign(raw, raw + 5);
    test_eq( c.size(), 5u );
    test_eq( c[0], 1 );
    test_pass("cxx03:vector::assign(range)");
}

TEST_CASE(vector, element_access) {
    STD::vector<int> v(3, 0);
    v[0] = 10; v[1] = 20; v[2] = 30;
    const STD::vector<int>& cv = v;

    test_eq( v[1], 20 );
    test_eq( cv[1], 20 );
    test_pass("cxx03:vector::operator[]");
    test_eq( v.front(), 10 );
    test_eq( cv.front(), 10 );
    test_pass("cxx03:vector::front");
    test_eq( v.back(), 30 );
    test_eq( cv.back(), 30 );
    test_pass("cxx03:vector::back");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( *v.data(), 10 );
    test_eq( *cv.data(), 10 );
    test_eq( v.data()[2], 30 );
    test_pass("cxx11:vector::data");
#elif 0
    test_eq( *STD::data(v), 10 );
    test_eq( *STD::data(cv), 10 );
    test_eq( STD::data(v)[2], 30 );
    test_skip("cxx11:vector::data");
#else
    test_eq( *(&v[0]), 10 );
    test_eq( *(&cv[0]), 10 );
    test_eq( (&v[0])[2], 30 );
    test_skip("cxx11:vector::data");
#endif

    test_eq( v.at(2), 30 );
    test_eq( cv.at(2), 30 );
    v.at(0) = 11;
    test_eq( v[0], 11 );
    test_throw( (void)v.at(3) );
    test_throw( (void)cv.at(99) );
    test_pass("cxx03:vector::at");
}

TEST_CASE(vector, iterators) {
    int raw[] = { 1, 2, 3, 4 };
    STD::vector<int> v(raw, raw + 4);
    const STD::vector<int>& cv = v;

    int sum = 0;
    for (STD::vector<int>::iterator it = v.begin(); it != v.end(); ++it) sum += *it;
    test_eq( sum, 10 );
    test_pass("cxx03:vector::begin");

    sum = 0;
    for (STD::vector<int>::const_iterator it = cv.begin(); it != cv.end(); ++it) sum += *it;
    test_eq( sum, 10 );
    test_pass("cxx03:vector::end");

    test_eq( v.end() - v.begin(), (STD::ptrdiff_t)4 );
    *v.begin() = 100;
    test_eq( v[0], 100 );
    v[0] = 1;

    test_eq( *v.rbegin(), 4 );
    test_eq( *cv.rbegin(), 4 );
    test_pass("cxx03:vector::rbegin");
    int last = *(v.rend() - 1);
    test_eq( last, 1 );
    test_pass("cxx03:vector::rend");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(vector, iterators_cxx11) {
    int raw[] = { 1, 2, 3, 4 };
    STD::vector<int> v(raw, raw + 4);
    const STD::vector<int>& cv = v;

    int sum = 0;
    for (STD::vector<int>::const_iterator it = v.cbegin(); it != v.cend(); ++it) sum += *it;
    test_eq( sum, 10 );
    test_pass("cxx11:vector::cbegin");

    test_eq( *v.crbegin(), 4 );
    test_pass("cxx11:vector::crbegin");
    test_eq( *(cv.crend() - 1), 1 );
    test_pass("cxx11:vector::crend");

    int rev = 0;
    for (STD::vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
        rev = rev * 10 + *it;
    test_eq( rev, 4321 );
}
#else
TEST_CASE_SKIP(vector, iterators_cxx11)
#endif

TEST_CASE(vector, capacity) {
    STD::vector<int> v;
    test_true( v.empty() );
    test_pass("cxx03:vector::empty");
    test_true( v.max_size() > 0u );
    test_pass("cxx03:vector::max_size");

    v.reserve(32);
    test_true( v.capacity() >= 32u );
    test_true( v.empty() );
    test_throw( v.reserve(v.max_size() + 1u) );
#if TEST_HAS_EH
    {
        bool lng = false;
        try { v.reserve(v.max_size() + 1u); } catch (STD::length_error&) { lng = true; } catch (...) {}
        test_true( lng );
    }
#else
    TEST_SKIP1();
    test_true( true );
#endif
    test_pass("cxx03:vector::reserve");

    for (int i = 0; i < 5; ++i) v.push_back(i);
    test_eq( v.size(), 5u );
    test_pass("cxx03:vector::size");
    test_true( !v.empty() );

    v.resize(8);
    test_eq( v.size(), 8u );
    test_eq( v[7], 0 );
    v.resize(10, 3);
    test_eq( v[9], 3 );
    v.resize(2);
    test_eq( v.size(), 2u );
    test_eq( v.back(), 1 );
    test_pass("cxx03:vector::resize");

    test_true( v.capacity() >= v.size() );
    test_pass("cxx03:vector::capacity");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(vector, capacity_cxx11) {
    STD::vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i);
    v.reserve(100);
    STD::vector<int>::size_type cap = v.capacity();
    v.resize(2);
    v.shrink_to_fit();
    test_true( v.capacity() <= cap );
    test_eq( v.size(), 2u );
    test_pass("cxx11:vector::shrink_to_fit");
}
#else
TEST_CASE_SKIP(vector, capacity_cxx11)
#endif

TEST_CASE(vector, nonmember_access) {
    STD::vector<int> v;
    v.push_back(10); v.push_back(20); v.push_back(30);
    const STD::vector<int>& cv = v;

#if TEST_TARGET_CXX >= 2017
    test_eq( *STD::data(v), 10 );
    test_eq( STD::data(v)[2], 30 );
    test_eq( *STD::data(cv), 10 );
    test_pass("cxx17:constexpr data(C& c)");

    test_eq( (long)STD::size(v), 3L );
    test_pass("cxx17:constexpr size(const C& c)");

    test_true( !STD::empty(v) );
    STD::vector<int> e;
    test_true( STD::empty(e) );
    test_pass("cxx17:constexpr empty(const C& c)");
#else
    TEST_NOTE("std::data / size / empty are C++17");
    test_skip("cxx17:constexpr data(C& c)");
    test_skip("cxx17:constexpr size(const C& c)");
    test_skip("cxx17:constexpr empty(const C& c)");
#endif

#if TEST_TARGET_CXX >= 2014
    int sum = 0;
    for (STD::vector<int>::const_iterator it = STD::cbegin(v); it != STD::cend(v); ++it) sum += *it;
    test_eq( sum, 60 );
    test_pass("cxx14:constexpr cbegin(const C& c)");
#else
    TEST_NOTE("std::cbegin / cend are C++14");
    test_skip("cxx14:constexpr cbegin(const C& c)");
#endif

    test_eq( *STD::begin(v), 10 );
    test_eq( *(STD::end(v) - 1), 30 );
    test_pass("cxx11:begin(C& c)");
}

TEST_CASE(vector, modifiers) {
    STD::vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i * 10);
    test_eq( v.size(), 5u );
    test_pass("cxx03:vector::push_back");

    v.pop_back();
    test_eq( v.size(), 4u );
    test_eq( v.back(), 30 );
    test_pass("cxx03:vector::pop_back");

    STD::vector<int>::iterator it = v.insert(v.begin(), -1);
    test_eq( *it, -1 );
    test_eq( v.front(), -1 );
    test_pass("cxx03:vector::insert");

    v.insert(v.begin() + 1, (STD::vector<int>::size_type)2, 5);
    test_eq( v[1], 5 );
    test_eq( v[2], 5 );
    test_eq( v[3], 0 );
    test_pass("cxx03:vector::insert (n copies)");

    int more[] = { 7, 8 };
    v.insert(v.end(), more, more + 2);
    test_eq( v.back(), 8 );
    test_pass("cxx03:vector::insert (range)");

    STD::vector<int>::iterator er = v.erase(v.begin());
    test_eq( *er, 5 );
    test_pass("cxx03:vector::erase");

    STD::vector<int> w(v);
    STD::vector<int>::iterator er2 = w.erase(w.begin(), w.begin() + 2);
    test_eq( w.size(), v.size() - 2 );
    (void)er2;
    test_pass("cxx03:vector::erase (range)");

    v.clear();
    test_true( v.empty() );
    test_pass("cxx03:vector::clear");

    STD::vector<int> p(3, 1), q(2, 9);
    p.swap(q);
    test_eq( p.size(), 2u );
    test_eq( p[0], 9 );
    test_eq( q.size(), 3u );
    test_eq( q[0], 1 );
    test_pass("cxx03:vector::swap");
    STD::swap(p, q);
    test_eq( p.size(), 3u );
    test_pass("cxx03:swap");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(vector, emplace) {
    STD::vector<int> v;
    v.emplace_back();
    test_eq( v.size(), 1u );
    test_eq( v.back(), 0 );
    test_pass("cxx11:vector::emplace_back");

#if TEST_TARGET_CXX >= 2011
    v.emplace_back(42);
    test_eq( v.back(), 42 );
    test_pass("cxx11:vector::emplace_back (args)");

    STD::vector<int>::iterator it = v.emplace(v.begin(), 7);
    test_eq( *it, 7 );
    test_eq( v.front(), 7 );
    test_pass("cxx11:vector::emplace");
#else
    TEST_SKIP_N(3);
#endif
}
#else
TEST_CASE_SKIP(vector, emplace)
#endif

namespace {
struct Tracked {
    static int live;
    int v;
    Tracked() : v(0) { ++live; }
    Tracked(int x) : v(x) { ++live; }
    Tracked(const Tracked& o) : v(o.v) { ++live; }
    Tracked& operator=(const Tracked& o) { v = o.v; return *this; }
    ~Tracked() { --live; }
    bool operator==(const Tracked& o) const { return v == o.v; }
};
int Tracked::live = 0;
}

TEST_CASE(vector, string_elements) {

    STD::vector<STD::string> v;
    v.push_back("alpha");
    v.push_back("beta");
    v.push_back("gamma");
    test_eq( v.size(), 3u );
    test_eq( v[0], STD::string("alpha") );
    test_eq( v.back(), STD::string("gamma") );
    test_pass("cxx03:vector::push_back#string");

    {
        STD::vector<STD::string> sv;
        sv.push_back("first-element-long-enough-to-allocate");
        for (int i = 0; i < 8; ++i) sv.push_back(sv[0]);
        test_eq( sv.size(), 9u );
        bool all_same = true;
        for (STD::size_t i = 0; i < sv.size(); ++i)
            if (sv[i] != STD::string("first-element-long-enough-to-allocate")) all_same = false;
        test_true( all_same );
        STD::vector<int> iv;
        iv.push_back(7);
        for (int i = 0; i < 40; ++i) iv.push_back(iv[0]);
        test_eq( iv.size(), 41u );
        test_eq( iv[40], 7 );
    }

    v.insert(v.begin() + 1, "INSERTED");
    test_eq( v[1], STD::string("INSERTED") );
    test_eq( v[2], STD::string("beta") );
    test_pass("cxx03:vector::insert#string");

    v.erase(v.begin());
    test_eq( v.front(), STD::string("INSERTED") );
    test_pass("cxx03:vector::erase#string");

    STD::vector<STD::string> w(v);
    test_true( w == v );
    w[0] = "CHANGED";
    test_true( w != v );
    test_pass("cxx03:vector::vector(const vector&)#string");

    v.resize(6, STD::string("pad"));
    test_eq( v.size(), 6u );
    test_eq( v.back(), STD::string("pad") );
    test_pass("cxx03:vector::resize#string");

    STD::vector<STD::string> big;
    big.push_back(STD::string(500, 'x'));
    test_eq( big[0].size(), 500u );
    test_pass("cxx03:vector#string_heap");

    v.clear();
    test_true( v.empty() );
}

TEST_CASE(vector, element_lifetime) {

    Tracked::live = 0;
    {
        STD::vector<Tracked> v;
        for (int i = 0; i < 8; ++i) v.push_back(Tracked(i));
        test_eq( Tracked::live, 8 );
        v.erase(v.begin());
        test_eq( Tracked::live, 7 );
        v.resize(3);
        test_eq( Tracked::live, 3 );
        STD::vector<Tracked> copy(v);
        test_eq( Tracked::live, 6 );
    }
    test_eq( Tracked::live, 0 );
    test_pass("cxx03:vector#element_lifetime");
}

TEST_CASE(vector, compare_and_allocator) {
    int raw[] = { 1, 2, 3 };
    STD::vector<int> a(raw, raw + 3);
    STD::vector<int> b(raw, raw + 3);
    STD::vector<int> c(raw, raw + 2);

    test_true( a == b );
    test_pass("cxx03:vector::operator==");
    test_true( a != c );
    test_pass("cxx03:vector::operator!=");
    test_true( c <  a );
    test_pass("cxx03:vector::operator<");
    test_true( a >  c );
    test_pass("cxx03:vector::operator>");
    test_true( a <= b );
    test_pass("cxx03:vector::operator<=");
    test_true( a >= b );
    test_pass("cxx03:vector::operator>=");

    STD::vector<int>::allocator_type al = a.get_allocator();
    (void)al;
    test_true( true );
    test_pass("cxx03:vector::get_allocator");
}

TEST_CASE(vector, vector_bool) {

    STD::vector<bool> v;
    v.push_back(true);
    v.push_back(false);
    v.push_back(true);
    test_eq( v.size(), 3u );
    test_true(  v[0] );
    test_true( !v[1] );
    test_true(  v[2] );
    test_pass("cxx03:vector<bool>");

    v[1] = true;
    test_true( v[1] );
    test_pass("cxx03:vector<bool>::reference");
    v.flip();
    test_true( !v[0] );
    test_true( !v[1] );
    test_pass("cxx03:vector<bool>::flip");

    bool f = v.front();
    bool b = v.back();
    test_true( !f );
    test_true( !b );
    test_pass("cxx03:vector<bool>::front");

    STD::vector<bool> w(5, true);
    test_eq( w.size(), 5u );
    test_true( w[4] );
    w.clear();
    test_true( w.empty() );
    test_pass("cxx03:vector<bool>#fill_clear");
}

#if _tst_cplusplus >= 201103L
TEST_CASE(vector, init_list) {
    STD::vector<int> v = { 1, 2, 3, 4 };
    test_eq( v.size(), 4u );
    test_eq( v[0], 1 );
    test_eq( v[3], 4 );
    test_pass("cxx11:vector::vector(initializer_list)");

    v = { 9, 8 };
    test_eq( v.size(), 2u );
    test_eq( v[0], 9 );
    test_pass("cxx11:vector::operator=(initializer_list)");

    v.assign({ 5, 6, 7 });
    test_eq( v.size(), 3u );
    test_eq( v[2], 7 );
    test_pass("cxx11:vector::assign(initializer_list)");

    v.insert(v.begin() + 1, { 100, 101 });
    test_eq( v.size(), 5u );
    test_eq( v[1], 100 );
    test_eq( v[2], 101 );
    test_pass("cxx11:vector::insert(initializer_list)");
}
#else
TEST_CASE_SKIP(vector, init_list)
#endif

TEST_CASE(vector, move_and_rvalue) {
#if TEST_TARGET_CXX >= 2011
    {
        STD::vector<STD::string> v;
        STD::string s("moved");
        v.push_back(STD::move(s));
        test_eq( v.back(), STD::string("moved") );
    }
    test_pass("cxx11:vector::push_back(T&&)");
#else
    TEST_SKIP1();
    test_skip("cxx11:vector::push_back(T&&)");
#endif

#if TEST_TARGET_CXX >= 2011
    {
        STD::vector<STD::string> v; v.push_back("a"); v.push_back("c");
        STD::string s("b");
        STD::vector<STD::string>::iterator it = v.insert(v.begin() + 1, STD::move(s));
        test_eq( *it, STD::string("b") );
        test_eq( v[1], STD::string("b") );
    }
    test_pass("cxx11:vector::insert(T&&)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:vector::insert(T&&)");
#endif

#if TEST_TARGET_CXX >= 2011
    {
        STD::vector<int> a; a.push_back(1); a.push_back(2);
        STD::vector<int> b(STD::move(a));
        test_eq( b.size(), 2u );
        test_eq( b[1], 2 );
    }
    test_pass("cxx11:vector::vector(vector&&)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:vector::vector(vector&&)");
#endif

#if TEST_TARGET_CXX >= 2011
    {
        STD::vector<int> a; a.push_back(7); a.push_back(8); a.push_back(9);
        STD::vector<int> b;
        b = STD::move(a);
        test_eq( b.size(), 3u );
        test_eq( b[0], 7 );
    }
    test_pass("cxx11:vector::operator=(vector&&)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:vector::operator=(vector&&)");
#endif

#if TEST_TARGET_CXX >= 2011
    {
        STD::allocator<int> al;
        STD::vector<int> v1(al);
        v1.push_back(1);
        test_eq( v1.front(), 1 );
        STD::vector<int> v2((STD::size_t)3, 5, al);
        test_eq( v2.size(), 3u );
        test_eq( v2[2], 5 );
    }
    test_pass("cxx11:vector::vector(const Allocator&) family");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:vector::vector(const Allocator&) family");
#endif

#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
    {
        STD::vector<int> v; v.push_back(1); v.push_back(2);
        test_eq( (long)(v.cend() - v.cbegin()), 2L );
    }
    test_pass("cxx11:vector::cend");
#else
    TEST_SKIP1();
    test_skip("cxx11:vector::cend");
#endif
}

TEST_CASE(vector, vector_bool_ref_swap) {
    STD::vector<bool> vb(2);
    vb[0] = true; vb[1] = false;
    STD::vector<bool>::swap(vb[0], vb[1]);
    test_true( vb[0] == false );
    test_true( vb[1] == true );
    test_pass("cxx03:vector<bool>::swap(reference,reference)");
}

TEST_CASE(vector, spaceship) {
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    STD::vector<int> a, b;
    a.push_back(1); a.push_back(2);
    b.push_back(1); b.push_back(3);
    test_true( (a <=> a) == 0 );
    test_true( (a <=> b) <  0 );
    test_true( (b <=> a) >  0 );
    test_pass("cxx20:operator<=>(vector)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>(vector)");
#endif
}

TEST_CASE(vector, ranges_ops) {
#if defined(__cpp_lib_containers_ranges)
    int src[] = { 4, 5, 6 };
    {
        STD::vector<int> v(STD::from_range, src);
        test_eq( v.size(), 3u );
        test_eq( v[0], 4 );
        test_pass("cxx23:vector::vector(from_range_t,R&&)");
    }
    {
        STD::vector<int> v; v.push_back(1);
        v.append_range(src);
        test_eq( v.size(), 4u );
        test_eq( v.back(), 6 );
        test_pass("cxx23:vector::append_range");
    }
    {
        STD::vector<int> v; v.push_back(9);
        v.assign_range(src);
        test_eq( v.size(), 3u );
        test_eq( v[1], 5 );
        test_pass("cxx23:vector::assign_range");
    }
    {
        STD::vector<int> v; v.push_back(1); v.push_back(9);
        v.insert_range(v.begin() + 1, src);
        test_eq( v.size(), 5u );
        test_eq( v[1], 4 );
        test_eq( v[3], 6 );
        test_pass("cxx23:vector::insert_range");
    }
#else
    test_skip("cxx23:vector::vector(from_range_t,R&&)");
    test_skip("cxx23:vector::append_range");
    test_skip("cxx23:vector::assign_range");
    test_skip("cxx23:vector::insert_range");
#endif
}

#if TEST_TARGET_CXX >= 2020
struct IsOdd { bool operator()(int x) const { return x % 2 != 0; } };

TEST_CASE(vector, erase_free) {
    int raw[] = { 1, 2, 3, 2, 5, 2 };
    STD::vector<int> v(raw, raw + 6);

    STD::vector<int>::size_type n = STD::erase(v, 2);
    test_eq( (long)n, 3L );
    test_eq( v.size(), 3u );
    test_eq( v[0], 1 );
    test_eq( v[1], 3 );
    test_eq( v[2], 5 );
    test_pass("cxx20:erase(vector)");

    STD::vector<int> w(raw, raw + 6);
    STD::vector<int>::size_type m = STD::erase_if(w, IsOdd());
    test_eq( (long)m, 3L );
    test_eq( w.size(), 3u );
    test_eq( w[0], 2 );
    test_pass("cxx20:erase_if(vector)");
}
#else
TEST_CASE_SKIP(vector, erase_free)
#endif


TEST_CASE(vector, member_types_cxx03) {
    typedef STD::vector<int> C;
    C c; c.push_back(1); c.push_back(2);
    const C& cc = c;

    test_true(( STD::is_same<C::value_type, int>::value ));
    test_pass("cxx03:vector::value_type");

    C::reference r = c.front();
    r = 5;
    test_eq( c.front(), 5 );
    test_true(( STD::is_same<C::reference, int&>::value ));
    test_pass("cxx03:vector::reference");

    C::const_reference cr = cc.front();
    test_eq( cr, 5 );
    test_true(( STD::is_same<C::const_reference, const int&>::value ));
    test_pass("cxx03:vector::const_reference");

    C::pointer p = &c.front();
    test_eq( *p, 5 );
    test_pass("cxx03:vector::pointer");

    C::const_pointer cp = &cc.front();
    test_eq( *cp, 5 );
    test_pass("cxx03:vector::const_pointer");

    C::size_type n = c.size();
    test_eq( n, (STD::size_t)2 );
    test_true( !STD::is_signed<C::size_type>::value );
    test_pass("cxx03:vector::size_type");

    C::difference_type d = 0;
    for (C::const_iterator i = cc.begin(); i != cc.end(); ++i) ++d;
    test_eq( (long)d, 2L );
    test_true( STD::is_signed<C::difference_type>::value );
    test_pass("cxx03:vector::difference_type");

    C::iterator it = c.begin();
    *it = 9;
    test_eq( c.front(), 9 );
    test_true( tst_type_is<C::iterator>(c.begin()) );
    test_pass("cxx03:vector::iterator");

    C::const_iterator ci = cc.begin();
    test_eq( *ci, 9 );
    test_true( tst_type_is<C::const_iterator>(cc.begin()) );
    test_pass("cxx03:vector::const_iterator");

    C::reverse_iterator ri = c.rbegin();
    test_eq( *ri, 2 );
    test_pass("cxx03:vector::reverse_iterator");

    C::const_reverse_iterator cri = cc.rbegin();
    test_eq( *cri, 2 );
    test_pass("cxx03:vector::const_reverse_iterator");
}

TEST_CASE(vector, allocator_type_cxx11) {
    typedef STD::vector<int> C;
    C c;
    test_true(( STD::is_same<C::allocator_type, STD::allocator<int> >::value ));
    C::allocator_type al = c.get_allocator();
    (void)al;
    test_pass("cxx03:vector::allocator_type");
}

#if _TST_HAS_CXX11_LIB_MEMBERS && !defined(__WATCOMC__)
TEST_CASE(vector, hash_bool_cxx11) {
    STD::vector<bool> a, b, c;
    a.push_back(true);  a.push_back(false); a.push_back(true);
    b.push_back(true);  b.push_back(false); b.push_back(true);
    c.push_back(false); c.push_back(false); c.push_back(true);

    STD::hash<STD::vector<bool> > h;
    test_true( h(a) == h(b) );
    test_true( h(a) != h(c) );
    test_true( tst_type_is<STD::size_t>(h(a)) );
    test_pass("cxx11:hash<vector<bool>>");
}
#else
TEST_CASE(vector, hash_bool_cxx11) {
    test_skip("cxx11:hash<vector<bool>>");
}
#endif

TEST_CASE(vector, feature_test_macros) {
#if defined(__cpp_lib_constexpr_vector) && (__cpp_lib_constexpr_vector) >= 201907L
    test_true( (__cpp_lib_constexpr_vector) >= 201907L );
    test_pass("cxx20:__cpp_lib_constexpr_vector=201907L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_constexpr_vector=201907L");
#endif
#if defined(__cpp_lib_erase_if) && (__cpp_lib_erase_if) >= 202002L
    test_true( (__cpp_lib_erase_if) >= 202002L );
    test_pass("cxx20:__cpp_lib_erase_if=202002L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx20:__cpp_lib_erase_if=202002L");
#endif
#if defined(__cpp_lib_containers_ranges) && (__cpp_lib_containers_ranges) >= 202202L
    test_true( (__cpp_lib_containers_ranges) >= 202202L );
    test_pass("cxx23:__cpp_lib_containers_ranges=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_containers_ranges=202202L");
#endif
}

TEST_CASE(vector, constexpr_range_access_cxx17) {
#if defined(__cpp_lib_array_constexpr) && (__cpp_lib_array_constexpr) >= 201603L
    struct K {
        static constexpr int first() { STD::array<int, 3> a = {7, 8, 9};
                                       return *STD::begin(a); }
        static constexpr int cdata() { STD::array<int, 3> a = {4, 5, 6};
                                       const STD::array<int, 3>& r = a; return *STD::data(r); }
    };
    static_assert(K::first() == 7, "constexpr begin(C&)");
    test_eq( K::first(), 7 );
    test_pass("cxx17:constexpr begin(C& c)");

    static_assert(K::cdata() == 4, "constexpr data(const C&)");
    test_eq( K::cdata(), 4 );
    test_pass("cxx17:constexpr data(const C& c)");
#else
    TEST_NOTE("begin/data became constexpr in C++17");
    TEST_SKIP_N(2);
    test_skip("cxx17:constexpr begin(C& c)");
    test_skip("cxx17:constexpr data(const C& c)");
#endif
}
