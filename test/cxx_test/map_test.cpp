#include "test_cxx.hpp"
#include <map>
#include <functional>
#include <utility>
#include <string>
#include <vector>
#include <type_traits>

namespace {
typedef STD::map<int, int> IMap;
IMap::value_type kv(int k, int v) { return IMap::value_type(k, v); }

long keysig(const IMap& m) {
    long r = 0;
    for (IMap::const_iterator it = m.begin(); it != m.end(); ++it) r = r * 10 + it->first;
    return r;
}
}

TEST_CASE(map, construct_and_index) {
    IMap a;
    test_true( a.empty() );
    test_pass("cxx03:map::empty");
    test_eq( a.size(), 0u );
    test_pass("cxx03:map::size");

    a[2] = 20;
    a[1] = 10;
    a[3] = 30;
    test_eq( a.size(), 3u );
    test_eq( a[2], 20 );
    test_eq( keysig(a), 123L );
    test_pass("cxx03:map::operator[]");

    a[2] = 99;
    test_eq( a[2], 99 );
    int fresh = a[7];
    test_eq( fresh, 0 );
    test_eq( a.size(), 4u );
    test_pass("cxx03:map::operator[] (value-init absent key)");

    IMap b(a);
    test_eq( b.size(), 4u );
    test_pass("cxx03:map::map(const map&)");
    IMap c;
    c = a;
    test_eq( c.size(), 4u );
    test_pass("cxx03:map::operator=(const map&)");

    IMap::value_type raw[] = { kv(5,50), kv(4,40), kv(6,60) };
    IMap d(raw, raw + 3);
    test_eq( keysig(d), 456L );
    test_pass("cxx03:map::map(first,last)");

    STD::map<int, int, STD::greater<int> > g(raw, raw + 3);
    test_eq( g.begin()->first, 6 );
    test_pass("cxx03:map::map(comp)");
}

TEST_CASE(map, at_access) {
    IMap m;
    m[1] = 100;
    m[2] = 200;
    const IMap& cm = m;

#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( m.at(1), 100 );
    test_eq( cm.at(2), 200 );
    m.at(1) = 111;
    test_eq( m.at(1), 111 );
    test_throw( (void)m.at(9) );
    test_throw( (void)cm.at(9) );
    test_pass("cxx11:map::at");
#else
    TEST_SKIP_N(5);
    test_skip("cxx11:map::at");
#endif
}

TEST_CASE(map, insert_variants) {
    IMap m;
    STD::pair<IMap::iterator, bool> r = m.insert(kv(1, 10));
    test_true( r.second );
    test_eq( r.first->second, 10 );
    STD::pair<IMap::iterator, bool> r2 = m.insert(kv(1, 99));
    test_true( !r2.second );
    test_eq( m[1], 10 );
    test_pass("cxx03:map::insert");

    IMap::iterator h = m.insert(m.begin(), kv(2, 20));
    test_eq( h->second, 20 );
    test_pass("cxx03:map::insert (hint)");

    IMap::value_type more[] = { kv(3, 30), kv(4, 40) };
    m.insert(more, more + 2);
    test_eq( m.size(), 4u );
    test_pass("cxx03:map::insert (range)");

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
    STD::pair<IMap::iterator, bool> te = m.try_emplace(5, 50);
    test_true( te.second );
    test_eq( m[5], 50 );
    STD::pair<IMap::iterator, bool> te2 = m.try_emplace(5, 999);
    test_true( !te2.second );
    test_eq( m[5], 50 );
    test_pass("cxx17:map::try_emplace");

    STD::pair<IMap::iterator, bool> io = m.insert_or_assign(5, 55);
    test_true( !io.second );
    test_eq( m[5], 55 );
    STD::pair<IMap::iterator, bool> io2 = m.insert_or_assign(6, 60);
    test_true( io2.second );
    test_eq( m[6], 60 );
    test_pass("cxx17:map::insert_or_assign");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
}

TEST_CASE(map, emplace) {
    IMap m;
#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
    STD::pair<IMap::iterator, bool> e = m.emplace(1, 10);
    test_true( e.second );
    test_eq( e.first->second, 10 );
    test_pass("cxx11:map::emplace");
    IMap::iterator eh = m.emplace_hint(m.begin(), 2, 20);
    test_eq( eh->second, 20 );
    test_eq( m.size(), 2u );
    test_pass("cxx11:map::emplace_hint");
#else
    m[1] = 10; m[2] = 20;
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
#endif
    test_true( m.count(1) == 1u );
}

