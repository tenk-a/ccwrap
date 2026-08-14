//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_AUTO_PTR_H
#define _CCW_LIBCPP___MEMORY_AUTO_PTR_H
#include "../__config"
#if _CCW_LIBCPP_NATIVE_STD
#  include <memory>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Tp>
class _CCW_LIBCPP_TEMPLATE_VIS auto_ptr {
    _Tp* __ptr_;
public:
    typedef _Tp element_type;
    _CCW_LIBCPP_HIDE_FROM_ABI explicit auto_ptr(_Tp* __p = 0) : __ptr_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI auto_ptr(auto_ptr& __a) : __ptr_(__a.release()) {}
    template <class _Up> _CCW_LIBCPP_HIDE_FROM_ABI auto_ptr(auto_ptr<_Up>& __a) : __ptr_(__a.release()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI auto_ptr& operator=(auto_ptr& __a) { reset(__a.release()); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI ~auto_ptr() { delete __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp& operator*() const  { return *__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator->() const { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* get() const        { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* release()          { _Tp* __t = __ptr_; __ptr_ = 0; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI void reset(_Tp* __p = 0) { if (__ptr_ != __p) { delete __ptr_; __ptr_ = __p; } }
};
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
