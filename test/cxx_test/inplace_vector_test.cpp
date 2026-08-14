#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2026 && _TST_TEST_HAS(<inplace_vector>)
#include <inplace_vector>
#include <new>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <utility>
#include <algorithm>

namespace {
int g_ipv_live = 0;
struct IpvLive {
    int v;
    IpvLive() : v(0) { ++g_ipv_live; }
    IpvLive(int x) : v(x) { ++g_ipv_live; }
    IpvLive(const IpvLive& o) : v(o.v) { ++g_ipv_live; }
    IpvLive& operator=(const IpvLive& o) { v = o.v; return *this; }
    ~IpvLive() { --g_ipv_live; }
};
}

#if TEST_HAS_EH
TEST_CASE(inplace_vector, capacity_and_overflow) {
    STD::inplace_vector<int, 4> iv;
    test_true( iv.empty() );
    test_eq( iv.capacity(), (STD::size_t)4 );

    iv.push_back(1); iv.push_back(2); iv.push_back(3);
    test_eq( iv.size(), (STD::size_t)3 );
    test_eq( iv[0], 1 );
    test_eq( iv.back(), 3 );

    test_true( iv.try_push_back(4) != 0 );
    test_true( iv.try_push_back(5) == 0 );

    bool threw = false;
    try { iv.push_back(9); } catch (STD::bad_alloc&) { threw = true; }
    test_true( threw );
    test_pass("cxx26:capacity and overflow");
}
#else   // !TEST_HAS_EH
TEST_CASE(inplace_vector, capacity_and_overflow) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx26:capacity and overflow");
}
#endif  // TEST_HAS_EH

TEST_CASE(inplace_vector, modify) {
    STD::inplace_vector<int, 4> iv;
    iv.push_back(1); iv.push_back(2); iv.push_back(3); iv.push_back(4);

    iv.pop_back();
    iv.emplace_back(7);
    test_eq( iv.back(), 7 );

    iv.pop_back();
    iv.insert(iv.begin() + 1, 99);
    test_eq( iv[1], 99 );
    test_eq( iv.size(), (STD::size_t)4 );

    iv.erase(iv.begin());
    test_eq( iv[0], 99 );
    test_eq( iv.size(), (STD::size_t)3 );

    STD::inplace_vector<int, 3> iw(3, 5);
    test_eq( iw.size(), (STD::size_t)3 );
    test_eq( iw[2], 5 );
    test_pass("cxx26:modify");
}

TEST_CASE(inplace_vector, access_and_iterators) {
    STD::inplace_vector<int, 5> iv;
    iv.push_back(10); iv.push_back(20); iv.push_back(30);
    const STD::inplace_vector<int, 5>& cv = iv;

    test_eq( iv.at(1), 20 );
    test_eq( cv.at(2), 30 );
    iv.at(0) = 11;
    test_eq( iv[0], 11 );
    test_throw( (void)iv.at(3) );
    test_throw( (void)cv.at(9) );

    test_eq( iv.front(), 11 );
    test_eq( cv.front(), 11 );
    test_eq( iv.back(), 30 );
    test_eq( *iv.data(), 11 );
    test_eq( cv.data()[2], 30 );
    test_eq( iv.max_size(), (STD::size_t)5 );

    int sum = 0;
    for (STD::inplace_vector<int, 5>::iterator it = iv.begin(); it != iv.end(); ++it) sum += *it;
    test_eq( sum, 61 );
    int csum = 0;
    for (STD::inplace_vector<int, 5>::const_iterator it = cv.cbegin(); it != cv.cend(); ++it) csum += *it;
    test_eq( csum, 61 );

    long rev = 0;
    for (STD::inplace_vector<int, 5>::reverse_iterator it = iv.rbegin(); it != iv.rend(); ++it) rev = rev * 100 + *it;
    test_eq( rev, 302011L );
    test_pass("cxx26:access and iterators");
}

