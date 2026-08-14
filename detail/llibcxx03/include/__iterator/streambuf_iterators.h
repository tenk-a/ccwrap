//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap: istreambuf_iterator / ostreambuf_iterator (classic C++03, Open Watcom).
//  The istream&/ostream& constructors are dependent, so <istream>/<ostream> only
//  need to be complete where the iterator is actually used, not here.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_STREAMBUF_ITERATORS_H
#define _CCW_LIBCPP___ITERATOR_STREAMBUF_ITERATORS_H

#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__string/char_traits.h"
#include <streambuf>
#include <cstddef>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _CharT, class _Traits> class basic_istream;
template <class _CharT, class _Traits> class basic_ostream;

template <class _CharT, class _Traits = char_traits<_CharT> >
class _CCW_LIBCPP_TEMPLATE_VIS istreambuf_iterator {
public:
    typedef input_iterator_tag                  iterator_category;
    typedef _CharT                              value_type;
    typedef typename _Traits::off_type          difference_type;
    typedef _CharT*                             pointer;
    typedef _CharT                              reference;
    typedef _CharT                              char_type;
    typedef _Traits                             traits_type;
    typedef typename _Traits::int_type          int_type;
    typedef basic_streambuf<_CharT, _Traits>    streambuf_type;
    typedef basic_istream<_CharT, _Traits>      istream_type;

    _CCW_LIBCPP_HIDE_FROM_ABI istreambuf_iterator() _CCW_LIBCPP_NOEXCEPT : __sbuf_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI istreambuf_iterator(istream_type& __s) _CCW_LIBCPP_NOEXCEPT : __sbuf_(__s.rdbuf()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI istreambuf_iterator(streambuf_type* __s) _CCW_LIBCPP_NOEXCEPT : __sbuf_(__s) {}

    _CCW_LIBCPP_HIDE_FROM_ABI _CharT operator*() const { return _Traits::to_char_type(__sbuf_->sgetc()); }
    _CCW_LIBCPP_HIDE_FROM_ABI istreambuf_iterator& operator++() { __sbuf_->sbumpc(); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI istreambuf_iterator  operator++(int) {
        istreambuf_iterator __t(*this); __sbuf_->sbumpc(); return __t;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool equal(const istreambuf_iterator& __b) const {
        return __test_for_eof() == __b.__test_for_eof();
    }
private:
    mutable streambuf_type* __sbuf_;
    _CCW_LIBCPP_HIDE_FROM_ABI bool __test_for_eof() const {
        if (__sbuf_ && _Traits::eq_int_type(__sbuf_->sgetc(), _Traits::eof())) __sbuf_ = 0;
        return __sbuf_ == 0;
    }
};

template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const istreambuf_iterator<_CharT, _Traits>& __a, const istreambuf_iterator<_CharT, _Traits>& __b) { return __a.equal(__b); }
template <class _CharT, class _Traits>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const istreambuf_iterator<_CharT, _Traits>& __a, const istreambuf_iterator<_CharT, _Traits>& __b) { return !__a.equal(__b); }

template <class _CharT, class _Traits = char_traits<_CharT> >
class _CCW_LIBCPP_TEMPLATE_VIS ostreambuf_iterator {
public:
    typedef output_iterator_tag                 iterator_category;
    typedef void                                value_type;
    typedef void                                difference_type;
    typedef void                                pointer;
    typedef void                                reference;
    typedef _CharT                              char_type;
    typedef _Traits                             traits_type;
    typedef basic_streambuf<_CharT, _Traits>    streambuf_type;
    typedef basic_ostream<_CharT, _Traits>      ostream_type;

    _CCW_LIBCPP_HIDE_FROM_ABI ostreambuf_iterator(ostream_type& __s) _CCW_LIBCPP_NOEXCEPT : __sbuf_(__s.rdbuf()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI ostreambuf_iterator(streambuf_type* __s) _CCW_LIBCPP_NOEXCEPT : __sbuf_(__s) {}

    _CCW_LIBCPP_HIDE_FROM_ABI ostreambuf_iterator& operator=(_CharT __c) {
        if (__sbuf_ && _Traits::eq_int_type(__sbuf_->sputc(__c), _Traits::eof())) __sbuf_ = 0;
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI ostreambuf_iterator& operator*()     { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI ostreambuf_iterator& operator++()    { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI ostreambuf_iterator& operator++(int) { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool failed() const _CCW_LIBCPP_NOEXCEPT { return __sbuf_ == 0; }
private:
    streambuf_type* __sbuf_;
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___ITERATOR_STREAMBUF_ITERATORS_H
