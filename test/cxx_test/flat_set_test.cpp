#include "test_cxx.hpp"
#if TEST_TARGET_CXX >= 2023
#include <flat_set>
#include <type_traits>
#include <iterator>
#include <memory>
#include <functional>
#include <vector>
#include <functional>
#include <utility>
#include <string>

namespace {
struct IsOdd { bool operator()(int x) const { return x % 2 != 0; } };
long fsig(const STD::flat_set<int>& s) {
    long r = 0;
    for (STD::flat_set<int>::const_iterator it = s.begin(); it != s.end(); ++it) r = r * 10 + *it;
    return r;
}
}

TEST_CASE(flat_set, construct_insert) {
    STD::flat_set<int> a;
    test_true( a.empty() );
    test_eq( a.size(), 0u );
    test_pass("cxx23:flat_set");

    int raw[] = { 3, 1, 2, 1, 3 };
    STD::flat_set<int> b(raw, raw + 5);
    test_eq( b.size(), 3u );
    test_eq( fsig(b), 123L );
    test_pass("cxx23:flat_set::range_ctor");

    STD::pair<STD::flat_set<int>::iterator, bool> r = a.insert(5);
    test_true( r.second );
    test_eq( *r.first, 5 );
    STD::pair<STD::flat_set<int>::iterator, bool> r2 = a.insert(5);
    test_true( !r2.second );
    test_eq( a.size(), 1u );

    a.insert(1);
    a.insert(3);
    test_eq( fsig(a), 135L );
    test_pass("cxx23:flat_set::insert");

    STD::flat_set<int> c(b);
    test_eq( fsig(c), 123L );
    test_pass("cxx23:flat_set::copy_ctor");
    STD::flat_set<int> d;
    d = b;
    test_eq( fsig(d), 123L );
    test_pass("cxx23:flat_set::copy_assign");
}

TEST_CASE(flat_set, lookup) {
    int raw[] = { 10, 20, 30, 40 };
    STD::flat_set<int> s(raw, raw + 4);

    test_true( s.find(20) != s.end() );
    test_true( s.find(25) == s.end() );
    test_pass("cxx23:flat_set::find");
    test_eq( s.count(20), 1u );
    test_eq( s.count(99), 0u );
    test_pass("cxx23:flat_set::count");
    test_true( s.contains(30) );
    test_true( !s.contains(31) );
    test_pass("cxx23:flat_set::contains");

    test_eq( *s.lower_bound(20), 20 );
    test_pass("cxx23:flat_set::lower_bound");
    test_eq( *s.upper_bound(20), 30 );
    test_pass("cxx23:flat_set::upper_bound");
    test_eq( *s.lower_bound(25), 30 );

    STD::pair<STD::flat_set<int>::iterator, STD::flat_set<int>::iterator> eq = s.equal_range(30);
    test_eq( *eq.first, 30 );
    test_eq( *eq.second, 40 );
    test_pass("cxx23:flat_set::equal_range");
}

TEST_CASE(flat_set, erase_iterate) {
    int raw[] = { 1, 2, 3, 4, 5 };
    STD::flat_set<int> s(raw, raw + 5);

    STD::flat_set<int>::size_type n = s.erase(3);
    test_eq( (long)n, 1L );
    test_eq( s.erase(99), 0u );
    test_eq( fsig(s), 1245L );
    test_pass("cxx23:flat_set::erase_key");

    STD::flat_set<int>::iterator it = s.find(2);
    s.erase(it);
    test_eq( fsig(s), 145L );
    test_pass("cxx23:flat_set::erase_iterator");

    long rev = 0;
    for (STD::flat_set<int>::reverse_iterator r = s.rbegin(); r != s.rend(); ++r) rev = rev * 10 + *r;
    test_eq( rev, 541L );
    test_pass("cxx23:flat_set::reverse_iterator");

    s.clear();
    test_true( s.empty() );
    test_pass("cxx23:flat_set::clear");
}

