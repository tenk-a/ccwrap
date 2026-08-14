#include "test_cxx.hpp"

#include <forward_list>
#include <functional>
#include <vector>
#include <string>
#include <type_traits>

static int sum_fl(const STD::forward_list<int>& fl) {
    int s = 0;
    for (STD::forward_list<int>::const_iterator it = fl.begin(); it != fl.end(); ++it) s += *it;
    return s;
}
static int len_fl(const STD::forward_list<int>& fl) {
    int n = 0;
    for (STD::forward_list<int>::const_iterator it = fl.begin(); it != fl.end(); ++it) ++n;
    return n;
}

TEST_CASE(forward_list, construct_access) {
    STD::forward_list<int> fl;
    test_true( fl.empty() );
    test_true( fl.max_size() > 0u );

    fl.push_front(3); fl.push_front(2); fl.push_front(1);
    test_true( !fl.empty() );
    test_eq( fl.front(), 1 );
    test_eq( sum_fl(fl), 6 );

    fl.pop_front();
    test_eq( fl.front(), 2 );

    STD::forward_list<int> cp(fl);
    test_true( cp == fl );
    cp.push_front(9);
    test_true( cp != fl );
    test_true( fl < cp );

    STD::forward_list<int> asg;
    asg = fl;
    test_true( asg == fl );

    int src[] = { 5, 6, 7 };
    STD::forward_list<int> r(src, src + 3);
    test_eq( sum_fl(r), 18 );
    STD::forward_list<int> cnt(4, 2);
    test_eq( sum_fl(cnt), 8 );
    test_eq( len_fl(cnt), 4 );

    const STD::forward_list<int>& cr = fl;
    test_eq( cr.front(), 2 );
    int c = 0;
    for (STD::forward_list<int>::const_iterator it = cr.cbegin(); it != cr.cend(); ++it) ++c;
    test_eq( c, 2 );
    test_true( cr.cbefore_begin() != cr.cend() );

    fl.clear();
    test_true( fl.empty() );
    test_pass("cxx11:construct access");
}

TEST_CASE(forward_list, insert_erase_resize) {
    STD::forward_list<int> fl;
    STD::forward_list<int>::iterator it = fl.insert_after(fl.before_begin(), 10);
    fl.insert_after(it, 20);
    fl.insert_after(fl.before_begin(), 3, 0);
    test_eq( sum_fl(fl), 30 );
    test_eq( len_fl(fl), 5 );

    int ins[] = { 1, 2 };
    fl.insert_after(fl.begin(), ins, ins + 2);
    test_eq( sum_fl(fl), 33 );

    fl.erase_after(fl.before_begin());
    STD::forward_list<int>::iterator b = fl.begin();
    fl.erase_after(b, fl.end());

    fl.assign(3, 5);
    test_eq( sum_fl(fl), 15 );
    int a2[] = { 1, 2, 3, 4 };
    fl.assign(a2, a2 + 4);
    test_eq( len_fl(fl), 4 );

    fl.resize(2);
    test_eq( len_fl(fl), 2 );
    fl.resize(5, 9);
    test_eq( len_fl(fl), 5 );

    fl.emplace_front(7);
    test_eq( fl.front(), 7 );
    fl.emplace_after(fl.before_begin(), 8);
    test_eq( fl.front(), 8 );
    test_pass("cxx11:insert erase resize");
}

struct IsThree { bool operator()(int x) const { return x == 3; } };
struct AbsEq   { bool operator()(int a, int b) const { return a == b; } };

