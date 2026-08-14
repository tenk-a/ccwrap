#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2023
#include <flat_map>
#include <functional>
#include <vector>
#include <memory>
#include <iterator>
#include <utility>
#include <string>
#include <type_traits>

namespace {
typedef STD::flat_map<int, int> FMap;
FMap::value_type fkv(int k, int v) { return FMap::value_type(k, v); }
long fkeysig(const FMap& m) {
    long r = 0;
    for (FMap::const_iterator it = m.begin(); it != m.end(); ++it) r = r * 10 + it->first;
    return r;
}
}

TEST_CASE(flat_map, construct_and_index) {
    FMap a;
    test_true( a.empty() );
    test_pass("cxx23:flat_map");

    a[3] = 30; a[1] = 10; a[2] = 20;
    test_eq( a.size(), 3u );
    test_eq( fkeysig(a), 123L );
    test_pass("cxx23:flat_map::operator[]");
    test_eq( a[2], 20 );
    a[2] = 99;
    test_eq( a[2], 99 );
    int fresh = a[7];
    test_eq( fresh, 0 );
    test_pass("cxx23:flat_map::operator[]_reassign");

    FMap::value_type raw[] = { fkv(5,50), fkv(4,40), fkv(6,60) };
    FMap d(raw, raw + 3);
    test_eq( fkeysig(d), 456L );
    test_pass("cxx23:flat_map::range_ctor");

    FMap c(a);
    test_eq( c.size(), a.size() );
    test_pass("cxx23:flat_map::copy_ctor");

    STD::flat_map<int, int, STD::greater<int> > g(raw, raw + 3);
    test_eq( g.begin()->first, 6 );
    test_pass("cxx23:flat_map::comparator_ctor");
}

TEST_CASE(flat_map, at_and_lookup) {
    FMap::value_type raw[] = { fkv(10,1), fkv(20,2), fkv(30,3) };
    FMap m(raw, raw + 3);
    const FMap& cm = m;

    test_eq( m.at(20), 2 );
    test_eq( cm.at(30), 3 );
    m.at(10) = 99;
    test_eq( m.at(10), 99 );
    test_throw( (void)m.at(99) );
    test_throw( (void)cm.at(99) );
    test_pass("cxx23:flat_map::at");

    test_true( m.find(20) != m.end() );
    test_eq( m.find(20)->second, 2 );
    test_true( m.find(25) == m.end() );
    test_pass("cxx23:flat_map::find");
    test_eq( m.count(20), 1u );
    test_eq( m.count(99), 0u );
    test_pass("cxx23:flat_map::count");
    test_true(  m.contains(30) );
    test_true( !m.contains(31) );
    test_pass("cxx23:flat_map::contains");

    test_eq( m.lower_bound(20)->first, 20 );
    test_pass("cxx23:flat_map::lower_bound");
    test_eq( m.upper_bound(20)->first, 30 );
    test_pass("cxx23:flat_map::upper_bound");
    STD::pair<FMap::iterator, FMap::iterator> eq = m.equal_range(20);
    test_eq( eq.first->first, 20 );
    test_eq( eq.second->first, 30 );
    test_pass("cxx23:flat_map::equal_range");
}

TEST_CASE(flat_map, insert_try_assign_erase) {
    FMap m;
    STD::pair<FMap::iterator, bool> r = m.insert(fkv(1, 10));
    test_true( r.second );
    test_eq( r.first->second, 10 );
    STD::pair<FMap::iterator, bool> r2 = m.insert(fkv(1, 99));
    test_true( !r2.second );
    test_eq( m[1], 10 );
    test_pass("cxx23:flat_map::insert");

    FMap::value_type more[] = { fkv(2, 20), fkv(3, 30) };
    m.insert(more, more + 2);
    test_eq( m.size(), 3u );
    test_pass("cxx23:flat_map::insert_range");

    STD::pair<FMap::iterator, bool> te = m.try_emplace(4, 40);
    test_true( te.second );
    test_eq( m[4], 40 );
    STD::pair<FMap::iterator, bool> te2 = m.try_emplace(4, 999);
    test_true( !te2.second );
    test_eq( m[4], 40 );
    test_pass("cxx23:flat_map::try_emplace");

    STD::pair<FMap::iterator, bool> io = m.insert_or_assign(4, 44);
    test_true( !io.second );
    test_eq( m[4], 44 );
    STD::pair<FMap::iterator, bool> io2 = m.insert_or_assign(5, 50);
    test_true( io2.second );
    test_eq( m[5], 50 );
    test_pass("cxx23:flat_map::insert_or_assign");

    FMap::size_type n = m.erase(3);
    test_eq( (long)n, 1L );
    test_eq( m.erase(99), 0u );
    test_pass("cxx23:flat_map::erase_key");
    FMap::iterator it = m.find(2);
    m.erase(it);
    test_true( m.find(2) == m.end() );
    test_pass("cxx23:flat_map::erase_iterator");

    m.clear();
    test_true( m.empty() );
    test_pass("cxx23:flat_map::clear");
}

