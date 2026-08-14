//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. back/front/insert output iterators.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_INSERT_ITERATORS_H
#define _CCW_LIBCPP___ITERATOR_INSERT_ITERATORS_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__utility/move.h"
#include <cstddef>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Container>
class back_insert_iterator {
protected:
    _Container* __c_;
public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef _CCW_STD::ptrdiff_t      difference_type;
    typedef void                pointer;
    typedef void                reference;
    typedef _Container          container_type;

    _CCW_LIBCPP_HIDE_FROM_ABI explicit back_insert_iterator(_Container& __c) : __c_(&__c) {}
    _CCW_LIBCPP_HIDE_FROM_ABI back_insert_iterator& operator=(const typename _Container::value_type& __v) { __c_->push_back(__v); return *this; }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI back_insert_iterator& operator=(_CCW_RV_REF_BEG typename _Container::value_type _CCW_RV_REF_END __v) { __c_->push_back(__v); return *this; }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI back_insert_iterator& operator*()     { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI back_insert_iterator& operator++()    { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI back_insert_iterator  operator++(int) { return *this; }
};
template <class _Container> _CCW_LIBCPP_HIDE_FROM_ABI back_insert_iterator<_Container> back_inserter(_Container& __c) { return back_insert_iterator<_Container>(__c); }

template <class _Container>
class front_insert_iterator {
protected:
    _Container* __c_;
public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef _CCW_STD::ptrdiff_t      difference_type;
    typedef void                pointer;
    typedef void                reference;
    typedef _Container          container_type;

    _CCW_LIBCPP_HIDE_FROM_ABI explicit front_insert_iterator(_Container& __c) : __c_(&__c) {}
    _CCW_LIBCPP_HIDE_FROM_ABI front_insert_iterator& operator=(const typename _Container::value_type& __v) { __c_->push_front(__v); return *this; }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI front_insert_iterator& operator=(_CCW_RV_REF_BEG typename _Container::value_type _CCW_RV_REF_END __v) { __c_->push_front(__v); return *this; }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI front_insert_iterator& operator*()     { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI front_insert_iterator& operator++()    { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI front_insert_iterator  operator++(int) { return *this; }
};
template <class _Container> _CCW_LIBCPP_HIDE_FROM_ABI front_insert_iterator<_Container> front_inserter(_Container& __c) { return front_insert_iterator<_Container>(__c); }

template <class _Container>
class insert_iterator {
protected:
    _Container* __c_;
    typename _Container::iterator __i_;
public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef _CCW_STD::ptrdiff_t      difference_type;
    typedef void                pointer;
    typedef void                reference;
    typedef _Container          container_type;

    _CCW_LIBCPP_HIDE_FROM_ABI insert_iterator(_Container& __c, typename _Container::iterator __i) : __c_(&__c), __i_(__i) {}
    _CCW_LIBCPP_HIDE_FROM_ABI insert_iterator& operator=(const typename _Container::value_type& __v) { __i_ = __c_->insert(__i_, __v); ++__i_; return *this; }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI insert_iterator& operator=(_CCW_RV_REF_BEG typename _Container::value_type _CCW_RV_REF_END __v) { __i_ = __c_->insert(__i_, __v); ++__i_; return *this; }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI insert_iterator& operator*()     { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI insert_iterator& operator++()    { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI insert_iterator& operator++(int) { return *this; }
};
template <class _Container, class _It> _CCW_LIBCPP_HIDE_FROM_ABI insert_iterator<_Container> inserter(_Container& __c, _It __i) { return insert_iterator<_Container>(__c, __i); }

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