TEST_CASE(forward_list, algorithms) {
    int src[] = { 3, 1, 2, 1, 3 };
    STD::forward_list<int> fl(src, src + 5);
    fl.remove(1);
    test_eq( sum_fl(fl), 8 );
    fl.remove_if(IsThree());
    test_eq( fl.front(), 2 );

    int s2[] = { 5, 4, 3, 2, 1 };
    STD::forward_list<int> u(s2, s2 + 5);
    u.sort();
    test_eq( u.front(), 1 );
    u.sort(STD::greater<int>());
    test_eq( u.front(), 5 );
    u.reverse();
    test_eq( u.front(), 1 );

    int a[] = { 1, 1, 2, 3, 3, 3 };
    STD::forward_list<int> uu(a, a + 6);
    uu.unique();
    test_eq( sum_fl(uu), 6 );
    int a2[] = { 5, 5, 6 };
    STD::forward_list<int> up(a2, a2 + 3);
    up.unique(AbsEq());
    test_eq( sum_fl(up), 11 );

    int la[] = { 1, 3, 5 }, lb[] = { 2, 4, 6 };
    STD::forward_list<int> x(la, la + 3), y(lb, lb + 3);
    x.merge(y);
    test_eq( sum_fl(x), 21 );
    test_true( y.empty() );
    int ha[] = { 5, 3, 1 }, hb[] = { 6, 4, 2 };
    STD::forward_list<int> hx(ha, ha + 3), hy(hb, hb + 3);
    hx.merge(hy, STD::greater<int>());
    test_eq( hx.front(), 6 );

    STD::forward_list<int> p(la, la + 3), q(lb, lb + 3);
    p.swap(q);
    test_eq( p.front(), 2 );
    using STD::swap;
    swap(p, q);
    test_eq( p.front(), 1 );
    test_pass("cxx11:algorithms");
}

TEST_CASE(forward_list, ctors_ops) {
    typedef STD::forward_list<int> FL;

    FL fn(3);
    test_eq( (long)sum_fl(fn), 0L );
    int cnt = 0; for (FL::iterator it = fn.begin(); it != fn.end(); ++it) ++cnt;
    test_eq( cnt, 3 );
    test_pass("cxx11:forward_list::forward_list(n)");

    static const int r123[3] = { 1, 2, 3 };
#if _tst_cplusplus >= 201103L
    FL il = { 1, 2, 3 };
    test_eq( sum_fl(il), 6 );
    test_pass("cxx11:forward_list::forward_list(initializer_list)");
#else
    FL il(r123, r123 + 3);
    test_skip("cxx11:forward_list::forward_list(initializer_list)");
#endif

    STD::allocator<int> al;
    FL fa(al);
    fa.push_front(9);
    test_eq( fa.front(), 9 );
    test_pass("cxx11:forward_list::forward_list(const Allocator&) family");

    (void)il.get_allocator();
    test_true( true );
    test_pass("cxx11:forward_list::get_allocator");

    static const int r456[3] = { 4, 5, 6 };
    FL m1(r456, r456 + 3);
    FL m2(STD::move(m1));
    test_true( sum_fl(m2) == 15 && m1.empty() );
    test_pass("cxx11:forward_list::forward_list(forward_list&&)");
    FL m3;
    m3 = STD::move(m2);
    test_true( sum_fl(m3) == 15 && m2.empty() );
    test_pass("cxx11:forward_list::operator=(forward_list&&)");

    FL a2;
#if _tst_cplusplus >= 201103L
    a2 = { 7, 8 };
    test_eq( sum_fl(a2), 15 );
    test_pass("cxx11:forward_list::operator=(initializer_list)");
    a2.assign({ 1, 1, 1 });
    test_eq( sum_fl(a2), 3 );
    test_pass("cxx11:forward_list::assign(initializer_list)");
#else
    test_skip("cxx11:forward_list::operator=(initializer_list)");
    test_skip("cxx11:forward_list::assign(initializer_list)");
#endif

    STD::forward_list<STD::string> sl;
    STD::string mv("front");
    sl.push_front(STD::move(mv));
    test_eq( sl.front(), STD::string("front") );
    test_pass("cxx11:forward_list::push_front(T&&)");
    STD::string mv2("mid");
    sl.insert_after(sl.begin(), STD::move(mv2));
    test_pass("cxx11:forward_list::insert_after(pos,T&&)");
#if _tst_cplusplus >= 201103L
    FL ins = { 1 };
    ins.insert_after(ins.begin(), { 2, 3 });
    test_eq( sum_fl(ins), 6 );
    test_pass("cxx11:forward_list::insert_after(initializer_list)");
#else
    test_skip("cxx11:forward_list::insert_after(initializer_list)");
#endif

    static const int r12[2] = { 1, 2 }, r13[2] = { 1, 3 };
    FL r1(r12, r12 + 2), r2(r13, r13 + 2);
    test_true( r1 <= r2 && r1 < r2 );
    test_pass("cxx11:forward_list::operator<=");
    test_true( r2 > r1 );
    test_pass("cxx11:forward_list::operator>");
    test_true( r2 >= r1 && r1 >= r1 );
    test_pass("cxx11:forward_list::operator>=");
}