TEST_CASE(inplace_vector, resize_assign_swap_compare) {
    STD::inplace_vector<int, 6> iv;
    iv.push_back(1); iv.push_back(2);

    iv.resize(4);
    test_eq( iv.size(), (STD::size_t)4 );
    test_eq( iv[3], 0 );
    iv.resize(1);
    test_eq( iv.size(), (STD::size_t)1 );

    int raw[] = { 5, 6, 7 };
    iv.assign(raw, raw + 3);
    test_eq( iv.size(), (STD::size_t)3 );
    test_eq( iv[0], 5 );

    iv.unchecked_push_back(8);
    test_eq( iv.back(), 8 );

    iv.clear();
    test_true( iv.empty() );

    STD::inplace_vector<int, 4> a, b, c;
    a.push_back(1); a.push_back(2);
    b.push_back(1); b.push_back(2);
    c.push_back(1);
    test_true( a == b );
    test_true( a != c );
    a.swap(c);
    test_eq( a.size(), (STD::size_t)1 );
    test_eq( c.size(), (STD::size_t)2 );
    STD::swap(a, c);
    test_eq( a.size(), (STD::size_t)2 );
    test_pass("cxx26:resize assign swap compare");
}

TEST_CASE(inplace_vector, string_elements) {

    STD::inplace_vector<STD::string, 4> iv;
    iv.push_back("alpha");
    iv.push_back("beta");
    iv.emplace_back("gamma");
    test_eq( iv.size(), (STD::size_t)3 );
    test_eq( iv[0], STD::string("alpha") );
    test_eq( iv.back(), STD::string("gamma") );

    iv.insert(iv.begin() + 1, STD::string("INS"));
    test_eq( iv[1], STD::string("INS") );
    test_eq( iv[2], STD::string("beta") );

    iv.erase(iv.begin());
    test_eq( iv.front(), STD::string("INS") );

    STD::inplace_vector<STD::string, 4> copy(iv);
    test_true( copy == iv );

    STD::inplace_vector<STD::string, 2> big;
    big.push_back(STD::string(300, 'x'));
    test_eq( big[0].size(), (STD::size_t)300 );

    iv.clear();
    test_true( iv.empty() );
    test_pass("cxx26:string elements");
}

namespace { struct IpvOdd { bool operator()(int x) const { return x % 2 != 0; } }; }

TEST_CASE(inplace_vector, member_types_cxx26) {
    typedef STD::inplace_vector<int, 4> IV;

    test_true(( STD::is_same<IV::value_type, int>::value ));
    test_pass("cxx26:inplace_vector::value_type");
    test_true(( STD::is_same<IV::pointer, int*>::value ));
    test_pass("cxx26:inplace_vector::pointer");
    test_true(( STD::is_same<IV::const_pointer, const int*>::value ));
    test_pass("cxx26:inplace_vector::const_pointer");
    test_true(( STD::is_same<IV::reference, int&>::value ));
    test_pass("cxx26:inplace_vector::reference");
    test_true(( STD::is_same<IV::const_reference, const int&>::value ));
    test_pass("cxx26:inplace_vector::const_reference");
    test_true( !STD::is_signed<IV::size_type>::value );
    test_pass("cxx26:inplace_vector::size_type");
    test_true( STD::is_signed<IV::difference_type>::value );
    test_pass("cxx26:inplace_vector::difference_type");

    IV v;
    v.push_back(1); v.push_back(2);
    test_true( &*v.begin() == v.data() );
    test_eq( (long)(v.end() - v.begin()), 2L );
    test_pass("cxx26:inplace_vector::iterator");
    const IV& cv = v;
    test_true( &*cv.begin() == cv.data() );
    test_pass("cxx26:inplace_vector::const_iterator");
    test_eq( *v.rbegin(), 2 );
    test_pass("cxx26:inplace_vector::reverse_iterator");
    test_eq( *cv.rbegin(), 2 );
    test_pass("cxx26:inplace_vector::const_reverse_iterator");
}

