#include "test_cxx.hpp"
#include <queue>
#include <vector>
#include <deque>
#include <list>
#include <functional>
#include <string>
#include <type_traits>
#if defined(__cpp_lib_format_ranges)
#include <format>
#endif

TEST_CASE(queue, fifo) {
    STD::queue<int> q;
    test_true( q.empty() );
    test_pass("cxx03:queue::empty");
    test_eq( q.size(), 0u );
    test_pass("cxx03:queue::size");

    q.push(1);
    q.push(2);
    q.push(3);
    test_eq( q.size(), 3u );
    test_pass("cxx03:queue::push");

    test_eq( q.front(), 1 );
    { const STD::queue<int>& cq = q; test_eq( cq.front(), 1 ); }
    q.front() = 10;
    test_eq( q.front(), 10 );
    test_pass("cxx03:queue::front");

    test_eq( q.back(), 3 );
    { const STD::queue<int>& cq = q; test_eq( cq.back(), 3 ); }
    q.back() = 30;
    test_eq( q.back(), 30 );
    test_pass("cxx03:queue::back");

    q.pop();
    test_eq( q.front(), 2 );
    test_eq( q.size(), 2u );
    test_pass("cxx03:queue::pop");
}

TEST_CASE(queue, container_ctor) {
    STD::deque<int> d;
    d.push_back(1); d.push_back(2);
    STD::queue<int> q(d);
    test_eq( q.size(), 2u );
    test_eq( q.front(), 1 );
    test_pass("cxx03:queue::queue(const Container&)");
}

