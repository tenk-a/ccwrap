/**
 *  @file   detail/llibcxx03/include/__exception/eh_anchor_all.h
 *  @brief  Every exception anchor in one place: the single-TU form of the
 *          Open Watcom C10 workaround. Include it from exactly one .cpp.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_LIBCPP___EXCEPTION_EH_ANCHOR_ALL_H
#define _CCW_LIBCPP___EXCEPTION_EH_ANCHOR_ALL_H
#include "../__config"
#include "eh_anchor.h"

#if defined(__WATCOMC__) && _CCW_HAS_EXCEPTIONS

#include <exception>
#include <new>
#include <stdexcept>
#include <system_error>
#include <typeinfo>
#include <any>
#include <optional>
#include <variant>
#include <memory>
#include <functional>
/* regex_error / ios_base::failure / filesystem_error / future_error live in
   headers that need libccwcxx.lib anyway, so they are opt-in: a program that
   catches them already links the library. */
#if defined(_CCW_EH_ANCHOR_FULL)
#include <regex>
#include <ios>
#include <filesystem>
#if defined(_CCW_OS_WIN)
#include <future>
#endif
#endif

_CCW_EH_ANCHOR_DEF(all_exc, exception, exception)
_CCW_EH_ANCHOR_DEF(all_bexc, bad_exception, bad_exception)
_CCW_EH_ANCHOR_DEF(all_balc, bad_alloc, bad_alloc)
_CCW_EH_ANCHOR_DEF(all_bcst, bad_cast, bad_cast)
_CCW_EH_ANCHOR_DEF(all_barr, bad_array_new_length, bad_array_new_length)
_CCW_EH_ANCHOR_DEF(all_elog, logic_error, logic_error)
_CCW_EH_ANCHOR_DEF(all_erun, runtime_error, runtime_error)
_CCW_EH_ANCHOR_DEF(all_edom, domain_error, domain_error)
_CCW_EH_ANCHOR_DEF(all_einv, invalid_argument, invalid_argument)
_CCW_EH_ANCHOR_DEF(all_elen, length_error, length_error)
_CCW_EH_ANCHOR_DEF(all_eoor, out_of_range, out_of_range)
_CCW_EH_ANCHOR_DEF(all_erng, range_error, range_error)
_CCW_EH_ANCHOR_DEF(all_eovf, overflow_error, overflow_error)
_CCW_EH_ANCHOR_DEF(all_eunf, underflow_error, underflow_error)
_CCW_EH_ANCHOR_DEF(all_esys, system_error, system_error)
_CCW_EH_ANCHOR_DEF(all_eopt, bad_optional_access, bad_optional_access)
_CCW_EH_ANCHOR_DEF(all_evar, bad_variant_access, bad_variant_access)
_CCW_EH_ANCHOR_DEF(all_etid, bad_typeid, bad_typeid)
_CCW_EH_ANCHOR_DEF(all_eany, bad_any_cast, bad_any_cast)
_CCW_EH_ANCHOR_DEF(all_ebfc, bad_function_call, bad_function_call)
_CCW_EH_ANCHOR_DEF(all_ebwp, bad_weak_ptr, bad_weak_ptr)
#if defined(_CCW_EH_ANCHOR_FULL)
_CCW_EH_ANCHOR_DEF(all_ergx, regex_error, regex_error)
_CCW_EH_ANCHOR_DEF(all_eios, ios_base::failure, failure)
_CCW_EH_ANCHOR_DEF(all_efs, filesystem::filesystem_error, filesystem_error)
#if defined(_CCW_OS_WIN)
_CCW_EH_ANCHOR_DEF(all_efut, future_error, future_error)
#endif
#endif

#endif // __WATCOMC__ && _CCW_HAS_EXCEPTIONS
#endif // _CCW_LIBCPP___EXCEPTION_EH_ANCHOR_ALL_H
