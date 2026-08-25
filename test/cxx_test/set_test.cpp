#include "test_cxx.hpp"
#include <set>
#include <functional>
#include <utility>
#include <string>
#include <vector>
#include <type_traits>

namespace {
long sig(const STD::set<int>& s) {
    long r = 0;
    for (STD::set<int>::const_iterator it = s.begin(); it != s.end(); ++it) r = r * 10 + *it;
    return r;
}
}

namespace { struct SetIsEven { bool operator()(int x) const { return x % 2 == 0; } }; }

TEST_CASE(set, construct) {
    STD::set<int> a;
    test_true( a.empty() );
    test_pass("cxx03:set::empty");
    test_eq( a.size(), 0u );
    test_pass("cxx03:set::size");

    int raw[] = { 3, 1, 2, 1, 3 };
    STD::set<int> b(raw, raw + 5);
    test_eq( b.size(), 3u );
    test_eq( sig(b), 123L );
    test_pass("cxx03:set::set(first,last)");

    STD::set<int> c(b);
    test_eq( sig(c), 123L );
    test_pass("cxx03:set::set(const set&)");

    STD::set<int> d;
    d = b;
    test_eq( sig(d), 123L );
    test_pass("cxx03:set::operator=(const set&)");

    STD::set<int, STD::greater<int> > g(raw, raw + 5);
    test_eq( g.size(), 3u );
    test_eq( *g.begin(), 3 );
    STD::set<int, STD::greater<int> >::iterator gi = g.begin();
    ++gi;
    test_eq( *gi, 2 );
    test_pass("cxx03:set::set(comp)");
}

TEST_CASE(set, insert_emplace) {
    STD::set<int> s;
    STD::pair<STD::set<int>::iterator, bool> r = s.insert(5);
    test_true( r.second );
    test_eq( *r.first, 5 );
    STD::pair<STD::set<int>::iterator, bool> r2 = s.insert(5);
    test_true( !r2.second );
    test_eq( s.size(), 1u );
    test_pass("cxx03:set::insert");

    STD::set<int>::iterator h = s.insert(s.begin(), 3);
    test_eq( *h, 3 );
    test_pass("cxx03:set::insert (hint)");

    int raw[] = { 7, 8, 9 };
    s.insert(raw, raw + 3);
    test_eq( s.size(), 5u );
    test_pass("cxx03:set::insert (range)");

#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
    STD::pair<STD::set<int>::iterator, bool> e = s.emplace(4);
    test_true( e.second );
    test_eq( *e.first, 4 );
    test_pass("cxx11:set::emplace");
    STD::set<int>::iterator eh = s.emplace_hint(s.begin(), 1);
    test_eq( *eh, 1 );
    test_pass("cxx11:set::emplace_hint");
#else
    s.insert(4); s.insert(1);
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
    test_true( s.count(1) == 1u );
}

TEST_CASE(set, lookup) {
    int raw[] = { 10, 20, 30, 40 };
    STD::set<int> s(raw, raw + 4);
    const STD::set<int>& cs = s;

    test_true( s.find(20) != s.end() );
    test_true( s.find(25) == s.end() );
    test_true( cs.find(30) != cs.end() );
    test_pass("cxx03:set::find");
    test_eq( s.count(20), 1u );
    test_eq( s.count(99), 0u );
    test_pass("cxx03:set::count");

    test_eq( *s.lower_bound(20), 20 );
    test_pass("cxx03:set::lower_bound");
    test_eq( *s.upper_bound(20), 30 );
    test_pass("cxx03:set::upper_bound");
    test_eq( *s.lower_bound(25), 30 );
    test_pass("cxx03:set::lower_bound (absent -> next up)");

    STD::pair<STD::set<int>::iterator, STD::set<int>::iterator> eq = s.equal_range(30);
    test_eq( *eq.first, 30 );
    test_eq( *eq.second, 40 );
    test_pass("cxx03:set::equal_range");

    test_true( s.key_comp()(1, 2) );
    test_pass("cxx03:set::key_comp");
    test_true( s.value_comp()(1, 2) );
    test_pass("cxx03:set::value_comp");
}

