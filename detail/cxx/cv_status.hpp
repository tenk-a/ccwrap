/**
 *  @file   detail/cxx/cv_status.hpp
 *  @brief  std::cv_status for the pre-C++11 <condition_variable> backports.
 *  @license Boost Software License Version 1.0
 *  @note   C++11 makes cv_status a scoped enum, so `cv_status::timeout` has to
 *          work. Without enum class, approximate it with a struct that carries
 *          the enumerators as members and converts back to the enum.
 */
#ifndef _CCW_DETAIL_CV_STATUS_HPP
#define _CCW_DETAIL_CV_STATUS_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

namespace std {

#if _CCW_HAS_ENUM_CLASS
enum class cv_status { no_timeout, timeout };
#else
struct cv_status {
    enum enum_type { no_timeout, timeout };
    enum_type __v_;
    cv_status() : __v_(no_timeout) {}
    cv_status(enum_type __v) : __v_(__v) {}
    operator enum_type() const { return __v_; }
};
#endif

}   // namespace std

#endif  // _CCW_DETAIL_CV_STATUS_HPP