#if TEST_HAS_EH
TEST_CASE(inplace_vector, observers_cxx26) {
    typedef STD::inplace_vector<int, 4> IV;
    IV v;

    test_eq( v.capacity(), (STD::size_t)4 );
    test_eq( IV().capacity(), (STD::size_t)4 );
    test_pass("cxx26:inplace_vector::capacity");
    test_eq( v.max_size(), v.capacity() );
    test_pass("cxx26:inplace_vector::max_size");

    test_true( v.empty() );
    test_eq( v.size(), (STD::size_t)0 );
    v.push_back(1); v.push_back(2); v.push_back(3);
    test_true( !v.empty() );
    test_pass("cxx26:inplace_vector::empty");
    test_eq( v.size(), (STD::size_t)3 );
    test_true( v.size() <= v.capacity() );
    test_pass("cxx26:inplace_vector::size");

    test_eq( v.front(), 1 );
    v.front() = 9;
    test_eq( v[0], 9 );
    test_pass("cxx26:inplace_vector::front");
    test_eq( v.back(), 3 );
    v.back() = 8;
    test_eq( v[2], 8 );
    test_pass("cxx26:inplace_vector::back");
    test_eq( v[1], 2 );
    v[1] = 7;
    test_eq( v[1], 7 );
    test_pass("cxx26:inplace_vector::operator[]");

    test_eq( v.at(1), 7 );
    bool threw = false;
    try { (void)v.at(99); } catch (const STD::out_of_range&) { threw = true; } catch (...) {}
    test_true( threw );
    test_pass("cxx26:inplace_vector::at");

    test_true( v.data() != 0 );
    test_eq( v.data()[0], 9 );
    test_eq( (long)(&v.data()[2] - &v.data()[0]), 2L );
    test_pass("cxx26:inplace_vector::data");

    test_true( v.begin() != v.end() );
    test_pass("cxx26:inplace_vector::begin");
    test_eq( (long)(v.end() - v.begin()), (long)v.size() );
    test_pass("cxx26:inplace_vector::end");
    const IV& cv = v;
    test_true( v.cbegin() == cv.begin() );
    test_pass("cxx26:inplace_vector::cbegin");
    test_true( v.cend() == cv.end() );
    test_pass("cxx26:inplace_vector::cend");
    test_eq( *v.rbegin(), 8 );
    test_pass("cxx26:inplace_vector::rbegin");
    test_eq( (long)(v.rend() - v.rbegin()), 3L );
    test_pass("cxx26:inplace_vector::rend");
#if defined(_CCW_HAS_INPLACE_VECTOR_FULL)
    test_true( v.crbegin() == cv.rbegin() );
    test_pass("cxx26:inplace_vector::crbegin");
    test_true( v.crend() == cv.rend() );
    test_pass("cxx26:inplace_vector::crend");
#else
    TEST_NOTE("no crbegin / crend in this inplace_vector");
    test_skip("cxx26:inplace_vector::crbegin");
    test_skip("cxx26:inplace_vector::crend");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(inplace_vector, observers_cxx26) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx26:inplace_vector::capacity");
    test_skip("cxx26:inplace_vector::max_size");
    test_skip("cxx26:inplace_vector::empty");
    test_skip("cxx26:inplace_vector::size");
    test_skip("cxx26:inplace_vector::front");
    test_skip("cxx26:inplace_vector::back");
    test_skip("cxx26:inplace_vector::operator[]");
    test_skip("cxx26:inplace_vector::at");
    test_skip("cxx26:inplace_vector::data");
    test_skip("cxx26:inplace_vector::begin");
    test_skip("cxx26:inplace_vector::end");
    test_skip("cxx26:inplace_vector::cbegin");
    test_skip("cxx26:inplace_vector::cend");
    test_skip("cxx26:inplace_vector::rbegin");
    test_skip("cxx26:inplace_vector::rend");
    test_skip("cxx26:inplace_vector::crbegin");
    test_skip("cxx26:inplace_vector::crend");
}
#endif  // TEST_HAS_EH

