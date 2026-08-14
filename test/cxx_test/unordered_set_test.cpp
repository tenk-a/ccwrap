#include "test_cxx.hpp"
#include <unordered_set>
#include <utility>
#include <string>
#include <vector>
#include <type_traits>

TEST_CASE(unordered_set, construct_insert) {
    STD::unordered_set<int> a;
    test_true( a.empty() );
    test_pass("cxx11:unordered_set::empty");
    test_eq( a.size(), 0u );
    test_pass("cxx11:unordered_set::size");

    STD::pair<STD::unordered_set<int>::iterator, bool> r = a.insert(5);
    test_true( r.second );
    test_eq( *r.first, 5 );
    STD::pair<STD::unordered_set<int>::iterator, bool> r2 = a.insert(5);
    test_true( !r2.second );
    test_eq( a.size(), 1u );
    test_pass("cxx11:unordered_set::insert");

    int raw[] = { 1, 2, 3, 2, 1 };
    STD::unordered_set<int> b(raw, raw + 5);
    test_eq( b.size(), 3u );
    test_pass("cxx11:unordered_set::unordered_set(first,last)");

    a.insert(raw, raw + 5);
    test_eq( a.size(), 4u );
    test_pass("cxx11:unordered_set::insert (range)");

    STD::unordered_set<int> c(b);
    test_eq( c.size(), 3u );
    test_pass("cxx11:unordered_set::unordered_set(const&)");
    STD::unordered_set<int> d;
    d = b;
    test_eq( d.size(), 3u );
    test_pass("cxx11:unordered_set::operator=(const&)");

#if TEST_TARGET_CXX >= 2011
    STD::pair<STD::unordered_set<int>::iterator, bool> e = a.emplace(99);
    test_true( e.second );
    test_true( a.count(99) == 1u );
    test_pass("cxx11:unordered_set::emplace");
#else
    a.insert(99);
    TEST_SKIP1(); TEST_SKIP1();
#endif
}

TEST_CASE(unordered_set, lookup_erase) {
    int raw[] = { 10, 20, 30, 40 };
    STD::unordered_set<int> s(raw, raw + 4);
    const STD::unordered_set<int>& cs = s;

    test_true( s.find(20) != s.end() );
    test_true( s.find(25) == s.end() );
    test_true( cs.find(30) != cs.end() );
    test_pass("cxx11:unordered_set::find");
    test_eq( s.count(20), 1u );
    test_eq( s.count(99), 0u );
    test_pass("cxx11:unordered_set::count");

    STD::pair<STD::unordered_set<int>::iterator, STD::unordered_set<int>::iterator> eq
        = s.equal_range(30);
    test_true( eq.first != eq.second );
    test_eq( *eq.first, 30 );
    test_pass("cxx11:unordered_set::equal_range");

    STD::unordered_set<int>::size_type n = s.erase(20);
    test_eq( (long)n, 1L );
    test_eq( s.erase(99), 0u );
    test_eq( s.size(), 3u );
    test_pass("cxx11:unordered_set::erase (key)");

    STD::unordered_set<int>::iterator it = s.find(10);
    s.erase(it);
    test_true( s.find(10) == s.end() );
    test_pass("cxx11:unordered_set::erase (iterator)");

    s.clear();
    test_true( s.empty() );
    test_pass("cxx11:unordered_set::clear");
}

TEST_CASE(unordered_set, buckets_and_hash_policy) {
    int raw[] = { 1, 2, 3, 4, 5 };
    STD::unordered_set<int> s(raw, raw + 5);

    test_true( s.bucket_count() >= 1u );
    test_pass("cxx11:unordered_set::bucket_count");
    test_true( s.load_factor() >= 0.0f );
    test_pass("cxx11:unordered_set::load_factor");
    test_true( s.max_load_factor() > 0.0f );

    s.max_load_factor(2.0f);
    test_true( s.max_load_factor() == 2.0f );
    test_pass("cxx11:unordered_set::max_load_factor");

    s.rehash(64);
    test_true( s.bucket_count() >= 64u );
    test_eq( s.size(), 5u );
    test_true( s.count(3) == 1u );
    test_pass("cxx11:unordered_set::rehash");

    s.reserve(100);
    test_true( s.bucket_count() >= 1u );
    test_pass("cxx11:unordered_set::reserve");

    STD::unordered_set<int>::hasher h = s.hash_function();
    test_true( h(3) == h(3) );
    test_pass("cxx11:unordered_set::hash_function");
    STD::unordered_set<int>::key_equal ke = s.key_eq();
    test_true( ke(3, 3) );
    test_true( !ke(3, 4) );
    test_pass("cxx11:unordered_set::key_eq");

    test_true( s.max_size() > 0u );
    test_pass("cxx11:unordered_set::max_size");
    STD::unordered_set<int>::allocator_type al = s.get_allocator();
    (void)al;
}

