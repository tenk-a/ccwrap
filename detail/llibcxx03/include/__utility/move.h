/*
 * ccwrap: libc++ layout. classic C++03 body for Open Watcom.
 *  _CCW_STD::move for compilers without rvalue references.
 *
 *  The mechanism (Boost.Move's rv<T>) lives in detail/cxx/move03.hpp so that the vc
 *  and gcc trees can use it too; this header is the llibcxx03 bridge, exactly like
 *  __fnctmpl.h bridges detail/fnctmpl.hpp. It publishes the emulation as
 *  _CCW_STD::move, so ordinary user code spells the ordinary name:
 *
 *      _CCW_STD::vector<buf> v;
 *      buf b;
 *      v.push_back(_CCW_STD::move(b));       * steals from b
 *
 *  This is an EXPLICIT move only: a temporary is never moved from automatically
 *  (Open Watcom cannot rank the conversion operators Boost uses for that).
 *  _CCW_STD::forward is NOT provided: perfect forwarding needs reference collapsing
 * See detail/move03.hpp for the full capability list and the probe results behind it.
 */
#ifndef _CCW_LIBCPP___UTILITY_MOVE_H
#define _CCW_LIBCPP___UTILITY_MOVE_H
#include "../__config"
#include "../../../cxx/move03.hpp"

#if _CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
#  include <utility>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
using ::_ccw::move;
using ::_ccw::move_if_noexcept;   // always moves here: no noexcept to query (see move03.hpp)
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_CXX11_LIB

#endif // _CCW_LIBCPP___UTILITY_MOVE_H