TEST_CASE(forward_list, splice_merge) {
    typedef STD::forward_list<int> FL;

    static const int r12[2] = { 1, 2 }, r34[2] = { 3, 4 };
    FL a(r12, r12 + 2), b(r34, r34 + 2);
    a.splice_after(a.begin(), b);
    test_true( sum_fl(a) == 10 && b.empty() );
    test_pass("cxx11:forward_list::splice_after");

    static const int r89[2] = { 8, 9 };
    FL c(r12, r12 + 2), d(r89, r89 + 2);
    c.splice_after(c.begin(), d, d.before_begin());
    test_eq( sum_fl(c), 11 );
    test_pass("cxx11:forward_list::splice_after (one)");

    static const int r1[1] = { 1 }, r567[3] = { 5, 6, 7 };
    FL e(r1, r1 + 1), f(r567, r567 + 3);
    e.splice_after(e.before_begin(), f, f.before_begin(), f.end());
    test_eq( sum_fl(e), 19 );
    test_pass("cxx11:forward_list::splice_after (range)");

    static const int r135[3] = { 1, 3, 5 }, r246[3] = { 2, 4, 6 };
    FL g(r135, r135 + 3), h(r246, r246 + 3);
    g.merge(STD::move(h));
    test_true( sum_fl(g) == 21 && h.empty() );
    int prev = 0; bool sorted = true;
    for (FL::iterator it = g.begin(); it != g.end(); ++it) { if (*it < prev) sorted = false; prev = *it; }
    test_true( sorted );
    test_pass("cxx11:forward_list::merge(forward_list&&)");
}

TEST_CASE(forward_list, cxx20_cxx23) {
#if defined(__cpp_lib_three_way_comparison)
    STD::forward_list<int> a = { 1, 2, 3 }, b = { 1, 2, 4 };
    test_true( (a <=> b) < 0 );
    test_true( (a <=> a) == 0 );
    test_pass("cxx20:operator<=>(forward_list)");
#else
    test_skip("cxx20:operator<=>(forward_list)");
#endif

#if TEST_TARGET_CXX >= 2020
    static const int r12232[5] = { 1, 2, 2, 3, 2 };
    STD::forward_list<int> e(r12232, r12232 + 5);
    STD::forward_list<int>::size_type n = STD::erase(e, 2);
    test_eq( (long)n, 3L );
    test_eq( sum_fl(e), 4 );
    test_pass("cxx20:erase(forward_list)");
    static const int r12345[5] = { 1, 2, 3, 4, 5 };
    STD::forward_list<int> e2(r12345, r12345 + 5);
    struct IsOdd { bool operator()(int x) const { return x % 2 == 1; } };
    STD::forward_list<int>::size_type n2 = STD::erase_if(e2, IsOdd());
    test_eq( (long)n2, 3L );
    test_pass("cxx20:erase_if(forward_list)");
#else
    test_skip("cxx20:erase(forward_list)");
    test_skip("cxx20:erase_if(forward_list)");
#endif

#if defined(__cpp_lib_containers_ranges)
    STD::vector<int> v; v.push_back(1); v.push_back(2); v.push_back(3);
    STD::forward_list<int> fr(STD::from_range, v);
    test_eq( sum_fl(fr), 6 );
    test_pass("cxx23:forward_list::forward_list(from_range_t,R&&)");
    STD::vector<int> v2; v2.push_back(4); v2.push_back(5);
    fr.prepend_range(v2);
    test_eq( sum_fl(fr), 15 );
    test_pass("cxx23:forward_list::prepend_range");
    STD::forward_list<int> ir = { 1 };
    ir.insert_range_after(ir.begin(), v2);
    test_eq( sum_fl(ir), 10 );
    test_pass("cxx23:forward_list::insert_range_after");
    STD::forward_list<int> ar;
    ar.assign_range(v);
    test_eq( sum_fl(ar), 6 );
    test_pass("cxx23:forward_list::assign_range");
#else
    test_skip("cxx23:forward_list::forward_list(from_range_t,R&&)");
    test_skip("cxx23:forward_list::prepend_range");
    test_skip("cxx23:forward_list::insert_range_after");
    test_skip("cxx23:forward_list::assign_range");
#endif
}


