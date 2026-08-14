/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ITERATOR_ACCESS_H
#define _CCW_LIBCPP___ITERATOR_ACCESS_H
#include "../__config"
#include <cstddef>
#if _CCW_LIBCPP_NATIVE_STD
#  include <iterator>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Cp> struct __ccw_range_iter          { typedef typename _Cp::iterator       type; };
template <class _Cp> struct __ccw_range_iter<const _Cp> { typedef typename _Cp::const_iterator type; };
template <class _Cp> struct __ccw_range_riter          { typedef typename _Cp::reverse_iterator       type; };
template <class _Cp> struct __ccw_range_riter<const _Cp> { typedef typename _Cp::const_reverse_iterator type; };

template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_range_iter<_Cp>::type begin(_Cp& __c) { return __c.begin(); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_range_iter<_Cp>::type end(_Cp& __c)   { return __c.end(); }

template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename _Cp::const_iterator         cbegin(const _Cp& __c) { return __c.begin(); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename _Cp::const_iterator         cend(const _Cp& __c)   { return __c.end(); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_range_riter<_Cp>::type rbegin(_Cp& __c) { return __c.rbegin(); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_range_riter<_Cp>::type rend(_Cp& __c)   { return __c.rend(); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename _Cp::const_reverse_iterator crbegin(const _Cp& __c) { return __c.rbegin(); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename _Cp::const_reverse_iterator crend(const _Cp& __c)   { return __c.rend(); }

template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename _Cp::size_type size(const _Cp& __c)  { return __c.size(); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::ptrdiff_t         ssize(const _Cp& __c) { return static_cast<_CCW_STD::ptrdiff_t>(__c.size()); }
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI bool                  empty(const _Cp& __c) { return __c.empty(); }

template <class _Cp> struct __ccw_range_data { typedef typename _Cp::value_type*       type; };
template <class _Cp> struct __ccw_range_data<const _Cp> { typedef const typename _Cp::value_type* type; };
template <class _Cp> _CCW_LIBCPP_HIDE_FROM_ABI typename __ccw_range_data<_Cp>::type data(_Cp& __c) { return __c.data(); }

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