TEST_CASE(flat_map, string_keys_and_values) {

    STD::flat_map<STD::string, STD::string> m;
    m["banana"] = "yellow";
    m["apple"]  = "red";
    m["cherry"] = "dark";
    test_eq( m.size(), 3u );
    test_eq( m.begin()->first, STD::string("apple") );
    test_pass("cxx23:flat_map::string_keys");
    test_eq( m["apple"], STD::string("red") );
    m["apple"] = "green";
    test_eq( m.at("apple"), STD::string("green") );
    test_throw( (void)m.at("missing") );
    test_pass("cxx23:flat_map::string_at");

    test_true( m.contains("banana") );
    STD::flat_map<STD::string, STD::string>::size_type n = m.erase("banana");
    test_eq( (long)n, 1L );
    test_eq( m.size(), 2u );
    test_pass("cxx23:flat_map::string_erase");

    STD::flat_map<int, STD::string> ms;
    ms[2] = STD::string(300, 'x');
    ms[1] = "one";
    test_eq( ms.begin()->first, 1 );
    test_eq( ms[2].size(), 300u );
    test_pass("cxx23:flat_map::string_values");
}

#if defined(__cpp_lib_flat_map)
namespace {
typedef STD::flat_multimap<int, int> FMMap;
FMMap::value_type mkv(int k, int v) { return FMMap::value_type(k, v); }

long mkeysig(const FMMap& m) {
    long r = 0;
    for (FMMap::const_iterator it = m.begin(); it != m.end(); ++it) r = r * 10 + it->first;
    return r;
}
long mvalsig(const FMMap& m) {
    long r = 0;
    for (FMMap::const_iterator it = m.begin(); it != m.end(); ++it) r = r * 10 + it->second;
    return r;
}
}

TEST_CASE(flat_map, multimap_insert_and_order) {
    FMMap m;
    test_true( m.empty() );
    test_eq( m.size(), 0u );
    test_pass("cxx23:flat_multimap");

    m.insert(mkv(3, 1));
    m.insert(mkv(1, 2));
    m.insert(mkv(3, 3));
    m.insert(mkv(2, 4));
    m.insert(mkv(3, 5));
    test_eq( m.size(), 5u );
    test_eq( mkeysig(m), 12333L );
    test_eq( mvalsig(m), 24135L );
    test_pass("cxx23:flat_multimap::insert");

    test_eq( m.count(3), 3u );
    test_eq( m.count(1), 1u );
    test_eq( m.count(9), 0u );
    test_pass("cxx23:flat_multimap::count");
    test_true( m.contains(2) );
    test_true( !m.contains(9) );
    test_pass("cxx23:flat_multimap::contains");

    FMMap::iterator ins = m.insert(mkv(2, 6));
    test_eq( ins->first, 2 );
    test_eq( ins->second, 6 );
    test_eq( m.count(2), 2u );
    test_pass("cxx23:flat_multimap::insert_returns_iterator");

    FMMap::iterator h = m.insert(m.begin(), mkv(4, 7));
    test_eq( h->first, 4 );
    test_pass("cxx23:flat_multimap::insert_hint");
    FMMap::iterator e = m.emplace(4, 8);
    test_eq( e->first, 4 );
    test_pass("cxx23:flat_multimap::emplace");
    FMMap::iterator eh = m.emplace_hint(m.end(), mkv(4, 9));
    test_eq( eh->first, 4 );
    test_eq( m.count(4), 3u );
    test_pass("cxx23:flat_multimap::emplace_hint");

    FMMap::iterator f = m.find(3);
    test_true( f != m.end() );
    test_eq( f->second, 1 );
    test_pass("cxx23:flat_multimap::find");
}