TEST_CASE(inplace_vector, construction_cxx26) {
    typedef STD::inplace_vector<int, 4> IV;

    IV d;
    test_true( d.empty() );
    test_eq( d.capacity(), (STD::size_t)4 );
    test_pass("cxx26:inplace_vector::inplace_vector()");

    IV src;
    src.push_back(1); src.push_back(2);
    IV copy(src);
    test_eq( copy.size(), (STD::size_t)2 );
    test_eq( copy[1], 2 );
    test_true( copy.data() != src.data() );
    test_eq( src.size(), (STD::size_t)2 );
    test_pass("cxx26:inplace_vector::inplace_vector(const inplace_vector&)");

    IV assigned;
    assigned.push_back(9);
    assigned = src;
    test_eq( assigned.size(), (STD::size_t)2 );
    test_eq( assigned[0], 1 );
    IV empty_src;
    assigned = empty_src;
    test_true( assigned.empty() );
    test_pass("cxx26:inplace_vector::operator=(const inplace_vector&)");

#if _TST_HAS_RVALUE_REF
    IV msrc;
    msrc.push_back(3); msrc.push_back(4);
    IV moved(STD::move(msrc));
    test_eq( moved.size(), (STD::size_t)2 );
    test_eq( moved[0], 3 );
    test_pass("cxx26:inplace_vector::inplace_vector(inplace_vector&&)");

    IV massigned;
    IV msrc2;
    msrc2.push_back(5);
    massigned = STD::move(msrc2);
    test_eq( massigned.size(), (STD::size_t)1 );
    test_eq( massigned[0], 5 );
    test_pass("cxx26:inplace_vector::operator=(inplace_vector&&)");
#else
    TEST_NOTE("no rvalue references here");
    test_skip("cxx26:inplace_vector::inplace_vector(inplace_vector&&)");
    test_skip("cxx26:inplace_vector::operator=(inplace_vector&&)");
#endif

    g_ipv_live = 0;
    {
        STD::inplace_vector<IpvLive, 4> lv;
        lv.push_back(IpvLive(1));
        lv.push_back(IpvLive(2));
        test_eq( g_ipv_live, 2 );
    }
    test_eq( g_ipv_live, 0 );
    test_pass("cxx26:inplace_vector::~inplace_vector");

#if _tst_cplusplus >= 201103L
    IV il = { 1, 2, 3 };
    test_eq( il.size(), (STD::size_t)3 );
    test_eq( il[2], 3 );
    test_pass("cxx26:inplace_vector::operator=(initializer_list)");
    il = { 7, 8 };
    test_eq( il.size(), (STD::size_t)2 );
    test_eq( il[0], 7 );
    il.assign({ 4 });
    test_eq( il.size(), (STD::size_t)1 );
    test_eq( il[0], 4 );
    test_pass("cxx26:inplace_vector::assign(initializer_list)");
    il.insert(il.begin(), { 1, 2 });
    test_eq( il.size(), (STD::size_t)3 );
    test_eq( il[0], 1 );
    test_pass("cxx26:inplace_vector::insert(initializer_list)");
#else
    TEST_NOTE("no initializer_list here");
    test_skip("cxx26:inplace_vector::operator=(initializer_list)");
    test_skip("cxx26:inplace_vector::assign(initializer_list)");
    test_skip("cxx26:inplace_vector::insert(initializer_list)");
#endif

    int raw[] = { 1, 2, 3 };
    IV ranged;
    ranged.assign(raw, raw + 3);
    test_eq( ranged.size(), (STD::size_t)3 );
    ranged.assign(raw, raw + 1);
    test_eq( ranged.size(), (STD::size_t)1 );
    test_pass("cxx26:inplace_vector::assign(first,last)");

    IV counted;
    counted.assign(3, 7);
    test_eq( counted.size(), (STD::size_t)3 );
    test_eq( counted[2], 7 );
    counted.assign(0, 7);
    test_true( counted.empty() );
    test_pass("cxx26:inplace_vector::assign(n,value)");
}