#if TEST_TARGET_CXX >= 2011 && _TST_HAS_CXX11_LIB_MEMBERS
namespace { struct CcwIsEven { bool operator()(int v) const { return v % 2 == 0; } }; }

TEST_CASE(forward_list, member_types_cxx11) {
    typedef STD::forward_list<int> F;
    F f;
    f.push_front(2);
    f.push_front(1);
    const F& cf = f;

    test_true(( STD::is_same<F::value_type, int>::value ));
    test_pass("cxx11:forward_list::value_type");

    F::reference r = f.front();
    r = 5;
    test_eq( f.front(), 5 );
    test_true(( STD::is_same<F::reference, int&>::value ));
    test_pass("cxx11:forward_list::reference");

    F::const_reference cr = cf.front();
    test_eq( cr, 5 );
    test_pass("cxx11:forward_list::const_reference");

    F::pointer p = &f.front();
    test_eq( *p, 5 );
    test_pass("cxx11:forward_list::pointer");

    F::const_pointer cp = &cf.front();
    test_eq( *cp, 5 );
    test_pass("cxx11:forward_list::const_pointer");

    test_true( !STD::is_signed<F::size_type>::value );
    F::size_type n = f.max_size();
    test_true( n > 0 );
    test_pass("cxx11:forward_list::size_type");

    test_true( STD::is_signed<F::difference_type>::value );
    {
        F::difference_type d = 0;
        F::const_iterator i;
        for (i = cf.begin(); i != cf.end(); ++i) ++d;
        test_eq( (long)d, 2L );
    }
    test_pass("cxx11:forward_list::difference_type");

    F::iterator it = f.begin();
    *it = 7;
    test_eq( f.front(), 7 );
    test_pass("cxx11:forward_list::iterator");

    F::const_iterator ci = cf.begin();
    test_eq( *ci, 7 );
    test_pass("cxx11:forward_list::const_iterator");

    test_true(( STD::is_same<F::allocator_type, STD::allocator<int> >::value ));
    F::allocator_type al = f.get_allocator();
    (void)al;
    test_pass("cxx11:forward_list::allocator_type");
}

TEST_CASE(forward_list, iteration_and_access_cxx11) {
    STD::forward_list<int> f;
    f.push_front(3); f.push_front(2); f.push_front(1);
    const STD::forward_list<int>& cf = f;

    test_eq( *f.begin(), 1 );
    test_pass("cxx11:forward_list::begin");
    test_true( f.begin() != f.end() );
    {
        int n = 0;
        STD::forward_list<int>::iterator i;
        for (i = f.begin(); i != f.end(); ++i) ++n;
        test_eq( n, 3 );
    }
    test_pass("cxx11:forward_list::end");

    test_eq( *cf.cbegin(), 1 );
    test_pass("cxx11:forward_list::cbegin");
    test_true( cf.cbegin() != cf.cend() );
    test_pass("cxx11:forward_list::cend");

    STD::forward_list<int>::iterator bb = f.before_begin();
    STD::forward_list<int>::iterator after_bb = bb;
    ++after_bb;
    test_true( after_bb == f.begin() );
    test_pass("cxx11:forward_list::before_begin");

    STD::forward_list<int>::const_iterator cbb = cf.cbefore_begin();
    STD::forward_list<int>::const_iterator after_cbb = cbb;
    ++after_cbb;
    test_true( after_cbb == cf.cbegin() );
    test_pass("cxx11:forward_list::cbefore_begin");

    test_eq( f.front(), 1 );
    f.front() = 9;
    test_eq( *f.begin(), 9 );
    test_pass("cxx11:forward_list::front");

    test_true( !f.empty() );
    test_pass("cxx11:forward_list::empty");

    test_true( f.max_size() >= (STD::size_t)3 );
    test_pass("cxx11:forward_list::max_size");
}

TEST_CASE(forward_list, modifiers_cxx11) {
    STD::forward_list<int> f;

    f.emplace_front(2);
    f.emplace_front(1);
    test_eq( f.front(), 1 );
    test_pass("cxx11:forward_list::emplace_front");

    STD::forward_list<int>::iterator it = f.emplace_after(f.begin(), 99);
    test_eq( *it, 99 );
    {
        STD::forward_list<int>::iterator i = f.begin();
        ++i;
        test_eq( *i, 99 );
    }
    test_pass("cxx11:forward_list::emplace_after");

    STD::forward_list<int>::iterator nxt = f.erase_after(f.begin());
    test_eq( *nxt, 2 );
    test_pass("cxx11:forward_list::erase_after");

    f.pop_front();
    test_eq( f.front(), 2 );
    test_pass("cxx11:forward_list::pop_front");

    f.clear();
    test_true( f.empty() );
    test_pass("cxx11:forward_list::clear");
}

