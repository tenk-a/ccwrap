#include "test_cxx.hpp"

#if _TST_HAS_RVALUE_REF || defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1600)

#if defined(_MSC_VER) && _MSC_VER < 1600
#  define _TST_MOVE_NATIVE_CONTAINERS 0
#else
#  define _TST_MOVE_NATIVE_CONTAINERS 1
#endif

#include <utility>
#include <type_traits>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <set>
#include <map>
#include <tuple>
#include <variant>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>
#include <iterator>
#include <memory>
#include <optional>
#include <expected>
#include <flat_map>
#include <flat_set>
#if _TST_HAS_OS_THREADS
#include <thread>
#endif
#if _TST_HAS_OS_THREADS
#include <future>
#endif
#include <functional>
#include <string>
#include <filesystem>

#if !defined(_CCW_RV_REF)
# define _CCW_RV_REF(TYPE)  TYPE&&
# define _CCW_MOVED_OBJ(X)  (X)
#endif

namespace {

int g_copies = 0;
int g_moves  = 0;

struct buf {
    int* p_;

    buf() : p_(new int(0)) {}
    explicit buf(int v) : p_(new int(v)) {}

    buf(const buf& o) : p_(new int(*o.p_)) { ++g_copies; }
    buf& operator=(const buf& o) {
        if (this != &o) { int* q = new int(*o.p_); delete p_; p_ = q; }
        ++g_copies;
        return *this;
    }

    buf(_CCW_RV_REF(buf) r) : p_(0) {
        buf& o = _CCW_MOVED_OBJ(r);
        p_ = o.p_; o.p_ = 0;
        ++g_moves;
    }
    buf& operator=(_CCW_RV_REF(buf) r) {
        buf& o = _CCW_MOVED_OBJ(r);
        if (this != &o) { delete p_; p_ = o.p_; o.p_ = 0; }
        ++g_moves;
        return *this;
    }
    ~buf() { delete p_; }

    int value() const { return p_ ? *p_ : -1; }
    bool empty() const { return p_ == 0; }
    bool operator==(const buf& o) const { return value() == o.value(); }
};

struct oldstyle {
    int x_;
    oldstyle() : x_(0) {}
    explicit oldstyle(int v) : x_(v) {}
    oldstyle(const oldstyle& o) : x_(o.x_) { ++g_copies; }
    oldstyle& operator=(const oldstyle& o) { x_ = o.x_; ++g_copies; return *this; }
};

void reset() { g_copies = 0; g_moves = 0; }

}

TEST_CASE(move03, explicit_move_of_class) {
    reset();
    buf a(7);
    int* raw = a.p_;

    buf b(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( b.p_ == raw );
    test_true( a.empty() );
    test_eq( b.value(), 7 );
    test_pass("cxx11:STD::move (class)");
}

TEST_CASE(move03, explicit_move_assign) {
    reset();
    buf a(3);
    int* raw = a.p_;
    buf b(9);

    b = STD::move(a);
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( b.p_ == raw );
    test_true( a.empty() );
    test_eq( b.value(), 3 );
    test_pass("cxx11:STD::move assign (class)");
}

TEST_CASE(move03, copy_still_copies) {
    reset();
    buf a(5);
    buf b(a);
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_true( b.p_ != a.p_ );
    test_eq( a.value(), 5 );
    test_eq( b.value(), 5 );
    test_pass("cxx11:copy still copies (class)");
}

TEST_CASE(move03, move_of_const_is_a_copy) {
    reset();
    const buf a(4);
    buf b(STD::move(a));
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_eq( a.value(), 4 );
    test_eq( b.value(), 4 );
    test_pass("cxx11:STD::move of const is a copy");
}

TEST_CASE(move03, move_without_move_ctor_falls_back_to_copy) {
    reset();
    oldstyle a(11);
    oldstyle b(STD::move(a));
    test_eq( g_copies, 1 );
    test_eq( b.x_, 11 );
    test_eq( a.x_, 11 );
    test_pass("cxx11:STD::move without move ctor falls back to copy");
}

TEST_CASE(move03, move_of_scalar_compiles) {

    int i = 42;
    int j(STD::move(i));
    test_eq( j, 42 );

    double d = 2.5;
    double e(STD::move(d));
    test_eq( e, 2.5 );

    int* p = &i;
    int* q(STD::move(p));
    test_true( q == &i );

    const int ci = 8;
    int k(STD::move(ci));
    test_eq( k, 8 );
    test_pass("cxx11:STD::move of scalar");
}

namespace {

template <class T>
T relocate(T& src) { return T(STD::move(src)); }
}

TEST_CASE(move03, move_in_generic_code) {
    reset();
    buf a(6);
    int* raw = a.p_;
    buf b = relocate(a);
    test_true( b.p_ == raw );
    test_true( a.empty() );

    int i = 13;
    int j = relocate(i);
    test_eq( j, 13 );
    test_pass("cxx11:STD::move in generic code");
}

TEST_CASE(move03, vector_move_ctor_steals_buffer) {
    reset();
    STD::vector<buf> v;
    v.reserve(4);
    for (int i = 0; i < 3; ++i) v.push_back(buf(i));
    const buf* raw = v.empty() ? 0 : &v[0];
    reset();

    STD::vector<buf> w(STD::move(v));

    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( w.size(), 3u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &w[0] == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( v.size(), 0u );
    test_eq( w[2].value(), 2 );
    test_pass("cxx11:vector move ctor");
}

TEST_CASE(move03, vector_move_assign_steals_buffer) {
    reset();
    STD::vector<buf> v;
    v.reserve(2);
    v.push_back(buf(1));
    v.push_back(buf(2));
    const buf* raw = &v[0];

    STD::vector<buf> w;
    w.push_back(buf(9));
    reset();

    w = STD::move(v);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( w.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &w[0] == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );
    test_eq( w[0].value(), 1 );
    test_eq( w[1].value(), 2 );
    test_pass("cxx11:vector move assign");
}

TEST_CASE(move03, vector_copy_still_deep_copies) {
    STD::vector<buf> v;
    v.reserve(2);
    v.push_back(buf(1));
    v.push_back(buf(2));
    reset();

    STD::vector<buf> w(v);
    test_eq( g_moves, 0 );
    test_true( g_copies >= 2 );
    test_true( &w[0] != &v[0] );
    test_eq( v.size(), 2u );
    test_eq( v[0].value(), 1 );
    test_eq( w[0].value(), 1 );
    test_pass("cxx11:vector copy deep-copies");
}

TEST_CASE(move03, moved_from_vector_is_reusable) {
    STD::vector<buf> v;
    v.reserve(2);
    v.push_back(buf(1));
    STD::vector<buf> w(STD::move(v));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );

    v.push_back(buf(5));
    v.push_back(buf(6));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( v.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( v[1].value(), 6 );
    v.clear();
    test_true( v.empty() );
    test_pass("cxx11:moved-from vector is reusable");
}

TEST_CASE(move03, vector_push_back_moves_the_element) {
    STD::vector<buf> v;
    v.reserve(4);
    buf a(21);
    int* raw = a.p_;
    reset();

    v.push_back(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v[0].p_ == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    test_eq( v[0].value(), 21 );
    test_pass("cxx11:vector push_back moves");
}

TEST_CASE(move03, vector_push_back_copies_without_move) {
    STD::vector<buf> v;
    v.reserve(4);
    buf a(22);
    reset();

    v.push_back(a);
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_true( v[0].p_ != a.p_ );
    test_eq( a.value(), 22 );
    test_pass("cxx11:vector push_back copies (lvalue)");
}

TEST_CASE(move03, vector_insert_moves_the_element) {
    STD::vector<buf> v;
    v.reserve(8);
    v.push_back(buf(1));
    v.push_back(buf(3));

    buf a(2);
    int* raw = a.p_;
    reset();

    v.insert(v.begin() + 1, STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    test_eq( v.size(), 3u );
    test_eq( v[0].value(), 1 );
    test_eq( v[1].value(), 2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v[1].p_ == raw );
    test_eq( v[2].value(), 3 );

    buf b(4);
    int* rawb = b.p_;
    v.insert(v.end(), STD::move(b));
    test_eq( v.size(), 4u );
    test_eq( v[3].value(), 4 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v[3].p_ == rawb );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.empty() );
    test_pass("cxx11:vector insert moves");
}

TEST_CASE(move03, vector_of_scalars_unaffected) {

    STD::vector<int> v;
    for (int i = 0; i < 4; ++i) v.push_back(i);
    test_eq( v.size(), 4u );
    test_eq( v[3], 3 );

    v.insert(v.begin(), 9);
    test_eq( v[0], 9 );
    test_eq( v.size(), 5u );

    STD::vector<int> w(STD::move(v));
    test_eq( w.size(), 5u );
    test_eq( w[0], 9 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );

    STD::vector<int> x;
    x = STD::move(w);
    test_eq( x.size(), 5u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( w.empty() );
    test_pass("cxx11:vector of scalars unaffected");
}

TEST_CASE(move03, vector_of_vectors) {

    STD::vector<STD::vector<int> > vv;
    vv.reserve(4);
    STD::vector<int> inner;
    inner.push_back(1); inner.push_back(2);
    const int* raw = &inner[0];

    vv.push_back(STD::move(inner));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( inner.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &vv[0][0] == raw );
    test_eq( vv[0].size(), 2u );

    STD::vector<STD::vector<int> > ww(STD::move(vv));
    test_eq( ww.size(), 1u );
    test_eq( ww[0][1], 2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( vv.empty() );
    test_pass("cxx11:vector of vectors move");
}

TEST_CASE(move03, vector_growth_preserves_values) {

    STD::vector<buf> v;
    for (int i = 0; i < 20; ++i) v.push_back(buf(i));
    test_eq( v.size(), 20u );
    for (int i = 0; i < 20; ++i) test_eq( v[i].value(), i );
    test_pass("cxx11:vector growth preserves values");
}

namespace { enum color { red, green, blue }; }

TEST_CASE(move03, vector_of_enum_still_compiles) {

    STD::vector<color> v;
    v.push_back(red);
    v.push_back(blue);
    for (int i = 0; i < 20; ++i) v.push_back(green);
    test_eq( v.size(), 22u );
    test_eq( v[0], red );
    test_eq( v[1], blue );

    STD::vector<color> w(STD::move(v));
    test_eq( w.size(), 22u );
    test_eq( w[1], blue );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );
    test_pass("cxx11:vector of enum move");
}

TEST_CASE(move03, vector_of_string) {

    STD::vector<STD::string> v;
    v.push_back(STD::string("alpha"));
    v.push_back(STD::string("beta"));
    STD::string s("gamma");
    v.push_back(STD::move(s));
    test_eq( v.size(), 3u );
    test_true( v[0] == "alpha" );
    test_true( v[2] == "gamma" );

    STD::vector<STD::string> w(STD::move(v));
    test_eq( w.size(), 3u );
    test_true( w[1] == "beta" );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );
    test_pass("cxx11:vector of string move");
}

TEST_CASE(move03, list_move_ctor_relinks) {
    reset();
    STD::list<buf> l;
    for (int i = 0; i < 3; ++i) l.push_back(buf(i));
    const buf* first = &l.front();
    reset();

    STD::list<buf> m(STD::move(l));

    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( m.size(), 3u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &m.front() == first );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( l.size(), 0u );
    test_eq( m.back().value(), 2 );
    test_pass("cxx11:list move ctor");
}

TEST_CASE(move03, list_move_assign_relinks) {
    reset();
    STD::list<buf> l;
    l.push_back(buf(1));
    l.push_back(buf(2));
    const buf* first = &l.front();

    STD::list<buf> m;
    m.push_back(buf(9));
    reset();

    m = STD::move(l);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( m.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &m.front() == first );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.empty() );
    test_eq( m.front().value(), 1 );
    test_pass("cxx11:list move assign");
}

TEST_CASE(move03, list_copy_still_deep_copies) {
    STD::list<buf> l;
    l.push_back(buf(1));
    l.push_back(buf(2));
    reset();

    STD::list<buf> m(l);
    test_eq( g_moves, 0 );
    test_true( g_copies >= 2 );
    test_true( &m.front() != &l.front() );
    test_eq( l.size(), 2u );
    test_eq( l.front().value(), 1 );
    test_pass("cxx11:list copy deep-copies");
}

TEST_CASE(move03, list_push_moves_the_element) {
    STD::list<buf> l;

    buf a(7);
    int* rawa = a.p_;
    reset();
    l.push_back(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.back().p_ == rawa );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );

    buf b(8);
    int* rawb = b.p_;
    reset();
    l.push_front(STD::move(b));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.front().p_ == rawb );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.empty() );

    buf c(9);
    int* rawc = c.p_;
    reset();
    STD::list<buf>::iterator it = l.begin(); ++it;
    l.insert(it, STD::move(c));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( c.empty() );

    test_eq( l.size(), 3u );
    STD::list<buf>::iterator k = l.begin();
    test_eq( k->value(), 8 );
    ++k;
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( k->p_ == rawc );
    test_eq( k->value(), 9 );
    ++k;
    test_eq( k->value(), 7 );
    test_pass("cxx11:list push moves");
}

TEST_CASE(move03, list_push_copies_without_move) {
    STD::list<buf> l;
    buf a(3);
    reset();
    l.push_back(a);
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_true( l.back().p_ != a.p_ );
    test_eq( a.value(), 3 );
    test_pass("cxx11:list push copies (lvalue)");
}