TEST_CASE(inplace_vector, modifiers_cxx26) {
    typedef STD::inplace_vector<int, 6> IV;
    IV v;

    int& r = v.push_back(1);
    test_eq( r, 1 );
    r = 2;
    test_eq( v[0], 2 );
    test_pass("cxx26:inplace_vector::push_back");

    int& e = v.emplace_back(3);
    test_eq( e, 3 );
    test_eq( v.size(), (STD::size_t)2 );
    test_pass("cxx26:inplace_vector::emplace_back");

    v.pop_back();
    test_eq( v.size(), (STD::size_t)1 );
    test_eq( v.back(), 2 );
    test_pass("cxx26:inplace_vector::pop_back");

#if defined(_CCW_HAS_INPLACE_VECTOR_FULL)
    IV::iterator at = v.emplace(v.begin(), 0);
    test_eq( *at, 0 );
    test_eq( v[1], 2 );
    test_pass("cxx26:inplace_vector::emplace");
#else
    TEST_NOTE("no emplace(pos, ...) in this inplace_vector");
    test_skip("cxx26:inplace_vector::emplace");
#endif

    const int five = 5;
    IV::iterator ins = v.insert(v.begin(), five);
    test_eq( *ins, 5 );
    test_eq( five, 5 );
    test_pass("cxx26:inplace_vector::insert(const T&)");

#if _TST_HAS_RVALUE_REF
    IV::iterator mi = v.insert(v.begin(), 6);
    test_eq( *mi, 6 );
    test_pass("cxx26:inplace_vector::insert(T&&)");
#else
    TEST_NOTE("no rvalue references here");
    test_skip("cxx26:inplace_vector::insert(T&&)");
#endif

#if defined(_CCW_HAS_INPLACE_VECTOR_FULL)
    {
        IV n;
        n.push_back(1);
        IV::iterator many = n.insert(n.begin(), (STD::size_t)3, 9);
        test_eq( *many, 9 );
        test_eq( n.size(), (STD::size_t)4 );
        test_eq( n[3], 1 );
        IV::iterator none = n.insert(n.begin(), (STD::size_t)0, 9);
        test_true( none == n.begin() );
        test_pass("cxx26:inplace_vector::insert(n copies)");
    }
    {
        IV n;
        n.push_back(1);
        int raw[] = { 7, 8 };
        IV::iterator many = n.insert(n.begin(), raw, raw + 2);
        test_eq( *many, 7 );
        test_eq( n.size(), (STD::size_t)3 );
        test_eq( n[2], 1 );
        IV::iterator none = n.insert(n.begin(), raw, raw);
        test_true( none == n.begin() );
        test_pass("cxx26:inplace_vector::insert(range)");
    }
#else
    TEST_NOTE("no insert(n, value) / insert(first, last) in this inplace_vector");
    test_skip("cxx26:inplace_vector::insert(n copies)");
    test_skip("cxx26:inplace_vector::insert(range)");
#endif
    {
        IV n;
        n.push_back(1); n.push_back(2); n.push_back(3);
        IV::iterator after = n.erase(n.begin());
        test_eq( *after, 2 );
        test_eq( n.size(), (STD::size_t)2 );
        test_pass("cxx26:inplace_vector::erase(pos)");

        IV m;
        m.push_back(1); m.push_back(2); m.push_back(3); m.push_back(4);
        IV::iterator it = m.begin();
        ++it;
        IV::iterator stop = it;
        ++stop; ++stop;
        IV::iterator rest = m.erase(it, stop);
        test_eq( *rest, 4 );
        test_eq( m.size(), (STD::size_t)2 );
        IV::iterator noop = m.erase(m.begin(), m.begin());
        test_true( noop == m.begin() );
        test_pass("cxx26:inplace_vector::erase(range)");
    }

    IV z;
    z.push_back(1); z.push_back(2);
    z.clear();
    test_true( z.empty() );
    test_eq( z.capacity(), (STD::size_t)6 );
    test_pass("cxx26:inplace_vector::clear");

    z.resize(3);
    test_eq( z.size(), (STD::size_t)3 );
    test_eq( z[2], 0 );
    z.resize(5, 4);
    test_eq( z[4], 4 );
    z.resize(1);
    test_eq( z.size(), (STD::size_t)1 );
    test_pass("cxx26:inplace_vector::resize");

    {   STD::inplace_vector<int, 8> w;
        w.resize(3, 7);
        test_eq( w.size(), (STD::size_t)3 );
        test_eq( w[0], 7 );
        test_eq( w[2], 7 );
        w.resize(5, 9);
        test_eq( w.size(), (STD::size_t)5 );
        test_eq( w[2], 7 );
        test_eq( w[4], 9 );
        w.resize(2, 9);
        test_eq( w.size(), (STD::size_t)2 );
        test_eq( w[0], 7 );
        w.resize(2, 1);
        test_eq( w.size(), (STD::size_t)2 ); }
    test_pass("cxx26:inplace_vector::resize(size_type n, const T& value)");

    z.reserve(6);
    test_eq( z.capacity(), (STD::size_t)6 );
    test_eq( z.size(), (STD::size_t)1 );
    test_pass("cxx26:inplace_vector::reserve");
    z.shrink_to_fit();
    test_eq( z.capacity(), (STD::size_t)6 );
    test_eq( z.size(), (STD::size_t)1 );
    test_pass("cxx26:inplace_vector::shrink_to_fit");

    IV a, b;
    a.push_back(1);
    b.push_back(8); b.push_back(9);
    a.swap(b);
    test_eq( a.size(), (STD::size_t)2 );
    test_eq( b.size(), (STD::size_t)1 );
    test_pass("cxx26:inplace_vector::swap");
    STD::swap(a, b);
    test_eq( a.size(), (STD::size_t)1 );
    test_pass("cxx26:swap(inplace_vector)");
}

