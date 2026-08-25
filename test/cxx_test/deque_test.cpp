#include "test_cxx.hpp"
#include <deque>

TEST_CASE(deque, header_compiles) { test_true( true ); test_pass("cxx03:deque#header"); }

#include <type_traits>

template <class _Want, class _Got>
bool same_type(const _Got&) { return STD::is_same<_Want, _Got>::value; }

#if TEST_TARGET_CXX >= 2011
#include <deque>
#include <memory_resource>
#include <algorithm>
#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using _ccw::string_view;

namespace {

struct Item {
    static int live;
    int         id;
    STD::string tag;

    Item() : id(0), tag("-")            { ++live; }
    explicit Item(int i) : id(i), tag("i") { ++live; }
    Item(int i, const char* t) : id(i), tag(t) { ++live; }
    Item(const Item& o) : id(o.id), tag(o.tag) { ++live; }
    Item& operator=(const Item& o) { id = o.id; tag = o.tag; return *this; }
#if _tst_cplusplus >= 201103L
    Item(Item&& o) noexcept : id(o.id), tag(STD::move(o.tag)) { ++live; o.id = -1; }
    Item& operator=(Item&& o) noexcept { id = o.id; tag = STD::move(o.tag); o.id = -1; return *this; }
#endif
    ~Item() { --live; }

    friend bool operator==(const Item& a, const Item& b) {
        return a.id == b.id && a.tag == b.tag;
    }
    friend bool operator!=(const Item& a, const Item& b) { return !(a == b); }
#if _tst_cplusplus >= 202002L
    friend auto operator<=>(const Item& a, const Item& b) {
        if (auto c = a.id <=> b.id; c != 0) return c;
        return a.tag <=> b.tag;
    }
#else

    friend bool operator<(const Item& a, const Item& b) {
        if (a.id != b.id) return a.id < b.id;
        return a.tag < b.tag;
    }
    friend bool operator> (const Item& a, const Item& b) { return b < a; }
    friend bool operator<=(const Item& a, const Item& b) { return !(b < a); }
    friend bool operator>=(const Item& a, const Item& b) { return !(a < b); }
#endif
};
int Item::live = 0;

template <class D>
STD::string join(const D& d) {
    STD::string s;
    for (typename D::const_iterator it = d.begin(); it != d.end(); ++it) {
        if (!s.empty()) s += ',';
        s += STD::to_string(*it);
    }
    return s;
}

inline bool is_even_i(int x)                  { return x % 2 == 0; }
inline bool always_true_i(int)                { return true; }
inline bool len_is_2(const STD::string& s)    { return s.size() == 2; }
inline bool id_is_odd(const Item& i)          { return i.id % 2 != 0; }
inline bool id_gt_2(const Item& i)            { return i.id > 2; }

}

TEST_CASE(deque, member_types) {
    typedef STD::deque<int> D;
    test_true( (STD::is_same<D::value_type, int>::value) );
    test_true( (STD::is_same<D::allocator_type, STD::allocator<int> >::value) );
    test_true( (STD::is_same<D::pointer, int*>::value) );
    test_true( (STD::is_same<D::const_pointer, const int*>::value) );
    test_true( (STD::is_same<D::reference, int&>::value) );
    test_true( (STD::is_same<D::const_reference, const int&>::value) );
    test_true( (STD::is_unsigned<D::size_type>::value) );
    test_true( (STD::is_signed<D::difference_type>::value) );
    test_true( (STD::is_same<D::difference_type, STD::ptrdiff_t>::value) );
    test_pass("cxx03:deque#member_types");

#if _TST_IS_CONVERTIBLE_CLASS
    test_true( (STD::is_convertible<D::iterator, D::const_iterator>::value) );
    test_true( !(STD::is_convertible<D::const_iterator, D::iterator>::value) );
#else
    D::iterator it_ = D().begin();
    D::const_iterator cit_ = it_;
    (void)cit_;
    TEST_SKIP1(); TEST_SKIP1();
#endif
    test_true( (STD::is_base_of<STD::random_access_iterator_tag,
                STD::iterator_traits<D::iterator>::iterator_category>::value) );
    test_true( (STD::is_same<STD::iterator_traits<D::iterator>::value_type, int>::value) );
    test_pass("cxx03:deque::iterator");

    test_true( (STD::is_same<D::reverse_iterator,
                             STD::reverse_iterator<D::iterator> >::value) );
    test_true( (STD::is_same<D::const_reverse_iterator,
                             STD::reverse_iterator<D::const_iterator> >::value) );
    test_pass("cxx03:deque::reverse_iterator");

    typedef STD::deque<Item> DI;
    test_true( (STD::is_same<DI::value_type, Item>::value) );
    test_true( (STD::is_same<DI::reference, Item&>::value) );
    test_pass("cxx03:deque#member_types (Item)");
}