TEST_CASE(move03, list_splice_and_merge_rvalue) {

    STD::list<int> a, b;
    a.push_back(1); a.push_back(4);
    b.push_back(2); b.push_back(3);

    STD::list<int>::iterator at = a.begin(); ++at;
    a.splice(at, STD::move(b));
    test_eq( a.size(), 4u );
    test_eq( b.size(), 0u );
    int expect1[4] = {1, 2, 3, 4};
    int i = 0;
    for (STD::list<int>::iterator it = a.begin(); it != a.end(); ++it, ++i) test_eq( *it, expect1[i] );
    test_pass("cxx11:list splice rvalue");

    STD::list<int> c, d;
    c.push_back(1); c.push_back(5);
    d.push_back(2); d.push_back(9);
    c.merge(STD::move(d));
    test_eq( c.size(), 4u );
    test_eq( d.size(), 0u );
    int expect2[4] = {1, 2, 5, 9};
    i = 0;
    for (STD::list<int>::iterator it = c.begin(); it != c.end(); ++it, ++i) test_eq( *it, expect2[i] );
    test_pass("cxx11:list merge rvalue");
}

TEST_CASE(move03, list_splice_single_and_range_rvalue) {
    STD::list<int> a, b;
    a.push_back(1);
    b.push_back(7); b.push_back(8); b.push_back(9);

    a.splice(a.end(), STD::move(b), b.begin());
    test_eq( a.size(), 2u );
    test_eq( b.size(), 2u );
    test_eq( a.back(), 7 );
    test_pass("cxx11:list splice single-element rvalue");

    a.splice(a.end(), STD::move(b), b.begin(), b.end());
    test_eq( a.size(), 4u );
    test_eq( b.size(), 0u );
    test_eq( a.back(), 9 );
    test_pass("cxx11:list splice range rvalue");
}

TEST_CASE(move03, list_of_enum_still_compiles) {
    STD::list<color> l;
    l.push_back(red);
    l.push_back(blue);
    test_eq( l.size(), 2u );
    test_eq( l.front(), red );

    STD::list<color> m(STD::move(l));
    test_eq( m.size(), 2u );
    test_eq( m.back(), blue );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.empty() );
    test_pass("cxx11:list of enum move");
}

TEST_CASE(move03, list_of_scalars_unaffected) {
    STD::list<int> l;
    for (int i = 0; i < 4; ++i) l.push_back(i);
    l.push_front(9);
    test_eq( l.size(), 5u );
    test_eq( l.front(), 9 );

    STD::list<int> m(STD::move(l));
    test_eq( m.size(), 5u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.empty() );

    STD::list<int> n;
    n = STD::move(m);
    test_eq( n.size(), 5u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( m.empty() );
    test_pass("cxx11:list of scalars unaffected");
}

TEST_CASE(move03, moved_from_list_is_reusable) {
    STD::list<buf> l;
    l.push_back(buf(1));
    STD::list<buf> m(STD::move(l));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.empty() );

    l.push_back(buf(5));
    l.push_front(buf(4));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( l.size(), 2u );
    test_eq( l.front().value(), 4 );
    l.clear();
    test_true( l.empty() );
    test_pass("cxx11:moved-from list is reusable");
}

TEST_CASE(move03, forward_list_move_ctor_relinks) {
    reset();
    STD::forward_list<buf> f;
    f.push_front(buf(1));
    f.push_front(buf(2));
    const buf* first = &f.front();
    reset();

    STD::forward_list<buf> g(STD::move(f));
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_true( &g.front() == first );
    test_true( f.empty() );
    test_eq( g.front().value(), 2 );
    test_pass("cxx11:forward_list move ctor");
}

TEST_CASE(move03, forward_list_move_assign_relinks) {
    STD::forward_list<buf> f;
    f.push_front(buf(1));
    const buf* first = &f.front();
    STD::forward_list<buf> g;
    g.push_front(buf(9));
    reset();

    g = STD::move(f);
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_true( &g.front() == first );
    test_true( f.empty() );
    test_eq( g.front().value(), 1 );
    test_pass("cxx11:forward_list move assign");
}

TEST_CASE(move03, forward_list_push_front_moves) {
    STD::forward_list<buf> f;
    buf a(7);
    int* raw = a.p_;
    reset();

    f.push_front(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( f.front().p_ == raw );
    test_true( a.empty() );

    buf b(8);
    reset();
    f.push_front(b);
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_true( f.front().p_ != b.p_ );
    test_eq( b.value(), 8 );
    test_pass("cxx11:forward_list push_front moves");
}

TEST_CASE(move03, forward_list_insert_after_moves) {
    STD::forward_list<buf> f;
    f.push_front(buf(1));
    buf a(2);
    int* raw = a.p_;
    reset();

    f.insert_after(f.begin(), STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( a.empty() );

    STD::forward_list<buf>::iterator it = f.begin();
    test_eq( it->value(), 1 );
    ++it;
    test_eq( it->value(), 2 );
    test_true( it->p_ == raw );
    test_pass("cxx11:forward_list insert_after moves");
}

TEST_CASE(move03, forward_list_splice_after_and_merge_rvalue) {
    STD::forward_list<int> a, b;
    a.push_front(4); a.push_front(1);
    b.push_front(3); b.push_front(2);

    a.splice_after(a.begin(), STD::move(b));
    test_true( b.empty() );
    int expect[4] = {1, 2, 3, 4};
    int i = 0;
    for (STD::forward_list<int>::iterator it = a.begin(); it != a.end(); ++it, ++i) test_eq( *it, expect[i] );
    test_eq( i, 4 );
    test_pass("cxx11:forward_list splice_after rvalue");

    STD::forward_list<int> c, d;
    c.push_front(5); c.push_front(1);
    d.push_front(9); d.push_front(2);
    c.merge(STD::move(d));
    test_true( d.empty() );
    int expect2[4] = {1, 2, 5, 9};
    i = 0;
    for (STD::forward_list<int>::iterator it = c.begin(); it != c.end(); ++it, ++i) test_eq( *it, expect2[i] );
    test_eq( i, 4 );
    test_pass("cxx11:forward_list merge rvalue");
}

TEST_CASE(move03, forward_list_of_enum_and_scalars) {
    STD::forward_list<color> f;
    f.push_front(red);
    f.push_front(blue);
    STD::forward_list<color> g(STD::move(f));
    test_eq( g.front(), blue );
    test_true( f.empty() );

    STD::forward_list<int> a;
    a.push_front(1);
    STD::forward_list<int> b(STD::move(a));
    test_eq( b.front(), 1 );
    test_true( a.empty() );

    STD::forward_list<int> c;
    c = STD::move(b);
    test_eq( c.front(), 1 );
    test_true( b.empty() );
    test_pass("cxx11:forward_list of enum and scalars move");
}

TEST_CASE(move03, deque_move_ctor_steals_buffer) {
    STD::deque<buf> d;
    for (int i = 0; i < 3; ++i) d.push_back(buf(i));
    const buf* raw = &d[0];
    reset();

    STD::deque<buf> e(STD::move(d));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( e.size(), 3u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &e[0] == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.empty() );
    test_eq( e[2].value(), 2 );
    test_pass("cxx11:deque move ctor");
}

TEST_CASE(move03, deque_move_assign_steals_buffer) {
    STD::deque<buf> d;
    d.push_back(buf(1));
    d.push_back(buf(2));
    const buf* raw = &d[0];

    STD::deque<buf> e;
    e.push_back(buf(9));
    reset();

    e = STD::move(d);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( e.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &e[0] == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.empty() );
    test_eq( e[1].value(), 2 );
    test_pass("cxx11:deque move assign");
}

TEST_CASE(move03, deque_copy_still_deep_copies) {
    STD::deque<buf> d;
    d.push_back(buf(1));
    d.push_back(buf(2));
    reset();

    STD::deque<buf> e(d);
#if !_TST_HAS_CXX11_LIB_MEMBERS || defined(__WATCOMC__)
    test_eq( g_moves, 0 );
#else
    TEST_SKIP1();
#endif
    test_true( g_copies >= 2 );
    test_true( &e[0] != &d[0] );
    test_eq( d.size(), 2u );
    test_eq( d[0].value(), 1 );
    test_pass("cxx11:deque copy deep-copies");
}

TEST_CASE(move03, deque_push_moves_the_element) {
    STD::deque<buf> d;
    buf a(7);
    int* rawa = a.p_;
    reset();
    d.push_back(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.back().p_ == rawa );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );

    buf b(8);
    int* rawb = b.p_;
    reset();
    d.push_front(STD::move(b));

    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.front().p_ == rawb );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.empty() );

    test_eq( d.size(), 2u );
    test_eq( d[0].value(), 8 );
    test_eq( d[1].value(), 7 );
    test_pass("cxx11:deque push moves");
}

TEST_CASE(move03, deque_push_copies_without_move) {
    STD::deque<buf> d;
    buf a(3);
    reset();
    d.push_back(a);
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_true( d.back().p_ != a.p_ );
    test_eq( a.value(), 3 );
    test_pass("cxx11:deque push copies (lvalue)");
}

TEST_CASE(move03, deque_insert_moves_the_element) {

    STD::deque<buf> d;
    d.push_back(buf(1));
    buf a(9);
    int* rawa = a.p_;
    reset();
    d.insert(d.end(), STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.back().p_ == rawa );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );

    STD::deque<buf> e;
    e.push_back(buf(1));
    e.push_back(buf(3));
    buf b(2);
    int* rawb = b.p_;
    e.insert(e.begin() + 1, STD::move(b));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.empty() );
    test_eq( e.size(), 3u );
    test_eq( e[0].value(), 1 );
    test_eq( e[1].value(), 2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( e[1].p_ == rawb );
    test_eq( e[2].value(), 3 );

    buf c(0);
    e.insert(e.begin(), STD::move(c));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( c.empty() );
    test_eq( e.size(), 4u );
    test_eq( e[0].value(), 0 );
    test_eq( e[1].value(), 1 );
    test_eq( e[3].value(), 3 );
    test_pass("cxx11:deque insert moves");
}

TEST_CASE(move03, deque_of_enum_and_scalars) {
    STD::deque<color> d;
    d.push_back(red);
    d.push_front(blue);
    for (int i = 0; i < 20; ++i) d.push_back(green);
    test_eq( d.size(), 22u );
    test_eq( d[0], blue );
    STD::deque<color> e(STD::move(d));
    test_eq( e.size(), 22u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.empty() );

    STD::deque<int> a;
    for (int i = 0; i < 4; ++i) a.push_back(i);
    a.insert(a.begin(), 9);
    test_eq( a[0], 9 );
    test_eq( a.size(), 5u );
    STD::deque<int> b(STD::move(a));
    test_eq( b.size(), 5u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    STD::deque<int> c;
    c = STD::move(b);
    test_eq( c.size(), 5u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.empty() );
    test_pass("cxx11:deque of enum and scalars move");
}

TEST_CASE(move03, moved_from_deque_is_reusable) {
    STD::deque<buf> d;
    d.push_back(buf(1));
    STD::deque<buf> e(STD::move(d));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.empty() );

    d.push_back(buf(5));
    d.push_front(buf(4));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( d.size(), 2u );
    test_eq( d[0].value(), 4 );
    d.clear();
    test_true( d.empty() );
    test_pass("cxx11:moved-from deque is reusable");
}

TEST_CASE(move03, string_move_ctor_steals_buffer) {
    STD::string s("a reasonably long string, well past any small-buffer size");
    const char* raw = s.data();
    STD::string::size_type n = s.size();

    STD::string t(STD::move(s));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( t.data() == raw );
    test_eq( t.size(), n );
    test_true( t == "a reasonably long string, well past any small-buffer size" );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( s.size(), 0u );
    test_true( s.data() != 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( s.c_str()[0], '\0' );
    test_pass("cxx11:string move ctor");
}

TEST_CASE(move03, string_move_assign_steals_buffer) {
    STD::string s("another string long enough to own a heap buffer");
    const char* raw = s.data();

    STD::string t("previous contents");
    t = STD::move(s);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( t.data() == raw );
    test_true( t == "another string long enough to own a heap buffer" );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( s.c_str()[0], '\0' );
    test_pass("cxx11:string move assign");
}

TEST_CASE(move03, string_copy_still_copies) {
    STD::string s("hello");
    STD::string t(s);
    test_true( t.data() != s.data() );
    test_true( s == "hello" );
    test_true( t == "hello" );

    STD::string u;
    u = s;
    test_true( u.data() != s.data() );
    test_true( s == "hello" );
    test_true( u == "hello" );
    test_pass("cxx11:string copy still copies");
}

TEST_CASE(move03, moved_from_string_is_reusable) {
    STD::string s("some content");
    STD::string t(STD::move(s));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.empty() );

    s = "reused";
    test_true( s == "reused" );
    s += " and appended";
    test_true( s == "reused and appended" );
    test_eq( s.size(), 19u );
    s.clear();
    test_true( s.empty() );
    test_pass("cxx11:moved-from string is reusable");
}

TEST_CASE(move03, wstring_move) {
    STD::wstring s(L"wide characters here, long enough for a heap buffer");
    const wchar_t* raw = s.data();
    STD::wstring t(STD::move(s));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( t.data() == raw );
    test_true( t == L"wide characters here, long enough for a heap buffer" );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.empty() );

    STD::wstring u;
    u = STD::move(t);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( u.data() == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( t.empty() );
    test_pass("cxx11:wstring move");
}