TEST_CASE(flat_set, string_elements) {

    STD::flat_set<STD::string> s;
    s.insert("banana");
    s.insert("apple");
    s.insert("cherry");
    s.insert("apple");
    test_eq( s.size(), 3u );
    test_pass("cxx23:flat_set::string_insert");

    STD::flat_set<STD::string>::iterator it = s.begin();
    test_eq( *it, STD::string("apple") );  ++it;
    test_eq( *it, STD::string("banana") ); ++it;
    test_eq( *it, STD::string("cherry") );
    test_pass("cxx23:flat_set::string_order");

    test_true( s.contains("banana") );
    test_true( !s.contains("durian") );
    test_eq( *s.lower_bound("b"), STD::string("banana") );
    test_pass("cxx23:flat_set::string_lookup");

    STD::flat_set<STD::string>::size_type n = s.erase("apple");
    test_eq( (long)n, 1L );
    test_eq( s.size(), 2u );
    test_pass("cxx23:flat_set::string_erase");

    STD::flat_set<STD::string> big;
    big.insert(STD::string(300, 'b'));
    big.insert(STD::string(300, 'a'));
    test_eq( big.size(), 2u );
    test_eq( big.begin()->size(), 300u );
    test_pass("cxx23:flat_set::string_long");
}

#if defined(__cpp_lib_flat_set)
TEST_CASE(flat_set, comparator_and_emplace) {
    int raw[] = { 3, 1, 2 };
    STD::flat_set<int, STD::greater<int> > g(raw, raw + 3);
    test_eq( *g.begin(), 3 );
    STD::flat_set<int, STD::greater<int> >::iterator gi = g.begin();
    ++gi;
    test_eq( *gi, 2 );
    test_pass("cxx23:flat_set::comparator");

#if TEST_TARGET_CXX >= 2011
    STD::flat_set<int> s;
    STD::pair<STD::flat_set<int>::iterator, bool> e = s.emplace(7);
    test_true( e.second );
    test_eq( *e.first, 7 );
    STD::pair<STD::flat_set<int>::iterator, bool> e2 = s.emplace(7);
    test_true( !e2.second );
    test_eq( s.size(), 1u );
    test_pass("cxx23:flat_set::emplace");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx23:flat_set::emplace");
#endif
}

TEST_CASE(flat_set, multiset_basics) {
    STD::flat_multiset<int> m;
    test_true( m.empty() );
    test_pass("cxx23:flat_multiset");

    m.insert(3);
    m.insert(1);
    m.insert(3);
    m.insert(2);
    m.insert(3);
    test_eq( (int)m.size(), 5 );
    test_eq( (int)m.count(3), 3 );
    test_eq( (int)m.count(1), 1 );
    test_eq( (int)m.count(9), 0 );
    test_pass("cxx23:flat_multiset::insert");
    test_true( m.contains(2) );
    test_true( !m.contains(9) );
    test_pass("cxx23:flat_multiset::contains");

    int expect[5] = { 1, 2, 3, 3, 3 };
    int i = 0;
    for (STD::flat_multiset<int>::const_iterator it = m.begin(); it != m.end(); ++it, ++i)
        test_eq( *it, expect[i] );
    test_eq( i, 5 );
    test_pass("cxx23:flat_multiset::ordered");

    STD::flat_multiset<int>::iterator ins = m.insert(2);
    test_eq( *ins, 2 );
    test_eq( (int)m.count(2), 2 );
    test_pass("cxx23:flat_multiset::insert_returns_iterator");

    STD::pair<STD::flat_multiset<int>::iterator, STD::flat_multiset<int>::iterator> r
        = m.equal_range(3);
    test_eq( (int)(r.second - r.first), 3 );
    test_true( m.lower_bound(3) == r.first );
    test_true( m.upper_bound(3) == r.second );
    test_pass("cxx23:flat_multiset::equal_range");

    int nerased = (int)m.erase(3);
    test_eq( nerased, 3 );
    test_eq( (int)m.count(3), 0 );
    test_eq( (int)m.size(), 3 );
    test_pass("cxx23:flat_multiset::erase_key");

    STD::flat_multiset<int>::iterator e = m.erase(m.find(2));
    test_eq( (int)m.count(2), 1 );
    test_true( e != m.end() );
    test_pass("cxx23:flat_multiset::erase_iterator");

    m.clear();
    test_true( m.empty() );
    test_pass("cxx23:flat_multiset::clear");
}

