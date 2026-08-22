//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_DECAY_H
#define _CCW_LIBCPP___TYPE_TRAITS_DECAY_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/remove_reference.h"
#include "../__type_traits/remove_cv.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>          struct __ccw_decay_sel           { typedef typename remove_cv<_Tp>::type type; };
template <class _Tp>          struct __ccw_decay_sel<_Tp[]>    { typedef _Tp* type; };
template <class _Tp, int _Np> struct __ccw_decay_sel<_Tp[_Np]> { typedef _Tp* type; };
template <class _Rp>
struct __ccw_decay_sel<_Rp()> { typedef _Rp (*type)(); };
template <class _Rp, class _A0>
struct __ccw_decay_sel<_Rp(_A0)> { typedef _Rp (*type)(_A0); };
template <class _Rp, class _A0, class _A1>
struct __ccw_decay_sel<_Rp(_A0, _A1)> { typedef _Rp (*type)(_A0, _A1); };
template <class _Rp, class _A0, class _A1, class _A2>
struct __ccw_decay_sel<_Rp(_A0, _A1, _A2)> { typedef _Rp (*type)(_A0, _A1, _A2); };
template <class _Rp, class _A0, class _A1, class _A2, class _A3>
struct __ccw_decay_sel<_Rp(_A0, _A1, _A2, _A3)> { typedef _Rp (*type)(_A0, _A1, _A2, _A3); };

template <class _Tp>
struct decay {
private:
    typedef typename remove_reference<_Tp>::type _Up;
public:
    typedef typename __ccw_decay_sel<_Up>::type type;
};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
