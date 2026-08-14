#include "test_cxx.hpp"
#include <unordered_map>
#include <utility>
#include <string>
#include <type_traits>
#include <functional>
#include <memory>

namespace {
typedef STD::unordered_map<int, int> UMap;
UMap::value_type kv(int k, int v) { return UMap::value_type(k, v); }
}

TEST_CASE(unordered_map, index_and_at) {
    UMap m;
    test_true( m.empty() );
    test_pass("cxx11:unordered_map::empty");

    m[1] = 10;
    m[2] = 20;
    test_eq( m.size(), 2u );
    test_pass("cxx11:unordered_map::size");
    test_eq( m[1], 10 );
    test_pass("cxx11:unordered_map::operator[]");
    m[1] = 99;
    test_eq( m[1], 99 );
    int fresh = m[7];
    test_eq( fresh, 0 );
    test_pass("cxx11:unordered_map::operator[] (assign/value-init absent)");

    const UMap& cm = m;
    test_eq( m.at(2), 20 );
    test_eq( cm.at(2), 20 );
    m.at(2) = 22;
    test_eq( m.at(2), 22 );
    test_throw( (void)m.at(999) );
    test_throw( (void)cm.at(999) );
    test_pass("cxx11:unordered_map::at");
}

TEST_CASE(unordered_map, insert_find_erase) {
    UMap m;
    STD::pair<UMap::iterator, bool> r = m.insert(kv(1, 10));
    test_true( r.second );
    test_eq( r.first->second, 10 );
    STD::pair<UMap::iterator, bool> r2 = m.insert(kv(1, 99));
    test_true( !r2.second );
    test_eq( m[1], 10 );
    test_pass("cxx11:unordered_map::insert");

    UMap::value_type raw[] = { kv(2, 20), kv(3, 30) };
    m.insert(raw, raw + 2);
    test_eq( m.size(), 3u );
    test_pass("cxx11:unordered_map::insert (range)");

#if TEST_TARGET_CXX >= 2011
    STD::pair<UMap::iterator, bool> e = m.emplace(4, 40);
    test_true( e.second );
    test_eq( m[4], 40 );
    test_pass("cxx11:unordered_map::emplace");
#else
    m[4] = 40;
    TEST_SKIP1(); TEST_SKIP1();
#endif

    const UMap& cm = m;
    test_true( m.find(2) != m.end() );
    test_eq( m.find(2)->second, 20 );
    test_true( cm.find(3) != cm.end() );
    test_true( m.find(99) == m.end() );
    test_pass("cxx11:unordered_map::find");
    test_eq( m.count(2), 1u );
    test_eq( m.count(99), 0u );
    test_pass("cxx11:unordered_map::count");

    STD::pair<UMap::iterator, UMap::iterator> eq = m.equal_range(3);
    test_true( eq.first != eq.second );
    test_eq( eq.first->first, 3 );
    test_pass("cxx11:unordered_map::equal_range");

    UMap::size_type n = m.erase(2);
    test_eq( (long)n, 1L );
    test_eq( m.erase(99), 0u );
    test_pass("cxx11:unordered_map::erase (key)");
    UMap::iterator it = m.find(3);
    m.erase(it);
    test_true( m.find(3) == m.end() );
    test_pass("cxx11:unordered_map::erase (iterator)");

    m.clear();
    test_true( m.empty() );
    test_pass("cxx11:unordered_map::clear");
}

