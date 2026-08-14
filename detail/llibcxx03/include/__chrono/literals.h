// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//  __chrono/literals.h -- the C++20 calendar literals ([time.cal.day.nonmembers],
//  [time.cal.year.nonmembers]). Needs user-defined literals, so it is gated.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___CHRONO_LITERALS_H
#define _CCW_LIBCPP___CHRONO_LITERALS_H

#include "../__config"

#if _CCW_HAS_UDL

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

inline namespace literals {
inline namespace chrono_literals {

inline chrono::day  operator "" d(unsigned long long __d) {
    return chrono::day(static_cast<unsigned>(__d));
}
inline chrono::year operator "" y(unsigned long long __y) {
    return chrono::year(static_cast<int>(__y));
}

}   // inline namespace chrono_literals
}   // inline namespace literals

_CCW_LIBCPP_END_NAMESPACE_STD

#endif  // _CCW_HAS_UDL
#endif  // _CCW_LIBCPP___CHRONO_LITERALS_H