TEST_CASE(unordered_set, iterators_and_swap) {
    int raw[] = { 1, 2, 3, 4 };
    STD::unordered_set<int> s(raw, raw + 4);

    int sum = 0, cnt = 0;
    for (STD::unordered_set<int>::iterator it = s.begin(); it != s.end(); ++it) { sum += *it; ++cnt; }
    test_eq( sum, 10 );
    test_eq( cnt, 4 );
    test_pass("cxx11:unordered_set::begin/end");

    int csum = 0;
    for (STD::unordered_set<int>::const_iterator it = s.cbegin(); it != s.cend(); ++it) csum += *it;
    test_eq( csum, 10 );
    test_pass("cxx11:unordered_set::cbegin/cend");

    STD::unordered_set<int> a(raw, raw + 2), b(raw + 2, raw + 4);
    a.swap(b);
    test_true( a.count(3) == 1u );
    test_true( b.count(1) == 1u );
    test_pass("cxx11:unordered_set::swap");
    STD::swap(a, b);
    test_true( a.count(1) == 1u );
    test_pass("cxx11:swap(unordered_set)");
}

TEST_CASE(unordered_set, multiset) {
    int raw[] = { 1, 2, 2, 3, 2 };
    STD::unordered_multiset<int> m(raw, raw + 5);
    test_eq( m.size(), 5u );
    test_eq( m.count(2), 3u );
    test_pass("cxx11:unordered_multiset");

    m.insert(2);
    test_eq( m.count(2), 4u );
    test_pass("cxx11:unordered_multiset::insert");

    STD::unordered_multiset<int>::size_type n = m.erase(2);
    test_eq( (long)n, 4L );
    test_eq( m.count(2), 0u );
    test_eq( m.size(), 2u );
    test_pass("cxx11:unordered_multiset::erase (all equal)");
}

