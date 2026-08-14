#include "test_cxx.hpp"

#include <unordered_set>
#include <unordered_map>
#include <string>

TEST_CASE(unordered_set, basic) {
    STD::unordered_set<int> s;
    test_true( s.empty() );
    test_pass("cxx11:unordered_set");
    s.insert(3); s.insert(1); s.insert(2);
    STD::pair<STD::unordered_set<int>::iterator, bool> r = s.insert(2);
    test_true( !r.second );
    test_pass("cxx11:unordered_set::insert");
    test_eq( s.size(), 3u );
    test_pass("cxx11:unordered_set::size");
    test_true( !s.empty() );
    test_pass("cxx11:unordered_set::empty");
    test_eq( s.count(2), 1u );
    test_eq( s.count(9), 0u );
    test_pass("cxx11:unordered_set::count");
    test_true( s.find(1) != s.end() );
    test_true( s.find(9) == s.end() );
    test_pass("cxx11:unordered_set::find");

    int total = 0;
    for (STD::unordered_set<int>::iterator it = s.begin(); it != s.end(); ++it) total += *it;
    test_eq( total, 6 );
    test_pass("cxx11:unordered_set::begin");

    STD::size_t er = s.erase(2);
    test_eq( er, 1u );
    test_eq( s.count(2), 0u );
    test_pass("cxx11:unordered_set::erase");
    test_true( s.bucket_count() > 0u );
    test_pass("cxx11:unordered_set::bucket_count");
    s.clear();
    test_true( s.empty() );
    test_pass("cxx11:unordered_set::clear");
}

TEST_CASE(unordered_set, grow_string) {
    STD::unordered_set<STD::string> s;
    s.insert("alpha"); s.insert("beta"); s.insert("gamma");
    test_eq( s.size(), 3u );
    test_true( s.find("beta") != s.end() );
    test_pass("cxx11:unordered_set#string");

    STD::unordered_set<int> big;
    for (int i = 0; i < 1000; ++i) big.insert(i);
    test_eq( big.size(), 1000u );
    for (int i = 0; i < 1000; ++i) test_true( big.find(i) != big.end() );
    test_true( big.load_factor() <= big.max_load_factor() );
    test_pass("cxx11:unordered_set::load_factor");

    STD::unordered_multiset<int> ms;
    ms.insert(5); ms.insert(5); ms.insert(5);
    test_eq( ms.count(5), 3u );
    test_pass("cxx11:unordered_multiset::count");
    test_eq( ms.size(), 3u );
    test_pass("cxx11:unordered_multiset::insert");
}

TEST_CASE(unordered_set, members) {
    STD::unordered_set<int> s;
    STD::pair<STD::unordered_set<int>::iterator, bool> e = s.emplace(7);
    test_true( e.second );
    test_eq( s.count(7), 1u );
    test_pass("cxx11:unordered_set::emplace");
#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( s.contains(7) );
#else
    test_true( s.count(7) == 1u );
#endif
    test_pass("cxx20:unordered_set::contains");

    s.max_load_factor(0.5f);
    test_true( s.max_load_factor() == 0.5f );
    test_pass("cxx11:unordered_set::max_load_factor");
    s.reserve(100);
    test_true( s.bucket_count() >= 1u );
    test_pass("cxx11:unordered_set::reserve");
    s.rehash(64);
    test_true( s.bucket_count() >= 1u );
    test_pass("cxx11:unordered_set::rehash");
    (void)s.hash_function();
    test_true( true );
    test_pass("cxx11:unordered_set::hash_function");
    (void)s.key_eq();
    test_true( true );
    test_pass("cxx11:unordered_set::key_eq");
    test_true( s.max_size() > 0u );
    test_pass("cxx11:unordered_set::max_size");

    STD::unordered_set<int> a, b;
    a.insert(1); b.insert(2);
    a.swap(b);
#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( a.contains(2) && b.contains(1) );
#else
    test_true( a.count(2) == 1u && b.count(1) == 1u );
#endif
    test_pass("cxx11:unordered_set::swap");
    STD::swap(a, b);
#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( a.contains(1) );
#else
    test_true( a.count(1) == 1u );
#endif
    test_pass("cxx11:swap(unordered_set)");
}