TEST_CASE(flat_set, multiset_construction) {

    STD::vector<int> v;
    v.push_back(5); v.push_back(1); v.push_back(5); v.push_back(3);
    STD::flat_multiset<int> a(v);
    test_eq( (int)a.size(), 4 );
    test_eq( *a.begin(), 1 );
    test_eq( (int)a.count(5), 2 );
    test_pass("cxx23:flat_multiset::container_ctor");

    STD::vector<int> sorted;
    sorted.push_back(1); sorted.push_back(2); sorted.push_back(2); sorted.push_back(7);
    STD::flat_multiset<int> b(STD::sorted_equivalent, sorted);
    test_eq( (int)b.size(), 4 );
    test_eq( (int)b.count(2), 2 );
    test_eq( *b.begin(), 1 );
    test_pass("cxx23:flat_multiset::sorted_equivalent_ctor");

    int raw[5] = { 4, 4, 2, 9, 2 };
    STD::flat_multiset<int> c(raw, raw + 5);
    test_eq( (int)c.size(), 5 );
    test_eq( (int)c.count(4), 2 );
    test_eq( (int)c.count(2), 2 );
    test_eq( *c.begin(), 2 );
    test_pass("cxx23:flat_multiset::range_ctor");

    int sraw[4] = { 1, 1, 5, 8 };
    STD::flat_multiset<int> d(STD::sorted_equivalent, sraw, sraw + 4);
    test_eq( (int)d.size(), 4 );
    test_eq( (int)d.count(1), 2 );
    test_pass("cxx23:flat_multiset::sorted_equivalent_range_ctor");

    STD::flat_multiset<int, STD::greater<int> > g;
    g.insert(1); g.insert(3); g.insert(3); g.insert(2);
    test_eq( *g.begin(), 3 );
    test_eq( (int)g.count(3), 2 );
    test_pass("cxx23:flat_multiset::comparator");

    STD::flat_multiset<int> x, y;
    x.insert(1); x.insert(1);
    y.insert(9);
    test_true( x != y );
    x.swap(y);
    test_eq( *x.begin(), 9 );
    test_eq( (int)y.count(1), 2 );
    test_pass("cxx23:flat_multiset::swap");

    STD::flat_multiset<int> z;
    z.insert(9);
    test_true( x == z );
    test_pass("cxx23:flat_multiset::operator==");

    test_true( (STD::is_same<STD::flat_multiset<int>::key_type, int>::value) );
    test_true( (STD::is_same<STD::flat_multiset<int>::value_type, int>::value) );
    test_true( (STD::is_same<STD::flat_multiset<int>::container_type, STD::vector<int> >::value) );
    test_pass("cxx23:flat_multiset::member_types");

    test_true( (STD::is_same<STD::sorted_equivalent_t,
                             STD::sorted_equivalent_t>::value) );
    test_true( !(STD::is_same<STD::sorted_equivalent_t, STD::sorted_unique_t>::value) );
    test_pass("cxx23:sorted_equivalent_t");
}
#else
TEST_CASE_SKIP(flat_set, comparator_and_emplace)
TEST_CASE_SKIP(flat_set, multiset_basics)
TEST_CASE_SKIP(flat_set, multiset_construction)
#endif

