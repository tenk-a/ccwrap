/*
 * ccwrap: libc++ layout. classic C++03 body.
 *   default_delete, _CCW_STD::align, uses_allocator, construct_at.
 */
#ifndef _CCW_LIBCPP___MEMORY_MISC_H
#define _CCW_LIBCPP___MEMORY_MISC_H
#include "../__config"
#include "../__fnctmpl.h"
#include <new>
#include <cstddef>

#if _CCW_LIBCPP_NATIVE_STD
#  include <memory>
#endif

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#if !_CCW_LIBCPP_NATIVE_STD
template <class _Tp>
struct default_delete {
    _CCW_LIBCPP_HIDE_FROM_ABI default_delete() {}
    template <class _Up> _CCW_LIBCPP_HIDE_FROM_ABI default_delete(const default_delete<_Up>&) {}
    _CCW_LIBCPP_HIDE_FROM_ABI void operator()(_Tp* __p) const { delete __p; }
};
template <class _Tp>
struct default_delete<_Tp[]> {
    _CCW_LIBCPP_HIDE_FROM_ABI default_delete() {}
    _CCW_LIBCPP_HIDE_FROM_ABI void operator()(_Tp* __p) const { delete[] __p; }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline void* align(_CCW_STD::size_t __alignment, _CCW_STD::size_t __size, void*& __ptr, _CCW_STD::size_t& __space) {
    if (__space < __size) return 0;
    _CCW_STD::size_t __p       = reinterpret_cast<_CCW_STD::size_t>(__ptr);
    _CCW_STD::size_t __aligned = (__p + __alignment - 1) & ~(__alignment - 1);
    _CCW_STD::size_t __pad     = __aligned - __p;
    if (__pad > __space - __size) return 0;
    __space -= __pad;
    __ptr = reinterpret_cast<void*>(__aligned);
    return __ptr;
}

template <class _Tp, class _Alloc>
struct uses_allocator { static const bool value = false; };
#endif // !_CCW_LIBCPP_NATIVE_STD

#if !_CCW_LIBCPP_NATIVE_STD
template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp* construct_at(_Tp* __p) { return ::new (static_cast<void*>(__p)) _Tp(); }
#if _CCW_NO_VARIADIC_TEMPLATES
#  define _CCW_GEN_CONSTRUCT_AT(N)                                                    \
    template <class _Tp _CCW_FNCTMPL_CM_TMPLARG(N)>                                    \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* construct_at(_Tp* __p _CCW_FNCTMPL_CM_DECLARG(N)) { return ::new (static_cast<void*>(__p)) _Tp(_CCW_FNCTMPL_FNCARG(N)); }
    _CCW_GEN_CONSTRUCT_AT(1) _CCW_GEN_CONSTRUCT_AT(2) _CCW_GEN_CONSTRUCT_AT(3) _CCW_GEN_CONSTRUCT_AT(4)
#  undef _CCW_GEN_CONSTRUCT_AT
#else
template <class _Tp, class... _Args>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp* construct_at(_Tp* __p, _Args&&... __a) { return ::new (static_cast<void*>(__p)) _Tp(static_cast<_Args&&>(__a)...); }
#endif
#endif // !_CCW_LIBCPP_NATIVE_STD

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