TEST_CASE(deque, constructors_int) {
    STD::allocator<int> al;

    STD::deque<int> d0;
    test_true( d0.empty() );
    test_eq( d0.size(), STD::size_t(0) );
    test_pass("cxx03:deque::deque()");

    STD::deque<int> d1(al);
    test_true( d1.empty() );
    test_pass("cxx11:deque::deque(const Allocator&)");

    STD::deque<int> d2(4);
    test_eq( d2.size(), STD::size_t(4) );
    test_eq( d2[0], 0 );
    test_eq( d2[3], 0 );
    test_pass("cxx03:deque::deque(n)");

#if _TST_HAS_CONTAINER_N_ALLOC
    STD::deque<int> d2a(4, al);
    test_eq( d2a.size(), STD::size_t(4) );
    test_eq( d2a[2], 0 );
    test_pass("cxx11:deque::deque(n,Allocator)");
#else
    TEST_NOTE("deque(size_type, const Allocator&) is LWG 2210; MSVC ships it from VS2017");
    TEST_SKIP_N(2);
    test_skip("cxx11:deque::deque(n,Allocator)");
#endif

    STD::deque<int> d3(3, 7);
    test_str_eq( join(d3), "7,7,7" );
    test_pass("cxx03:deque::deque(n,value)");
    STD::deque<int> d3a(3, 7, al);
    test_true( d3a == d3 );
    test_pass("cxx11:deque::deque(n,value,Allocator)");

    int raw[] = { 1, 2, 3, 4 };
    STD::deque<int> d4(raw, raw + 4);
    test_str_eq( join(d4), "1,2,3,4" );
    test_pass("cxx03:deque::deque(first,last)");
    STD::deque<int> d4a(raw, raw + 4, al);
    test_true( d4a == d4 );
    test_pass("cxx11:deque::deque(first,last,Allocator)");

    STD::list<int> src(raw, raw + 4);
    STD::deque<int> d4b(src.begin(), src.end());
    test_true( d4b == d4 );
    test_pass("cxx03:deque::deque(input_iterator)");

    const int vec_init[] = { 5, 6, 7 };
    STD::vector<int> vec(vec_init, vec_init + 3);
#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
    STD::deque<int> d5(STD::from_range, vec);
    test_str_eq( join(d5), "5,6,7" );
    test_pass("cxx23:deque::deque(from_range_t,R)");
    STD::deque<int> d5a(STD::from_range, vec, al);
    test_true( d5a == d5 );
    test_pass("cxx23:deque::deque(from_range_t,R,Allocator)");
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    STD::deque<int> d6(d4);
    test_true( d6 == d4 );
    test_pass("cxx03:deque::deque(const deque&)");
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::deque<int> d6a(d4, al);
    test_true( d6a == d4 );
    test_pass("cxx11:deque::deque(const deque&,Allocator)");

    STD::deque<int> tmp(d4);
    STD::deque<int> d7(STD::move(tmp));
    test_true( d7 == d4 );
    test_true( tmp.empty() );
    test_pass("cxx11:deque::deque(deque&&)");

    STD::deque<int> tmp2(d4);
    STD::deque<int> d7a(STD::move(tmp2), al);
    test_true( d7a == d4 );
    test_pass("cxx11:deque::deque(deque&&,Allocator)");
#else
    test_skip("cxx11:deque::deque(const deque&,Allocator)");
    test_skip("cxx11:deque::deque(deque&&)");
    test_skip("cxx11:deque::deque(deque&&,Allocator)");
#endif

    const int d8_init[] = { 9, 8, 7 };
    STD::deque<int> d8(d8_init, d8_init + 3);
    test_str_eq( join(d8), "9,8,7" );
#if _tst_cplusplus >= 201103L
    STD::deque<int> d8a({ 9, 8, 7 }, al);
#else
    STD::deque<int> d8a(d8_init, d8_init + 3, al);
#endif
    test_true( d8a == d8 );
#if _tst_cplusplus >= 201103L
    test_pass("cxx11:deque::deque(initializer_list,Allocator)");
#endif

}

TEST_CASE(deque, constructors_string_and_item) {

    STD::deque<STD::string> s0;
    test_true( s0.empty() );

    STD::deque<STD::string> s1(3, STD::string("ab"));
    test_eq( s1.size(), STD::size_t(3) );
    test_str_eq( s1[1], "ab" );
    test_pass("cxx03:deque::deque(n,value)#string");

    const STD::string s2_init[] = { "x", "yy", "zzz" };
    STD::deque<STD::string> s2(s2_init, s2_init + 3);
    test_eq( s2.size(), STD::size_t(3) );
    test_str_eq( s2.front(), "x" );
    test_str_eq( s2.back(), "zzz" );
    test_pass("cxx03:deque::deque(first,last)#string");

    STD::deque<STD::string> s3(s2.begin(), s2.end());
    test_true( s3 == s2 );
    test_pass("cxx03:deque::deque(iterator_range)#string");

    STD::deque<STD::string> s4(s2);
    STD::deque<STD::string> s5(STD::move(s4));
    test_true( s5 == s2 );
    test_pass("cxx11:deque::deque(deque&&)#string");

    STD::deque<STD::string> s6(2);
    test_true( s6[0].empty() );
    test_pass("cxx03:deque::deque(n)#string");

    int base = Item::live;
    {
        STD::deque<Item> a(3, Item(5, "k"));
        test_eq( a.size(), STD::size_t(3) );
        test_eq( a[2].id, 5 );
        test_str_eq( a[2].tag, "k" );

        STD::deque<Item> b(a);
        test_true( b == a );

        STD::deque<Item> c(STD::move(b));
        test_true( c == a );

        STD::deque<Item> d(a.begin(), a.end());
        test_true( d == a );

#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
        STD::deque<Item> e(STD::from_range, a);
        test_true( e == a );
#else
        TEST_SKIP1();
#endif

        STD::deque<Item> f(2);
        test_eq( f[0].id, 0 );
        test_str_eq( f[1].tag, "-" );

        test_gt( Item::live, base );
    }
    test_eq( Item::live, base );
    test_pass("cxx03:deque#element_lifetime");
}