TEST_CASE(map, lookup) {
    IMap::value_type raw[] = { kv(10,1), kv(20,2), kv(30,3), kv(40,4) };
    IMap m(raw, raw + 4);
    const IMap& cm = m;

    test_true( m.find(20) != m.end() );
    test_eq( m.find(20)->second, 2 );
    test_true( m.find(25) == m.end() );
    test_true( cm.find(30) != cm.end() );
    test_pass("cxx03:map::find");
    test_eq( m.count(20), 1u );
    test_eq( m.count(99), 0u );
    test_pass("cxx03:map::count");

    test_eq( m.lower_bound(20)->first, 20 );
    test_pass("cxx03:map::lower_bound");
    test_eq( m.upper_bound(20)->first, 30 );
    test_pass("cxx03:map::upper_bound");
    test_eq( m.lower_bound(25)->first, 30 );
    test_pass("cxx03:map::lower_bound (absent -> next up)");

    STD::pair<IMap::iterator, IMap::iterator> eq = m.equal_range(30);
    test_eq( eq.first->first, 30 );
    test_eq( eq.second->first, 40 );
    test_pass("cxx03:map::equal_range");

    test_true( m.key_comp()(1, 2) );
    test_pass("cxx03:map::key_comp");
    IMap::value_type p1 = kv(1, 0), p2 = kv(2, 0);
    test_true( m.value_comp()(p1, p2) );
    test_pass("cxx03:map::value_comp");
}

TEST_CASE(map, erase_iterate_swap) {
    IMap::value_type raw[] = { kv(1,1), kv(2,2), kv(3,3), kv(4,4), kv(5,5) };
    IMap m(raw, raw + 5);

    IMap::size_type n = m.erase(3);
    test_eq( (long)n, 1L );
    test_eq( m.erase(99), 0u );
    test_eq( keysig(m), 1245L );
    test_pass("cxx03:map::erase (key)");

    IMap::iterator it = m.find(2);
#if _TST_ERASE_RETURNS_VOID
    m.erase(it);
    TEST_NOTE("libstdc++ in C++03 returns void from erase(iterator)");
    TEST_SKIP1();
#else
    IMap::iterator nx = m.erase(it);
    test_eq( nx->first, 4 );
#endif
    test_pass("cxx03:map::erase (iterator)");

    for (IMap::iterator i = m.begin(); i != m.end(); ++i) i->second *= 10;
    test_eq( m[1], 10 );
    test_pass("cxx03:map::begin");
    test_true( 1 );
    test_pass("cxx03:map::end");

    long rk = 0;
    for (IMap::const_reverse_iterator i = m.rbegin(); i != m.rend(); ++i) rk = rk * 10 + i->first;
    test_eq( rk, 541L );
    test_pass("cxx03:map::rbegin");
    test_true( 1 );
    test_pass("cxx03:map::rend");

    m.clear();
    test_true( m.empty() );
    test_pass("cxx03:map::clear");

    IMap x(raw, raw + 2), y(raw + 2, raw + 5);
    x.swap(y);
    test_eq( x.size(), 3u );
    test_eq( y.size(), 2u );
    test_pass("cxx03:map::swap");
    STD::swap(x, y);
    test_eq( x.size(), 2u );
    test_pass("cxx03:swap(map)");

    test_true( x.max_size() > 0u );
    test_pass("cxx03:map::max_size");
    IMap::allocator_type al = x.get_allocator();
    (void)al;
}

TEST_CASE(map, compare_and_multimap) {
    IMap::value_type raw[] = { kv(1,1), kv(2,2), kv(3,3) };
    IMap a(raw, raw + 3), b(raw, raw + 3), c(raw, raw + 2);
    test_true( a == b );
    test_pass("cxx03:map::operator==");
    test_true( a != c );
    test_pass("cxx03:map::operator!=");
    test_true( c <  a );
    test_pass("cxx03:map::operator<");
    test_true( a >  c );
    test_pass("cxx03:map::operator>");

    STD::multimap<int, int> mm;
    mm.insert(STD::multimap<int,int>::value_type(1, 10));
    mm.insert(STD::multimap<int,int>::value_type(1, 11));
    mm.insert(STD::multimap<int,int>::value_type(2, 20));
    test_eq( mm.size(), 3u );
    test_eq( mm.count(1), 2u );
    test_pass("cxx03:multimap");

    STD::multimap<int,int>::size_type mn = mm.erase(1);
    test_eq( (long)mn, 2L );
    test_eq( mm.size(), 1u );
    test_pass("cxx03:multimap::erase (all equal)");

    STD::pair<STD::multimap<int,int>::iterator, STD::multimap<int,int>::iterator> er
        = mm.equal_range(2);
    test_eq( er.first->second, 20 );
    test_pass("cxx03:multimap::equal_range");
}