TEST_CASE(forward_list, operations_cxx11) {
    STD::forward_list<int> f;
    f.push_front(3); f.push_front(1); f.push_front(3); f.push_front(2);

    f.remove(3);
    {
        int n = 0;
        STD::forward_list<int>::iterator i;
        for (i = f.begin(); i != f.end(); ++i) ++n;
        test_eq( n, 2 );
    }
    test_pass("cxx11:forward_list::remove");

    f.push_front(8); f.push_front(4);
    f.remove_if(CcwIsEven());
    test_eq( f.front(), 1 );
    test_pass("cxx11:forward_list::remove_if");

    f.clear();
    f.push_front(3); f.push_front(1); f.push_front(2);
    f.sort();
    {
        STD::forward_list<int>::iterator i = f.begin();
        test_eq( *i, 1 ); ++i;
        test_eq( *i, 2 ); ++i;
        test_eq( *i, 3 );
    }
    test_pass("cxx11:forward_list::sort");

    f.reverse();
    test_eq( f.front(), 3 );
    test_pass("cxx11:forward_list::reverse");

    f.clear();
    f.push_front(1); f.push_front(1); f.push_front(2); f.push_front(2);
    f.unique();
    {
        int n = 0;
        STD::forward_list<int>::iterator i;
        for (i = f.begin(); i != f.end(); ++i) ++n;
        test_eq( n, 2 );
    }
    test_pass("cxx11:forward_list::unique");

    STD::forward_list<int> a, b;
    a.push_front(1);
    b.push_front(9); b.push_front(8);
    a.swap(b);
    test_eq( a.front(), 8 );
    test_eq( b.front(), 1 );
    test_pass("cxx11:forward_list::swap");

    STD::swap(a, b);
    test_eq( a.front(), 1 );
    test_eq( b.front(), 8 );
    test_pass("cxx11:swap");
}
#else
TEST_CASE(forward_list, member_types_cxx11) {
    test_skip("cxx11:forward_list::value_type");
    test_skip("cxx11:forward_list::reference");
    test_skip("cxx11:forward_list::const_reference");
    test_skip("cxx11:forward_list::pointer");
    test_skip("cxx11:forward_list::const_pointer");
    test_skip("cxx11:forward_list::size_type");
    test_skip("cxx11:forward_list::difference_type");
    test_skip("cxx11:forward_list::iterator");
    test_skip("cxx11:forward_list::const_iterator");
    test_skip("cxx11:forward_list::allocator_type");
}
TEST_CASE(forward_list, iteration_and_access_cxx11) {
    test_skip("cxx11:forward_list::begin");
    test_skip("cxx11:forward_list::end");
    test_skip("cxx11:forward_list::cbegin");
    test_skip("cxx11:forward_list::cend");
    test_skip("cxx11:forward_list::before_begin");
    test_skip("cxx11:forward_list::cbefore_begin");
    test_skip("cxx11:forward_list::front");
    test_skip("cxx11:forward_list::empty");
    test_skip("cxx11:forward_list::max_size");
}
TEST_CASE(forward_list, modifiers_cxx11) {
    test_skip("cxx11:forward_list::emplace_front");
    test_skip("cxx11:forward_list::emplace_after");
    test_skip("cxx11:forward_list::erase_after");
    test_skip("cxx11:forward_list::pop_front");
    test_skip("cxx11:forward_list::clear");
}
TEST_CASE(forward_list, operations_cxx11) {
    test_skip("cxx11:forward_list::remove");
    test_skip("cxx11:forward_list::remove_if");
    test_skip("cxx11:forward_list::sort");
    test_skip("cxx11:forward_list::reverse");
    test_skip("cxx11:forward_list::unique");
    test_skip("cxx11:forward_list::swap");
    test_skip("cxx11:swap");
}
#endif

