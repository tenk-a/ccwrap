//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_ALLOCATOR_H
#define _CCW_LIBCPP___MEMORY_ALLOCATOR_H
#include "../__config"
#include "../__memory/addressof.h"
#include "../__utility/move.h"
#include <cstddef>
#include <new>
#if _CCW_LIBCPP_NATIVE_STD
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
class _CCW_LIBCPP_TEMPLATE_VIS allocator {
public:
    typedef _Tp             value_type;
    typedef _Tp*            pointer;
    typedef const _Tp*      const_pointer;
    typedef _Tp&            reference;
    typedef const _Tp&      const_reference;
    typedef _CCW_STD::size_t     size_type;
    typedef _CCW_STD::ptrdiff_t  difference_type;
    template <class _Up> struct rebind { typedef allocator<_Up> other; };

    _CCW_LIBCPP_HIDE_FROM_ABI allocator() {}
    template <class _Up> _CCW_LIBCPP_HIDE_FROM_ABI allocator(const allocator<_Up>&) {}

    _CCW_LIBCPP_HIDE_FROM_ABI pointer       address(reference __x) const       { return _CCW_STD::addressof(__x); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_pointer address(const_reference __x) const { return _CCW_STD::addressof(__x); }

    _CCW_LIBCPP_HIDE_FROM_ABI pointer allocate(size_type __n, const void* = 0) {
#if defined(__WATCOMC__)
        if (__n > max_size()) _CCW_STD::__ccw_throw_bad_array_new_length();
#else
        if (__n > max_size()) _CCW_THROW(_CCW_STD::bad_array_new_length());
#endif
        return static_cast<pointer>(::operator new(__n * sizeof(_Tp)));
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void deallocate(pointer __p, size_type) {
        ::operator delete(static_cast<void*>(__p));
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void construct(pointer __p, const _Tp& __v) { ::new (static_cast<void*>(__p)) _Tp(__v); }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI void construct(pointer __p, _CCW_RV_REF(_Tp) __v) { ::new (static_cast<void*>(__p)) _Tp(__v); }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI void destroy(pointer __p) { __p->~_Tp(); }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type max_size() const { return size_type(-1) / sizeof(_Tp); }
};

template <class _Tp, class _Up>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const allocator<_Tp>&, const allocator<_Up>&) { return true; }
template <class _Tp, class _Up>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const allocator<_Tp>&, const allocator<_Up>&) { return false; }

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
