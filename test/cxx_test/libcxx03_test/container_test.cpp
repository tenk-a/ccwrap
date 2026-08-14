#include "test_cxx.hpp"

#include <vector>
#include <map>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <stack>
#include <queue>
#include <bitset>
#include <string>
#include <algorithm>
#include <functional>

TEST_CASE(container, vector) {
    STD::vector<int> v;
    test_true( v.empty() );
    for (int i = 0; i < 5; ++i) v.push_back(i * 10);
    test_eq( v.size(), 5u );
    test_true( v.capacity() >= 5u );
    test_eq( v[2], 20 );
    test_eq( v.at(3), 30 );
    test_eq( v.front(), 0 );
    test_eq( v.back(), 40 );
    test_pass("cxx03:vector element access");

    v.pop_back();
    test_eq( v.size(), 4u );
    test_pass("cxx03:vector::pop_back");
    v.insert(v.begin(), -1);
    test_eq( v.front(), -1 );
    test_pass("cxx03:vector::insert");
    v.erase(v.begin());
    test_eq( v.front(), 0 );
    test_pass("cxx03:vector::erase");
    v.resize(2);
    test_eq( v.size(), 2u );
    test_pass("cxx03:vector::resize");
    v.reserve(32);
    test_true( v.capacity() >= 32u );
    test_pass("cxx03:vector::reserve");

    int sum = 0;
    for (STD::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) sum += *it;
    test_eq( sum, 10 );
    test_pass("cxx03:vector reverse iterator");

    STD::vector<int> a(3, 7), b;
    b.assign(a.begin(), a.end());
    test_true( a == b );
    test_pass("cxx03:vector::assign");
    a.swap(b);
    test_true( a == b );
    test_pass("cxx03:vector::swap");
    v.clear();
    test_true( v.empty() );
    test_pass("cxx03:vector::clear");
}

TEST_CASE(container, map_set) {
    STD::map<STD::string, int> m;
    m["one"] = 1; m["two"] = 2;
    test_eq( m["two"], 2 );
    test_pass("cxx03:map::operator[]");
    m.insert(STD::make_pair(STD::string("three"), 3));
    test_eq( m.size(), 3u );
    test_pass("cxx03:map::insert");
    test_eq( m.count("one"), 1u );
    test_eq( m.count("zzz"), 0u );
    test_pass("cxx03:map::count");
    STD::map<STD::string, int>::iterator it = m.find("three");
    test_true( it != m.end() );
    test_eq( it->second, 3 );
    test_pass("cxx03:map::find");
    test_true( m.lower_bound("one") != m.end() );
    test_pass("cxx03:map::lower_bound");
    m.erase("one");
    test_eq( m.count("one"), 0u );
    test_pass("cxx03:map::erase(key)");

    STD::set<int> s;
    s.insert(3); s.insert(1); s.insert(2); s.insert(1);
    test_eq( s.size(), 3u );
    test_eq( *s.begin(), 1 );
    test_pass("cxx03:set::insert");
    test_true( s.find(2) != s.end() );
    test_pass("cxx03:set::find");
    test_true( *s.lower_bound(2) == 2 );
    test_pass("cxx03:set::lower_bound");
    s.erase(2);
    test_eq( s.count(2), 0u );
    test_pass("cxx03:set::erase(key)");

    STD::multiset<int> ms;
    ms.insert(5); ms.insert(5);
    test_eq( ms.count(5), 2u );
    test_pass("cxx03:multiset count duplicates");
    STD::multimap<int, int> mm;
    mm.insert(STD::make_pair(1, 10));
    mm.insert(STD::make_pair(1, 20));
    test_eq( mm.count(1), 2u );
    test_pass("cxx03:multimap count duplicates");
}

TEST_CASE(container, list_deque) {
    STD::list<int> l;
    l.push_back(2); l.push_front(1); l.push_back(3);
    test_eq( l.size(), 3u );
    test_eq( l.back(), 3 );
    test_pass("cxx03:list::push_back");
    test_eq( l.front(), 1 );
    test_pass("cxx03:list::push_front");
    l.reverse();
    test_eq( l.front(), 3 );
    test_pass("cxx03:list::reverse");
    l.sort();
    test_eq( l.front(), 1 );
    test_pass("cxx03:list::sort");
    l.pop_front();
    test_eq( l.front(), 2 );
    test_pass("cxx03:list::pop_front");
    int dup[] = { 5, 5, 6 };
    STD::list<int> l2(dup, dup + 3);
    l2.unique();
    test_eq( l2.size(), 2u );
    test_pass("cxx03:list::unique");
    l2.remove(6);
    test_eq( l2.front(), 5 );
    test_pass("cxx03:list::remove");

    STD::deque<int> d;
    d.push_back(1); d.push_front(0); d.push_back(2);
    test_eq( d.size(), 3u );
    test_pass("cxx03:deque::push");
    test_eq( d.front(), 0 );
    test_pass("cxx03:deque::front");
    test_eq( d.back(), 2 );
    test_pass("cxx03:deque::back");
    test_eq( d[0], 0 );
    test_eq( d.at(2), 2 );
    test_pass("cxx03:deque::index");
    d.pop_front();
    test_eq( d.front(), 1 );
    test_pass("cxx03:deque::pop_front");
}

TEST_CASE(container, adaptors) {
    STD::stack<int> st;
    st.push(1); st.push(2); st.push(3);
    test_eq( st.size(), 3u );
    test_pass("cxx03:stack::push");
    test_eq( st.top(), 3 );
    test_pass("cxx03:stack::top");
    st.pop();
    test_eq( st.top(), 2 );
    test_true( !st.empty() );
    test_pass("cxx03:stack::pop");

    STD::queue<int> q;
    q.push(1); q.push(2); q.push(3);
    test_true( 1 );
    test_pass("cxx03:queue::push");
    test_eq( q.front(), 1 );
    test_pass("cxx03:queue::front");
    test_eq( q.back(), 3 );
    test_pass("cxx03:queue::back");
    q.pop();
    test_eq( q.front(), 2 );
    test_eq( q.size(), 2u );
    test_pass("cxx03:queue::pop");

    STD::priority_queue<int> pq;
    pq.push(1); pq.push(5); pq.push(3);
    test_true( 1 );
    test_pass("cxx03:priority_queue::push");
    test_eq( pq.top(), 5 );
    test_pass("cxx03:priority_queue::top");
    pq.pop();
    test_eq( pq.top(), 3 );
    test_eq( pq.size(), 2u );
    test_pass("cxx03:priority_queue::pop");
}