TEST_CASE(move03, set_move_ctor_takes_nodes) {
    STD::set<int> s;
    for (int i = 0; i < 5; ++i) s.insert(i * 10);
    const int* first = &(*s.begin());

    STD::set<int> t(STD::move(s));
    test_eq( t.size(), 5u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &(*t.begin()) == first );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( s.size(), 0u );
    test_true( t.find(30) != t.end() );
    test_true( t.find(99) == t.end() );
    test_pass("cxx11:set move ctor");
}

TEST_CASE(move03, set_move_assign_takes_nodes) {
    STD::set<int> s;
    for (int i = 0; i < 4; ++i) s.insert(i);
    const int* first = &(*s.begin());

    STD::set<int> t;
    t.insert(99);
    t = STD::move(s);
    test_eq( t.size(), 4u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &(*t.begin()) == first );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.empty() );
    test_true( t.find(99) == t.end() );
    test_pass("cxx11:set move assign");
}

TEST_CASE(move03, moved_from_set_is_reusable) {
    STD::set<int> s;
    s.insert(1);
    STD::set<int> t(STD::move(s));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.empty() );

    for (int i = 0; i < 10; ++i) s.insert(i);
    test_eq( s.size(), 10u );
    s.clear();
    test_true( s.empty() );
    test_pass("cxx11:moved-from set is reusable");
}

TEST_CASE(move03, set_insert_moves_the_value) {
    STD::set<STD::string> s;
    STD::string a("a string long enough to own a heap buffer");
    const char* raw = a.data();

    STD::pair<STD::set<STD::string>::iterator, bool> r = s.insert(STD::move(a));
    test_true( r.second );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.begin()->data() == raw );
    test_eq( s.size(), 1u );

    STD::string b("a string long enough to own a heap buffer");
    STD::pair<STD::set<STD::string>::iterator, bool> r2 = s.insert(STD::move(b));
    test_true( !r2.second );
    test_true( !b.empty() );
    test_true( b == "a string long enough to own a heap buffer" );
    test_eq( s.size(), 1u );
    test_pass("cxx11:set insert moves");
}

TEST_CASE(move03, multiset_insert_moves_the_value) {
    STD::multiset<STD::string> s;
    STD::string a("first long enough string for a heap buffer");
    const char* raw = a.data();
    STD::multiset<STD::string>::iterator it = s.insert(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( it->data() == raw );

    STD::string b("first long enough string for a heap buffer");
    s.insert(STD::move(b));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.empty() );
    test_eq( s.size(), 2u );
    test_eq( s.count("first long enough string for a heap buffer"), 2u );
    test_pass("cxx11:multiset insert moves");
}