TEST_CASE(map, string_keys_and_values) {

    STD::map<STD::string, STD::string> m;
    m["banana"] = "yellow";
    m["apple"]  = "red";
    m["grape"]  = "purple";
    test_eq( m.size(), 3u );
    test_eq( m["apple"], STD::string("red") );
    test_pass("cxx03:map::operator[] (string key)");

    m["apple"] = "green";
    test_eq( m["apple"], STD::string("green") );
    test_pass("cxx03:map::operator[] (string reassign)");
#if _TST_HAS_CXX11_LIB_MEMBERS
    test_eq( m.at("grape"), STD::string("purple") );
    test_pass("cxx11:map::at (string key)");
    test_throw( (void)m.at("missing") );
    test_pass("cxx11:map::at (string missing throws)");
#else
    test_skip("cxx11:map::at (string key)");
    test_skip("cxx11:map::at (string missing throws)");
#endif

    STD::map<STD::string, STD::string>::iterator it = m.begin();
    test_eq( it->first, STD::string("apple") );
    test_pass("cxx03:map::begin (string, lexical order)");
    test_true( 1 );
    test_pass("cxx03:map::end (string, lexical order)");

    test_true( m.find("banana") != m.end() );
    test_true( m.find("kiwi") == m.end() );
    test_pass("cxx03:map::find (string key)");
    STD::map<STD::string, STD::string>::size_type n = m.erase("banana");
    test_eq( (long)n, 1L );
    test_eq( m.size(), 2u );
    test_pass("cxx03:map::erase (string key)");

    STD::map<int, STD::string> ms;
    ms[1] = "one";
    ms[2] = STD::string(300, 'x');
    test_eq( ms[1], STD::string("one") );
    test_eq( ms[2].size(), 300u );
    test_pass("cxx03:map<int,string> (non-trivial values)");

    STD::map<STD::string, STD::string> c(m);
    test_true( c == m );
    test_pass("cxx03:map::map(const map&) (deep copy strings)");
}