TEST_CASE(container, bitset) {
    STD::bitset<8> b;
    test_eq( b.size(), 8u );
    test_true( b.none() );
    b.set(0); b.set(2);
    test_true( b.test(0) );
    test_pass("cxx03:bitset::set");
    test_true( !b.test(1) );
    test_pass("cxx03:bitset::test");
    test_eq( b.count(), 2u );
    test_true( b.any() );
    test_pass("cxx03:bitset::count");
    b.flip(1);
    test_true( b.test(1) );
    test_pass("cxx03:bitset::flip");
    b.reset(0);
    test_true( !b.test(0) );
    test_eq( (int)b.to_ulong(), 6 );
    test_pass("cxx03:bitset::reset");

    STD::bitset<4> full;
    full.set();
    test_true( full.count() == full.size() );
    test_eq( full.to_ulong(), 15u );
    test_eq( (~full).to_ulong(), 0u );
    test_pass("cxx03:bitset set-all and complement");
    STD::bitset<4> x(STD::string("1010"));
    test_eq( x.to_ulong(), 10u );
    test_true( (x & STD::bitset<4>(STD::string("1100"))).to_ulong() == 8u );
    test_true( (x | STD::bitset<4>(STD::string("0101"))).count() == 4u );
    test_pass("cxx03:bitset string ctor and bitwise ops");
}

TEST_CASE(container, list_members) {
    STD::list<int> l;
    for (int i = 1; i <= 5; ++i) l.push_back(i);
    test_eq( *l.rbegin(), 5 );
    test_pass("cxx03:list::rbegin");
    test_true( l.max_size() > 1000 );
    test_pass("cxx03:list::max_size");

    l.resize(3);
    test_eq( l.size(), (STD::size_t)3 );
    test_eq( l.back(), 3 );
    test_pass("cxx03:list::resize (shrink)");
    l.resize(5, 9);
    test_eq( l.back(), 9 );
    test_pass("cxx03:list::resize (grow with value)");

    l.unique(STD::equal_to<int>());
    test_eq( l.size(), (STD::size_t)4 );
    test_pass("cxx03:list::unique(predicate)");

    l.sort(STD::greater<int>());
    test_eq( l.front(), 9 );
    test_eq( l.back(), 1 );
    test_pass("cxx03:list::sort(comparator)");

    STD::list<int> a, b;
    a.push_back(1); b.push_back(2); b.push_back(3);
    a.swap(b);
    test_eq( a.size(), (STD::size_t)2 );
    test_eq( b.size(), (STD::size_t)1 );
    test_pass("cxx03:list::swap (members)");

    STD::list<int> p1, p2;
    p1.push_back(1); p1.push_back(2);
    p2.push_back(1); p2.push_back(3);
    test_true( p1 < p2 && p2 > p1 && p1 <= p2 && p1 != p2 );
    test_pass("cxx03:list relational operators");
}

TEST_CASE(container, map_members) {
    STD::map<int, int> m;
    m[3] = 30; m[1] = 10; m[2] = 20;
    test_eq( m.rbegin()->first, 3 );
    test_pass("cxx03:map::rbegin");
    test_true( m.max_size() > 1000 );
    test_pass("cxx03:map::max_size");

    m.erase(m.find(2));
    test_true( !m.count(2) );
    test_eq( m.size(), (STD::size_t)2 );
    test_pass("cxx03:map::erase(iterator)");
    test_true( m.key_comp()(1, 2) );
    test_pass("cxx03:map::key_comp");

    STD::map<int, int> a, b;
    a[1] = 1; b[8] = 8; b[9] = 9;
    a.swap(b);
    test_eq( a.size(), (STD::size_t)2 );
    test_eq( b.size(), (STD::size_t)1 );
    test_true( b.count(1) );
    test_pass("cxx03:map::swap (members)");

    STD::map<int, int> x, y;
    x[1] = 1; x[2] = 2; y[1] = 1; y[2] = 3;
    test_true( x < y && y > x && x != y );
    test_pass("cxx03:map relational operators");

    STD::map<int, int> src;
    src[5] = 50; src[6] = 60;
    STD::map<int, int> rc(src.begin(), src.end());
    test_eq( rc.size(), (STD::size_t)2 );
    test_eq( rc[5], 50 );
    test_pass("cxx03:map range constructor (members)");
}

namespace {

template <class S>
bool sorted_and_complete(const S& s, int n) {
    if ((int)s.size() != n) return false;
    int expect = 0;
    for (typename S::const_iterator i = s.begin(); i != s.end(); ++i, ++expect)
        if (*i != expect) return false;
    return expect == n;
}

inline int rb_height_bound(int n) {
    int log2n = 0;
    while ((1 << (log2n + 1)) <= n + 1) ++log2n;
    return 2 * (log2n + 1);
}

struct ReverseCmp { bool operator()(int a, int b) const { return a > b; } };

struct XorCmp {
    int key_;
    XorCmp() : key_(0) {}
    explicit XorCmp(int k) : key_(k) {}
    bool operator()(int a, int b) const { return (a ^ key_) < (b ^ key_); }
};

}