TEST_CASE(move03, map_move_ctor_takes_nodes) {
    STD::map<int, STD::string> m;
    m[1] = "one";
    m[2] = "two";
    const STD::pair<const int, STD::string>* first = &(*m.begin());

    STD::map<int, STD::string> n(STD::move(m));
    test_eq( n.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &(*n.begin()) == first );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( m.empty() );
    test_true( n[2] == "two" );
    test_pass("cxx11:map move ctor");

    STD::map<int, STD::string> o;
    o = STD::move(n);
    test_eq( o.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( n.empty() );
    test_true( o[1] == "one" );
    test_pass("cxx11:map move assign");
}

TEST_CASE(move03, map_insert_rvalue) {
    STD::map<int, STD::string> m;
    STD::pair<const int, STD::string> v(1, "a long enough value string for heap");

    STD::pair<STD::map<int, STD::string>::iterator, bool> r = m.insert(STD::move(v));
    test_true( r.second );
    test_eq( m.size(), 1u );
    test_true( m[1] == "a long enough value string for heap" );

    STD::pair<const int, STD::string> v2(1, "other but long enough for a heap buffer");
    STD::pair<STD::map<int, STD::string>::iterator, bool> r2 = m.insert(STD::move(v2));
    test_true( !r2.second );
    test_eq( m.size(), 1u );
    test_true( m[1] == "a long enough value string for heap" );
    test_true( v2.second == "other but long enough for a heap buffer" );
    test_pass("cxx11:map insert rvalue");
}

TEST_CASE(move03, multimap_move_and_insert) {
    STD::multimap<int, int> m;
    m.insert(STD::make_pair(1, 10));
    m.insert(STD::make_pair(1, 11));
    test_eq( m.size(), 2u );

    STD::multimap<int, int> n(STD::move(m));
    test_eq( n.size(), 2u );
    test_eq( n.count(1), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( m.empty() );
    test_pass("cxx11:multimap move ctor");

    STD::pair<const int, int> v(2, 20);
    n.insert(STD::move(v));
    test_eq( n.size(), 3u );
    test_eq( n.count(2), 1u );
    test_pass("cxx11:multimap insert rvalue");
}

TEST_CASE(move03, set_with_comparator_survives_move) {

    STD::set<int, STD::greater<int> > s;
    s.insert(1); s.insert(5); s.insert(3);
    test_eq( *s.begin(), 5 );

    STD::set<int, STD::greater<int> > t(STD::move(s));
    test_eq( t.size(), 3u );
    test_eq( *t.begin(), 5 );
    t.insert(9);
    test_eq( *t.begin(), 9 );
    test_pass("cxx11:set with comparator survives move");
}

#if defined(__WATCOMC__)
TEST_CASE(move03, libcxx03_tree_valid_after_move) {
    STD::set<int> s;
    for (int i = 0; i < 50; ++i) s.insert(i * 7 % 50);
    test_true( s.__tree_rb_valid() );

    STD::set<int> t(STD::move(s));
    test_true( t.__tree_rb_valid() );
    test_true( s.__tree_rb_valid() );

    for (int i = 0; i < 20; ++i) s.insert(i);
    test_true( s.__tree_rb_valid() );

    STD::set<int> u;
    u.insert(1);
    u = STD::move(t);
    test_true( u.__tree_rb_valid() );
    test_pass("cxx11:set RB-tree valid after move");
}
#else
TEST_CASE_SKIP(move03, libcxx03_tree_valid_after_move)
#endif

TEST_CASE(move03, unordered_set_move_takes_nodes) {
    STD::unordered_set<int> s;
    for (int i = 0; i < 20; ++i) s.insert(i);

    STD::unordered_set<int> t(STD::move(s));
    test_eq( t.size(), 20u );
    test_true( s.empty() );
    test_eq( s.size(), 0u );
    for (int i = 0; i < 20; ++i) test_true( t.find(i) != t.end() );
    test_true( t.find(99) == t.end() );

    s.insert(5);
    test_eq( s.size(), 1u );
    test_true( s.find(5) != s.end() );
    test_pass("cxx11:unordered_set move ctor");

    STD::unordered_set<int> u;
    u.insert(77);
    u = STD::move(t);
    test_eq( u.size(), 20u );
    test_true( t.empty() );
    test_true( u.find(77) == u.end() );
    test_true( u.find(19) != u.end() );
    test_pass("cxx11:unordered_set move assign");
}

TEST_CASE(move03, unordered_set_insert_moves_the_value) {
    STD::unordered_set<STD::string> s;
    STD::string a("a string long enough to own a heap buffer");
    const char* raw = a.data();

    STD::pair<STD::unordered_set<STD::string>::iterator, bool> r = s.insert(STD::move(a));
    test_true( r.second );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( r.first->data() == raw );

    STD::string b("a string long enough to own a heap buffer");
    STD::pair<STD::unordered_set<STD::string>::iterator, bool> r2 = s.insert(STD::move(b));
    test_true( !r2.second );
    test_true( b == "a string long enough to own a heap buffer" );
    test_eq( s.size(), 1u );
    test_pass("cxx11:unordered_set insert moves");
}

TEST_CASE(move03, unordered_multiset_insert_moves_the_value) {
    STD::unordered_multiset<STD::string> s;
    STD::string a("long enough string for a heap buffer");
    const char* raw = a.data();
    STD::unordered_multiset<STD::string>::iterator it = s.insert(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( it->data() == raw );

    STD::string b("long enough string for a heap buffer");
    s.insert(STD::move(b));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.empty() );
    test_eq( s.size(), 2u );
    test_eq( s.count("long enough string for a heap buffer"), 2u );
    test_pass("cxx11:unordered_multiset insert moves");
}

TEST_CASE(move03, unordered_map_move_and_insert) {
    STD::unordered_map<int, STD::string> m;
    m[1] = "one";
    m[2] = "two";

    STD::unordered_map<int, STD::string> n(STD::move(m));
    test_eq( n.size(), 2u );
    test_true( m.empty() );
    test_true( n[1] == "one" );
    test_true( n[2] == "two" );
    test_pass("cxx11:unordered_map move ctor");

    STD::unordered_map<int, STD::string> o;
    o = STD::move(n);
    test_eq( o.size(), 2u );
    test_true( n.empty() );
    test_true( o[2] == "two" );
    test_pass("cxx11:unordered_map move assign");

    STD::pair<const int, STD::string> v(3, "three");
    STD::pair<STD::unordered_map<int, STD::string>::iterator, bool> r = o.insert(STD::move(v));
    test_true( r.second );
    test_true( o[3] == "three" );
    STD::pair<const int, STD::string> v2(3, "other");
    test_true( !o.insert(STD::move(v2)).second );
    test_true( o[3] == "three" );
    test_pass("cxx11:unordered_map insert rvalue");
}

TEST_CASE(move03, unordered_multimap_move_and_insert) {
    STD::unordered_multimap<int, int> m;
    m.insert(STD::make_pair(1, 10));
    m.insert(STD::make_pair(1, 11));
    test_eq( m.size(), 2u );

    STD::unordered_multimap<int, int> n(STD::move(m));
    test_eq( n.size(), 2u );
    test_eq( n.count(1), 2u );
    test_true( m.empty() );
    test_pass("cxx11:unordered_multimap move ctor");

    STD::pair<const int, int> v(2, 20);
    n.insert(STD::move(v));
    test_eq( n.size(), 3u );
    test_eq( n.count(2), 1u );
    test_pass("cxx11:unordered_multimap insert rvalue");
}

TEST_CASE(move03, moved_from_unordered_map_rehashes_fine) {
    STD::unordered_map<int, int> m;
    for (int i = 0; i < 50; ++i) m[i] = i * 2;
    STD::unordered_map<int, int> n(STD::move(m));
    test_true( m.empty() );

    for (int i = 0; i < 100; ++i) m[i] = i;
    test_eq( m.size(), 100u );
    for (int i = 0; i < 100; ++i) test_eq( m[i], i );
    test_eq( n.size(), 50u );
    test_eq( n[49], 98 );
    test_pass("cxx11:moved-from unordered_map rehashes");
}

TEST_CASE(move03, stack_move_of_container) {
    STD::vector<buf> v;
    v.reserve(4);
    v.push_back(buf(1));
    v.push_back(buf(2));
    const buf* raw = &v[0];
    reset();

    STD::stack<buf, STD::vector<buf> > s(STD::move(v));

    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( s.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( &s.top() == raw + 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );
    test_eq( s.top().value(), 2 );
    test_pass("cxx11:stack move of container");
}

TEST_CASE(move03, stack_push_moves_the_element) {
    STD::stack<buf, STD::vector<buf> > s;
    buf a(7);
    int* raw = a.p_;
    reset();

    s.push(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s.top().p_ == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    test_eq( s.size(), 1u );

    buf b(8);
    reset();
    s.push(b);
#if !_TST_HAS_CXX11_LIB_MEMBERS || defined(__WATCOMC__)
    test_eq( g_moves, 0 );
#else
    TEST_SKIP1();
#endif
    test_true( g_copies >= 1 );
    test_eq( b.value(), 8 );
    test_eq( s.top().value(), 8 );
    test_pass("cxx11:stack push moves");
}

TEST_CASE(move03, queue_move_of_container) {
    STD::deque<buf> d;
    d.push_back(buf(1));
    d.push_back(buf(2));
    reset();

    STD::queue<buf, STD::deque<buf> > q(STD::move(d));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( g_moves, 0 );
    test_eq( q.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.empty() );
    test_eq( q.front().value(), 1 );
    test_eq( q.back().value(), 2 );
    test_pass("cxx11:queue move of container");
}

TEST_CASE(move03, queue_push_moves_the_element) {
    STD::queue<buf, STD::deque<buf> > q;
    buf a(5);
    int* raw = a.p_;
    reset();

    q.push(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( q.back().p_ == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    test_eq( q.front().value(), 5 );

    q.pop();
    test_true( q.empty() );
    test_pass("cxx11:queue push moves");
}

TEST_CASE(move03, priority_queue_move_and_push) {

    STD::vector<int> v;
    v.push_back(3); v.push_back(1); v.push_back(4); v.push_back(1); v.push_back(5);
    STD::priority_queue<int, STD::vector<int>, STD::less<int> > pq(STD::less<int>(), STD::move(v));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );
    test_eq( pq.size(), 5u );
    test_eq( pq.top(), 5 );
    test_pass("cxx11:priority_queue move ctor");

    pq.push(9);
    test_eq( pq.top(), 9 );
    pq.pop();
    test_eq( pq.top(), 5 );

    STD::priority_queue<STD::string> ps;
    STD::string a("zzz a long enough string for a heap buffer");
    ps.push(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );
    ps.push(STD::string("aaa"));
    test_eq( ps.size(), 2u );
    test_true( ps.top() == "zzz a long enough string for a heap buffer" );
    test_pass("cxx11:priority_queue push moves");
}

TEST_CASE(move03, stack_queue_of_scalars_unaffected) {
    STD::stack<int> s;
    s.push(1); s.push(2);
    test_eq( s.top(), 2 );
    test_eq( s.size(), 2u );

    STD::vector<int> v;
    v.push_back(7);
    STD::stack<int, STD::vector<int> > s2(STD::move(v));
    test_eq( s2.top(), 7 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.empty() );
    test_pass("cxx11:stack of scalars unaffected");

    STD::queue<int> q;
    q.push(1); q.push(2);
    test_eq( q.front(), 1 );
    test_eq( q.back(), 2 );
    test_pass("cxx11:queue of scalars unaffected");
}

TEST_CASE(move03, unique_ptr_move_transfers_ownership) {
    STD::unique_ptr<buf> a(new buf(1));
    buf* raw = a.get();

    STD::unique_ptr<buf> b(STD::move(a));
    test_true( b.get() == raw );
    test_true( a.get() == 0 );
    test_eq( b->value(), 1 );
    test_pass("cxx11:unique_ptr move ctor");

    STD::unique_ptr<buf> c(new buf(2));
    buf* raw2 = c.get();
    STD::unique_ptr<buf> d;
    d = STD::move(c);
    test_true( d.get() == raw2 );
    test_true( c.get() == 0 );
    test_eq( d->value(), 2 );
    test_pass("cxx11:unique_ptr move assign");
}

namespace { STD::unique_ptr<buf> make_buf(int v) { STD::unique_ptr<buf> p(new buf(v)); return p; } }

TEST_CASE(move03, unique_ptr_return_by_value) {
    STD::unique_ptr<buf> p = make_buf(7);
    test_true( p.get() != 0 );
    test_eq( p->value(), 7 );

    STD::unique_ptr<buf> q = STD::move(p);
    test_true( p.get() == 0 );
    test_eq( q->value(), 7 );
    test_pass("cxx11:unique_ptr return by value");
}

TEST_CASE(move03, unique_ptr_reset_release_swap) {
    STD::unique_ptr<buf> a(new buf(1));
    STD::unique_ptr<buf> b(new buf(2));
    a.swap(b);
    test_eq( a->value(), 2 );
    test_eq( b->value(), 1 );
    test_pass("cxx11:unique_ptr swap");

    buf* r = a.release();
    test_true( a.get() == 0 );
    test_eq( r->value(), 2 );
    delete r;
    test_pass("cxx11:unique_ptr release");

    b.reset(new buf(9));
    test_eq( b->value(), 9 );
    b.reset();
    test_true( b.get() == 0 );
    test_pass("cxx11:unique_ptr reset");
}

TEST_CASE(move03, shared_ptr_move_does_not_touch_refcount) {
    STD::shared_ptr<buf> a(new buf(1));
    buf* raw = a.get();
    test_eq( a.use_count(), 1L );

    STD::shared_ptr<buf> b(STD::move(a));
    test_true( b.get() == raw );
    test_eq( b.use_count(), 1L );
    test_true( a.get() == 0 );
    test_eq( a.use_count(), 0L );
    test_pass("cxx11:shared_ptr move ctor");

    STD::shared_ptr<buf> c(new buf(2));
    STD::shared_ptr<buf> d;
    d = STD::move(c);
    test_eq( d.use_count(), 1L );
    test_true( c.get() == 0 );
    test_eq( d->value(), 2 );
    test_pass("cxx11:shared_ptr move assign");
}

TEST_CASE(move03, shared_ptr_copy_still_shares) {
    STD::shared_ptr<buf> a(new buf(1));
    STD::shared_ptr<buf> b(a);
    test_eq( a.use_count(), 2L );
    test_eq( b.use_count(), 2L );
    test_true( a.get() == b.get() );

    STD::shared_ptr<buf> c(STD::move(b));
    test_eq( a.use_count(), 2L );
    test_eq( c.use_count(), 2L );
    test_true( b.get() == 0 );
    test_pass("cxx11:shared_ptr copy shares");
}

TEST_CASE(move03, shared_ptr_move_releases_old_target) {
    STD::shared_ptr<buf> keep(new buf(1));
    STD::shared_ptr<buf> a(keep);
    test_eq( keep.use_count(), 2L );

    STD::shared_ptr<buf> b(new buf(2));
    a = STD::move(b);
    test_eq( keep.use_count(), 1L );
    test_eq( a->value(), 2 );
    test_eq( a.use_count(), 1L );
    test_true( b.get() == 0 );
    test_pass("cxx11:shared_ptr move releases old target");
}

TEST_CASE(move03, weak_ptr_move) {
    STD::shared_ptr<buf> s(new buf(5));
    STD::weak_ptr<buf> a(s);
    test_true( !a.expired() );

    STD::weak_ptr<buf> b(STD::move(a));
    test_true( !b.expired() );
    test_eq( b.use_count(), 1L );
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC before VS2015 has no weak_ptr move constructor");
#endif
    test_true( a.expired() );
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC before VS2015 has no weak_ptr move constructor");
#endif
    test_eq( a.use_count(), 0L );

    STD::shared_ptr<buf> locked = b.lock();
    test_true( locked.get() == s.get() );
    test_eq( locked->value(), 5 );

    STD::weak_ptr<buf> c;
    c = STD::move(b);
    test_true( !c.expired() );
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC before VS2015 has no weak_ptr move constructor");
#endif
    test_true( b.expired() );

    locked.reset();
    s.reset();
    test_true( c.expired() );
    test_true( !c.lock() );
    test_pass("cxx11:weak_ptr move");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(move03, optional_move_ctor_moves_the_value) {
    STD::optional<buf> a(buf(3));
    int* raw = a->p_;
    reset();

    STD::optional<buf> b(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( b->p_ == raw );
    test_true( b.has_value() );

    test_true( a.has_value() );
    test_true( a->empty() );
    test_pass("cxx11:optional move ctor");
}

TEST_CASE(move03, optional_move_assign) {
    STD::optional<buf> a(buf(1));
    int* raw = a->p_;

    STD::optional<buf> b;
    reset();
    b = STD::move(a);
    test_eq( g_moves, 1 );
    test_true( b->p_ == raw );
    test_true( b.has_value() );

    STD::optional<buf> c(buf(2));
    int* raw2 = c->p_;
    reset();
    b = STD::move(c);
    test_eq( g_moves, 1 );
    test_true( b->p_ == raw2 );

    STD::optional<buf> d;
    b = STD::move(d);
    test_true( !b.has_value() );
    test_pass("cxx11:optional move assign");
}

TEST_CASE(move03, optional_construct_from_moved_value) {
    buf a(9);
    int* raw = a.p_;
    reset();

    STD::optional<buf> o(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( o->p_ == raw );
    test_true( a.empty() );

    buf b(10);
    int* rawb = b.p_;
    reset();
    o = STD::move(b);
    test_eq( g_moves, 1 );
    test_true( o->p_ == rawb );
    test_true( b.empty() );
    test_pass("cxx11:optional construct from moved value");
}

TEST_CASE(move03, optional_copy_still_copies) {
    STD::optional<buf> a(buf(4));
    reset();
    STD::optional<buf> b(a);
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_true( b->p_ != a->p_ );
    test_eq( a->value(), 4 );
    test_eq( b->value(), 4 );
    test_pass("cxx11:optional copy still copies");
}

TEST_CASE(move03, optional_of_scalar_and_string) {
    STD::optional<int> i(5);
    STD::optional<int> j(STD::move(i));
    test_eq( *j, 5 );
    test_true( i.has_value() );

    STD::optional<STD::string> s(STD::string("a long enough string for a heap buffer"));
    const char* raw = s->data();
    STD::optional<STD::string> t(STD::move(s));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( t->data() == raw );
    test_true( t.has_value() );
    test_true( s.has_value() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( s->empty() );

    STD::optional<STD::string> u = STD::make_optional(STD::string("made"));
    test_true( *u == "made" );
    test_pass("cxx11:optional of scalar and string move");
}
#else
TEST_CASE(move03, optional_move_ctor_moves_the_value) {
    TEST_NOTE("<optional> is C++17");
    test_skip("cxx11:optional move ctor");
}
TEST_CASE(move03, optional_move_assign) {
    TEST_NOTE("<optional> is C++17");
    test_skip("cxx11:optional move assign");
}
TEST_CASE(move03, optional_construct_from_moved_value) {
    TEST_NOTE("<optional> is C++17");
    test_skip("cxx11:optional construct from moved value");
}
TEST_CASE(move03, optional_copy_still_copies) {
    TEST_NOTE("<optional> is C++17");
    test_skip("cxx11:optional copy still copies");
}
TEST_CASE(move03, optional_of_scalar_and_string) {
    TEST_NOTE("<optional> is C++17");
    test_skip("cxx11:optional of scalar and string move");
}
#endif

#if !defined(__GNUC__)

TEST_CASE(move03, unexpected_move) {
    STD::unexpected<buf> a(buf(3));
    int* raw = a.error().p_;
    reset();

    STD::unexpected<buf> b(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( b.error().p_ == raw );
    test_true( a.error().empty() );
    test_pass("cxx11:unexpected move ctor");

    STD::unexpected<buf> c(buf(4));
    int* raw2 = c.error().p_;
    reset();
    b = STD::move(c);
    test_eq( g_moves, 1 );
    test_true( b.error().p_ == raw2 );
    test_true( c.error().empty() );
    test_pass("cxx11:unexpected move assign");
}

TEST_CASE(move03, expected_move_ctor_keeps_the_alternative) {
    STD::expected<buf, int> a(buf(1));
    int* raw = a->p_;
    reset();

    STD::expected<buf, int> b(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( b.has_value() );
    test_true( b->p_ == raw );

    test_true( a.has_value() );
    test_true( a->empty() );
    test_pass("cxx11:expected move ctor");
}

TEST_CASE(move03, expected_move_of_error_state) {
    STD::expected<int, buf> a(STD::unexpected<buf>(buf(9)));
    test_true( !a.has_value() );
    int* raw = a.error().p_;
    reset();

    STD::expected<int, buf> b(STD::move(a));
    test_eq( g_moves, 1 );
    test_true( !b.has_value() );
    test_true( b.error().p_ == raw );
    test_true( a.error().empty() );
    test_pass("cxx11:expected move of error state");
}

TEST_CASE(move03, expected_move_assign) {
    STD::expected<buf, int> a(buf(1));
    int* raw = a->p_;
    STD::expected<buf, int> b(buf(2));
    reset();

    b = STD::move(a);
    test_eq( g_moves, 1 );
    test_true( b.has_value() );
    test_true( b->p_ == raw );

    b = STD::unexpected<int>(7);
    test_true( !b.has_value() );
    test_eq( b.error(), 7 );
    test_pass("cxx11:expected move assign");
}

TEST_CASE(move03, expected_construct_from_moved_value) {
    buf a(5);
    int* raw = a.p_;
    reset();

    STD::expected<buf, int> e(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_true( e.has_value() );
    test_true( e->p_ == raw );
    test_true( a.empty() );

    buf b(6);
    int* rawb = b.p_;
    reset();
    e = STD::move(b);
    test_eq( g_moves, 1 );
    test_true( e->p_ == rawb );
    test_true( b.empty() );
    test_pass("cxx11:expected construct from moved value");
}

TEST_CASE(move03, expected_copy_still_copies) {
    STD::expected<buf, int> a(buf(4));
    reset();
    STD::expected<buf, int> b(a);
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_true( b->p_ != a->p_ );
    test_eq( a->value(), 4 );
    test_eq( b->value(), 4 );
    test_pass("cxx11:expected copy still copies");
}

TEST_CASE(move03, expected_of_scalars_unaffected) {
    STD::expected<int, int> a(5);
    test_true( a.has_value() );
    test_eq( *a, 5 );

    STD::expected<int, int> b(STD::move(a));
    test_eq( *b, 5 );

    STD::expected<int, int> c(STD::unexpected<int>(3));
    test_true( !c.has_value() );
    test_eq( c.error(), 3 );

    STD::expected<STD::string, int> s(STD::string("a long enough string for heap"));
    const char* raw = s->data();
    STD::expected<STD::string, int> t(STD::move(s));
    TEST_SKIP_VC090("the value here is std::string -- MSVC 9's own, and it cannot be moved");
    test_true( t->data() == raw );
    TEST_SKIP_VC090("the value here is std::string -- MSVC 9's own, and it cannot be moved");
    test_true( s->empty() );
    test_pass("cxx11:expected of scalars unaffected");
}

TEST_CASE(move03, flat_set_move) {
    STD::flat_set<int> a;
    for (int i = 5; i > 0; --i) a.insert(i);
    test_eq( a.size(), 5u );
    const int* raw = &(*a.begin());

    STD::flat_set<int> b(STD::move(a));
    test_eq( b.size(), 5u );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( &(*b.begin()) == raw );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( a.empty() );
    test_eq( *b.begin(), 1 );
    test_true( b.find(3) != b.end() );
    test_pass("cxx11:flat_set move ctor");

    STD::flat_set<int> c;
    c.insert(99);
    c = STD::move(b);
    test_eq( c.size(), 5u );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( b.empty() );
    test_true( c.find(99) == c.end() );

    b.insert(7);
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_eq( b.size(), 1u );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_eq( *b.begin(), 7 );
    test_pass("cxx11:flat_set move assign");
}

TEST_CASE(move03, flat_set_insert_moves_the_value) {
    STD::flat_set<STD::string> s;
    STD::string a("a string long enough to own a heap buffer");
    const char* raw = a.data();

    STD::pair<STD::flat_set<STD::string>::iterator, bool> r = s.insert(STD::move(a));
    test_true( r.second );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( a.empty() );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( s.begin()->data() == raw );

    STD::string b("a string long enough to own a heap buffer");
    STD::pair<STD::flat_set<STD::string>::iterator, bool> r2 = s.insert(STD::move(b));
    test_true( !r2.second );
    test_true( b == "a string long enough to own a heap buffer" );
    test_eq( s.size(), 1u );
    test_pass("cxx11:flat_set insert moves");
}

TEST_CASE(move03, flat_map_move) {
    STD::flat_map<int, STD::string> m;
    m[2] = "two";
    m[1] = "one";
    test_eq( m.size(), 2u );

    STD::flat_map<int, STD::string> n(STD::move(m));
    test_eq( n.size(), 2u );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( m.empty() );
    test_true( n[1] == "one" );
    test_true( n[2] == "two" );
    test_true( n.begin()->first == 1 );
    test_pass("cxx11:flat_map move ctor");

    STD::flat_map<int, STD::string> o;
    o[9] = "nine";
    o = STD::move(n);
    test_eq( o.size(), 2u );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( n.empty() );
    test_true( o.find(9) == o.end() );

    n[5] = "five";
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_eq( n.size(), 1u );
    test_true( n[5] == "five" );
    test_pass("cxx11:flat_map move assign");
}

TEST_CASE(move03, flat_map_insert_rvalue) {

    STD::flat_map<int, STD::string> m;
    STD::pair<int, STD::string> v(2, "two");
    STD::pair<STD::flat_map<int, STD::string>::iterator, bool> r = m.insert(STD::move(v));
    test_true( r.second );

    STD::pair<int, STD::string> v0(1, "one");
    test_true( m.insert(STD::move(v0)).second );
    test_eq( m.size(), 2u );
    test_true( m.begin()->first == 1 );
    test_true( m[2] == "two" );

    STD::pair<int, STD::string> dup(1, "other");
    test_true( !m.insert(STD::move(dup)).second );
    test_true( m[1] == "one" );
    test_eq( m.size(), 2u );
    test_pass("cxx11:flat_map insert rvalue");
}

#else
TEST_CASE_SKIP(move03, unexpected_move)
TEST_CASE_SKIP(move03, expected_move_ctor_keeps_the_alternative)
TEST_CASE_SKIP(move03, expected_move_of_error_state)
TEST_CASE_SKIP(move03, expected_move_assign)
TEST_CASE_SKIP(move03, expected_construct_from_moved_value)
TEST_CASE_SKIP(move03, expected_copy_still_copies)
TEST_CASE_SKIP(move03, expected_of_scalars_unaffected)
TEST_CASE_SKIP(move03, flat_set_move)
TEST_CASE_SKIP(move03, flat_set_insert_moves_the_value)
TEST_CASE_SKIP(move03, flat_map_move)
TEST_CASE_SKIP(move03, flat_map_insert_rvalue)
#endif

namespace { void ccw_mv_thread_fn() {} }

#if _TST_HAS_OS_THREADS
TEST_CASE(move03, thread_move_transfers_the_handle) {
    STD::thread a(ccw_mv_thread_fn);
    test_true( a.joinable() );
    STD::thread::id id = a.get_id();

    STD::thread b(STD::move(a));
    test_true( b.joinable() );
    test_true( !a.joinable() );
    test_true( b.get_id() == id );
    test_true( a.get_id() == STD::thread::id() );
    b.join();
    test_true( !b.joinable() );
    test_pass("cxx11:thread move ctor");
}
#else
TEST_CASE_SKIP(move03, thread_move_transfers_the_handle)
#endif

#if _TST_HAS_OS_THREADS
TEST_CASE(move03, thread_move_assign) {
    STD::thread a(ccw_mv_thread_fn);
    STD::thread::id id = a.get_id();

    STD::thread b;
    test_true( !b.joinable() );
    b = STD::move(a);
    test_true( b.joinable() );
    test_true( !a.joinable() );
    test_true( b.get_id() == id );
    b.join();

    STD::thread c(ccw_mv_thread_fn);
    a = STD::move(c);
    test_true( a.joinable() );
    test_true( !c.joinable() );
    a.join();
    test_pass("cxx11:thread move assign");
}
#else
TEST_CASE_SKIP(move03, thread_move_assign)
#endif

#if _TST_HAS_OS_THREADS
TEST_CASE(move03, thread_swap) {
    STD::thread a(ccw_mv_thread_fn);
    STD::thread b;
    STD::thread::id ida = a.get_id();

    a.swap(b);
    test_true( !a.joinable() );
    test_true( b.joinable() );
    test_true( b.get_id() == ida );
    b.join();
    test_pass("cxx11:thread swap");
}
#else
TEST_CASE_SKIP(move03, thread_swap)
#endif

#if !defined(_MSC_VER) || _MSC_VER >= 1700
#if _TST_HAS_OS_THREADS
TEST_CASE(move03, promise_future_move) {
    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    test_true( f.valid() );

    STD::future<int> g(STD::move(f));
    test_true( g.valid() );
    test_true( !f.valid() );
    p.set_value(42);

    int gv = g.get();
    test_eq( gv, 42 );
    test_true( !g.valid() );
    test_pass("cxx11:future move ctor");

    STD::promise<int> p2;
    STD::future<int> f2 = p2.get_future();
    STD::future<int> h;
    test_true( !h.valid() );
    h = STD::move(f2);
    test_true( h.valid() );
    test_true( !f2.valid() );
    p2.set_value(7);
    int hv = h.get();
    test_eq( hv, 7 );
    test_pass("cxx11:future move assign");
}
#else
TEST_CASE_SKIP(move03, promise_future_move)
#endif

#if _TST_HAS_OS_THREADS
TEST_CASE(move03, promise_move) {
    STD::promise<int> a;
    STD::future<int> f = a.get_future();

    STD::promise<int> b(STD::move(a));
    b.set_value(5);
    int fv = f.get();
    test_eq( fv, 5 );
    test_pass("cxx11:promise move ctor");

    STD::promise<int> c;
    STD::future<int> g = c.get_future();
    STD::promise<int> d;
    d = STD::move(c);
    d.set_value(9);
    int gv2 = g.get();
    test_eq( gv2, 9 );
    test_pass("cxx11:promise move assign");
}
#else
TEST_CASE_SKIP(move03, promise_move)
#endif

#if _TST_HAS_OS_THREADS
TEST_CASE(move03, shared_future_move) {
    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    p.set_value(3);

    STD::shared_future<int> sf(STD::move(f));
    test_true( sf.valid() );
    test_true( !f.valid() );

    int v1 = sf.get(); test_eq( v1, 3 );
    int v2 = sf.get(); test_eq( v2, 3 );

    STD::shared_future<int> sg(STD::move(sf));
    test_true( sg.valid() );
    test_true( !sf.valid() );
    int v3 = sg.get(); test_eq( v3, 3 );

    STD::shared_future<int> sh;
    sh = STD::move(sg);
    test_true( sh.valid() );
    test_true( !sg.valid() );
    int v4 = sh.get(); test_eq( v4, 3 );
    test_pass("cxx11:shared_future move");
}
#else
TEST_CASE_SKIP(move03, shared_future_move)
#endif

#if _TST_HAS_OS_THREADS
TEST_CASE(move03, shared_future_copy_still_shares) {
    STD::promise<int> p;
    STD::future<int> f = p.get_future();
    STD::shared_future<int> a(STD::move(f));
    p.set_value(11);

    STD::shared_future<int> b(a);
    test_true( a.valid() );
    test_true( b.valid() );
    int av = a.get(); test_eq( av, 11 );
    int bv = b.get(); test_eq( bv, 11 );
    test_pass("cxx11:shared_future copy shares");
}
#else
TEST_CASE_SKIP(move03, shared_future_copy_still_shares)
#endif

#else
TEST_CASE_SKIP(move03, promise_future_move)
TEST_CASE_SKIP(move03, promise_move)
TEST_CASE_SKIP(move03, shared_future_move)
TEST_CASE_SKIP(move03, shared_future_copy_still_shares)
#endif

#if TEST_TARGET_CXX >= 2014
TEST_CASE(move03, transparent_comparators) {
    STD::less<void> lt;
    test_true( lt(1, 2L) );
    test_true( !lt(2.5, 1) );
    test_true( lt(STD::string("a"), "b") );
    test_true( lt(1, 2) );

    STD::greater<void> gt;
    test_true( gt(3L, 1) );
    test_true( !gt(1, 3) );

    STD::equal_to<void> eq;
    test_true( eq(1, 1L) );
    test_true( !eq(1, 2) );
    test_true( eq(STD::string("x"), "x") );

    STD::not_equal_to<void> ne;
    test_true( ne(1, 2L) );
    test_true( !ne(5, 5) );

    STD::less_equal<void> le;
    test_true( le(1, 1L) );
    test_true( le(1, 2L) );
    test_true( !le(3, 2L) );

    STD::greater_equal<void> ge;
    test_true( ge(1, 1L) );
    test_true( ge(2, 1L) );
    test_true( !ge(1, 2L) );

    STD::logical_and<void> la;
    test_true( la(true, 1) );
    test_true( !la(true, 0) );

    STD::logical_or<void> lo;
    test_true( lo(false, 1) );
    test_true( !lo(false, 0) );

    STD::logical_not<void> ln;
    test_true( ln(0) );
    test_true( !ln(5) );

#if _TST_HAS_CXX14_LIB_MEMBERS
    typedef STD::less<void>::is_transparent __t;
#endif
    test_true( sizeof(STD::less<void>) >= 1 );
    test_pass("cxx11:transparent comparators");
}
#else
TEST_CASE(move03, transparent_comparators) {
    TEST_NOTE("the transparent comparators (less<void> etc.) are C++14");
    test_skip("cxx11:transparent comparators");
}
#endif

namespace {

int g_fn_copies = 0;
struct counting_callable {
    int v_;
    explicit counting_callable(int v) : v_(v) {}
    counting_callable(const counting_callable& o) : v_(o.v_) { ++g_fn_copies; }
    int operator()() const { return v_; }
    int operator()(int a) const { return v_ + a; }
};
}

TEST_CASE(move03, function_move_takes_the_target) {
    STD::function<int()> f((counting_callable(7)));
    g_fn_copies = 0;

    STD::function<int()> g(STD::move(f));
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC before VS2015 copies a small function target on move");
#endif
    test_eq( g_fn_copies, 0 );
    test_true( g );
    test_true( !f );
    test_eq( g(), 7 );

    g_fn_copies = 0;
    STD::function<int()> h(g);
    test_true( g_fn_copies >= 1 );
    test_true( g );
    test_eq( h(), 7 );
    test_eq( g(), 7 );
    test_pass("cxx11:function move ctor");
}

TEST_CASE(move03, function_move_assign) {
    STD::function<int()> f((counting_callable(3)));
    STD::function<int()> g((counting_callable(9)));
    g_fn_copies = 0;

    g = STD::move(f);
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC before VS2015 copies a small function target on move");
#endif
    test_eq( g_fn_copies, 0 );
    test_eq( g(), 3 );
    test_true( !f );

    STD::function<int()> e;
    g = STD::move(e);
    test_true( !g );
    test_pass("cxx11:function move assign");
}

TEST_CASE(move03, function_move_with_args) {

    STD::function<int(int)> src((counting_callable(10)));
    g_fn_copies = 0;
    STD::function<int(int)> g(STD::move(src));
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC before VS2015 copies a small function target on move");
#endif
    test_eq( g_fn_copies, 0 );
    int r = g(5);
    test_eq( r, 15 );
    test_true( !src );

    test_throw( src(1) );
    test_pass("cxx11:function move with args");
}

TEST_CASE(move03, move_iterator_moves_elements) {
    STD::vector<buf> src;
    src.reserve(3);
    for (int i = 0; i < 3; ++i) src.push_back(buf(i));
    int* raw0 = src[0].p_;
    int* raw2 = src[2].p_;

    STD::vector<buf> dst;
    dst.reserve(3);
    reset();
    STD::copy(STD::make_move_iterator(src.begin()),
              STD::make_move_iterator(src.end()),
              STD::back_inserter(dst));

    test_eq( dst.size(), 3u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 3 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( dst[0].p_ == raw0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( dst[2].p_ == raw2 );
    test_eq( dst[2].value(), 2 );

    test_eq( src.size(), 3u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( src[0].empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( src[2].empty() );
    test_pass("cxx11:move_iterator moves elements");
}

TEST_CASE(move03, move_iterator_range_ctor) {

    STD::vector<buf> src;
    src.reserve(3);
    for (int i = 0; i < 3; ++i) src.push_back(buf(i));
    reset();

    STD::vector<buf> dst(STD::make_move_iterator(src.begin()),
                         STD::make_move_iterator(src.end()));
    test_eq( dst.size(), 3u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 3 );
    test_eq( dst[0].value(), 0 );
    test_eq( dst[2].value(), 2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( src[0].empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( src[2].empty() );
    test_pass("cxx11:move_iterator range ctor");
}

TEST_CASE(move03, move_iterator_basics) {
    STD::vector<int> v;
    for (int i = 0; i < 4; ++i) v.push_back(i * 10);

    STD::vector<int>::iterator b = v.begin();
    STD::move_iterator<STD::vector<int>::iterator> m(b);
    test_eq( *m, 0 );
    ++m;
    test_eq( *m, 10 );
    test_eq( m[1], 20 );
    --m;
    test_true( m.base() == b );

    STD::move_iterator<STD::vector<int>::iterator> e =
        STD::make_move_iterator(v.end());
    test_eq( e - m, 4 );
    test_true( m != e );
    test_true( m < e );
    test_true( e > m );
    test_true( m <= e );
    test_true( m + 4 == e );

    STD::vector<int> w(STD::make_move_iterator(v.begin()),
                       STD::make_move_iterator(v.end()));
    test_eq( w.size(), 4u );
    test_eq( w[3], 30 );
    test_eq( v[3], 30 );
    test_pass("cxx11:move_iterator basics");
}

TEST_CASE(move03, move_iterator_into_list) {
    STD::list<buf> src;
    src.push_back(buf(1));
    src.push_back(buf(2));
    reset();

    STD::list<buf> dst(STD::make_move_iterator(src.begin()),
                       STD::make_move_iterator(src.end()));
    test_eq( dst.size(), 2u );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( g_moves >= 2 );
    test_eq( dst.front().value(), 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( src.front().empty() );
    test_pass("cxx11:move_iterator into list");
}

TEST_CASE(move03, insert_iterators_forward_the_move) {
    STD::vector<buf> v;
    v.reserve(4);
    buf a(5);
    int* raw = a.p_;
    reset();

    STD::back_insert_iterator<STD::vector<buf> > out = STD::back_inserter(v);
    *out++ = STD::move(a);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v[0].p_ == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.empty() );

    buf b(6);
    reset();
    *out++ = b;
    test_eq( g_copies, 1 );
    test_eq( g_moves, 0 );
    test_eq( b.value(), 6 );
    test_pass("cxx11:back_insert_iterator forwards move");

    STD::list<buf> l;
    buf c(7);
    int* rawc = c.p_;
    reset();
    STD::front_insert_iterator<STD::list<buf> > fout = STD::front_inserter(l);
    *fout++ = STD::move(c);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.front().p_ == rawc );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( c.empty() );
    test_pass("cxx11:front_insert_iterator forwards move");

    buf d(8);
    int* rawd = d.p_;
    reset();
    STD::insert_iterator<STD::list<buf> > iout(l, l.begin());
    *iout++ = STD::move(d);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( l.front().p_ == rawd );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.empty() );
    test_eq( l.size(), 2u );
    test_pass("cxx11:insert_iterator forwards move");
}

TEST_CASE(move03, move_of_enum_is_a_copy) {

    color c = blue;
    color d(STD::move(c));
    test_eq( (int)d, (int)blue );
    test_eq( (int)c, (int)blue );

    color e = relocate(c);
    test_eq( (int)e, (int)blue );

    STD::vector<color> v;
    v.push_back(red);
    v.push_back(STD::move(d));
    test_eq( v.size(), 2u );
    test_eq( (int)v.back(), (int)blue );
    test_pass("cxx11:STD::move of enum is a copy");
}

#if defined(__WATCOMC__)
TEST_CASE(move03, is_class_classification) {

    test_true( _ccw::__ccw_is_class<buf>::value );
    test_true( _ccw::__ccw_is_class<STD::string>::value );
    test_true( _ccw::__ccw_is_class<STD::vector<int> >::value );
    test_true( !_ccw::__ccw_is_class<int>::value );
    test_true( !_ccw::__ccw_is_class<double>::value );
    test_true( !_ccw::__ccw_is_class<color>::value );
    test_true( !_ccw::__ccw_is_class<int*>::value );
    test_true( !_ccw::__ccw_is_class<char>::value );
    test_pass("cxx11:__ccw_is_class classification");
}
#else
TEST_CASE_SKIP(move03, is_class_classification)
#endif

TEST_CASE(move03, pair_move_is_member_wise) {
    STD::pair<buf, buf> a((buf(1)), buf(2));
    int* raw1 = a.first.p_;
    int* raw2 = a.second.p_;
    reset();

    STD::pair<buf, buf> b(STD::move(a));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.first.p_ == raw1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.second.p_ == raw2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.first.empty() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.second.empty() );
    test_pass("cxx11:pair move ctor (member-wise)");
}

TEST_CASE(move03, pair_move_assign) {
    STD::pair<buf, buf> a((buf(1)), buf(2));
    int* raw1 = a.first.p_;
    STD::pair<buf, buf> b((buf(8)), buf(9));
    reset();

    b = STD::move(a);
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( b.first.p_ == raw1 );
    test_eq( b.second.value(), 2 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( a.first.empty() );
    test_pass("cxx11:pair move assign");
}

TEST_CASE(move03, pair_copy_still_copies) {
    STD::pair<buf, buf> a((buf(1)), buf(2));
    reset();
    STD::pair<buf, buf> b(a);
    test_eq( g_copies, 2 );
    test_eq( g_moves, 0 );
    test_true( b.first.p_ != a.first.p_ );
    test_eq( a.first.value(), 1 );
    test_pass("cxx11:pair copy still copies");
}

TEST_CASE(move03, pair_with_non_class_members) {

    STD::pair<int, color> a(1, blue);
    STD::pair<int, color> b(STD::move(a));
    test_eq( b.first, 1 );
    test_eq( (int)b.second, (int)blue );
    test_eq( a.first, 1 );
    test_eq( (int)a.second, (int)blue );

    STD::pair<int, buf> c(5, buf(7));
    int* raw = c.second.p_;
    reset();
    STD::pair<int, buf> d(STD::move(c));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_moves, 1 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_eq( g_copies, 0 );
    test_eq( d.first, 5 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( d.second.p_ == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( c.second.empty() );
    test_pass("cxx11:pair with non-class members move");
}

TEST_CASE(move03, map_insert_now_moves_the_mapped_value) {

    STD::map<int, STD::string> m;
    STD::pair<const int, STD::string> v(1, "a long enough value string for heap");
    const char* raw = v.second.data();

    STD::pair<STD::map<int, STD::string>::iterator, bool> r = m.insert(STD::move(v));
    test_true( r.second );
    test_eq( m.size(), 1u );
    test_true( m[1] == "a long enough value string for heap" );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( m.begin()->second.data() == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( v.second.empty() );
    test_eq( v.first, 1 );
    test_pass("cxx11:map insert moves mapped value");
}

TEST_CASE(move03, std_is_class) {
    test_true( STD::is_class<buf>::value );
    test_true( STD::is_class<STD::string>::value );
    test_true( STD::is_class<STD::vector<int> >::value );
    test_true( STD::is_class<const buf>::value );
    test_true( STD::is_class<volatile buf>::value );

    test_true( !STD::is_class<int>::value );
    test_true( !STD::is_class<double>::value );
    test_true( !STD::is_class<color>::value );
    test_true( !STD::is_class<int*>::value );
    test_true( !STD::is_class<int&>::value );
    test_true( !STD::is_class<void>::value );
    test_pass("cxx11:STD::is_class");
}

namespace {
struct MBase { virtual ~MBase() {} int b; };
struct MDerived : MBase { int d; };
int g_mbase_dtors = 0;
struct MCountBase { virtual ~MCountBase() { ++g_mbase_dtors; } };
struct MCountDerived : MCountBase { };
}

TEST_CASE(move03, shared_ptr_heterogeneous_copy) {
    STD::shared_ptr<MDerived> d(new MDerived);
    d->b = 1; d->d = 2;

    STD::shared_ptr<MBase> b(d);
    test_eq( b->b, 1 );
    test_eq( d.use_count(), 2L );
    test_eq( b.use_count(), 2L );
    test_true( b.get() == d.get() );

    STD::shared_ptr<MBase> b2;
    b2 = d;
    test_eq( d.use_count(), 3L );
    test_true( b2.get() == d.get() );
    test_pass("cxx11:shared_ptr heterogeneous copy");
}

TEST_CASE(move03, shared_ptr_heterogeneous_move) {
    STD::shared_ptr<MDerived> d(new MDerived);
    d->b = 5; d->d = 6;
    MDerived* raw = d.get();

    STD::shared_ptr<MBase> b(STD::move(d));
    test_eq( b.use_count(), 1L );
    test_true( b.get() == raw );
    test_true( !d );
    test_eq( b->b, 5 );

    STD::shared_ptr<MDerived> d2(new MDerived);
    d2->b = 7;
    STD::shared_ptr<MBase> b2;
    b2 = STD::move(d2);
    test_eq( b2.use_count(), 1L );
    test_true( !d2 );
    test_eq( b2->b, 7 );
    test_pass("cxx11:shared_ptr heterogeneous move");
}

TEST_CASE(move03, shared_ptr_destroys_as_original_type) {

    g_mbase_dtors = 0;
    {
        STD::shared_ptr<MCountBase> b;
        {
            STD::shared_ptr<MCountDerived> d(new MCountDerived);
            b = d;
        }
        test_eq( g_mbase_dtors, 0 );
    }
    test_eq( g_mbase_dtors, 1 );
    test_pass("cxx11:shared_ptr destroys as original type");
}

TEST_CASE(move03, weak_ptr_heterogeneous) {
    STD::shared_ptr<MDerived> d(new MDerived);
    d->b = 3;

    STD::weak_ptr<MBase> wb(d);
    test_true( !wb.expired() );
    STD::shared_ptr<MBase> lb = wb.lock();
    test_true( lb.get() == d.get() );
    test_eq( lb->b, 3 );

    STD::weak_ptr<MDerived> wd(d);
    STD::weak_ptr<MBase> wb2(wd);
    test_true( !wb2.expired() );

    STD::weak_ptr<MBase> wb3(STD::move(wb2));
    test_true( !wb3.expired() );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
#if defined(_MSC_VER) && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC before VS2015 has no weak_ptr move constructor");
#endif
    test_true( wb2.expired() );
    test_pass("cxx11:weak_ptr heterogeneous");
}

TEST_CASE(move03, static_and_const_pointer_cast) {
    STD::shared_ptr<MDerived> d(new MDerived);
    d->b = 1; d->d = 2;

    STD::shared_ptr<MBase> b = STD::static_pointer_cast<MBase>(d);
    test_true( b.get() == d.get() );
    test_eq( d.use_count(), 2L );
    test_pass("cxx11:static_pointer_cast");

    STD::shared_ptr<const MBase> cb = b;
    STD::shared_ptr<MBase> mb = STD::const_pointer_cast<MBase>(cb);
    test_eq( mb->b, 1 );
    test_true( mb.get() == b.get() );
    test_pass("cxx11:const_pointer_cast");
}

TEST_CASE(move03, owner_less_orders_by_control_block) {
    STD::shared_ptr<int> a(new int(1));
    STD::shared_ptr<int> b(new int(1));
    STD::owner_less<STD::shared_ptr<int> > cmp;

    test_true( cmp(a, b) != cmp(b, a) );

    STD::shared_ptr<int> a2 = a;
    test_true( !cmp(a, a2) && !cmp(a2, a) );

    STD::map<STD::shared_ptr<int>, int, STD::owner_less<STD::shared_ptr<int> > > m;
    m[a] = 10;
    m[b] = 20;
    m[a2] = 11;
    test_eq( (int)m.size(), 2 );
    test_eq( m[a], 11 );
    test_eq( m[b], 20 );
    test_pass("cxx11:owner_less orders by control block");
}

#if TEST_TARGET_CXX >= 2017
TEST_CASE(move03, variant_move) {
    STD::variant<int, buf> a;
    a = buf(7);
    test_eq( a.index(), 1u );
    int* raw = STD::get<buf>(a).p_;
    reset();

    STD::variant<int, buf> b(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_eq( b.index(), 1u );
    test_true( STD::get<buf>(b).p_ == raw );
    test_true( STD::get<buf>(a).empty() );
    test_pass("cxx11:variant move ctor");

    STD::variant<int, buf> c;
    c = buf(9);
    int* raw2 = STD::get<buf>(c).p_;
    reset();
    b = STD::move(c);
    test_eq( g_moves, 1 );
    test_true( STD::get<buf>(b).p_ == raw2 );
    test_pass("cxx11:variant move assign");
}

TEST_CASE(move03, variant_move_scalar_alternative) {

    STD::variant<int, buf> a;
    a = 42;
    test_eq( a.index(), 0u );
    STD::variant<int, buf> b(STD::move(a));
    test_eq( b.index(), 0u );
    test_eq( STD::get<int>(b), 42 );

    STD::variant<int, STD::string> s;
    s = STD::string("a long enough string for a heap buffer");
    const char* raw = STD::get<STD::string>(s).data();
    STD::variant<int, STD::string> t(STD::move(s));
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( STD::get<STD::string>(t).data() == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( STD::get<STD::string>(s).empty() );
    test_pass("cxx11:variant move scalar alternative");
}
#else
TEST_CASE(move03, variant_move) {
    TEST_NOTE("<variant> is C++17");
    test_skip("cxx11:variant move ctor");
    test_skip("cxx11:variant move assign");
}
TEST_CASE(move03, variant_move_scalar_alternative) {
    TEST_NOTE("<variant> is C++17");
    test_skip("cxx11:variant move scalar alternative");
}
#endif

TEST_CASE(move03, tuple_move) {
    STD::tuple<int, buf> a(1, buf(7));
    int* raw = STD::get<1>(a).p_;
    reset();

    STD::tuple<int, buf> b(STD::move(a));
    test_eq( g_moves, 1 );
    test_eq( g_copies, 0 );
    test_eq( STD::get<0>(b), 1 );
    test_true( STD::get<1>(b).p_ == raw );
    test_true( STD::get<1>(a).empty() );
    test_pass("cxx11:tuple move ctor");

    STD::tuple<int, buf> c(2, buf(9));
    int* raw2 = STD::get<1>(c).p_;
    reset();
    b = STD::move(c);
    test_eq( g_moves, 1 );
    test_eq( STD::get<0>(b), 2 );
    test_true( STD::get<1>(b).p_ == raw2 );
    test_pass("cxx11:tuple move assign");
}

TEST_CASE(move03, tuple_move_multi_element) {
    STD::tuple<buf, buf, buf> a((buf(1)), buf(2), buf(3));
    int* r0 = STD::get<0>(a).p_;
    int* r2 = STD::get<2>(a).p_;
    reset();

    STD::tuple<buf, buf, buf> b(STD::move(a));
    test_eq( g_moves, 3 );
    test_eq( g_copies, 0 );
    test_true( STD::get<0>(b).p_ == r0 );
    test_true( STD::get<2>(b).p_ == r2 );
    test_true( STD::get<0>(a).empty() );
    test_true( STD::get<2>(a).empty() );

    STD::tuple<int, STD::string> s(5, STD::string("long enough string for a heap buf"));
    const char* raw = STD::get<1>(s).data();
    STD::tuple<int, STD::string> t(STD::move(s));
    test_eq( STD::get<0>(t), 5 );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( STD::get<1>(t).data() == raw );
    TEST_SKIP_VC090("no move: this is MSVC 9's own type, and a move cannot be added to it");
    test_true( STD::get<1>(s).empty() );
    test_pass("cxx11:tuple move multi-element");
}

#if !defined(__GNUC__)
TEST_CASE(move03, flat_map_insert_or_assign_rvalue_mapped) {
    STD::flat_map<int, buf> m;
    buf a(7);
    int* raw = a.p_;
    reset();

    STD::pair<STD::flat_map<int, buf>::iterator, bool> r = m.insert_or_assign(1, STD::move(a));
    test_true( r.second );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_eq( g_copies, 0 );
    test_true( g_moves >= 1 );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( m[1].p_ == raw );
    test_true( a.empty() );

    buf b(9);
    int* rawb = b.p_;
    reset();
    STD::pair<STD::flat_map<int, buf>::iterator, bool> r2 = m.insert_or_assign(1, STD::move(b));
    test_true( !r2.second );
    test_true( m[1].p_ == rawb );
    test_true( b.empty() );
    test_pass("cxx11:flat_map insert_or_assign rvalue mapped");
}

TEST_CASE(move03, flat_map_operator_bracket_rvalue_key) {
    STD::flat_map<STD::string, int> m;
    STD::string k("a long enough key string for a heap buffer");
    const char* raw = k.data();

    m[STD::move(k)] = 5;
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( k.empty() );
    test_eq( m.size(), 1u );
    TEST_SKIP_VC090("flat_set/flat_map sit on std::vector, which is MSVC 9's own and cannot be moved");
    test_true( m.begin()->first.data() == raw );
    test_eq( m["a long enough key string for a heap buffer"], 5 );

    STD::string k2("a long enough key string for a heap buffer");
    m[STD::move(k2)] = 9;
    test_eq( m.size(), 1u );
    test_eq( m.begin()->second, 9 );
    test_pass("cxx11:flat_map operator[] rvalue key");
}
#else
TEST_CASE_SKIP(move03, flat_map_insert_or_assign_rvalue_mapped)
TEST_CASE_SKIP(move03, flat_map_operator_bracket_rvalue_key)
#endif

#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, set_extract_and_insert_node) {
    STD::set<int> s;
    for (int i = 0; i < 5; ++i) s.insert(i * 10);

    STD::set<int>::node_type nh = s.extract(20);
    test_true( !nh.empty() );
    test_eq( nh.value(), 20 );
    test_eq( s.size(), 4u );
    test_true( s.find(20) == s.end() );
    test_pass("cxx11:set extract node");

    STD::set<int> t;
    STD::set<int>::insert_return_type r = t.insert(STD::move(nh));
    test_true( r.inserted );
    test_true( nh.empty() );
    test_eq( *r.position, 20 );
    test_eq( t.size(), 1u );
    test_true( t.find(20) != t.end() );

    STD::set<int>::node_type nh2 = s.extract(s.begin());
    test_eq( nh2.value(), 0 );
    test_eq( s.size(), 3u );
    test_pass("cxx11:set insert node");
}
#else
TEST_CASE_SKIP(move03, set_extract_and_insert_node)
#endif

#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, set_insert_node_duplicate_is_returned) {
    STD::set<int> a, b;
    a.insert(1); a.insert(2);
    b.insert(2);

    STD::set<int>::node_type nh = a.extract(2);
    test_eq( nh.value(), 2 );

    STD::set<int>::insert_return_type r = b.insert(STD::move(nh));
    test_true( !r.inserted );
    test_true( !r.node.empty() );
    test_eq( r.node.value(), 2 );
    test_eq( *r.position, 2 );
    test_eq( b.size(), 1u );
    test_pass("cxx11:set insert node duplicate returned");
}
#else
TEST_CASE_SKIP(move03, set_insert_node_duplicate_is_returned)
#endif

#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, set_extract_missing_key) {
    STD::set<int> s;
    s.insert(1);
    STD::set<int>::node_type nh = s.extract(99);
    test_true( nh.empty() );
    test_eq( s.size(), 1u );
    test_pass("cxx11:set extract missing key");
}
#else
TEST_CASE_SKIP(move03, set_extract_missing_key)
#endif

#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, multiset_extract_and_insert_node) {
    STD::multiset<int> s;
    s.insert(5); s.insert(5); s.insert(7);
    test_eq( s.count(5), 2u );

    STD::multiset<int>::node_type nh = s.extract(5);
    test_eq( nh.value(), 5 );
    test_eq( s.count(5), 1u );
    test_eq( s.size(), 2u );
    test_pass("cxx11:multiset extract node");

    STD::multiset<int>::iterator it = s.insert(STD::move(nh));
    test_eq( *it, 5 );
    test_true( nh.empty() );
    test_eq( s.count(5), 2u );
    test_pass("cxx11:multiset insert node");
}
#else
TEST_CASE_SKIP(move03, multiset_extract_and_insert_node)
#endif

#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, map_extract_and_insert_node) {
    STD::map<int, STD::string> m;
    m[1] = "one"; m[2] = "two"; m[3] = "three";
    const STD::string* raw = &m.find(2)->second;

    STD::map<int, STD::string>::node_type nh = m.extract(2);
    test_true( !nh.empty() );
    test_eq( nh.key(), 2 );
    test_true( nh.mapped() == "two" );
    test_eq( m.size(), 2u );
    test_true( m.find(2) == m.end() );
    test_pass("cxx11:map extract node");

    nh.key() = 9;
    STD::map<int, STD::string> n;
    STD::map<int, STD::string>::insert_return_type r = n.insert(STD::move(nh));
    test_true( r.inserted );
    test_eq( r.position->first, 9 );
    test_true( n[9] == "two" );
    test_true( &n.find(9)->second == raw );
    test_true( nh.empty() );
    test_pass("cxx11:map insert node");
}
#else
TEST_CASE_SKIP(move03, map_extract_and_insert_node)
#endif

#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, map_extract_key_change_reinsert_same_map) {
    STD::map<int, int> m;
    m[1] = 10; m[2] = 20;
    STD::map<int, int>::node_type nh = m.extract(1);
    nh.key() = 5;
    m.insert(STD::move(nh));
    test_true( m.find(1) == m.end() );
    test_eq( m[5], 10 );
    test_eq( m[2], 20 );
    test_eq( m.size(), 2u );
    test_pass("cxx11:map extract, rekey, reinsert");
}
#else
TEST_CASE_SKIP(move03, map_extract_key_change_reinsert_same_map)
#endif

#if defined(__WATCOMC__)
#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, libcxx03_tree_valid_after_node_handles) {
    STD::set<int> s;
    STD::multiset<int> ms;
    STD::map<int, int> m;
    int i;

    for (i = 0; i < 32; ++i) s.insert(i * 7 % 32);
    {
        STD::set<int>::node_type nh = s.extract(20);
        STD::set<int> t;
        test_true( s.__tree_rb_valid() );
        t.insert(STD::move(nh));
        test_true( t.__tree_rb_valid() );
    }

    ms.insert(5); ms.insert(5); ms.insert(7);
    {
        STD::multiset<int>::node_type nh = ms.extract(5);
        ms.insert(STD::move(nh));
        test_true( ms.__tree_rb_valid() );
    }

    m[1] = 10; m[2] = 20;
    {
        STD::map<int, int>::node_type nh = m.extract(1);
        nh.key() = 5;
        m.insert(STD::move(nh));
        test_true( m.__tree_rb_valid() );
    }
    test_pass("cxx11:RB-tree valid after node handles");
}
#else
TEST_CASE_SKIP(move03, libcxx03_tree_valid_after_node_handles)
#endif
#else
TEST_CASE_SKIP(move03, libcxx03_tree_valid_after_node_handles)
#endif

#if _TST_MOVE_NATIVE_CONTAINERS && _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, node_handle_frees_when_dropped) {

    STD::set<int> s;
    s.insert(1); s.insert(2);
    {
        STD::set<int>::node_type nh = s.extract(1);
        test_eq( nh.value(), 1 );
    }
    test_eq( s.size(), 1u );
    test_true( s.find(1) == s.end() );
    test_pass("cxx11:node handle frees when dropped");
}
#else
TEST_CASE_SKIP(move03, node_handle_frees_when_dropped)
#endif

#if _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, unordered_set_extract_insert_merge) {
    STD::unordered_set<int> s;
    for (int i = 0; i < 10; ++i) s.insert(i);

    STD::unordered_set<int>::node_type nh = s.extract(5);
    test_true( !nh.empty() );
    test_eq( nh.value(), 5 );
    test_eq( s.size(), 9u );
    test_true( s.find(5) == s.end() );

    STD::unordered_set<int>::node_type miss = s.extract(999);
    test_true( miss.empty() );
    test_pass("cxx11:unordered_set extract node");

    STD::unordered_set<int> t;
    STD::unordered_set<int>::insert_return_type r = t.insert(STD::move(nh));
    test_true( r.inserted );
    test_true( nh.empty() );
    test_eq( *r.position, 5 );
    test_true( t.find(5) != t.end() );
    test_pass("cxx11:unordered_set insert node");

    STD::unordered_set<int> a, b;
    a.insert(1); a.insert(2); a.insert(3);
    b.insert(2); b.insert(4);
    b.merge(a);

    test_true( b.find(1) != b.end() );
    test_true( b.find(3) != b.end() );
    test_true( b.find(4) != b.end() );
    test_eq( b.size(), 4u );
    test_true( a.find(2) != a.end() );
    test_eq( a.size(), 1u );
    test_pass("cxx11:unordered_set merge");
}
#else
TEST_CASE_SKIP(move03, unordered_set_extract_insert_merge)
#endif

#if _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, unordered_multiset_extract_insert_merge) {
    STD::unordered_multiset<int> s;
    s.insert(5); s.insert(5); s.insert(7);
    STD::unordered_multiset<int>::node_type nh = s.extract(5);
    test_eq( nh.value(), 5 );
    test_eq( s.count(5), 1u );
    test_pass("cxx11:unordered_multiset extract node");

    STD::unordered_multiset<int>::iterator it = s.insert(STD::move(nh));
    test_eq( *it, 5 );
    test_eq( s.count(5), 2u );
    test_true( nh.empty() );
    test_pass("cxx11:unordered_multiset insert node");

    STD::unordered_multiset<int> a, b;
    a.insert(1); a.insert(1);
    b.insert(1);
    b.merge(a);
    test_eq( b.count(1), 3u );
    test_eq( a.size(), 0u );
    test_pass("cxx11:unordered_multiset merge");
}
#else
TEST_CASE_SKIP(move03, unordered_multiset_extract_insert_merge)
#endif

#if _TST_HAS_CXX17_LIB_MEMBERS
TEST_CASE(move03, unordered_map_extract_insert_merge) {
    STD::unordered_map<int, STD::string> m;
    m[1] = "one"; m[2] = "two";
    const STD::string* raw = &m.find(1)->second;

    STD::unordered_map<int, STD::string>::node_type nh = m.extract(1);
    test_eq( nh.key(), 1 );
    test_true( nh.mapped() == "one" );
    test_pass("cxx11:unordered_map extract node");
    nh.key() = 9;

    STD::unordered_map<int, STD::string> n;
    STD::unordered_map<int, STD::string>::insert_return_type r = n.insert(STD::move(nh));
    test_true( r.inserted );
    test_true( n[9] == "one" );
    test_true( &n.find(9)->second == raw );
    test_pass("cxx11:unordered_map insert node");

    STD::unordered_map<int, int> a, b;
    a[1] = 10; a[2] = 20;
    b[2] = 99;
    b.merge(a);
    test_eq( b[1], 10 );
    test_eq( b[2], 99 );
    test_true( a.find(2) != a.end() );
    test_true( a.find(1) == a.end() );
    test_pass("cxx11:unordered_map merge");
}
#else
TEST_CASE_SKIP(move03, unordered_map_extract_insert_merge)
#endif

#if defined(__WATCOMC__)
TEST_CASE(move03, path_move) {
    namespace fs = STD::filesystem;
    typedef fs::path::string_type fs_str;
    typedef fs::path::value_type  fs_chr;

    const fs_str LONG_NAME = fs::path("/a/reasonably/long/path/name/that/owns/a/heap/buffer.txt").native();
    fs::path p(LONG_NAME);
    const fs_chr* raw = p.c_str();

    fs::path q(STD::move(p));
    test_true( q.c_str() == raw );
    test_true( p.empty() );
    test_true( q.native() == LONG_NAME );

    fs::path r;
    r = STD::move(q);
    test_true( r.c_str() == raw );
    test_true( q.empty() );
    test_pass("cxx11:path move");

    fs_str s = fs::path("/moved/from/a/string/long/enough/for/a/heap/buffer").native();
    const fs_chr* sraw = s.data();
    fs::path t(STD::move(s));
    test_true( t.native().data() == sraw );
    test_true( s.empty() );

    fs_str s2 = fs::path("/another/heap/string/assigned/into/a/path/object/here").native();
    const fs_chr* s2raw = s2.data();
    fs::path u;
    u = STD::move(s2);
    test_true( u.native().data() == s2raw );
    test_true( s2.empty() );

    fs_str s3 = fs::path("/with/an/explicit/format/tag/long/enough/for/heap").native();
    const fs_chr* s3raw = s3.data();
    fs::path v(STD::move(s3), fs::path::format::native_format);
    test_true( v.native().data() == s3raw );
    test_true( s3.empty() );
    test_pass("cxx11:path from string&& move");
}

TEST_CASE(move03, directory_entry_move) {
    namespace fs = STD::filesystem;
    fs::directory_entry e(fs::path("/some/long/enough/directory/entry/path/here"));
    const fs::path::value_type* raw = e.path().c_str();

    fs::directory_entry f(STD::move(e));
    test_true( f.path().c_str() == raw );
    test_true( e.path().empty() );

    fs::directory_entry g;
    g = STD::move(f);
    test_true( g.path().c_str() == raw );
    test_true( f.path().empty() );
    test_pass("cxx11:directory_entry move");
}

TEST_CASE(move03, file_status_move) {
    namespace fs = STD::filesystem;
    fs::file_status a(fs::regular_file);
    fs::file_status b(STD::move(a));
    test_true( b.type() == fs::regular_file );

    fs::file_status c;
    c = STD::move(b);
    test_true( c.type() == fs::regular_file );
    test_pass("cxx11:file_status move");
}
#else
TEST_CASE_SKIP(move03, path_move)
TEST_CASE_SKIP(move03, directory_entry_move)
TEST_CASE_SKIP(move03, file_status_move)
#endif
#else
TEST_CASE_SKIP(move03, explicit_move_of_class)
TEST_CASE_SKIP(move03, explicit_move_assign)
TEST_CASE_SKIP(move03, copy_still_copies)
TEST_CASE_SKIP(move03, move_of_const_is_a_copy)
TEST_CASE_SKIP(move03, move_without_move_ctor_falls_back_to_copy)
TEST_CASE_SKIP(move03, move_of_scalar_compiles)
TEST_CASE_SKIP(move03, move_in_generic_code)
TEST_CASE_SKIP(move03, vector_move_ctor_steals_buffer)
TEST_CASE_SKIP(move03, vector_move_assign_steals_buffer)
TEST_CASE_SKIP(move03, vector_copy_still_deep_copies)
TEST_CASE_SKIP(move03, moved_from_vector_is_reusable)
TEST_CASE_SKIP(move03, vector_push_back_moves_the_element)
TEST_CASE_SKIP(move03, vector_push_back_copies_without_move)
TEST_CASE_SKIP(move03, vector_insert_moves_the_element)
TEST_CASE_SKIP(move03, vector_of_scalars_unaffected)
TEST_CASE_SKIP(move03, vector_of_vectors)
TEST_CASE_SKIP(move03, vector_growth_preserves_values)
TEST_CASE_SKIP(move03, vector_of_enum_still_compiles)
TEST_CASE_SKIP(move03, vector_of_string)
TEST_CASE_SKIP(move03, list_move_ctor_relinks)
TEST_CASE_SKIP(move03, list_move_assign_relinks)
TEST_CASE_SKIP(move03, list_copy_still_deep_copies)
TEST_CASE_SKIP(move03, list_push_moves_the_element)
TEST_CASE_SKIP(move03, list_push_copies_without_move)
TEST_CASE_SKIP(move03, list_splice_and_merge_rvalue)
TEST_CASE_SKIP(move03, list_splice_single_and_range_rvalue)
TEST_CASE_SKIP(move03, list_of_enum_still_compiles)
TEST_CASE_SKIP(move03, list_of_scalars_unaffected)
TEST_CASE_SKIP(move03, moved_from_list_is_reusable)
TEST_CASE_SKIP(move03, forward_list_move_ctor_relinks)
TEST_CASE_SKIP(move03, forward_list_move_assign_relinks)
TEST_CASE_SKIP(move03, forward_list_push_front_moves)
TEST_CASE_SKIP(move03, forward_list_insert_after_moves)
TEST_CASE_SKIP(move03, forward_list_splice_after_and_merge_rvalue)
TEST_CASE_SKIP(move03, forward_list_of_enum_and_scalars)
TEST_CASE_SKIP(move03, deque_move_ctor_steals_buffer)
TEST_CASE_SKIP(move03, deque_move_assign_steals_buffer)
TEST_CASE_SKIP(move03, deque_copy_still_deep_copies)
TEST_CASE_SKIP(move03, deque_push_moves_the_element)
TEST_CASE_SKIP(move03, deque_push_copies_without_move)
TEST_CASE_SKIP(move03, deque_insert_moves_the_element)
TEST_CASE_SKIP(move03, deque_of_enum_and_scalars)
TEST_CASE_SKIP(move03, moved_from_deque_is_reusable)
TEST_CASE_SKIP(move03, string_move_ctor_steals_buffer)
TEST_CASE_SKIP(move03, string_move_assign_steals_buffer)
TEST_CASE_SKIP(move03, string_copy_still_copies)
TEST_CASE_SKIP(move03, moved_from_string_is_reusable)
TEST_CASE_SKIP(move03, wstring_move)
TEST_CASE_SKIP(move03, set_move_ctor_takes_nodes)
TEST_CASE_SKIP(move03, set_move_assign_takes_nodes)
TEST_CASE_SKIP(move03, moved_from_set_is_reusable)
TEST_CASE_SKIP(move03, set_insert_moves_the_value)
TEST_CASE_SKIP(move03, multiset_insert_moves_the_value)
TEST_CASE_SKIP(move03, map_move_ctor_takes_nodes)
TEST_CASE_SKIP(move03, map_insert_rvalue)
TEST_CASE_SKIP(move03, multimap_move_and_insert)
TEST_CASE_SKIP(move03, set_with_comparator_survives_move)
TEST_CASE_SKIP(move03, libcxx03_tree_valid_after_move)
TEST_CASE_SKIP(move03, unordered_set_move_takes_nodes)
TEST_CASE_SKIP(move03, unordered_set_insert_moves_the_value)
TEST_CASE_SKIP(move03, unordered_multiset_insert_moves_the_value)
TEST_CASE_SKIP(move03, unordered_map_move_and_insert)
TEST_CASE_SKIP(move03, unordered_multimap_move_and_insert)
TEST_CASE_SKIP(move03, moved_from_unordered_map_rehashes_fine)
TEST_CASE_SKIP(move03, stack_move_of_container)
TEST_CASE_SKIP(move03, stack_push_moves_the_element)
TEST_CASE_SKIP(move03, queue_move_of_container)
TEST_CASE_SKIP(move03, queue_push_moves_the_element)
TEST_CASE_SKIP(move03, priority_queue_move_and_push)
TEST_CASE_SKIP(move03, stack_queue_of_scalars_unaffected)
TEST_CASE_SKIP(move03, unique_ptr_move_transfers_ownership)
TEST_CASE_SKIP(move03, unique_ptr_return_by_value)
TEST_CASE_SKIP(move03, unique_ptr_reset_release_swap)
TEST_CASE_SKIP(move03, shared_ptr_move_does_not_touch_refcount)
TEST_CASE_SKIP(move03, shared_ptr_copy_still_shares)
TEST_CASE_SKIP(move03, shared_ptr_move_releases_old_target)
TEST_CASE_SKIP(move03, weak_ptr_move)
TEST_CASE_SKIP(move03, optional_move_ctor_moves_the_value)
TEST_CASE_SKIP(move03, optional_move_assign)
TEST_CASE_SKIP(move03, optional_construct_from_moved_value)
TEST_CASE_SKIP(move03, optional_copy_still_copies)
TEST_CASE_SKIP(move03, optional_of_scalar_and_string)
TEST_CASE_SKIP(move03, unexpected_move)
TEST_CASE_SKIP(move03, expected_move_ctor_keeps_the_alternative)
TEST_CASE_SKIP(move03, expected_move_of_error_state)
TEST_CASE_SKIP(move03, expected_move_assign)
TEST_CASE_SKIP(move03, expected_construct_from_moved_value)
TEST_CASE_SKIP(move03, expected_copy_still_copies)
TEST_CASE_SKIP(move03, expected_of_scalars_unaffected)
TEST_CASE_SKIP(move03, flat_set_move)
TEST_CASE_SKIP(move03, flat_set_insert_moves_the_value)
TEST_CASE_SKIP(move03, flat_map_move)
TEST_CASE_SKIP(move03, flat_map_insert_rvalue)
TEST_CASE_SKIP(move03, thread_move_transfers_the_handle)
TEST_CASE_SKIP(move03, thread_move_assign)
TEST_CASE_SKIP(move03, thread_swap)
TEST_CASE_SKIP(move03, promise_future_move)
TEST_CASE_SKIP(move03, promise_move)
TEST_CASE_SKIP(move03, shared_future_move)
TEST_CASE_SKIP(move03, shared_future_copy_still_shares)
TEST_CASE_SKIP(move03, transparent_comparators)
TEST_CASE_SKIP(move03, function_move_takes_the_target)
TEST_CASE_SKIP(move03, function_move_assign)
TEST_CASE_SKIP(move03, function_move_with_args)
TEST_CASE_SKIP(move03, move_iterator_moves_elements)
TEST_CASE_SKIP(move03, move_iterator_range_ctor)
TEST_CASE_SKIP(move03, move_iterator_basics)
TEST_CASE_SKIP(move03, move_iterator_into_list)
TEST_CASE_SKIP(move03, insert_iterators_forward_the_move)
TEST_CASE_SKIP(move03, move_of_enum_is_a_copy)
TEST_CASE_SKIP(move03, is_class_classification)
TEST_CASE_SKIP(move03, pair_move_is_member_wise)
TEST_CASE_SKIP(move03, pair_move_assign)
TEST_CASE_SKIP(move03, pair_copy_still_copies)
TEST_CASE_SKIP(move03, pair_with_non_class_members)
TEST_CASE_SKIP(move03, map_insert_now_moves_the_mapped_value)
TEST_CASE_SKIP(move03, std_is_class)
TEST_CASE_SKIP(move03, shared_ptr_heterogeneous_copy)
TEST_CASE_SKIP(move03, shared_ptr_heterogeneous_move)
TEST_CASE_SKIP(move03, shared_ptr_destroys_as_original_type)
TEST_CASE_SKIP(move03, weak_ptr_heterogeneous)
TEST_CASE_SKIP(move03, static_and_const_pointer_cast)
TEST_CASE_SKIP(move03, owner_less_orders_by_control_block)
TEST_CASE_SKIP(move03, variant_move)
TEST_CASE_SKIP(move03, variant_move_scalar_alternative)
TEST_CASE_SKIP(move03, tuple_move)
TEST_CASE_SKIP(move03, tuple_move_multi_element)
TEST_CASE_SKIP(move03, flat_map_insert_or_assign_rvalue_mapped)
TEST_CASE_SKIP(move03, flat_map_operator_bracket_rvalue_key)
TEST_CASE_SKIP(move03, set_extract_and_insert_node)
TEST_CASE_SKIP(move03, set_insert_node_duplicate_is_returned)
TEST_CASE_SKIP(move03, set_extract_missing_key)
TEST_CASE_SKIP(move03, multiset_extract_and_insert_node)
TEST_CASE_SKIP(move03, map_extract_and_insert_node)
TEST_CASE_SKIP(move03, map_extract_key_change_reinsert_same_map)
TEST_CASE_SKIP(move03, libcxx03_tree_valid_after_node_handles)
TEST_CASE_SKIP(move03, node_handle_frees_when_dropped)
TEST_CASE_SKIP(move03, unordered_set_extract_insert_merge)
TEST_CASE_SKIP(move03, unordered_multiset_extract_insert_merge)
TEST_CASE_SKIP(move03, unordered_map_extract_insert_merge)
TEST_CASE_SKIP(move03, path_move)
TEST_CASE_SKIP(move03, directory_entry_move)
TEST_CASE_SKIP(move03, file_status_move)
#endif