TEST_CASE(unordered_set, string_elements) {

    STD::unordered_set<STD::string> s;
    s.insert("apple");
    s.insert("banana");
    s.insert("apple");
    test_eq( s.size(), 2u );
    test_pass("cxx11:unordered_set::insert (string key)");

    test_true( s.find("banana") != s.end() );
    test_true( s.find("cherry") == s.end() );
    test_true( s.count("apple") == 1u );
    test_pass("cxx11:unordered_set::find (string key)");

    STD::string big(300, 'k');
    s.insert(big);
    test_true( s.count(big) == 1u );
    test_pass("cxx11:unordered_set::insert (long string key)");

    STD::unordered_set<STD::string>::size_type n = s.erase("apple");
    test_eq( (long)n, 1L );
    test_true( s.find("apple") == s.end() );
    test_pass("cxx11:unordered_set::erase (string key)");

    STD::unordered_set<STD::string> c(s);
    test_true( c.count("banana") == 1u );
    test_pass("cxx11:unordered_set::unordered_set(const&) (deep copy strings)");

    STD::size_t total = 0, cnt = 0;
    for (STD::unordered_set<STD::string>::iterator it = s.begin(); it != s.end(); ++it) { total += it->size(); ++cnt; }
    test_eq( cnt, s.size() );
    test_true( total > 0u );
    test_pass("cxx11:unordered_set::begin/end (string)");
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(unordered_set, node_handle) {
    int raw[] = { 1, 2, 3 };
    STD::unordered_set<int> s(raw, raw + 3);

    STD::unordered_set<int>::node_type nh = s.extract(2);
    test_true( !nh.empty() );
    test_eq( nh.value(), 2 );
    test_eq( s.size(), 2u );
    test_pass("cxx17:unordered_set::extract");

    STD::unordered_set<int> t;
    STD::unordered_set<int>::insert_return_type ir = t.insert(STD::move(nh));
    test_true( ir.inserted );
    test_eq( *ir.position, 2 );
    test_pass("cxx17:unordered_set::insert(node_type)");

    STD::unordered_set<int>::node_type miss = s.extract(99);
    test_true( miss.empty() );
    test_pass("cxx17:unordered_set::extract (absent -> empty)");
}
#else
TEST_CASE_SKIP(unordered_set, node_handle)
#endif

#if TEST_TARGET_CXX >= 2020 && _TST_HAS_CXX20_LIB_MEMBERS
TEST_CASE(unordered_set, contains_erase_if) {
    int raw[] = { 1, 2, 3, 4, 5, 6 };
    STD::unordered_set<int> s(raw, raw + 6);
    test_true(  s.contains(3) );
    test_true( !s.contains(9) );
    test_pass("cxx20:unordered_set::contains");

    struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
    STD::unordered_set<int>::size_type n = STD::erase_if(s, IsEven());
    test_eq( (long)n, 3L );
    test_eq( s.size(), 3u );
    test_pass("cxx20:erase_if(unordered_set)");
}
#else
TEST_CASE_SKIP(unordered_set, contains_erase_if)
#endif

TEST_CASE(unordered_set, member_types) {
    typedef STD::unordered_set<int> US;
    test_true(( STD::is_same<US::key_type, int>::value ));
    test_pass("cxx11:unordered_set::key_type");
    test_true(( STD::is_same<US::value_type, int>::value ));
    test_pass("cxx11:unordered_set::value_type");
    test_true(( STD::is_same<US::hasher, STD::hash<int> >::value ));
    test_pass("cxx11:unordered_set::hasher");
    test_true(( STD::is_same<US::key_equal, STD::equal_to<int> >::value ));
    test_pass("cxx11:unordered_set::key_equal");
    test_true(( STD::is_same<US::allocator_type, STD::allocator<int> >::value ));
    test_pass("cxx11:unordered_set::allocator_type");
    test_true( !STD::is_signed<US::size_type>::value );
    test_pass("cxx11:unordered_set::size_type");

    US s; s.insert(1); s.insert(2);
    US::iterator it = s.begin();
    test_true( it != s.end() );
    test_pass("cxx11:unordered_set::iterator");
    US::const_iterator ci = s.cbegin();
    test_true( ci != s.cend() );
    test_pass("cxx11:unordered_set::const_iterator");
    US::size_type b = s.bucket(1);
    US::local_iterator li = s.begin(b);
    test_true( li != s.end(b) );
    test_pass("cxx11:unordered_set::local_iterator");
    US::const_local_iterator cli = s.cbegin(b);
    test_true( cli != s.cend(b) );
    test_pass("cxx11:unordered_set::const_local_iterator");
}

TEST_CASE(unordered_set, buckets_ctors_ops) {
    typedef STD::unordered_set<int> US;

    US s; for (int i = 0; i < 6; ++i) s.insert(i);
    US::size_type bk = s.bucket(3);
    int seen = 0;
    for (US::local_iterator it = s.begin(bk); it != s.end(bk); ++it) ++seen;
    test_eq( (long)seen, (long)s.bucket_size(bk) );
    test_pass("cxx11:unordered_set::begin(size_type)");
    test_pass("cxx11:unordered_set::end(size_type)");
    test_true( bk < s.bucket_count() );
    test_pass("cxx11:unordered_set::bucket");
    test_true( s.bucket_size(bk) >= 1 );
    test_pass("cxx11:unordered_set::bucket_size");
    test_true( s.max_bucket_count() >= s.bucket_count() );
    test_pass("cxx11:unordered_set::max_bucket_count");

    US::iterator eh = s.emplace_hint(s.end(), 100);
    test_true( *eh == 100 && s.count(100) == 1 );
    test_pass("cxx11:unordered_set::emplace_hint");

    US s2(16, US::hasher(), US::key_equal(), US::allocator_type());
    s2.insert(7);
    test_true( s2.bucket_count() >= 16 && s2.count(7) == 1 );
    test_pass("cxx11:unordered_set::unordered_set(bucket_count, hash, equal, alloc)");

    US::allocator_type al;
    US s3(al);
    s3.insert(9);
    test_true( s3.count(9) == 1 );
    test_pass("cxx11:unordered_set::unordered_set(const Alloc&) family");

#if _tst_cplusplus >= 201103L
    US il = { 1, 2, 3 };
    test_eq( il.size(), 3u );
    test_pass("cxx11:unordered_set::unordered_set(initializer_list)");
    il = { 4, 5 };
    test_true( il.size() == 2 && il.count(4) == 1 );
    test_pass("cxx11:unordered_set::operator=(initializer_list)");
#else
    test_skip("cxx11:unordered_set::unordered_set(initializer_list)");
    test_skip("cxx11:unordered_set::operator=(initializer_list)");
#endif

    US m1;
    m1.insert(8); m1.insert(9);
    US m2(STD::move(m1));
    test_true( m2.count(8) == 1 && m1.empty() );
    test_pass("cxx11:unordered_set::unordered_set(unordered_set&&)");
    US m3;
    m3 = STD::move(m2);
    test_true( m3.count(9) == 1 && m2.empty() );
    test_pass("cxx11:unordered_set::operator=(unordered_set&&)");

    static const int r123[3] = { 1, 2, 3 }, r321[3] = { 3, 2, 1 }, r12[2] = { 1, 2 };
    US a(r123, r123 + 3), b(r321, r321 + 3), c(r12, r12 + 2);
    test_true( a == b );
    test_pass("cxx11:operator==(unordered_set)");
    test_true( a != c );
    test_pass("cxx11:operator!=(unordered_set)");

    STD::unordered_multiset<int> ms;
    ms.insert(5); ms.insert(5); ms.insert(6);
    STD::pair<STD::unordered_multiset<int>::iterator,
              STD::unordered_multiset<int>::iterator> er = ms.equal_range(5);
    int cnt = 0;
    for (; er.first != er.second; ++er.first) ++cnt;
    test_eq( cnt, 2 );
    test_pass("cxx11:unordered_multiset::equal_range");
}

#if TEST_TARGET_CXX >= 2017 && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(unordered_set, merge_and_insert_return) {
    STD::unordered_set<int> a, b;
    a.insert(1); a.insert(2);
    b.insert(2); b.insert(3);
    a.merge(b);
    test_true( a.count(3) == 1 && a.size() == 3 );
    test_true( b.count(2) == 1 && b.size() == 1 );
    test_pass("cxx17:unordered_set::merge");

    STD::unordered_set<int> s; s.insert(7);
    STD::unordered_set<int>::node_type nh = s.extract(7);
    STD::unordered_set<int>::insert_return_type irt = s.insert(STD::move(nh));
    test_true( irt.inserted && *irt.position == 7 );
    test_pass("cxx17:unordered_set::insert_return_type");
}
#else
TEST_CASE(unordered_set, merge_and_insert_return) {
    test_skip("cxx17:unordered_set::merge");
    test_skip("cxx17:unordered_set::insert_return_type");
}
#endif