TEST_CASE(flat_map, multimap_lookup_and_erase) {
    FMMap::value_type raw[6] = { mkv(5, 1), mkv(1, 2), mkv(5, 3), mkv(3, 4), mkv(5, 5), mkv(1, 6) };
    FMMap m(raw, raw + 6);
    test_eq( m.size(), 6u );
    test_eq( mkeysig(m), 113555L );
    test_pass("cxx23:flat_multimap::range_ctor");

    STD::pair<FMMap::iterator, FMMap::iterator> r = m.equal_range(5);
    test_eq( (int)(r.second - r.first), 3 );
    test_true( m.lower_bound(5) == r.first );
    test_true( m.upper_bound(5) == r.second );
    test_pass("cxx23:flat_multimap::equal_range");

    STD::pair<FMMap::iterator, FMMap::iterator> none = m.equal_range(4);
    test_true( none.first == none.second );
    test_eq( none.first->first, 5 );
    test_pass("cxx23:flat_multimap::equal_range_empty");

    FMMap::size_type n = m.erase(5);
    test_eq( n, 3u );
    test_eq( m.count(5), 0u );
    test_eq( mkeysig(m), 113L );
    test_pass("cxx23:flat_multimap::erase_key");

    FMMap::iterator after = m.erase(m.find(1));
    test_eq( m.count(1), 1u );
    test_eq( after->first, 1 );
    test_pass("cxx23:flat_multimap::erase_iterator");

    FMMap m2(raw, raw + 6);
    STD::pair<FMMap::iterator, FMMap::iterator> run = m2.equal_range(5);
    FMMap::iterator tail = m2.erase(run.first, run.second);
    test_eq( m2.size(), 3u );
    test_true( tail == m2.end() );
    test_pass("cxx23:flat_multimap::erase_range");

    m2.clear();
    test_true( m2.empty() );
    test_pass("cxx23:flat_multimap::clear");
}

TEST_CASE(flat_map, multimap_construct_compare_swap) {

    FMMap::value_type sorted[4] = { mkv(1, 1), mkv(2, 2), mkv(2, 3), mkv(7, 4) };
    FMMap a(STD::sorted_equivalent, sorted, sorted + 4);
    test_eq( a.size(), 4u );
    test_eq( a.count(2), 2u );
    test_eq( mkeysig(a), 1227L );
    test_pass("cxx23:flat_multimap::sorted_equivalent_ctor");

    STD::flat_multimap<int, int, STD::greater<int> > g;
    g.insert(STD::flat_multimap<int, int, STD::greater<int> >::value_type(1, 0));
    g.insert(STD::flat_multimap<int, int, STD::greater<int> >::value_type(3, 0));
    g.insert(STD::flat_multimap<int, int, STD::greater<int> >::value_type(3, 0));
    g.insert(STD::flat_multimap<int, int, STD::greater<int> >::value_type(2, 0));
    test_eq( g.begin()->first, 3 );
    test_eq( g.count(3), 2u );
    test_pass("cxx23:flat_multimap::comparator");

    FMMap b(STD::sorted_equivalent, sorted, sorted + 4, STD::less<int>());
    test_true( a == b );
    test_pass("cxx23:flat_multimap::operator==");

    FMMap c(STD::sorted_equivalent, sorted, sorted + 3);
    test_true( a != c );
    test_pass("cxx23:flat_multimap operator!= (rewritten from operator==)");

    FMMap x, y;
    x.insert(mkv(9, 9));
    y.insert(mkv(1, 1)); y.insert(mkv(1, 2));
    x.swap(y);
    test_eq( x.count(1), 2u );
    test_eq( y.begin()->first, 9 );
    test_pass("cxx23:flat_multimap::swap");
    STD::swap(x, y);
    test_eq( x.begin()->first, 9 );
    test_eq( y.count(1), 2u );
    test_pass("cxx23:flat_multimap::swap_free");

    FMMap::const_reverse_iterator ri = a.rbegin();
    test_eq( ri->first, 7 );
    long back = 0;
    for (; ri != a.rend(); ++ri) back = back * 10 + ri->first;
    test_eq( back, 7221L );
    test_pass("cxx23:flat_multimap::reverse_iterator");

    test_true( (STD::is_same<FMMap::key_type, int>::value) );
    test_true( (STD::is_same<FMMap::mapped_type, int>::value) );
    test_true( (STD::is_same<FMMap::value_type, STD::pair<int, int> >::value) );
    test_true( (STD::is_same<FMMap::key_compare, STD::less<int> >::value) );
    test_pass("cxx23:flat_multimap::member_types");

    test_true( !(STD::is_same<STD::sorted_equivalent_t, STD::sorted_unique_t>::value) );
    test_pass("cxx23:sorted_equivalent_t");
}
#else
TEST_CASE_SKIP(flat_map, multimap_insert_and_order)
TEST_CASE_SKIP(flat_map, multimap_lookup_and_erase)
TEST_CASE_SKIP(flat_map, multimap_construct_compare_swap)
#endif


