#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2026 && _TST_TEST_HAS(<hive>)
#include <hive>
#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <utility>
#if TEST_TARGET_CXX >= 2017
#include <memory_resource>
#endif

namespace { struct IsMultipleOf10 { bool operator()(int v) const { return v % 10 == 0; } }; }

TEST_CASE(hive, stable_addresses) {
    STD::hive<int> hv;
    STD::hive<int>::iterator h1 = hv.insert(10);
    STD::hive<int>::iterator h2 = hv.insert(20);
    hv.insert(30);

    int* pstable = &*h2;
    hv.insert(40); hv.insert(50);

    test_eq( hv.size(), (STD::size_t)5 );
    test_eq( *pstable, 20 );
    test_true( &*h2 == pstable );

    int sum = 0;
    for (STD::hive<int>::iterator it = hv.begin(); it != hv.end(); ++it) sum += *it;
    test_eq( sum, 150 );

    hv.erase(h1);
    STD::size_t rem = STD::erase_if(hv, IsMultipleOf10());
    test_true( hv.empty() );
    test_true( rem >= 1 );
    test_pass("cxx26:stable addresses");
}

TEST_CASE(hive, copy) {
    STD::hive<int> hc;
    hc.insert(1); hc.insert(2);
    STD::hive<int> hcopy = hc;
    test_eq( hcopy.size(), (STD::size_t)2 );
    test_pass("cxx26:copy");
}

TEST_CASE(hive, emplace_capacity_swap) {
    STD::hive<int> hv;
    test_true( hv.empty() );

    STD::hive<int>::iterator e = hv.emplace(7);
    test_eq( *e, 7 );
    hv.emplace(8);
    test_eq( hv.size(), (STD::size_t)2 );

    hv.reserve(16);
    test_true( hv.capacity() >= hv.size() );

    int* p = &*e;
    STD::hive<int>::iterator back = hv.get_iterator(p);
    test_true( &*back == p );
    test_eq( *back, 7 );

    STD::hive<int> other;
    other.insert(100);
    hv.swap(other);
    test_eq( hv.size(), (STD::size_t)1 );
    test_eq( *hv.begin(), 100 );
    test_eq( other.size(), (STD::size_t)2 );

    hv.clear();
    test_true( hv.empty() );
    test_pass("cxx26:emplace capacity swap");
}

TEST_CASE(hive, string_elements) {

    STD::hive<STD::string> hv;
    STD::hive<STD::string>::iterator a = hv.insert(STD::string("alpha"));
    hv.insert(STD::string("beta"));
    STD::string* pstable = &*a;

    for (int i = 0; i < 20; ++i) hv.insert(STD::string(50, 'x'));
    test_true( &*a == pstable );
    test_eq( *pstable, STD::string("alpha") );
    test_eq( hv.size(), (STD::size_t)22 );

    hv.erase(a);
    test_eq( hv.size(), (STD::size_t)21 );
    test_pass("cxx26:string elements");
}

namespace {
int g_hv_live = 0;
struct HvLive {
    int v;
    HvLive() : v(0) { ++g_hv_live; }
    HvLive(int x) : v(x) { ++g_hv_live; }
    HvLive(const HvLive& o) : v(o.v) { ++g_hv_live; }
    HvLive& operator=(const HvLive& o) { v = o.v; return *this; }
    ~HvLive() { --g_hv_live; }
};
struct HvOdd { bool operator()(int x) const { return x % 2 != 0; } };
long hsum(const STD::hive<int>& h) {
    long s = 0;
    for (STD::hive<int>::const_iterator it = h.begin(); it != h.end(); ++it) s += *it;
    return s;
}
long hcount(const STD::hive<int>& h) {
    long n = 0;
    for (STD::hive<int>::const_iterator it = h.begin(); it != h.end(); ++it) ++n;
    return n;
}
}

