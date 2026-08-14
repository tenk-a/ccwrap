//===----------------------------------------------------------------------===//
// ccwrap: libc++ layout, classic C++03 body for Open Watcom.
// [time.hash] hash support for the chrono value classes (P2592, C++26).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_HASH_H
#define _CCW_LIBCPP___CHRONO_HASH_H
#include "../__config"
#include "../__functional/hash.h"
#include <cstddef>

#if _CCW_TARGET_CXX >= 2026

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

_CCW_LIBCPP_HIDE_FROM_ABI inline _CCW_STD::size_t __ccw_hash_mix(_CCW_STD::size_t __seed, _CCW_STD::size_t __v) {
    return __seed ^ (__v + 0x9e3779b9u + (__seed << 6) + (__seed >> 2));
}

template <class _Rep, class _Period>
struct hash<chrono::duration<_Rep, _Period> > {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::duration<_Rep, _Period>& __d) const {
        hash<_Rep> __h;
        return __h(__d.count());
    }
};

template <class _Clock, class _Duration>
struct hash<chrono::time_point<_Clock, _Duration> > {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::time_point<_Clock, _Duration>& __t) const {
        hash<_Duration> __h;
        return __h(__t.time_since_epoch());
    }
};

template <> struct hash<chrono::day> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::day& __d) const {
        return (_CCW_STD::size_t)(unsigned)__d;
    }
};
template <> struct hash<chrono::month> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::month& __m) const {
        return (_CCW_STD::size_t)(unsigned)__m;
    }
};
template <> struct hash<chrono::year> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::year& __y) const {
        return (_CCW_STD::size_t)(int)__y;
    }
};
template <> struct hash<chrono::weekday> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::weekday& __w) const {
        return (_CCW_STD::size_t)__w.c_encoding();
    }
};
template <> struct hash<chrono::weekday_indexed> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::weekday_indexed& __wi) const {
        hash<chrono::weekday> __hw;
        return __ccw_hash_mix(__hw(__wi.weekday()), (_CCW_STD::size_t)__wi.index());
    }
};
template <> struct hash<chrono::weekday_last> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::weekday_last& __wl) const {
        hash<chrono::weekday> __hw;
        return __ccw_hash_mix(__hw(__wl.weekday()), 0x6c617374u);   // 'last'
    }
};
template <> struct hash<chrono::month_day> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::month_day& __md) const {
        hash<chrono::month> __hm;  hash<chrono::day> __hd;
        return __ccw_hash_mix(__hm(__md.month()), __hd(__md.day()));
    }
};
template <> struct hash<chrono::month_day_last> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::month_day_last& __mdl) const {
        hash<chrono::month> __hm;
        return __ccw_hash_mix(__hm(__mdl.month()), 0x6c617374u);
    }
};
template <> struct hash<chrono::month_weekday> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::month_weekday& __mw) const {
        hash<chrono::month> __hm;  hash<chrono::weekday_indexed> __hwi;
        return __ccw_hash_mix(__hm(__mw.month()), __hwi(__mw.weekday_indexed()));
    }
};
template <> struct hash<chrono::month_weekday_last> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::month_weekday_last& __mwl) const {
        hash<chrono::month> __hm;  hash<chrono::weekday_last> __hwl;
        return __ccw_hash_mix(__hm(__mwl.month()), __hwl(__mwl.weekday_last()));
    }
};
template <> struct hash<chrono::year_month> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::year_month& __ym) const {
        hash<chrono::year> __hy;  hash<chrono::month> __hm;
        return __ccw_hash_mix(__hy(__ym.year()), __hm(__ym.month()));
    }
};
template <> struct hash<chrono::year_month_day> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::year_month_day& __ymd) const {
        hash<chrono::year> __hy;  hash<chrono::month> __hm;  hash<chrono::day> __hd;
        _CCW_STD::size_t __r = __ccw_hash_mix(__hy(__ymd.year()), __hm(__ymd.month()));
        return __ccw_hash_mix(__r, __hd(__ymd.day()));
    }
};
template <> struct hash<chrono::year_month_day_last> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::year_month_day_last& __ymdl) const {
        hash<chrono::year> __hy;  hash<chrono::month_day_last> __hmdl;
        return __ccw_hash_mix(__hy(__ymdl.year()), __hmdl(__ymdl.month_day_last()));
    }
};
template <> struct hash<chrono::year_month_weekday> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::year_month_weekday& __ymw) const {
        hash<chrono::year> __hy;  hash<chrono::month> __hm;  hash<chrono::weekday_indexed> __hwi;
        _CCW_STD::size_t __r = __ccw_hash_mix(__hy(__ymw.year()), __hm(__ymw.month()));
        return __ccw_hash_mix(__r, __hwi(__ymw.weekday_indexed()));
    }
};
template <> struct hash<chrono::year_month_weekday_last> {
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t operator()(const chrono::year_month_weekday_last& __ymwl) const {
        hash<chrono::year> __hy;  hash<chrono::month> __hm;  hash<chrono::weekday_last> __hwl;
        _CCW_STD::size_t __r = __ccw_hash_mix(__hy(__ymwl.year()), __hm(__ymwl.month()));
        return __ccw_hash_mix(__r, __hwl(__ymwl.weekday_last()));
    }
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_TARGET_CXX >= 2026
#endif // _CCW_LIBCPP___CHRONO_HASH_H