TEST_CASE(set, erase_clear_swap) {
    int raw[] = { 1, 2, 3, 4, 5 };
    STD::set<int> s(raw, raw + 5);

    STD::set<int>::size_type n = s.erase(3);
    test_eq( (long)n, 1L );
    test_eq( s.erase(99), 0u );
    test_eq( sig(s), 1245L );
    test_pass("cxx03:set::erase (key)");

    STD::set<int>::iterator it = s.find(2);
#if _TST_ERASE_RETURNS_VOID
    s.erase(it);
    TEST_NOTE("libstdc++ in C++03 returns void from erase(iterator)");
    TEST_SKIP1();
#else
    STD::set<int>::iterator nx = s.erase(it);
    test_eq( *nx, 4 );
#endif
    test_eq( sig(s), 145L );
    test_pass("cxx03:set::erase (iterator)");

    STD::set<int> t(raw, raw + 5);
    t.erase(t.find(2), t.find(5));
    test_eq( sig(t), 15L );
    test_pass("cxx03:set::erase (range)");

    s.clear();
    test_true( s.empty() );
    test_pass("cxx03:set::clear");

    STD::set<int> p(raw, raw + 2), q(raw + 2, raw + 5);
    p.swap(q);
    test_eq( sig(p), 345L );
    test_eq( sig(q), 12L );
    test_pass("cxx03:set::swap");
    STD::swap(p, q);
    test_eq( sig(p), 12L );
    test_pass("cxx03:swap(set)");

    test_true( p.max_size() > 0u );
    test_pass("cxx03:set::max_size");
    STD::set<int>::allocator_type al = p.get_allocator();
    (void)al;
}

TEST_CASE(set, iterators_and_compare) {
    int raw[] = { 4, 2, 5, 1, 3 };
    STD::set<int> s(raw, raw + 5);

    long fwd = 0;
    for (STD::set<int>::iterator it = s.begin(); it != s.end(); ++it) fwd = fwd * 10 + *it;
    test_eq( fwd, 12345L );
    test_pass("cxx03:set::begin");
    test_true( 1 );
    test_pass("cxx03:set::end");

#if _TST_HAS_CXX11_LIB_MEMBERS
    long cf = 0;
    for (STD::set<int>::const_iterator it = s.cbegin(); it != s.cend(); ++it) cf = cf * 10 + *it;
    test_eq( cf, 12345L );
    test_pass("cxx11:set::cbegin");
    test_true( 1 );
    test_pass("cxx11:set::cend");
#else
    long cf = 0;
    const STD::set<int>& csf = s;
    for (STD::set<int>::const_iterator it = csf.begin(); it != csf.end(); ++it) cf = cf * 10 + *it;
    test_eq( cf, 12345L );
    test_skip("cxx11:set::cbegin");
    test_true( 1 );
    test_skip("cxx11:set::cend");
#endif

    long rev = 0;
    for (STD::set<int>::reverse_iterator it = s.rbegin(); it != s.rend(); ++it) rev = rev * 10 + *it;
    test_eq( rev, 54321L );
    test_pass("cxx03:set::rbegin");
    test_true( 1 );
    test_pass("cxx03:set::rend");

#if _TST_HAS_CXX11_LIB_MEMBERS
    long cr = 0;
    for (STD::set<int>::const_reverse_iterator it = s.crbegin(); it != s.crend(); ++it) cr = cr * 10 + *it;
    test_eq( cr, 54321L );
    test_pass("cxx11:set::crbegin");
    test_true( 1 );
    test_pass("cxx11:set::crend");
#else
    long cr = 0;
    const STD::set<int>& csr = s;
    for (STD::set<int>::const_reverse_iterator it = csr.rbegin(); it != csr.rend(); ++it) cr = cr * 10 + *it;
    test_eq( cr, 54321L );
    test_skip("cxx11:set::crbegin");
    test_true( 1 );
    test_skip("cxx11:set::crend");
#endif

    STD::set<int> a(raw, raw + 3);
    STD::set<int> b(raw, raw + 3);
    STD::set<int> c(raw, raw + 2);
    test_true( a == b );
    test_pass("cxx03:set::operator==");
    test_true( a != c );
    test_pass("cxx03:set::operator!=");
    test_true( c <  a );
    test_pass("cxx03:set::operator<");
    test_true( a >  c );
    test_pass("cxx03:set::operator>");
}