TEST_CASE(unordered_map, basic) {
    STD::unordered_map<STD::string, int> m;
    test_true( m.empty() );
    test_pass("cxx11:unordered_map::empty");
    m["one"] = 1;
    m["two"] = 2;
    m.insert(STD::make_pair(STD::string("three"), 3));
    test_eq( m.size(), 3u );
    test_pass("cxx11:unordered_map::insert");
    test_eq( m["two"], 2 );
    test_pass("cxx11:unordered_map::operator[]");
    test_eq( m.at("one"), 1 );
    test_pass("cxx11:unordered_map::at");
    test_eq( m.count("three"), 1u );
    test_eq( m.count("zzz"), 0u );
    test_pass("cxx11:unordered_map::count");

    STD::unordered_map<STD::string, int>::iterator it = m.find("three");
    test_true( it != m.end() );
    test_eq( it->second, 3 );
    test_pass("cxx11:unordered_map::find");

    m["one"] = 11;
    test_eq( m["one"], 11 );

    int sum = 0;
    for (STD::unordered_map<STD::string, int>::iterator i = m.begin(); i != m.end(); ++i) sum += i->second;
    test_eq( sum, 11 + 2 + 3 );
    test_pass("cxx11:unordered_map::begin");

    STD::size_t er = m.erase("two");
    test_eq( er, 1u );
    test_eq( m.count("two"), 0u );
    test_pass("cxx11:unordered_map::erase");
    test_throw( (void)m.at("missing") );
    test_pass("cxx11:unordered_map::at (throws)");
}

TEST_CASE(unordered_map, grow_multi) {
    STD::unordered_map<int, int> m;
    for (int i = 0; i < 500; ++i) m[i] = i * 2;
    test_eq( m.size(), 500u );
    for (int i = 0; i < 500; ++i) test_eq( m[i], i * 2 );
    test_pass("cxx11:unordered_map");

    STD::unordered_multimap<int, int> mm;
    mm.insert(STD::make_pair(1, 10));
    mm.insert(STD::make_pair(1, 20));
    mm.insert(STD::make_pair(2, 30));
    test_eq( mm.count(1), 2u );
    test_pass("cxx11:unordered_multimap::count");
    test_eq( mm.size(), 3u );
    test_pass("cxx11:unordered_multimap::insert");
}

TEST_CASE(unordered_map, equal_range_bucket) {
    STD::unordered_map<int, int> m;
    m[1] = 10; m[2] = 20;
    STD::pair<STD::unordered_map<int, int>::iterator, STD::unordered_map<int, int>::iterator> r = m.equal_range(1);
    int cnt = 0; int val = 0;
    for (STD::unordered_map<int, int>::iterator it = r.first; it != r.second; ++it) { ++cnt; val = it->second; }
    test_eq( cnt, 1 );
    test_eq( val, 10 );
    test_true( m.equal_range(99).first == m.equal_range(99).second );
    test_pass("cxx11:unordered_map::equal_range");

    test_true( m.bucket(1) < m.bucket_count() );
    test_pass("cxx11:unordered_map::bucket");
    test_true( m.bucket_size(m.bucket(1)) >= 1u );
    test_pass("cxx11:unordered_map::bucket_size");
    test_true( m.max_bucket_count() >= m.bucket_count() );
    test_pass("cxx11:unordered_map::max_bucket_count");

    STD::unordered_multimap<int, int> mm;
    mm.insert(STD::make_pair(5, 1));
    mm.insert(STD::make_pair(6, 99));
    mm.insert(STD::make_pair(5, 2));
    mm.insert(STD::make_pair(5, 3));
    STD::pair<STD::unordered_multimap<int, int>::iterator, STD::unordered_multimap<int, int>::iterator> rr = mm.equal_range(5);
    int c = 0, sum = 0;
    for (STD::unordered_multimap<int, int>::iterator it = rr.first; it != rr.second; ++it) { ++c; sum += it->second; }
    test_eq( c, 3 );
    test_eq( sum, 6 );
    test_pass("cxx11:unordered_multimap::equal_range");
}