TEST_CASE(flat_map, insert_forms_cxx23) {
    typedef STD::flat_map<int, int> M;
    typedef M::value_type V;
    typedef STD::flat_multimap<int, int> MM;
    typedef MM::value_type MV;

    {   M m;
        m.insert(V(5, 50));
        M::iterator a = m.insert(m.end(), V(9, 90));
        test_eq( a->first, 9 );
        test_eq( a->second, 90 );
        M::iterator b = m.insert(m.begin(), V(1, 10));
        test_eq( b->first, 1 );
        M::iterator dup = m.insert(m.begin(), V(5, 51));
        test_eq( dup->first, 5 );
        test_eq( dup->second, 50 );
        test_eq( (long)m.size(), 3L ); }
    test_pass("cxx23:flat_map::insert_hint");

    {   V src[4] = { V(3, 30), V(1, 10), V(3, 31), V(2, 20) };
        M m;
        m.insert(src, src);
        test_true( m.empty() );
        m.insert(src, src + 4);
        test_eq( (long)m.size(), 3L );
        test_eq( m.begin()->first, 1 );
        test_eq( m.at(3), 30 );
        m.insert(src, src + 1);
        test_eq( (long)m.size(), 3L ); }
    test_pass("cxx23:flat_map::insert_iterator_pair");

    {   MV src[4] = { MV(3, 30), MV(1, 10), MV(3, 31), MV(2, 20) };
        MM m;
        m.insert(src, src);
        test_true( m.empty() );
        m.insert(src, src + 4);
        test_eq( (long)m.size(), 4L );
        test_eq( (long)m.count(3), 2L );
        test_eq( m.begin()->first, 1 );
        m.insert(src, src + 1);
        test_eq( (long)m.count(3), 3L ); }
    test_pass("cxx23:flat_multimap::insert_iterator_pair");
}

#else
TEST_CASE_SKIP(flat_map, unimplemented)
#endif

#if defined(__cpp_lib_flat_map)
namespace {
struct FmOddKey {
    template <class P> bool operator()(const P& p) const { return p.first % 2 != 0; }
};
}

