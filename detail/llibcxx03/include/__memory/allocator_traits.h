//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_ALLOCATOR_TRAITS_H
#define _CCW_LIBCPP___MEMORY_ALLOCATOR_TRAITS_H
#include "../__config"
#include "../__type_traits/integral_constant.h"
#include "../__type_traits/__ccw_intrinsic_traits.h"   // is_empty (needs an intrinsic; absent on Open Watcom)
#include "../__memory/allocate_at_least.h"   // allocation_result (returned by allocate_at_least)
#include <cstddef>
#if _CCW_LIBCPP_HAS_NATIVE_MEMORY_TRAITS
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#define _CCW_AT_DETECT(NAME)                                                                  template <class _Tp> char __ccw_at_probe_##NAME(typename _Tp::NAME*);                      template <class _Tp> struct __ccw_at_no_##NAME { char __pad_[2]; };                        template <class _Tp> __ccw_at_no_##NAME<_Tp> __ccw_at_probe_##NAME(...);                   template <class _Tp> struct __ccw_at_has_##NAME {                                              enum { value = sizeof(__ccw_at_probe_##NAME<_Tp>(0)) == 1 };                           };                                                                                         template <bool _Have, class _Tp, class _Dflt> struct __ccw_at_pick_##NAME                      { typedef typename _Tp::NAME type; };                                                  template <class _Tp, class _Dflt> struct __ccw_at_pick_##NAME<false, _Tp, _Dflt>               { typedef _Dflt type; };                                                               template <class _Tp, class _Dflt> struct __ccw_at_get_##NAME {                                 enum { __have = __ccw_at_has_##NAME<_Tp>::value };                                         typedef typename __ccw_at_pick_##NAME<(bool)__have, _Tp, _Dflt>::type type;            };

_CCW_AT_DETECT(pointer)
_CCW_AT_DETECT(const_pointer)
_CCW_AT_DETECT(void_pointer)
_CCW_AT_DETECT(const_void_pointer)
_CCW_AT_DETECT(size_type)
_CCW_AT_DETECT(difference_type)
_CCW_AT_DETECT(propagate_on_container_copy_assignment)
_CCW_AT_DETECT(propagate_on_container_move_assignment)
_CCW_AT_DETECT(propagate_on_container_swap)
_CCW_AT_DETECT(is_always_equal)
#undef _CCW_AT_DETECT

template <class _Alloc>
struct allocator_traits {
    typedef _Alloc                                allocator_type;
    typedef typename _Alloc::value_type           value_type;
    typedef typename __ccw_at_get_pointer<_Alloc, value_type*>::type            pointer;
    typedef typename __ccw_at_get_const_pointer<_Alloc, const value_type*>::type const_pointer;
    typedef typename __ccw_at_get_void_pointer<_Alloc, void*>::type             void_pointer;
    typedef typename __ccw_at_get_const_void_pointer<_Alloc, const void*>::type const_void_pointer;
    typedef typename __ccw_at_get_size_type<_Alloc, _CCW_STD::size_t>::type     size_type;
    typedef typename __ccw_at_get_difference_type<_Alloc, _CCW_STD::ptrdiff_t>::type difference_type;

    typedef typename __ccw_at_get_propagate_on_container_copy_assignment<_Alloc, false_type>::type
        propagate_on_container_copy_assignment;
    typedef typename __ccw_at_get_propagate_on_container_move_assignment<_Alloc, false_type>::type
        propagate_on_container_move_assignment;
    typedef typename __ccw_at_get_propagate_on_container_swap<_Alloc, false_type>::type
        propagate_on_container_swap;
#if defined(__WATCOMC__)
    typedef typename __ccw_at_get_is_always_equal<_Alloc, false_type>::type is_always_equal;
#else
    typedef typename __ccw_at_get_is_always_equal<_Alloc,
        integral_constant<bool, is_empty<_Alloc>::value> >::type is_always_equal;
#endif

    template <class _Tp> struct rebind_alloc { typedef typename _Alloc::template rebind<_Tp>::other type; };
    template <class _Tp> struct rebind_traits { typedef allocator_traits<typename _Alloc::template rebind<_Tp>::other> type; };

    _CCW_LIBCPP_HIDE_FROM_ABI static pointer allocate(_Alloc& __a, size_type __n) { return __a.allocate(__n); }
    _CCW_LIBCPP_HIDE_FROM_ABI static pointer allocate(_Alloc& __a, size_type __n, const_pointer __hint) {
        return __a.allocate(__n, (const void*)__hint);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static void    deallocate(_Alloc& __a, pointer __p, size_type __n) { __a.deallocate(__p, __n); }
    _CCW_LIBCPP_HIDE_FROM_ABI static void    construct(_Alloc& __a, pointer __p, const value_type& __v) { __a.construct(__p, __v); }
    _CCW_LIBCPP_HIDE_FROM_ABI static void    destroy(_Alloc& __a, pointer __p) { __a.destroy(__p); }
    _CCW_LIBCPP_HIDE_FROM_ABI static size_type max_size(const _Alloc& __a) { return __a.max_size(); }
    _CCW_LIBCPP_HIDE_FROM_ABI static _Alloc  select_on_container_copy_construction(const _Alloc& __a) { return __a; }

#if !_CCW_LIBCPP_NATIVE_STD
    _CCW_LIBCPP_HIDE_FROM_ABI static allocation_result<pointer, size_type> allocate_at_least(_Alloc& __a, size_type __n) {
        allocation_result<pointer, size_type> __r; __r.ptr = __a.allocate(__n); __r.count = __n; return __r;
    }
#endif
};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_MEMORY_TRAITS
#endif
