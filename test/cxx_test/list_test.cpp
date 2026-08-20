#include "test_cxx.hpp"
#include <list>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>
#if TEST_TARGET_CXX >= 2017
#include <memory_resource>
#endif

namespace {

long sig(const STD::list<int>& l) {
    long s = 0;
    for (STD::list<int>::const_iterator it = l.begin(); it != l.end(); ++it) s = s * 10 + *it;
    return s;
}
struct Less3 { bool operator()(int a, int b) const { return a < b; } };
struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
struct Same   { bool operator()(int a, int b) const { return a == b; } };

int g_live = 0;
struct Live {
    int v;
    Live() : v(0) { ++g_live; }
    Live(int x) : v(x) { ++g_live; }
    Live(const Live& o) : v(o.v) { ++g_live; }
    Live& operator=(const Live& o) { v = o.v; return *this; }
    ~Live() { --g_live; }
};
}

TEST_CASE(list, construct_and_assign) {
    STD::list<int> a;
    test_true( a.empty() );
    test_pass("cxx03:list::empty");
    test_eq( a.size(), 0u );
    test_pass("cxx03:list::list()");

    STD::list<int> b(3, 5);
    test_eq( b.size(), 3u );
    test_eq( sig(b), 555L );
    test_pass("cxx03:list::list(n,value)");

    int raw[] = { 1, 2, 3, 4 };
    STD::list<int> c(raw, raw + 4);
    test_eq( sig(c), 1234L );
    test_pass("cxx03:list::list(range)");

    STD::list<int> d(c);
    test_eq( sig(d), 1234L );
    test_pass("cxx03:list::list(const list&)");

    STD::list<int> e;
    e = c;
    test_eq( sig(e), 1234L );
    test_pass("cxx03:list::operator=(const list&)");

    b.assign(2, 8);
    test_eq( sig(b), 88L );
    test_pass("cxx03:list::assign(n,value)");
    b.assign(raw, raw + 3);
    test_eq( sig(b), 123L );
    test_pass("cxx03:list::assign(range)");
}

TEST_CASE(list, destructor) {
    g_live = 0;
    {
        STD::list<Live> l;
        l.push_back(Live(1));
        l.push_back(Live(2));
        l.push_back(Live(3));
        test_eq( g_live, 3 );
    }
    test_eq( g_live, 0 );
    {
        STD::list<Live> l(4, Live(7));
        l.clear();
        test_eq( g_live, 0 );
    }
    test_eq( g_live, 0 );
    test_pass("cxx03:list::~list");
}

TEST_CASE(list, front_back_push_pop) {
    STD::list<int> l;
    l.push_back(2);
    l.push_back(3);
    l.push_front(1);
    test_eq( sig(l), 123L );
    test_pass("cxx03:list::push_back");
    test_eq( l.front(), 1 );
    test_pass("cxx03:list::front");
    test_eq( l.back(), 3 );
    test_pass("cxx03:list::back");

    const STD::list<int>& cl = l;
    test_eq( cl.front(), 1 );
    test_eq( cl.back(), 3 );

    l.front() = 9;
    l.back() = 7;
    test_eq( sig(l), 927L );
    l.front() = 1; l.back() = 3;

    l.pop_front();
    test_eq( l.front(), 2 );
    test_pass("cxx03:list::pop_front");
    l.pop_back();
    test_eq( l.back(), 2 );
    test_eq( l.size(), 1u );
    test_pass("cxx03:list::pop_back");
}