TEST_CASE(unordered_map, buckets_iterate_swap) {
    UMap::value_type raw[] = { kv(1,1), kv(2,2), kv(3,3), kv(4,4) };
    UMap m(raw, raw + 4);

    test_true( m.bucket_count() >= 1u );
    test_pass("cxx11:unordered_map::bucket_count");
    test_true( m.max_bucket_count() > 0u );
    test_pass("cxx11:unordered_map::max_bucket_count");
    test_true( m.load_factor() >= 0.0f );
    test_pass("cxx11:unordered_map::load_factor");
    m.max_load_factor(3.0f);
    test_true( m.max_load_factor() == 3.0f );
    test_pass("cxx11:unordered_map::max_load_factor");
    m.rehash(64);
    test_true( m.bucket_count() >= 64u );
    test_eq( m.size(), 4u );
    test_pass("cxx11:unordered_map::rehash");
    m.reserve(128);
    test_true( m.bucket_count() >= 4u );
    test_pass("cxx11:unordered_map::reserve");

    UMap::hasher h = m.hash_function();
    test_true( h(3) == h(3) );
    test_pass("cxx11:unordered_map::hash_function");
    UMap::key_equal ke = m.key_eq();
    test_true( ke(3, 3) );
    test_pass("cxx11:unordered_map::key_eq");

    int ksum = 0, vsum = 0;
    for (UMap::iterator i = m.begin(); i != m.end(); ++i) { ksum += i->first; vsum += i->second; i->second += 100; }
    test_eq( ksum, 10 );
    test_eq( vsum, 10 );
    test_eq( m[1], 101 );
    test_pass("cxx11:unordered_map::begin/end");

    int cvsum = 0;
    for (UMap::const_iterator i = m.cbegin(); i != m.cend(); ++i) cvsum += i->second;
    test_eq( cvsum, 410 );
    test_pass("cxx11:unordered_map::cbegin/cend");

    UMap a(raw, raw + 2), b(raw + 2, raw + 4);
    a.swap(b);
    test_true( a.count(3) == 1u );
    test_true( b.count(1) == 1u );
    test_pass("cxx11:unordered_map::swap");
    STD::swap(a, b);
    test_true( a.count(1) == 1u );
    test_pass("cxx11:swap(unordered_map)");

    test_true( m.max_size() > 0u );
    test_pass("cxx11:unordered_map::max_size");
    UMap::allocator_type al = m.get_allocator();
    (void)al;
}

TEST_CASE(unordered_map, multimap) {
    STD::unordered_multimap<int, int> mm;
    mm.insert(STD::unordered_multimap<int,int>::value_type(1, 10));
    mm.insert(STD::unordered_multimap<int,int>::value_type(1, 11));
    mm.insert(STD::unordered_multimap<int,int>::value_type(2, 20));
    test_eq( mm.size(), 3u );
    test_eq( mm.count(1), 2u );
    test_pass("cxx11:unordered_multimap");

    STD::unordered_multimap<int,int>::size_type n = mm.erase(1);
    test_eq( (long)n, 2L );
    test_eq( mm.size(), 1u );
    test_pass("cxx11:unordered_multimap::erase (all equal)");
}

TEST_CASE(unordered_map, string_keys_and_values) {

    STD::unordered_map<STD::string, STD::string> m;
    m["apple"]  = "red";
    m["banana"] = "yellow";
    test_eq( m.size(), 2u );
    test_eq( m["apple"], STD::string("red") );
    test_pass("cxx11:unordered_map::operator[] (string key)");
    m["apple"] = "green";
    test_eq( m["apple"], STD::string("green") );
    test_pass("cxx11:unordered_map::operator[] (string reassign)");
    test_eq( m.at("banana"), STD::string("yellow") );
    test_pass("cxx11:unordered_map::at (string key)");
    test_throw( (void)m.at("missing") );
    test_pass("cxx11:unordered_map::at (string missing throws)");

    test_true( m.find("apple") != m.end() );
    test_true( m.find("kiwi") == m.end() );
    test_pass("cxx11:unordered_map::find (string key)");

    STD::string bigk(300, 'k'), bigv(300, 'v');
    m[bigk] = bigv;
    test_eq( m[bigk].size(), 300u );
    test_pass("cxx11:unordered_map::operator[] (long string key/value)");

    STD::unordered_map<STD::string, STD::string>::size_type n = m.erase("apple");
    test_eq( (long)n, 1L );
    test_true( m.find("apple") == m.end() );
    test_pass("cxx11:unordered_map::erase (string key)");

    STD::unordered_map<STD::string, STD::string> c(m);
    test_eq( c.at("banana"), STD::string("yellow") );
    test_pass("cxx11:unordered_map::unordered_map(const&) (deep copy strings)");
}