#if !_TST_HAS_CTAD
TEST_CASE_SKIP(deque, deduction_guides)
#else
TEST_CASE(deque, deduction_guides) {
    const int v_init[] = { 1, 2, 3 };
    STD::vector<int> v(v_init, v_init + 3);
    STD::allocator<int> al;

    STD::deque d1(v.begin(), v.end());
    test_true( (STD::is_same<decltype(d1), STD::deque<int> >::value) );
    test_str_eq( join(d1), "1,2,3" );
    test_pass("cxx17:deque#ctad(It,It)");

    STD::deque d2(v.begin(), v.end(), al);
    test_true( (STD::is_same<decltype(d2), STD::deque<int, STD::allocator<int> > >::value) );
    test_true( d2 == d1 );
    test_pass("cxx17:deque#ctad(It,It,Alloc)");

#if !defined(__GNUC__) && defined(__cpp_lib_containers_ranges)
    STD::deque d3(STD::from_range, v);
    test_true( (STD::is_same<decltype(d3), STD::deque<int> >::value) );
    test_true( d3 == d1 );
    test_pass("cxx23:deque#ctad(from_range,R)");

    STD::deque d4(STD::from_range, v, al);
    test_true( (STD::is_same<decltype(d4), STD::deque<int, STD::allocator<int> > >::value) );
    test_true( d4 == d1 );
    test_pass("cxx23:deque#ctad(from_range,R,Alloc)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif

    const STD::string vs_init[] = { "a", "b" };
    STD::vector<STD::string> vs(vs_init, vs_init + 2);
    STD::deque d5(vs.begin(), vs.end());
    test_true( (STD::is_same<decltype(d5), STD::deque<STD::string> >::value) );
    test_str_eq( d5.back(), "b" );
    test_pass("cxx17:deque#ctad_string");
}

#endif

#if !_TST_HAS_ALIAS_TEMPLATE || !_TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE_SKIP(deque, pmr_alias)
#else
TEST_CASE(deque, pmr_alias) {

    test_true( (STD::is_same<STD::pmr::deque<int>,
                STD::deque<int, STD::pmr::polymorphic_allocator<int> > >::value) );
    test_pass("cxx17:pmr::deque");

    unsigned char buf[1024];
    STD::pmr::monotonic_buffer_resource res(buf, sizeof buf);
    STD::pmr::deque<int> d(&res);
    d.push_back(1);
    d.push_back(2);
    d.push_front(0);
    test_eq( d.size(), STD::size_t(3) );
    test_eq( d.front(), 0 );
    test_eq( d.back(), 2 );
    test_true( d.get_allocator().resource() == &res );
    test_pass("cxx17:pmr::deque#uses_resource");

#if _tst_cplusplus >= 201103L
    STD::pmr::deque<STD::string> ds({ "aa", "bb" }, &res);
#else
    (void)0;
#endif
    test_eq( ds.size(), STD::size_t(2) );
    test_str_eq( ds[1], "bb" );
    test_pass("cxx17:pmr::deque#string");
}

#endif

TEST_CASE(deque, assignment_and_allocator) {
    const int a_init[] = { 1, 2, 3 };
    STD::deque<int> a(a_init, a_init + 3);
    STD::deque<int> b;

    b = a;
    test_true( b == a );
    test_pass("cxx03:deque::operator=(const deque&)");

    STD::deque<int> c;
    STD::deque<int> tmp = a;
    c = STD::move(tmp);
    test_true( c == a );
    test_pass("cxx11:deque::operator=(deque&&)");

#if _tst_cplusplus >= 201103L
    c = { 4, 5 };
#else

    {
        const int c_init[] = { 4, 5 };
        c.assign(c_init, c_init + 2);
    }
#endif
    test_str_eq( join(c), "4,5" );
#if _tst_cplusplus >= 201103L
    test_pass("cxx11:deque::operator=(initializer_list)");
#endif

    int raw[] = { 7, 8, 9, 10 };
    c.assign(raw, raw + 4);
    test_str_eq( join(c), "7,8,9,10" );
    test_pass("cxx03:deque::assign(first,last)");

    STD::list<int> lst(raw, raw + 3);
    c.assign(lst.begin(), lst.end());
    test_str_eq( join(c), "7,8,9" );
    test_pass("cxx03:deque::assign(input_iterator)");

    c.assign(STD::size_t(2), 6);
    test_str_eq( join(c), "6,6" );
    test_pass("cxx03:deque::assign(n,value)");

#if _tst_cplusplus >= 201103L
    c.assign({ 1, 1, 1, 1 });
#else
    { static const int a4[] = { 1, 1, 1, 1 }; c.assign(a4, a4 + 4); }
#endif
    test_eq( c.size(), STD::size_t(4) );
#if _tst_cplusplus >= 201103L
    test_pass("cxx11:deque::assign(initializer_list)");
#endif

#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
    const int src_init[] = { 3, 2, 1 };
    STD::vector<int> src(src_init, src_init + 3);
    c.assign_range(src);
    test_str_eq( join(c), "3,2,1" );
    test_pass("cxx23:deque::assign_range");
#else
    TEST_SKIP1();
#endif

    STD::deque<int> g;
    STD::allocator<int> al = g.get_allocator();
    test_true( al == STD::allocator<int>() );
    test_true( same_type<STD::allocator<int> >(g.get_allocator()) );
    test_pass("cxx03:deque::get_allocator");

    int base = Item::live;
    {
        const Item x_init[] = { Item(1), Item(2) };
        STD::deque<Item> x(x_init, x_init + 2);
        STD::deque<Item> y;
        y = x;
        test_true( y == x );
        y.assign(STD::size_t(3), Item(9));
        test_eq( y.size(), STD::size_t(3) );
        test_eq( y[2].id, 9 );
        y.assign(x.begin(), x.end());
        test_true( y == x );
#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
        const Item vi_init[] = { Item(4), Item(5) };
        STD::vector<Item> vi(vi_init, vi_init + 2);
        y.assign_range(vi);
        test_eq( y.size(), STD::size_t(2) );
        test_eq( y.front().id, 4 );
#else
        TEST_SKIP1(); TEST_SKIP1();
#endif
        STD::deque<Item> z;
        z = STD::move(y);
        test_eq( z.back().id, 5 );
    }
    test_eq( Item::live, base );
    test_pass("cxx03:deque#assign_lifetime");

    STD::deque<STD::string> s;
#if _tst_cplusplus >= 201103L
    s.assign({ "p", "q" });
#else
    { static const char* const pq[] = { "p", "q" }; s.assign(pq, pq + 2); }
#endif
    test_str_eq( s.back(), "q" );
    s.assign(STD::size_t(2), STD::string("r"));
    test_str_eq( s.front(), "r" );
    test_pass("cxx03:deque::assign#string");
}

TEST_CASE(deque, iterators) {
    const int d_init[] = { 1, 2, 3, 4 };
    STD::deque<int> d(d_init, d_init + 4);
    const STD::deque<int>& cd = d;

    test_eq( *d.begin(), 1 );
    test_eq( *cd.begin(), 1 );
    test_pass("cxx03:deque::begin");
    test_eq( *(d.end() - 1), 4 );
    test_eq( *(cd.end() - 1), 4 );
    test_eq( (int)(d.end() - d.begin()), 4 );
    test_pass("cxx03:deque::end");

    test_eq( *d.rbegin(), 4 );
    test_eq( *cd.rbegin(), 4 );
    test_pass("cxx03:deque::rbegin");
    test_eq( *(d.rend() - 1), 1 );
    test_eq( *(cd.rend() - 1), 1 );
    test_pass("cxx03:deque::rend");

#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( *d.cbegin(), 1 );
    test_pass("cxx11:deque::cbegin");
    test_eq( *(d.cend() - 1), 4 );
    test_pass("cxx11:deque::cend");
    test_eq( *d.crbegin(), 4 );
    test_pass("cxx11:deque::crbegin");
    test_eq( *(d.crend() - 1), 1 );
    test_pass("cxx11:deque::crend");
#else
    test_skip("cxx11:deque::cbegin"); test_skip("cxx11:deque::cend");
    test_skip("cxx11:deque::crbegin"); test_skip("cxx11:deque::crend");
#endif

#if _TST_HAS_CXX11_LIB_MEMBERS
    test_true( same_type<STD::deque<int>::const_iterator>(d.cbegin()) );
    test_true( same_type<STD::deque<int>::const_reverse_iterator>(d.crbegin()) );
    test_true( d.cbegin() == cd.begin() );
    test_true( d.cend() == cd.end() );
    test_true( d.crbegin() == cd.rbegin() );
    test_true( d.crend() == cd.rend() );
#endif

    *d.begin() = 10;
    test_eq( d.front(), 10 );
    *d.rbegin() = 40;
    test_eq( d.back(), 40 );

    STD::vector<int> fwd(d.begin(), d.end());
    STD::vector<int> rev(d.rbegin(), d.rend());
    STD::reverse(rev.begin(), rev.end());
    test_true( fwd == rev );
    test_pass("cxx03:deque#reverse_traversal");

    STD::deque<int> e;
    test_true( e.begin() == e.end() );
    test_true( e.rbegin() == e.rend() );
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_true( e.cbegin() == e.cend() );
#endif
    test_pass("cxx03:deque#empty_iterators");

    const Item di_init[] = { Item(1, "a"), Item(2, "b") };
    STD::deque<Item> di(di_init, di_init + 2);
    test_str_eq( di.begin()->tag, "a" );
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( di.cbegin()->id, 1 );
#endif
    test_str_eq( di.rbegin()->tag, "b" );
    test_pass("cxx03:deque#iterator_arrow");

    const STD::string ds_init[] = { "x", "y", "z" };
    STD::deque<STD::string> ds(ds_init, ds_init + 3);
    STD::string cat;

    for (STD::deque<STD::string>::const_iterator it = ds.begin(); it != ds.end(); ++it)
        cat += *it;
    test_str_eq( cat, "xyz" );
}

TEST_CASE(deque, capacity) {
    STD::deque<int> d;
    test_true( d.empty() );
    test_pass("cxx03:deque::empty");
    test_eq( d.size(), STD::size_t(0) );
    test_pass("cxx03:deque::size");
    test_gt( d.max_size(), STD::size_t(0) );
    test_pass("cxx03:deque::max_size");

    d.push_back(1);
    test_true( !d.empty() );
    test_eq( d.size(), STD::size_t(1) );
    test_ge( d.max_size(), d.size() );

    d.resize(4);
    test_eq( d.size(), STD::size_t(4) );
    test_str_eq( join(d), "1,0,0,0" );

    d.resize(2);
    test_str_eq( join(d), "1,0" );

    d.resize(5, 9);
    test_str_eq( join(d), "1,0,9,9,9" );

    d.resize(1, 9);
    test_str_eq( join(d), "1" );

    d.resize(0);
    test_true( d.empty() );
    test_pass("cxx03:deque::resize");

    STD::deque<int> big;
    for (int i = 0; i < 300; ++i)
        big.push_back(i);
    for (int i = 0; i < 250; ++i)
        big.pop_front();
#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::deque<int> before = big;
    big.shrink_to_fit();
    test_true( big == before );
    test_eq( big.size(), STD::size_t(50) );
    test_eq( big.front(), 250 );
    test_eq( big.back(), 299 );
    test_pass("cxx11:deque::shrink_to_fit");
#else
    test_skip("cxx11:deque::shrink_to_fit");
#endif

    int base = Item::live;
    {
        STD::deque<Item> di;
        di.resize(5);
        test_eq( di.size(), STD::size_t(5) );
        test_eq( di[4].id, 0 );
        di.resize(8, Item(3, "z"));
        test_eq( di[7].id, 3 );
        test_str_eq( di[7].tag, "z" );
        di.resize(2);
        test_eq( di.size(), STD::size_t(2) );
    }
    test_eq( Item::live, base );
    test_pass("cxx03:deque::resize#Item");

    STD::deque<STD::string> ds;
    ds.resize(3, STD::string("q"));
    test_str_eq( ds[2], "q" );
    test_eq( ds.size(), STD::size_t(3) );
    test_pass("cxx03:deque::resize#string");
}

#if TEST_HAS_EH
TEST_CASE(deque, element_access) {
    const int d_init[] = { 10, 20, 30 };
    STD::deque<int> d(d_init, d_init + 3);
    const STD::deque<int>& cd = d;

    test_eq( d[0], 10 );
    test_eq( cd[2], 30 );
    d[1] = 21;
    test_eq( d[1], 21 );
    test_pass("cxx03:deque::operator[]");

    test_eq( d.at(0), 10 );
    test_eq( cd.at(1), 21 );
    d.at(2) = 31;
    test_eq( d.at(2), 31 );

    test_throw( (void)d.at(3) );
    test_throw( (void)cd.at(99) );
    test_no_throw( (void)d.at(2) );
    bool caught = false;
    try { (void)d.at(3); } catch (const STD::out_of_range&) { caught = true; }
    test_true( caught );
    test_pass("cxx03:deque::at");

    test_eq( d.front(), 10 );
    test_eq( cd.front(), 10 );
    test_pass("cxx03:deque::front");
    test_eq( d.back(), 31 );
    test_eq( cd.back(), 31 );
    d.front() = 1;
    d.back() = 3;
    test_str_eq( join(d), "1,21,3" );
    test_pass("cxx03:deque::back");

#if _tst_cplusplus >= 201103L
    test_true( (STD::is_same<decltype(d.front()), int&>::value) );
    test_true( (STD::is_same<decltype(cd.front()), const int&>::value) );
    test_true( (STD::is_same<decltype(d.at(0)), int&>::value) );
    test_true( (STD::is_same<decltype(cd[0]), const int&>::value) );
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif

    const STD::string ds_init[] = { "aa", "bb", "cc" };
    STD::deque<STD::string> ds(ds_init, ds_init + 3);
    const STD::deque<STD::string>& cds = ds;
    test_str_eq( ds[1], "bb" );
    test_str_eq( cds.at(2), "cc" );
    test_str_eq( ds.front(), "aa" );
    test_str_eq( cds.back(), "cc" );
    ds.at(0) += "!";
    test_str_eq( ds.front(), "aa!" );
    test_pass("cxx03:deque#element_access_string");

    const Item di_init[] = { Item(1, "a"), Item(2, "b") };
    STD::deque<Item> di(di_init, di_init + 2);
    const STD::deque<Item>& cdi = di;
    test_eq( di[0].id, 1 );
    test_eq( cdi.at(1).id, 2 );
    test_str_eq( di.front().tag, "a" );
    test_str_eq( cdi.back().tag, "b" );
    test_throw( (void)cdi.at(5) );
    test_pass("cxx03:deque#element_access_Item");
}
#else   // !TEST_HAS_EH
TEST_CASE(deque, element_access) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:deque::operator[]");
    test_skip("cxx03:deque::at");
    test_skip("cxx03:deque::front");
    test_skip("cxx03:deque::back");
    test_skip("cxx03:deque#element_access_string");
    test_skip("cxx03:deque#element_access_Item");
}
#endif  // TEST_HAS_EH