TEST_CASE(hive, member_types_cxx26) {
    typedef STD::hive<int> H;

    test_true(( STD::is_same<H::value_type, int>::value ));
    test_pass("cxx26:hive::value_type");
    test_true(( STD::is_same<H::allocator_type, STD::allocator<int> >::value ));
    test_pass("cxx26:hive::allocator_type");
    test_true(( STD::is_same<H::pointer,
                STD::allocator_traits<STD::allocator<int> >::pointer>::value ));
    test_pass("cxx26:hive::pointer");
    test_true(( STD::is_same<H::const_pointer,
                STD::allocator_traits<STD::allocator<int> >::const_pointer>::value ));
    test_pass("cxx26:hive::const_pointer");
    test_true(( STD::is_same<H::reference, int&>::value ));
    test_pass("cxx26:hive::reference");
    test_true(( STD::is_same<H::const_reference, const int&>::value ));
    test_pass("cxx26:hive::const_reference");
    test_true( !STD::is_signed<H::size_type>::value );
    test_pass("cxx26:hive::size_type");
    test_true( STD::is_signed<H::difference_type>::value );
    test_pass("cxx26:hive::difference_type");

    H h;
    h.insert(1); h.insert(2);
    test_true( h.begin() != h.end() );
    test_pass("cxx26:hive::iterator");
    const H& ch = h;
    test_true( ch.begin() != ch.end() );
    test_pass("cxx26:hive::const_iterator");
    test_true( h.rbegin() != h.rend() );
    test_pass("cxx26:hive::reverse_iterator");
    test_true( ch.rbegin() != ch.rend() );
    test_pass("cxx26:hive::const_reverse_iterator");

    STD::allocator<int> a = h.get_allocator();
    (void)a;
    test_true( true );
    test_pass("cxx26:hive::get_allocator");
}

TEST_CASE(hive, observers_cxx26) {
    STD::hive<int> h;
    test_true( h.empty() );
    test_eq( (long)h.size(), 0L );
    test_true( h.begin() == h.end() );

    h.insert(1); h.insert(2); h.insert(3);
    test_true( !h.empty() );
    test_pass("cxx26:hive::empty");
    test_eq( (long)h.size(), 3L );
    test_eq( (long)h.size(), hcount(h) );
    test_pass("cxx26:hive::size");

    test_true( h.begin() != h.end() );
    test_eq( hsum(h), 6L );
    test_pass("cxx26:hive::begin");
    test_pass("cxx26:hive::end");

    const STD::hive<int>& ch = h;
    test_true( h.cbegin() == ch.begin() );
    test_pass("cxx26:hive::cbegin");
    test_true( h.cend() == ch.end() );
    test_pass("cxx26:hive::cend");

    long rsum = 0;
    for (STD::hive<int>::reverse_iterator it = h.rbegin(); it != h.rend(); ++it) rsum += *it;
    test_eq( rsum, 6L );
    test_pass("cxx26:hive::rbegin");
    test_pass("cxx26:hive::rend");
    test_true( h.crbegin() == ch.rbegin() );
    test_pass("cxx26:hive::crbegin");
    test_true( h.crend() == ch.rend() );
    test_pass("cxx26:hive::crend");

    test_true( h.max_size() >= h.size() );
    test_true( h.max_size() > 0 );
    test_pass("cxx26:hive::max_size");
    test_true( h.capacity() >= h.size() );
    test_pass("cxx26:hive::capacity");

    h.clear();
    test_true( h.empty() );
    test_eq( (long)h.size(), 0L );
    test_pass("cxx26:hive::clear");
}

