/**
 *  @file   detail/cxx/has_allocator_type.hpp
 *  @brief  __ccw::__has_allocator_type<T> -- does T have a nested allocator_type?
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_HAS_ALLOCATOR_TYPE_H
#define _CCW_DETAIL_HAS_ALLOCATOR_TYPE_H
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

namespace __ccw {

typedef char __hat_yes_t;
struct __hat_no_t { char __pad[2]; };

template <class _Tp>
struct __has_allocator_type {
    template <class _Up> static __hat_yes_t __test(typename _Up::allocator_type*);
    template <class _Up> static __hat_no_t  __test(...);
    enum { __m = sizeof(__test<_Tp>(0)) == sizeof(__hat_yes_t) };
    enum { value = __m };
};

}   // namespace __ccw

#endif  // _CCW_DETAIL_HAS_ALLOCATOR_TYPE_H
