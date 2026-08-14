/*
// ccwrap: libc++ layout, classic C++03 body.
//
// _CCW_STD::boyer_moore_searcher / _CCW_STD::boyer_moore_horspool_searcher (C++17):
// wrap a needle [first,last) and, when called with a haystack [f,l),
// return the subrange where the needle first occurs (or {l,l}).
//
// Shortcut (cost vs. fidelity): both searchers use only the *bad-character* skip,
// computed by scanning the needle on each mismatch (rather than a precomputed hash
// table). This keeps them usable for any equality-comparable value type without
// requiring the Hash to be usable as a key, and needs no unordered container. The
// Hash template parameter is accepted (for signature compatibility) but not used to
// build a table. Results are identical to the standard; only the constant factor of
// the skip lookup differs. boyer_moore does not add the good-suffix rule.
//
// Indices use _CCW_STD::ptrdiff_t (random-access iterators have ptrdiff_t difference),
// which also sidesteps an Open Watcom quirk where a local `typename T::type` typedef
// inside a member function template is rejected (E912).
*/
#ifndef _CCW_LIBCPP___FUNCTIONAL_BOYER_MOORE_SEARCHER_H
#define _CCW_LIBCPP___FUNCTIONAL_BOYER_MOORE_SEARCHER_H
#include "../__config"
#include <cstddef>
#include "../__functional/hash.h"
#include "../__functional/operations.h"
#include "../__iterator/advance.h"
#include "../__iterator/distance.h"
#include "../__iterator/iterator_traits.h"
#include "../__utility/pair.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _CCW_SEARCHER_EQ_DEFINED   // shared by both searcher headers
#define _CCW_SEARCHER_EQ_DEFINED
struct __ccw_searcher_eq {
    template <class _T1, class _T2> bool operator()(const _T1& __x, const _T2& __y) const { return __x == __y; }
};
#endif

struct __bm_default_hash {};

template <class _RandomAccessIterator1,
          class _Hash = __bm_default_hash,
          class _BinaryPredicate = __ccw_searcher_eq >
class _CCW_LIBCPP_TEMPLATE_VIS boyer_moore_searcher {
public:
    _CCW_LIBCPP_HIDE_FROM_ABI
    boyer_moore_searcher(_RandomAccessIterator1 __f, _RandomAccessIterator1 __l,
                         _Hash __h = _Hash(), _BinaryPredicate __p = _BinaryPredicate())
        : __first_(__f), __last_(__l), __hash_(__h), __pred_(__p) {}

    template <class _RandomAccessIterator2>
    _CCW_LIBCPP_HIDE_FROM_ABI pair<_RandomAccessIterator2, _RandomAccessIterator2>
    operator()(_RandomAccessIterator2 __f, _RandomAccessIterator2 __l) const {
        _CCW_STD::ptrdiff_t __m = _CCW_STD::distance(__first_, __last_);
        _CCW_STD::ptrdiff_t __n = _CCW_STD::distance(__f, __l);
        if (__m == 0)
            return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__f, __f);
        if (__n < __m)
            return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__l, __l);
        _CCW_STD::ptrdiff_t __pos = 0;
        while (__pos + __m <= __n) {
            _CCW_STD::ptrdiff_t __i = __m - 1;
            while (__i >= 0 && __pred_(*(__first_ + __i), *(__f + (__pos + __i))))
                --__i;
            if (__i < 0) {
                _RandomAccessIterator2 __b = __f; _CCW_STD::advance(__b, __pos);
                _RandomAccessIterator2 __e = __b; _CCW_STD::advance(__e, __m);
                return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__b, __e);
            }
            __pos += __bad_char_skip(*(__f + (__pos + __m - 1)), __m);
        }
        return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__l, __l);
    }

private:
    template <class _Elem>
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::ptrdiff_t
    __bad_char_skip(const _Elem& __c, _CCW_STD::ptrdiff_t __m) const {
        for (_CCW_STD::ptrdiff_t __k = __m - 2; __k >= 0; --__k)
            if (__pred_(*(__first_ + __k), __c))
                return (__m - 1) - __k;
        return __m;
    }
    _RandomAccessIterator1 __first_;
    _RandomAccessIterator1 __last_;
    _Hash                  __hash_;
    _BinaryPredicate       __pred_;
};

template <class _RandomAccessIterator1,
          class _Hash = __bm_default_hash,
          class _BinaryPredicate = __ccw_searcher_eq >
class _CCW_LIBCPP_TEMPLATE_VIS boyer_moore_horspool_searcher {
public:
    _CCW_LIBCPP_HIDE_FROM_ABI
    boyer_moore_horspool_searcher(_RandomAccessIterator1 __f, _RandomAccessIterator1 __l,
                                  _Hash __h = _Hash(), _BinaryPredicate __p = _BinaryPredicate())
        : __first_(__f), __last_(__l), __hash_(__h), __pred_(__p) {}

    template <class _RandomAccessIterator2>
    _CCW_LIBCPP_HIDE_FROM_ABI pair<_RandomAccessIterator2, _RandomAccessIterator2>
    operator()(_RandomAccessIterator2 __f, _RandomAccessIterator2 __l) const {
        _CCW_STD::ptrdiff_t __m = _CCW_STD::distance(__first_, __last_);
        _CCW_STD::ptrdiff_t __n = _CCW_STD::distance(__f, __l);
        if (__m == 0)
            return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__f, __f);
        if (__n < __m)
            return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__l, __l);
        _CCW_STD::ptrdiff_t __pos = 0;
        while (__pos + __m <= __n) {
            _CCW_STD::ptrdiff_t __i = __m - 1;
            while (__i >= 0 && __pred_(*(__first_ + __i), *(__f + (__pos + __i))))
                --__i;
            if (__i < 0) {
                _RandomAccessIterator2 __b = __f; _CCW_STD::advance(__b, __pos);
                _RandomAccessIterator2 __e = __b; _CCW_STD::advance(__e, __m);
                return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__b, __e);
            }
            __pos += __bad_char_skip(*(__f + (__pos + __m - 1)), __m);
        }
        return pair<_RandomAccessIterator2, _RandomAccessIterator2>(__l, __l);
    }

private:
    template <class _Elem>
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::ptrdiff_t
    __bad_char_skip(const _Elem& __c, _CCW_STD::ptrdiff_t __m) const {
        for (_CCW_STD::ptrdiff_t __k = __m - 2; __k >= 0; --__k)
            if (__pred_(*(__first_ + __k), __c))
                return (__m - 1) - __k;
        return __m;
    }
    _RandomAccessIterator1 __first_;
    _RandomAccessIterator1 __last_;
    _Hash                  __hash_;
    _BinaryPredicate       __pred_;
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___FUNCTIONAL_BOYER_MOORE_SEARCHER_H