TEST_CASE(hive, construction_cxx26) {
    typedef STD::hive<int> H;

    H d;
    test_true( d.empty() );
    test_pass("cxx26:hive::hive()");

#if defined(_CCW_HAS_HIVE_FULL)
    H sized((STD::size_t)3);
    test_eq( (long)sized.size(), 3L );
    test_eq( hsum(sized), 0L );
    test_pass("cxx26:hive::hive(n)");

    H filled((STD::size_t)3, 5);
    test_eq( (long)filled.size(), 3L );
    test_eq( hsum(filled), 15L );
    test_pass("cxx26:hive::hive(n, value)");
#else
    TEST_NOTE("no sized constructors in this hive");
    test_skip("cxx26:hive::hive(n)");
    test_skip("cxx26:hive::hive(n, value)");
#endif

    int raw[] = { 1, 2, 3 };
    H ranged(raw, raw + 3);
    test_eq( (long)ranged.size(), 3L );
    test_eq( hsum(ranged), 6L );
    test_pass("cxx26:hive::hive(first, last)");

    H copy(ranged);
    test_eq( (long)copy.size(), 3L );
    test_eq( hsum(copy), 6L );
    test_true( &*copy.begin() != &*ranged.begin() );
    test_pass("cxx26:hive::hive(const hive&)");

    H assigned;
    assigned.insert(9);
    assigned = ranged;
    test_eq( hsum(assigned), 6L );
    H empty_src;
    assigned = empty_src;
    test_true( assigned.empty() );
    test_pass("cxx26:hive::operator=(const hive&)");

#if _TST_HAS_RVALUE_REF
    H msrc(raw, raw + 3);
    H moved(STD::move(msrc));
    test_eq( hsum(moved), 6L );
    test_pass("cxx26:hive::hive(hive&&)");
    H massigned;
    H msrc2(raw, raw + 3);
    massigned = STD::move(msrc2);
    test_eq( hsum(massigned), 6L );
    test_pass("cxx26:hive::operator=(hive&&)");
#else
    TEST_NOTE("no rvalue references here");
    test_skip("cxx26:hive::hive(hive&&)");
    test_skip("cxx26:hive::operator=(hive&&)");
#endif

    g_hv_live = 0;
    {
        STD::hive<HvLive> lv;
        lv.insert(HvLive(1));
        lv.insert(HvLive(2));
        lv.insert(HvLive(3));
        test_eq( g_hv_live, 3 );
    }
    test_eq( g_hv_live, 0 );
    test_pass("cxx26:hive::~hive");

#if _tst_cplusplus >= 201103L
    H il = { 1, 2, 3 };
    test_eq( hsum(il), 6L );
    test_pass("cxx26:hive::hive(initializer_list)");
    il = { 4, 5 };
    test_eq( hsum(il), 9L );
    test_eq( (long)il.size(), 2L );
    test_pass("cxx26:hive::operator=(initializer_list)");
    il.insert({ 6, 7 });
    test_eq( (long)il.size(), 4L );
    test_eq( hsum(il), 22L );
    test_pass("cxx26:hive::insert(initializer_list)");
#else
    TEST_NOTE("no initializer_list here");
    test_skip("cxx26:hive::hive(initializer_list)");
    test_skip("cxx26:hive::operator=(initializer_list)");
    test_skip("cxx26:hive::insert(initializer_list)");
#endif

#if defined(_CCW_HAS_HIVE_FULL)
    H ar;
    ar.assign(raw, raw + 3);
    test_eq( (long)ar.size(), 3L );
    ar.assign(raw, raw + 1);
    test_eq( (long)ar.size(), 1L );
    test_pass("cxx26:hive::assign(first, last)");

    H an;
    an.assign((STD::size_t)3, 4);
    test_eq( hsum(an), 12L );
    an.assign((STD::size_t)0, 4);
    test_true( an.empty() );
    test_pass("cxx26:hive::assign(n, value)");
#else
    TEST_NOTE("no assign in this hive");
    test_skip("cxx26:hive::assign(first, last)");
    test_skip("cxx26:hive::assign(n, value)");
#endif
}

