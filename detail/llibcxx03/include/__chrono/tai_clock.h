//===----------------------------------------------------------------------===//
// ccwrap: libc++ layout, classic C++03 body for Open Watcom.
// [time.clock.tai] and [time.clock.gps] -- both are a fixed offset from utc_clock.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_TAI_CLOCK_H
#define _CCW_LIBCPP___CHRONO_TAI_CLOCK_H
#include "../__config"
#include "utc_clock.h"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono {

class tai_clock {
public:
    typedef chrono::seconds                         duration;
    typedef duration::rep                           rep;
    typedef duration::period                        period;
    typedef chrono::time_point<tai_clock, duration> time_point;
    static const bool is_steady = false;

    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    to_utc(const chrono::time_point<tai_clock, _Dur>& __t) {
        return chrono::time_point<utc_clock, _Dur>(__t.time_since_epoch() - duration_cast<_Dur>(seconds(378691210)));
    }
    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<tai_clock, _Dur>
    from_utc(const chrono::time_point<utc_clock, _Dur>& __t) {
        return chrono::time_point<tai_clock, _Dur>(__t.time_since_epoch() + duration_cast<_Dur>(seconds(378691210)));
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static time_point now() { return from_utc(utc_clock::now()); }
};

typedef chrono::time_point<tai_clock, seconds> tai_seconds;
#if _CCW_HAS_ALIAS_TEMPLATE
template <class _Dur> using tai_time = chrono::time_point<tai_clock, _Dur>;
#endif

class gps_clock {
public:
    typedef chrono::seconds                         duration;
    typedef duration::rep                           rep;
    typedef duration::period                        period;
    typedef chrono::time_point<gps_clock, duration> time_point;
    static const bool is_steady = false;

    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    to_utc(const chrono::time_point<gps_clock, _Dur>& __t) {
        return chrono::time_point<utc_clock, _Dur>(__t.time_since_epoch() + duration_cast<_Dur>(seconds(315964809)));
    }
    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<gps_clock, _Dur>
    from_utc(const chrono::time_point<utc_clock, _Dur>& __t) {
        return chrono::time_point<gps_clock, _Dur>(__t.time_since_epoch() - duration_cast<_Dur>(seconds(315964809)));
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static time_point now() { return from_utc(utc_clock::now()); }
};

typedef chrono::time_point<gps_clock, seconds> gps_seconds;
#if _CCW_HAS_ALIAS_TEMPLATE
template <class _Dur> using gps_time = chrono::time_point<gps_clock, _Dur>;
#endif

}   // namespace chrono
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___CHRONO_TAI_CLOCK_H