TEST_CASE(unordered_set, erase_iter_citer) {
    STD::unordered_set<int> s;
    s.insert(1); s.insert(2); s.insert(3);
    STD::unordered_set<int>::iterator it = s.find(2);
    s.erase(it);
    test_eq( s.size(), 2u );
#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( !s.contains(2) );
#else
    test_true( s.count(2) == 0u );
#endif
    test_pass("cxx11:unordered_set::erase (iterator)");

    int total = 0;
    for (STD::unordered_set<int>::const_iterator ci = s.cbegin(); ci != s.cend(); ++ci) total += *ci;
    test_eq( total, 4 );
    test_pass("cxx11:unordered_set::cbegin");
    test_true( s.cbegin() != s.cend() );
    test_pass("cxx11:unordered_set::cend");
}

TEST_CASE(unordered_map, members) {
    STD::unordered_map<int, int> m;
    m.emplace(STD::make_pair(1, 10));
#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( m.contains(1) );
#else
    test_true( m.count(1) == 1u );
#endif
    test_pass("cxx11:unordered_map::emplace");
    #if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( !m.contains(999) );
    #else
    test_true( m.count(999) == 0u );
    #endif
    test_pass("cxx20:unordered_map::contains");
    const STD::unordered_map<int, int>& cr = m;
    test_eq( cr.at(1), 10 );
    test_pass("cxx11:unordered_map::at (const)");
    test_true( cr.find(1) != cr.end() );
    test_pass("cxx11:unordered_map::find (const)");

    int sum = 0;
    for (STD::unordered_map<int, int>::const_iterator i = cr.begin(); i != cr.end(); ++i) sum += i->second;
    test_eq( sum, 10 );
    test_pass("cxx11:unordered_map::begin (const)");

    m.max_load_factor(0.75f);
    test_true( m.max_load_factor() == 0.75f );
    test_pass("cxx11:unordered_map::max_load_factor");
    test_true( m.load_factor() >= 0.f );
    test_pass("cxx11:unordered_map::load_factor");
    m.reserve(50);
    test_true( m.bucket_count() >= 1u );
    test_pass("cxx11:unordered_map::reserve");
    m.rehash(32);
    test_true( m.bucket_count() >= 1u );
    test_pass("cxx11:unordered_map::rehash");
    (void)m.hash_function();
    test_true( true );
    test_pass("cxx11:unordered_map::hash_function");
    (void)m.key_eq();
    test_true( true );
    test_pass("cxx11:unordered_map::key_eq");
    test_true( m.max_size() > 0u );
    test_pass("cxx11:unordered_map::max_size");

    STD::unordered_map<int, int> a, b;
    a[1] = 1; b[2] = 2;
    a.swap(b);
    #if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( a.contains(2) && b.contains(1) );
    #else
    test_true( a.count(2) == 1u && b.count(1) == 1u );
    #endif
    test_pass("cxx11:unordered_map::swap");
    STD::swap(a, b);
    #if _TST_HAS_CXX20_LIB_MEMBERS
    test_true( a.contains(1) );
    #else
    test_true( a.count(1) != 0u );
    #endif
    test_pass("cxx11:swap(unordered_map)");
}

namespace {
struct UnordStats {
    static int allocs;
    static int deallocs;
    static void reset() { allocs = 0; deallocs = 0; }
    static int live() { return allocs - deallocs; }
};
int UnordStats::allocs = 0;
int UnordStats::deallocs = 0;

template <class T>
struct CntAlloc : STD::allocator<T> {
    typedef STD::size_t size_type;
    typedef T*          pointer;
    template <class U> struct rebind { typedef CntAlloc<U> other; };

    typedef STD::true_type propagate_on_container_swap;

    int tag;
    CntAlloc() : tag(0) {}
    explicit CntAlloc(int t) : tag(t) {}
    template <class U> CntAlloc(const CntAlloc<U>& o) : tag(o.tag) {}

    pointer allocate(size_type n, const void* = 0) {
        ++UnordStats::allocs;
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }
    void deallocate(pointer p, size_type) {
        ++UnordStats::deallocs;
        ::operator delete(static_cast<void*>(p));
    }
};
}