TEST_CASE(list, iterators) {
    int raw[] = { 1, 2, 3, 4 };
    STD::list<int> l(raw, raw + 4);
    const STD::list<int>& cl = l;

    int sum = 0;
    for (STD::list<int>::iterator it = l.begin(); it != l.end(); ++it) sum += *it;
    test_eq( sum, 10 );
    test_pass("cxx03:list::begin");

    sum = 0;
    for (STD::list<int>::const_iterator it = cl.begin(); it != cl.end(); ++it) sum += *it;
    test_eq( sum, 10 );
    test_pass("cxx03:list::end");

    STD::list<int>::iterator e = l.end();
    --e;
    test_eq( *e, 4 );
    test_pass("cxx03:list#bidirectional");

    test_eq( *l.rbegin(), 4 );
    test_eq( *cl.rbegin(), 4 );
    test_pass("cxx03:list::rbegin");
    long rev = 0;
    for (STD::list<int>::const_reverse_iterator it = l.rbegin(); it != l.rend(); ++it)
        rev = rev * 10 + *it;
    test_eq( rev, 4321L );
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(list, iterators_cxx11) {
    int raw[] = { 1, 2, 3, 4 };
    STD::list<int> l(raw, raw + 4);

    int sum = 0;
    for (STD::list<int>::const_iterator it = l.cbegin(); it != l.cend(); ++it) sum += *it;
    test_eq( sum, 10 );
    test_pass("cxx11:list::cbegin");

    test_eq( *l.crbegin(), 4 );
    test_pass("cxx11:list::crbegin");
    long rev = 0;
    for (STD::list<int>::const_reverse_iterator it = l.crbegin(); it != l.crend(); ++it)
        rev = rev * 10 + *it;
    test_eq( rev, 4321L );
    test_pass("cxx11:list::crend");
}
#else
TEST_CASE_SKIP(list, iterators_cxx11)
#endif

TEST_CASE(list, insert_erase_resize) {
    STD::list<int> l;
    for (int i = 1; i <= 3; ++i) l.push_back(i);

    STD::list<int>::iterator it = l.begin();
    ++it;
    STD::list<int>::iterator r = l.insert(it, 9);
    test_eq( *r, 9 );
    test_eq( sig(l), 1923L );
    test_pass("cxx03:list::insert");

    l.insert(l.end(), (STD::list<int>::size_type)2, 7);
    test_eq( sig(l), 192377L );
    test_pass("cxx03:list::insert (n copies)");

    int more[] = { 5, 6 };
    l.insert(l.begin(), more, more + 2);
    test_eq( sig(l), 56192377L );
    test_pass("cxx03:list::insert (range)");

    STD::list<int>::iterator er = l.erase(l.begin());
    test_eq( *er, 6 );
    test_pass("cxx03:list::erase");

    l.clear();
    test_true( l.empty() );
    test_pass("cxx03:list::clear");

    l.resize(4);
    test_eq( l.size(), 4u );
    test_pass("cxx03:list::size");
    test_eq( sig(l), 0L );
    l.resize(6, 5);
    test_eq( l.size(), 6u );
    test_eq( l.back(), 5 );
    l.resize(2);
    test_eq( l.size(), 2u );
    test_pass("cxx03:list::resize");
}

TEST_CASE(list, splice) {
    int a1[] = { 1, 2, 3 };
    int a2[] = { 7, 8 };

    STD::list<int> a(a1, a1 + 3);
    STD::list<int> b(a2, a2 + 2);
    STD::list<int>::iterator mid = a.begin(); ++mid;
    a.splice(mid, b);
    test_eq( sig(a), 17823L );
    test_true( b.empty() );
    test_pass("cxx03:list::splice");

    STD::list<int> c(a1, a1 + 3);
    STD::list<int> d(a2, a2 + 2);
    c.splice(c.end(), d, d.begin());
    test_eq( sig(c), 1237L );
    test_eq( sig(d), 8L );
    test_pass("cxx03:list::splice (one)");

    STD::list<int> e(a1, a1 + 3);
    STD::list<int> f(a2, a2 + 2);
    e.splice(e.begin(), f, f.begin(), f.end());
    test_eq( sig(e), 78123L );
    test_true( f.empty() );
    test_pass("cxx03:list::splice (range)");
}

TEST_CASE(list, remove_unique_reverse_sort) {
    int raw[] = { 3, 1, 2, 1, 3, 2, 1 };
    STD::list<int> l(raw, raw + 7);

    l.remove(1);
    test_eq( sig(l), 3232L );
    test_pass("cxx03:list::remove");

    l.remove_if(IsEven());
    test_eq( sig(l), 33L );
    test_pass("cxx03:list::remove_if");

#if defined(__cpp_lib_list_remove_return_type)
    {
        STD::list<int> c(raw, raw + 7);
        long n = (long)c.remove(1);
        test_eq( n, 3L );
        long again = (long)c.remove(1);
        test_eq( again, 0L );
        test_pass("cxx20:list::remove returns size_type");
        long ev = (long)c.remove_if(IsEven());
        test_eq( ev, 2L );
        test_pass("cxx20:list::remove_if returns size_type");
        int r[] = { 1, 1, 2, 2, 2, 3 };
        STD::list<int> d(r, r + 6);
        long dup = (long)d.unique();
        test_eq( dup, 3L );
        test_pass("cxx20:list::unique returns size_type");
    }
    test_skip("cxx03:list::remove returns void");
    test_skip("cxx03:list::remove_if returns void");
    test_skip("cxx03:list::unique returns void");
#else
  #if _TST_HAS_DECLTYPE
    {
        STD::list<int> c;
        test_true(( STD::is_same<decltype(c.remove(1)), void>::value ));
        test_true(( STD::is_same<decltype(c.remove_if(IsEven())), void>::value ));
        test_true(( STD::is_same<decltype(c.unique()), void>::value ));
    }
    test_pass("cxx03:list::remove returns void");
  #else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx03:list::remove returns void");
  #endif
    test_pass("cxx03:list::remove_if returns void");
    test_pass("cxx03:list::unique returns void");
    test_skip("cxx20:list::remove returns size_type");
    test_skip("cxx20:list::remove_if returns size_type");
    test_skip("cxx20:list::unique returns size_type");
#endif

    int r2[] = { 1, 1, 2, 3, 3, 3, 2 };
    STD::list<int> u(r2, r2 + 7);
    u.unique();
    test_eq( sig(u), 1232L );
    test_pass("cxx03:list::unique");

    STD::list<int> u2(r2, r2 + 7);
    u2.unique(Same());
    test_eq( sig(u2), 1232L );
    test_pass("cxx03:list::unique (pred)");

    int r3[] = { 1, 2, 3, 4 };
    STD::list<int> rv(r3, r3 + 4);
    rv.reverse();
    test_eq( sig(rv), 4321L );
    test_pass("cxx03:list::reverse");

    int r4[] = { 3, 1, 4, 1, 5, 2 };
    STD::list<int> s(r4, r4 + 6);
    s.sort();
    test_eq( sig(s), 112345L );
    test_pass("cxx03:list::sort");
    s.sort(Less3());
    test_eq( sig(s), 112345L );
    test_pass("cxx03:list::sort (comp)");
}

TEST_CASE(list, merge_swap) {
    int a1[] = { 1, 3, 5 };
    int a2[] = { 2, 4, 6 };
    STD::list<int> a(a1, a1 + 3);
    STD::list<int> b(a2, a2 + 3);
    a.merge(b);
    test_eq( sig(a), 123456L );
    test_true( b.empty() );
    test_pass("cxx03:list::merge");

    STD::list<int> c(a1, a1 + 3);
    STD::list<int> d(a2, a2 + 3);
    c.merge(d, Less3());
    test_eq( sig(c), 123456L );
    test_pass("cxx03:list::merge (comp)");

    STD::list<int> p(a1, a1 + 3);
    STD::list<int> q(a2, a2 + 3);
    p.swap(q);
    test_eq( sig(p), 246L );
    test_eq( sig(q), 135L );
    test_pass("cxx03:list::swap");
    using STD::swap;
    swap(p, q);
    test_eq( sig(p), 135L );
    test_pass("cxx03:swap");
}

TEST_CASE(list, emplace_compare_allocator) {
    STD::list<int> l;
#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
    l.emplace_back(3);
    l.emplace_front(1);
    test_eq( l.back(), 3 );
    test_pass("cxx11:list::emplace_back");
    test_eq( l.front(), 1 );
    test_pass("cxx11:list::emplace_front");
    STD::list<int>::iterator it = l.begin(); ++it;
    l.emplace(it, 2);
    test_eq( sig(l), 123L );
    test_pass("cxx11:list::emplace");
#else
    l.push_back(1); l.push_back(2); l.push_back(3);
    TEST_SKIP_N(3);
    test_skip("cxx11:list::emplace_back");
    test_skip("cxx11:list::emplace_front");
    test_skip("cxx11:list::emplace");
#endif

    int raw[] = { 1, 2, 3 };
    STD::list<int> x(raw, raw + 3);
    STD::list<int> y(raw, raw + 3);
    STD::list<int> z(raw, raw + 2);
    test_true( x == y );
    test_pass("cxx03:list::operator==");
    test_true( x != z );
    test_pass("cxx03:list::operator!=");
    test_true( z <  x );
    test_pass("cxx03:list::operator<");
    test_true( x >  z );
    test_pass("cxx03:list::operator>");
    test_true( x <= y );
    test_pass("cxx03:list::operator<=");
    test_true( x >= y );
    test_pass("cxx03:list::operator>=");

    STD::list<int>::allocator_type al = x.get_allocator();
    (void)al;
    test_true( al == STD::list<int>::allocator_type() );
    test_pass("cxx03:list::get_allocator");
    test_true( x.max_size() > 0u );
    test_pass("cxx03:list::max_size");
}

TEST_CASE(list, string_elements) {

    STD::list<STD::string> l;
    l.push_back("b");
    l.push_front("a");
    l.push_back("c");
    test_eq( l.front(), STD::string("a") );
    test_eq( l.back(), STD::string("c") );
    test_eq( l.size(), 3u );
    test_pass("cxx03:list::push_front");

    STD::list<STD::string>::iterator it = l.begin();
    ++it;
    l.insert(it, "X");
    test_eq( *(++l.begin()), STD::string("X") );
    test_pass("cxx03:list::insert#string");

    l.remove("X");
    test_eq( l.size(), 3u );
    test_pass("cxx03:list::remove#string");

    l.push_back("a");
    l.sort();
    test_eq( l.front(), STD::string("a") );
    test_pass("cxx03:list::sort#string");
    l.unique();
    test_eq( l.size(), 3u );
    test_pass("cxx03:list::unique#string");

    STD::list<STD::string> other;
    other.push_back("z");
    l.splice(l.end(), other);
    test_eq( l.back(), STD::string("z") );
    test_true( other.empty() );
    test_pass("cxx03:list::splice#string");

    STD::list<STD::string> big;
    big.push_back(STD::string(400, 'q'));
    test_eq( big.front().size(), 400u );
    test_pass("cxx03:list#string_heap");

    l.clear();
    test_true( l.empty() );
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(list, erase_free) {
    int raw[] = { 1, 2, 3, 2, 5, 2 };
    STD::list<int> l(raw, raw + 6);
    STD::list<int>::size_type n = STD::erase(l, 2);
    test_eq( (long)n, 3L );
    test_eq( sig(l), 135L );
    test_pass("cxx20:erase(list)");

    STD::list<int> m(raw, raw + 6);
    STD::list<int>::size_type k = STD::erase_if(m, IsEven());
    test_eq( (long)k, 3L );
    test_eq( sig(m), 135L );
    test_pass("cxx20:erase_if(list)");
}
#else
TEST_CASE_SKIP(list, erase_free)
#endif

static int list_sum(const STD::list<int>& l) {
    int s = 0;
    for (STD::list<int>::const_iterator it = l.begin(); it != l.end(); ++it) s += *it;
    return s;
}

TEST_CASE(list, ctors_ops_fill) {
    typedef STD::list<int> L;

#if _tst_cplusplus >= 201103L
    L il = { 1, 2, 3 };
    test_eq( list_sum(il), 6 );
    test_pass("cxx11:list::list(initializer_list)");
#else
    test_skip("cxx11:list::list(initializer_list)");
#endif

    STD::allocator<int> al;
    L la(al);
    la.push_back(9);
    test_eq( la.front(), 9 );
    test_pass("cxx11:list::list(const Allocator&) family");

    static const int r456[3] = { 4, 5, 6 };
    L m1(r456, r456 + 3);
    L m2(STD::move(m1));
    TEST_SKIP_NATIVE_NO_MOVE("no move below C++11: the type here is the native library's own, and a move cannot be added to it");
    test_true( list_sum(m2) == 15 && m1.empty() );
    test_pass("cxx11:list::list(list&&)");
    L m3;
    m3 = STD::move(m2);
    TEST_SKIP_NATIVE_NO_MOVE("no move below C++11: the type here is the native library's own, and a move cannot be added to it");
    test_true( list_sum(m3) == 15 && m2.empty() );
    test_pass("cxx11:list::operator=(list&&)");

    L a2;
#if _tst_cplusplus >= 201103L
    a2 = { 7, 8 };
    test_eq( list_sum(a2), 15 );
    test_pass("cxx11:list::operator=(initializer_list)");
    a2.assign({ 1, 1, 1 });
    test_eq( list_sum(a2), 3 );
    test_pass("cxx11:list::assign(initializer_list)");
#else
    test_skip("cxx11:list::operator=(initializer_list)");
    test_skip("cxx11:list::assign(initializer_list)");
#endif

    STD::list<STD::string> sl;
    STD::string b("back"), f("front");
    sl.push_back(STD::move(b));
    test_eq( sl.back(), STD::string("back") );
    test_pass("cxx11:list::push_back(T&&)");
    sl.push_front(STD::move(f));
    test_eq( sl.front(), STD::string("front") );
    test_pass("cxx11:list::push_front(T&&)");
    STD::string m("mid");
    sl.insert(++sl.begin(), STD::move(m));
    test_eq( (long)sl.size(), 3L );
    test_pass("cxx11:list::insert(T&&)");
#if _tst_cplusplus >= 201103L
    L ins = { 1 };
    ins.insert(ins.end(), { 2, 3 });
    test_eq( list_sum(ins), 6 );
    test_pass("cxx11:list::insert(initializer_list)");
#else
    test_skip("cxx11:list::insert(initializer_list)");
#endif

    static const int r123[3] = { 1, 2, 3 };
    L it(r123, r123 + 3);
    test_eq( *it.rbegin(), 3 );
    test_true( it.rbegin() != it.rend() );
    test_pass("cxx03:list::rend");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_true( it.cbegin() != it.cend() );
    test_pass("cxx11:list::cend");
#else
    TEST_SKIP1();
    test_skip("cxx11:list::cend");
#endif
}

TEST_CASE(list, splice_merge_compare) {
    typedef STD::list<int> L;

    static const int r12[2] = { 1, 2 }, r34[2] = { 3, 4 };
    L a(r12, r12 + 2), b(r34, r34 + 2);
    a.splice(a.end(), STD::move(b));
    test_true( list_sum(a) == 10 && b.empty() );
    test_pass("cxx11:list::splice(list&&)");

    static const int r135[3] = { 1, 3, 5 }, r246[3] = { 2, 4, 6 };
    L g(r135, r135 + 3), h(r246, r246 + 3);
    g.merge(STD::move(h));
    test_true( list_sum(g) == 21 && h.empty() );
    int prev = 0; bool sorted = true;
    for (L::iterator it = g.begin(); it != g.end(); ++it) { if (*it < prev) sorted = false; prev = *it; }
    test_true( sorted );
    test_pass("cxx11:list::merge(list&&)");

#if defined(__cpp_lib_three_way_comparison)
    L l1 = { 1, 2, 3 }, l2 = { 1, 2, 4 };
    test_true( (l1 <=> l2) < 0 && (l1 <=> l1) == 0 );
    test_pass("cxx20:operator<=>(list)");
#else
    test_skip("cxx20:operator<=>(list)");
#endif
}

TEST_CASE(list, cxx23_ranges) {
#if defined(__cpp_lib_containers_ranges)
    STD::vector<int> v; v.push_back(1); v.push_back(2); v.push_back(3);
    STD::list<int> fr(STD::from_range, v);
    test_eq( list_sum(fr), 6 );
    test_pass("cxx23:list::list(from_range_t,R&&)");
    STD::vector<int> v2; v2.push_back(4); v2.push_back(5);
    fr.append_range(v2);
    test_eq( list_sum(fr), 15 );
    test_pass("cxx23:list::append_range");
    fr.prepend_range(v2);
    test_eq( list_sum(fr), 24 );
    test_pass("cxx23:list::prepend_range");
    STD::list<int> ir = { 1 };
    ir.insert_range(ir.end(), v2);
    test_eq( list_sum(ir), 10 );
    test_pass("cxx23:list::insert_range");
    STD::list<int> ar;
    ar.assign_range(v);
    test_eq( list_sum(ar), 6 );
    test_pass("cxx23:list::assign_range");
#else
    test_skip("cxx23:list::list(from_range_t,R&&)");
    test_skip("cxx23:list::append_range");
    test_skip("cxx23:list::prepend_range");
    test_skip("cxx23:list::insert_range");
    test_skip("cxx23:list::assign_range");
#endif
}


TEST_CASE(list, member_types_cxx03) {
    typedef STD::list<int> C;
    C c; c.push_back(1); c.push_back(2);
    const C& cc = c;

    test_true(( STD::is_same<C::value_type, int>::value ));
    test_pass("cxx03:list::value_type");

    C::reference r = c.front();
    r = 5;
    test_eq( c.front(), 5 );
    test_true(( STD::is_same<C::reference, int&>::value ));
    test_pass("cxx03:list::reference");

    C::const_reference cr = cc.front();
    test_eq( cr, 5 );
    test_true(( STD::is_same<C::const_reference, const int&>::value ));
    test_pass("cxx03:list::const_reference");

    C::pointer p = &c.front();
    test_eq( *p, 5 );
    test_pass("cxx03:list::pointer");

    C::const_pointer cp = &cc.front();
    test_eq( *cp, 5 );
    test_pass("cxx03:list::const_pointer");

    C::size_type n = c.size();
    test_eq( n, (STD::size_t)2 );
    test_true( !STD::is_signed<C::size_type>::value );
    test_pass("cxx03:list::size_type");

    C::difference_type d = 0;
    for (C::const_iterator i = cc.begin(); i != cc.end(); ++i) ++d;
    test_eq( (long)d, 2L );
    test_true( STD::is_signed<C::difference_type>::value );
    test_pass("cxx03:list::difference_type");

    C::iterator it = c.begin();
    *it = 9;
    test_eq( c.front(), 9 );
    test_true( tst_type_is<C::iterator>(c.begin()) );
    test_pass("cxx03:list::iterator");

    C::const_iterator ci = cc.begin();
    test_eq( *ci, 9 );
    test_true( tst_type_is<C::const_iterator>(cc.begin()) );
    test_pass("cxx03:list::const_iterator");

    C::reverse_iterator ri = c.rbegin();
    test_eq( *ri, 2 );
    test_pass("cxx03:list::reverse_iterator");

    C::const_reverse_iterator cri = cc.rbegin();
    test_eq( *cri, 2 );
    test_pass("cxx03:list::const_reverse_iterator");
}

TEST_CASE(list, allocator_type_cxx11) {
    typedef STD::list<int> C;
    C c;
    test_true(( STD::is_same<C::allocator_type, STD::allocator<int> >::value ));
    C::allocator_type al = c.get_allocator();
    (void)al;
    test_pass("cxx03:list::allocator_type");
}

TEST_CASE(list, feature_test_macros) {
#if defined(__cpp_lib_list_remove_return_type)
    test_true( __cpp_lib_list_remove_return_type > 0L );
    test_pass("cxx20:__cpp_lib_list_remove_return_type");
#else
    test_skip("cxx20:__cpp_lib_list_remove_return_type");
#endif
#if defined(__cpp_lib_containers_ranges)
    test_true( __cpp_lib_containers_ranges > 0L );
    test_pass("cxx23:__cpp_lib_containers_ranges");
#else
    test_skip("cxx23:__cpp_lib_containers_ranges");
#endif
#if defined(__cpp_lib_constexpr_list)
    test_true( __cpp_lib_constexpr_list > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_list");
    {
        struct K {
            static constexpr int run() {
                STD::list<int> l;
                l.push_back(2);
                l.push_front(1);
                l.push_back(3);
                l.remove(2);
                l.reverse();
                return (int)(l.size() * 10 + l.front());
            }
        };
        static_assert(K::run() == 23, "list operations in a constant expression");
        test_eq( K::run(), 23 );
    }
    test_pass("cxx26:constexpr list operations");
#else
    TEST_NOTE("constexpr list is C++26 (P3372); not in this library");
    test_skip("cxx26:__cpp_lib_constexpr_list");
    test_skip("cxx26:constexpr list operations");
#endif
}

#if !_TST_HAS_ALIAS_TEMPLATE || !_TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE_SKIP(list, pmr_alias)
#else
TEST_CASE(list, pmr_alias) {
    test_true(( STD::is_same<STD::pmr::list<int>,
                STD::list<int, STD::pmr::polymorphic_allocator<int> > >::value ));

    unsigned char buf[1024];
    STD::pmr::monotonic_buffer_resource res(buf, sizeof buf);
    STD::pmr::list<int> l(&res);
    l.push_back(1);
    l.push_front(0);
    test_eq( l.size(), STD::size_t(2) );
    test_eq( l.front(), 0 );
    test_eq( l.back(), 1 );
    test_true( l.get_allocator().resource() == &res );
    test_pass("cxx17:pmr::list");
}
#endif

TEST_CASE(list, overload_forms) {
    typedef STD::list<int> L;
    static const int r4[4] = { 1, 2, 3, 4 };

    {
        L a(r4, r4 + 4);
        L::iterator none = a.erase(a.begin(), a.begin());
        test_eq( *none, 1 );
        test_eq( (long)a.size(), 4L );

        L::iterator mid = a.begin(); ++mid; ++mid;
        L::iterator er = a.erase(a.begin(), mid);
        test_eq( *er, 3 );
        test_eq( (long)a.size(), 2L );

        L::iterator all = a.erase(a.begin(), a.end());
        test_true( all == a.end() );
        test_true( a.empty() );
    }
    test_pass("cxx03:list::erase (range)");

    {
        L b;
        b.resize(3);
        test_eq( (long)b.size(), 3L );
        test_eq( b.front(), 0 );
        test_eq( b.back(), 0 );
        b.resize(3);
        test_eq( (long)b.size(), 3L );
        b.resize(1);
        test_eq( (long)b.size(), 1L );
        b.resize(0);
        test_true( b.empty() );
    }
    test_pass("cxx11:list::resize(size_type n)");

    {
        L b(r4, r4 + 2);
        b.resize(4, 9);
        test_eq( (long)b.size(), 4L );
        test_eq( b.front(), 1 );
        test_eq( b.back(), 9 );
        b.resize(4, 7);
        test_eq( b.back(), 9 );
        b.resize(1, 7);
        test_eq( (long)b.size(), 1L );
        test_eq( b.back(), 1 );
    }
    test_pass("cxx11:list::resize(size_type n, const T& c)");

    {
        L c(r4, r4 + 2), d(r4 + 2, r4 + 4);
        c.splice(c.end(), STD::move(d), d.begin());
        test_eq( (long)c.size(), 3L );
        test_eq( (long)d.size(), 1L );
        test_eq( c.back(), 3 );
        test_eq( d.front(), 4 );

        c.splice(c.begin(), STD::move(d), d.begin());
        test_eq( c.front(), 4 );
        test_true( d.empty() );

        L::iterator second = c.begin(); ++second;
        c.splice(second, STD::move(c), c.begin());
        test_eq( (long)c.size(), 4L );
        test_eq( c.front(), 4 );
    }
    test_pass("cxx11:list::splice(list&&, iterator)");

    {
        L e(r4, r4 + 2), f(r4 + 2, r4 + 4);
        e.splice(e.end(), STD::move(f), f.begin(), f.begin());
        test_eq( (long)e.size(), 2L );
        test_eq( (long)f.size(), 2L );

        e.splice(e.end(), STD::move(f), f.begin(), f.end());
        test_eq( (long)e.size(), 4L );
        test_true( f.empty() );
        test_eq( e.back(), 4 );
    }
    test_pass("cxx11:list::splice(list&&, iterator, iterator)");

    {
        L g, h;
        g.push_back(5); g.push_back(1);
        h.push_back(4); h.push_back(2);
        g.merge(STD::move(h), STD::greater<int>());
        test_eq( (long)g.size(), 4L );
        test_true( h.empty() );
        test_eq( g.front(), 5 );
        test_eq( g.back(), 1 );
        L::iterator gi = g.begin(); ++gi;
        test_eq( *gi, 4 );
        ++gi;
        test_eq( *gi, 2 );

        L empty_l;
        g.merge(STD::move(empty_l), STD::greater<int>());
        test_eq( (long)g.size(), 4L );

        L only;
        only.push_back(3);
        only.merge(STD::move(g), STD::greater<int>());
        test_eq( (long)only.size(), 5L );
        test_eq( only.front(), 5 );
        test_true( g.empty() );
    }
    test_pass("cxx11:list::merge(list&&, Compare)");
}