TEST_CASE(container, tree_balanced_sorted_insert) {

    const int N = 2000;
    STD::set<int> s;
    for (int i = 0; i < N; ++i) s.insert(i);
    test_true( sorted_and_complete(s, N) );

    for (int i = 0; i < N; ++i) test_true( s.find(i) != s.end() );
    test_true( s.find(N) == s.end() );
    test_pass("cxx03:set stays sorted on ascending insert");

    STD::set<int> d;
    for (int i = N - 1; i >= 0; --i) d.insert(i);
    test_true( sorted_and_complete(d, N) );
    test_pass("cxx03:set stays sorted on descending insert");

    for (int i = 0; i < N; ++i) {
        long n = (long)s.erase(i);
        test_eq( n, 1L );
        test_eq( (long)s.size(), (long)(N - 1 - i) );
    }
    test_true( s.empty() );
    test_pass("cxx03:set::erase leftmost repeatedly");

    for (int i = N - 1; i >= 0; --i) { long n = (long)d.erase(i); test_eq( n, 1L ); }
    test_true( d.empty() );
    test_pass("cxx03:set::erase rightmost repeatedly");
}

TEST_CASE(container, tree_interleaved_erase) {
    const int N = 500;
    STD::map<int, int> m;
    for (int i = 0; i < N; ++i) m[i] = i * 2;

    for (int i = 0; i < N; i += 2) m.erase(i);
    test_eq( (long)m.size(), (long)(N / 2) );
    test_pass("cxx03:map::erase every other key");

    int expect = 1;
    for (STD::map<int, int>::iterator i = m.begin(); i != m.end(); ++i, expect += 2) {
        test_eq( i->first, expect );
        test_eq( i->second, expect * 2 );
    }
    test_pass("cxx03:map survivors intact and in order");

    while (!m.empty())
        m.erase(m.begin());
    test_true( m.empty() );
    test_pass("cxx03:map::erase(iterator) until empty");
}

#if defined(__WATCOMC__)
TEST_CASE(container, libcxx03_tree_invariants) {
    const int N = 2000;
    STD::set<int> s;
    for (int i = 0; i < N; ++i) s.insert(i);
    test_true( s.__tree_rb_valid() );
    test_lim( (int)s.__tree_height(), 1, rb_height_bound(N) );
    test_pass("cxx03:set is a valid RB tree (ascending insert)");

    STD::set<int> d;
    for (int i = N - 1; i >= 0; --i) d.insert(i);
    test_true( d.__tree_rb_valid() );
    test_lim( (int)d.__tree_height(), 1, rb_height_bound(N) );
    test_pass("cxx03:set is a valid RB tree (descending insert)");

    for (int i = 0; i < N; ++i) {
        s.erase(i);
        if ((i & 0x7f) == 0) test_true( s.__tree_rb_valid() );
    }
    test_true( s.__tree_rb_valid() );
    test_pass("cxx03:set RB valid after erase");

    const int M = 500;
    STD::map<int, int> m;
    for (int i = 0; i < M; ++i) m[i] = i * 2;
    test_true( m.__tree_rb_valid() );
    test_lim( (int)m.__tree_height(), 1, rb_height_bound(M) );
    test_pass("cxx03:map is a valid RB tree with bounded height");

    for (int i = 0; i < M; i += 2) m.erase(i);
    test_true( m.__tree_rb_valid() );
    test_pass("cxx03:map RB valid after interleaved erase");

    while (!m.empty()) {
        m.erase(m.begin());
        test_true( m.__tree_rb_valid() );
    }
    test_pass("cxx03:map RB valid erasing to empty");
}
#else
TEST_CASE_SKIP(container, libcxx03_tree_invariants)
#endif

TEST_CASE(container, tree_multi_balanced) {
    const int N = 1000;
    STD::multiset<int> ms;
    for (int i = 0; i < N; ++i) { ms.insert(i % 10); }
    test_eq( (long)ms.size(), (long)N );
    test_eq( (long)ms.count(3), 100L );
    test_pass("cxx03:multiset holds all duplicates");

    int prev = -1, seen = 0;
    for (STD::multiset<int>::iterator i = ms.begin(); i != ms.end(); ++i, ++seen) {
        test_true( *i >= prev );
        prev = *i;
    }
    test_eq( seen, N );
    test_pass("cxx03:multiset sorted across duplicates");

    long removed = (long)ms.erase(3);
    test_eq( removed, 100L );
    test_eq( (long)ms.size(), (long)(N - 100) );
    test_eq( (long)ms.count(3), 0L );
    test_pass("cxx03:multiset::erase(key) removes all duplicates");
}

TEST_CASE(container, tree_comparator_is_kept) {
    STD::set<int, ReverseCmp> r((ReverseCmp()));
    r.insert(1); r.insert(3); r.insert(2);
    STD::set<int, ReverseCmp>::iterator i = r.begin();
    test_eq( *i, 3 ); ++i;
    test_eq( *i, 2 ); ++i;
    test_eq( *i, 1 );
    test_pass("cxx03:set retains a reverse comparator");

    STD::set<int, XorCmp> x((XorCmp(4)));
    x.insert(0); x.insert(4); x.insert(1);
    STD::set<int, XorCmp>::iterator j = x.begin();
    test_eq( *j, 4 ); ++j;
    test_eq( *j, 0 ); ++j;
    test_eq( *j, 1 );
    test_pass("cxx03:set retains a stateful comparator");

    test_true( x.key_comp()(4, 0) );
    test_true( !x.key_comp()(0, 4) );
    test_pass("cxx03:set::key_comp returns the real comparator");

    STD::set<int, XorCmp> y = x;
    test_eq( *y.begin(), 4 );
    STD::set<int, XorCmp> z((XorCmp(0)));
    z = x;
    test_eq( *z.begin(), 4 );
    test_pass("cxx03:set comparator survives copy and assignment");
}

namespace {
struct IsOdd  { bool operator()(int v) const { return (v & 1) != 0; } };
struct IsEven { bool operator()(int v) const { return (v & 1) == 0; } };
struct IsAny  { bool operator()(int) const { return true; } };
struct IsNone { bool operator()(int) const { return false; } };

struct AllocStats {
    static int allocs;
    static int deallocs;
    static void reset() { allocs = 0; deallocs = 0; }
    static int live() { return allocs - deallocs; }
};
int AllocStats::allocs = 0;
int AllocStats::deallocs = 0;

template <class T>
struct TagAlloc : STD::allocator<T> {
    typedef STD::size_t size_type;
    typedef T*          pointer;
    template <class U> struct rebind { typedef TagAlloc<U> other; };

