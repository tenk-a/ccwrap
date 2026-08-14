//===----------------------------------------------------------------------===//
// ccwrap: libc++ layout, classic C++03 body for Open Watcom.
// [time.clock.utc] utc_clock -- the clock that counts leap seconds.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_UTC_CLOCK_H
#define _CCW_LIBCPP___CHRONO_UTC_CLOCK_H
#include "../__config"
#include "leap_second.h"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono {

class utc_clock {
public:
    typedef chrono::seconds                         duration;
    typedef duration::rep                           rep;
    typedef duration::period                        period;
    typedef chrono::time_point<utc_clock, duration> time_point;
    static const bool is_steady = false;

    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    from_sys(const chrono::time_point<system_clock, _Dur>& __t) {
        seconds __s = duration_cast<seconds>(__t.time_since_epoch());
        long long __add = chrono::__ccw_leaps_at_sys((long long)__s.count());
        _Dur __off = duration_cast<_Dur>(seconds(__add));
        return chrono::time_point<utc_clock, _Dur>(__t.time_since_epoch() + __off);
    }

    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<system_clock, _Dur>
    to_sys(const chrono::time_point<utc_clock, _Dur>& __t) {
        seconds    __s = duration_cast<seconds>(__t.time_since_epoch());
        long long  __elapsed;
        bool       __is_leap;
        chrono::__ccw_leaps_at_utc((long long)__s.count(), __elapsed, __is_leap);
        _Dur __off = duration_cast<_Dur>(seconds(__elapsed));
        return chrono::time_point<system_clock, _Dur>(__t.time_since_epoch() - __off);
    }

    _CCW_LIBCPP_HIDE_FROM_ABI static time_point now() {
        return chrono::time_point_cast<duration>(from_sys(system_clock::now())); }
};

typedef chrono::time_point<utc_clock, seconds> utc_seconds;
#if _CCW_HAS_ALIAS_TEMPLATE
template <class _Dur> using utc_time = chrono::time_point<utc_clock, _Dur>;
#endif

template <class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI leap_second_info get_leap_second_info(const chrono::time_point<utc_clock, _Dur>& __t) {
    seconds   __s = duration_cast<seconds>(__t.time_since_epoch());
    long long __elapsed;
    bool      __is_leap;
    chrono::__ccw_leaps_at_utc((long long)__s.count(), __elapsed, __is_leap);
    return leap_second_info(__is_leap, seconds(__elapsed));
}

template <class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI chrono::time_point<utc_clock, _Dur>
file_clock::to_utc(const chrono::time_point<file_clock, _Dur>& __t) {
    return utc_clock::from_sys(file_clock::to_sys(__t));
}
template <class _Dur>
_CCW_LIBCPP_HIDE_FROM_ABI chrono::time_point<file_clock, _Dur>
file_clock::from_utc(const chrono::time_point<utc_clock, _Dur>& __t) {
    return file_clock::from_sys(utc_clock::to_sys(__t));
}

}   // namespace chrono
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___CHRONO_UTC_CLOCK_H
