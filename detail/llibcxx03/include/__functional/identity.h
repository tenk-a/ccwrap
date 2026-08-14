//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_IDENTITY_H
#define _CCW_LIBCPP___FUNCTIONAL_IDENTITY_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct identity {
  typedef void is_transparent;
  template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI _Tp&       operator()(_Tp& __t)       const { return __t; }
  template <class _Tp> _CCW_LIBCPP_HIDE_FROM_ABI const _Tp& operator()(const _Tp& __t) const { return __t; }
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