TEST_CASE(deque, modifiers_push_pop_emplace) {
    STD::deque<int> d;

    d.push_back(2);
    const int cv = 3;
    d.push_back(cv);
    test_str_eq( join(d), "2,3" );
    test_pass("cxx03:deque::push_back");
    d.push_front(1);
    const int cv0 = 0;
    d.push_front(cv0);
    test_str_eq( join(d), "0,1,2,3" );
    test_pass("cxx03:deque::push_front");

#if _TST_HAS_CXX11_LIB_MEMBERS
#if _TST_HAS_CXX17_LIB_MEMBERS
    int& r1 = d.emplace_back(4);
    test_eq( r1, 4 );
#else
    d.emplace_back(4);
    test_eq( d.back(), 4 );
#endif
    test_pass("cxx11:deque::emplace_back");
#if _TST_HAS_CXX17_LIB_MEMBERS
    int& r2 = d.emplace_front(-1);
    test_eq( r2, -1 );
#else
    d.emplace_front(-1);
    test_eq( d.front(), -1 );
#endif
    test_str_eq( join(d), "-1,0,1,2,3,4" );
    test_pass("cxx11:deque::emplace_front");

    STD::deque<int>::iterator it = d.emplace(d.begin() + 1, 99);
    test_eq( *it, 99 );
    test_str_eq( join(d), "-1,99,0,1,2,3,4" );
    test_pass("cxx11:deque::emplace");

    d.pop_front();
    test_str_eq( join(d), "99,0,1,2,3,4" );
    test_pass("cxx03:deque::pop_front");
    d.pop_back();
    test_str_eq( join(d), "99,0,1,2,3" );
    test_pass("cxx03:deque::pop_back");
#else
    test_skip("cxx11:deque::emplace_back");
    test_skip("cxx11:deque::emplace_front");
    test_skip("cxx11:deque::emplace");
    d.push_front(-1);
    d.pop_front();
    test_str_eq( join(d), "0,1,2,3" );
    test_pass("cxx03:deque::pop_front");
    d.pop_back();
    test_str_eq( join(d), "0,1,2" );
    test_pass("cxx03:deque::pop_back");
#endif

    STD::deque<STD::string> ds;
    STD::string s1 = "moved-back";
    ds.push_back(STD::move(s1));
    STD::string s2 = "moved-front";
    ds.push_front(STD::move(s2));
    test_str_eq( ds.front(), "moved-front" );
    test_str_eq( ds.back(), "moved-back" );
    test_pass("cxx11:deque#push_move_string");

#if _TST_HAS_CXX11_LIB_MEMBERS
#if _TST_HAS_CXX17_LIB_MEMBERS
    STD::string& sr = ds.emplace_back(3u, 'z');
    test_str_eq( sr, "zzz" );
#else
    ds.emplace_back(3u, 'z');
    test_str_eq( ds.back(), "zzz" );
#endif
    test_pass("cxx11:deque::emplace_back#string");
#if _TST_HAS_CXX17_LIB_MEMBERS
    STD::string& sf = ds.emplace_front(2u, 'y');
    test_str_eq( sf, "yy" );
#else
    ds.emplace_front(2u, 'y');
    test_str_eq( ds.front(), "yy" );
#endif
    test_eq( ds.size(), STD::size_t(4) );
    ds.pop_back();
    ds.pop_front();
    test_eq( ds.size(), STD::size_t(2) );
    test_pass("cxx11:deque::emplace_front#string");
#else
    test_skip("cxx11:deque::emplace_back#string");
    test_skip("cxx11:deque::emplace_front#string");
#endif

    int base = Item::live;
    {
        STD::deque<Item> di;
#if _TST_HAS_CXX11_LIB_MEMBERS
#if _TST_HAS_CXX17_LIB_MEMBERS
        Item& a = di.emplace_back(1, "back");
        test_eq( a.id, 1 );
        test_str_eq( a.tag, "back" );
        Item& b = di.emplace_front(2, "front");
        test_eq( b.id, 2 );
#else
        di.emplace_back(1, "back");
        test_eq( di.back().id, 1 );
        test_str_eq( di.back().tag, "back" );
        di.emplace_front(2, "front");
        test_eq( di.front().id, 2 );
#endif
        STD::deque<Item>::iterator mid = di.emplace(di.begin() + 1, 3, "mid");
        test_eq( mid->id, 3 );
        test_eq( di.size(), STD::size_t(3) );
        test_str_eq( di[1].tag, "mid" );
#else
        di.push_back(Item(2, "front"));
        di.push_back(Item(3, "mid"));
        di.push_back(Item(1, "back"));
        test_eq( di.size(), STD::size_t(3) );
        test_str_eq( di[1].tag, "mid" );
#endif

        Item tmp(4, "cp");
        di.push_back(tmp);
        di.push_front(Item(5, "mv"));
        test_eq( di.front().id, 5 );
        test_eq( di.back().id, 4 );

        di.pop_front();
        di.pop_back();
        test_eq( di.size(), STD::size_t(3) );
    }
    test_eq( Item::live, base );
    test_pass("cxx03:deque#emplace_Item");
}