#if TEST_HAS_EH
TEST_CASE(inplace_vector, bounded_insertion_cxx26) {
    typedef STD::inplace_vector<int, 2> IV;
    IV v;
    v.push_back(1); v.push_back(2);
    test_eq( v.size(), v.capacity() );

    bool threw = false;
    try { v.push_back(3); } catch (const STD::bad_alloc&) { threw = true; } catch (...) {}
    test_true( threw );
    test_eq( v.size(), (STD::size_t)2 );
    test_pass("cxx26:inplace_vector insertion overflow throws bad_alloc");

    int* p = v.try_push_back(3);
    test_true( p == 0 );
    test_eq( v.size(), (STD::size_t)2 );
    IV room;
    int* ok = room.try_push_back(7);
    test_true( ok != 0 && *ok == 7 );
    test_pass("cxx26:inplace_vector::try_push_back");

#if defined(_CCW_HAS_INPLACE_VECTOR_FULL)
    int* pe = v.try_emplace_back(4);
    test_true( pe == 0 );
    int* oke = room.try_emplace_back(8);
    test_true( oke != 0 && *oke == 8 );
    test_eq( room.size(), (STD::size_t)2 );
    test_pass("cxx26:inplace_vector::try_emplace_back");
#else
    TEST_NOTE("no try_emplace_back in this inplace_vector");
    test_skip("cxx26:inplace_vector::try_emplace_back");
#endif

    IV u;
    int& ur = u.unchecked_push_back(1);
    test_eq( ur, 1 );
    test_eq( u.size(), (STD::size_t)1 );
    test_pass("cxx26:inplace_vector::unchecked_push_back");
#if defined(_CCW_HAS_INPLACE_VECTOR_FULL)
    int& ue = u.unchecked_emplace_back(2);
    test_eq( ue, 2 );
    test_eq( u.size(), (STD::size_t)2 );
    test_pass("cxx26:inplace_vector::unchecked_emplace_back");
#else
    TEST_NOTE("no unchecked_emplace_back in this inplace_vector");
    test_skip("cxx26:inplace_vector::unchecked_emplace_back");
#endif
}
#else   // !TEST_HAS_EH
TEST_CASE(inplace_vector, bounded_insertion_cxx26) {
    TEST_NOTE("needs exceptions; skipped in a no-exception build");
    test_skip("cxx26:inplace_vector insertion overflow throws bad_alloc");
    test_skip("cxx26:inplace_vector::try_push_back");
    test_skip("cxx26:inplace_vector::try_emplace_back");
    test_skip("cxx26:inplace_vector::unchecked_push_back");
    test_skip("cxx26:inplace_vector::unchecked_emplace_back");
}
#endif  // TEST_HAS_EH