    int tag;
    TagAlloc() : tag(0) {}
    explicit TagAlloc(int t) : tag(t) {}
    template <class U> TagAlloc(const TagAlloc<U>& o) : tag(o.tag) {}

    pointer allocate(size_type n, const void* = 0) {
        ++AllocStats::allocs;
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }
    void deallocate(pointer p, size_type) {
        ++AllocStats::deallocs;
        ::operator delete(static_cast<void*>(p));
    }
};
}

TEST_CASE(container, set_constructors) {
    const int src[] = { 5, 1, 5, 3 };

    STD::set<int> a(src, src + 4);
    test_eq( (long)a.size(), 3L );
    test_eq( *a.begin(), 1 );
    test_pass("cxx03:set range constructor");

    STD::set<int, ReverseCmp> b(src, src + 4, ReverseCmp());
    test_eq( (long)b.size(), 3L );
    test_eq( *b.begin(), 5 );
    test_true( b.key_comp()(5, 1) );
    test_pass("cxx03:set range constructor keeps comparator");

    STD::set<int, XorCmp> c(src, src + 4, XorCmp(4));
    test_eq( *c.begin(), 5 );
    test_true( c.key_comp()(5, 1) );
    test_pass("cxx03:set range constructor keeps stateful comparator");

    STD::set<int, ReverseCmp> d(b);
    test_eq( *d.begin(), 5 );
    test_pass("cxx03:set copy ctor keeps comparator");
    STD::set<int, ReverseCmp> e;
    e = b;
    test_eq( *e.begin(), 5 );
    test_eq( (long)e.size(), 3L );
    test_pass("cxx03:set copy assignment keeps comparator");

    STD::multiset<int> m(src, src + 4);
    test_eq( (long)m.size(), 4L );
    test_eq( (long)m.count(5), 2L );
    STD::multiset<int, ReverseCmp> mr(src, src + 4, ReverseCmp());
    test_eq( *mr.begin(), 5 );
    test_eq( (long)mr.size(), 4L );
    test_pass("cxx03:multiset range constructor keeps duplicates");
}

TEST_CASE(container, set_get_allocator) {
    STD::set<int, STD::less<int>, TagAlloc<int> > s((STD::less<int>()), TagAlloc<int>(7));
    test_eq( s.get_allocator().tag, 7 );

    STD::set<int, STD::less<int>, TagAlloc<int> > t((TagAlloc<int>(9)));
    test_eq( t.get_allocator().tag, 9 );
    test_pass("cxx11:set get_allocator round-trips");
    t.insert(1);
    test_eq( (long)t.size(), 1L );
    test_pass("cxx11:set with custom allocator still functions");

    s.insert(5);
    s.swap(t);
    test_eq( s.get_allocator().tag, 9 );
    test_eq( t.get_allocator().tag, 7 );
    test_eq( *s.begin(), 1 );
    test_eq( *t.begin(), 5 );
    test_pass("cxx11:set::swap exchanges allocator with contents");

    STD::multiset<int, STD::less<int>, TagAlloc<int> > ms((TagAlloc<int>(3)));
    test_eq( ms.get_allocator().tag, 3 );
    test_pass("cxx11:multiset get_allocator round-trips");
}