#if defined(__cpp_lib_flat_set)
TEST_CASE(flat_set, member_types_cxx23) {
    typedef STD::flat_set<int> FS;

    test_true(( STD::is_same<FS::key_type, int>::value ));
    test_pass("cxx23:flat_set::key_type");
    test_true(( STD::is_same<FS::value_type, int>::value ));
    test_true(( STD::is_same<FS::key_type, FS::value_type>::value ));
    test_pass("cxx23:flat_set::value_type");
    test_true(( STD::is_same<FS::key_compare, STD::less<int> >::value ));
    test_pass("cxx23:flat_set::key_compare");
    test_true(( STD::is_same<FS::value_compare, FS::key_compare>::value ));
    test_pass("cxx23:flat_set::value_compare");
    test_true(( STD::is_same<FS::container_type, STD::vector<int> >::value ));
    test_pass("cxx23:flat_set::container_type");

    test_true(( STD::is_same<FS::reference, int&>::value ));
    test_true(( STD::is_same<FS::iterator, FS::const_iterator>::value ));
    test_pass("cxx23:flat_set::reference");
    test_true(( STD::is_same<FS::const_reference, const int&>::value ));
    test_pass("cxx23:flat_set::const_reference");

    test_true(( STD::is_same<FS::iterator, FS::const_iterator>::value ));
    test_true(( STD::is_same<STD::iterator_traits<FS::iterator>::iterator_category,
                             STD::random_access_iterator_tag>::value ));
    test_pass("cxx23:flat_set::iterator");
    test_pass("cxx23:flat_set::const_iterator");
    test_true(( STD::is_same<FS::const_reverse_iterator,
                             STD::reverse_iterator<FS::const_iterator> >::value ));
    test_pass("cxx23:flat_set::const_reverse_iterator");
    test_true(( STD::is_same<FS::difference_type,
                STD::iterator_traits<FS::iterator>::difference_type>::value ));
    test_true( STD::is_signed<FS::difference_type>::value );
    test_pass("cxx23:flat_set::difference_type");
    test_true( !STD::is_signed<FS::size_type>::value );
    test_pass("cxx23:flat_set::size_type");

    typedef STD::flat_multiset<int> FM;
    test_true(( STD::is_same<FM::key_type, int>::value ));
    test_pass("cxx23:flat_multiset::key_type");
    test_true(( STD::is_same<FM::value_type, int>::value ));
    test_pass("cxx23:flat_multiset::value_type");
    test_true(( STD::is_same<FM::key_compare, STD::less<int> >::value ));
    test_pass("cxx23:flat_multiset::key_compare");
    test_true(( STD::is_same<FM::value_compare, FM::key_compare>::value ));
    test_pass("cxx23:flat_multiset::value_compare");
    test_true(( STD::is_same<FM::container_type, STD::vector<int> >::value ));
    test_pass("cxx23:flat_multiset::container_type");
    test_true(( STD::is_same<FM::iterator, FM::const_iterator>::value ));
    test_pass("cxx23:flat_multiset::iterator");
    test_pass("cxx23:flat_multiset::const_iterator");
    test_true(( STD::is_same<FM::reverse_iterator,
                             STD::reverse_iterator<FM::iterator> >::value ));
    test_pass("cxx23:flat_multiset::reverse_iterator");
    test_true(( STD::is_same<FM::const_reverse_iterator,
                             STD::reverse_iterator<FM::const_iterator> >::value ));
    test_pass("cxx23:flat_multiset::const_reverse_iterator");
    test_true(( STD::is_same<FM::reference, int&>::value ));
    test_pass("cxx23:flat_multiset::reference");
    test_true(( STD::is_same<FM::const_reference, const int&>::value ));
    test_pass("cxx23:flat_multiset::const_reference");
    test_true( STD::is_signed<FM::difference_type>::value );
    test_pass("cxx23:flat_multiset::difference_type");
    test_true( !STD::is_signed<FM::size_type>::value );
    test_pass("cxx23:flat_multiset::size_type");

    test_true(( STD::uses_allocator<STD::flat_set<int, STD::less<int>,
                     STD::vector<int, STD::allocator<int> > >,
                 STD::allocator<int> >::value ));
    test_pass("cxx23:uses_allocator");
}

