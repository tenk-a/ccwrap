/**
 *  @file   detail/cxx/ranges_algo_pack.hpp
 *  @brief  The ranges algorithm supplements, bundled so <algorithm> / <vector> /
 *          <string> can all pull them in; no-op while a needed native container
 *          header is still being included (see doc/src/detail/cxx/ranges_algo_pack.hpp.md).
 *  @license Boost Software License Version 1.0
 */
#if !(defined(_LIBCPP_VECTOR) && !defined(_CCW_VECTOR_DONE)) && \
    !(defined(_LIBCPP_STRING) && !defined(_CCW_STRING_DONE))
#ifndef _CCW_DETAIL_RANGES_ALGO_PACK_HPP
#define _CCW_DETAIL_RANGES_ALGO_PACK_HPP

#include <ccwrap_common.h>
#include <vector>
#include <../../detail/llibcxx03/include/__algorithm/in_fun_result.h>
#include <../../detail/llibcxx03/include/__algorithm/in_in_result.h>
#include <../../detail/llibcxx03/include/__algorithm/in_out_result.h>
#include <../../detail/llibcxx03/include/__algorithm/in_in_out_result.h>
#include <../../detail/llibcxx03/include/__algorithm/in_out_out_result.h>
#include <../../detail/llibcxx03/include/__algorithm/min_max_result.h>
#include <../../detail/llibcxx03/include/__algorithm/in_found_result.h>
#include <../../detail/llibcxx03/include/__algorithm/in_value_result.h>
#include <../../detail/llibcxx03/include/__algorithm/out_value_result.h>
#include <../../detail/llibcxx03/include/__algorithm/ranges_util.h>
#include <../../detail/llibcxx03/include/__algorithm/ranges_nonmodifying.h>
#include <../../detail/llibcxx03/include/__algorithm/ranges_modifying.h>
#include <../../detail/llibcxx03/include/__algorithm/ranges_sorting.h>
#include <../../detail/llibcxx03/include/__algorithm/ranges_setops.h>
#include <../../detail/llibcxx03/include/__algorithm/ranges_minmax.h>

#endif  // _CCW_DETAIL_RANGES_ALGO_PACK_HPP
#endif  // not inside native <vector> / <string>
