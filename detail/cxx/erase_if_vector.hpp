/**
 *  @file   detail/cxx/erase_if_vector.hpp
 *  @brief  std::erase / std::erase_if for <vector> (C++20, [vector.erasure]).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_ERASE_IF_VECTOR_HPP
#define _CCW_DETAIL_ERASE_IF_VECTOR_HPP
#include <ccwrap_common.h>

#if defined(__cplusplus) && !defined(__cpp_lib_erase_if)
#include <algorithm>
#include <vector>

namespace _CCW_STD20 {

template <class _Tp, class _Alloc, class _Up>
inline typename std::vector<_Tp, _Alloc>::size_type erase(std::vector<_Tp, _Alloc>& __c, const _Up& __v) {
    typename std::vector<_Tp, _Alloc>::iterator __it = std::remove(__c.begin(), __c.end(), __v);
    typename std::vector<_Tp, _Alloc>::size_type __n =
        static_cast<typename std::vector<_Tp, _Alloc>::size_type>(__c.end() - __it);
    __c.erase(__it, __c.end());
    return __n;
}

template <class _Tp, class _Alloc, class _Pred>
inline typename std::vector<_Tp, _Alloc>::size_type erase_if(std::vector<_Tp, _Alloc>& __c, _Pred __p) {
    typename std::vector<_Tp, _Alloc>::iterator __it = std::remove_if(__c.begin(), __c.end(), __p);
    typename std::vector<_Tp, _Alloc>::size_type __n =
        static_cast<typename std::vector<_Tp, _Alloc>::size_type>(__c.end() - __it);
    __c.erase(__it, __c.end());
    return __n;
}

}   // _CCW_STD20
#endif

#endif  /* _CCW_DETAIL_ERASE_IF_VECTOR_HPP */