TEST_CASE(flat_set, iteration_and_size_cxx23) {
    int raw[] = { 3, 1, 2 };
    STD::flat_set<int> s(raw, raw + 3);

    test_eq( *s.begin(), 1 );
    test_pass("cxx23:flat_set::begin");
    test_eq( (long)(s.end() - s.begin()), 3L );
    test_pass("cxx23:flat_set::end");
    test_true( s.cbegin() == s.begin() );
    test_eq( *s.cbegin(), 1 );
    test_pass("cxx23:flat_set::cbegin");
    test_true( s.cend() == s.end() );
    test_pass("cxx23:flat_set::cend");

    test_eq( *s.rbegin(), 3 );
    test_pass("cxx23:flat_set::rbegin");
    test_eq( (long)(s.rend() - s.rbegin()), 3L );
    test_pass("cxx23:flat_set::rend");
    test_true( s.crbegin() == s.rbegin() );
    test_eq( *s.crbegin(), 3 );
    test_pass("cxx23:flat_set::crbegin");
    test_true( s.crend() == s.rend() );
    test_pass("cxx23:flat_set::crend");

    test_eq( s.size(), (STD::size_t)3 );
    s.insert(1);
    test_eq( s.size(), (STD::size_t)3 );
    test_pass("cxx23:flat_set::size");
    test_true( !s.empty() );
    STD::flat_set<int> none;
    test_true( none.empty() );
    test_true( none.begin() == none.end() );
    test_pass("cxx23:flat_set::empty");
    test_true( s.max_size() >= s.size() );
    test_true( s.max_size() > 0 );
    test_pass("cxx23:flat_set::max_size");

    STD::flat_multiset<int> m(raw, raw + 3);
    m.insert(1);
    test_eq( *m.rbegin(), 3 );
    test_pass("cxx23:flat_multiset::rbegin");
    test_eq( (long)(m.rend() - m.rbegin()), 4L );
    test_pass("cxx23:flat_multiset::rend");
    test_true( m.crbegin() == m.rbegin() );
    test_pass("cxx23:flat_multiset::crbegin");
    test_true( m.crend() == m.rend() );
    test_pass("cxx23:flat_multiset::crend");
}

TEST_CASE(flat_set, construction_and_container_access_cxx23) {
    STD::vector<int> v;
    v.push_back(3); v.push_back(1); v.push_back(3); v.push_back(2);
    STD::flat_set<int> fromc(v);
    test_eq( fsig(fromc), 123L );
    test_pass("cxx23:flat_set::flat_set(container_type)");

    STD::vector<int> sorted;
    sorted.push_back(1); sorted.push_back(2); sorted.push_back(3);
    STD::flat_set<int> presorted(STD::sorted_unique, sorted);
    test_eq( fsig(presorted), 123L );
    test_pass("cxx23:flat_set::flat_set(sorted_unique_t, ...)");
    test_true( !(STD::is_same<STD::sorted_unique_t, STD::sorted_equivalent_t>::value) );
    STD::sorted_unique_t tag = STD::sorted_unique;
    (void)tag;
    test_pass("cxx23:sorted_unique_t");

    STD::flat_set<int> il = { 3, 1, 2, 1 };
    test_eq( fsig(il), 123L );
    test_pass("cxx23:flat_set::flat_set(initializer_list)");

    STD::flat_set<int> fromr(STD::from_range, v);
    test_eq( fsig(fromr), 123L );
    test_pass("cxx23:flat_set::flat_set(from_range_t, R&&)");

    STD::flat_set<int> movable = { 5, 4 };
    STD::flat_set<int>::container_type taken = STD::move(movable).extract();
    test_eq( taken.size(), (STD::size_t)2 );
    test_eq( taken[0], 4 );
    test_true( movable.empty() );
    test_pass("cxx23:flat_set::extract");

    STD::vector<int> fresh;
    fresh.push_back(7); fresh.push_back(8);
    movable.replace(STD::move(fresh));
    test_eq( fsig(movable), 78L );
    test_pass("cxx23:flat_set::replace");

    STD::flat_set<int, STD::greater<int> > desc = { 1, 2, 3 };
    test_true( desc.key_comp()(3, 1) );
    test_eq( *desc.begin(), 3 );
    test_pass("cxx23:flat_set::key_comp");
    test_true( desc.value_comp()(3, 1) );
    test_true( !desc.value_comp()(1, 3) );
    test_pass("cxx23:flat_set::value_comp");

    test_true( desc.key_comp()(2, 1) );
    test_true( !desc.key_comp()(1, 2) );
    test_pass("cxx23:flat_set::compare");

    STD::flat_multiset<int, STD::greater<int> > mdesc = { 1, 2, 2 };
    test_true( mdesc.key_comp()(2, 1) );
    test_eq( *mdesc.begin(), 2 );
    test_pass("cxx23:flat_multiset::compare");
}

