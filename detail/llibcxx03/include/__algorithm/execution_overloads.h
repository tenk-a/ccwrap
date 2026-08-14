//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  The ExecutionPolicy overloads of the <algorithm> algorithms.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_EXECUTION_OVERLOADS_H
#define _CCW_LIBCPP___ALGORITHM_EXECUTION_OVERLOADS_H
#include "../__config"
#include "../__iterator/advance.h"
#include "../__iterator/iterator_traits.h"
#include <execution>
#include <utility>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#define _CCW_EXEC_OVERLOADS(_Pol)                                              \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                      \
    all_of(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::all_of(__f, __l, __p); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                      \
    any_of(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::any_of(__f, __l, __p); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                      \
    none_of(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::none_of(__f, __l, __p); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                      \
    is_partitioned(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::is_partitioned(__f, __l, __p); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                      \
    is_sorted(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::is_sorted(__f, __l); }                   \
    template <class _It, class _Cp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                      \
    is_sorted(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Cp __c) { return _CCW_STD::is_sorted(__f, __l, __c); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    is_sorted_until(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::is_sorted_until(__f, __l); }                   \
    template <class _It, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    find(const _CCW_STD::execution::_Pol&, _It __f, _It __l, const _Tp& __v) { return _CCW_STD::find(__f, __l, __v); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    find_if(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::find_if(__f, __l, __p); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    find_if_not(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::find_if_not(__f, __l, __p); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    adjacent_find(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::adjacent_find(__f, __l); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    min_element(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::min_element(__f, __l); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    max_element(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::max_element(__f, __l); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI pair<_It, _It>                                      \
    minmax_element(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::minmax_element(__f, __l); }                   \
    template <class _I1, class _I2>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _I1                                      \
    search(const _CCW_STD::execution::_Pol&, _I1 __f, _I1 __l, _I2 __sf, _I2 __sl) { return _CCW_STD::search(__f, __l, __sf, __sl); }                   \
    template <class _It, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_It>::difference_type                                      \
    count(const _CCW_STD::execution::_Pol&, _It __f, _It __l, const _Tp& __v) { return _CCW_STD::count(__f, __l, __v); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_It>::difference_type                                      \
    count_if(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::count_if(__f, __l, __p); }                   \
    template <class _I1, class _I2>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                      \
    equal(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2) { return _CCW_STD::equal(__f1, __l1, __f2); }                   \
    template <class _It, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    copy(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __o) { return _CCW_STD::copy(__f, __l, __o); }                   \
    template <class _It, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    move(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __o) { return _CCW_STD::move(__f, __l, __o); }                   \
    template <class _It, class _Ot, class _Op>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    transform(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __o, _Op __op) { return _CCW_STD::transform(__f, __l, __o, __op); }                   \
    template <class _I1, class _I2, class _Ot, class _Op>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    transform(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _Ot __o, _Op __op) { return _CCW_STD::transform(__f1, __l1, __f2, __o, __op); }                   \
    template <class _It, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    remove(const _CCW_STD::execution::_Pol&, _It __f, _It __l, const _Tp& __v) { return _CCW_STD::remove(__f, __l, __v); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    remove_if(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::remove_if(__f, __l, __p); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    unique(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::unique(__f, __l); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    partition(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::partition(__f, __l, __p); }                   \
    template <class _It, class _Pr>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _It                                      \
    stable_partition(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p) { return _CCW_STD::stable_partition(__f, __l, __p); }                   \
    template <class _I1, class _I2, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    merge(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2, _Ot __o) { return _CCW_STD::merge(__f1, __l1, __f2, __l2, __o); }                   \
    template <class _I1, class _I2, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    set_union(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2, _Ot __o) { return _CCW_STD::set_union(__f1, __l1, __f2, __l2, __o); }                   \
    template <class _I1, class _I2, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    set_intersection(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2, _Ot __o) { return _CCW_STD::set_intersection(__f1, __l1, __f2, __l2, __o); }                   \
    template <class _I1, class _I2, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    set_difference(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2, _Ot __o) { return _CCW_STD::set_difference(__f1, __l1, __f2, __l2, __o); }                   \
    template <class _It, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    fill(const _CCW_STD::execution::_Pol&, _It __f, _It __l, const _Tp& __v) { _CCW_STD::fill(__f, __l, __v); }                   \
    template <class _It, class _Sz, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    fill_n(const _CCW_STD::execution::_Pol&, _It __f, _Sz __n, const _Tp& __v) { _CCW_STD::fill_n(__f, __n, __v); }                   \
    template <class _It, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    replace(const _CCW_STD::execution::_Pol&, _It __f, _It __l, const _Tp& __o, const _Tp& __n) { _CCW_STD::replace(__f, __l, __o, __n); }                   \
    template <class _It, class _Pr, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    replace_if(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Pr __p, const _Tp& __n) { _CCW_STD::replace_if(__f, __l, __p, __n); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    reverse(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { _CCW_STD::reverse(__f, __l); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    sort(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { _CCW_STD::sort(__f, __l); }                   \
    template <class _It, class _Cp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    sort(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Cp __c) { _CCW_STD::sort(__f, __l, __c); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    stable_sort(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { _CCW_STD::stable_sort(__f, __l); }                   \
    template <class _It, class _Cp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    stable_sort(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Cp __c) { _CCW_STD::stable_sort(__f, __l, __c); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    partial_sort(const _CCW_STD::execution::_Pol&, _It __f, _It __m, _It __l) { _CCW_STD::partial_sort(__f, __m, __l); }                   \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    nth_element(const _CCW_STD::execution::_Pol&, _It __f, _It __m, _It __l) { _CCW_STD::nth_element(__f, __m, __l); }                   \
    template <class _It, class _Fn>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    for_each(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Fn __fn) { _CCW_STD::for_each(__f, __l, __fn); }                   \
    template <class _It, class _Sz, class _Fn>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void                                      \
    for_each_n(const _CCW_STD::execution::_Pol&, _It __f, _Sz __n, _Fn __fn) { _CCW_STD::for_each_n(__f, __n, __fn); }                   \
    template <class _It, class _Out, class _Pr>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    copy_if(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Out __o, _Pr __p) { return _CCW_STD::copy_if(__f, __l, __o, __p); }   \
    template <class _It, class _Sz, class _Out>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    copy_n(const _CCW_STD::execution::_Pol&, _It __f, _Sz __n, _Out __o) { return _CCW_STD::copy_n(__f, __n, __o); }   \
    template <class _It, class _Out>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    reverse_copy(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Out __o) { return _CCW_STD::reverse_copy(__f, __l, __o); }   \
    template <class _It>   \
    _CCW_LIBCPP_HIDE_FROM_ABI void   \
    rotate(const _CCW_STD::execution::_Pol&, _It __f, _It __m, _It __l) { _CCW_STD::rotate(__f, __m, __l); }   \
    template <class _It, class _Out>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    rotate_copy(const _CCW_STD::execution::_Pol&, _It __f, _It __m, _It __l, _Out __o) { return _CCW_STD::rotate_copy(__f, __m, __l, __o); }   \
    template <class _It, class _Out>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    unique_copy(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Out __o) { return _CCW_STD::unique_copy(__f, __l, __o); }   \
    template <class _I1, class _I2, class _Out>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    set_symmetric_difference(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2, _Out __o) { return _CCW_STD::set_symmetric_difference(__f1, __l1, __f2, __l2, __o); }   \
    template <class _I1, class _I2>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _I2   \
    swap_ranges(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2) { return _CCW_STD::swap_ranges(__f1, __l1, __f2); }   \
    template <class _It, class _Gn>   \
    _CCW_LIBCPP_HIDE_FROM_ABI void   \
    generate(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Gn __g) { _CCW_STD::generate(__f, __l, __g); }   \
    template <class _It, class _Sz, class _Gn>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _It   \
    generate_n(const _CCW_STD::execution::_Pol&, _It __f, _Sz __n, _Gn __g) { _CCW_STD::generate_n(__f, __n, __g); _CCW_STD::advance(__f, __n); return __f; }   \
    template <class _I1, class _I2>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _I1   \
    find_end(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2) { return _CCW_STD::find_end(__f1, __l1, __f2, __l2); }   \
    template <class _I1, class _I2>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _I1   \
    find_first_of(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2) { return _CCW_STD::find_first_of(__f1, __l1, __f2, __l2); }   \
    template <class _It, class _Sz, class _Tp>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _It   \
    search_n(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Sz __n, const _Tp& __v) { return _CCW_STD::search_n(__f, __l, __n, __v); }   \
    template <class _I1, class _I2>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::pair<_I1, _I2>   \
    mismatch(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2) { return _CCW_STD::mismatch(__f1, __l1, __f2); }   \
    template <class _I1, class _I2>   \
    _CCW_LIBCPP_HIDE_FROM_ABI bool   \
    lexicographical_compare(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2) { return _CCW_STD::lexicographical_compare(__f1, __l1, __f2, __l2); }   \
    template <class _I1, class _I2>   \
    _CCW_LIBCPP_HIDE_FROM_ABI bool   \
    includes(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2) { return _CCW_STD::includes(__f1, __l1, __f2, __l2); }   \
    template <class _It>   \
    _CCW_LIBCPP_HIDE_FROM_ABI bool   \
    is_heap(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::is_heap(__f, __l); }   \
    template <class _It>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _It   \
    is_heap_until(const _CCW_STD::execution::_Pol&, _It __f, _It __l) { return _CCW_STD::is_heap_until(__f, __l); }   \
    template <class _It>   \
    _CCW_LIBCPP_HIDE_FROM_ABI void   \
    inplace_merge(const _CCW_STD::execution::_Pol&, _It __f, _It __m, _It __l) { _CCW_STD::inplace_merge(__f, __m, __l); }   \
    template <class _I1, class _I2>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _I2   \
    partial_sort_copy(const _CCW_STD::execution::_Pol&, _I1 __f, _I1 __l, _I2 __df, _I2 __dl) { return _CCW_STD::partial_sort_copy(__f, __l, __df, __dl); }   \
    template <class _It, class _O1, class _O2, class _Pr>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::pair<_O1, _O2>   \
    partition_copy(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _O1 __ot, _O2 __of, _Pr __p) { return _CCW_STD::partition_copy(__f, __l, __ot, __of, __p); }   \
    template <class _It, class _Out, class _Tp>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    remove_copy(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Out __o, const _Tp& __v) { return _CCW_STD::remove_copy(__f, __l, __o, __v); }   \
    template <class _It, class _Out, class _Pr>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    remove_copy_if(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Out __o, _Pr __p) { return _CCW_STD::remove_copy_if(__f, __l, __o, __p); }   \
    template <class _It, class _Out, class _Tp>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    replace_copy(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Out __o, const _Tp& __ov, const _Tp& __nv) { return _CCW_STD::replace_copy(__f, __l, __o, __ov, __nv); }   \
    template <class _It, class _Out, class _Pr, class _Tp>   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Out   \
    replace_copy_if(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Out __o, _Pr __p, const _Tp& __nv) { return _CCW_STD::replace_copy_if(__f, __l, __o, __p, __nv); }   \
    template <class _I1, class _I2>                                             \
    _CCW_LIBCPP_HIDE_FROM_ABI bool                                             \
    equal(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _I2 __l2) {   \
        for (; __f1 != __l1 && __f2 != __l2; ++__f1, ++__f2)                   \
            if (!(*__f1 == *__f2)) return false;                               \
        return __f1 == __l1 && __f2 == __l2;                                   \
    }

_CCW_EXEC_OVERLOADS(sequenced_policy)
_CCW_EXEC_OVERLOADS(parallel_policy)
_CCW_EXEC_OVERLOADS(parallel_unsequenced_policy)
_CCW_EXEC_OVERLOADS(unsequenced_policy)

#undef _CCW_EXEC_OVERLOADS

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