#if !defined(__GNUC__)
TEST_CASE(deque, modifiers_insert_erase_clear_swap) {
    const int d_init[] = { 1, 5 };
    STD::deque<int> d(d_init, d_init + 2);

    STD::deque<int>::iterator it;

    const int v2 = 2;
    it = d.insert(d.begin() + 1, v2);
    test_eq( *it, 2 );
    test_str_eq( join(d), "1,2,5" );
    test_pass("cxx03:deque::insert(const T&)");

    it = d.insert(d.begin() + 2, int(3));
    test_eq( *it, 3 );
    test_str_eq( join(d), "1,2,3,5" );
    test_pass("cxx11:deque::insert(T&&)");

#if _TST_HAS_CXX11_LIB_MEMBERS
    it = d.insert(d.end(), STD::size_t(2), 9);
    test_eq( *it, 9 );
#else
    d.insert(d.end(), STD::size_t(2), 9);
#endif
    test_str_eq( join(d), "1,2,3,5,9,9" );
    test_pass("cxx03:deque::insert(n,value)");

    int raw[] = { 6, 7 };
#if _TST_HAS_CXX11_LIB_MEMBERS
    it = d.insert(d.begin() + 4, raw, raw + 2);
    test_eq( *it, 6 );
#else
    d.insert(d.begin() + 4, raw, raw + 2);
#endif
    test_str_eq( join(d), "1,2,3,5,6,7,9,9" );
    test_pass("cxx03:deque::insert(first,last)");

#if _tst_cplusplus >= 201103L
    it = d.insert(d.begin() + 3, { 4 });
#else
    { static const int one[] = { 4 };
#if _TST_HAS_CXX11_LIB_MEMBERS
      it = d.insert(d.begin() + 3, one, one + 1);
#else
      d.insert(d.begin() + 3, one, one + 1); it = d.begin() + 3;
#endif
    }
#endif
    test_eq( *it, 4 );
    test_str_eq( join(d), "1,2,3,4,5,6,7,9,9" );
#if _tst_cplusplus >= 201103L
    test_pass("cxx11:deque::insert(initializer_list)");
#endif

#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
    const int src_init[] = { 8 };
    STD::vector<int> src(src_init, src_init + 1);
    it = d.insert_range(d.begin() + 7, src);
    test_eq( *it, 8 );
    test_str_eq( join(d), "1,2,3,4,5,6,7,8,9,9" );

    STD::vector<int> none;
    it = d.insert_range(d.begin() + 2, none);
    test_eq( *it, 3 );
    test_eq( d.size(), STD::size_t(10) );
    test_pass("cxx23:deque::insert_range");
#else
    { static const int one8[] = { 8 };
      d.insert(d.begin() + 7, one8, one8 + 1); }
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif

    it = d.erase(d.end() - 1);
    test_true( it == d.end() );
    test_str_eq( join(d), "1,2,3,4,5,6,7,8,9" );
    test_pass("cxx03:deque::erase");

    it = d.erase(d.begin() + 1, d.begin() + 4);
    test_eq( *it, 5 );
    test_str_eq( join(d), "1,5,6,7,8,9" );

    it = d.erase(d.begin() + 2, d.begin() + 2);
    test_eq( *it, 6 );
    test_eq( d.size(), STD::size_t(6) );
    test_pass("cxx03:deque::erase(first,last)");

#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
    const int head_init[] = { -2, -1 };
    STD::vector<int> head(head_init, head_init + 2);
    d.prepend_range(head);
    test_str_eq( join(d), "-2,-1,1,5,6,7,8,9" );
    test_pass("cxx23:deque::prepend_range");
    const int tail_init[] = { 10, 11 };
    STD::vector<int> tail(tail_init, tail_init + 2);
    d.append_range(tail);
    test_str_eq( join(d), "-2,-1,1,5,6,7,8,9,10,11" );
    test_pass("cxx23:deque::append_range");
#else
    TEST_SKIP1(); TEST_SKIP1();
#endif

    const int a_init[] = { 1, 2 };
    STD::deque<int> a(a_init, a_init + 2);
    const int b_init[] = { 3, 4, 5 };
    STD::deque<int> b(b_init, b_init + 3);
    STD::deque<int> a0 = a, b0 = b;
    a.swap(b);
    test_true( a == b0 );
    test_true( b == a0 );
    test_pass("cxx03:deque::swap");
    STD::swap(a, b);
    test_true( a == a0 );
    test_true( b == b0 );
    test_pass("cxx03:swap(deque)");

    a.clear();
    test_true( a.empty() );
    test_eq( a.size(), STD::size_t(0) );
    test_true( a.begin() == a.end() );
    a.push_back(1);
    test_eq( a.front(), 1 );
    test_pass("cxx03:deque::clear");

    const STD::string ds_init[] = { "a", "d" };
    STD::deque<STD::string> ds(ds_init, ds_init + 2);
    STD::string mv = "b";
    ds.insert(ds.begin() + 1, STD::move(mv));
    const STD::string cs = "c";
    ds.insert(ds.begin() + 2, cs);
    test_eq( ds.size(), STD::size_t(4) );
    test_str_eq( ds[1], "b" );
    test_str_eq( ds[2], "c" );
#if _tst_cplusplus >= 201103L
    ds.insert(ds.end(), { "e", "f" });
#else

    { static const char* const ef[] = { "e", "f" }; ds.insert(ds.end(), ef, ef + 2); }
#endif
    test_str_eq( ds.back(), "f" );
    ds.erase(ds.begin(), ds.begin() + 2);
    test_str_eq( ds.front(), "c" );
    ds.clear();
    test_true( ds.empty() );
    test_pass("cxx03:deque#insert_erase_string");

    int base = Item::live;
    {
        const Item di_init[] = { Item(1), Item(4) };
        STD::deque<Item> di(di_init, di_init + 2);
        di.insert(di.begin() + 1, Item(2));
        Item cp(3);
        di.insert(di.begin() + 2, cp);
        test_eq( di.size(), STD::size_t(4) );
        test_eq( di[2].id, 3 );

        di.insert(di.end(), STD::size_t(2), Item(9));
        test_eq( di.size(), STD::size_t(6) );
        test_eq( di.back().id, 9 );

#if _TST_HAS_CXX20_LIB_MEMBERS && !defined(__GNUC__)
        const Item vi_init[] = { Item(7), Item(8) };
        STD::vector<Item> vi(vi_init, vi_init + 2);
        di.insert_range(di.begin(), vi);
        test_eq( di.front().id, 7 );
        di.prepend_range(vi);
        di.append_range(vi);
        test_eq( di.back().id, 8 );
#else
        TEST_SKIP1(); TEST_SKIP1();
#endif

        di.erase(di.begin());
        di.erase(di.begin(), di.begin() + 3);
        test_gt( di.size(), STD::size_t(0) );

        STD::deque<Item> other;
        di.swap(other);
        test_true( di.empty() );
        test_gt( other.size(), STD::size_t(0) );
        other.clear();
        test_true( other.empty() );
    }
    test_eq( Item::live, base );
    test_pass("cxx03:deque#insert_erase_Item");
}
#else
TEST_CASE_SKIP(deque, modifiers_insert_erase_clear_swap)
#endif