TEST_CASE(queue, emplace) {
    STD::queue<int> q;
    q.push(1);
#if _TST_HAS_CXX11_LIB_MEMBERS
    q.emplace(9);
    test_eq( q.back(), 9 );
    test_pass("cxx11:queue::emplace");
#else
    q.push(9);
    TEST_SKIP1();
    test_skip("cxx11:queue::emplace");
#endif
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(queue, cxx11_swap) {
    STD::queue<int> a, b;
    a.push(1); a.push(2);
    b.push(9);
    a.swap(b);
    test_eq( a.front(), 9 );
    test_eq( a.size(), 1u );
    test_pass("cxx11:queue::swap");
    STD::swap(a, b);
    test_eq( a.size(), 2u );
    test_pass("cxx11:swap(queue)");
}
#else
TEST_CASE_SKIP(queue, cxx11_swap)
#endif

TEST_CASE(queue, compare) {
    STD::queue<int> x, y, z;
    x.push(1); x.push(2);
    y.push(1); y.push(2);
    z.push(1);
    test_true( x == y );
    test_pass("cxx03:operator==(queue)");
    test_true( x != z );
    test_pass("cxx03:operator!=(queue)");
    test_true( z <  x );
    test_pass("cxx03:operator<(queue)");
    test_true( x >  z );
    test_pass("cxx03:operator>(queue)");
    test_true( z <= x );
    test_pass("cxx03:operator<=(queue)");
    test_true( x >= y );
    test_pass("cxx03:operator>=(queue)");
}

TEST_CASE(queue, string_elements) {

    STD::queue<STD::string> q;
    q.push("a");
    q.push(STD::string(300, 'c'));
    test_eq( q.front(), STD::string("a") );
    test_eq( q.back().size(), 300u );
    test_pass("cxx03:queue element-type independence (string)");
}

TEST_CASE(queue, priority_queue_max) {
    STD::priority_queue<int> pq;
    test_true( pq.empty() );
    test_pass("cxx03:priority_queue::empty");

    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(1);
    pq.push(5);
    test_eq( pq.size(), 5u );
    test_pass("cxx03:priority_queue::size");
    test_eq( pq.top(), 5 );
    test_pass("cxx03:priority_queue::push");

    int order[5];
    for (int i = 0; i < 5; ++i) { order[i] = pq.top(); pq.pop(); }
    test_eq( order[0], 5 );
    test_eq( order[4], 1 );
    test_true( pq.empty() );
    test_pass("cxx03:priority_queue::top");
    test_true( pq.empty() );
    test_pass("cxx03:priority_queue::pop");
}

TEST_CASE(queue, priority_queue_emplace) {
    STD::priority_queue<int> pq;
    pq.push(1);
#if _TST_HAS_CXX11_LIB_MEMBERS
    pq.emplace(7);
    test_eq( pq.top(), 7 );
    test_pass("cxx11:priority_queue::emplace");
#else
    pq.push(7);
    TEST_SKIP1();
    test_skip("cxx11:priority_queue::emplace");
#endif
}

TEST_CASE(queue, priority_queue_ctors) {

    STD::priority_queue<int, STD::vector<int>, STD::greater<int> > pq;
    pq.push(3);
    pq.push(1);
    pq.push(4);
    test_eq( pq.top(), 1 );
    test_pass("cxx03:priority_queue custom Compare (min-heap)");

    int raw[] = { 2, 8, 5, 1, 9 };
    STD::priority_queue<int> h(raw, raw + 5);
    test_eq( h.top(), 9 );
    test_eq( h.size(), 5u );
    test_pass("cxx03:priority_queue::priority_queue(first, last)");

    STD::vector<int> seed;
    seed.push_back(7); seed.push_back(4);
    STD::priority_queue<int, STD::vector<int>, STD::greater<int> > hc(STD::greater<int>(), seed);
    test_eq( hc.top(), 4 );
    test_pass("cxx03:priority_queue::priority_queue(const Compare&, const Container&)");
}

TEST_CASE(queue, priority_queue_string) {

    STD::priority_queue<STD::string> pq;
    pq.push("apple");
    pq.push("cherry");
    pq.push("banana");
    test_eq( pq.top(), STD::string("cherry") );
    pq.pop();
    test_eq( pq.top(), STD::string("banana") );
    test_pass("cxx03:priority_queue element-type independence (string)");
}

TEST_CASE(queue, rvalue_push_and_move) {
    {
        STD::queue<STD::string> q;
        STD::string s("moved");
        q.push(STD::move(s));
        test_eq( q.back(), STD::string("moved") );
        test_pass("cxx11:queue::push(T&&)");
    }
    {
        STD::priority_queue<STD::string> pq;
        STD::string s("z");
        pq.push(STD::move(s));
        pq.push(STD::string("a"));
        test_eq( pq.top(), STD::string("z") );
        test_pass("cxx11:priority_queue::push(T&&)");
    }
    {
        STD::queue<int> a; a.push(1); a.push(2);
        STD::queue<int> b(STD::move(a));
        test_eq( b.size(), (STD::size_t)2 );
        test_eq( b.front(), 1 );
        test_pass("cxx11:queue::queue(queue&&)");
    }
    {
        STD::priority_queue<int> a; a.push(5); a.push(9); a.push(1);
        STD::priority_queue<int> b(STD::move(a));
        test_eq( b.top(), 9 );
        test_pass("cxx11:priority_queue::priority_queue(priority_queue&&)");
    }
    {
        STD::deque<int> d; d.push_back(7); d.push_back(8);
        STD::queue<int> q(STD::move(d));
        test_eq( q.front(), 7 );
        test_eq( q.back(), 8 );
        test_pass("cxx11:queue::queue(Container&&)");
    }
}

#if _TST_HAS_CXX11_LIB_MEMBERS
TEST_CASE(queue, swap_and_allocators) {
    {
        STD::priority_queue<int> a; a.push(1); a.push(4);
        STD::priority_queue<int> b; b.push(9);
        a.swap(b);
        test_eq( a.top(), 9 );
        test_eq( b.top(), 4 );
        test_pass("cxx11:priority_queue::swap");
        STD::swap(a, b);
        test_eq( a.top(), 4 );
        test_eq( b.top(), 9 );
        test_pass("cxx11:swap(priority_queue)");
    }
    {
        STD::allocator<int> al;
        STD::queue<int> q(al);
        q.push(3);
        test_eq( q.front(), 3 );
        STD::deque<int> d; d.push_back(1);
        STD::queue<int> q2(d, al);
        test_eq( q2.front(), 1 );
        test_pass("cxx11:queue::queue(const Alloc&) family");
    }
    {
        STD::allocator<int> al;
        STD::less<int> cmp;
        STD::priority_queue<int> pq(cmp, al);
        pq.push(2); pq.push(8);
        test_eq( pq.top(), 8 );
        test_pass("cxx11:priority_queue allocator-extended ctors");
    }
    {
        test_true( (STD::uses_allocator<STD::queue<int>, STD::allocator<int> >::value) );
        test_pass("cxx11:uses_allocator<queue>");
        test_true( (STD::uses_allocator<STD::priority_queue<int>, STD::allocator<int> >::value) );
        test_pass("cxx11:uses_allocator<priority_queue>");
    }
}
#else
TEST_CASE_SKIP(queue, swap_and_allocators)
#endif

TEST_CASE(queue, spaceship) {
#if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
    STD::queue<int> a, b;
    a.push(1); a.push(2);
    b.push(1); b.push(3);
    test_true( (a <=> a) == 0 );
    test_true( (a <=> b) <  0 );
    test_true( (b <=> a) >  0 );
    test_pass("cxx20:operator<=>(queue)");
#else
    TEST_SKIP1(); TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx20:operator<=>(queue)");
#endif
}

TEST_CASE(queue, from_range_and_push_range) {
#if defined(__cpp_lib_containers_ranges)
    int src[] = { 4, 1, 3 };
    {
        STD::queue<int> q(STD::from_range, src);
        test_eq( q.size(), (STD::size_t)3 );
        test_eq( q.front(), 4 );
        test_pass("cxx23:queue::queue(from_range_t, R&&)");
    }
    {
        STD::queue<int> q;
        q.push_range(src);
        test_eq( q.size(), (STD::size_t)3 );
        test_eq( q.back(), 3 );
        test_pass("cxx23:queue::push_range");
    }
    {
        STD::priority_queue<int> pq(STD::from_range, src);
        test_eq( pq.top(), 4 );
        test_pass("cxx23:priority_queue::priority_queue(from_range_t, ...)");
    }
    {
        STD::priority_queue<int> pq;
        pq.push_range(src);
        test_eq( pq.top(), 4 );
        test_pass("cxx23:priority_queue::push_range");
    }
#else
    test_skip("cxx23:queue::queue(from_range_t, R&&)");
    test_skip("cxx23:queue::push_range");
    test_skip("cxx23:priority_queue::priority_queue(from_range_t, ...)");
    test_skip("cxx23:priority_queue::push_range");
#endif
}


TEST_CASE(queue, member_types_cxx03) {
    typedef STD::queue<int> Q;
    Q q; q.push(1); q.push(2);
    const Q& cq = q;

    test_true(( STD::is_same<Q::value_type, int>::value ));
    test_pass("cxx03:queue::value_type");

    Q::reference r = q.front();
    r = 5;
    test_eq( q.front(), 5 );
    test_true(( STD::is_same<Q::reference, int&>::value ));
    test_pass("cxx03:queue::reference");

    Q::size_type n = q.size();
    test_eq( n, (STD::size_t)2 );
    test_true( !STD::is_signed<Q::size_type>::value );
    test_pass("cxx03:queue::size_type");

    Q::const_reference cr = cq.front();
    test_eq( cr, 5 );
    test_true(( STD::is_same<Q::const_reference, const int&>::value ));
    test_pass("cxx03:queue::const_reference");

    test_true(( STD::is_same<Q::container_type, STD::deque<int> >::value ));
    test_true(( STD::is_same<STD::queue<int, STD::list<int> >::container_type,
                             STD::list<int> >::value ));
    test_pass("cxx03:queue::container_type");
}

TEST_CASE(queue, priority_queue_member_types_cxx03) {
    typedef STD::priority_queue<int> P;
    P p; p.push(1); p.push(9); p.push(5);

    test_true(( STD::is_same<P::value_type, int>::value ));
    test_pass("cxx03:priority_queue::value_type");

    test_true(( STD::is_same<P::reference, int&>::value ));
    test_eq( p.top(), 9 );
    test_pass("cxx03:priority_queue::reference");

    P::size_type n = p.size();
    test_eq( n, (STD::size_t)3 );
    test_true( !STD::is_signed<P::size_type>::value );
    test_pass("cxx03:priority_queue::size_type");

    P::const_reference cr = p.top();
    test_eq( cr, 9 );
    test_true(( STD::is_same<P::const_reference, const int&>::value ));
    test_pass("cxx03:priority_queue::const_reference");

    test_true(( STD::is_same<P::container_type, STD::vector<int> >::value ));
    test_pass("cxx03:priority_queue::container_type");

#if _TST_HAS_CXX17_LIB_MEMBERS
    test_true(( STD::is_same<P::value_compare, STD::less<int> >::value ));
    typedef STD::priority_queue<int, STD::vector<int>, STD::greater<int> > PGt;
    PGt mn;
    test_true(( STD::is_same<PGt::value_compare, STD::greater<int> >::value ));
    mn.push(4); mn.push(2);
    test_eq( mn.top(), 2 );
    test_pass("cxx03:priority_queue::value_compare");
#else
    test_skip("cxx03:priority_queue::value_compare");
#endif
}

TEST_CASE(queue, feature_test_macros) {
#if defined(__cpp_lib_adaptor_iterator_pair_constructor) && (__cpp_lib_adaptor_iterator_pair_constructor) >= 202106L
    test_true( (__cpp_lib_adaptor_iterator_pair_constructor) >= 202106L );
    test_pass("cxx23:__cpp_lib_adaptor_iterator_pair_constructor=202106L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_adaptor_iterator_pair_constructor=202106L");
#endif
#if defined(__cpp_lib_containers_ranges) && (__cpp_lib_containers_ranges) >= 202202L
    test_true( (__cpp_lib_containers_ranges) >= 202202L );
    test_pass("cxx23:__cpp_lib_containers_ranges=202202L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx23:__cpp_lib_containers_ranges=202202L");
#endif
#if defined(__cpp_lib_constexpr_queue) && (__cpp_lib_constexpr_queue) >= 202502L
    test_true( (__cpp_lib_constexpr_queue) >= 202502L );
    test_pass("cxx26:__cpp_lib_constexpr_queue=202502L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_constexpr_queue=202502L");
#endif
}

TEST_CASE(queue, adaptor_additions_cxx23) {
#if defined(__cpp_lib_adaptor_iterator_pair_constructor) && (__cpp_lib_adaptor_iterator_pair_constructor) >= 202106L
    {
        const int raw[] = { 1, 2, 3 };
        STD::queue<int> q(raw, raw + 3);
        test_eq( (int)q.size(), 3 );
        test_eq( q.front(), 1 );
        test_eq( q.back(), 3 );
    }
    test_pass("cxx23:queue::queue(first,last)");
#else
    TEST_NOTE("the iterator-pair adaptor constructors are C++23 (P1425R4)");
    test_skip("cxx23:queue::queue(first,last)");
#endif

#if defined(__cpp_lib_format_ranges)
    {
        STD::queue<int> q;
        q.push(1); q.push(2);
        STD::string out = STD::format("{}", q);
        test_true( out.size() > 0 );
    }
    test_pass("cxx23:formatter<queue>");
    {
        STD::priority_queue<int> pq;
        pq.push(1); pq.push(2);
        STD::string out = STD::format("{}", pq);
        test_true( out.size() > 0 );
    }
    test_pass("cxx23:formatter<priority_queue>");
#else
    TEST_NOTE("formatter<queue> comes with the C++23 range formatters");
    test_skip("cxx23:formatter<queue>");
    test_skip("cxx23:formatter<priority_queue>");
#endif
}

TEST_CASE(queue, constexpr_cxx26) {
#if defined(__cpp_lib_constexpr_queue) || defined(__cpp_lib_constexpr_deque)
    {
        STD::queue<int> q;
        q.push(5);
        q.push(6);
        test_eq( q.front(), 5 );
        q.pop();
        test_eq( q.front(), 6 );
    }
    test_pass("cxx26:constexpr queue");
    {
        STD::priority_queue<int> pq;
        pq.push(1); pq.push(9); pq.push(4);
        test_eq( pq.top(), 9 );
        pq.pop();
        test_eq( pq.top(), 4 );
    }
    test_pass("cxx26:constexpr priority_queue");
#else
    TEST_NOTE("constexpr queue follows the constexpr container work (C++26)");
    test_skip("cxx26:constexpr queue");
    test_skip("cxx26:constexpr priority_queue");
#endif
}

static int g_q_live = 0;

struct QLive {
    STD::string s_;
    QLive() { ++g_q_live; }
    QLive(const char* s) : s_(s) { ++g_q_live; }
    QLive(const QLive& o) : s_(o.s_) { ++g_q_live; }
    QLive& operator=(const QLive& o) { s_ = o.s_; return *this; }
    ~QLive() { --g_q_live; }
    bool operator< (const QLive& o) const { return s_ <  o.s_; }
};

static QLive q_live_key(int i) {
    STD::string t("k");
    t += (char)('0' + i);
    t += "_long_enough_to_leave_the_sso_buffer";
    return QLive(t.c_str());
}

TEST_CASE(queue, class_elements_lifetime) {
    g_q_live = 0;
    {
        STD::queue<QLive> q;
        for (int i = 0; i < 8; ++i) q.push(q_live_key(i));
        test_eq( (int)q.size(), 8 );
        test_eq( g_q_live, 8 );

        STD::queue<QLive> c(q);
        test_eq( g_q_live, 16 );

        q.pop();
        test_eq( (int)q.size(), 7 );
        test_eq( g_q_live, 15 );

        while (!c.empty()) c.pop();
        test_eq( g_q_live, 7 );

        STD::priority_queue<QLive> p;
        for (int i = 0; i < 5; ++i) p.push(q_live_key(4 - i));
        test_eq( (int)p.size(), 5 );
        test_eq( g_q_live, 12 );
        p.pop();
        test_eq( g_q_live, 11 );
        STD::priority_queue<QLive> pc(p);
        test_eq( (int)pc.size(), 4 );
        test_eq( g_q_live, 15 );
    }
    test_eq( g_q_live, 0 );
}