TEST_CASE(unordered_map, member_types) {
    test_true( (STD::is_same<UMap::key_type, int>::value) );
    test_pass("cxx11:unordered_map::key_type");
    test_true( (STD::is_same<UMap::mapped_type, int>::value) );
    test_pass("cxx11:unordered_map::mapped_type");
    test_true( (STD::is_same<UMap::value_type, STD::pair<const int, int> >::value) );
    test_pass("cxx11:unordered_map::value_type");
    test_true( (STD::is_same<UMap::hasher, STD::hash<int> >::value) );
    test_pass("cxx11:unordered_map::hasher");
    test_true( (STD::is_same<UMap::key_equal, STD::equal_to<int> >::value) );
    test_pass("cxx11:unordered_map::key_equal");
    test_true( (STD::is_same<UMap::allocator_type, STD::allocator<STD::pair<const int, int> > >::value) );
    test_pass("cxx11:unordered_map::allocator_type");
    { UMap::size_type n = 0; n = (UMap::size_type)(n - 1); test_true( n > 0u ); }
    test_pass("cxx11:unordered_map::size_type");

    UMap m; m[1] = 10; m[2] = 20;
    UMap::iterator it = m.begin();
    test_true( it != m.end() );
    test_pass("cxx11:unordered_map::iterator");
    UMap::const_iterator cit = m.cbegin();
    test_true( cit != m.cend() );
    test_pass("cxx11:unordered_map::const_iterator");
    UMap::size_type bk = m.bucket(1);
    UMap::local_iterator li = m.begin(bk);
    test_true( li != m.end(bk) );
    test_pass("cxx11:unordered_map::local_iterator");
    UMap::const_local_iterator cli = m.cbegin(bk);
    test_true( cli != m.cend(bk) );
    test_pass("cxx11:unordered_map::const_local_iterator");
    test_true( m.begin(bk) != m.end(bk) );
    test_pass("cxx11:unordered_map::begin(size_type)");
    test_true( m.end(bk) == m.end(bk) );
    test_pass("cxx11:unordered_map::end(size_type)");
}