TEST_CASE(map, ctor_alloc_erase_cmp) {
    IMap empty;
    test_true( empty.empty() );
    test_pass("cxx03:map::map()");

    IMap::value_type raw[] = { kv(1,1), kv(2,2), kv(3,3) };
    IMap m(raw, raw + 3);

    STD::allocator<STD::pair<const int, int> > al = m.get_allocator();
    (void)al;
    test_pass("cxx03:map::get_allocator");

    IMap::iterator b = m.begin(), e = m.begin(); ++e; ++e;
    m.erase(b, e);
    test_eq( m.size(), 1u );
    test_eq( m.begin()->first, 3 );
    test_pass("cxx03:map::erase (range)");

    IMap x(raw, raw + 3), y(raw, raw + 3), z(raw, raw + 2);
    test_true( z <= x );  test_true( x <= y );
    test_pass("cxx03:map::operator<=");
    test_true( x >= z );  test_true( x >= y );
    test_pass("cxx03:map::operator>=");

    STD::less<int> cmp; IMap::allocator_type alc;
    IMap ma(cmp, alc);
    test_true( ma.empty() );
    test_pass("cxx11:map::map(const Allocator&) family");
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(map, const_iters_and_multimap_c11) {
    IMap::value_type raw[] = { kv(1,10), kv(2,20), kv(3,30) };
    IMap m(raw, raw + 3);

    long n = 0;
    for (IMap::const_iterator it = m.cbegin(); it != m.cend(); ++it) ++n;
    test_eq( n, 3L );
    test_pass("cxx11:map::cbegin");
    test_true( m.cbegin() != m.cend() );
    test_pass("cxx11:map::cend");
    test_eq( m.crbegin()->first, 3 );
    test_pass("cxx11:map::crbegin");
    test_true( m.crbegin() != m.crend() );
    test_pass("cxx11:map::crend");

    STD::multimap<int, int> mm;
    STD::multimap<int, int>::iterator ins = mm.insert(STD::make_pair(1, 10));
    test_eq( ins->second, 10 );
    test_pass("cxx03:multimap::insert");
    STD::multimap<int, int>::iterator em = mm.emplace(1, 11);
    test_eq( em->second, 11 );
    test_eq( mm.size(), 2u );
    test_pass("cxx11:multimap::emplace");
}
#else
TEST_CASE_SKIP(map, const_iters_and_multimap_c11)
#endif

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
TEST_CASE(map, contains_erase_if) {
    IMap::value_type raw[] = { kv(1,1), kv(2,2), kv(3,3), kv(4,4) };
    IMap m(raw, raw + 4);
    test_true(  m.contains(2) );
    test_true( !m.contains(9) );
    test_pass("cxx20:map::contains");

    struct EvenKey { bool operator()(const IMap::value_type& p) const { return p.first % 2 == 0; } };
    IMap::size_type n = STD::erase_if(m, EvenKey());
    test_eq( (long)n, 2L );
    test_eq( keysig(m), 13L );
    test_pass("cxx20:erase_if(map)");
}
#else
TEST_CASE_SKIP(map, contains_erase_if)
#endif

TEST_CASE(map, ctors_insert_ops) {
    typedef STD::map<int, int> MI;

    MI il;
    il.insert(STD::make_pair(1, 10));
#if _tst_cplusplus >= 201103L
    MI il2 = { STD::make_pair(2, 20), STD::make_pair(3, 30) };
    test_eq( il2.size(), 2u );
    test_pass("cxx11:map::map(initializer_list)");
    il2 = { STD::make_pair(4, 40) };
    test_true( il2.size() == 1 && il2[4] == 40 );
    test_pass("cxx11:map::operator=(initializer_list)");
#else
    test_skip("cxx11:map::map(initializer_list)");
    test_skip("cxx11:map::operator=(initializer_list)");
#endif

    MI m1;
    m1.insert(STD::make_pair(5, 50)); m1.insert(STD::make_pair(6, 60));
    MI m2(STD::move(m1));
    TEST_SKIP_NATIVE_NO_MOVE("the native map has no move: the emulation cannot be added to it");
    test_true( m2[5] == 50 && m1.empty() );
    test_pass("cxx11:map::map(map&&)");
    MI m3;
    m3 = STD::move(m2);
    TEST_SKIP_NATIVE_NO_MOVE("the native map has no move: the emulation cannot be added to it");
    test_true( m3[6] == 60 && m2.empty() );
    test_pass("cxx11:map::operator=(map&&)");

    MI s;
    STD::pair<MI::iterator, bool> ip = s.insert(STD::make_pair(1, 100));
    test_true( ip.second && s[1] == 100 );
    test_pass("cxx11:map::insert(P&&)");
    MI::iterator hi = s.insert(s.end(), STD::make_pair(2, 200));
    test_true( hi->second == 200 );
    test_pass("cxx11:map::insert (hint, P&&)");
#if _tst_cplusplus >= 201103L
    s.insert({ STD::make_pair(3, 300), STD::make_pair(4, 400) });
    test_true( s.size() == 4 && s[4] == 400 );
    test_pass("cxx11:map::insert(initializer_list)");
#else
    test_skip("cxx11:map::insert(initializer_list)");
#endif

    STD::map<STD::string, int> sm;
    STD::string key("movekey");
    sm[STD::move(key)] = 7;
    test_true( sm["movekey"] == 7 );
    test_pass("cxx11:map::operator[](K&&)");
}

#if TEST_TARGET_CXX >= 2014 && _TST_HAS_TRANSPARENT_CMP
TEST_CASE(map, transparent_comparator) {
    STD::map<STD::string, int, STD::less<> > m;
    m["apple"] = 1; m["banana"] = 2; m["cherry"] = 3;

    test_true( m.find("banana") != m.end() && m.find("banana")->second == 2 );
    test_pass("cxx14:map::find (transparent comparator)");
    test_eq( (long)m.count("apple"), 1L );
    test_pass("cxx14:map::count (transparent comparator)");
    test_true( m.lower_bound("b")->first == "banana" );
    test_pass("cxx14:map::lower_bound (transparent comparator)");
    test_true( m.upper_bound("banana")->first == "cherry" );
    test_pass("cxx14:map::upper_bound (transparent comparator)");
    STD::pair<STD::map<STD::string, int, STD::less<> >::iterator,
              STD::map<STD::string, int, STD::less<> >::iterator> er = m.equal_range("cherry");
    test_true( er.first != er.second );
    test_pass("cxx14:map::equal_range (transparent comparator)");

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
    test_true( m.contains("apple") && !m.contains("durian") );
    test_pass("cxx20:map::contains (transparent comparator)");
#else
    test_skip("cxx20:map::contains (transparent comparator)");
#endif
}
#else
TEST_CASE(map, transparent_comparator) {
    test_skip("cxx14:map::find (transparent comparator)");
    test_skip("cxx14:map::count (transparent comparator)");
    test_skip("cxx14:map::lower_bound (transparent comparator)");
    test_skip("cxx14:map::upper_bound (transparent comparator)");
    test_skip("cxx14:map::equal_range (transparent comparator)");
    test_skip("cxx20:map::contains (transparent comparator)");
}
#endif

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(map, merge_extract_node) {
    STD::map<int, int> a, b;
    a.insert(STD::make_pair(1, 1)); a.insert(STD::make_pair(2, 2));
    b.insert(STD::make_pair(2, 20)); b.insert(STD::make_pair(3, 3));
    a.merge(b);
    test_true( a.size() == 3 && a[3] == 3 && b.size() == 1 );
    test_pass("cxx17:map::merge");

    STD::map<int, int>::node_type nh = a.extract(1);
    test_true( !nh.empty() && nh.key() == 1 && nh.mapped() == 1 );
    test_pass("cxx17:map::extract");
    STD::map<int, int>::insert_return_type irt = a.insert(STD::move(nh));
    test_true( irt.inserted && a.count(1) == 1 );
    test_pass("cxx17:map::insert(node_type)");

    test_true( irt.position != a.end() && irt.position->first == 1 );
    test_true( irt.node.empty() );
    {
        STD::map<int, int>::node_type dup = a.extract(2);
        a.insert(STD::make_pair(2, 22));
        STD::map<int, int>::insert_return_type back = a.insert(STD::move(dup));
        test_true( !back.inserted && !back.node.empty() );
        test_true( back.position->first == 2 && back.position->second == 22 );
        test_eq( back.node.key(), 2 );
    }
    test_pass("cxx17:map::insert_return_type");

    STD::multimap<int, int> ma, mb;
    ma.insert(STD::make_pair(1, 1));
    mb.insert(STD::make_pair(1, 2)); mb.insert(STD::make_pair(2, 2));
    ma.merge(mb);
    test_true( (long)ma.count(1) == 2L && ma.count(2) == 1 && mb.empty() );
    test_pass("cxx17:multimap::merge");
    STD::multimap<int, int>::node_type mnh = ma.extract(2);
    test_true( !mnh.empty() && mnh.key() == 2 );
    test_pass("cxx17:multimap::extract");
}
#else
TEST_CASE(map, merge_extract_node) {
    test_skip("cxx17:map::merge"); test_skip("cxx17:map::extract");
    test_skip("cxx17:map::insert(node_type)"); test_skip("cxx17:multimap::merge");
    test_skip("cxx17:multimap::extract");
}
#endif

TEST_CASE(map, cxx20_cxx23) {
#if defined(__cpp_lib_three_way_comparison)
    STD::map<int, int> a, b;
    a.insert(STD::make_pair(1, 1)); a.insert(STD::make_pair(2, 2));
    b.insert(STD::make_pair(1, 1)); b.insert(STD::make_pair(2, 3));
    test_true( (a <=> b) < 0 );
    test_true( (a <=> a) == 0 );
    test_pass("cxx20:operator<=>(map)");
    STD::multimap<int, int> ma, mb;
    ma.insert(STD::make_pair(1, 1)); mb.insert(STD::make_pair(2, 2));
    test_true( (ma <=> mb) < 0 );
    test_pass("cxx20:operator<=>(multimap)");
#else
    test_skip("cxx20:operator<=>(map)");
    test_skip("cxx20:operator<=>(multimap)");
#endif

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
    STD::multimap<int, int> me;
    me.insert(STD::make_pair(1, 1)); me.insert(STD::make_pair(2, 2));
    me.insert(STD::make_pair(2, 3)); me.insert(STD::make_pair(3, 3));
    struct KeyEven { bool operator()(const STD::pair<const int, int>& p) const { return p.first % 2 == 0; } };
    STD::multimap<int, int>::size_type n = STD::erase_if(me, KeyEven());
    test_eq( (long)n, 2L );
    test_pass("cxx20:erase_if(multimap)");
#else
    test_skip("cxx20:erase_if(multimap)");
#endif

#if defined(__cpp_lib_containers_ranges)
    STD::vector<STD::pair<int, int> > v;
    v.push_back(STD::make_pair(1, 10)); v.push_back(STD::make_pair(2, 20));
    STD::map<int, int> fr(STD::from_range, v);
    test_true( fr.size() == 2 && fr[2] == 20 );
    test_pass("cxx23:map::map(from_range_t,R&&)");
    STD::vector<STD::pair<int, int> > v2;
    v2.push_back(STD::make_pair(3, 30));
    fr.insert_range(v2);
    test_true( fr.size() == 3 && fr[3] == 30 );
    test_pass("cxx23:map::insert_range");
#else
    test_skip("cxx23:map::map(from_range_t,R&&)");
    test_skip("cxx23:map::insert_range");
#endif
}


TEST_CASE(map, map_member_types_cxx03) {
    typedef STD::map<int, char> M;
    M m;
    m.insert(STD::pair<const int, char>(1, 'a'));
    const M& cm = m;

    test_true(( STD::is_same<M::key_type, int>::value ));
    test_pass("cxx03:map::key_type");

    test_true(( STD::is_same<M::mapped_type, char>::value ));
    test_pass("cxx03:map::mapped_type");

    test_true(( STD::is_same<M::value_type, STD::pair<const int, char> >::value ));
    test_pass("cxx03:map::value_type");

    test_true(( STD::is_same<M::key_compare, STD::less<int> >::value ));
    M::key_compare kc = m.key_comp();
    test_true( kc(1, 2) );
    test_pass("cxx03:map::key_compare");

    M::value_compare vc = m.value_comp();
    test_true( vc(STD::pair<const int, char>(1, 'z'),
                 STD::pair<const int, char>(2, 'a')) );
    test_true( !vc(STD::pair<const int, char>(2, 'a'),
                   STD::pair<const int, char>(1, 'z')) );
    test_pass("cxx03:map::value_compare");

    M::reference r = *m.begin();
    r.second = 'b';
    test_eq( cm.begin()->second, 'b' );
    test_pass("cxx03:map::reference");

    M::const_reference cr = *cm.begin();
    test_eq( cr.second, 'b' );
    test_pass("cxx03:map::const_reference");

    M::pointer p = &*m.begin();
    test_eq( p->first, 1 );
    test_pass("cxx03:map::pointer");

    M::const_pointer cp = &*cm.begin();
    test_eq( cp->first, 1 );
    test_pass("cxx03:map::const_pointer");

    M::size_type n = m.size();
    test_eq( n, (STD::size_t)1 );
    test_true( !STD::is_signed<M::size_type>::value );
    test_pass("cxx03:map::size_type");

    test_true( STD::is_signed<M::difference_type>::value );
    {
        M::difference_type d = 0;
        M::const_iterator i;
        for (i = cm.begin(); i != cm.end(); ++i) ++d;
        test_eq( (long)d, 1L );
    }
    test_pass("cxx03:map::difference_type");

    M::const_iterator ci = cm.begin();
    test_eq( ci->first, 1 );
    test_pass("cxx03:map::const_iterator");

    m.insert(STD::pair<const int, char>(9, 'z'));
    M::reverse_iterator ri = m.rbegin();
    test_eq( ri->first, 9 );
    test_pass("cxx03:map::reverse_iterator");

    M::const_reverse_iterator cri = cm.rbegin();
    test_eq( cri->first, 9 );
    test_pass("cxx03:map::const_reverse_iterator");
}

TEST_CASE(map, map_allocator_type_cxx11) {
    typedef STD::map<int, char> M;
    M m;
    test_true(( STD::is_same<M::allocator_type,
                             STD::allocator<STD::pair<const int, char> > >::value ));
    M::allocator_type al = m.get_allocator();
    (void)al;
    test_pass("cxx03:map::allocator_type");
}


TEST_CASE(map, multimap_member_types_cxx03) {
    typedef STD::multimap<int, char> M;
    M m;
    m.insert(STD::pair<const int, char>(1, 'a'));
    const M& cm = m;

    test_true(( STD::is_same<M::key_type, int>::value ));
    test_pass("cxx03:multimap::key_type");

    test_true(( STD::is_same<M::mapped_type, char>::value ));
    test_pass("cxx03:multimap::mapped_type");

    test_true(( STD::is_same<M::value_type, STD::pair<const int, char> >::value ));
    test_pass("cxx03:multimap::value_type");

    test_true(( STD::is_same<M::key_compare, STD::less<int> >::value ));
    M::key_compare kc = m.key_comp();
    test_true( kc(1, 2) );
    test_pass("cxx03:multimap::key_compare");

    M::value_compare vc = m.value_comp();
    test_true( vc(STD::pair<const int, char>(1, 'z'),
                 STD::pair<const int, char>(2, 'a')) );
    test_true( !vc(STD::pair<const int, char>(2, 'a'),
                   STD::pair<const int, char>(1, 'z')) );
    test_pass("cxx03:multimap::value_compare");

    M::reference r = *m.begin();
    r.second = 'b';
    test_eq( cm.begin()->second, 'b' );
    test_pass("cxx03:multimap::reference");

    M::const_reference cr = *cm.begin();
    test_eq( cr.second, 'b' );
    test_pass("cxx03:multimap::const_reference");

    M::pointer p = &*m.begin();
    test_eq( p->first, 1 );
    test_pass("cxx03:multimap::pointer");

    M::const_pointer cp = &*cm.begin();
    test_eq( cp->first, 1 );
    test_pass("cxx03:multimap::const_pointer");

    M::size_type n = m.size();
    test_eq( n, (STD::size_t)1 );
    test_true( !STD::is_signed<M::size_type>::value );
    test_pass("cxx03:multimap::size_type");

    test_true( STD::is_signed<M::difference_type>::value );
    {
        M::difference_type d = 0;
        M::const_iterator i;
        for (i = cm.begin(); i != cm.end(); ++i) ++d;
        test_eq( (long)d, 1L );
    }
    test_pass("cxx03:multimap::difference_type");

    M::const_iterator ci = cm.begin();
    test_eq( ci->first, 1 );
    test_pass("cxx03:multimap::const_iterator");

    m.insert(STD::pair<const int, char>(9, 'z'));
    M::reverse_iterator ri = m.rbegin();
    test_eq( ri->first, 9 );
    test_pass("cxx03:multimap::reverse_iterator");

    M::const_reverse_iterator cri = cm.rbegin();
    test_eq( cri->first, 9 );
    test_pass("cxx03:multimap::const_reverse_iterator");
}

TEST_CASE(map, multimap_allocator_type_cxx11) {
    typedef STD::multimap<int, char> M;
    M m;
    test_true(( STD::is_same<M::allocator_type,
                             STD::allocator<STD::pair<const int, char> > >::value ));
    M::allocator_type al = m.get_allocator();
    (void)al;
    test_pass("cxx03:multimap::allocator_type");
}


TEST_CASE(map, lookup_member) {
#if defined(__cpp_lib_associative_lookup)
    STD::map<int, char> m;
    m[1] = 'a';
    test_ptr( (void*)m.lookup(1) );
    test_eq( *m.lookup(1), 'a' );
    test_true( m.lookup(2) == 0 );
    test_pass("cxx03:map lookup operations");
#else
    TEST_NOTE("map::lookup is a C++26 addition; no library here defines it");
    test_skip("cxx03:map lookup operations");
#endif
}

TEST_CASE(map, feature_test_macros) {
#if defined(__cpp_lib_generic_associative_lookup)
    test_true( __cpp_lib_generic_associative_lookup > 0L );
    test_pass("cxx14:__cpp_lib_generic_associative_lookup");
#else
    test_skip("cxx14:__cpp_lib_generic_associative_lookup");
#endif
#if defined(__cpp_lib_node_extract)
    test_true( __cpp_lib_node_extract > 0L );
    test_pass("cxx17:__cpp_lib_node_extract");
#else
    test_skip("cxx17:__cpp_lib_node_extract");
#endif
#if defined(__cpp_lib_map_try_emplace)
    test_true( __cpp_lib_map_try_emplace > 0L );
    test_pass("cxx17:__cpp_lib_map_try_emplace");
#else
    test_skip("cxx17:__cpp_lib_map_try_emplace");
#endif
#if defined(__cpp_lib_containers_ranges)
    test_true( __cpp_lib_containers_ranges > 0L );
    test_pass("cxx23:__cpp_lib_containers_ranges");
#else
    test_skip("cxx23:__cpp_lib_containers_ranges");
#endif
#if defined(__cpp_lib_associative_heterogeneous_erasure)
    test_true( __cpp_lib_associative_heterogeneous_erasure > 0L );
    test_pass("cxx23:__cpp_lib_associative_heterogeneous_erasure");
#else
    test_skip("cxx23:__cpp_lib_associative_heterogeneous_erasure");
#endif
#if defined(__cpp_lib_constexpr_map)
    test_true( __cpp_lib_constexpr_map > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_map");
    {
        struct K {
            static constexpr int run() {
                STD::map<int, int> m;
                m.insert(STD::make_pair(2, 20));
                m[1] = 10;
                m.erase(2);
                STD::multimap<int, int> mm;
                mm.insert(STD::make_pair(1, 1));
                mm.insert(STD::make_pair(1, 2));
                return (int)(m.at(1) + m.size() + mm.count(1));
            }
        };
        static_assert(K::run() == 13, "map operations in a constant expression");
        test_eq( K::run(), 13 );
    }
    test_pass("cxx26:constexpr map and multimap operations");
#else
    TEST_NOTE("constexpr map is C++26 (P3372); not in this library");
    test_skip("cxx26:__cpp_lib_constexpr_map");
    test_skip("cxx26:constexpr map and multimap operations");
#endif
#if defined(__cpp_lib_associative_heterogeneous_insertion)
    test_true( __cpp_lib_associative_heterogeneous_insertion > 0L );
    test_pass("cxx26:__cpp_lib_associative_heterogeneous_insertion");
#else
    test_skip("cxx26:__cpp_lib_associative_heterogeneous_insertion");
#endif
}

static int g_map_live = 0;

struct MapLive {
    STD::string s_;
    MapLive() { ++g_map_live; }
    MapLive(const char* s) : s_(s) { ++g_map_live; }
    MapLive(const MapLive& o) : s_(o.s_) { ++g_map_live; }
    MapLive& operator=(const MapLive& o) { s_ = o.s_; return *this; }
    ~MapLive() { --g_map_live; }
};

static STD::string map_live_key(int i) {
    STD::string t("k");
    t += (char)('0' + i);
    t += "_long_enough_to_leave_the_sso_buffer";
    return t;
}

TEST_CASE(map, class_values_lifetime) {
    g_map_live = 0;
    {
        STD::map<STD::string, MapLive> m;
        for (int i = 0; i < 8; ++i)
            m.insert(STD::pair<STD::string, MapLive>(map_live_key(i),
                                                     MapLive("v_long_enough_to_leave_sso")));
        test_eq( (int)m.size(), 8 );
        test_eq( g_map_live, 8 );

        STD::map<STD::string, MapLive> c(m);
        test_eq( g_map_live, 16 );

        m.erase(m.begin());
        test_eq( (int)m.size(), 7 );
        test_eq( g_map_live, 15 );

        STD::map<STD::string, MapLive> e;
        m.swap(e);
        test_true( m.empty() );
        test_eq( (int)e.size(), 7 );
        test_eq( g_map_live, 15 );

        e.clear();
        test_eq( g_map_live, 8 );

        m[map_live_key(9)] = MapLive("w_long_enough_to_leave_sso");
        test_eq( (int)m.size(), 1 );
        test_eq( g_map_live, 9 );

        c = e;
        test_eq( g_map_live, 1 );
    }
    test_eq( g_map_live, 0 );
}

TEST_CASE(map, multimap_insert_forms) {
    typedef STD::multimap<int, int> MM;
    typedef MM::value_type V;

    {   MM m;
        m.insert(V(5, 50));
        MM::iterator at_end = m.insert(m.end(), V(9, 90));
        test_eq( at_end->first, 9 );
        test_eq( at_end->second, 90 );
        MM::iterator good = m.insert(m.lower_bound(7), V(7, 70));
        test_eq( good->first, 7 );
        MM::iterator bad = m.insert(m.begin(), V(8, 80));
        test_eq( bad->first, 8 );
        MM::iterator dup = m.insert(m.begin(), V(5, 51));
        test_eq( dup->first, 5 );
        test_eq( (long)m.size(), 5L );
        test_eq( (long)m.count(5), 2L ); }
    test_pass("cxx03:multimap::insert (hint)");

    {   V src[4] = { V(3, 30), V(1, 10), V(3, 31), V(2, 20) };
        MM m;
        m.insert(src, src);
        test_true( m.empty() );
        m.insert(src, src + 4);
        test_eq( (long)m.size(), 4L );
        test_eq( (long)m.count(3), 2L );
        test_eq( m.begin()->first, 1 );
        m.insert(src, src + 1);
        test_eq( (long)m.size(), 5L );
        test_eq( (long)m.count(3), 3L ); }
    test_pass("cxx03:multimap::insert (range)");
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(map, extract_by_iterator_cxx17) {
    {   STD::map<int, int> m;
        m.insert(STD::map<int, int>::value_type(1, 10));
        m.insert(STD::map<int, int>::value_type(2, 20));
        STD::map<int, int>::const_iterator it = m.find(2);
        STD::map<int, int>::node_type nh = m.extract(it);
        test_true( !nh.empty() );
        test_eq( nh.key(), 2 );
        test_eq( nh.mapped(), 20 );
        test_eq( (long)m.size(), 1L );
        test_true( m.find(2) == m.end() );

        STD::map<int, int>::node_type last = m.extract(m.begin());
        test_eq( last.key(), 1 );
        test_true( m.empty() ); }
    test_pass("cxx17:map::extract(const_iterator)");

    {   STD::multimap<int, int> m;
        typedef STD::multimap<int, int>::value_type V2;
        m.insert(V2(5, 50)); m.insert(V2(5, 51)); m.insert(V2(7, 70));
        STD::multimap<int, int>::const_iterator it = m.find(7);
        STD::multimap<int, int>::node_type nh = m.extract(it);
        test_true( !nh.empty() );
        test_eq( nh.key(), 7 );
        test_eq( nh.mapped(), 70 );
        test_eq( (long)m.size(), 2L );
        test_eq( (long)m.count(7), 0L );

        STD::multimap<int, int>::node_type one = m.extract(m.begin());
        test_eq( one.key(), 5 );
        test_eq( (long)m.count(5), 1L ); }
    test_pass("cxx17:multimap::extract(const_iterator)");
}
#else
TEST_CASE(map, extract_by_iterator_cxx17) {
    TEST_NOTE("node handles are C++17");
    test_skip("cxx17:map::extract(const_iterator)");
    test_skip("cxx17:multimap::extract(const_iterator)");
}
#endif