TEST_CASE(flat_map, member_types_cxx23) {
    typedef STD::flat_map<int, char> FM;

    test_true(( STD::is_same<FM::key_type, int>::value ));
    test_pass("cxx23:flat_map::key_type");
    test_true(( STD::is_same<FM::mapped_type, char>::value ));
    test_pass("cxx23:flat_map::mapped_type");
    test_true(( STD::is_same<FM::value_type, STD::pair<int, char> >::value ));
    test_pass("cxx23:flat_map::value_type");
    test_true(( STD::is_same<FM::key_compare, STD::less<int> >::value ));
    test_pass("cxx23:flat_map::key_compare");
    test_true(( STD::is_same<FM::key_container_type, STD::vector<int> >::value ));
    test_pass("cxx23:flat_map::key_container_type");
    test_true(( STD::is_same<FM::mapped_container_type, STD::vector<char> >::value ));
    test_pass("cxx23:flat_map::mapped_container_type");

    test_true(( STD::is_same<FM::reference, STD::pair<const int&, char&> >::value ));
    test_pass("cxx23:flat_map::reference");
    test_true(( STD::is_same<FM::const_reference, STD::pair<const int&, const char&> >::value ));
    test_pass("cxx23:flat_map::const_reference");

    test_true( STD::is_signed<FM::difference_type>::value );
    test_pass("cxx23:flat_map::difference_type");
    test_true( !STD::is_signed<FM::size_type>::value );
    test_pass("cxx23:flat_map::size_type");

    FM m;
    m.insert(FM::value_type(1, 'a'));
    test_true(( STD::is_same<decltype(m.begin()), FM::iterator>::value ));
    test_pass("cxx23:flat_map::iterator");
    const FM& cm = m;
    test_true(( STD::is_same<decltype(cm.begin()), FM::const_iterator>::value ));
    test_pass("cxx23:flat_map::const_iterator");
    test_true(( STD::is_same<FM::reverse_iterator,
                             STD::reverse_iterator<FM::iterator> >::value ));
    test_pass("cxx23:flat_map::reverse_iterator");
    test_true(( STD::is_same<FM::const_reverse_iterator,
                             STD::reverse_iterator<FM::const_iterator> >::value ));
    test_pass("cxx23:flat_map::const_reverse_iterator");

    FM::value_compare vc = m.value_comp();
    test_true( vc(FM::value_type(1, 'z'), FM::value_type(2, 'a')) );
    test_true( !vc(FM::value_type(2, 'a'), FM::value_type(1, 'z')) );
    test_pass("cxx23:flat_map::value_compare");

    typedef STD::flat_multimap<int, char> FMM;
    test_true(( STD::is_same<FMM::key_type, int>::value ));
    test_pass("cxx23:flat_multimap::key_type");
    test_true(( STD::is_same<FMM::mapped_type, char>::value ));
    test_pass("cxx23:flat_multimap::mapped_type");
    test_true(( STD::is_same<FMM::value_type, STD::pair<int, char> >::value ));
    test_pass("cxx23:flat_multimap::value_type");
    test_true(( STD::is_same<FMM::key_compare, STD::less<int> >::value ));
    test_pass("cxx23:flat_multimap::key_compare");
    test_true(( STD::is_same<FMM::key_container_type, STD::vector<int> >::value ));
    test_pass("cxx23:flat_multimap::key_container_type");
    test_true(( STD::is_same<FMM::mapped_container_type, STD::vector<char> >::value ));
    test_pass("cxx23:flat_multimap::mapped_container_type");
    test_true(( STD::is_same<FMM::reference, STD::pair<const int&, char&> >::value ));
    test_pass("cxx23:flat_multimap::reference");
    test_true(( STD::is_same<FMM::const_reference,
                             STD::pair<const int&, const char&> >::value ));
    test_pass("cxx23:flat_multimap::const_reference");
    test_true( STD::is_signed<FMM::difference_type>::value );
    test_pass("cxx23:flat_multimap::difference_type");
    test_true( !STD::is_signed<FMM::size_type>::value );
    test_pass("cxx23:flat_multimap::size_type");
    FMM mm;
    mm.insert(FMM::value_type(1, 'a'));
    test_true(( STD::is_same<decltype(mm.begin()), FMM::iterator>::value ));
    test_pass("cxx23:flat_multimap::iterator");
    const FMM& cmm = mm;
    test_true(( STD::is_same<decltype(cmm.begin()), FMM::const_iterator>::value ));
    test_pass("cxx23:flat_multimap::const_iterator");
    test_true(( STD::is_same<FMM::const_reverse_iterator,
                             STD::reverse_iterator<FMM::const_iterator> >::value ));
    test_pass("cxx23:flat_multimap::const_reverse_iterator");
    FMM::value_compare mvc = mm.value_comp();
    test_true( mvc(FMM::value_type(1, 'z'), FMM::value_type(2, 'a')) );
    test_pass("cxx23:flat_multimap::value_compare");

    test_true(( STD::uses_allocator<STD::flat_map<int, int, STD::less<int>,
                     STD::vector<int>, STD::vector<int> >,
                 STD::allocator<int> >::value ));
    test_pass("cxx23:uses_allocator");
}

TEST_CASE(flat_map, iteration_and_size_cxx23) {
    STD::flat_map<int, int> m;
    m.insert(fkv(3, 30));
    m.insert(fkv(1, 10));
    m.insert(fkv(2, 20));

    test_eq( m.begin()->first, 1 );
    test_pass("cxx23:flat_map::begin");
    test_eq( (long)(m.end() - m.begin()), 3L );
    test_pass("cxx23:flat_map::end");
    test_true( m.cbegin() == m.begin() );
    test_eq( m.cbegin()->second, 10 );
    test_pass("cxx23:flat_map::cbegin");
    test_true( m.cend() == m.end() );
    test_pass("cxx23:flat_map::cend");
    test_eq( m.rbegin()->first, 3 );
    test_pass("cxx23:flat_map::rbegin");
    test_eq( (long)(m.rend() - m.rbegin()), 3L );
    test_pass("cxx23:flat_map::rend");
    test_true( m.crbegin() == m.rbegin() );
    test_pass("cxx23:flat_map::crbegin");
    test_true( m.crend() == m.rend() );
    test_pass("cxx23:flat_map::crend");

    test_eq( m.size(), (STD::size_t)3 );
    m.insert(fkv(1, 99));
    test_eq( m.size(), (STD::size_t)3 );
    test_eq( m.at(1), 10 );
    test_pass("cxx23:flat_map::size");
    test_true( !m.empty() );
    STD::flat_map<int, int> none;
    test_true( none.empty() );
    test_true( none.begin() == none.end() );
    test_pass("cxx23:flat_map::empty");
    test_true( m.max_size() >= m.size() && m.max_size() > 0 );
    test_pass("cxx23:flat_map::max_size");

    STD::flat_multimap<int, int> mm;
    mm.insert(FMMap::value_type(1, 10));
    mm.insert(FMMap::value_type(1, 11));
    test_eq( mm.crbegin()->first, 1 );
    test_pass("cxx23:flat_multimap::crbegin");
    test_true( mm.crend() == mm.rend() );
    test_eq( (long)(mm.crend() - mm.crbegin()), 2L );
    test_pass("cxx23:flat_multimap::crend");
}