TEST_CASE(unordered_map, ctors_and_equality) {
    UMap::value_type raw[] = { kv(1,10), kv(2,20) };
    UMap fl(raw, raw + 2);
    test_eq( fl.size(), 2u );
    test_pass("cxx11:unordered_map::unordered_map(first, last)");

    STD::hash<int> h; STD::equal_to<int> eq; UMap::allocator_type alc;
    UMap ubc(8, h, eq, alc);
    test_true( ubc.empty() );
    test_pass("cxx11:unordered_map::unordered_map(bucket_count, hash, equal, alloc)");
    UMap ualloc(alc);
    test_true( ualloc.empty() );
    test_pass("cxx11:unordered_map::unordered_map(const Alloc&) family");

    UMap dst; dst = fl;
    test_eq( dst.size(), 2u );
    test_pass("cxx11:unordered_map::operator=(const unordered_map&)");

    UMap e;
    e.emplace_hint(e.end(), 5, 50);
    test_eq( e[5], 50 );
    test_pass("cxx11:unordered_map::emplace_hint");

    UMap a; a[1] = 1; a[2] = 2;
    UMap b; b[2] = 2; b[1] = 1;
    UMap c; c[1] = 1;
    test_true( a == b );
    test_pass("cxx11:operator==(unordered_map)");
    test_true( a != c );
    test_pass("cxx11:operator!=(unordered_map)");
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(unordered_map, node_handle) {
    UMap::value_type raw[] = { kv(1,10), kv(2,20), kv(3,30) };
    UMap m(raw, raw + 3);

    UMap::node_type nh = m.extract(2);
    test_true( !nh.empty() );
    test_eq( nh.key(), 2 );
    test_eq( nh.mapped(), 20 );
    test_eq( m.size(), 2u );
    test_pass("cxx17:unordered_map::extract");

    UMap t;
    UMap::insert_return_type ir = t.insert(STD::move(nh));
    test_true( ir.inserted );
    test_eq( ir.position->first, 2 );
    test_eq( t[2], 20 );
    test_pass("cxx17:unordered_map::insert(node_type)");

    UMap::node_type miss = m.extract(99);
    test_true( miss.empty() );
    test_pass("cxx17:unordered_map::extract (absent -> empty)");
}
#else
TEST_CASE_SKIP(unordered_map, node_handle)
#endif

#if _tst_cplusplus >= 201103L
TEST_CASE(unordered_map, init_list) {
    UMap m = { {1, 10}, {2, 20}, {3, 30} };
    test_eq( m.size(), 3u );
    test_eq( m[2], 20 );
    test_pass("cxx11:unordered_map::unordered_map(initializer_list)");

    m = { {4, 40}, {5, 50} };
    test_eq( m.size(), 2u );
    test_eq( m[5], 50 );
    test_true( m.find(1) == m.end() );
    test_pass("cxx11:unordered_map::operator=(initializer_list)");
}
#else
TEST_CASE_SKIP(unordered_map, init_list)
#endif

TEST_CASE(unordered_map, move_ops) {
#if TEST_TARGET_CXX >= 2011
    {
        UMap::value_type raw[] = { kv(1,10), kv(2,20) };
        UMap a(raw, raw + 2);
        UMap b(STD::move(a));
        test_eq( b.size(), 2u );
        test_eq( b[1], 10 );
    }
    test_pass("cxx11:unordered_map::unordered_map(unordered_map&&)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:unordered_map::unordered_map(unordered_map&&)");
#endif

#if TEST_TARGET_CXX >= 2011
    {
        UMap::value_type raw[] = { kv(7,70), kv(8,80), kv(9,90) };
        UMap a(raw, raw + 3);
        UMap b;
        b = STD::move(a);
        test_eq( b.size(), 3u );
        test_eq( b[9], 90 );
    }
    test_pass("cxx11:unordered_map::operator=(unordered_map&&)");
#else
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:unordered_map::operator=(unordered_map&&)");
#endif
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(unordered_map, cxx17_inserts_and_merge) {
    UMap m;

    STD::pair<UMap::iterator, bool> t1 = m.try_emplace(1, 10);
    test_true( t1.second );
    test_eq( m[1], 10 );
    STD::pair<UMap::iterator, bool> t2 = m.try_emplace(1, 999);
    test_true( !t2.second );
    test_eq( m[1], 10 );
    test_pass("cxx17:unordered_map::try_emplace");

    STD::pair<UMap::iterator, bool> a1 = m.insert_or_assign(2, 20);
    test_true( a1.second );
    STD::pair<UMap::iterator, bool> a2 = m.insert_or_assign(2, 22);
    test_true( !a2.second );
    test_eq( m[2], 22 );
    test_pass("cxx17:unordered_map::insert_or_assign");

    UMap src; src[5] = 50;
    UMap::node_type nh = src.extract(5);
    UMap::insert_return_type ir = m.insert(STD::move(nh));
    test_true( ir.inserted );
    test_eq( ir.position->second, 50 );
    test_true( ir.node.empty() );
    test_pass("cxx17:unordered_map::insert_return_type");

    UMap other; other[100] = 1; other[2] = 999;
    m.merge(other);
    test_true( m.find(100) != m.end() );
    test_eq( m[100], 1 );
    test_true( other.find(2) != other.end() );
    test_true( other.find(100) == other.end() );
    test_pass("cxx17:unordered_map::merge");
}
#else
TEST_CASE_SKIP(unordered_map, cxx17_inserts_and_merge)
#endif

TEST_CASE(unordered_map, ranges_ops) {
#if defined(__cpp_lib_containers_ranges)
    STD::pair<int,int> src[] = { STD::pair<int,int>(1,10), STD::pair<int,int>(2,20) };
    {
        UMap m(STD::from_range, src);
        test_eq( m.size(), 2u );
        test_eq( m[2], 20 );
        test_pass("cxx23:unordered_map::unordered_map(from_range_t, R&&)");
    }
    {
        UMap m; m[9] = 90;
        m.insert_range(src);
        test_eq( m.size(), 3u );
        test_eq( m[1], 10 );
        test_pass("cxx23:unordered_map::insert_range");
    }
#else
    test_skip("cxx23:unordered_map::unordered_map(from_range_t, R&&)");
    test_skip("cxx23:unordered_map::insert_range");
#endif
}

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
TEST_CASE(unordered_map, contains_erase_if) {
    UMap::value_type raw[] = { kv(1,1), kv(2,2), kv(3,3), kv(4,4) };
    UMap m(raw, raw + 4);
    test_true(  m.contains(2) );
    test_true( !m.contains(9) );
    test_pass("cxx20:unordered_map::contains");

    struct EvenKey { bool operator()(const UMap::value_type& p) const { return p.first % 2 == 0; } };
    UMap::size_type n = STD::erase_if(m, EvenKey());
    test_eq( (long)n, 2L );
    test_eq( m.size(), 2u );
    test_pass("cxx20:erase_if(unordered_map)");
}
#else
TEST_CASE_SKIP(unordered_map, contains_erase_if)
#endif


TEST_CASE(unordered_map, member_types_cxx11) {
    typedef STD::unordered_map<int, char> M;
    M m;
    m.insert(STD::pair<const int, char>(1, 'a'));
    const M& cm = m;

    test_true(( STD::is_same<M::reference, STD::pair<const int, char>&>::value ));
    M::reference r = *m.begin();
    r.second = 'b';
    test_eq( cm.begin()->second, 'b' );
    test_pass("cxx11:unordered_map::reference");

    M::const_reference cr = *cm.begin();
    test_eq( cr.second, 'b' );
    test_true(( STD::is_same<M::const_reference,
                             const STD::pair<const int, char>&>::value ));
    test_pass("cxx11:unordered_map::const_reference");

    M::pointer p = &*m.begin();
    test_eq( p->first, 1 );
    test_pass("cxx11:unordered_map::pointer");

    M::const_pointer cp = &*cm.begin();
    test_eq( cp->first, 1 );
    test_pass("cxx11:unordered_map::const_pointer");

    test_true( STD::is_signed<M::difference_type>::value );
    M::difference_type d = 0;
    for (M::const_iterator i = cm.begin(); i != cm.end(); ++i) ++d;
    test_eq( (long)d, 1L );
    test_pass("cxx11:unordered_map::difference_type");
}


TEST_CASE(unordered_map, multimap_member_types_cxx11) {
    typedef STD::unordered_multimap<int, char> M;
    M m;
    m.insert(STD::pair<const int, char>(1, 'a'));
    const M& cm = m;

    test_true(( STD::is_same<M::reference, STD::pair<const int, char>&>::value ));
    M::reference r = *m.begin();
    r.second = 'b';
    test_eq( cm.begin()->second, 'b' );
    test_pass("cxx11:unordered_multimap::reference");

    M::const_reference cr = *cm.begin();
    test_eq( cr.second, 'b' );
    test_true(( STD::is_same<M::const_reference,
                             const STD::pair<const int, char>&>::value ));
    test_pass("cxx11:unordered_multimap::const_reference");

    M::pointer p = &*m.begin();
    test_eq( p->first, 1 );
    test_pass("cxx11:unordered_multimap::pointer");

    M::const_pointer cp = &*cm.begin();
    test_eq( cp->first, 1 );
    test_pass("cxx11:unordered_multimap::const_pointer");

    test_true( STD::is_signed<M::difference_type>::value );
    M::difference_type d = 0;
    for (M::const_iterator i = cm.begin(); i != cm.end(); ++i) ++d;
    test_eq( (long)d, 1L );
    test_pass("cxx11:unordered_multimap::difference_type");
}


TEST_CASE(unordered_map, lookup_cxx26) {
    TEST_NOTE("unordered_map::lookup is C++26; no library here defines it");
}

TEST_CASE(unordered_map, feature_test_macros) {
#if defined(__cpp_lib_unordered_map_try_emplace) && (__cpp_lib_unordered_map_try_emplace) >= 201411L
    test_true( (__cpp_lib_unordered_map_try_emplace) >= 201411L );
    test_pass("cxx17:__cpp_lib_unordered_map_try_emplace=201411L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx17:__cpp_lib_unordered_map_try_emplace=201411L");
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
#if defined(__cpp_lib_constexpr_unordered_map) && (__cpp_lib_constexpr_unordered_map) >= 202502L
    test_true( (__cpp_lib_constexpr_unordered_map) >= 202502L );
    test_pass("cxx26:__cpp_lib_constexpr_unordered_map=202502L");
    {
        struct K {
            static constexpr int run() {
                STD::unordered_map<int, int> m;
                m.insert(STD::make_pair(2, 20));
                m[1] = 10;
                m.erase(2);
                return (int)(m.at(1) + m.size() + m.count(2));
            }
        };
        static_assert(K::run() == 11, "unordered_map operations in a constant expression");
        test_eq( K::run(), 11 );
    }
    test_pass("cxx26:constexpr unordered_map");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_constexpr_unordered_map=202502L");
    test_skip("cxx26:constexpr unordered_map");
#endif
}

static int g_um_live = 0;

struct UmLive {
    STD::string s_;
    UmLive() { ++g_um_live; }
    UmLive(const char* s) : s_(s) { ++g_um_live; }
    UmLive(const UmLive& o) : s_(o.s_) { ++g_um_live; }
    UmLive& operator=(const UmLive& o) { s_ = o.s_; return *this; }
    ~UmLive() { --g_um_live; }
};

static STD::string um_live_key(int i) {
    STD::string t("k");
    t += (char)('0' + i);
    t += "_long_enough_to_leave_the_sso_buffer";
    return t;
}

TEST_CASE(unordered_map, class_values_lifetime) {
    g_um_live = 0;
    {
        STD::unordered_map<STD::string, UmLive> m;
        for (int i = 0; i < 8; ++i)
            m.insert(STD::pair<STD::string, UmLive>(um_live_key(i),
                                                    UmLive("v_long_enough_to_leave_sso")));
        test_eq( (int)m.size(), 8 );
        test_eq( g_um_live, 8 );

        STD::unordered_map<STD::string, UmLive> c(m);
        test_eq( g_um_live, 16 );

        m.erase(m.begin());
        test_eq( (int)m.size(), 7 );
        test_eq( g_um_live, 15 );

        m.rehash(64);
        test_eq( g_um_live, 15 );

        STD::unordered_map<STD::string, UmLive> e;
        m.swap(e);
        test_true( m.empty() );
        test_eq( g_um_live, 15 );

        e.clear();
        test_eq( g_um_live, 8 );

        m[um_live_key(9)] = UmLive("w_long_enough_to_leave_sso");
        test_eq( (int)m.size(), 1 );
        test_eq( g_um_live, 9 );

        c = e;
        test_eq( g_um_live, 1 );
    }
    test_eq( g_um_live, 0 );
}

TEST_CASE(unordered_map, multimap_insert_forms) {
    typedef STD::unordered_multimap<int, int> MM;
    typedef MM::value_type V;

    {   MM m;
        m.insert(V(5, 50));
        MM::iterator a = m.insert(m.end(), V(9, 90));
        test_eq( a->first, 9 );
        test_eq( a->second, 90 );
        MM::iterator b = m.insert(m.begin(), V(5, 51));
        test_eq( b->first, 5 );
        test_eq( (long)m.size(), 3L );
        test_eq( (long)m.count(5), 2L ); }
    test_pass("cxx11:unordered_multimap::insert (hint)");

    {   V src[4] = { V(3, 30), V(1, 10), V(3, 31), V(2, 20) };
        MM m;
        m.insert(src, src);
        test_true( m.empty() );
        m.insert(src, src + 4);
        test_eq( (long)m.size(), 4L );
        test_eq( (long)m.count(3), 2L );
        m.insert(src, src + 1);
        test_eq( (long)m.count(3), 3L ); }
    test_pass("cxx11:unordered_multimap::insert (range)");

    {   STD::unordered_map<int, int> u;
        float before = u.max_load_factor();
        test_true( before > 0.0f );
        u.max_load_factor(4.0f);
        test_eq( u.max_load_factor(), 4.0f );
        u.max_load_factor(before);
        test_eq( u.max_load_factor(), before ); }
    test_pass("cxx11:unordered_map::max_load_factor(float)");
}