TEST_CASE(flat_set, modifiers_and_swap_cxx23) {
    STD::flat_set<int> s = { 1, 2, 3, 4, 5 };

    STD::flat_set<int>::iterator after = s.erase(s.begin());
    test_eq( *after, 2 );
    long removed = (long)s.erase(3);
    test_eq( removed, 1L );
    long missing = (long)s.erase(99);
    test_eq( missing, 0L );
    test_eq( fsig(s), 245L );
    test_pass("cxx23:flat_set::erase");

    STD::flat_set<int> r = { 1, 2, 3, 4, 5 };
    STD::flat_set<int>::iterator it = r.begin();
    ++it;
    STD::flat_set<int>::iterator stop = it;
    ++stop; ++stop;
    r.erase(it, stop);
    test_eq( fsig(r), 145L );
    test_pass("cxx23:flat_set::erase_range");

    STD::flat_set<int> h = { 1, 3 };
    STD::flat_set<int>::iterator placed = h.emplace_hint(h.end(), 2);
    test_eq( *placed, 2 );
    test_eq( fsig(h), 123L );
    test_pass("cxx23:flat_set::emplace_hint");

    STD::vector<int> more;
    more.push_back(9); more.push_back(0);
    h.insert_range(more);
    test_eq( fsig(h), 1239L );
    test_eq( h.size(), (STD::size_t)5 );
    test_eq( *h.begin(), 0 );
    test_pass("cxx23:flat_set::insert_range");

    STD::flat_set<int> a = { 1, 2 }, b = { 8, 9 };
    a.swap(b);
    test_eq( fsig(a), 89L );
    test_eq( fsig(b), 12L );
    test_pass("cxx23:flat_set::swap");
    swap(a, b);
    test_eq( fsig(a), 12L );
    test_pass("cxx23:swap(flat_set)");

    long gone = (long)STD::erase_if(a, IsOdd());
    test_eq( gone, 1L );
    test_eq( fsig(a), 2L );
    test_pass("cxx23:erase_if(flat_set)");

    STD::flat_multiset<int> ms = { 1, 2, 2, 3 };
    long mgone = (long)ms.erase(2);
    test_eq( mgone, 2L );
    test_eq( ms.size(), (STD::size_t)2 );
    test_pass("cxx23:flat_multiset::erase");

    STD::flat_set<int> src = { 4, 5, 6 };
    STD::flat_set<int> moved(STD::move(src));
    test_eq( fsig(moved), 456L );
    test_pass("cxx23:flat_set::move");
    STD::flat_multiset<int> msrc = { 7, 7 };
    STD::flat_multiset<int> mmoved(STD::move(msrc));
    test_eq( mmoved.size(), (STD::size_t)2 );
    test_pass("cxx23:flat_multiset::move");
}

TEST_CASE(flat_set, comparison_operators_cxx23) {
    STD::flat_set<int> a = { 1, 2, 3 }, b = { 1, 2, 3 }, c = { 1, 2, 4 };
    STD::flat_set<int> shorter = { 1, 2 };

    test_true( a == b );
    test_true( !(a == c) );
    test_true( !(a == shorter) );
    test_pass("cxx23:operator==(flat_set)");

    test_true( (a <=> c) < 0 );
    test_true( (c <=> a) > 0 );
    test_true( (a <=> b) == 0 );
    test_true( (shorter <=> a) < 0 );
    test_pass("cxx23:operator<=>(flat_set)");
}