TEST_CASE(unordered_set, cxx23_ranges) {
#if defined(__cpp_lib_containers_ranges)
    STD::vector<int> v; v.push_back(1); v.push_back(2); v.push_back(3);
    STD::unordered_set<int> fr(STD::from_range, v);
    test_true( fr.size() == 3 && fr.count(2) == 1 );
    test_pass("cxx23:unordered_set::unordered_set(from_range_t, R&&)");
    STD::vector<int> v2; v2.push_back(4); v2.push_back(5);
    fr.insert_range(v2);
    test_true( fr.size() == 5 && fr.count(5) == 1 );
    test_pass("cxx23:unordered_set::insert_range");
#else
    test_skip("cxx23:unordered_set::unordered_set(from_range_t, R&&)");
    test_skip("cxx23:unordered_set::insert_range");
#endif
}


TEST_CASE(unordered_set, member_types_cxx11) {
    typedef STD::unordered_set<int> S;
    S s; s.insert(3);
    const S& cs = s;

    test_true(( STD::is_same<S::reference, int&>::value ));
    test_pass("cxx11:unordered_set::reference");

    S::const_reference cr = *cs.begin();
    test_eq( cr, 3 );
    test_true(( STD::is_same<S::const_reference, const int&>::value ));
    test_pass("cxx11:unordered_set::const_reference");

    test_true(( STD::is_same<S::pointer, int*>::value ));
    test_pass("cxx11:unordered_set::pointer");

    S::const_pointer cp = &*cs.begin();
    test_eq( *cp, 3 );
    test_pass("cxx11:unordered_set::const_pointer");

    test_true( STD::is_signed<S::difference_type>::value );
    S::difference_type d = 0;
    for (S::const_iterator i = cs.begin(); i != cs.end(); ++i) ++d;
    test_eq( (long)d, 1L );
    test_pass("cxx11:unordered_set::difference_type");
}