TEST_CASE(inplace_vector, ranges_and_erase_cxx26) {
    typedef STD::inplace_vector<int, 8> IV;

#if defined(__cpp_lib_containers_ranges)
    STD::vector<int> src;
    src.push_back(1); src.push_back(2); src.push_back(3);

    IV fromr(STD::from_range, src);
    test_eq( fromr.size(), (STD::size_t)3 );
    test_eq( fromr[2], 3 );
    test_pass("cxx26:inplace_vector::inplace_vector(from_range_t, R&&)");

    IV ar;
    ar.assign_range(src);
    test_eq( ar.size(), (STD::size_t)3 );
    ar.assign_range(src);
    test_eq( ar.size(), (STD::size_t)3 );
    test_pass("cxx26:inplace_vector::assign_range");

    IV ap;
    ap.push_back(9);
    ap.append_range(src);
    test_eq( ap.size(), (STD::size_t)4 );
    test_eq( ap[0], 9 );
    test_pass("cxx26:inplace_vector::append_range");

    IV ir;
    ir.push_back(9);
    IV::iterator at = ir.insert_range(ir.begin(), src);
    test_eq( *at, 1 );
    test_eq( ir.size(), (STD::size_t)4 );
    test_eq( ir[3], 9 );
    test_pass("cxx26:inplace_vector::insert_range");

    {
        STD::inplace_vector<int, 4> small;
        small.push_back(0); small.push_back(0);
        STD::vector<int> four;
        four.push_back(1); four.push_back(2); four.push_back(3); four.push_back(4);
        STD::vector<int>::iterator stopped = small.try_append_range(four);
        test_eq( small.size(), (STD::size_t)4 );
        test_true( stopped != four.end() );
        test_eq( (long)(stopped - four.begin()), 2L );
        test_pass("cxx26:inplace_vector::try_append_range");
    }
#else
    TEST_NOTE("the range members need __cpp_lib_containers_ranges");
    test_skip("cxx26:inplace_vector::inplace_vector(from_range_t, R&&)");
    test_skip("cxx26:inplace_vector::assign_range");
    test_skip("cxx26:inplace_vector::append_range");
    test_skip("cxx26:inplace_vector::insert_range");
    test_skip("cxx26:inplace_vector::try_append_range");
#endif

#if defined(_CCW_HAS_INPLACE_VECTOR_FULL)
    IV e;
    e.push_back(1); e.push_back(2); e.push_back(1); e.push_back(3);
    long gone = (long)STD::erase(e, 1);
    test_eq( gone, 2L );
    test_eq( e.size(), (STD::size_t)2 );
    test_eq( e[0], 2 );
    test_pass("cxx26:erase(inplace_vector)");

    IV f;
    f.push_back(1); f.push_back(2); f.push_back(3);
    long odd = (long)STD::erase_if(f, IpvOdd());
    test_eq( odd, 2L );
    test_eq( f.size(), (STD::size_t)1 );
    test_eq( f[0], 2 );
    test_pass("cxx26:erase_if(inplace_vector)");
#else
    TEST_NOTE("no erase / erase_if overload for inplace_vector here");
    test_skip("cxx26:erase(inplace_vector)");
    test_skip("cxx26:erase_if(inplace_vector)");
#endif
}

TEST_CASE(inplace_vector, comparison_cxx26) {
    typedef STD::inplace_vector<int, 4> IV;
    IV a, b, c, shorter;
    a.push_back(1); a.push_back(2);
    b.push_back(1); b.push_back(2);
    c.push_back(1); c.push_back(3);
    shorter.push_back(1);

    test_true( a == b );
    test_true( !(a == c) );
    test_true( !(a == shorter) );
    test_pass("cxx26:inplace_vector::operator==");

#if _TST_HAS_SPACESHIP
    test_true( (a <=> c) < 0 );
    test_true( (c <=> a) > 0 );
    test_true( (a <=> b) == 0 );
    test_true( (shorter <=> a) < 0 );
    test_pass("cxx26:inplace_vector::operator<=>");
#else
    TEST_NOTE("no three-way comparison here");
    test_skip("cxx26:inplace_vector::operator<=>");
#endif
}