TEST_CASE(forward_list, feature_test_macros) {
#if defined(__cpp_lib_containers_ranges)
    test_true( __cpp_lib_containers_ranges > 0L );
    test_pass("cxx23:__cpp_lib_containers_ranges");
#else
    test_skip("cxx23:__cpp_lib_containers_ranges");
#endif
#if defined(__cpp_lib_constexpr_forward_list)
    test_true( __cpp_lib_constexpr_forward_list > 0L );
    test_pass("cxx26:__cpp_lib_constexpr_forward_list");
    {
        struct K {
            static constexpr int run() {
                STD::forward_list<int> f;
                f.push_front(3);
                f.push_front(1);
                f.insert_after(f.begin(), 2);
                f.remove(3);
                int s = 0;
                for (STD::forward_list<int>::iterator it = f.begin(); it != f.end(); ++it)
                    s = s * 10 + *it;
                return s;
            }
        };
        static_assert(K::run() == 12, "forward_list operations in a constant expression");
        test_eq( K::run(), 12 );
    }
    test_pass("cxx26:constexpr forward_list operations");
#else
    TEST_NOTE("constexpr forward_list is C++26 (P3372); not in this library");
    test_skip("cxx26:__cpp_lib_constexpr_forward_list");
    test_skip("cxx26:constexpr forward_list operations");
#endif
}

namespace {
int g_fl_live = 0;
struct FlLive {
    int v;
    FlLive() : v(0) { ++g_fl_live; }
    FlLive(int x) : v(x) { ++g_fl_live; }
    FlLive(const FlLive& o) : v(o.v) { ++g_fl_live; }
    FlLive& operator=(const FlLive& o) { v = o.v; return *this; }
    ~FlLive() { --g_fl_live; }
};
struct FlOdd { bool operator()(int x) const { return x % 2 != 0; } };
}

TEST_CASE(forward_list, members_cxx11) {
    typedef STD::forward_list<int> FL;

    FL d;
    test_true( d.empty() );
    test_true( d.begin() == d.end() );
    test_true( d.before_begin() != d.end() );
    test_pass("cxx11:forward_list::forward_list()");

    int src[] = { 1, 2, 3 };
    FL r(src, src + 3);
    test_eq( len_fl(r), 3 );
    test_eq( r.front(), 1 );
    test_eq( sum_fl(r), 6 );
    FL empty_range(src, src);
    test_true( empty_range.empty() );
    test_pass("cxx11:forward_list::forward_list(first,last)");

    FL cp(r);
    test_eq( len_fl(cp), 3 );
    test_true( cp == r );
    test_true( &cp.front() != &r.front() );
    cp.push_front(0);
    test_eq( len_fl(r), 3 );
    test_pass("cxx11:forward_list::forward_list(const forward_list&)");

    g_fl_live = 0;
    {
        STD::forward_list<FlLive> fl;
        fl.push_front(FlLive(1));
        fl.push_front(FlLive(2));
        fl.push_front(FlLive(3));
        test_eq( g_fl_live, 3 );
    }
    test_eq( g_fl_live, 0 );
    test_pass("cxx11:forward_list::~forward_list");

    FL a(src, src + 3), b;
    b = a;
    test_true( b == a );
    b.push_front(9);
    test_eq( len_fl(a), 3 );
    b = FL();
    test_true( b.empty() );
    test_pass("cxx11:forward_list::operator=(const forward_list&)");

    int other[] = { 7, 8 };
    FL as(src, src + 3);
    as.assign(other, other + 2);
    test_eq( len_fl(as), 2 );
    test_eq( as.front(), 7 );
    test_pass("cxx11:forward_list::assign(first,last)");

    as.assign(4, 5);
    test_eq( len_fl(as), 4 );
    test_eq( sum_fl(as), 20 );
    as.assign(0, 5);
    test_true( as.empty() );
    test_pass("cxx11:forward_list::assign(n,value)");

    FL pf;
    const int five = 5;
    pf.push_front(five);
    pf.push_front(4);
    test_eq( pf.front(), 4 );
    test_eq( len_fl(pf), 2 );
    test_eq( five, 5 );
    test_pass("cxx11:forward_list::push_front(const T&)");

    FL ins;
    ins.push_front(1);
    const int two = 2;
    FL::iterator it = ins.insert_after(ins.begin(), two);
    test_eq( *it, 2 );
    test_eq( len_fl(ins), 2 );
    ins.insert_after(ins.before_begin(), 0);
    test_eq( ins.front(), 0 );
    test_pass("cxx11:forward_list::insert_after(pos,const T&)");

    FL rng;
    rng.push_front(9);
    FL::iterator last = rng.insert_after(rng.begin(), src, src + 3);
    test_eq( *last, 3 );
    test_eq( len_fl(rng), 4 );
    test_eq( sum_fl(rng), 15 );
    FL::iterator none = rng.insert_after(rng.begin(), src, src);
    test_true( none == rng.begin() );
    test_pass("cxx11:forward_list::insert_after(first,last)");

    FL rs(src, src + 3);
    rs.resize(5);
    test_eq( len_fl(rs), 5 );
    test_eq( sum_fl(rs), 6 );
    rs.resize(2);
    test_eq( len_fl(rs), 2 );
    test_eq( sum_fl(rs), 3 );
    rs.resize(4, 7);
    test_eq( sum_fl(rs), 17 );
    test_pass("cxx11:forward_list::resize");
}