TEST_CASE(hive, modifiers_cxx26) {
    typedef STD::hive<int> H;
    H h;

    const int five = 5;
    H::iterator ins = h.insert(five);
    test_eq( *ins, 5 );
    test_eq( five, 5 );
    test_pass("cxx26:hive::insert(const T&)");

#if _TST_HAS_RVALUE_REF
    H::iterator mi = h.insert(6);
    test_eq( *mi, 6 );
    test_pass("cxx26:hive::insert(T&&)");
#else
    TEST_NOTE("no rvalue references here");
    test_skip("cxx26:hive::insert(T&&)");
#endif

    long before_emplace = (long)h.size();
    H::iterator em = h.emplace(7);
    test_eq( *em, 7 );
    test_eq( (long)h.size(), before_emplace + 1 );
    test_pass("cxx26:hive::emplace");

#if defined(_CCW_HAS_HIVE_FULL)
    H::iterator eh = h.emplace_hint(h.begin(), 8);
    test_eq( *eh, 8 );
    test_eq( (long)h.size(), 4L );
    test_pass("cxx26:hive::emplace_hint");

    H n;
    n.insert((STD::size_t)3, 9);
    test_eq( (long)n.size(), 3L );
    test_eq( hsum(n), 27L );
    test_pass("cxx26:hive::insert(n copies)");
#else
    TEST_NOTE("no emplace_hint / insert(n, value) in this hive");
    test_skip("cxx26:hive::emplace_hint");
    test_skip("cxx26:hive::insert(n copies)");
#endif

    int raw[] = { 1, 2 };
    H r;
    r.insert(raw, raw + 2);
    test_eq( hsum(r), 3L );
    test_pass("cxx26:hive::insert(range)");

    H stable;
    stable.insert(1);
    H::iterator keep = stable.insert(2);
    stable.insert(3);
    int* addr = &*keep;
    H::iterator gone = stable.erase(stable.begin());
    test_true( gone != stable.end() );
    test_eq( (long)stable.size(), 2L );
    test_true( &*keep == addr );
    test_eq( *keep, 2 );
    test_pass("cxx26:hive::erase(pos)");

    H rng;
    rng.insert(1); rng.insert(2); rng.insert(3); rng.insert(4);
    H::iterator first = rng.begin();
    ++first;
    H::iterator last = first;
    ++last; ++last;
    rng.erase(first, last);
    test_eq( (long)rng.size(), 2L );
    test_pass("cxx26:hive::erase(range)");

    H a, b;
    a.insert(1);
    b.insert(8); b.insert(9);
    a.swap(b);
    test_eq( (long)a.size(), 2L );
    test_eq( (long)b.size(), 1L );
    test_pass("cxx26:hive::swap");
#if !defined(__WATCOMC__)
    STD::swap(a, b);
    test_eq( (long)a.size(), 1L );
    test_pass("cxx26:swap(hive)");
#else
    TEST_NOTE("Open Watcom cannot rank a type-specific free swap against std::swap (A11)");
    test_skip("cxx26:swap(hive)");
#endif

    H e;
    e.insert(1); e.insert(2); e.insert(1);
    long removed = (long)STD::erase(e, 1);
    test_eq( removed, 2L );
    test_eq( (long)e.size(), 1L );
    test_pass("cxx26:erase(hive)");

    H f;
    f.insert(1); f.insert(2); f.insert(3);
    long odd = (long)STD::erase_if(f, HvOdd());
    test_eq( odd, 2L );
    test_eq( (long)f.size(), 1L );
    test_pass("cxx26:erase_if(hive)");
}

TEST_CASE(hive, hive_specific_cxx26) {
    typedef STD::hive<int> H;

#if defined(_CCW_HAS_HIVE_FULL)
    STD::hive_limits lim(8, 64);
    test_eq( (long)lim.min, 8L );
    test_pass("cxx26:hive_limits::min");
    test_eq( (long)lim.max, 64L );
    test_true( lim.min <= lim.max );
    test_pass("cxx26:hive_limits::max");
    test_true( sizeof(STD::hive_limits) >= sizeof(STD::size_t) );
    test_pass("cxx26:hive_limits");

    STD::hive_limits hard = H::block_capacity_hard_limits();
    test_true( hard.min <= hard.max );
    test_true( hard.min >= 1 );
    test_pass("cxx26:hive::block_capacity_hard_limits");
    STD::hive_limits def = H::block_capacity_default_limits();
    test_true( def.min >= hard.min && def.max <= hard.max );
    test_pass("cxx26:hive::block_capacity_default_limits");

    test_true( H::is_within_hard_limits(def) );
    test_true( !H::is_within_hard_limits(STD::hive_limits(0, 0)) );
    test_pass("cxx26:hive::is_within_hard_limits");

    H limited(def);
    limited.insert(1);
    STD::hive_limits got = limited.block_capacity_limits();
    test_true( got.min >= hard.min && got.max <= hard.max );
    test_pass("cxx26:hive::hive(hive_limits)");
    test_pass("cxx26:hive::block_capacity_limits");

    H rs;
    for (int i = 0; i < 20; ++i) rs.insert(i);
    long before = hsum(rs);
    rs.reshape(def);
    test_eq( hsum(rs), before );
    test_eq( (long)rs.size(), 20L );
    test_pass("cxx26:hive::reshape");

#else
    TEST_NOTE("no block-shaping surface in this hive");
    test_skip("cxx26:hive_limits::min");
    test_skip("cxx26:hive_limits::max");
    test_skip("cxx26:hive_limits");
    test_skip("cxx26:hive::block_capacity_hard_limits");
    test_skip("cxx26:hive::block_capacity_default_limits");
    test_skip("cxx26:hive::is_within_hard_limits");
    test_skip("cxx26:hive::hive(hive_limits)");
    test_skip("cxx26:hive::block_capacity_limits");
    test_skip("cxx26:hive::reshape");
#endif

    H cap;
    cap.insert(1);
    cap.reserve(64);
    test_eq( (long)cap.size(), 1L );
    test_eq( hsum(cap), 1L );
#if defined(_CCW_HAS_HIVE_FULL)
    test_true( cap.capacity() >= 64 );
    test_pass("cxx26:hive::reserve");
#else
    TEST_NOTE("this hive's reserve does not grow the capacity");
    TEST_SKIP1();
    test_skip("cxx26:hive::reserve");
#endif
    cap.trim_capacity();
    test_true( cap.capacity() >= cap.size() );
    test_eq( hsum(cap), 1L );
    test_pass("cxx26:hive::trim_capacity");
    cap.shrink_to_fit();
    test_true( cap.capacity() >= cap.size() );
    test_eq( hsum(cap), 1L );
    test_pass("cxx26:hive::shrink_to_fit");

    H gi;
    gi.insert(1);
    H::iterator second = gi.insert(2);
    int* p = &*second;
    H::iterator back = gi.get_iterator(p);
    test_true( &*back == p );
    test_eq( *back, 2 );
    test_pass("cxx26:hive::get_iterator");

#if defined(_CCW_HAS_HIVE_FULL)
    H dst, src;
    dst.insert(1);
    H::iterator kept = src.insert(2);
    src.insert(3);
    int* addr = &*kept;
    dst.splice(src);
    test_eq( (long)dst.size(), 3L );
    test_true( src.empty() );
    test_eq( *(int*)addr, 2 );
    test_pass("cxx26:hive::splice");

    H s;
    s.insert(3); s.insert(1); s.insert(2);
    s.sort();
    test_eq( *s.begin(), 1 );
    long prev = -1; bool ordered = true;
    for (H::iterator it = s.begin(); it != s.end(); ++it) {
        if ((long)*it < prev) ordered = false;
        prev = *it;
    }
    test_true( ordered );
    test_pass("cxx26:hive::sort");

    H u;
    u.insert(1); u.insert(1); u.insert(2); u.insert(2); u.insert(3);
    u.sort();
    long dropped = (long)u.unique();
    test_eq( dropped, 2L );
    test_eq( (long)u.size(), 3L );
    test_pass("cxx26:hive::unique");
#else
    TEST_NOTE("no splice / sort / unique in this hive");
    test_skip("cxx26:hive::splice");
    test_skip("cxx26:hive::sort");
    test_skip("cxx26:hive::unique");
#endif
}