#else
TEST_CASE(inplace_vector, member_types_cxx26) {
    test_skip("cxx26:inplace_vector::value_type");
    test_skip("cxx26:inplace_vector::pointer");
    test_skip("cxx26:inplace_vector::const_pointer");
    test_skip("cxx26:inplace_vector::reference");
    test_skip("cxx26:inplace_vector::const_reference");
    test_skip("cxx26:inplace_vector::size_type");
    test_skip("cxx26:inplace_vector::difference_type");
    test_skip("cxx26:inplace_vector::iterator");
    test_skip("cxx26:inplace_vector::const_iterator");
    test_skip("cxx26:inplace_vector::reverse_iterator");
    test_skip("cxx26:inplace_vector::const_reverse_iterator");
}
TEST_CASE(inplace_vector, observers_cxx26) {
    test_skip("cxx26:inplace_vector::capacity");
    test_skip("cxx26:inplace_vector::max_size");
    test_skip("cxx26:inplace_vector::empty");
    test_skip("cxx26:inplace_vector::size");
    test_skip("cxx26:inplace_vector::front");
    test_skip("cxx26:inplace_vector::back");
    test_skip("cxx26:inplace_vector::operator[]");
    test_skip("cxx26:inplace_vector::at");
    test_skip("cxx26:inplace_vector::data");
    test_skip("cxx26:inplace_vector::begin");
    test_skip("cxx26:inplace_vector::end");
    test_skip("cxx26:inplace_vector::cbegin");
    test_skip("cxx26:inplace_vector::cend");
    test_skip("cxx26:inplace_vector::rbegin");
    test_skip("cxx26:inplace_vector::rend");
    test_skip("cxx26:inplace_vector::crbegin");
    test_skip("cxx26:inplace_vector::crend");
}
TEST_CASE(inplace_vector, construction_cxx26) {
    test_skip("cxx26:inplace_vector::inplace_vector()");
    test_skip("cxx26:inplace_vector::inplace_vector(const inplace_vector&)");
    test_skip("cxx26:inplace_vector::operator=(const inplace_vector&)");
    test_skip("cxx26:inplace_vector::inplace_vector(inplace_vector&&)");
    test_skip("cxx26:inplace_vector::operator=(inplace_vector&&)");
    test_skip("cxx26:inplace_vector::~inplace_vector");
    test_skip("cxx26:inplace_vector::operator=(initializer_list)");
    test_skip("cxx26:inplace_vector::assign(initializer_list)");
    test_skip("cxx26:inplace_vector::insert(initializer_list)");
    test_skip("cxx26:inplace_vector::assign(first,last)");
    test_skip("cxx26:inplace_vector::assign(n,value)");
}
TEST_CASE(inplace_vector, modifiers_cxx26) {
    test_skip("cxx26:inplace_vector::push_back");
    test_skip("cxx26:inplace_vector::emplace_back");
    test_skip("cxx26:inplace_vector::pop_back");
    test_skip("cxx26:inplace_vector::emplace");
    test_skip("cxx26:inplace_vector::insert(const T&)");
    test_skip("cxx26:inplace_vector::insert(T&&)");
    test_skip("cxx26:inplace_vector::insert(n copies)");
    test_skip("cxx26:inplace_vector::insert(range)");
    test_skip("cxx26:inplace_vector::erase(pos)");
    test_skip("cxx26:inplace_vector::erase(range)");
    test_skip("cxx26:inplace_vector::clear");
    test_skip("cxx26:inplace_vector::resize");
    test_skip("cxx26:inplace_vector::reserve");
    test_skip("cxx26:inplace_vector::shrink_to_fit");
    test_skip("cxx26:inplace_vector::swap");
    test_skip("cxx26:swap(inplace_vector)");
}
TEST_CASE(inplace_vector, bounded_insertion_cxx26) {
    test_skip("cxx26:inplace_vector insertion overflow throws bad_alloc");
    test_skip("cxx26:inplace_vector::try_push_back");
    test_skip("cxx26:inplace_vector::try_emplace_back");
    test_skip("cxx26:inplace_vector::unchecked_push_back");
    test_skip("cxx26:inplace_vector::unchecked_emplace_back");
}
TEST_CASE(inplace_vector, ranges_and_erase_cxx26) {
    test_skip("cxx26:inplace_vector::inplace_vector(from_range_t, R&&)");
    test_skip("cxx26:inplace_vector::assign_range");
    test_skip("cxx26:inplace_vector::append_range");
    test_skip("cxx26:inplace_vector::insert_range");
    test_skip("cxx26:inplace_vector::try_append_range");
    test_skip("cxx26:erase(inplace_vector)");
    test_skip("cxx26:erase_if(inplace_vector)");
}
TEST_CASE(inplace_vector, comparison_cxx26) {
    test_skip("cxx26:inplace_vector::operator==");
    test_skip("cxx26:inplace_vector::operator<=>");
}
TEST_CASE_SKIP(inplace_vector, capacity_and_overflow)
TEST_CASE_SKIP(inplace_vector, modify)
TEST_CASE_SKIP(inplace_vector, access_and_iterators)
TEST_CASE_SKIP(inplace_vector, resize_assign_swap_compare)
TEST_CASE_SKIP(inplace_vector, string_elements)
#endif

TEST_CASE(inplace_vector, feature_test_macros) {
#if defined(__cpp_lib_inplace_vector)
    test_true( __cpp_lib_inplace_vector > 0L );
    test_pass("cxx26:__cpp_lib_inplace_vector");
#else
    test_skip("cxx26:__cpp_lib_inplace_vector");
#endif
#if defined(__cpp_lib_constexpr_inplace_vector)
    test_true( __cpp_lib_constexpr_inplace_vector > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_inplace_vector");
#else
    test_skip("cxx26:__cpp_lib_constexpr_inplace_vector");
#endif
}
