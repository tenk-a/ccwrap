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
template <class _Tp>
struct decay {
private:
    typedef typename remove_reference<_Tp>::type _U;
public:
    typedef typename remove_cv<_U>::type type;
};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