TEST_CASE(hive, ranges_and_pmr_cxx26) {
#if defined(__cpp_lib_containers_ranges)
    STD::vector<int> src;
    src.push_back(1); src.push_back(2); src.push_back(3);

    STD::hive<int> fromr(STD::from_range, src);
    test_eq( hsum(fromr), 6L );
    test_pass("cxx26:hive::hive(from_range_t, R&&)");

    STD::hive<int> ar;
    ar.assign_range(src);
    test_eq( (long)ar.size(), 3L );
    ar.assign_range(src);
    test_eq( (long)ar.size(), 3L );
    test_pass("cxx26:hive::assign_range");

    STD::hive<int> ir;
    ir.insert(9);
    ir.insert_range(src);
    test_eq( (long)ir.size(), 4L );
    test_eq( hsum(ir), 15L );
    test_pass("cxx26:hive::insert_range");
#else
    TEST_NOTE("the range members need __cpp_lib_containers_ranges");
    test_skip("cxx26:hive::hive(from_range_t, R&&)");
    test_skip("cxx26:hive::assign_range");
    test_skip("cxx26:hive::insert_range");
#endif

#if _TST_HAS_ALIAS_TEMPLATE && _TST_HAS_CXX17_LIB_MEMBERS && defined(_CCW_HAS_PMR_HIVE)
    test_true(( STD::is_same<STD::pmr::hive<int>,
                STD::hive<int, STD::pmr::polymorphic_allocator<int> > >::value ));
    unsigned char buf[1024];
    STD::pmr::monotonic_buffer_resource res(buf, sizeof buf);
    STD::pmr::hive<int> ph(&res);
    ph.insert(1);
    test_eq( (long)ph.size(), 1L );
    test_true( ph.get_allocator().resource() == &res );
    test_pass("cxx26:pmr::hive");
#else
    TEST_NOTE("no pmr::hive alias here");
    test_skip("cxx26:pmr::hive");
#endif
}