TEST_CASE(flat_set, constexpr_cxx26) {
#if defined(__cpp_lib_constexpr_flat_set)
    {
        struct K {
            static constexpr int run() {
                STD::flat_set<int> s;
                s.insert(2);
                s.insert(1);
                s.insert(2);
                STD::flat_multiset<int> m;
                m.insert(5);
                m.insert(5);
                return (int)(s.size() * 10 + m.size());
            }
        };
        static_assert(K::run() == 22, "flat_set in a constant expression");
        test_eq( K::run(), 22 );
    }
    test_pass("cxx26:constexpr flat_set and flat_multiset");
#else
    TEST_NOTE("constexpr flat_set is C++26 (P3372); not in this library");
    test_skip("cxx26:constexpr flat_set and flat_multiset");
#endif
}
#else
TEST_CASE(flat_set, member_types_cxx23) {
    TEST_SKIP_N(0);
    test_skip("cxx23:flat_set::key_type");
    test_skip("cxx23:flat_set::value_type");
    test_skip("cxx23:flat_set::key_compare");
    test_skip("cxx23:flat_set::value_compare");
    test_skip("cxx23:flat_set::container_type");
    test_skip("cxx23:flat_set::reference");
    test_skip("cxx23:flat_set::const_reference");
    test_skip("cxx23:flat_set::iterator");
    test_skip("cxx23:flat_set::const_iterator");
    test_skip("cxx23:flat_set::const_reverse_iterator");
    test_skip("cxx23:flat_set::difference_type");
    test_skip("cxx23:flat_set::size_type");
    test_skip("cxx23:flat_multiset::key_type");
    test_skip("cxx23:flat_multiset::value_type");
    test_skip("cxx23:flat_multiset::key_compare");
    test_skip("cxx23:flat_multiset::value_compare");
    test_skip("cxx23:flat_multiset::container_type");
    test_skip("cxx23:flat_multiset::iterator");
    test_skip("cxx23:flat_multiset::const_iterator");
    test_skip("cxx23:flat_multiset::reverse_iterator");
    test_skip("cxx23:flat_multiset::const_reverse_iterator");
    test_skip("cxx23:flat_multiset::reference");
    test_skip("cxx23:flat_multiset::const_reference");
    test_skip("cxx23:flat_multiset::difference_type");
    test_skip("cxx23:flat_multiset::size_type");
    test_skip("cxx23:uses_allocator");
}
TEST_CASE(flat_set, iteration_and_size_cxx23) {
    test_skip("cxx23:flat_set::begin");
    test_skip("cxx23:flat_set::end");
    test_skip("cxx23:flat_set::cbegin");
    test_skip("cxx23:flat_set::cend");
    test_skip("cxx23:flat_set::rbegin");
    test_skip("cxx23:flat_set::rend");
    test_skip("cxx23:flat_set::crbegin");
    test_skip("cxx23:flat_set::crend");
    test_skip("cxx23:flat_set::size");
    test_skip("cxx23:flat_set::empty");
    test_skip("cxx23:flat_set::max_size");
    test_skip("cxx23:flat_multiset::rbegin");
    test_skip("cxx23:flat_multiset::rend");
    test_skip("cxx23:flat_multiset::crbegin");
    test_skip("cxx23:flat_multiset::crend");
}
TEST_CASE(flat_set, construction_and_container_access_cxx23) {
    test_skip("cxx23:flat_set::flat_set(container_type)");
    test_skip("cxx23:flat_set::flat_set(sorted_unique_t, ...)");
    test_skip("cxx23:sorted_unique_t");
    test_skip("cxx23:flat_set::flat_set(initializer_list)");
    test_skip("cxx23:flat_set::flat_set(from_range_t, R&&)");
    test_skip("cxx23:flat_set::extract");
    test_skip("cxx23:flat_set::replace");
    test_skip("cxx23:flat_set::key_comp");
    test_skip("cxx23:flat_set::value_comp");
    test_skip("cxx23:flat_set::compare");
    test_skip("cxx23:flat_multiset::compare");
}
TEST_CASE(flat_set, modifiers_and_swap_cxx23) {
    test_skip("cxx23:flat_set::erase");
    test_skip("cxx23:flat_set::erase_range");
    test_skip("cxx23:flat_set::emplace_hint");
    test_skip("cxx23:flat_set::insert_range");
    test_skip("cxx23:flat_set::swap");
    test_skip("cxx23:swap(flat_set)");
    test_skip("cxx23:erase_if(flat_set)");
    test_skip("cxx23:flat_multiset::erase");
    test_skip("cxx23:flat_set::move");
    test_skip("cxx23:flat_multiset::move");
}
TEST_CASE(flat_set, comparison_operators_cxx23) {
    test_skip("cxx23:operator==(flat_set)");
    test_skip("cxx23:operator<=>(flat_set)");
}
TEST_CASE(flat_set, constexpr_cxx26) {
    test_skip("cxx26:constexpr flat_set and flat_multiset");
}
#endif


