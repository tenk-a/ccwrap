//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_ITERATOR_TRAITS_H
#define _CCW_LIBCPP___ITERATOR_ITERATOR_TRAITS_H
#include "../__config"
#include <cstddef>

#if _CCW_LIBCPP_HAS_NATIVE_ITERATOR_TRAITS
#  include <iterator>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct _CCW_LIBCPP_TEMPLATE_VIS input_iterator_tag {};
struct _CCW_LIBCPP_TEMPLATE_VIS output_iterator_tag {};
struct _CCW_LIBCPP_TEMPLATE_VIS forward_iterator_tag       : public input_iterator_tag {};
struct _CCW_LIBCPP_TEMPLATE_VIS bidirectional_iterator_tag : public forward_iterator_tag {};
struct _CCW_LIBCPP_TEMPLATE_VIS random_access_iterator_tag : public bidirectional_iterator_tag {};
struct _CCW_LIBCPP_TEMPLATE_VIS contiguous_iterator_tag    : public random_access_iterator_tag {};  // C++20

template <class _Iter>
struct _CCW_LIBCPP_TEMPLATE_VIS iterator_traits {
    typedef typename _Iter::difference_type   difference_type;
    typedef typename _Iter::value_type        value_type;
    typedef typename _Iter::pointer           pointer;
    typedef typename _Iter::reference         reference;
    typedef typename _Iter::iterator_category iterator_category;
};

template <class _Tp>
struct _CCW_LIBCPP_TEMPLATE_VIS iterator_traits<_Tp*> {
    typedef _CCW_STD::ptrdiff_t               difference_type;
    typedef _Tp                          value_type;
    typedef _Tp*                         pointer;
    typedef _Tp&                         reference;
    typedef random_access_iterator_tag   iterator_category;
};

template <class _Tp>
struct _CCW_LIBCPP_TEMPLATE_VIS iterator_traits<const _Tp*> {
    typedef _CCW_STD::ptrdiff_t               difference_type;
    typedef _Tp                          value_type;
    typedef const _Tp*                   pointer;
    typedef const _Tp&                   reference;
    typedef random_access_iterator_tag   iterator_category;
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_ITERATOR_TRAITS
#endif
