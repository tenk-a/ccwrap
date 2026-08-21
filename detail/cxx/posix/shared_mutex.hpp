/**
 *  @file   detail/cxx/posix/shared_mutex.hpp
 *  @brief  <shared_mutex> backport over POSIX threads (pre-C++14 fallback).
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_POSIX_SHARED_MUTEX_HPP
#define _CCW_DETAIL_POSIX_SHARED_MUTEX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <exception>
#include "../move03.hpp"

namespace std {

class shared_mutex {
    ::pthread_rwlock_t rw_;
    shared_mutex(const shared_mutex&);
    shared_mutex& operator=(const shared_mutex&);
public:
    typedef ::pthread_rwlock_t* native_handle_type;
    shared_mutex()  { ::pthread_rwlock_init(&rw_, 0); }
    ~shared_mutex() { ::pthread_rwlock_destroy(&rw_); }
    void lock()             { ::pthread_rwlock_wrlock(&rw_); }
    bool try_lock()         { return ::pthread_rwlock_trywrlock(&rw_) == 0; }
    void unlock()           { ::pthread_rwlock_unlock(&rw_); }
    void lock_shared()      { ::pthread_rwlock_rdlock(&rw_); }
    bool try_lock_shared()  { return ::pthread_rwlock_tryrdlock(&rw_) == 0; }
    void unlock_shared()    { ::pthread_rwlock_unlock(&rw_); }
    native_handle_type native_handle() { return &rw_; }
};

}   // namespace std

#include "../shared_mutex_generic.hpp"

#endif  // _CCW_DETAIL_POSIX_SHARED_MUTEX_HPP