TEST_CASE(flat_map, containers_and_construction_cxx23) {
    typedef STD::flat_map<int, int> FM;

    STD::vector<int> keys, vals;
    keys.push_back(3); keys.push_back(1); keys.push_back(2);
    vals.push_back(30); vals.push_back(10); vals.push_back(20);
    FM built(keys, vals);
    test_eq( built.size(), (STD::size_t)3 );
    test_eq( built.at(1), 10 );
    test_eq( built.at(3), 30 );
    test_pass("cxx23:flat_map::flat_map(key_cont, mapped_cont)");

    STD::vector<int> sk, sv;
    sk.push_back(1); sk.push_back(2);
    sv.push_back(10); sv.push_back(20);
    FM presorted(STD::sorted_unique, sk, sv);
    test_eq( presorted.at(2), 20 );
    test_pass("cxx23:flat_map::flat_map(sorted_unique_t, ...)");
    STD::sorted_unique_t tag = STD::sorted_unique;
    (void)tag;
    test_true( !(STD::is_same<STD::sorted_unique_t, STD::sorted_equivalent_t>::value) );
    test_pass("cxx23:sorted_unique_t");

    FM il = { { 2, 20 }, { 1, 10 } };
    test_eq( il.begin()->first, 1 );
    test_pass("cxx23:flat_map::flat_map(initializer_list)");

    STD::vector<STD::pair<int, int> > pairs;
    pairs.push_back(STD::pair<int, int>(2, 20));
    pairs.push_back(STD::pair<int, int>(1, 10));
    FM fromr(STD::from_range, pairs);
    test_eq( fromr.at(1), 10 );
    test_pass("cxx23:flat_map::flat_map(from_range_t, R&&)");

    test_eq( built.keys().size(), (STD::size_t)3 );
    test_eq( built.keys()[0], 1 );
    test_pass("cxx23:flat_map::keys");
    test_eq( built.values().size(), built.keys().size() );
    test_eq( built.values()[0], 10 );
    test_pass("cxx23:flat_map::values");

    FM movable = { { 1, 10 }, { 2, 20 } };
    FM::containers taken = STD::move(movable).extract();
    test_eq( taken.keys.size(), (STD::size_t)2 );
    test_eq( taken.values.size(), (STD::size_t)2 );
    test_true( movable.empty() );
    test_pass("cxx23:flat_map::extract");
    test_pass("cxx23:flat_map::containers");

    movable.replace(STD::move(taken.keys), STD::move(taken.values));
    test_eq( movable.size(), (STD::size_t)2 );
    test_eq( movable.at(2), 20 );
    test_pass("cxx23:flat_map::replace");

    STD::flat_multimap<int, int> mm = { { 1, 10 }, { 1, 11 } };
    STD::flat_multimap<int, int>::containers mtaken = STD::move(mm).extract();
    test_eq( mtaken.keys.size(), (STD::size_t)2 );
    test_pass("cxx23:flat_multimap::containers");

    STD::flat_map<int, int, STD::greater<int> > desc = { { 1, 1 }, { 2, 2 } };
    test_true( desc.key_comp()(2, 1) );
    test_eq( desc.begin()->first, 2 );
    test_pass("cxx23:flat_map::key_comp");
    test_true( desc.value_comp()(STD::pair<int, int>(2, 0), STD::pair<int, int>(1, 0)) );
    test_pass("cxx23:flat_map::value_comp");
    test_true( desc.key_comp()(2, 1) && !desc.key_comp()(1, 2) );
    test_pass("cxx23:flat_map::compare");

    STD::flat_multimap<int, int, STD::greater<int> > mdesc = { { 1, 1 }, { 2, 2 } };
    test_true( mdesc.key_comp()(2, 1) );
    test_eq( mdesc.begin()->first, 2 );
    test_pass("cxx23:flat_multimap::compare");
}