TEST_CASE(deque, comparison_operators) {
    const int a_init[] = { 1, 2, 3 };
    STD::deque<int> a(a_init, a_init + 3);
    const int b_init[] = { 1, 2, 3 };
    STD::deque<int> b(b_init, b_init + 3);
    const int c_init[] = { 1, 2, 4 };
    STD::deque<int> c(c_init, c_init + 3);
    const int shorter_init[] = { 1, 2 };
    STD::deque<int> shorter(shorter_init, shorter_init + 2);
    STD::deque<int> empty1, empty2;

    test_true( a == b );
    test_true( !(a == c) );
    test_true( empty1 == empty2 );
    test_pass("cxx03:deque::operator==");
    test_true( a != c );
    test_true( !(a != b) );
    test_true( empty1 != a );
    test_pass("cxx03:deque::operator!=");

    test_true( a < c );
    test_true( !(c < a) );
    test_true( shorter < a );
    test_pass("cxx03:deque::operator<");
    test_true( c > a );
    test_pass("cxx03:deque::operator>");
    test_true( a <= b );
    test_true( a <= c );
    test_pass("cxx03:deque::operator<=");
    test_true( a >= b );
    test_true( c >= a );
    test_pass("cxx03:deque::operator>=");

#if _tst_cplusplus >= 202002L
#if _tst_cplusplus >= 202002L
    test_true( (a <=> b) == 0 );
    test_true( (a <=> c) < 0 );
    test_true( (c <=> a) > 0 );
    test_true( (shorter <=> a) < 0 );
    test_true( same_type<STD::strong_ordering>(a <=> b) );
#else

    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
#if _tst_cplusplus >= 202002L
    test_pass("cxx20:deque::operator<=>");
#endif

    const STD::string sa_init[] = { "a", "b" };
    STD::deque<STD::string> sa(sa_init, sa_init + 2);
    const STD::string sb_init[] = { "a", "b" };
    STD::deque<STD::string> sb(sb_init, sb_init + 2);
    const STD::string sc_init[] = { "a", "c" };
    STD::deque<STD::string> sc(sc_init, sc_init + 2);
    test_true( sa == sb );
    test_true( sa < sc );
    test_true( sc > sa );
    test_true( sa <= sb );
    test_true( sa >= sb );
    test_true( sa != sc );
    test_pass("cxx03:deque#compare_string");
#if _tst_cplusplus >= 202002L
    test_true( (sa <=> sc) < 0 );
#else

    TEST_SKIP1();
#endif

    const Item ia_init[] = { Item(1, "a"), Item(2, "b") };
    STD::deque<Item> ia(ia_init, ia_init + 2);
    const Item ib_init[] = { Item(1, "a"), Item(2, "b") };
    STD::deque<Item> ib(ib_init, ib_init + 2);
    const Item ic_init[] = { Item(1, "a"), Item(3, "b") };
    STD::deque<Item> ic(ic_init, ic_init + 2);
    test_true( ia == ib );
    test_true( ia != ic );
    test_true( ia < ic );
    test_true( ic >= ia );
    test_pass("cxx03:deque#compare_Item");
#if _tst_cplusplus >= 202002L
    test_true( (ia <=> ic) < 0 );
#else

    TEST_SKIP1();
#endif
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(deque, erase_erase_if) {

    const int d_init[] = { 1, 2, 3, 2, 4, 2 };
    STD::deque<int> d(d_init, d_init + 6);
    STD::deque<int>::size_type n = STD::erase(d, 2);
    test_eq( n, STD::size_t(3) );
    test_str_eq( join(d), "1,3,4" );

    n = STD::erase(d, 99);
    test_eq( n, STD::size_t(0) );
    test_eq( d.size(), STD::size_t(3) );
    test_pass("cxx20:erase(deque)");

    const int e_init[] = { 1, 2, 3, 4, 5, 6 };
    STD::deque<int> e(e_init, e_init + 6);
    STD::deque<int>::size_type m = STD::erase_if(e, is_even_i);
    test_eq( m, STD::size_t(3) );
    test_str_eq( join(e), "1,3,5" );

    m = STD::erase_if(e, always_true_i);
    test_eq( m, STD::size_t(3) );
    test_true( e.empty() );
    test_pass("cxx20:erase_if(deque)");

    const STD::string ds_init[] = { "aa", "b", "cc", "d" };
    STD::deque<STD::string> ds(ds_init, ds_init + 4);
    STD::deque<STD::string>::size_type ns = STD::erase(ds, STD::string("b"));
    test_eq( ns, STD::size_t(1) );
    test_eq( ds.size(), STD::size_t(3) );
    ns = STD::erase_if(ds, len_is_2);
    test_eq( ns, STD::size_t(2) );
    test_eq( ds.size(), STD::size_t(1) );
    test_str_eq( ds.front(), "d" );
    test_pass("cxx20:erase(deque)#string");

    int base = Item::live;
    {
        const Item di_init[] = { Item(1), Item(2), Item(3), Item(2) };
        STD::deque<Item> di(di_init, di_init + 4);
        STD::deque<Item>::size_type ni = STD::erase(di, Item(2));
        test_eq( ni, STD::size_t(2) );
        test_eq( di.size(), STD::size_t(2) );
        ni = STD::erase_if(di, id_gt_2);
        test_eq( ni, STD::size_t(1) );
        test_eq( di.size(), STD::size_t(1) );
        test_eq( di.front().id, 1 );
    }
    test_eq( Item::live, base );
    test_pass("cxx20:erase(deque)#Item");
}
#else
TEST_CASE(deque, erase_erase_if) {
    TEST_NOTE("std::erase / erase_if are C++20 free functions");
    test_skip("cxx20:erase(deque)");
    test_skip("cxx20:erase_if(deque)");
    test_skip("cxx20:erase(deque)#string");
    test_skip("cxx20:erase(deque)#Item");
}
#endif

TEST_CASE(deque, block_boundary_stress) {

    const int N = 400;

    STD::deque<int> d;
    for (int i = 0; i < N; ++i)
        d.push_front(i);
    test_eq( d.size(), STD::size_t(N) );
    test_eq( d.front(), N - 1 );
    test_eq( d.back(), 0 );
    test_eq( d[0], N - 1 );
    test_eq( d[N - 1], 0 );
    test_eq( d.at(1), N - 2 );

    bool ok = true;
    for (int i = 0; i < N; ++i)
        if (d[i] != N - 1 - i) { ok = false; break; }
    test_true( ok );
    test_eq( (int)(d.end() - d.begin()), N );
    test_eq( STD::accumulate(d.begin(), d.end(), 0), (N - 1) * N / 2 );

    ok = true;
    for (int i = 0; i < N; ++i) {
        int f = d.front();
        if (f != N - 1 - i) { ok = false; break; }
        d.pop_front();
    }
    test_true( ok );
    test_true( d.empty() );
    test_pass("cxx03:deque#block_boundary_push_front");

    for (int i = 0; i < N; ++i) {
        d.push_back(i);
        d.push_front(-i);
    }
    test_eq( d.size(), STD::size_t(2 * N) );
    test_eq( d.front(), -(N - 1) );
    test_eq( d.back(), N - 1 );
    for (int i = 0; i < N; ++i) {
        d.pop_front();
        d.pop_back();
    }
    test_true( d.empty() );
    test_pass("cxx03:deque#block_boundary_interleaved");

    int base = Item::live;
    {
        STD::deque<Item> di;
        for (int i = 0; i < N; ++i) {
            di.push_front(Item(i));
#if _TST_HAS_CXX11_LIB_MEMBERS
            di.emplace_back(i, "b");
#else
            di.push_back(Item(i, "b"));
#endif
        }
        test_eq( di.size(), STD::size_t(2 * N) );
        test_eq( di.front().id, N - 1 );
        test_eq( di.back().id, N - 1 );
        test_eq( Item::live, base + 2 * N );

        di.insert(di.begin() + N, Item(-1, "mid"));
        test_eq( di[N].id, -1 );
        di.erase(di.begin() + N);
        test_eq( di.size(), STD::size_t(2 * N) );

        for (int i = 0; i < N; ++i) {
            di.pop_back();
            di.pop_front();
        }
        test_true( di.empty() );
        test_eq( Item::live, base );
    }
    test_eq( Item::live, base );
    test_pass("cxx03:deque#block_boundary_Item");

    STD::deque<STD::string> ds;
    for (int i = 0; i < N; ++i)
        ds.push_front(STD::to_string(i));
    test_eq( ds.size(), STD::size_t(N) );
    test_str_eq( ds.front(), STD::to_string(N - 1) );
    test_str_eq( ds.back(), "0" );
    ds.resize(10);
    test_eq( ds.size(), STD::size_t(10) );
#if _TST_HAS_CXX11_LIB_MEMBERS
    ds.shrink_to_fit();
#endif
    test_str_eq( ds.front(), STD::to_string(N - 1) );
    test_pass("cxx03:deque#block_boundary_string");
}
#else
TEST_CASE_SKIP(deque, member_types)
TEST_CASE_SKIP(deque, constructors_int)
TEST_CASE_SKIP(deque, constructors_string_and_item)
TEST_CASE_SKIP(deque, deduction_guides)
TEST_CASE_SKIP(deque, pmr_alias)
TEST_CASE_SKIP(deque, assignment_and_allocator)
TEST_CASE_SKIP(deque, iterators)
TEST_CASE_SKIP(deque, capacity)
TEST_CASE_SKIP(deque, element_access)
TEST_CASE_SKIP(deque, modifiers_push_pop_emplace)
TEST_CASE_SKIP(deque, modifiers_insert_erase_clear_swap)
TEST_CASE_SKIP(deque, comparison_operators)
TEST_CASE_SKIP(deque, erase_erase_if)
TEST_CASE_SKIP(deque, block_boundary_stress)
#endif

#if _tst_cplusplus >= 201103L
TEST_CASE(deque, init_list) {
    STD::deque<int> d = { 1, 2, 3, 4 };
    test_eq( d.size(), STD::size_t(4) );
    test_eq( d.front(), 1 );
    test_eq( d.back(), 4 );
    test_eq( d[2], 3 );
    test_pass("cxx11:deque::deque(initializer_list)");
}
#else
TEST_CASE_SKIP(deque, init_list)
#endif


TEST_CASE(deque, member_types_cxx03) {
    typedef STD::deque<int> D;
    D d; d.push_back(1);

    test_true(( STD::is_same<D::value_type, int>::value ));
    test_pass("cxx03:deque::value_type");

    D::reference r = d.front();
    r = 5;
    test_eq( d.front(), 5 );
    test_true(( STD::is_same<D::reference, int&>::value ));
    test_pass("cxx03:deque::reference");

    const D& cd = d;
    D::const_reference cr = cd.front();
    test_eq( cr, 5 );
    test_true(( STD::is_same<D::const_reference, const int&>::value ));
    test_pass("cxx03:deque::const_reference");

    D::pointer p = &d.front();
    test_eq( *p, 5 );
    test_pass("cxx03:deque::pointer");

    D::size_type n = d.size();
    test_eq( n, (STD::size_t)1 );
    test_true( !STD::is_signed<D::size_type>::value );
    test_pass("cxx03:deque::size_type");

    D::difference_type diff = d.end() - d.begin();
    test_eq( (long)diff, 1L );
    test_true( STD::is_signed<D::difference_type>::value );
    test_pass("cxx03:deque::difference_type");

    D::const_iterator ci = cd.begin();
    test_eq( *ci, 5 );
    test_true( same_type<D::const_iterator>(cd.begin()) );
    test_pass("cxx03:deque::const_iterator");

    D::const_reverse_iterator cri = cd.rbegin();
    test_eq( *cri, 5 );
    test_true( same_type<D::const_reverse_iterator>(cd.rbegin()) );
    test_pass("cxx03:deque::const_reverse_iterator");
}

TEST_CASE(deque, member_types_cxx11) {
    typedef STD::deque<int> D;
    D d; d.push_back(7);
    const D& cd = d;

    test_true(( STD::is_same<D::allocator_type, STD::allocator<int> >::value ));
    D::allocator_type al = d.get_allocator();
    (void)al;
    test_pass("cxx03:deque::allocator_type");

    D::const_pointer cp = &cd.front();
    test_eq( *cp, 7 );
    test_pass("cxx03:deque::const_pointer");
}


TEST_CASE(deque, allocator_and_const_pointer_cxx03) {
    typedef STD::deque<int> D;
    D d; d.push_back(1);
    const D& cd = d;

    test_true(( STD::is_same<D::allocator_type, STD::allocator<int> >::value ));
    D::allocator_type al = d.get_allocator();
    (void)al;
    test_pass("cxx03:deque::allocator_type");

    D::const_pointer cp = &cd.front();
    test_eq( *cp, 1 );
    test_true(( STD::is_same<D::const_pointer, const int*>::value ));
    test_pass("cxx03:deque::const_pointer");
}

TEST_CASE(deque, emplace_return_types) {
    STD::deque<int> d;
#if defined(__cpp_lib_emplace_return_type) || _TST_HAS_CXX17_LIB_MEMBERS
    {
        int& b = d.emplace_back(7);
        test_eq( b, 7 );
        b = 8;
        test_eq( d.back(), 8 );
        test_pass("cxx17:deque::emplace_back returns reference");

        int& f = d.emplace_front(1);
        test_eq( f, 1 );
        f = 2;
        test_eq( d.front(), 2 );
        test_pass("cxx17:deque::emplace_front returns reference");
    }
    TEST_NOTE("this library returns a reference (C++17), so the void form cannot apply");
    test_skip("cxx11:deque::emplace_back returns void");
    test_skip("cxx11:deque::emplace_front returns void");
#elif _TST_HAS_CXX11_LIB_MEMBERS
    d.emplace_back(7);
    test_eq( d.back(), 7 );
    test_pass("cxx11:deque::emplace_back returns void");
    d.emplace_front(1);
    test_eq( d.front(), 1 );
    test_pass("cxx11:deque::emplace_front returns void");
    test_skip("cxx17:deque::emplace_back returns reference");
    test_skip("cxx17:deque::emplace_front returns reference");
#else
    test_skip("cxx11:deque::emplace_back returns void");
    test_skip("cxx11:deque::emplace_front returns void");
    test_skip("cxx17:deque::emplace_back returns reference");
    test_skip("cxx17:deque::emplace_front returns reference");
#endif
}

TEST_CASE(deque, feature_macros) {
#if defined(__cpp_lib_containers_ranges)
    test_true( __cpp_lib_containers_ranges >= 202202L );
    {
        int raw[3] = {1, 2, 3};
        STD::deque<int> d(STD::from_range, STD::ranges::subrange<int*>(raw, raw + 3));
        test_eq( (int)d.size(), 3 );
        test_eq( d.front(), 1 );
    }
    test_pass("cxx23:__cpp_lib_containers_ranges");
#else
    TEST_NOTE("__cpp_lib_containers_ranges is not defined by this library");
    test_skip("cxx23:__cpp_lib_containers_ranges");
#endif

#if defined(__cpp_lib_constexpr_deque)
    test_true( __cpp_lib_constexpr_deque >= 202502L );
    test_pass("cxx26:__cpp_lib_constexpr_deque");
    {
        STD::deque<int> d;
        d.push_back(1);
        d.push_front(0);
        test_eq( d.front(), 0 );
        test_eq( d.back(), 1 );
    }
    test_pass("cxx26:constexpr deque operations");
#else
    TEST_NOTE("constexpr deque is C++26 (P3372); not in this library");
    test_skip("cxx26:__cpp_lib_constexpr_deque");
    test_skip("cxx26:constexpr deque operations");
#endif
}
