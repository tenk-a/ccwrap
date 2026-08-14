//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_UNINITIALIZED_ALGORITHMS_H
#define _CCW_LIBCPP___MEMORY_UNINITIALIZED_ALGORITHMS_H
#include "../__config"
#include "../__memory/addressof.h"
#include "../__iterator/iterator_traits.h"
#include <new>
#if _CCW_LIBCPP_NATIVE_STD
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _InputIter, class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter uninitialized_copy(_InputIter __first, _InputIter __last, _FwdIter __d) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    _FwdIter __cur = __d;
    for (; __first != __last; ++__first, ++__cur)
        ::new (static_cast<void*>(_CCW_STD::addressof(*__cur))) _Vt(*__first);
    return __cur;
}

template <class _FwdIter, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI void uninitialized_fill(_FwdIter __first, _FwdIter __last, const _Tp& __x) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    for (; __first != __last; ++__first)
        ::new (static_cast<void*>(_CCW_STD::addressof(*__first))) _Vt(__x);
}

template <class _FwdIter, class _Size, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter uninitialized_fill_n(_FwdIter __first, _Size __n, const _Tp& __x) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    for (; __n > 0; --__n, ++__first)
        ::new (static_cast<void*>(_CCW_STD::addressof(*__first))) _Vt(__x);
    return __first;
}

template <class _InputIter, class _Size, class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter uninitialized_copy_n(_InputIter __first, _Size __n, _FwdIter __d) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    for (; __n > 0; --__n, ++__first, ++__d)
        ::new (static_cast<void*>(_CCW_STD::addressof(*__d))) _Vt(*__first);
    return __d;
}

template <class _InputIter, class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter uninitialized_move(_InputIter __first, _InputIter __last, _FwdIter __d) {
    return _CCW_STD::uninitialized_copy(__first, __last, __d);
}
template <class _InputIter, class _Size, class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter uninitialized_move_n(_InputIter __first, _Size __n, _FwdIter __d) {
    return _CCW_STD::uninitialized_copy_n(__first, __n, __d);
}

template <class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI void uninitialized_default_construct(_FwdIter __first, _FwdIter __last) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    for (; __first != __last; ++__first) ::new (static_cast<void*>(_CCW_STD::addressof(*__first))) _Vt;
}
template <class _FwdIter, class _Size>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter uninitialized_default_construct_n(_FwdIter __first, _Size __n) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    for (; __n > 0; --__n, ++__first) ::new (static_cast<void*>(_CCW_STD::addressof(*__first))) _Vt;
    return __first;
}
template <class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI void uninitialized_value_construct(_FwdIter __first, _FwdIter __last) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    for (; __first != __last; ++__first) ::new (static_cast<void*>(_CCW_STD::addressof(*__first))) _Vt();
}
template <class _FwdIter, class _Size>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter uninitialized_value_construct_n(_FwdIter __first, _Size __n) {
    typedef typename iterator_traits<_FwdIter>::value_type _Vt;
    for (; __n > 0; --__n, ++__first) ::new (static_cast<void*>(_CCW_STD::addressof(*__first))) _Vt();
    return __first;
}

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI void destroy_at(_Tp* __p) { __p->~_Tp(); }
template <class _FwdIter>
_CCW_LIBCPP_HIDE_FROM_ABI void destroy(_FwdIter __first, _FwdIter __last) {
    for (; __first != __last; ++__first) _CCW_STD::destroy_at(_CCW_STD::addressof(*__first));
}
template <class _FwdIter, class _Size>
_CCW_LIBCPP_HIDE_FROM_ABI _FwdIter destroy_n(_FwdIter __first, _Size __n) {
    for (; __n > 0; --__n, ++__first) _CCW_STD::destroy_at(_CCW_STD::addressof(*__first));
    return __first;
}

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