TEST_CASE(unordered_set, multiset_member_types_cxx11) {
    typedef STD::unordered_multiset<int> S;
    S s; s.insert(3); s.insert(3);
    const S& cs = s;

    test_true(( STD::is_same<S::reference, int&>::value ));
    test_pass("cxx11:unordered_multiset::reference");

    S::const_reference cr = *cs.begin();
    test_eq( cr, 3 );
    test_true(( STD::is_same<S::const_reference, const int&>::value ));
    test_pass("cxx11:unordered_multiset::const_reference");

    test_true(( STD::is_same<S::pointer, int*>::value ));
    test_pass("cxx11:unordered_multiset::pointer");

    S::const_pointer cp = &*cs.begin();
    test_eq( *cp, 3 );
    test_pass("cxx11:unordered_multiset::const_pointer");

    test_true( STD::is_signed<S::difference_type>::value );
    S::difference_type d = 0;
    for (S::const_iterator i = cs.begin(); i != cs.end(); ++i) ++d;
    test_eq( (long)d, 2L );
    test_pass("cxx11:unordered_multiset::difference_type");
}

TEST_CASE(unordered_set, feature_test_macros) {
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
#if defined(__cpp_lib_constexpr_unordered_set) && (__cpp_lib_constexpr_unordered_set) >= 202502L
    test_true( (__cpp_lib_constexpr_unordered_set) >= 202502L );
    test_pass("cxx26:__cpp_lib_constexpr_unordered_set=202502L");
    {
        struct K {
            static constexpr int run() {
                STD::unordered_set<int> s;
                s.insert(7);
                s.insert(7);
                s.insert(9);
                s.erase(9);
                return (int)(s.size() + s.count(7) + (s.find(9) == s.end() ? 4 : 0));
            }
        };
        static_assert(K::run() == 6, "unordered_set operations in a constant expression");
        test_eq( K::run(), 6 );
    }
    test_pass("cxx26:constexpr unordered_set");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_constexpr_unordered_set=202502L");
    test_skip("cxx26:constexpr unordered_set");
#endif
}

static int g_us_live = 0;

struct UsLive {
    STD::string s_;
    UsLive() { ++g_us_live; }
    UsLive(const char* s) : s_(s) { ++g_us_live; }
    UsLive(const UsLive& o) : s_(o.s_) { ++g_us_live; }
    UsLive& operator=(const UsLive& o) { s_ = o.s_; return *this; }
    ~UsLive() { --g_us_live; }
    bool operator==(const UsLive& o) const { return s_ == o.s_; }
};

static UsLive us_live_key(int i) {
    STD::string t("k");
    t += (char)('0' + i);
    t += "_long_enough_to_leave_the_sso_buffer";
    return UsLive(t.c_str());
}

struct UsLiveHash {
    STD::size_t operator()(const UsLive& v) const { return STD::hash<STD::string>()(v.s_); }
};

TEST_CASE(unordered_set, class_elements_lifetime) {
    g_us_live = 0;
    {
        STD::unordered_set<UsLive, UsLiveHash> s;
        for (int i = 0; i < 8; ++i) s.insert(us_live_key(i));
        test_eq( (int)s.size(), 8 );
        test_eq( g_us_live, 8 );

        STD::unordered_set<UsLive, UsLiveHash> c(s);
        test_eq( g_us_live, 16 );

        s.erase(s.begin());
        test_eq( (int)s.size(), 7 );
        test_eq( g_us_live, 15 );

        s.rehash(64);
        test_eq( (int)s.size(), 7 );
        test_eq( g_us_live, 15 );

        STD::unordered_set<UsLive, UsLiveHash> e;
        s.swap(e);
        test_true( s.empty() );
        test_eq( (int)e.size(), 7 );
        test_eq( g_us_live, 15 );

        e.clear();
        test_eq( g_us_live, 8 );

        c = e;
        test_eq( g_us_live, 0 );
    }
    test_eq( g_us_live, 0 );
}