#else
TEST_CASE(hive, member_types_cxx26) {
    test_skip("cxx26:hive::value_type");
    test_skip("cxx26:hive::allocator_type");
    test_skip("cxx26:hive::pointer");
    test_skip("cxx26:hive::const_pointer");
    test_skip("cxx26:hive::reference");
    test_skip("cxx26:hive::const_reference");
    test_skip("cxx26:hive::size_type");
    test_skip("cxx26:hive::difference_type");
    test_skip("cxx26:hive::iterator");
    test_skip("cxx26:hive::const_iterator");
    test_skip("cxx26:hive::reverse_iterator");
    test_skip("cxx26:hive::const_reverse_iterator");
    test_skip("cxx26:hive::get_allocator");
}
TEST_CASE(hive, observers_cxx26) {
    test_skip("cxx26:hive::empty");
    test_skip("cxx26:hive::size");
    test_skip("cxx26:hive::begin");
    test_skip("cxx26:hive::end");
    test_skip("cxx26:hive::cbegin");
    test_skip("cxx26:hive::cend");
    test_skip("cxx26:hive::rbegin");
    test_skip("cxx26:hive::rend");
    test_skip("cxx26:hive::crbegin");
    test_skip("cxx26:hive::crend");
    test_skip("cxx26:hive::max_size");
    test_skip("cxx26:hive::capacity");
    test_skip("cxx26:hive::clear");
}
TEST_CASE(hive, construction_cxx26) {
    test_skip("cxx26:hive::hive()");
    test_skip("cxx26:hive::hive(n)");
    test_skip("cxx26:hive::hive(n, value)");
    test_skip("cxx26:hive::hive(first, last)");
    test_skip("cxx26:hive::hive(const hive&)");
    test_skip("cxx26:hive::operator=(const hive&)");
    test_skip("cxx26:hive::hive(hive&&)");
    test_skip("cxx26:hive::operator=(hive&&)");
    test_skip("cxx26:hive::~hive");
    test_skip("cxx26:hive::hive(initializer_list)");
    test_skip("cxx26:hive::operator=(initializer_list)");
    test_skip("cxx26:hive::insert(initializer_list)");
    test_skip("cxx26:hive::assign(first, last)");
    test_skip("cxx26:hive::assign(n, value)");
}
TEST_CASE(hive, modifiers_cxx26) {
    test_skip("cxx26:hive::insert(const T&)");
    test_skip("cxx26:hive::insert(T&&)");
    test_skip("cxx26:hive::emplace");
    test_skip("cxx26:hive::emplace_hint");
    test_skip("cxx26:hive::insert(n copies)");
    test_skip("cxx26:hive::insert(range)");
    test_skip("cxx26:hive::erase(pos)");
    test_skip("cxx26:hive::erase(range)");
    test_skip("cxx26:hive::swap");
    test_skip("cxx26:swap(hive)");
    test_skip("cxx26:erase(hive)");
    test_skip("cxx26:erase_if(hive)");
}
TEST_CASE(hive, hive_specific_cxx26) {
    test_skip("cxx26:hive_limits::min");
    test_skip("cxx26:hive_limits::max");
    test_skip("cxx26:hive_limits");
    test_skip("cxx26:hive::block_capacity_hard_limits");
    test_skip("cxx26:hive::block_capacity_default_limits");
    test_skip("cxx26:hive::is_within_hard_limits");
    test_skip("cxx26:hive::hive(hive_limits)");
    test_skip("cxx26:hive::block_capacity_limits");
    test_skip("cxx26:hive::reshape");
    test_skip("cxx26:hive::reserve");
    test_skip("cxx26:hive::trim_capacity");
    test_skip("cxx26:hive::shrink_to_fit");
    test_skip("cxx26:hive::get_iterator");
    test_skip("cxx26:hive::splice");
    test_skip("cxx26:hive::sort");
    test_skip("cxx26:hive::unique");
}
TEST_CASE(hive, ranges_and_pmr_cxx26) {
    test_skip("cxx26:hive::hive(from_range_t, R&&)");
    test_skip("cxx26:hive::assign_range");
    test_skip("cxx26:hive::insert_range");
    test_skip("cxx26:pmr::hive");
}
TEST_CASE_SKIP(hive, stable_addresses)
TEST_CASE_SKIP(hive, copy)
TEST_CASE_SKIP(hive, emplace_capacity_swap)
TEST_CASE_SKIP(hive, string_elements)
#endif

TEST_CASE(hive, feature_test_macros) {
#if defined(__cpp_lib_hive)
    test_true( __cpp_lib_hive > 0L );
    test_pass("cxx26:__cpp_lib_hive");
#else
    test_skip("cxx26:__cpp_lib_hive");
#endif
}
