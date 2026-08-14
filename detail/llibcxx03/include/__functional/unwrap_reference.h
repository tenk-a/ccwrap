//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom. unwrap_reference / unwrap_ref_decay (C++20).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_UNWRAP_REFERENCE_H
#define _CCW_LIBCPP___FUNCTIONAL_UNWRAP_REFERENCE_H
#include "../__config"
#include "../__functional/reference_wrapper.h"
#include "../__type_traits/decay.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct unwrap_reference { typedef _Tp type; };
template <class _Tp> struct unwrap_reference< reference_wrapper<_Tp> > { typedef _Tp& type; };
template <class _Tp> struct unwrap_ref_decay {
    typedef typename unwrap_reference<typename decay<_Tp>::type>::type type;
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
