//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FUNCTIONAL_DEFAULT_SEARCHER_H
#define _CCW_LIBCPP___FUNCTIONAL_DEFAULT_SEARCHER_H
#include "../__config"
#include "../__algorithm/search.h"
#include "../__functional/operations.h"
#include "../__iterator/advance.h"
#include "../__iterator/distance.h"
#include "../__utility/pair.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _CCW_SEARCHER_EQ_DEFINED   // shared by both searcher headers
#define _CCW_SEARCHER_EQ_DEFINED
struct __ccw_searcher_eq {
    template <class _T1, class _T2> bool operator()(const _T1& __x, const _T2& __y) const { return __x == __y; }
};
#endif

template <class _ForwardIterator, class _BinaryPredicate = __ccw_searcher_eq >
class _CCW_LIBCPP_TEMPLATE_VIS default_searcher {
public:
    _CCW_LIBCPP_HIDE_FROM_ABI
    default_searcher(_ForwardIterator __f, _ForwardIterator __l,
                     _BinaryPredicate __p = _BinaryPredicate())
        : __first_(__f), __last_(__l), __pred_(__p) {}

    template <class _ForwardIterator2>
    _CCW_LIBCPP_HIDE_FROM_ABI pair<_ForwardIterator2, _ForwardIterator2>
    operator()(_ForwardIterator2 __f, _ForwardIterator2 __l) const {
        _ForwardIterator2 __r = _CCW_STD::search(__f, __l, __first_, __last_, __pred_);
        if (__r == __l)
            return pair<_ForwardIterator2, _ForwardIterator2>(__l, __l);
        _ForwardIterator2 __e = __r;
        _CCW_STD::advance(__e, _CCW_STD::distance(__first_, __last_));
        return pair<_ForwardIterator2, _ForwardIterator2>(__r, __e);
    }

private:
    _ForwardIterator __first_;
    _ForwardIterator __last_;
    _BinaryPredicate __pred_;
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___FUNCTIONAL_DEFAULT_SEARCHER_H
