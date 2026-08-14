// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//  __chrono/file_clock.h -- chrono::file_clock (C++20, [time.clock.file]).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_FILE_CLOCK_H
#define _CCW_LIBCPP___CHRONO_FILE_CLOCK_H
#include "../__config"
#include <ctime>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono {

class utc_clock;   // for to_utc / from_utc below; defined in __chrono/utc_clock.h

class file_clock {
public:
    typedef chrono::seconds                          duration;
    typedef duration::rep                            rep;
    typedef duration::period                         period;
    typedef chrono::time_point<file_clock, duration> time_point;
    static const bool is_steady = false;

    _CCW_LIBCPP_HIDE_FROM_ABI static time_point now() { return time_point(duration((long long)_CCW_STD::time(0))); }
    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<system_clock, _Dur>
    to_sys(const chrono::time_point<file_clock, _Dur>& __t)
        { return chrono::time_point<system_clock, _Dur>(__t.time_since_epoch()); }
    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<file_clock, _Dur>
    from_sys(const chrono::time_point<system_clock, _Dur>& __t)
        { return chrono::time_point<file_clock, _Dur>(__t.time_since_epoch()); }

    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<utc_clock, _Dur>
    to_utc(const chrono::time_point<file_clock, _Dur>& __t);
    template <class _Dur>
    _CCW_LIBCPP_HIDE_FROM_ABI static chrono::time_point<file_clock, _Dur>
    from_utc(const chrono::time_point<utc_clock, _Dur>& __t);
};
template <class _Dur> struct __file_time_helper { typedef time_point<file_clock, _Dur> type; };
#if _CCW_HAS_ALIAS_TEMPLATE
template <class _Dur> using file_time = time_point<file_clock, _Dur>;
#endif

}   // namespace chrono
_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___CHRONO_FILE_CLOCK_H