TEST_CASE(unordered_set, constexpr_multiset_copy_cxx26) {
#if defined(__cpp_lib_constexpr_unordered_set) && (__cpp_lib_constexpr_unordered_set) >= 202502L
    struct K {
        static constexpr int copy_ctor() {
            STD::unordered_multiset<int> a; a.insert(5); a.insert(5); a.insert(7);
            STD::unordered_multiset<int> b(a);
            return (int)(b.size() + b.count(5)); }
        static constexpr int copy_assign() {
            STD::unordered_multiset<int> a; a.insert(5); a.insert(5); a.insert(7);
            STD::unordered_multiset<int> b; b.insert(1);
            b = a;
            return (int)(b.size() + b.count(5)); }
    };
    static_assert(K::copy_ctor() == 5, "constexpr unordered_multiset copy ctor");
    test_eq( K::copy_ctor(), 5 );
    test_pass("cxx26:constexpr unordered_multiset::unordered_multiset(const unordered_multiset&)");

    static_assert(K::copy_assign() == 5, "constexpr unordered_multiset copy assignment");
    test_eq( K::copy_assign(), 5 );
    test_pass("cxx26:constexpr unordered_multiset::operator=(const unordered_multiset&)");
#else
    TEST_NOTE("constexpr unordered containers are C++26 (P3372R3)");
    TEST_SKIP_N(2);
    test_skip("cxx26:constexpr unordered_multiset::unordered_multiset(const unordered_multiset&)");
    test_skip("cxx26:constexpr unordered_multiset::operator=(const unordered_multiset&)");
#endif
}

TEST_CASE(unordered_set, multiset_insert_forms) {
    typedef STD::unordered_multiset<int> MS;

    {   MS m;
        m.insert(5);
        MS::iterator a = m.insert(m.end(), 9);
        test_eq( *a, 9 );
        MS::iterator b = m.insert(m.begin(), 5);
        test_eq( *b, 5 );
        test_eq( (long)m.size(), 3L );
        test_eq( (long)m.count(5), 2L );
        test_eq( (long)m.count(9), 1L ); }
    test_pass("cxx11:unordered_multiset::insert (hint)");

    {   static const int src[4] = { 3, 1, 3, 2 };
        MS m;
        m.insert(src, src);
        test_true( m.empty() );
        m.insert(src, src + 4);
        test_eq( (long)m.size(), 4L );
        test_eq( (long)m.count(3), 2L );
        m.insert(src, src + 1);
        test_eq( (long)m.count(3), 3L ); }
    test_pass("cxx11:unordered_multiset::insert (range)");

    {   typedef STD::unordered_multiset<STD::string> MSS;
        MSS m;
        STD::string a("alpha"), b("alpha");
        MSS::iterator i1 = m.insert(STD::move(a));
        test_eq( *i1, STD::string("alpha") );
        m.insert(STD::move(b));
        test_eq( (long)m.size(), 2L );
        test_eq( (long)m.count(STD::string("alpha")), 2L ); }
    test_pass("cxx11:unordered_multiset::insert(T&&)");

    {   typedef STD::unordered_multiset<STD::string> MSS;
        MSS m;
        STD::string a("m"), b("z");
        m.insert(STD::move(a));
        MSS::iterator hi = m.insert(m.end(), STD::move(b));
        test_eq( *hi, STD::string("z") );
        test_eq( (long)m.size(), 2L );
        test_eq( (long)m.count(STD::string("z")), 1L ); }
    test_pass("cxx11:unordered_multiset::insert (hint, T&&)");
}

TEST_CASE(unordered_set, bucket_iterators_and_load_factor) {
    STD::unordered_set<int> s;
    for (int i = 0; i < 8; ++i) s.insert(i);

    {   float before = s.max_load_factor();
        test_true( before > 0.0f );
        s.max_load_factor(4.0f);
        test_eq( s.max_load_factor(), 4.0f );
        s.max_load_factor(before);
        test_eq( s.max_load_factor(), before ); }
    test_pass("cxx11:unordered_set::max_load_factor(float)");

    {   long seen = 0;
        STD::unordered_set<int>::size_type nb = s.bucket_count();
        for (STD::unordered_set<int>::size_type b = 0; b < nb; ++b) {
            long n = 0;
            for (STD::unordered_set<int>::const_local_iterator i = s.cbegin(b);
                 i != s.cend(b); ++i) ++n;
            test_eq( n, (long)s.bucket_size(b) );
            seen += n;
        }
        test_eq( seen, 8L );
        STD::unordered_set<int>::size_type hb = s.bucket(3);
        test_true( s.cbegin(hb) != s.cend(hb) ); }
    test_pass("cxx11:unordered_set::cbegin(size_type)");
    test_pass("cxx11:unordered_set::cend(size_type)");
}