TEST_CASE(unordered_set, allocator_is_used) {
    typedef STD::unordered_set<int, STD::hash<int>, STD::equal_to<int>, CntAlloc<int> > S;

    UnordStats::reset();
    {
        S s((CntAlloc<int>(5)));
        test_eq( s.get_allocator().tag, 5 );
        test_pass("cxx11:unordered_set::get_allocator");
        TEST_SKIP_GCC();
        test_true( UnordStats::allocs > 0 );
        int after_ctor = UnordStats::allocs;

        for (int i = 0; i < 20; ++i) s.insert(i);
        test_eq( (long)s.size(), 20L );
        test_true( UnordStats::allocs >= after_ctor + 20 );

        s.erase(3);
        test_eq( (long)s.size(), 19L );
        test_true( UnordStats::deallocs > 0 );

        s.clear();
        test_eq( (long)s.size(), 0L );
    }
    test_eq( UnordStats::live(), 0 );

    UnordStats::reset();
    {
        S a((CntAlloc<int>(6)));
        for (int i = 0; i < 4; ++i) a.insert(i);
        int with_a = UnordStats::live();
        {
            S b(a);
            test_eq( (long)b.size(), 4L );
            test_true( UnordStats::live() > with_a );
        }
        test_eq( UnordStats::live(), with_a );
    }
    test_eq( UnordStats::live(), 0 );
    test_pass("cxx11:unordered_set::unordered_set(const unordered_set&)");

    UnordStats::reset();
    {
        S x((CntAlloc<int>(7)));
        S y((CntAlloc<int>(9)));
        x.insert(1); y.insert(2);
        x.swap(y);
        TEST_SKIP_VC120("MSVC 12's unordered swap does not exchange the allocators");
        test_eq( x.get_allocator().tag, 9 );
        TEST_SKIP_VC120("MSVC 12's unordered swap does not exchange the allocators");
        test_eq( y.get_allocator().tag, 7 );
        test_true( x.find(2) != x.end() );
        test_true( y.find(1) != y.end() );
    }
    test_eq( UnordStats::live(), 0 );
    test_pass("cxx11:unordered_set#allocator_aware");
}

TEST_CASE(unordered_map, allocator_is_used) {
    typedef CntAlloc<STD::pair<const int, int> > A;
    typedef STD::unordered_map<int, int, STD::hash<int>, STD::equal_to<int>, A> M;

    UnordStats::reset();
    {
        M m((A(11)));
        test_eq( m.get_allocator().tag, 11 );
        test_pass("cxx11:unordered_map::get_allocator");
        for (int i = 0; i < 10; ++i) m[i] = i * 3;
        test_eq( (long)m.size(), 10L );
        test_eq( m[7], 21 );
        m.erase(4);
        test_eq( (long)m.size(), 9L );
    }
    test_eq( UnordStats::live(), 0 );
    test_pass("cxx11:unordered_map#allocator_aware");

    UnordStats::reset();
    {
        typedef CntAlloc<STD::pair<const int, int> > A2;
        typedef STD::unordered_multimap<int, int, STD::hash<int>, STD::equal_to<int>, A2> MM;
        MM a((A2(1)));
        a.insert(STD::pair<const int, int>(1, 1));
        a.insert(STD::pair<const int, int>(1, 2));
        MM b((A2(2)));
        b.insert(STD::pair<const int, int>(9, 9));
        b = a;
        test_eq( (long)b.size(), 2L );
        test_eq( (long)b.count(1), 2L );
        test_eq( (long)b.count(9), 0L );
    }
    test_eq( UnordStats::live(), 0 );
    test_pass("cxx11:unordered_multimap::operator=");
}

TEST_CASE(unordered_set, multiset_copy) {
    STD::unordered_multiset<int> ms;
    ms.insert(5); ms.insert(5); ms.insert(7);
    STD::unordered_multiset<int> ms2(ms);
    test_eq( (long)ms2.size(), 3L );
    test_eq( (long)ms2.count(5), 2L );
    test_pass("cxx11:unordered_multiset::unordered_multiset(const unordered_multiset&)");
    STD::unordered_multiset<int> ms3;
    ms3 = ms;
    test_eq( (long)ms3.size(), 3L );
    test_eq( (long)ms3.count(5), 2L );
    test_pass("cxx11:unordered_multiset::operator=(const unordered_multiset&)");
}