TEST_CASE(flat_map, modifiers_cxx23) {
    typedef STD::flat_map<int, int> FM;
    FM m = { { 1, 10 }, { 2, 20 }, { 3, 30 } };

    STD::pair<FM::iterator, bool> em = m.emplace(4, 40);
    test_true( em.second );
    test_eq( em.first->second, 40 );
    STD::pair<FM::iterator, bool> dup = m.emplace(4, 99);
    test_true( !dup.second );
    test_eq( m.at(4), 40 );
    test_pass("cxx23:flat_map::emplace");

    FM::iterator hinted = m.emplace_hint(m.end(), 5, 50);
    test_eq( hinted->first, 5 );
    test_eq( m.size(), (STD::size_t)5 );
    test_pass("cxx23:flat_map::emplace_hint");

    FM::iterator after = m.erase(m.begin());
    test_eq( after->first, 2 );
    long gone = (long)m.erase(3);
    test_eq( gone, 1L );
    long missing = (long)m.erase(99);
    test_eq( missing, 0L );
    test_pass("cxx23:flat_map::erase");

    FM r = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 } };
    FM::iterator it = r.begin();
    ++it;
    FM::iterator stop = it;
    ++stop; ++stop;
    r.erase(it, stop);
    test_eq( r.size(), (STD::size_t)2 );
    test_eq( r.begin()->first, 1 );
    test_pass("cxx23:flat_map::erase_range");

    FM a = { { 1, 1 } }, b = { { 8, 8 }, { 9, 9 } };
    a.swap(b);
    test_eq( a.size(), (STD::size_t)2 );
    test_eq( b.size(), (STD::size_t)1 );
    test_pass("cxx23:flat_map::swap");
    swap(a, b);
    test_eq( a.size(), (STD::size_t)1 );
    test_pass("cxx23:swap(flat_map)");

    FM e = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
    long odd = (long)STD::erase_if(e, FmOddKey());
    test_eq( odd, 2L );
    test_eq( e.size(), (STD::size_t)1 );
    test_eq( e.begin()->first, 2 );
    test_pass("cxx23:erase_if(flat_map)");

    STD::flat_multimap<int, int> mm = { { 1, 10 }, { 1, 11 }, { 2, 20 } };
    long mgone = (long)mm.erase(1);
    test_eq( mgone, 2L );
    test_eq( mm.size(), (STD::size_t)1 );
    test_pass("cxx23:flat_multimap::erase");
}

TEST_CASE(flat_map, comparison_and_lookup_cxx23) {
    STD::flat_map<int, int> a = { { 1, 1 }, { 2, 2 } };
    STD::flat_map<int, int> b = { { 1, 1 }, { 2, 2 } };
    STD::flat_map<int, int> c = { { 1, 1 }, { 2, 3 } };
    STD::flat_map<int, int> shorter = { { 1, 1 } };

    test_true( a == b );
    test_true( !(a == c) );
    test_true( !(a == shorter) );
    test_pass("cxx23:operator==(flat_map)");

    test_true( (a <=> c) < 0 );
    test_true( (c <=> a) > 0 );
    test_true( (a <=> b) == 0 );
    test_true( (shorter <=> a) < 0 );
    test_pass("cxx23:operator<=>(flat_map)");

    STD::flat_map<STD::string, int, STD::less<> > tr;
    tr.insert(STD::pair<STD::string, int>(STD::string("abc"), 1));
    test_true( tr.find("abc") != tr.end() );
    test_eq( (long)tr.count("abc"), 1L );
    test_true( tr.contains("abc") );
}

