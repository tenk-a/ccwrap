/*
 * ccwrap: libc++ layout. classic C++03 body.
 * unreachable_sentinel_t / unreachable_sentinel (C++20): a sentinel that compares
 * unequal to every iterator, i.e. an "infinite" end. The comparison operators are
 * free templates (not in-class friends: Open Watcom rejects a template friend defined
 * inside the class body with E296 "friend declarations may only be declared in a
 * class"); they need no private access, so free functions are equivalent.
 */
#ifndef _CCW_LIBCPP___ITERATOR_UNREACHABLE_SENTINEL_H
#define _CCW_LIBCPP___ITERATOR_UNREACHABLE_SENTINEL_H
#include "../__config"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct unreachable_sentinel_t {};
_ccw_inline_const unreachable_sentinel_t unreachable_sentinel = unreachable_sentinel_t();

template <class _Iter>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(unreachable_sentinel_t, const _Iter&) { return false; }
template <class _Iter>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const _Iter&, unreachable_sentinel_t) { return false; }
template <class _Iter>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(unreachable_sentinel_t, const _Iter&) { return true; }
template <class _Iter>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const _Iter&, unreachable_sentinel_t) { return true; }

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___ITERATOR_UNREACHABLE_SENTINEL_H
