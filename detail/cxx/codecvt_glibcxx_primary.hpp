/**
 *  @file   detail/cxx/codecvt_glibcxx_primary.hpp
 *  @brief  Bodies for libstdc++'s primary codecvt<I, E, S> virtuals (C++03 <codecvt>).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_CODECVT_GLIBCXX_PRIMARY_HPP
#define _CCW_DETAIL_CODECVT_GLIBCXX_PRIMARY_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if defined(__GLIBCXX__)

#include <locale>
#include <cstddef>

namespace std {

template <class _ICw, class _ECw, class _SCw>
codecvt_base::result
codecvt<_ICw, _ECw, _SCw>::do_out(_SCw&, const _ICw* __frm, const _ICw*,
                                  const _ICw*& __frm_nxt, _ECw* __to, _ECw*,
                                  _ECw*& __to_nxt) const
{ __frm_nxt = __frm; __to_nxt = __to; return codecvt_base::noconv; }

template <class _ICw, class _ECw, class _SCw>
codecvt_base::result
codecvt<_ICw, _ECw, _SCw>::do_in(_SCw&, const _ECw* __frm, const _ECw*,
                                 const _ECw*& __frm_nxt, _ICw* __to, _ICw*,
                                 _ICw*& __to_nxt) const
{ __frm_nxt = __frm; __to_nxt = __to; return codecvt_base::noconv; }

template <class _ICw, class _ECw, class _SCw>
codecvt_base::result
codecvt<_ICw, _ECw, _SCw>::do_unshift(_SCw&, _ECw* __to, _ECw*, _ECw*& __to_nxt) const
{ __to_nxt = __to; return codecvt_base::noconv; }

template <class _ICw, class _ECw, class _SCw>
int codecvt<_ICw, _ECw, _SCw>::do_encoding() const throw()
{ return 1; }

template <class _ICw, class _ECw, class _SCw>
bool codecvt<_ICw, _ECw, _SCw>::do_always_noconv() const throw()
{ return true; }

template <class _ICw, class _ECw, class _SCw>
int codecvt<_ICw, _ECw, _SCw>::do_length(_SCw&, const _ECw* __frm, const _ECw* __end,
                                         _CCW_STD::size_t __mx) const
{
    _CCW_STD::size_t __n = (_CCW_STD::size_t)(__end - __frm);
    return (int)(__n < __mx ? __n : __mx);
}

template <class _ICw, class _ECw, class _SCw>
int codecvt<_ICw, _ECw, _SCw>::do_max_length() const throw()
{ return 1; }

}   // namespace std

#endif  // defined(__GLIBCXX__)
#endif  // _CCW_DETAIL_CODECVT_GLIBCXX_PRIMARY_HPP