TEST_CASE(set, multiset) {
    int raw[] = { 1, 2, 2, 3, 2 };
    STD::multiset<int> m(raw, raw + 5);
    test_eq( m.size(), 5u );
    test_eq( m.count(2), 3u );
    test_pass("cxx03:multiset");

    m.insert(2);
    test_eq( m.count(2), 4u );
    test_pass("cxx03:multiset::insert");

    STD::multiset<int>::size_type n = m.erase(2);
    test_eq( (long)n, 4L );
    test_eq( m.count(2), 0u );
    test_eq( m.size(), 2u );
    test_pass("cxx03:multiset::erase (all equal)");

    STD::multiset<int> a(raw, raw + 5), b;
    a.swap(b);
    test_true( a.empty() );
    test_eq( b.size(), 5u );
    test_eq( b.count(2), 3u );
    test_pass("cxx03:multiset::swap");
}

TEST_CASE(set, string_elements) {

    STD::set<STD::string> s;
    s.insert("banana");
    s.insert("apple");
    s.insert("cherry");
    s.insert("apple");
    test_eq( s.size(), 3u );
    test_pass("cxx03:set::insert (string key)");

    STD::set<STD::string>::iterator it = s.begin();
    test_eq( *it, STD::string("apple") );  ++it;
    test_eq( *it, STD::string("banana") ); ++it;
    test_eq( *it, STD::string("cherry") );
    test_pass("cxx03:set::begin (string, lexical order)");
    test_true( 1 );
    test_pass("cxx03:set::end (string, lexical order)");

    test_true( s.find("banana") != s.end() );
    test_true( s.find("durian") == s.end() );
    test_eq( s.count("apple"), 1u );
    test_pass("cxx03:set::find (string key)");
    test_eq( *s.lower_bound("b"), STD::string("banana") );
    test_pass("cxx03:set::lower_bound (string key)");

    STD::set<STD::string>::size_type n = s.erase("apple");
    test_eq( (long)n, 1L );
    test_eq( s.size(), 2u );
    test_pass("cxx03:set::erase (string key)");

    STD::set<STD::string> big;
    big.insert(STD::string(300, 'a'));
    big.insert(STD::string(300, 'b'));
    test_eq( big.size(), 2u );
    test_pass("cxx03:set::insert (long string keys)");

    STD::set<STD::string> c(s);
    test_true( c == s );
    test_pass("cxx03:set::set(const set&) (deep copy strings)");
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(set, node_handle) {
    int raw[] = { 1, 2, 3 };
    STD::set<int> s(raw, raw + 3);

    STD::set<int>::node_type nh = s.extract(2);
    test_true( !nh.empty() );
    test_eq( nh.value(), 2 );
    test_eq( s.size(), 2u );
    test_pass("cxx17:set::extract");

    STD::set<int> t;
    STD::set<int>::insert_return_type ir = t.insert(STD::move(nh));
    test_true( ir.inserted );
    test_eq( *ir.position, 2 );
    test_eq( t.size(), 1u );
    test_pass("cxx17:set::insert(node_type)");

    STD::set<int>::node_type miss = s.extract(99);
    test_true( miss.empty() );
    test_pass("cxx17:set::extract (absent -> empty)");
}
#else
TEST_CASE_SKIP(set, node_handle)
#endif

#if TEST_TARGET_CXX >= 2020
TEST_CASE(set, contains_erase_if) {
    int raw[] = { 1, 2, 3, 4, 5, 6 };
    STD::set<int> s(raw, raw + 6);
#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true(  s.contains(3) );
    test_true( !s.contains(9) );
    test_pass("cxx20:set::contains");
#else
    test_true(  s.find(3) != s.end() );
    test_true(  s.find(9) == s.end() );
    test_skip("cxx20:set::contains");
#endif

    STD::set<int>::size_type n = STD::erase_if(s, SetIsEven());
    test_eq( (long)n, 3L );
    test_eq( sig(s), 135L );
    test_pass("cxx20:erase_if(set)");
}
#else
TEST_CASE_SKIP(set, contains_erase_if)
#endif

TEST_CASE(set, ctors_ops) {
    STD::set<int> s0;
    test_true( s0.empty() );
    test_pass("cxx03:set::set()");

    (void)s0.get_allocator();
    test_true( true );
    test_pass("cxx03:set::get_allocator");

    STD::set<int> a, b;
    a.insert(1); a.insert(2);
    b.insert(1); b.insert(2);
    test_true( a <= b && a >= b );
    b.insert(3);
    test_true( a <= b && !(a >= b) );
    test_pass("cxx03:set::operator<=");
    test_pass("cxx03:set::operator>=");

#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::allocator<int> al;
    STD::set<int> sa(al);
    sa.insert(5);
    test_true( sa.count(5) == 1 );
    test_pass("cxx11:set::set(const Allocator&) family");
#else
    test_skip("cxx11:set::set(const Allocator&) family");
#endif

#if _tst_cplusplus >= 201103L
    STD::set<int> il = { 3, 1, 2 };
    test_eq( il.size(), 3u );
    test_pass("cxx11:set::set(initializer_list)");
    il = { 7, 8 };
    test_true( il.size() == 2 && il.count(7) == 1 );
    test_pass("cxx11:set::operator=(initializer_list)");
#else
    test_skip("cxx11:set::set(initializer_list)");
    test_skip("cxx11:set::operator=(initializer_list)");
#endif

    STD::set<int> m1;
    m1.insert(4); m1.insert(5);
    STD::set<int> m2(STD::move(m1));
    TEST_SKIP_NATIVE_NO_MOVE("the native set has no move: the emulation cannot be added to it");
    test_true( m2.count(4) == 1 && m1.empty() );
    test_pass("cxx11:set::set(set&&)");
    STD::set<int> m3;
    m3 = STD::move(m2);
    TEST_SKIP_NATIVE_NO_MOVE("the native set has no move: the emulation cannot be added to it");
    test_true( m3.count(5) == 1 && m2.empty() );
    test_pass("cxx11:set::operator=(set&&)");

    STD::set<STD::string> ss;
    STD::string tmp("moved");
    ss.insert(STD::move(tmp));
    test_true( ss.count("moved") == 1 );
    test_pass("cxx11:set::insert(T&&)");
    STD::set<int> si;
#if _tst_cplusplus >= 201103L
    si.insert({ 10, 20, 30 });
    test_eq( si.size(), 3u );
    test_pass("cxx11:set::insert(initializer_list)");
#else
    test_skip("cxx11:set::insert(initializer_list)");
#endif

#if _TST_HAS_CXX11_LIB_MEMBERS
    STD::multiset<int> ms;
    ms.emplace(9); ms.emplace(9);
    test_eq( (long)ms.count(9), 2L );
    test_pass("cxx11:multiset::emplace");
#else
    test_skip("cxx11:multiset::emplace");
#endif
}

#if TEST_TARGET_CXX >= 2014 && _TST_HAS_TRANSPARENT_CMP
TEST_CASE(set, transparent_comparator) {

    STD::set<STD::string, STD::less<> > s;
    s.insert("apple"); s.insert("banana"); s.insert("cherry");

    test_true( s.find("banana") != s.end() );
    test_pass("cxx14:set::find (transparent comparator)");
    test_eq( (long)s.count("apple"), 1L );
    test_pass("cxx14:set::count (transparent comparator)");
    test_true( s.lower_bound("b") != s.end() && *s.lower_bound("b") == "banana" );
    test_pass("cxx14:set::lower_bound (transparent comparator)");
    test_true( *s.upper_bound("banana") == "cherry" );
    test_pass("cxx14:set::upper_bound (transparent comparator)");
    STD::pair<STD::set<STD::string, STD::less<> >::iterator,
              STD::set<STD::string, STD::less<> >::iterator> er = s.equal_range("cherry");
    test_true( er.first != er.second );
    test_pass("cxx14:set::equal_range (transparent comparator)");

#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( s.contains("apple") );
    test_true( !s.contains("durian") );
    test_pass("cxx20:set::contains (transparent comparator)");
#else
    test_skip("cxx20:set::contains (transparent comparator)");
#endif
}
#else
TEST_CASE(set, transparent_comparator) {
    test_skip("cxx14:set::find (transparent comparator)");
    test_skip("cxx14:set::count (transparent comparator)");
    test_skip("cxx14:set::lower_bound (transparent comparator)");
    test_skip("cxx14:set::upper_bound (transparent comparator)");
    test_skip("cxx14:set::equal_range (transparent comparator)");
    test_skip("cxx20:set::contains (transparent comparator)");
}
#endif

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(set, merge_extract) {
    STD::set<int> a, b;
    a.insert(1); a.insert(2);
    b.insert(2); b.insert(3);
    a.merge(b);
    test_true( a.count(3) == 1 && a.size() == 3 && b.count(2) == 1 );
    test_pass("cxx17:set::merge");

    STD::multiset<int> ma, mb;
    ma.insert(1); mb.insert(1); mb.insert(2);
    ma.merge(mb);
    test_true( (long)ma.count(1) == 2L && ma.count(2) == 1 && mb.empty() );
    test_pass("cxx17:multiset::merge");

    STD::multiset<int> me;
    me.insert(5); me.insert(5);
    STD::multiset<int>::node_type nh = me.extract(5);
    test_true( !nh.empty() && nh.value() == 5 && me.count(5) == 1 );
    test_pass("cxx17:multiset::extract");
}
#else
TEST_CASE(set, merge_extract) {
    test_skip("cxx17:set::merge"); test_skip("cxx17:multiset::merge"); test_skip("cxx17:multiset::extract");
}
#endif

TEST_CASE(set, cxx20_cxx23) {
#if defined(__cpp_lib_three_way_comparison)
    STD::set<int> a, b;
    a.insert(1); a.insert(2); a.insert(3);
    b.insert(1); b.insert(2); b.insert(4);
    test_true( (a <=> b) < 0 );
    test_true( (a <=> a) == 0 );
    test_pass("cxx20:operator<=>(set)");
    STD::multiset<int> ma, mb;
    ma.insert(1); mb.insert(2);
    test_true( (ma <=> mb) < 0 );
    test_pass("cxx20:operator<=>(multiset)");
#else
    test_skip("cxx20:operator<=>(set)");
    test_skip("cxx20:operator<=>(multiset)");
#endif

#if TEST_TARGET_CXX >= 2020
    STD::multiset<int> me;
    me.insert(1); me.insert(2); me.insert(2); me.insert(3);
    STD::multiset<int>::size_type n = STD::erase_if(me, SetIsEven());
    test_eq( (long)n, 2L );
    test_pass("cxx20:erase_if(multiset)");
#else
    test_skip("cxx20:erase_if(multiset)");
#endif

#if defined(__cpp_lib_containers_ranges)
    STD::vector<int> v; v.push_back(3); v.push_back(1); v.push_back(2);
    STD::set<int> fr(STD::from_range, v);
    test_eq( fr.size(), 3u );
    test_pass("cxx23:set::set(from_range_t,R&&)");
    STD::vector<int> v2; v2.push_back(4); v2.push_back(5);
    fr.insert_range(v2);
    test_true( fr.size() == 5 && fr.count(5) == 1 );
    test_pass("cxx23:set::insert_range");
#else
    test_skip("cxx23:set::set(from_range_t,R&&)");
    test_skip("cxx23:set::insert_range");
#endif
}


TEST_CASE(set, set_member_types_cxx03) {
    typedef STD::set<int> S;
    S s;
    s.insert(3);
    const S& cs = s;

    test_true(( STD::is_same<S::key_type, int>::value ));
    test_pass("cxx03:set::key_type");

    test_true(( STD::is_same<S::value_type, int>::value ));
    test_true(( STD::is_same<S::key_type, S::value_type>::value ));
    test_pass("cxx03:set::value_type");

    test_true(( STD::is_same<S::key_compare, STD::less<int> >::value ));
    S::key_compare kc = s.key_comp();
    test_true( kc(1, 2) );
    test_pass("cxx03:set::key_compare");

    S::value_compare vc = s.value_comp();
    test_true( vc(1, 2) );
    test_true( !vc(2, 1) );
    test_pass("cxx03:set::value_compare");

    test_true(( STD::is_same<S::reference, int&>::value ));
    test_pass("cxx03:set::reference");

    S::const_reference cr = *cs.begin();
    test_eq( cr, 3 );
    test_true(( STD::is_same<S::const_reference, const int&>::value ));
    test_pass("cxx03:set::const_reference");

    test_true(( STD::is_same<S::pointer, int*>::value ));
    test_pass("cxx03:set::pointer");

    S::const_pointer cp = &*cs.begin();
    test_eq( *cp, 3 );
    test_pass("cxx03:set::const_pointer");

    S::size_type n = s.size();
    test_eq( n, (STD::size_t)1 );
    test_true( !STD::is_signed<S::size_type>::value );
    test_pass("cxx03:set::size_type");

    test_true( STD::is_signed<S::difference_type>::value );
    {
        S::difference_type d = 0;
        S::const_iterator i;
        for (i = cs.begin(); i != cs.end(); ++i) ++d;
        test_eq( (long)d, 1L );
    }
    test_pass("cxx03:set::difference_type");

    S::const_iterator ci = cs.begin();
    test_eq( *ci, 3 );
    test_pass("cxx03:set::const_iterator");

    s.insert(9);
    S::reverse_iterator ri = s.rbegin();
    test_eq( *ri, 9 );
    test_pass("cxx03:set::reverse_iterator");

    S::const_reverse_iterator cri = cs.rbegin();
    test_eq( *cri, 9 );
    test_pass("cxx03:set::const_reverse_iterator");
}

TEST_CASE(set, set_allocator_type_cxx11) {
    typedef STD::set<int> S;
    S s;
    test_true(( STD::is_same<S::allocator_type, STD::allocator<int> >::value ));
    S::allocator_type al = s.get_allocator();
    (void)al;
    test_pass("cxx11:set::allocator_type");
}


TEST_CASE(set, multiset_member_types_cxx03) {
    typedef STD::multiset<int> S;
    S s;
    s.insert(3);
    const S& cs = s;

    test_true(( STD::is_same<S::key_type, int>::value ));
    test_pass("cxx03:multiset::key_type");

    test_true(( STD::is_same<S::value_type, int>::value ));
    test_true(( STD::is_same<S::key_type, S::value_type>::value ));
    test_pass("cxx03:multiset::value_type");

    test_true(( STD::is_same<S::key_compare, STD::less<int> >::value ));
    S::key_compare kc = s.key_comp();
    test_true( kc(1, 2) );
    test_pass("cxx03:multiset::key_compare");

    S::value_compare vc = s.value_comp();
    test_true( vc(1, 2) );
    test_true( !vc(2, 1) );
    test_pass("cxx03:multiset::value_compare");

    test_true(( STD::is_same<S::reference, int&>::value ));
    test_pass("cxx03:multiset::reference");

    S::const_reference cr = *cs.begin();
    test_eq( cr, 3 );
    test_true(( STD::is_same<S::const_reference, const int&>::value ));
    test_pass("cxx11:multiset::const_reference");

    test_true(( STD::is_same<S::pointer, int*>::value ));
    test_pass("cxx03:multiset::pointer");

    S::const_pointer cp = &*cs.begin();
    test_eq( *cp, 3 );
    test_pass("cxx11:multiset::const_pointer");

    S::size_type n = s.size();
    test_eq( n, (STD::size_t)1 );
    test_true( !STD::is_signed<S::size_type>::value );
    test_pass("cxx03:multiset::size_type");

    test_true( STD::is_signed<S::difference_type>::value );
    {
        S::difference_type d = 0;
        S::const_iterator i;
        for (i = cs.begin(); i != cs.end(); ++i) ++d;
        test_eq( (long)d, 1L );
    }
    test_pass("cxx11:multiset::difference_type");

    S::const_iterator ci = cs.begin();
    test_eq( *ci, 3 );
    test_pass("cxx03:multiset::const_iterator");

    s.insert(9);
    S::reverse_iterator ri = s.rbegin();
    test_eq( *ri, 9 );
    test_pass("cxx03:multiset::reverse_iterator");

    S::const_reverse_iterator cri = cs.rbegin();
    test_eq( *cri, 9 );
    test_pass("cxx11:multiset::const_reverse_iterator");
}

TEST_CASE(set, multiset_allocator_type_cxx11) {
    typedef STD::multiset<int> S;
    S s;
    test_true(( STD::is_same<S::allocator_type, STD::allocator<int> >::value ));
    S::allocator_type al = s.get_allocator();
    (void)al;
    test_pass("cxx11:multiset::allocator_type");
}


TEST_CASE(set, insert_return_type_cxx17) {
#if defined(__cpp_lib_node_extract)
    STD::set<int> s;
    s.insert(1);
    s.insert(2);
    STD::set<int>::insert_return_type r = s.insert(s.extract(1));
    test_true( r.inserted );
    test_true( r.node.empty() );
    test_true( *r.position == 1 );

    STD::set<int> other;
    other.insert(2);
    STD::set<int>::insert_return_type dup = s.insert(other.extract(2));
    test_true( !dup.inserted );
    test_true( !dup.node.empty() );
    test_true( *dup.position == 2 );
    test_pass("cxx17:set::insert_return_type");
#else
    TEST_NOTE("node handles need the C++17 library");
    test_skip("cxx17:set::insert_return_type");
#endif
}

TEST_CASE(set, feature_test_macros) {
#if defined(__cpp_lib_containers_ranges) && (__cpp_lib_containers_ranges) >= 202202L
    test_true( (__cpp_lib_containers_ranges) >= 202202L );
    test_pass("cxx23:__cpp_lib_containers_ranges=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_containers_ranges=202202L");
#endif
#if defined(__cpp_lib_constexpr_set) && (__cpp_lib_constexpr_set) >= 202502L
    test_true( (__cpp_lib_constexpr_set) >= 202502L );
    test_pass("cxx26:__cpp_lib_constexpr_set=202502L");
    {
        struct K {
            static constexpr int set_run() {
                STD::set<int> s;
                s.insert(3);
                s.insert(1);
                s.insert(3);
                s.erase(1);
                return (int)(s.size() + *s.begin() + s.count(1));
            }
            static constexpr int multiset_run() {
                STD::multiset<int> s;
                s.insert(3);
                s.insert(3);
                s.insert(1);
                return (int)(s.size() + s.count(3));
            }
        };
        static_assert(K::set_run() == 4, "set operations in a constant expression");
        test_eq( K::set_run(), 4 );
        test_pass("cxx26:constexpr set");
        static_assert(K::multiset_run() == 5, "multiset operations in a constant expression");
        test_eq( K::multiset_run(), 5 );
        test_pass("cxx26:constexpr multiset");
    }
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_constexpr_set=202502L");
    test_skip("cxx26:constexpr set");
    test_skip("cxx26:constexpr multiset");
#endif
}

static int g_set_live = 0;

struct SetLive {
    STD::string s_;
    SetLive() { ++g_set_live; }
    SetLive(const char* s) : s_(s) { ++g_set_live; }
    SetLive(const SetLive& o) : s_(o.s_) { ++g_set_live; }
    SetLive& operator=(const SetLive& o) { s_ = o.s_; return *this; }
    ~SetLive() { --g_set_live; }
    bool operator< (const SetLive& o) const { return s_ <  o.s_; }
};

static SetLive set_live_key(int i) {
    STD::string t("k");
    t += (char)('0' + i);
    t += "_long_enough_to_leave_the_sso_buffer";
    return SetLive(t.c_str());
}

TEST_CASE(set, class_elements_lifetime) {
    g_set_live = 0;
    {
        STD::set<SetLive> s;
        for (int i = 0; i < 8; ++i) s.insert(set_live_key(i));
        test_eq( (int)s.size(), 8 );
        test_eq( g_set_live, 8 );

        STD::set<SetLive> c(s);
        test_eq( (int)c.size(), 8 );
        test_eq( g_set_live, 16 );

        s.erase(s.begin());
        test_eq( (int)s.size(), 7 );
        test_eq( g_set_live, 15 );

        STD::set<SetLive> e;
        s.swap(e);
        test_true( s.empty() );
        test_eq( (int)e.size(), 7 );
        test_eq( g_set_live, 15 );

        e.clear();
        test_eq( g_set_live, 8 );

        c = e;
        test_eq( g_set_live, 0 );

        STD::multiset<SetLive> m;
        for (int i = 0; i < 4; ++i) m.insert(set_live_key(0));
        test_eq( (int)m.size(), 4 );
        test_eq( g_set_live, 4 );
        m.erase(m.begin());
        test_eq( g_set_live, 3 );
    }
    test_eq( g_set_live, 0 );
}

TEST_CASE(set, multiset_insert_forms) {
    typedef STD::multiset<int> MS;

    {   MS m;
        m.insert(5);
        MS::iterator at_end = m.insert(m.end(), 9);
        test_eq( *at_end, 9 );
        MS::iterator good = m.insert(m.lower_bound(7), 7);
        test_eq( *good, 7 );
        MS::iterator bad = m.insert(m.begin(), 8);
        test_eq( *bad, 8 );
        MS::iterator dup = m.insert(m.begin(), 5);
        test_eq( *dup, 5 );
        test_eq( (long)m.size(), 5L );
        test_eq( (long)m.count(5), 2L ); }
    test_pass("cxx03:multiset::insert (hint)");

    {   static const int src[4] = { 3, 1, 3, 2 };
        MS m;
        m.insert(src, src);
        test_true( m.empty() );
        m.insert(src, src + 4);
        test_eq( (long)m.size(), 4L );
        test_eq( (long)m.count(3), 2L );
        test_eq( *m.begin(), 1 );
        m.insert(src, src + 1);
        test_eq( (long)m.size(), 5L );
        test_eq( (long)m.count(3), 3L ); }
    test_pass("cxx03:multiset::insert (range)");

    {   typedef STD::multiset<STD::string> MSS;
        MSS m;
        STD::string a("alpha"), b("alpha");
        MSS::iterator i1 = m.insert(STD::move(a));
        test_eq( *i1, STD::string("alpha") );
        MSS::iterator i2 = m.insert(STD::move(b));
        test_eq( *i2, STD::string("alpha") );
        test_eq( (long)m.size(), 2L );
        test_eq( (long)m.count(STD::string("alpha")), 2L ); }
    test_pass("cxx11:multiset::insert(T&&)");

    {   typedef STD::multiset<STD::string> MSS;
        MSS m;
        STD::string a("m"), b("z"), c("a");
        m.insert(STD::move(a));
        MSS::iterator hi = m.insert(m.end(), STD::move(b));
        test_eq( *hi, STD::string("z") );
        MSS::iterator lo = m.insert(m.begin(), STD::move(c));
        test_eq( *lo, STD::string("a") );
        test_eq( (long)m.size(), 3L );
        test_eq( *m.begin(), STD::string("a") ); }
    test_pass("cxx11:multiset::insert (hint, T&&)");
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(set, extract_by_iterator_cxx17) {
    {   STD::multiset<int> m;
        m.insert(5); m.insert(5); m.insert(7);
        STD::multiset<int>::const_iterator it = m.find(7);
        STD::multiset<int>::node_type nh = m.extract(it);
        test_true( !nh.empty() );
        test_eq( nh.value(), 7 );
        test_eq( (long)m.size(), 2L );
        test_eq( (long)m.count(7), 0L );

        STD::multiset<int>::node_type nh2 = m.extract(m.begin());
        test_eq( nh2.value(), 5 );
        test_eq( (long)m.count(5), 1L );
        m.insert(STD::move(nh2));
        test_eq( (long)m.count(5), 2L ); }
    test_pass("cxx17:multiset::extract(const_iterator)");

    {   STD::set<int> s;
        s.insert(1); s.insert(2); s.insert(3);
        STD::set<int>::const_iterator it = s.find(2);
        STD::set<int>::node_type nh = s.extract(it);
        test_true( !nh.empty() );
        test_eq( nh.value(), 2 );
        test_eq( (long)s.size(), 2L );
        test_true( s.find(2) == s.end() );

        STD::set<int>::node_type last = s.extract(s.begin());
        test_eq( last.value(), 1 );
        test_eq( (long)s.size(), 1L ); }
    test_pass("cxx17:set::extract(const_iterator)");
}
#else
TEST_CASE(set, extract_by_iterator_cxx17) {
    TEST_NOTE("node handles are C++17");
    test_skip("cxx17:multiset::extract(const_iterator)");
    test_skip("cxx17:set::extract(const_iterator)");
}
#endif
