//===----------------------------------------------------------------------===//
// ccwrap: libc++ layout, classic C++03 body for Open Watcom.
// [time.clock.cast] clock_time_conversion and clock_cast.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_CLOCK_CAST_H
#define _CCW_LIBCPP___CHRONO_CLOCK_CAST_H
#include "../__config"
#include "tai_clock.h"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono {

template <class _Clock> struct __ccw_clock_utc;

template <> struct __ccw_clock_utc<system_clock> {
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    __to(const chrono::time_point<system_clock, _Dur>& __t) { return utc_clock::from_sys(__t); }
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<system_clock, _Dur>
    __from(const chrono::time_point<utc_clock, _Dur>& __t) { return utc_clock::to_sys(__t); }
};
template <> struct __ccw_clock_utc<utc_clock> {
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    __to(const chrono::time_point<utc_clock, _Dur>& __t) { return __t; }
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    __from(const chrono::time_point<utc_clock, _Dur>& __t) { return __t; }
};
template <> struct __ccw_clock_utc<tai_clock> {
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    __to(const chrono::time_point<tai_clock, _Dur>& __t) { return tai_clock::to_utc(__t); }
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<tai_clock, _Dur>
    __from(const chrono::time_point<utc_clock, _Dur>& __t) { return tai_clock::from_utc(__t); }
};
template <> struct __ccw_clock_utc<gps_clock> {
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    __to(const chrono::time_point<gps_clock, _Dur>& __t) { return gps_clock::to_utc(__t); }
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<gps_clock, _Dur>
    __from(const chrono::time_point<utc_clock, _Dur>& __t) { return gps_clock::from_utc(__t); }
};
template <> struct __ccw_clock_utc<file_clock> {
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    __to(const chrono::time_point<file_clock, _Dur>& __t) { return file_clock::to_utc(__t); }
    template <class _Dur> _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<file_clock, _Dur>
    __from(const chrono::time_point<utc_clock, _Dur>& __t) { return file_clock::from_utc(__t); }
};

template <class _DestClock, class _SourceClock>
struct clock_time_conversion {
    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::time_point<_DestClock, _Dur>
    operator()(const chrono::time_point<_SourceClock, _Dur>& __t) const {
        return __ccw_clock_utc<_DestClock>::__from(__ccw_clock_utc<_SourceClock>::__to(__t));
    }
};

template <class _Clock>
struct clock_time_conversion<_Clock, _Clock> {
    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI chrono::time_point<_Clock, _Dur>
    operator()(const chrono::time_point<_Clock, _Dur>& __t) const { return __t; }
};

template <class _DestClock, class _SourceClock, class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI chrono::time_point<_DestClock, _Dur>
clock_cast(const chrono::time_point<_SourceClock, _Dur>& __t) {
    return clock_time_conversion<_DestClock, _SourceClock>()(__t);
}

}   // namespace chrono
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___CHRONO_CLOCK_CAST_H
