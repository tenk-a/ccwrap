/**
 *  @file   detail/llibcxx03/include/__exception/eh_anchor.h
 *  @brief  _CCW_EH_ANCHOR: force out the destructor body that a catch handler's
 *          type table names but Open Watcom never emits (bug C10).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_LIBCPP___EXCEPTION_EH_ANCHOR_H
#define _CCW_LIBCPP___EXCEPTION_EH_ANCHOR_H
#include "../__config"

#define _CCW_EH_ANCHOR_DEF(__id, __qual, __name)                              \
    namespace {                                                               \
    void __ccw_eh_dtor_##__id(_CCW_STD::__qual* __p) {                        \
        __p->_CCW_STD::__qual::~__name();                                     \
    }                                                                         \
    void* const __ccw_eh_anchor_##__id = (void*)&__ccw_eh_dtor_##__id;        \
    }

#if defined(__WATCOMC__) && _CCW_HAS_EXCEPTIONS && !defined(_CCW_EH_NO_ANCHOR)
#  define _CCW_EH_ANCHOR(__id, __qual, __name) _CCW_EH_ANCHOR_DEF(__id, __qual, __name)
#else
#  define _CCW_EH_ANCHOR(__id, __qual, __name)
#endif

#endif // _CCW_LIBCPP___EXCEPTION_EH_ANCHOR_H