TEST_CASE(forward_list, comparison_operators_cxx11) {
    int a3[] = { 1, 2, 3 };
    STD::forward_list<int> a(a3, a3 + 3), b(a3, a3 + 3);
    int c4[] = { 1, 2, 4 };
    STD::forward_list<int> c(c4, c4 + 3);
    int s2[] = { 1, 2 };
    STD::forward_list<int> shorter(s2, s2 + 2);
    STD::forward_list<int> none, none2;

    test_true( a == b );
    test_true( none == none2 );
    test_true( !(a == c) );
    test_true( !(a == shorter) );
    test_pass("cxx11:forward_list::operator==");

    test_true( a != c );
    test_true( a != shorter );
    test_true( !(a != b) );
    test_pass("cxx11:forward_list::operator!=");

    test_true( a < c );
    test_true( shorter < a );
    test_true( none < a );
    test_true( !(a < b) );
    test_true( !(c < a) );
    test_pass("cxx11:forward_list::operator<");
}

TEST_CASE(forward_list, erasing_return_types) {
    int raw[] = { 3, 1, 2, 1, 3, 2, 1 };

#if defined(__cpp_lib_list_remove_return_type)
    {
        STD::forward_list<int> f(raw, raw + 7);
        long n = (long)f.remove(1);
        test_eq( n, 3L );
        long again = (long)f.remove(1);
        test_eq( again, 0L );
        test_pass("cxx20:forward_list::remove returns size_type");

        long odd = (long)f.remove_if(FlOdd());
        test_eq( odd, 2L );
        test_pass("cxx20:forward_list::remove_if returns size_type");

        int r2[] = { 1, 1, 2, 2, 2, 3 };
        STD::forward_list<int> u(r2, r2 + 6);
        long dup = (long)u.unique();
        test_eq( dup, 3L );
        test_pass("cxx20:forward_list::unique returns size_type");
    }
    test_skip("cxx11:forward_list::remove returns void");
    test_skip("cxx11:forward_list::remove_if returns void");
    test_skip("cxx11:forward_list::unique returns void");
#else
    {
        STD::forward_list<int> f(raw, raw + 7);
  #if _TST_HAS_DECLTYPE
        test_true(( STD::is_same<decltype(f.remove(1)), void>::value ));
        test_true(( STD::is_same<decltype(f.remove_if(FlOdd())), void>::value ));
        test_true(( STD::is_same<decltype(f.unique()), void>::value ));
  #else
        TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
  #endif
        f.remove(1);
        test_eq( len_fl(f), 4 );
    }
    test_pass("cxx11:forward_list::remove returns void");
    test_pass("cxx11:forward_list::remove_if returns void");
    test_pass("cxx11:forward_list::unique returns void");
    test_skip("cxx20:forward_list::remove returns size_type");
    test_skip("cxx20:forward_list::remove_if returns size_type");
    test_skip("cxx20:forward_list::unique returns size_type");
#endif
}

namespace {
static long fl_len(const STD::forward_list<int>& f) {
    long n = 0;
    for (STD::forward_list<int>::const_iterator i = f.begin(); i != f.end(); ++i) ++n;
    return n;
}
}