TEST_CASE(flat_set, insert_forms_cxx23) {
    static const int src[4] = { 3, 1, 3, 2 };

    {   STD::flat_set<int> s;
        s.insert(5);
        STD::flat_set<int>::iterator a = s.insert(s.end(), 9);
        test_eq( *a, 9 );
        STD::flat_set<int>::iterator b = s.insert(s.begin(), 1);
        test_eq( *b, 1 );
        STD::flat_set<int>::iterator dup = s.insert(s.begin(), 5);
        test_eq( *dup, 5 );
        test_eq( (long)s.size(), 3L );
        test_eq( *s.begin(), 1 ); }
    test_pass("cxx23:flat_set::insert_hint");

    {   STD::flat_set<int> s;
        s.insert(src, src);
        test_true( s.empty() );
        s.insert(src, src + 4);
        test_eq( (long)s.size(), 3L );
        test_eq( *s.begin(), 1 );
        s.insert(src, src + 1);
        test_eq( (long)s.size(), 3L ); }
    test_pass("cxx23:flat_set::insert_iterator_pair");

    {   STD::flat_multiset<int> m;
        m.insert(5);
        STD::flat_multiset<int>::iterator a = m.insert(m.end(), 9);
        test_eq( *a, 9 );
        STD::flat_multiset<int>::iterator dup = m.insert(m.begin(), 5);
        test_eq( *dup, 5 );
        test_eq( (long)m.size(), 3L );
        test_eq( (long)m.count(5), 2L ); }
    test_pass("cxx23:flat_multiset::insert_hint");

    {   STD::flat_multiset<int> m;
        m.insert(src, src);
        test_true( m.empty() );
        m.insert(src, src + 4);
        test_eq( (long)m.size(), 4L );
        test_eq( (long)m.count(3), 2L );
        test_eq( *m.begin(), 1 ); }
    test_pass("cxx23:flat_multiset::insert_iterator_pair");

    {   STD::flat_multiset<int> m;
        m.insert(src, src + 4);
        STD::flat_multiset<int>::iterator none = m.erase(m.begin(), m.begin());
        test_true( none == m.begin() );
        test_eq( (long)m.size(), 4L );
        STD::flat_multiset<int>::iterator it = m.begin();
        ++it; ++it;
        m.erase(m.begin(), it);
        test_eq( (long)m.size(), 2L );
        test_eq( *m.begin(), 3 );
        m.erase(m.begin(), m.end());
        test_true( m.empty() ); }
    test_pass("cxx23:flat_multiset::erase_range");
}

#endif   // TEST_TARGET_CXX >= 2023
TEST_CASE(flat_set, feature_test_macros) {
#if defined(__cpp_lib_flat_set)
    test_true( __cpp_lib_flat_set > 0L );
    test_pass("cxx23:__cpp_lib_flat_set");
#else
    test_skip("cxx23:__cpp_lib_flat_set");
#endif
#if defined(__cpp_lib_constexpr_flat_set)
    test_true( __cpp_lib_constexpr_flat_set > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_flat_set");
#else
    test_skip("cxx26:__cpp_lib_constexpr_flat_set");
#endif
#if defined(__cpp_lib_flat_set)
    test_true( __cpp_lib_flat_set > 0L );
    test_pass("cxx26:__cpp_lib_flat_set");
#else
    test_skip("cxx26:__cpp_lib_flat_set");
#endif
}