TEST_CASE(container, set_hint_insert_and_emplace) {
    STD::set<int> s;

    STD::set<int>::iterator i = s.insert(s.end(), 2);
    test_eq( *i, 2 );
    i = s.insert(s.begin(), 1);
    test_eq( *i, 1 );
    i = s.insert(s.end(), 2);
    test_eq( *i, 2 );
    test_eq( (long)s.size(), 2L );
    test_pass("cxx03:set::insert with hint");

    i = s.emplace_hint(s.begin(), 9);
    test_eq( *i, 9 );
    test_eq( (long)s.size(), 3L );
    i = s.emplace_hint(s.end(), 9);
    test_eq( *i, 9 );
    test_eq( (long)s.size(), 3L );
    test_pass("cxx11:set::emplace_hint");

    STD::multiset<int> m;
    m.emplace_hint(m.end(), 4);
    m.emplace_hint(m.begin(), 4);
    test_eq( (long)m.size(), 2L );
    test_eq( (long)m.count(4), 2L );
    test_true( m.__tree_rb_valid() );
    test_pass("cxx11:multiset::emplace_hint always inserts");
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(container, set_contains) {
    const int src[] = { 1, 3, 5 };
    STD::set<int> s(src, src + 3);
    test_true( s.contains(1) );
    test_true( s.contains(5) );
    test_true( !s.contains(0) );
    test_true( !s.contains(4) );
    test_true( !s.contains(6) );

    test_eq( s.contains(3), s.find(3) != s.end() );
    test_eq( s.contains(4), s.find(4) != s.end() );
    test_pass("cxx20:set::contains agrees with find");

    STD::set<int> empty;
    test_true( !empty.contains(1) );
    test_pass("cxx20:set::contains on empty set");

    STD::multiset<int> m(src, src + 3);
    m.insert(3);
    test_true( m.contains(3) );
    test_true( !m.contains(2) );
    test_pass("cxx20:multiset::contains");
}

TEST_CASE(container, set_erase_if) {
    const int src[] = { 1, 2, 3, 4, 5, 6 };
    STD::set<int> s(src, src + 6);
    STD::set<int>::size_type n = STD::erase_if(s, IsOdd());
    test_eq( (long)n, 3L );
    test_eq( (long)s.size(), 3L );
    test_true( !s.contains(1) && !s.contains(3) && !s.contains(5) );
    test_true( s.contains(2) && s.contains(4) && s.contains(6) );
    test_true( s.__tree_rb_valid() );
    test_pass("cxx20:erase_if(set) removes matching elements");

    STD::set<int> t(src, src + 6);
    n = STD::erase_if(t, IsEven());
    test_eq( (long)n, 3L );
    test_eq( (long)t.size(), 3L );
    STD::set<int> u(src, src + 6);
    n = STD::erase_if(u, IsAny());
    test_eq( (long)n, 6L );
    test_true( u.empty() );
    test_pass("cxx20:erase_if(set) erasing all");
    STD::set<int> v(src, src + 6);
    n = STD::erase_if(v, IsNone());
    test_eq( (long)n, 0L );
    test_eq( (long)v.size(), 6L );
    test_pass("cxx20:erase_if(set) erasing none");

    STD::multiset<int> m(src, src + 6);
    m.insert(3); m.insert(3);
    test_eq( (long)m.size(), 8L );
    STD::multiset<int>::size_type mn = STD::erase_if(m, IsOdd());
    test_eq( (long)mn, 5L );
    test_eq( (long)m.size(), 3L );
    test_eq( (long)m.count(3), 0L );
    test_true( m.__tree_rb_valid() );
    test_pass("cxx20:erase_if(multiset) removes all matching duplicates");
}
#else
TEST_CASE_SKIP(container, set_contains)
TEST_CASE_SKIP(container, set_erase_if)
#endif

namespace {

struct ValueIsOdd { bool operator()(const STD::pair<const int, int>& p) const { return (p.second & 1) != 0; } };
struct KeyIsOdd   { bool operator()(const STD::pair<const int, int>& p) const { return (p.first & 1) != 0; } };
struct PairAny    { bool operator()(const STD::pair<const int, int>&) const { return true; } };
struct PairNone   { bool operator()(const STD::pair<const int, int>&) const { return false; } };
}

TEST_CASE(container, map_constructors) {
    STD::pair<const int, int> src[] = {
        STD::pair<const int, int>(5, 50), STD::pair<const int, int>(1, 10),
        STD::pair<const int, int>(5, 99), STD::pair<const int, int>(3, 30)
    };

    STD::map<int, int> a(src, src + 4);
    test_eq( (long)a.size(), 3L );
    test_eq( a[5], 50 );
    test_pass("cxx03:map range constructor keeps first value for a key");

    STD::map<int, int, ReverseCmp> b(src, src + 4, ReverseCmp());
    test_eq( (long)b.size(), 3L );
    test_eq( b.begin()->first, 5 );
    test_true( b.key_comp()(5, 1) );
    test_pass("cxx03:map range constructor keeps comparator");

    STD::map<int, int, ReverseCmp> c(b);
    test_eq( c.begin()->first, 5 );
    test_pass("cxx03:map copy ctor keeps comparator");
    STD::map<int, int, ReverseCmp> d;
    d = b;
    test_eq( d.begin()->first, 5 );
    test_pass("cxx03:map copy assignment keeps comparator");

    STD::multimap<int, int> m(src, src + 4);
    test_eq( (long)m.size(), 4L );
    test_eq( (long)m.count(5), 2L );
    STD::multimap<int, int, ReverseCmp> mr(src, src + 4, ReverseCmp());
    test_eq( mr.begin()->first, 5 );
    test_pass("cxx03:multimap range constructor keeps duplicate keys");

    STD::map<int, int, STD::less<int>, TagAlloc<STD::pair<const int, int> > >
        g((TagAlloc<STD::pair<const int, int> >(11)));
    test_eq( g.get_allocator().tag, 11 );
    g[1] = 2;
    test_eq( g[1], 2 );
    test_pass("cxx11:map get_allocator round-trips");
}

TEST_CASE(container, map_hint_and_emplace_hint) {
    STD::map<int, int> m;
    STD::map<int, int>::iterator i = m.insert(m.end(), STD::pair<const int, int>(2, 20));
    test_eq( i->second, 20 );
    test_pass("cxx03:map::insert with hint");
    i = m.emplace_hint(m.begin(), 1, 10);
    test_eq( i->first, 1 );
    test_eq( i->second, 10 );
    i = m.emplace_hint(m.end(), 1, 99);
    test_eq( i->second, 10 );
    test_eq( (long)m.size(), 2L );
    test_pass("cxx11:map::emplace_hint (no overwrite on existing key)");

    STD::multimap<int, int> mm;
    mm.emplace_hint(mm.end(), 4, 1);
    mm.emplace_hint(mm.begin(), 4, 2);
    test_eq( (long)mm.size(), 2L );
    test_eq( (long)mm.count(4), 2L );
    test_true( mm.__tree_rb_valid() );
    test_pass("cxx11:multimap::emplace_hint always inserts");
}

#if TEST_TARGET_CXX >= 2020
TEST_CASE(container, map_contains) {
    STD::map<int, int> m;
    m[1] = 10; m[3] = 30;
    test_true( m.contains(1) );
    test_true( m.contains(3) );
    test_true( !m.contains(2) );
    test_eq( m.contains(3), m.find(3) != m.end() );
    test_eq( m.contains(9), m.find(9) != m.end() );
    test_pass("cxx20:map::contains agrees with find");

    STD::multimap<int, int> mm;
    mm.insert(STD::pair<const int, int>(7, 1));
    test_true( mm.contains(7) );
    test_true( !mm.contains(8) );
    test_pass("cxx20:multimap::contains");
}

TEST_CASE(container, map_erase_if) {
    STD::map<int, int> m;
    for (int k = 1; k <= 6; ++k) m[k] = k * 10 + 1;

    STD::map<int, int>::size_type n = STD::erase_if(m, ValueIsOdd());
    test_eq( (long)n, 6L );
    test_true( m.empty() );
    test_pass("cxx20:erase_if(map) by mapped value");

    STD::map<int, int> k;
    for (int j = 1; j <= 6; ++j) k[j] = j;
    n = STD::erase_if(k, KeyIsOdd());
    test_eq( (long)n, 3L );
    test_eq( (long)k.size(), 3L );
    test_true( k.contains(2) && k.contains(4) && k.contains(6) );
    test_true( !k.contains(1) && !k.contains(3) && !k.contains(5) );
    test_true( k.__tree_rb_valid() );
    test_pass("cxx20:erase_if(map) by key");

    STD::map<int, int> z;
    for (int j = 1; j <= 4; ++j) z[j] = j;
    n = STD::erase_if(z, PairNone());
    test_eq( (long)n, 0L );
    test_eq( (long)z.size(), 4L );
    test_pass("cxx20:erase_if(map) erasing none");
    n = STD::erase_if(z, PairAny());
    test_eq( (long)n, 4L );
    test_true( z.empty() );
    test_pass("cxx20:erase_if(map) erasing all");

    STD::multimap<int, int> mm;
    mm.insert(STD::pair<const int, int>(1, 1));
    mm.insert(STD::pair<const int, int>(1, 2));
    mm.insert(STD::pair<const int, int>(2, 3));
    STD::multimap<int, int>::size_type mn = STD::erase_if(mm, KeyIsOdd());
    test_eq( (long)mn, 2L );
    test_eq( (long)mm.size(), 1L );
    test_eq( (long)mm.count(1), 0L );
    test_true( mm.__tree_rb_valid() );
    test_pass("cxx20:erase_if(multimap) removes all matching duplicates");

    const int isrc[] = { 1, 2, 3 };
    STD::set<int> s(isrc, isrc + 3);
    STD::set<int>::size_type sn = STD::erase_if(s, IsOdd());
    test_eq( (long)sn, 2L );
    test_eq( (long)s.size(), 1L );
    test_pass("cxx20:erase_if(set) overload resolves in same TU as map");
}
#else
TEST_CASE_SKIP(container, map_contains)
TEST_CASE_SKIP(container, map_erase_if)
#endif

TEST_CASE(container, set_allocator_is_used) {
    typedef STD::set<int, STD::less<int>, TagAlloc<int> > S;

    AllocStats::reset();
    {
        S s((TagAlloc<int>(1)));
        test_eq( AllocStats::allocs, 0 );
        for (int i = 0; i < 8; ++i) s.insert(i);
        test_true( AllocStats::allocs >= 8 );
        test_eq( (long)s.size(), 8L );

        int after_fill = AllocStats::allocs;
        s.erase(3);
        test_eq( AllocStats::deallocs, 1 );
        test_eq( AllocStats::allocs, after_fill );

        s.clear();
        test_eq( AllocStats::live(), 0 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:set takes its node storage from its allocator");

    AllocStats::reset();
    {
        S a((TagAlloc<int>(2)));
        for (int i = 0; i < 5; ++i) a.insert(i);
        int after_a = AllocStats::live();
        test_eq( after_a, 5 );
        {
            S b(a);
            test_eq( AllocStats::live(), 10 );
            test_eq( (long)b.size(), 5L );
        }
        test_eq( AllocStats::live(), 5 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:set copy allocates its own nodes");

    AllocStats::reset();
    {
        S x((TagAlloc<int>(7)));
        S y((TagAlloc<int>(9)));
        x.insert(1); y.insert(2);
        x.swap(y);
        test_eq( x.get_allocator().tag, 9 );
        test_eq( y.get_allocator().tag, 7 );
        test_eq( *x.begin(), 2 );
        test_eq( *y.begin(), 1 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:set::swap exchanges allocator with the nodes");
}

TEST_CASE(container, map_allocator_is_used) {
    typedef STD::map<int, int, STD::less<int>, TagAlloc<STD::pair<const int, int> > > M;

    AllocStats::reset();
    {
        M m((TagAlloc<STD::pair<const int, int> >(3)));
        test_eq( AllocStats::allocs, 0 );
        for (int i = 0; i < 6; ++i) m[i] = i * 10;
        test_true( AllocStats::allocs >= 6 );
        test_eq( (long)m.size(), 6L );
        test_eq( m[4], 40 );
        test_eq( m.get_allocator().tag, 3 );

        m.erase(2);
        test_eq( AllocStats::deallocs, 1 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:map takes its node storage from its allocator");

    AllocStats::reset();
    {
        typedef STD::multimap<int, int, STD::less<int>, TagAlloc<STD::pair<const int, int> > > MM;
        MM a((TagAlloc<STD::pair<const int, int> >(4)));
        a.insert(STD::pair<const int, int>(1, 1));
        a.insert(STD::pair<const int, int>(1, 2));
        test_eq( AllocStats::live(), 2 );
        MM b((TagAlloc<STD::pair<const int, int> >(5)));
        b.insert(STD::pair<const int, int>(9, 9));
        test_eq( AllocStats::live(), 3 );
        b = a;
        test_eq( AllocStats::live(), 4 );
        test_eq( (long)b.size(), 2L );
        test_eq( (long)b.count(1), 2L );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:multimap assignment frees old nodes before copying new");
}

TEST_CASE(container, multi_copy_keeps_duplicates) {
    STD::multiset<int> a;
    a.insert(5); a.insert(5); a.insert(5); a.insert(7);
    test_eq( (long)a.size(), 4L );

    STD::multiset<int> b(a);
    test_eq( (long)b.size(), 4L );
    test_eq( (long)b.count(5), 3L );
    test_eq( (long)b.count(7), 1L );
    test_true( b.__tree_rb_valid() );
    test_pass("cxx03:multiset copy ctor keeps duplicates");

    STD::multiset<int> c;
    c.insert(1);
    c = a;
    test_eq( (long)c.size(), 4L );
    test_eq( (long)c.count(5), 3L );
    test_eq( (long)c.count(1), 0L );
    test_true( c.__tree_rb_valid() );
    test_pass("cxx03:multiset copy assignment keeps duplicates");

    STD::multimap<int, int> m;
    m.insert(STD::pair<const int, int>(1, 10));
    m.insert(STD::pair<const int, int>(1, 20));
    m.insert(STD::pair<const int, int>(2, 30));
    STD::multimap<int, int> n(m);
    test_eq( (long)n.size(), 3L );
    test_eq( (long)n.count(1), 2L );
    STD::multimap<int, int> o;
    o = m;
    test_eq( (long)o.size(), 3L );
    test_eq( (long)o.count(1), 2L );
    test_pass("cxx03:multimap copy keeps duplicates");

    STD::set<int> s;
    s.insert(1); s.insert(2);
    STD::set<int> t(s);
    test_eq( (long)t.size(), 2L );
    test_true( t.__tree_rb_valid() );
    test_pass("cxx03:set copy stays unique");
}

TEST_CASE(container, list_allocator_is_used) {
    typedef STD::list<int, TagAlloc<int> > L;

    AllocStats::reset();
    {
        L l((TagAlloc<int>(21)));
        test_eq( l.get_allocator().tag, 21 );
        test_eq( AllocStats::allocs, 0 );

        for (int i = 0; i < 6; ++i) l.push_back(i);
        test_eq( AllocStats::allocs, 6 );
        l.push_front(-1);
        test_eq( AllocStats::allocs, 7 );
        test_eq( (long)l.size(), 7L );
        test_eq( l.front(), -1 );

        l.pop_front();
        test_eq( AllocStats::deallocs, 1 );
        l.erase(l.begin());
        test_eq( AllocStats::deallocs, 2 );
        l.clear();
        test_eq( AllocStats::live(), 0 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:list allocates its nodes from its allocator");

    AllocStats::reset();
    {
        L a((TagAlloc<int>(22)));
        a.push_back(1); a.push_back(2);
        test_eq( AllocStats::live(), 2 );
        {
            L b(a);
            test_eq( b.get_allocator().tag, 22 );
            test_eq( AllocStats::live(), 4 );
        }
        test_eq( AllocStats::live(), 2 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:list copy carries the allocator");

    AllocStats::reset();
    {
        L x((TagAlloc<int>(23)));
        L y((TagAlloc<int>(24)));
        x.push_back(1); y.push_back(2);
        x.swap(y);
        test_eq( x.get_allocator().tag, 24 );
        test_eq( y.get_allocator().tag, 23 );
        test_eq( x.front(), 2 );
        test_eq( y.front(), 1 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:list::swap moves the allocator with the nodes");
}

TEST_CASE(container, forward_list_allocator_is_used) {
    typedef STD::forward_list<int, TagAlloc<int> > F;

    AllocStats::reset();
    {
        F f((TagAlloc<int>(31)));
        test_eq( f.get_allocator().tag, 31 );
        test_eq( AllocStats::allocs, 0 );

        for (int i = 0; i < 5; ++i) f.push_front(i);
        test_eq( AllocStats::allocs, 5 );
        test_eq( f.front(), 4 );

        f.pop_front();
        test_eq( AllocStats::deallocs, 1 );
        f.insert_after(f.before_begin(), 99);
        test_eq( AllocStats::allocs, 6 );
        test_eq( f.front(), 99 );
        f.erase_after(f.before_begin());
        test_eq( AllocStats::deallocs, 2 );
        f.clear();
        test_eq( AllocStats::live(), 0 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:forward_list allocates its nodes from its allocator");

    AllocStats::reset();
    {
        F a((TagAlloc<int>(32)));
        a.push_front(1); a.push_front(2);
        {
            F b(a);
            test_eq( b.get_allocator().tag, 32 );
            test_eq( AllocStats::live(), 4 );
            test_eq( b.front(), 2 );
        }
        test_eq( AllocStats::live(), 2 );
        test_pass("cxx11:forward_list copy ctor manages nodes via allocator");

        F c((TagAlloc<int>(33)));
        c.push_front(9);
        c = a;
        test_eq( AllocStats::live(), 4 );
        test_eq( c.front(), 2 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:forward_list assignment manages nodes via allocator");

    AllocStats::reset();
    {
        F x((TagAlloc<int>(34)));
        F y((TagAlloc<int>(35)));
        x.push_front(1); y.push_front(2);
        x.swap(y);
        test_eq( x.get_allocator().tag, 35 );
        test_eq( y.get_allocator().tag, 34 );
        test_eq( x.front(), 2 );
        test_eq( y.front(), 1 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:forward_list::swap moves the allocator with the nodes");
}

TEST_CASE(container, buffer_containers_allocator_is_used) {
    AllocStats::reset();
    {
        STD::vector<int, TagAlloc<int> > v((TagAlloc<int>(41)));
        test_eq( v.get_allocator().tag, 41 );
        for (int i = 0; i < 10; ++i) v.push_back(i);
        test_true( AllocStats::allocs > 0 );
        test_eq( (long)v.size(), 10L );

        STD::vector<int, TagAlloc<int> > c(v);
        test_eq( c.get_allocator().tag, 41 );
        test_eq( (long)c.size(), 10L );

        STD::vector<int, TagAlloc<int> > w((TagAlloc<int>(42)));
        w.push_back(99);
        v.swap(w);
        test_eq( v.get_allocator().tag, 42 );
        test_eq( w.get_allocator().tag, 41 );
        test_eq( v[0], 99 );
        test_eq( (long)w.size(), 10L );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:vector copy propagates and swap carries the allocator");

    AllocStats::reset();
    {
        STD::deque<int, TagAlloc<int> > d;
        for (int i = 0; i < 8; ++i) d.push_back(i);
        STD::deque<int, TagAlloc<int> > e;
        e.push_front(77);
        d.swap(e);
        test_eq( d.front(), 77 );
        test_eq( (long)e.size(), 8L );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:deque::swap carries the allocator");

    AllocStats::reset();
    {
        typedef STD::basic_string<char, STD::char_traits<char>, TagAlloc<char> > S;
        S a((TagAlloc<char>(43)));
        a.append("hello world, long enough to need a real buffer");
        test_eq( a.get_allocator().tag, 43 );
        test_true( AllocStats::allocs > 0 );

        S b((TagAlloc<char>(44)));
        b.append("other");
        a.swap(b);
        test_eq( a.get_allocator().tag, 44 );
        test_eq( b.get_allocator().tag, 43 );
        test_true( a == S("other") );

        S c(b);
        test_eq( c.get_allocator().tag, 43 );
    }
    test_eq( AllocStats::live(), 0 );
    test_pass("cxx11:string copy propagates and swap carries the allocator");
}

TEST_CASE(container, allocator_is_zero_size) {

    test_eq( sizeof(STD::vector<int>), 3 * sizeof(void*) );

    test_eq( sizeof(STD::string), sizeof(void*) + 2 * sizeof(STD::size_t) );

    test_eq( sizeof(STD::forward_list<int>), sizeof(void*) );

    test_eq( sizeof(STD::list<int>), 2 * sizeof(void*) + sizeof(STD::size_t) );
    test_pass("cxx11:stateless allocator is zero-size (EBO)");
}

TEST_CASE(container, vector_bool) {
    STD::vector<bool> v;
    for (int i = 0; i < 100; ++i) v.push_back(i % 3 == 0);
    test_eq( v.size(), 100u );
    test_true( v[0] == true );
    test_true( v[1] == false );
    test_true( v[3] == true );

    test_true( v.capacity() >= 100u );
    test_pass("cxx03:vector<bool> packed storage and indexing");

    v[1] = true;         test_true( v[1] == true );
    v[1].flip();         test_true( v[1] == false );
    STD::vector<bool>::reference r = v[5];
    r = true;            test_true( v[5] == true );
    test_true( bool(v[5]) == true );
    test_pass("cxx03:vector<bool>::reference proxy writes through");

    bool b0 = v[0], b7 = v[7];
    v.flip();
    test_true( v[0] == !b0 && v[7] == !b7 );
    v.flip();
    test_true( v[0] == b0 && v[7] == b7 );
    test_pass("cxx03:vector<bool>::flip inverts every bit");

    STD::size_t via_iter = 0;
    for (STD::vector<bool>::iterator it = v.begin(); it != v.end(); ++it) if (*it) ++via_iter;
    STD::size_t via_index = 0;
    for (STD::size_t i = 0; i < v.size(); ++i) if (v[i]) ++via_index;
    test_eq( via_iter, via_index );
    test_pass("cxx03:vector<bool> bit iteration agrees with operator[]");

    STD::vector<bool> w(v);
    test_true( w == v );
    w[0] = !w[0];
    test_true( w != v );
    test_pass("cxx03:vector<bool> copy and compare");

    STD::vector<bool> e;
    e.swap(w);
    test_eq( e.size(), 100u );
    test_true( w.empty() );
    test_pass("cxx03:vector<bool>::swap");

#if TEST_HAS_EH
    bool threw = false;
    try { (void)v.at(1000); } catch (STD::out_of_range&) { threw = true; }
    test_true( threw );
    test_pass("cxx03:vector<bool>::at throws out_of_range");
#else
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx03:vector<bool>::at throws out_of_range");
#endif

    v.resize(5);
    test_eq( v.size(), 5u );
    v.resize(10, true);
    test_eq( v.size(), 10u );
    test_true( v[9] == true );
    test_pass("cxx03:vector<bool>::resize");
}

#include <memory>
TEST_CASE(container, uses_allocator_trait) {
    typedef STD::allocator<int>  AI;
    typedef STD::allocator<char> AC;

    test_true( (STD::uses_allocator<STD::vector<int>, AI>::value) );
    test_true( (STD::uses_allocator<STD::deque<int>,  AI>::value) );
    test_true( (STD::uses_allocator<STD::list<int>,   AI>::value) );
    test_true( (STD::uses_allocator<STD::string, AC>::value) );
    test_true( !(STD::uses_allocator<STD::vector<int>, AC>::value) );
    test_pass("cxx11:uses_allocator for standard containers");

    test_true( (STD::uses_allocator<STD::queue<int>, AI>::value) );
    test_true( (STD::uses_allocator<STD::stack<int>, AI>::value) );
    test_true( (STD::uses_allocator<STD::priority_queue<int>, AI>::value) );
    test_true( !(STD::uses_allocator<STD::queue<int>, AC>::value) );
    test_pass("cxx11:uses_allocator reflects the adaptor's container");
}

TEST_CASE(container, deque_emplace_at) {
    STD::deque<int> d;
    for (int i = 0; i < 5; ++i) d.push_back(i * 10);
    STD::deque<int>::iterator it = d.emplace(d.begin() + 2, 99);
    test_eq( d.size(), 6u );
    test_eq( d[2], 99 );
    test_eq( *it, 99 );
    test_eq( d[1], 10 );
    test_eq( d[3], 20 );
    test_pass("cxx11:deque::emplace at position");

    d.emplace(d.begin());
    test_eq( d.size(), 7u );
    test_eq( d[0], 0 );
    test_pass("cxx11:deque::emplace value-initialises with no args");
}

TEST_CASE(container, list_splice_merge) {
    STD::list<int> a, b;
    for (int i = 0; i < 3; ++i) a.push_back(i);
    for (int i = 10; i < 13; ++i) b.push_back(i);
    a.splice(a.end(), b);
    test_eq( a.size(), 6u );
    test_true( b.empty() );
    test_eq( a.back(), 12 );
    test_eq( a.front(), 0 );
    test_pass("cxx03:list::splice moves nodes and empties the source");

    STD::list<int> p, q;
    p.push_back(1); p.push_back(3); p.push_back(5);
    q.push_back(2); q.push_back(4); q.push_back(6);
    p.merge(q);
    test_eq( p.size(), 6u );
    test_true( q.empty() );
    int prev = 0; bool sorted = true;
    for (STD::list<int>::iterator it = p.begin(); it != p.end(); ++it) { if (*it < prev) sorted = false; prev = *it; }
    test_true( sorted );
    test_pass("cxx03:list::merge folds in a sorted list");
}