TEST_CASE(flat_map, constexpr_cxx26) {
#if defined(__cpp_lib_constexpr_flat_map)
    {
        struct K {
            static constexpr int run() {
                STD::flat_map<int, int> m;
                m.insert(STD::pair<int, int>(2, 20));
                m.insert(STD::pair<int, int>(1, 10));
                STD::flat_multimap<int, int> mm;
                mm.insert(STD::pair<int, int>(5, 1));
                mm.insert(STD::pair<int, int>(5, 2));
                return (int)(m.size() * 10 + mm.size());
            }
        };
        static_assert(K::run() == 22, "flat_map in a constant expression");
        test_eq( K::run(), 22 );
    }
    test_pass("cxx26:constexpr flat_map and flat_multimap");
#else
    TEST_NOTE("constexpr flat_map is C++26 (P3372); not in this library");
    test_skip("cxx26:constexpr flat_map and flat_multimap");
#endif
}
#else
TEST_CASE(flat_map, member_types_cxx23) {
    test_skip("cxx23:flat_map::key_type");
    test_skip("cxx23:flat_map::mapped_type");
    test_skip("cxx23:flat_map::value_type");
    test_skip("cxx23:flat_map::key_compare");
    test_skip("cxx23:flat_map::key_container_type");
    test_skip("cxx23:flat_map::mapped_container_type");
    test_skip("cxx23:flat_map::reference");
    test_skip("cxx23:flat_map::const_reference");
    test_skip("cxx23:flat_map::difference_type");
    test_skip("cxx23:flat_map::size_type");
    test_skip("cxx23:flat_map::iterator");
    test_skip("cxx23:flat_map::const_iterator");
    test_skip("cxx23:flat_map::reverse_iterator");
    test_skip("cxx23:flat_map::const_reverse_iterator");
    test_skip("cxx23:flat_map::value_compare");
    test_skip("cxx23:flat_multimap::key_type");
    test_skip("cxx23:flat_multimap::mapped_type");
    test_skip("cxx23:flat_multimap::value_type");
    test_skip("cxx23:flat_multimap::key_compare");
    test_skip("cxx23:flat_multimap::key_container_type");
    test_skip("cxx23:flat_multimap::mapped_container_type");
    test_skip("cxx23:flat_multimap::reference");
    test_skip("cxx23:flat_multimap::const_reference");
    test_skip("cxx23:flat_multimap::difference_type");
    test_skip("cxx23:flat_multimap::size_type");
    test_skip("cxx23:flat_multimap::iterator");
    test_skip("cxx23:flat_multimap::const_iterator");
    test_skip("cxx23:flat_multimap::const_reverse_iterator");
    test_skip("cxx23:flat_multimap::value_compare");
    test_skip("cxx23:uses_allocator");
}
TEST_CASE(flat_map, iteration_and_size_cxx23) {
    test_skip("cxx23:flat_map::begin");
    test_skip("cxx23:flat_map::end");
    test_skip("cxx23:flat_map::cbegin");
    test_skip("cxx23:flat_map::cend");
    test_skip("cxx23:flat_map::rbegin");
    test_skip("cxx23:flat_map::rend");
    test_skip("cxx23:flat_map::crbegin");
    test_skip("cxx23:flat_map::crend");
    test_skip("cxx23:flat_map::size");
    test_skip("cxx23:flat_map::empty");
    test_skip("cxx23:flat_map::max_size");
    test_skip("cxx23:flat_multimap::crbegin");
    test_skip("cxx23:flat_multimap::crend");
}
TEST_CASE(flat_map, containers_and_construction_cxx23) {
    test_skip("cxx23:flat_map::flat_map(key_cont, mapped_cont)");
    test_skip("cxx23:flat_map::flat_map(sorted_unique_t, ...)");
    test_skip("cxx23:sorted_unique_t");
    test_skip("cxx23:flat_map::flat_map(initializer_list)");
    test_skip("cxx23:flat_map::flat_map(from_range_t, R&&)");
    test_skip("cxx23:flat_map::keys");
    test_skip("cxx23:flat_map::values");
    test_skip("cxx23:flat_map::extract");
    test_skip("cxx23:flat_map::containers");
    test_skip("cxx23:flat_map::replace");
    test_skip("cxx23:flat_multimap::containers");
    test_skip("cxx23:flat_map::key_comp");
    test_skip("cxx23:flat_map::value_comp");
    test_skip("cxx23:flat_map::compare");
    test_skip("cxx23:flat_multimap::compare");
}
TEST_CASE(flat_map, modifiers_cxx23) {
    test_skip("cxx23:flat_map::emplace");
    test_skip("cxx23:flat_map::emplace_hint");
    test_skip("cxx23:flat_map::erase");
    test_skip("cxx23:flat_map::erase_range");
    test_skip("cxx23:flat_map::swap");
    test_skip("cxx23:swap(flat_map)");
    test_skip("cxx23:erase_if(flat_map)");
    test_skip("cxx23:flat_multimap::erase");
}
TEST_CASE(flat_map, comparison_and_lookup_cxx23) {
    test_skip("cxx23:operator==(flat_map)");
    test_skip("cxx23:operator<=>(flat_map)");
}
TEST_CASE(flat_map, constexpr_cxx26) {
    test_skip("cxx26:constexpr flat_map and flat_multimap");
}
#endif

TEST_CASE(flat_map, feature_test_macros) {
#if defined(__cpp_lib_flat_map)
    test_true( __cpp_lib_flat_map > 0L );
    test_pass("cxx23:__cpp_lib_flat_map");
#else
    test_skip("cxx23:__cpp_lib_flat_map");
#endif
#if defined(__cpp_lib_constexpr_flat_map)
    test_true( __cpp_lib_constexpr_flat_map > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_flat_map");
#else
    test_skip("cxx26:__cpp_lib_constexpr_flat_map");
#endif
#if defined(__cpp_lib_flat_map)
    test_true( __cpp_lib_flat_map > 0L );
    test_pass("cxx26:__cpp_lib_flat_map");
#else
    test_skip("cxx26:__cpp_lib_flat_map");
#endif
}