TEST_CASE(forward_list, overload_forms) {
    typedef STD::forward_list<int> F;
    static const int r4[4] = { 1, 2, 3, 4 };

    {
        F a(r4, r4 + 2), b(r4 + 2, r4 + 4);
        a.splice_after(a.before_begin(), STD::move(b));
        test_eq( a.front(), 3 );
        test_eq( (long)fl_len(a), 4L );
        test_true( b.empty() );

        F empty_f;
        a.splice_after(a.before_begin(), STD::move(empty_f));
        test_eq( (long)fl_len(a), 4L );
        test_eq( a.front(), 3 );

        F tail(r4, r4 + 1);
        a.splice_after(a.begin(), STD::move(tail));
        test_eq( (long)fl_len(a), 5L );
        F::iterator ai = a.begin(); ++ai;
        test_eq( *ai, 1 );
    }
    test_pass("cxx11:forward_list::splice_after(forward_list&&)");

    {
        F c(r4, r4 + 2), d(r4 + 2, r4 + 4);
        c.splice_after(c.before_begin(), STD::move(d), d.before_begin());
        test_eq( c.front(), 3 );
        test_eq( (long)fl_len(c), 3L );
        test_eq( (long)fl_len(d), 1L );
        test_eq( d.front(), 4 );

        c.splice_after(c.begin(), STD::move(d), d.before_begin());
        test_eq( (long)fl_len(c), 4L );
        F::iterator ci = c.begin(); ++ci;
        test_eq( *ci, 4 );
        test_true( d.empty() );
    }
    test_pass("cxx11:forward_list::splice_after(forward_list&&, iterator)");

    {
        F e(r4, r4 + 2), f(r4 + 2, r4 + 4);
        e.splice_after(e.before_begin(), STD::move(f), f.before_begin(), f.begin());
        test_eq( (long)fl_len(e), 2L );
        test_eq( (long)fl_len(f), 2L );

        e.splice_after(e.before_begin(), STD::move(f), f.before_begin(), f.end());
        test_eq( (long)fl_len(e), 4L );
        test_true( f.empty() );
        test_eq( e.front(), 3 );
    }
    test_pass("cxx11:forward_list::splice_after(forward_list&&, iterator, iterator)");

    {
        F empty_f;
        empty_f.sort(STD::greater<int>());
        test_true( empty_f.empty() );

        F one;
        one.push_front(7);
        one.sort(STD::greater<int>());
        test_eq( one.front(), 7 );

        F g;
        g.push_front(2); g.push_front(5); g.push_front(1); g.push_front(5);
        g.sort(STD::greater<int>());
        test_eq( g.front(), 5 );
        F::iterator gi = g.begin();
        ++gi; test_eq( *gi, 5 );
        ++gi; test_eq( *gi, 2 );
        ++gi; test_eq( *gi, 1 );

        g.sort(STD::greater<int>());
        test_eq( g.front(), 5 );
        test_eq( (long)fl_len(g), 4L );
    }
    test_pass("cxx11:forward_list::sort(Compare)");

    {
        F empty_f;
        empty_f.unique(STD::equal_to<int>());
        test_true( empty_f.empty() );

        F none;
        none.push_front(3); none.push_front(2); none.push_front(1);
        none.unique(STD::equal_to<int>());
        test_eq( (long)fl_len(none), 3L );

        F h;
        h.push_front(4); h.push_front(2); h.push_front(2); h.push_front(1);
        h.unique(STD::equal_to<int>());
        test_eq( (long)fl_len(h), 3L );
        F::iterator hi = h.begin();
        test_eq( *hi, 1 ); ++hi;
        test_eq( *hi, 2 ); ++hi;
        test_eq( *hi, 4 );

        F all;
        all.push_front(9); all.push_front(9); all.push_front(9);
        all.unique(STD::equal_to<int>());
        test_eq( (long)fl_len(all), 1L );
    }
    test_pass("cxx11:forward_list::unique(BinaryPredicate)");

    {
        F k(r4, r4 + 4);
        k.erase_after(k.before_begin(), k.begin());
        test_eq( (long)fl_len(k), 4L );

        F::iterator ke = k.begin(); ++ke; ++ke;
        F::iterator kr = k.erase_after(k.before_begin(), ke);
        test_true( kr == k.begin() );
        test_eq( k.front(), 3 );
        test_eq( (long)fl_len(k), 2L );

        k.erase_after(k.before_begin(), k.end());
        test_true( k.empty() );
    }
    test_pass("cxx11:forward_list::erase_after (range)");
}
