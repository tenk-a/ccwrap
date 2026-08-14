// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_POINTER_TRAITS_H
#define _CCW_LIBCPP___MEMORY_POINTER_TRAITS_H
#include "../__config"
#include "../__memory/addressof.h"
#include <cstddef>
#if _CCW_LIBCPP_HAS_NATIVE_MEMORY_TRAITS
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Ptr>
struct pointer_traits {
    typedef _Ptr                             pointer;
    typedef typename _Ptr::element_type      element_type;
    typedef typename _Ptr::difference_type   difference_type;
    _CCW_LIBCPP_HIDE_FROM_ABI static pointer pointer_to(element_type& __r) { return _Ptr::pointer_to(__r); }
};

template <class _Tp>
struct pointer_traits<_Tp*> {
    typedef _Tp*             pointer;
    typedef _Tp              element_type;
    typedef _CCW_STD::ptrdiff_t   difference_type;
    template <class _Up> struct rebind { typedef _Up* other; };
    _CCW_LIBCPP_HIDE_FROM_ABI static pointer pointer_to(element_type& __r) { return _CCW_STD::addressof(__r); }
};

#if !_CCW_LIBCPP_NATIVE_STD
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp* to_address(_Tp* __p) { return __p; }
template <class _Ptr>
_CCW_LIBCPP_HIDE_FROM_ABI typename pointer_traits<_Ptr>::element_type* to_address(const _Ptr& __p) { return _CCW_STD::to_address(__p.operator->()); }
#endif // !_CCW_LIBCPP_NATIVE_STD

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_MEMORY_TRAITS
#endif
