#include "test_cxx.hpp"

#if _TST_TEST_HAS(<version>)
#  include <version>
#endif

#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <sstream>
#include <cstddef>
#include <type_traits>

namespace {
    struct ItPair {
        int a;
        int b;
        ItPair(int x = 0, int y = 0) : a(x), b(y) {}
    };
}

TEST_CASE(iterator, insert_iterators_members_cxx03) {
    typedef STD::vector<int> Vec;
    typedef STD::deque<int> Deq;
    typedef STD::back_insert_iterator<Vec> BackIt;
    typedef STD::front_insert_iterator<Deq> FrontIt;
    typedef STD::insert_iterator<Vec> InsIt;

    test_true(( STD::is_same<BackIt::container_type, Vec>::value ));
    test_true(( STD::is_same<FrontIt::container_type, Deq>::value ));
    test_pass("cxx11:back_insert_iterator::container_type");
    test_true(( STD::is_same<InsIt::container_type, Vec>::value ));
    test_pass("cxx11:front_insert_iterator::container_type");
    test_pass("cxx11:insert_iterator::container_type");

    test_true(( STD::is_same<BackIt::iterator_category, STD::output_iterator_tag>::value ));
    test_pass("cxx03:back_insert_iterator::iterator_category");
    test_true(( STD::is_same<FrontIt::iterator_category, STD::output_iterator_tag>::value ));
    test_pass("cxx03:front_insert_iterator::iterator_category");
    test_true(( STD::is_same<InsIt::iterator_category, STD::output_iterator_tag>::value ));
    test_pass("cxx03:insert_iterator::iterator_category");

    test_true( STD::is_void<BackIt::value_type>::value );
    test_pass("cxx03:back_insert_iterator::value_type");
    test_true( STD::is_void<FrontIt::value_type>::value );
    test_pass("cxx03:front_insert_iterator::value_type");
    test_true( STD::is_void<InsIt::value_type>::value );
    test_pass("cxx03:insert_iterator::value_type");

    test_true( STD::is_void<BackIt::pointer>::value );
    test_pass("cxx03:back_insert_iterator::pointer");
    test_true( STD::is_void<FrontIt::pointer>::value );
    test_pass("cxx03:front_insert_iterator::pointer");
    test_true( STD::is_void<InsIt::pointer>::value );
    test_pass("cxx03:insert_iterator::pointer");

    test_true( STD::is_void<BackIt::reference>::value );
    test_pass("cxx03:back_insert_iterator::reference");
    test_true( STD::is_void<FrontIt::reference>::value );
    test_pass("cxx03:front_insert_iterator::reference");
    test_true( STD::is_void<InsIt::reference>::value );
    test_pass("cxx03:insert_iterator::reference");

#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true(( STD::is_same<BackIt::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx03:back_insert_iterator::difference_type");
    test_pass("cxx20:back_insert_iterator::difference_type is ptrdiff_t");
    test_true(( STD::is_same<FrontIt::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx03:front_insert_iterator::difference_type");
    test_pass("cxx20:front_insert_iterator::difference_type is ptrdiff_t");
    test_true(( STD::is_same<InsIt::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx03:insert_iterator::difference_type");
    test_pass("cxx20:insert_iterator::difference_type is ptrdiff_t");
#else
    test_true( STD::is_void<BackIt::difference_type>::value );
    test_pass("cxx03:back_insert_iterator::difference_type");
    test_skip("cxx20:back_insert_iterator::difference_type is ptrdiff_t");
    test_true( STD::is_void<FrontIt::difference_type>::value );
    test_pass("cxx03:front_insert_iterator::difference_type");
    test_skip("cxx20:front_insert_iterator::difference_type is ptrdiff_t");
    test_true( STD::is_void<InsIt::difference_type>::value );
    test_pass("cxx03:insert_iterator::difference_type");
    test_skip("cxx20:insert_iterator::difference_type is ptrdiff_t");
#endif
}

TEST_CASE(iterator, insert_iterators_operators_cxx03) {
    STD::vector<int> v;
    STD::back_insert_iterator<STD::vector<int> > bi(v);

    *bi = 1;
    test_eq( (int)v.size(), 1 );
    test_eq( v[0], 1 );
    test_pass("cxx11:back_insert_iterator");
    test_pass("cxx03:back_insert_iterator::operator*");

    bi = 2;
    test_eq( (int)v.size(), 2 );
    test_eq( v[1], 2 );
    test_pass("cxx03:back_insert_iterator::operator=");

    ++bi;
    bi++;
    *bi = 3;
    test_eq( (int)v.size(), 3 );
    test_eq( v[2], 3 );
    test_pass("cxx03:back_insert_iterator::operator++");

    STD::deque<int> d;
    STD::front_insert_iterator<STD::deque<int> > fi(d);
    *fi = 1;
    *fi = 2;
    test_eq( (int)d.size(), 2 );
    test_eq( d[0], 2 );
    test_pass("cxx11:front_insert_iterator");
    test_pass("cxx03:front_insert_iterator::operator*");
    fi = 3;
    test_eq( d[0], 3 );
    test_pass("cxx03:front_insert_iterator::operator=");
    ++fi; fi++;
    *fi = 4;
    test_eq( d[0], 4 );
    test_eq( (int)d.size(), 4 );
    test_pass("cxx03:front_insert_iterator::operator++");

    STD::vector<int> w;
    w.push_back(10);
    w.push_back(30);
    STD::insert_iterator<STD::vector<int> > ii(w, w.begin() + 1);
    *ii = 20;
    test_eq( (int)w.size(), 3 );
    test_eq( w[1], 20 );
    test_pass("cxx03:insert_iterator");
    test_pass("cxx03:insert_iterator::operator*");
    ii = 25;
    test_eq( w[2], 25 );
    test_eq( w[3], 30 );
    test_pass("cxx03:insert_iterator::operator=");
    ++ii; ii++;
    *ii = 27;
    test_eq( (int)w.size(), 5 );
    test_eq( w[3], 27 );
    test_pass("cxx03:insert_iterator::operator++");
}

TEST_CASE(iterator, reverse_iterator_members_cxx03) {
    typedef STD::vector<ItPair>::iterator It;
    typedef STD::reverse_iterator<It> RIt;

    test_true(( STD::is_same<RIt::iterator_type, It>::value ));
    test_pass("cxx11:reverse_iterator::iterator_type");
    test_true(( STD::is_same<RIt::value_type, ItPair>::value ));
    test_pass("cxx03:reverse_iterator::value_type");
    test_true(( STD::is_same<RIt::difference_type,
                             STD::iterator_traits<It>::difference_type>::value ));
    test_pass("cxx03:reverse_iterator::difference_type");
    test_true(( STD::is_same<RIt::pointer, ItPair*>::value ));
    test_pass("cxx03:reverse_iterator::pointer");
    test_true(( STD::is_same<RIt::reference, ItPair&>::value ));
    test_pass("cxx03:reverse_iterator::reference");
    test_true(( STD::is_same<RIt::iterator_category,
                             STD::random_access_iterator_tag>::value ));
    test_pass("cxx03:reverse_iterator::iterator_category");

#if defined(__cpp_lib_concepts)
    test_true(( STD::is_same<RIt::iterator_concept,
                             STD::random_access_iterator_tag>::value ));
    test_pass("cxx20:reverse_iterator::iterator_concept");
#else
    test_skip("cxx20:reverse_iterator::iterator_concept");
#endif
}

TEST_CASE(iterator, reverse_iterator_operators_cxx03) {
    STD::vector<ItPair> v;
    for (int i = 0; i < 5; ++i) v.push_back(ItPair(i, i * 10));
    typedef STD::vector<ItPair>::iterator It;
    typedef STD::reverse_iterator<It> RIt;

    RIt r(v.end());
    test_true( r.base() == v.end() );
    test_eq( (*r).a, 4 );
    test_pass("cxx03:reverse_iterator::base");
    test_pass("cxx11:base");

    test_eq( (*r).a, 4 );
    (*r).a = 40;
    test_eq( v[4].a, 40 );
    test_pass("cxx03:reverse_iterator::operator*");

    test_eq( r->b, 40 );
    r->b = 41;
    test_eq( v[4].b, 41 );
    test_pass("cxx03:reverse_iterator::operator->");

    RIt s = r;
    test_true( s == r );
    RIt t(v.begin() + 3);
    s = t;
    test_true( s == t );
    test_true( s != r );
    test_pass("cxx03:reverse_iterator::operator=");

    RIt p(v.end());
    ++p;
    test_eq( (*p).a, 3 );
    RIt q = p++;
    test_eq( (*q).a, 3 );
    test_eq( (*p).a, 2 );
    test_pass("cxx03:reverse_iterator::operator++");

    --p;
    test_eq( (*p).a, 3 );
    RIt u = p--;
    test_eq( (*u).a, 3 );
    test_eq( (*p).a, 40 );
    test_pass("cxx03:reverse_iterator::operator--");

    RIt a(v.end());
    RIt b = a + 2;
    test_eq( (*b).a, 2 );
    test_eq( (*a).a, 40 );
    test_pass("cxx03:reverse_iterator::operator+");

    RIt c = b - 1;
    test_eq( (*c).a, 3 );
    test_eq( (int)(b - a), 2 );
    test_pass("cxx03:reverse_iterator::operator-");

    RIt d(v.end());
    d += 3;
    test_eq( (*d).a, 1 );
    test_pass("cxx03:reverse_iterator::operator+=");
    d -= 2;
    test_eq( (*d).a, 3 );
    test_pass("cxx03:reverse_iterator::operator-=");
}

#if defined(__WATCOMC__) && !defined(_CCW_HAS_STREAM_ITER_MEMBER_TYPES)
#  define _TST_STREAM_ITER_MEMBER_TYPES 0
#else
#  define _TST_STREAM_ITER_MEMBER_TYPES 1
#endif

TEST_CASE(iterator, stream_iterators_members_cxx03) {
    typedef STD::istream_iterator<int> IsIt;
    typedef STD::ostream_iterator<int> OsIt;

#if _TST_STREAM_ITER_MEMBER_TYPES
    test_true(( STD::is_same<IsIt::char_type, char>::value ));
    test_pass("cxx03:istream_iterator::char_type");
#else
    test_skip("cxx03:istream_iterator::char_type");
#endif
    test_true(( STD::is_same<IsIt::value_type, int>::value ));
    test_pass("cxx03:istream_iterator::value_type");
    test_true(( STD::is_same<IsIt::difference_type, STD::ptrdiff_t>::value ));
    test_pass("cxx03:istream_iterator::difference_type");
    test_true(( STD::is_same<IsIt::pointer, const int*>::value ));
    test_pass("cxx03:istream_iterator::pointer");
    test_true(( STD::is_same<IsIt::reference, const int&>::value ));
    test_pass("cxx03:istream_iterator::reference");
    test_true(( STD::is_same<IsIt::iterator_category, STD::input_iterator_tag>::value ));
    test_pass("cxx03:istream_iterator::iterator_category");
#if _TST_STREAM_ITER_MEMBER_TYPES
    test_true(( STD::is_same<IsIt::istream_type, STD::istream>::value ));
    test_pass("cxx11:istream_iterator::istream_type");
    test_true(( STD::is_same<IsIt::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:istream_iterator::traits_type");
#else
    test_skip("cxx11:istream_iterator::istream_type");
    test_skip("cxx11:istream_iterator::traits_type");
#endif

#if _TST_STREAM_ITER_MEMBER_TYPES
    test_true(( STD::is_same<OsIt::char_type, char>::value ));
    test_pass("cxx03:ostream_iterator::char_type");
#else
    test_skip("cxx03:ostream_iterator::char_type");
#endif
    test_true( STD::is_void<OsIt::value_type>::value );
    test_pass("cxx03:ostream_iterator::value_type");
    test_true( STD::is_void<OsIt::pointer>::value );
    test_pass("cxx03:ostream_iterator::pointer");
    test_true( STD::is_void<OsIt::reference>::value );
    test_pass("cxx03:ostream_iterator::reference");
    test_true(( STD::is_same<OsIt::iterator_category, STD::output_iterator_tag>::value ));
    test_pass("cxx03:ostream_iterator::iterator_category");
#if _TST_STREAM_ITER_MEMBER_TYPES
    test_true(( STD::is_same<OsIt::ostream_type, STD::ostream>::value ));
    test_pass("cxx11:ostream_iterator::ostream_type");
    test_true(( STD::is_same<OsIt::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:ostream_iterator::traits_type");
#else
    test_skip("cxx11:ostream_iterator::ostream_type");
    test_skip("cxx11:ostream_iterator::traits_type");
#endif
#if _TST_HAS_CXX20_LIB_MEMBERS
    test_true(( STD::is_same<OsIt::difference_type, STD::ptrdiff_t>::value ));
#else
    test_true( STD::is_void<OsIt::difference_type>::value );
#endif
    test_pass("cxx03:ostream_iterator::difference_type");
}

TEST_CASE(iterator, stream_iterators_operators_cxx03) {
    {
        STD::istringstream is("1 2 3");
        STD::istream_iterator<int> it(is), end;
        test_eq( *it, 1 );
        test_pass("cxx03:istream_iterator::operator*");
        ++it;
        test_eq( *it, 2 );
        STD::istream_iterator<int> prev = it++;
        test_eq( *prev, 2 );
        test_eq( *it, 3 );
        test_pass("cxx03:istream_iterator::operator++");
        ++it;
        test_true( it == end );
        test_pass("cxx20:istream_iterator::operator==");
    }
    {
        STD::istringstream is2("5");
        STD::istream_iterator<int> a(is2);
        STD::istream_iterator<int> b = a;
        test_eq( *b, 5 );
        b = a;
        test_eq( *b, 5 );
        test_pass("cxx20:istream_iterator::operator=");
    }
    {
        STD::istringstream is("11 22");
        STD::istream_iterator<STD::string> it(is);
        test_eq( it->size(), (STD::size_t)2 );
        test_pass("cxx03:istream_iterator::operator->");
    }
    {
        STD::ostringstream os;
        STD::ostream_iterator<int> it(os, ",");
        *it = 1;
        test_eq( os.str(), STD::string("1,") );
        test_pass("cxx03:ostream_iterator::operator*");
        it = 2;
        test_eq( os.str(), STD::string("1,2,") );
        test_pass("cxx03:ostream_iterator::operator=");
        ++it; it++;
        *it = 3;
        test_eq( os.str(), STD::string("1,2,3,") );
        test_pass("cxx03:ostream_iterator::operator++");
    }
}

TEST_CASE(iterator, streambuf_iterators_cxx03) {
    typedef STD::istreambuf_iterator<char> InIt;
    typedef STD::ostreambuf_iterator<char> OutIt;

    test_true(( STD::is_same<InIt::char_type, char>::value ));
    test_pass("cxx03:istreambuf_iterator::char_type");
    test_true(( STD::is_same<InIt::int_type, STD::char_traits<char>::int_type>::value ));
    test_pass("cxx03:istreambuf_iterator::int_type");
    test_true(( STD::is_same<InIt::streambuf_type, STD::streambuf>::value ));
    test_pass("cxx03:istreambuf_iterator::streambuf_type");
    test_true(( STD::is_same<InIt::istream_type, STD::istream>::value ));
    test_pass("cxx11:istreambuf_iterator::istream_type");
    test_true(( STD::is_same<InIt::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:istreambuf_iterator::traits_type");
    test_true(( STD::is_same<InIt::value_type, char>::value ));
    test_pass("cxx03:istreambuf_iterator::value_type");
    TEST_SKIP_WAT("libcxx03 istreambuf_iterator::difference_type is not streamoff");
    test_true(( STD::is_same<InIt::difference_type, STD::streamoff>::value ));
    test_pass("cxx03:istreambuf_iterator::difference_type");
    test_true(( STD::is_same<InIt::iterator_category, STD::input_iterator_tag>::value ));
    test_pass("cxx03:istreambuf_iterator::iterator_category");
#if _tst_cplusplus >= 201103L
    test_true(( STD::is_same<InIt::reference, char>::value ));
#else
    test_true(( STD::is_same<InIt::reference, char>::value ) ||
              ( STD::is_same<InIt::reference, char&>::value ));
#endif
    test_pass("cxx03:istreambuf_iterator::reference");
    test_true(( STD::is_same<InIt::pointer, char*>::value ) ||
              ( STD::is_same<InIt::pointer, const char*>::value ) ||
              STD::is_void<InIt::pointer>::value );
    test_pass("cxx03:istreambuf_iterator::pointer");

    test_true(( STD::is_same<OutIt::char_type, char>::value ));
    test_pass("cxx03:ostreambuf_iterator::char_type");
    test_true(( STD::is_same<OutIt::streambuf_type, STD::streambuf>::value ));
    test_pass("cxx03:ostreambuf_iterator::streambuf_type");
    test_true(( STD::is_same<OutIt::ostream_type, STD::ostream>::value ));
    test_pass("cxx11:ostreambuf_iterator::ostream_type");
    test_true(( STD::is_same<OutIt::traits_type, STD::char_traits<char> >::value ));
    test_pass("cxx11:ostreambuf_iterator::traits_type");
    test_true( STD::is_void<OutIt::value_type>::value );
    test_pass("cxx03:ostreambuf_iterator::value_type");
    test_true( STD::is_void<OutIt::pointer>::value );
    test_pass("cxx03:ostreambuf_iterator::pointer");
    test_true( STD::is_void<OutIt::reference>::value );
    test_pass("cxx03:ostreambuf_iterator::reference");
    test_true(( STD::is_same<OutIt::iterator_category, STD::output_iterator_tag>::value ));
    test_pass("cxx03:ostreambuf_iterator::iterator_category");
#if _TST_HAS_CXX20_LIB_MEMBERS
    TEST_SKIP_WAT("libcxx03 ostreambuf_iterator::difference_type is void, not ptrdiff_t");
    test_true(( STD::is_same<OutIt::difference_type, STD::ptrdiff_t>::value ));
#else
    test_true( STD::is_void<OutIt::difference_type>::value );
#endif
    test_pass("cxx03:ostreambuf_iterator::difference_type");

    {
        STD::istringstream is("abc");
        InIt it(is), end;
        test_eq( *it, 'a' );
        test_eq( *it, 'a' );
        test_pass("cxx03:istreambuf_iterator::operator*");
        ++it;
        test_eq( *it, 'b' );
        it++;
        test_eq( *it, 'c' );
        test_pass("cxx03:istreambuf_iterator::operator++");
        InIt copy = it;
        test_true( copy.equal(it) );
        test_true( !copy.equal(end) );
        test_pass("cxx03:istreambuf_iterator::equal");
        test_true( copy == it );
        test_true( copy != end );
        test_pass("cxx03:istreambuf_iterator::operator==");
        InIt assigned;
        assigned = it;
        test_eq( *assigned, 'c' );
        test_pass("cxx03:istreambuf_iterator::operator=");
        ++it;
        test_true( it == end );
    }
    {
        STD::ostringstream os;
        OutIt it(os);
        *it = 'x';
        test_eq( os.str(), STD::string("x") );
        test_pass("cxx11:ostreambuf_iterator::operator*");
        it = 'y';
        test_eq( os.str(), STD::string("xy") );
        test_pass("cxx03:ostreambuf_iterator::operator=");
        ++it; it++;
        *it = 'z';
        test_eq( os.str(), STD::string("xyz") );
        test_pass("cxx11:ostreambuf_iterator::operator++");
        test_true( !it.failed() );
        test_pass("cxx03:ostreambuf_iterator::failed");
    }
}

#if _TST_HAS_RVALUE_REF && TEST_TARGET_CXX >= 2011
TEST_CASE(iterator, move_iterator_members_cxx11) {
    typedef STD::vector<STD::string>::iterator It;
    typedef STD::move_iterator<It> MIt;

    test_true(( STD::is_same<MIt::iterator_type, It>::value ));
    test_pass("cxx11:move_iterator::iterator_type");
    test_true(( STD::is_same<MIt::value_type, STD::string>::value ));
    test_pass("cxx11:move_iterator::value_type");
    test_true(( STD::is_same<MIt::difference_type,
                             STD::iterator_traits<It>::difference_type>::value ));
    test_pass("cxx11:move_iterator::difference_type");
    test_true(( STD::is_same<MIt::reference, STD::string&&>::value ));
    test_pass("cxx11:move_iterator::reference");
    test_true(( STD::is_same<MIt::pointer, It>::value ));
    test_pass("cxx11:move_iterator::pointer");
    test_true(( STD::is_same<MIt::iterator_category,
                             STD::random_access_iterator_tag>::value ));
    test_pass("cxx11:move_iterator::iterator_category");

#if defined(__cpp_lib_concepts)
    test_true(( STD::is_same<MIt::iterator_concept,
                             STD::random_access_iterator_tag>::value ));
    test_pass("cxx20:move_iterator::iterator_concept");
#else
    test_skip("cxx20:move_iterator::iterator_concept");
#endif
}

TEST_CASE(iterator, move_iterator_operators_cxx11) {
    STD::vector<STD::string> v;
    v.push_back("aa");
    v.push_back("bb");
    v.push_back("cc");
    typedef STD::vector<STD::string>::iterator It;
    typedef STD::move_iterator<It> MIt;

    MIt m(v.begin());
    test_true( m.base() == v.begin() );
    test_pass("cxx11:move_iterator::base");

    {
        STD::string taken = *m;
        test_eq( taken, STD::string("aa") );
        test_true( v[0].empty() );
    }
    test_pass("cxx11:move_iterator::operator*");

    v[0] = "aa";
    test_eq( m->size(), (STD::size_t)2 );
    test_pass("cxx11:move_iterator::operator->");

    MIt n(v.begin());
    ++n;
    test_eq( (*n.base()), STD::string("bb") );
    MIt o = n++;
    test_true( o.base() == v.begin() + 1 );
    test_true( n.base() == v.begin() + 2 );
    test_pass("cxx11:move_iterator::operator++");

    --n;
    test_true( n.base() == v.begin() + 1 );
    MIt p = n--;
    test_true( p.base() == v.begin() + 1 );
    test_true( n.base() == v.begin() );
    test_pass("cxx11:move_iterator::operator--");

    MIt q = n + 2;
    test_true( q.base() == v.begin() + 2 );
    test_pass("cxx11:move_iterator::operator+");
    MIt r = q - 1;
    test_true( r.base() == v.begin() + 1 );
    test_eq( (int)(q - n), 2 );
    test_pass("cxx11:move_iterator::operator-");

    MIt s(v.begin());
    s += 2;
    test_true( s.base() == v.begin() + 2 );
    test_pass("cxx11:move_iterator::operator+=");
    s -= 1;
    test_true( s.base() == v.begin() + 1 );
    test_pass("cxx11:move_iterator::operator-=");

    MIt t(v.begin());
    {
        STD::string taken = t[2];
        test_eq( taken, STD::string("cc") );
        test_true( v[2].empty() );
        v[2] = "cc";
    }
    test_pass("cxx11:move_iterator::operator[]");

    MIt u(v.begin());
    MIt w(v.end());
    w = u;
    test_true( w.base() == v.begin() );
    test_pass("cxx11:move_iterator::operator=");

    test_true( u == w );
    test_true( u != MIt(v.end()) );
    test_pass("cxx20:move_iterator::operator==");
}
#else
TEST_CASE_SKIP(iterator, move_iterator_members_cxx11)
TEST_CASE_SKIP(iterator, move_iterator_operators_cxx11)
#endif

TEST_CASE(iterator, sentinels_cxx20) {
#if defined(__cpp_lib_concepts)
    STD::vector<int> v;
    for (int i = 0; i < 3; ++i) v.push_back(i);
    STD::counted_iterator<STD::vector<int>::iterator> ci(v.begin(), 3);
    test_true( ci != STD::default_sentinel );
    STD::counted_iterator<STD::vector<int>::iterator> done(v.end(), 0);
    test_true( done == STD::default_sentinel );
    test_true(( STD::is_same<STD::default_sentinel_t,
                             STD::remove_const<decltype(STD::default_sentinel)>::type>::value ));
    test_pass("cxx20:default_sentinel_t");

    test_true( v.begin() != STD::unreachable_sentinel );
    test_true( v.end() != STD::unreachable_sentinel );
    test_true(( STD::is_same<STD::unreachable_sentinel_t,
                             STD::remove_const<decltype(STD::unreachable_sentinel)>::type>::value ));
    test_pass("cxx20:unreachable_sentinel_t");

    test_true(( !STD::disable_sized_sentinel_for<STD::vector<int>::iterator,
                                                 STD::vector<int>::iterator> ));
    test_true(( STD::sized_sentinel_for<STD::vector<int>::iterator,
                                        STD::vector<int>::iterator> ));
    test_pass("cxx20:disable_sized_sentinel_for");

    STD::move_sentinel<STD::default_sentinel_t> ms;
    STD::default_sentinel_t inner = ms.base();
    (void)inner;
    test_true( true );
    test_pass("cxx20:move_sentinel::base");
    STD::move_sentinel<STD::default_sentinel_t> ms2;
    ms2 = ms;
    test_true(( STD::is_same<decltype(ms2.base()), STD::default_sentinel_t>::value ));
    test_pass("cxx20:move_sentinel::operator=");
#else
    test_skip("cxx20:default_sentinel_t");
    test_skip("cxx20:unreachable_sentinel_t");
    test_skip("cxx20:disable_sized_sentinel_for");
    test_skip("cxx20:move_sentinel::base");
    test_skip("cxx20:move_sentinel::operator=");
#endif
}

TEST_CASE(iterator, counted_iterator_cxx20) {
#if defined(__cpp_lib_concepts)
    STD::vector<ItPair> v;
    for (int i = 0; i < 5; ++i) v.push_back(ItPair(i, i * 10));
    typedef STD::vector<ItPair>::iterator It;
    typedef STD::counted_iterator<It> CIt;

    CIt c(v.begin(), 4);
    test_true( c.base() == v.begin() );
    test_pass("cxx20:counted_iterator::base");
    test_eq( (int)c.count(), 4 );
    test_pass("cxx20:counted_iterator::count");
    test_true(( STD::is_same<CIt::iterator_type, It>::value ));
    test_pass("cxx20:counted_iterator::iterator_type");
    test_true(( STD::is_same<CIt::value_type, ItPair>::value ));
    test_pass("cxx23:counted_iterator::value_type");
    test_true(( STD::is_same<CIt::difference_type,
                             STD::iterator_traits<It>::difference_type>::value ));
    test_pass("cxx23:counted_iterator::difference_type");
    test_true(( STD::is_same<CIt::iterator_category,
                             STD::random_access_iterator_tag>::value ));
    test_pass("cxx23:counted_iterator::iterator_category");
    test_true(( STD::is_same<CIt::iterator_concept, STD::contiguous_iterator_tag>::value ||
                STD::is_same<CIt::iterator_concept, STD::random_access_iterator_tag>::value ));
    test_pass("cxx23:counted_iterator::iterator_concept");

    test_eq( (*c).a, 0 );
    (*c).a = 100;
    test_eq( v[0].a, 100 );
    v[0].a = 0;
    test_pass("cxx20:counted_iterator::operator*");

    test_eq( c->b, 0 );
    test_eq( c->a, 0 );
    test_pass("cxx23:counted_iterator::operator->");

    CIt d = c;
    ++d;
    test_eq( (int)d.count(), 3 );
    test_eq( (*d).a, 1 );
    CIt e = d++;
    test_eq( (int)e.count(), 3 );
    test_eq( (int)d.count(), 2 );
    test_pass("cxx20:counted_iterator::operator++");

    --d;
    test_eq( (int)d.count(), 3 );
    CIt f = d--;
    test_eq( (int)f.count(), 3 );
    test_eq( (int)d.count(), 4 );
    test_pass("cxx20:counted_iterator::operator--");

    CIt g = c + 2;
    test_eq( (int)g.count(), 2 );
    test_eq( (*g).a, 2 );
    test_pass("cxx20:counted_iterator::operator+");

    CIt h = g - 1;
    test_eq( (int)h.count(), 3 );
    test_eq( (int)(g - h), 1 );
    test_pass("cxx20:counted_iterator::operator-");

    CIt i2 = c;
    i2 += 3;
    test_eq( (int)i2.count(), 1 );
    test_pass("cxx20:counted_iterator::operator+=");
    i2 -= 2;
    test_eq( (int)i2.count(), 3 );
    test_pass("cxx20:counted_iterator::operator-=");

    test_eq( c[2].a, 2 );
    test_pass("cxx20:counted_iterator::operator[]");

    CIt j(v.begin(), 1);
    j = c;
    test_eq( (int)j.count(), 4 );
    test_pass("cxx20:counted_iterator::operator=");

    test_true( j == c );
    test_true( j != (c + 1) );
    test_true( CIt(v.end(), 0) == STD::default_sentinel );
    test_pass("cxx20:counted_iterator::operator==");

    test_true( (c <=> (c + 1)) < 0 );
    test_true( ((c + 1) <=> c) > 0 );
    test_true( (c <=> j) == 0 );
    test_pass("cxx20:counted_iterator::operator<=>");
#else
    test_skip("cxx20:counted_iterator::base");
    test_skip("cxx20:counted_iterator::count");
    test_skip("cxx20:counted_iterator::iterator_type");
    test_skip("cxx23:counted_iterator::value_type");
    test_skip("cxx23:counted_iterator::difference_type");
    test_skip("cxx23:counted_iterator::iterator_category");
    test_skip("cxx23:counted_iterator::iterator_concept");
    test_skip("cxx20:counted_iterator::operator*");
    test_skip("cxx23:counted_iterator::operator->");
    test_skip("cxx20:counted_iterator::operator++");
    test_skip("cxx20:counted_iterator::operator--");
    test_skip("cxx20:counted_iterator::operator+");
    test_skip("cxx20:counted_iterator::operator-");
    test_skip("cxx20:counted_iterator::operator+=");
    test_skip("cxx20:counted_iterator::operator-=");
    test_skip("cxx20:counted_iterator::operator[]");
    test_skip("cxx20:counted_iterator::operator=");
    test_skip("cxx20:counted_iterator::operator==");
    test_skip("cxx20:counted_iterator::operator<=>");
#endif
}

TEST_CASE(iterator, common_iterator_cxx20) {
#if defined(__cpp_lib_concepts)
    STD::vector<ItPair> v;
    for (int i = 0; i < 4; ++i) v.push_back(ItPair(i, i * 10));
    typedef STD::counted_iterator<STD::vector<ItPair>::iterator> CIt;
    typedef STD::common_iterator<CIt, STD::default_sentinel_t> ComIt;

    ComIt it(CIt(v.begin(), 3));
    ComIt end(STD::default_sentinel);

    test_eq( (*it).a, 0 );
    test_pass("cxx20:common_iterator::operator*");
    test_eq( it->b, 0 );
    test_pass("cxx20:common_iterator::operator->");

    ++it;
    test_eq( (*it).a, 1 );
    it++;
    test_eq( (*it).a, 2 );
    test_pass("cxx20:common_iterator::operator++");

    test_true( it != end );
    ++it;
    test_true( it == end );
    test_pass("cxx20:common_iterator::operator==");

    ComIt a(CIt(v.begin(), 3));
    ComIt b(a);
    b = ComIt(CIt(v.begin() + 1, 2));
    test_eq( (*b).a, 1 );
    b = a;
    test_eq( (*b).a, 0 );
    test_pass("cxx20:common_iterator::operator=");

    ComIt from(CIt(v.begin(), 3));
    test_eq( (int)(end - from), 3 );
    test_pass("cxx20:common_iterator::operator-");
#else
    test_skip("cxx20:common_iterator::operator*");
    test_skip("cxx20:common_iterator::operator->");
    test_skip("cxx20:common_iterator::operator++");
    test_skip("cxx20:common_iterator::operator==");
    test_skip("cxx20:common_iterator::operator=");
    test_skip("cxx20:common_iterator::operator-");
#endif
}

TEST_CASE(iterator, basic_const_iterator_cxx23) {
#if defined(__cpp_lib_ranges_as_const)
    STD::vector<ItPair> v;
    for (int i = 0; i < 5; ++i) v.push_back(ItPair(i, i * 10));
    typedef STD::vector<ItPair>::iterator It;
    typedef STD::basic_const_iterator<It> BCIt;

    BCIt c(v.begin());
    test_eq( (*c).a, 0 );
    test_true(( STD::is_same<decltype(*c), const ItPair&>::value ));
    test_pass("cxx23:basic_const_iterator");

    test_true( c.base() == v.begin() );
    test_pass("cxx23:basic_const_iterator::base");
    test_true(( STD::is_same<BCIt::value_type, ItPair>::value ));
    test_pass("cxx23:basic_const_iterator::value_type");
    test_true(( STD::is_same<BCIt::difference_type,
                             STD::iterator_traits<It>::difference_type>::value ));
    test_pass("cxx23:basic_const_iterator::difference_type");
    test_true(( STD::is_same<BCIt::iterator_category,
                             STD::random_access_iterator_tag>::value ));
    test_pass("cxx23:basic_const_iterator::iterator_category");
    test_true(( STD::is_same<BCIt::iterator_concept, STD::contiguous_iterator_tag>::value ||
                STD::is_same<BCIt::iterator_concept, STD::random_access_iterator_tag>::value ));
    test_pass("cxx23:basic_const_iterator::iterator_concept");

    test_eq( c->b, 0 );
    test_pass("cxx23:basic_const_iterator::operator->");

    BCIt d = c;
    ++d;
    test_eq( (*d).a, 1 );
    BCIt e = d++;
    test_eq( (*e).a, 1 );
    test_eq( (*d).a, 2 );
    test_pass("cxx23:basic_const_iterator::operator++");
    --d;
    test_eq( (*d).a, 1 );
    BCIt f = d--;
    test_eq( (*f).a, 1 );
    test_eq( (*d).a, 0 );
    test_pass("cxx23:basic_const_iterator::operator--");

    BCIt g = c + 3;
    test_eq( (*g).a, 3 );
    test_pass("cxx23:basic_const_iterator::operator+");
    BCIt h = g - 1;
    test_eq( (*h).a, 2 );
    test_eq( (int)(g - c), 3 );
    test_pass("cxx23:basic_const_iterator::operator-");

    BCIt i2 = c;
    i2 += 4;
    test_eq( (*i2).a, 4 );
    test_pass("cxx23:basic_const_iterator::operator+=");
    i2 -= 2;
    test_eq( (*i2).a, 2 );
    test_pass("cxx23:basic_const_iterator::operator-=");

    test_true( c == BCIt(v.begin()) );
    test_true( c != g );
    test_pass("cxx23:basic_const_iterator::operator==");
    test_true( c < g );
    test_pass("cxx23:basic_const_iterator::operator<");
    test_true( c <= BCIt(v.begin()) );
    test_pass("cxx23:basic_const_iterator::operator<=");
    test_true( g > c );
    test_pass("cxx23:basic_const_iterator::operator>");
    test_true( g >= BCIt(v.begin() + 3) );
    test_pass("cxx23:basic_const_iterator::operator>=");
    test_true( (c <=> g) < 0 );
    test_true( (c <=> BCIt(v.begin())) == 0 );
    test_pass("cxx23:basic_const_iterator::operator<=>");

    STD::vector<ItPair>::const_iterator plain = c;
    test_true( plain == v.cbegin() );
    test_pass("cxx23:basic_const_iterator conversion operator");

    test_true(( STD::is_same<decltype(STD::ranges::iter_move(c)),
                             const ItPair&&>::value ));
    test_pass("cxx23:iter_move(basic_const_iterator)");
#else
    test_skip("cxx23:basic_const_iterator");
    test_skip("cxx23:basic_const_iterator::base");
    test_skip("cxx23:basic_const_iterator::value_type");
    test_skip("cxx23:basic_const_iterator::difference_type");
    test_skip("cxx23:basic_const_iterator::iterator_category");
    test_skip("cxx23:basic_const_iterator::iterator_concept");
    test_skip("cxx23:basic_const_iterator::operator->");
    test_skip("cxx23:basic_const_iterator::operator++");
    test_skip("cxx23:basic_const_iterator::operator--");
    test_skip("cxx23:basic_const_iterator::operator+");
    test_skip("cxx23:basic_const_iterator::operator-");
    test_skip("cxx23:basic_const_iterator::operator+=");
    test_skip("cxx23:basic_const_iterator::operator-=");
    test_skip("cxx23:basic_const_iterator::operator==");
    test_skip("cxx23:basic_const_iterator::operator<");
    test_skip("cxx23:basic_const_iterator::operator<=");
    test_skip("cxx23:basic_const_iterator::operator>");
    test_skip("cxx23:basic_const_iterator::operator>=");
    test_skip("cxx23:basic_const_iterator::operator<=>");
    test_skip("cxx23:basic_const_iterator conversion operator");
    test_skip("cxx23:iter_move(basic_const_iterator)");
#endif
}

TEST_CASE(iterator, const_iterator_helpers_cxx23) {
#if defined(__cpp_lib_ranges_as_const)
    STD::vector<int> v;
    for (int i = 1; i <= 3; ++i) v.push_back(i);

    test_true(( STD::is_same<STD::const_iterator<STD::vector<int>::iterator>,
                             STD::basic_const_iterator<STD::vector<int>::iterator> >::value ));
    test_true(( STD::is_same<STD::const_iterator<STD::vector<int>::const_iterator>,
                             STD::vector<int>::const_iterator>::value ));
    test_pass("cxx23:const_iterator");

    test_true(( STD::is_same<STD::const_sentinel<STD::vector<int>::const_iterator>,
                             STD::vector<int>::const_iterator>::value ));
    test_true( sizeof(STD::const_sentinel<STD::vector<int>::iterator>) > 0 );
    test_pass("cxx23:const_sentinel");

    test_true(( STD::is_same<STD::iter_const_reference_t<STD::vector<int>::iterator>,
                             const int&>::value ));
    test_true(( STD::is_same<STD::iter_const_reference_t<STD::vector<int>::const_iterator>,
                             const int&>::value ));
    test_pass("cxx23:iter_const_reference_t");

    {
        STD::const_iterator<STD::vector<int>::iterator> c = STD::make_const_iterator(v.begin());
        test_eq( *c, 1 );
        test_true(( STD::is_same<decltype(*c), const int&>::value ));
    }
    test_pass("cxx23:make_const_iterator");

    {
        STD::const_sentinel<STD::vector<int>::iterator> s = STD::make_const_sentinel(v.end());
        STD::const_iterator<STD::vector<int>::iterator> c = STD::make_const_iterator(v.begin());
        int n = 0;
        while (c != s) { ++c; ++n; }
        test_eq( n, 3 );
        test_true( true );
    }
    test_pass("cxx23:make_const_sentinel");

    test_true(( STD::is_same<STD::common_type<STD::basic_const_iterator<STD::vector<int>::iterator>,
                                              STD::vector<int>::iterator>::type,
                             STD::basic_const_iterator<STD::vector<int>::iterator> >::value ));
    test_pass("cxx23:common_type");
#else
    test_skip("cxx23:const_iterator");
    test_skip("cxx23:const_sentinel");
    test_skip("cxx23:iter_const_reference_t");
    test_skip("cxx23:make_const_iterator");
    test_skip("cxx23:make_const_sentinel");
    test_skip("cxx23:common_type");
#endif
}

TEST_CASE(iterator, projected_value_t_cxx26) {
#if defined(__cpp_lib_algorithm_default_value_type) || defined(__cpp_lib_projected_value_t)
    test_true(( STD::is_same<STD::projected_value_t<STD::vector<int>::iterator,
                                                    STD::identity>, int>::value ));
    test_true(( STD::is_same<STD::projected_value_t<STD::vector<ItPair>::iterator,
                                                    STD::identity>, ItPair>::value ));
    test_pass("cxx26:projected_value_t");
#else
    TEST_NOTE("no feature macro announces projected_value_t here");
    test_skip("cxx26:projected_value_t");
#endif
}
