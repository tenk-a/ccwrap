/**
 *  @file   detail/mdspan.hpp
 *  @brief  <mdspan> (C++23) for compilers that have C++11/14 but no native <mdspan>.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MDSPAN_HPP
#define _CCW_DETAIL_MDSPAN_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "eh_fail.hpp"

#if !defined(__cpp_lib_mdspan) && \
    ((defined(_MSC_VER) && _MSC_VER >= 1900) || \
     (!defined(_MSC_VER) && !defined(__WATCOMC__) && _ccw_cplusplus >= 201402L))

#include <cstddef>
#include <array>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <span>          // dynamic_extent

#define _CCW_HAS_MDSPAN 1

namespace _CCW_STD23 {

namespace __ccw_md {

using _CCW_STD::size_t;

inline constexpr size_t __dyn() { return _CCW_STD20::dynamic_extent; }

constexpr size_t __count_dynamic(const size_t* __se, size_t __n) {
    return __n == 0 ? (size_t)0
         : (__se[__n - 1] == __dyn() ? (size_t)1 : (size_t)0) + __count_dynamic(__se, __n - 1);
}

constexpr size_t __dynamic_index(const size_t* __se, size_t __r) {
    return __count_dynamic(__se, __r);
}

}   // namespace __ccw_md

template <class _IndexType, _CCW_STD::size_t... _Extents>
class extents {
    typedef _CCW_STD::size_t __sz;

    static constexpr __sz __se_[sizeof...(_Extents) + 1] = { _Extents..., 0 };

public:
    typedef _IndexType                                          index_type;
    typedef typename _CCW_STD::make_unsigned<_IndexType>::type  size_type;
    typedef __sz                                                rank_type;

    static constexpr rank_type rank() { return sizeof...(_Extents); }
    static constexpr rank_type rank_dynamic() { return __ccw_md::__count_dynamic(__se_, rank()); }
    static constexpr __sz static_extent(rank_type __r) { return __se_[__r]; }

    constexpr extents() : __dyn_() {}

    template <class... _OtherIndexTypes>
    _ccw_constexpr14 explicit extents(_OtherIndexTypes... __exts) : __dyn_() {
        __fill_pack(0, sizeof...(_OtherIndexTypes), static_cast<index_type>(__exts)...);
    }

    template <class _OtherIndexType, __sz _Np>
    _ccw_constexpr14 extents(const _CCW_STD::array<_OtherIndexType, _Np>& __exts) : __dyn_() {
        for (__sz __i = 0; __i < _Np; ++__i) __put(__i, _Np, static_cast<index_type>(__exts[__i]));
    }

    template <class _OtherIndexType, __sz... _OtherExtents>
    _ccw_constexpr14 extents(const extents<_OtherIndexType, _OtherExtents...>& __other) : __dyn_() {
        for (rank_type __r = 0; __r < rank(); ++__r)
            if (__se_[__r] == __ccw_md::__dyn())
                __dyn_[__ccw_md::__dynamic_index(__se_, __r)] = static_cast<index_type>(__other.extent(__r));
    }

    constexpr index_type extent(rank_type __r) const {
        return __se_[__r] == __ccw_md::__dyn()
                   ? __dyn_[__ccw_md::__dynamic_index(__se_, __r)]
                   : static_cast<index_type>(__se_[__r]);
    }

private:
    _ccw_constexpr14 void __put(__sz __i, __sz __n, index_type __v) {
        if (__n == rank_dynamic()) {
            __dyn_[__i] = __v;
        } else if (__se_[__i] == __ccw_md::__dyn()) {
            __dyn_[__ccw_md::__dynamic_index(__se_, __i)] = __v;
        }
    }
    _ccw_constexpr14 void __fill_pack(__sz, __sz) {}
    template <class... _Rest>
    _ccw_constexpr14 void __fill_pack(__sz __i, __sz __n, index_type __v, _Rest... __rest) {
        __put(__i, __n, __v);
        __fill_pack(__i + 1, __n, __rest...);
    }

    _CCW_STD::array<index_type, __ccw_md::__count_dynamic(__se_, sizeof...(_Extents))> __dyn_;
};

template <class _IndexType, _CCW_STD::size_t... _Extents>
constexpr _CCW_STD::size_t extents<_IndexType, _Extents...>::__se_[sizeof...(_Extents) + 1];

template <class _IndexType, _CCW_STD::size_t... _E1, class _OtherIndexType, _CCW_STD::size_t... _E2>
_ccw_constexpr14 bool operator==(const extents<_IndexType, _E1...>& __a, const extents<_OtherIndexType, _E2...>& __b) {
    if (sizeof...(_E1) != sizeof...(_E2)) return false;
    for (_CCW_STD::size_t __r = 0; __r < sizeof...(_E1); ++__r)
        if (static_cast<_CCW_STD::size_t>(__a.extent(__r)) != static_cast<_CCW_STD::size_t>(__b.extent(__r)))
            return false;
    return true;
}
template <class _IndexType, _CCW_STD::size_t... _E1, class _OtherIndexType, _CCW_STD::size_t... _E2>
constexpr bool operator!=(const extents<_IndexType, _E1...>& __a, const extents<_OtherIndexType, _E2...>& __b) {
    return !(__a == __b);
}

namespace __ccw_md {
template <class _IndexType, _CCW_STD::size_t _Rank, _CCW_STD::size_t... _Es>
struct __dext { typedef typename __dext<_IndexType, _Rank - 1, _CCW_STD20::dynamic_extent, _Es...>::type type; };
template <class _IndexType, _CCW_STD::size_t... _Es>
struct __dext<_IndexType, 0, _Es...> { typedef extents<_IndexType, _Es...> type; };
}
template <class _IndexType, _CCW_STD::size_t _Rank>
using dextents = typename __ccw_md::__dext<_IndexType, _Rank>::type;

template <_CCW_STD::size_t _Rank, class _IndexType = _CCW_STD::size_t>
using dims = dextents<_IndexType, _Rank>;

struct layout_left;
struct layout_right;
struct layout_stride;

struct layout_right {
    template <class _Extents>
    class mapping {
    public:
        typedef _Extents                             extents_type;
        typedef typename extents_type::index_type    index_type;
        typedef typename extents_type::size_type     size_type;
        typedef typename extents_type::rank_type     rank_type;
        typedef layout_right                         layout_type;

        constexpr mapping() : __e_() {}
        constexpr mapping(const extents_type& __e) : __e_(__e) {}
        template <class _OtherExtents>
        constexpr mapping(const mapping<_OtherExtents>& __o) : __e_(__o.extents()) {}

        constexpr const extents_type& extents() const { return __e_; }

        constexpr bool __any_zero(rank_type __r) const {
            return __r == 0 ? false
                 : (__e_.extent(__r - 1) == 0 ? true : __any_zero(__r - 1));
        }
        constexpr index_type __prod_upto(rank_type __r) const {
            return __r == 0 ? (index_type)1 : __prod_upto(__r - 1) * __e_.extent(__r - 1);
        }
        constexpr index_type required_span_size() const {
            return __any_zero(extents_type::rank()) ? (index_type)0
                 : __prod_upto(extents_type::rank());
        }

        constexpr index_type __prod_from(rank_type __i) const {
            return __i >= extents_type::rank() ? (index_type)1
                 : __e_.extent(__i) * __prod_from(__i + 1);
        }
        constexpr index_type stride(rank_type __r) const { return __prod_from(__r + 1); }

        constexpr index_type __dot(rank_type) const { return 0; }
        template <class... _Rest>
        constexpr index_type __dot(rank_type __r, index_type __v, _Rest... __rest) const {
            return __v * stride(__r) + __dot(__r + 1, __rest...);
        }

        template <class... _Indices>
        constexpr index_type operator()(_Indices... __idx) const {
            return __dot(0, static_cast<index_type>(__idx)...);
        }

        static constexpr bool is_always_unique() { return true; }
        static constexpr bool is_always_exhaustive() { return true; }
        static constexpr bool is_always_strided() { return true; }
        static constexpr bool is_unique() { return true; }
        static constexpr bool is_exhaustive() { return true; }
        static constexpr bool is_strided() { return true; }

    private:
        extents_type __e_;
    };
};

struct layout_left {
    template <class _Extents>
    class mapping {
    public:
        typedef _Extents                             extents_type;
        typedef typename extents_type::index_type    index_type;
        typedef typename extents_type::size_type     size_type;
        typedef typename extents_type::rank_type     rank_type;
        typedef layout_left                          layout_type;

        constexpr mapping() : __e_() {}
        constexpr mapping(const extents_type& __e) : __e_(__e) {}
        template <class _OtherExtents>
        constexpr mapping(const mapping<_OtherExtents>& __o) : __e_(__o.extents()) {}

        constexpr const extents_type& extents() const { return __e_; }

        constexpr bool __any_zero(rank_type __r) const {
            return __r == 0 ? false
                 : (__e_.extent(__r - 1) == 0 ? true : __any_zero(__r - 1));
        }
        constexpr index_type __prod_upto(rank_type __r) const {
            return __r == 0 ? (index_type)1 : __prod_upto(__r - 1) * __e_.extent(__r - 1);
        }
        constexpr index_type required_span_size() const {
            return __any_zero(extents_type::rank()) ? (index_type)0
                 : __prod_upto(extents_type::rank());
        }

        constexpr index_type stride(rank_type __r) const { return __prod_upto(__r); }

        constexpr index_type __dot(rank_type) const { return 0; }
        template <class... _Rest>
        constexpr index_type __dot(rank_type __r, index_type __v, _Rest... __rest) const {
            return __v * stride(__r) + __dot(__r + 1, __rest...);
        }

        template <class... _Indices>
        constexpr index_type operator()(_Indices... __idx) const {
            return __dot(0, static_cast<index_type>(__idx)...);
        }

        static constexpr bool is_always_unique() { return true; }
        static constexpr bool is_always_exhaustive() { return true; }
        static constexpr bool is_always_strided() { return true; }
        static constexpr bool is_unique() { return true; }
        static constexpr bool is_exhaustive() { return true; }
        static constexpr bool is_strided() { return true; }

    private:
        extents_type __e_;
    };
};

struct layout_stride {
    template <class _Extents>
    class mapping {
    public:
        typedef _Extents                             extents_type;
        typedef typename extents_type::index_type    index_type;
        typedef typename extents_type::size_type     size_type;
        typedef typename extents_type::rank_type     rank_type;
        typedef layout_stride                        layout_type;

        constexpr mapping() : __e_(), __s_() {}

        template <class _OtherIndexType>
        _ccw_constexpr14 mapping(const extents_type& __e,
                          const _CCW_STD::array<_OtherIndexType, _Extents::rank()>& __s)
            : __e_(__e), __s_() {
            for (rank_type __r = 0; __r < extents_type::rank(); ++__r)
                __s_[__r] = static_cast<index_type>(__s[__r]);
        }

        template <class _StridedMapping>
        _ccw_constexpr14 mapping(const _StridedMapping& __o) : __e_(__o.extents()), __s_() {
            for (rank_type __r = 0; __r < extents_type::rank(); ++__r) __s_[__r] = __o.stride(__r);
        }

        constexpr const extents_type& extents() const { return __e_; }
        constexpr _CCW_STD::array<index_type, _Extents::rank()> strides() const { return __s_; }
        constexpr index_type stride(rank_type __r) const { return __s_[__r]; }

        constexpr bool __any_zero(rank_type __r) const {
            return __r == 0 ? false
                 : (__e_.extent(__r - 1) == 0 ? true : __any_zero(__r - 1));
        }
        constexpr index_type __span_sum(rank_type __r) const {
            return __r == 0 ? (index_type)1
                 : __span_sum(__r - 1) + (__e_.extent(__r - 1) - 1) * __s_[__r - 1];
        }
        constexpr index_type required_span_size() const {
            return __any_zero(extents_type::rank()) ? (index_type)0
                 : __span_sum(extents_type::rank());
        }

        constexpr index_type __dot(rank_type) const { return 0; }
        template <class... _Rest>
        constexpr index_type __dot(rank_type __r, index_type __v, _Rest... __rest) const {
            return __v * __s_[__r] + __dot(__r + 1, __rest...);
        }

        template <class... _Indices>
        constexpr index_type operator()(_Indices... __idx) const {
            return __dot(0, static_cast<index_type>(__idx)...);
        }

        static constexpr bool is_always_unique() { return true; }
        static constexpr bool is_always_exhaustive() { return false; }
        static constexpr bool is_always_strided() { return true; }
        static constexpr bool is_unique() { return true; }
        static constexpr bool is_strided() { return true; }

        constexpr index_type __prod_upto(rank_type __r) const {
            return __r == 0 ? (index_type)1 : __prod_upto(__r - 1) * __e_.extent(__r - 1);
        }
        constexpr bool is_exhaustive() const {
            return __prod_upto(extents_type::rank()) == required_span_size();
        }

    private:
        extents_type __e_;
        _CCW_STD::array<index_type, _Extents::rank()> __s_;
    };
};

template <class _ElementType>
struct default_accessor {
    typedef default_accessor  offset_policy;
    typedef _ElementType      element_type;
    typedef _ElementType&     reference;
    typedef _ElementType*     data_handle_type;

    constexpr default_accessor() {}
    template <class _OtherElementType>
    constexpr default_accessor(default_accessor<_OtherElementType>) {}

    constexpr reference access(data_handle_type __p, _CCW_STD::size_t __i) const { return __p[__i]; }
    constexpr data_handle_type offset(data_handle_type __p, _CCW_STD::size_t __i) const { return __p + __i; }
};

template <class _ElementType,
          class _Extents,
          class _LayoutPolicy   = layout_right,
          class _AccessorPolicy = default_accessor<_ElementType> >
class mdspan {
public:
    typedef _Extents                                              extents_type;
    typedef _LayoutPolicy                                         layout_type;
    typedef _AccessorPolicy                                       accessor_type;
    typedef typename layout_type::template mapping<extents_type>  mapping_type;
    typedef _ElementType                                          element_type;
    typedef typename _CCW_STD::remove_cv<element_type>::type      value_type;
    typedef typename extents_type::index_type                     index_type;
    typedef typename extents_type::size_type                      size_type;
    typedef typename extents_type::rank_type                      rank_type;
    typedef typename accessor_type::data_handle_type              data_handle_type;
    typedef typename accessor_type::reference                     reference;

    static constexpr rank_type rank() { return extents_type::rank(); }
    static constexpr rank_type rank_dynamic() { return extents_type::rank_dynamic(); }
    static constexpr _CCW_STD::size_t static_extent(rank_type __r) { return extents_type::static_extent(__r); }
    constexpr index_type extent(rank_type __r) const { return extents().extent(__r); }

    constexpr mdspan() : __acc_(), __map_(), __ptr_() {}

    template <class... _OtherIndexTypes>
    constexpr explicit mdspan(data_handle_type __p, _OtherIndexTypes... __exts)
        : __acc_(), __map_(extents_type(static_cast<index_type>(__exts)...)), __ptr_(__p) {}

    constexpr mdspan(data_handle_type __p, const extents_type& __e)
        : __acc_(), __map_(__e), __ptr_(__p) {}
    constexpr mdspan(data_handle_type __p, const mapping_type& __m)
        : __acc_(), __map_(__m), __ptr_(__p) {}
    constexpr mdspan(data_handle_type __p, const mapping_type& __m, const accessor_type& __a)
        : __acc_(__a), __map_(__m), __ptr_(__p) {}

    template <class _OtherIndexType, _CCW_STD::size_t _Np>
    constexpr mdspan(data_handle_type __p, const _CCW_STD::array<_OtherIndexType, _Np>& __exts)
        : __acc_(), __map_(extents_type(__exts)), __ptr_(__p) {}

#if defined(_CCW_STD_RELOCATED) || !defined(__cpp_multidimensional_subscript)
    template <class... _OtherIndexTypes>
    constexpr reference operator()(_OtherIndexTypes... __indices) const {
        return __acc_.access(__ptr_, __map_(static_cast<index_type>(__indices)...));
    }
#endif

    template <class _OtherIndexType, _CCW_STD::size_t _Np>
    constexpr reference operator[](const _CCW_STD::array<_OtherIndexType, _Np>& __indices) const {
        return __acc_.access(__ptr_, __offset_of(__indices));
    }

#if defined(__cpp_multidimensional_subscript)
    template <class... _OtherIndexTypes>
    constexpr reference operator[](_OtherIndexTypes... __indices) const {
        return __acc_.access(__ptr_, __map_(static_cast<index_type>(__indices)...));
    }
#endif

    template <class... _OtherIndexTypes>
    constexpr reference at(_OtherIndexTypes... __indices) const {
        __check_pack(0, static_cast<index_type>(__indices)...);
        return __acc_.access(__ptr_, __map_(static_cast<index_type>(__indices)...));
    }

    template <class _OtherIndexType, _CCW_STD::size_t _Np>
    _ccw_constexpr14 reference at(const _CCW_STD::array<_OtherIndexType, _Np>& __indices) const {
        for (rank_type __r = 0; __r < rank(); ++__r)
            __check_one(__r, static_cast<index_type>(__indices[__r]));
        return __acc_.access(__ptr_, __offset_of(__indices));
    }

    constexpr size_type __size_upto(rank_type __r) const {
        return __r == 0 ? (size_type)1
             : __size_upto(__r - 1) * static_cast<size_type>(extent(__r - 1));
    }
    constexpr size_type size() const { return __size_upto(rank()); }
    constexpr bool empty() const { return size() == 0; }

    constexpr const extents_type&     extents() const     { return __map_.extents(); }
    constexpr const data_handle_type& data_handle() const { return __ptr_; }
    constexpr const mapping_type&     mapping() const     { return __map_; }
    constexpr const accessor_type&    accessor() const    { return __acc_; }

    static constexpr bool is_always_unique()     { return mapping_type::is_always_unique(); }
    static constexpr bool is_always_exhaustive() { return mapping_type::is_always_exhaustive(); }
    static constexpr bool is_always_strided()    { return mapping_type::is_always_strided(); }
    constexpr bool is_unique() const     { return __map_.is_unique(); }
    constexpr bool is_exhaustive() const { return __map_.is_exhaustive(); }
    constexpr bool is_strided() const    { return __map_.is_strided(); }
    constexpr index_type stride(rank_type __r) const { return __map_.stride(__r); }

    friend void swap(mdspan& __x, mdspan& __y) {
        mdspan __t = __x; __x = __y; __y = __t;
    }

private:
    template <class _OtherIndexType, _CCW_STD::size_t _Np>
    _ccw_constexpr14 index_type __offset_of(const _CCW_STD::array<_OtherIndexType, _Np>& __indices) const {
        index_type __off = 0;
        for (rank_type __r = 0; __r < rank(); ++__r)
            __off += static_cast<index_type>(__indices[__r]) * __map_.stride(__r);
        return __off;
    }
    constexpr void __check_one(rank_type __r, index_type __v) const {
        if (!(__v >= 0 && static_cast<size_type>(__v) < static_cast<size_type>(extent(__r))))
            _CCW_THROW(_CCW_STD::out_of_range("mdspan::at"));
    }
    constexpr void __check_pack(rank_type) const {}
    template <class... _Rest>
    constexpr void __check_pack(rank_type __r, index_type __v, _Rest... __rest) const {
        __check_one(__r, __v);
        __check_pack(__r + 1, __rest...);
    }

    accessor_type    __acc_;
    mapping_type     __map_;
    data_handle_type __ptr_;
};

}   // namespace _CCW_STD23

#endif  // gate
#endif  // _CCW_DETAIL_MDSPAN_HPP
