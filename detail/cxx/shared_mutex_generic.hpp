/**
 *  @file   detail/cxx/shared_mutex_generic.hpp
 *  @brief  std::shared_timed_mutex and std::shared_lock for the pre-C++14
 *          <shared_mutex> backports. Built from std::mutex and
 *          std::condition_variable, so the same body serves every OS layer.
 *  @license Boost Software License Version 1.0
 *  @note   Included by detail/cxx/{win,posix}/shared_mutex.hpp after they have
 *          defined std::shared_mutex.
 */
#ifndef _CCW_DETAIL_SHARED_MUTEX_GENERIC_HPP
#define _CCW_DETAIL_SHARED_MUTEX_GENERIC_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <mutex>
#include <condition_variable>
#include <exception>
#include "move03.hpp"

namespace std {

class shared_timed_mutex {
    mutex               m_;
    condition_variable  gate1_;
    condition_variable  gate2_;
    bool                writer_;
    unsigned            readers_;
    shared_timed_mutex(const shared_timed_mutex&);
    shared_timed_mutex& operator=(const shared_timed_mutex&);
public:
    shared_timed_mutex() : writer_(false), readers_(0) {}
    ~shared_timed_mutex() {}

    void lock() {
        unique_lock<mutex> __lk(m_);
        while (writer_) gate1_.wait(__lk);
        writer_ = true;
        while (readers_) gate2_.wait(__lk);
    }
    bool try_lock() {
        unique_lock<mutex> __lk(m_, try_to_lock);
        if (!__lk.owns_lock() || writer_ || readers_) return false;
        writer_ = true;
        return true;
    }
    void unlock() {
        { unique_lock<mutex> __lk(m_); writer_ = false; }
        gate1_.notify_all();
    }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        unique_lock<mutex> __lk(m_);
        while (writer_) {
            if (gate1_.wait_until(__lk, __tp) == cv_status::timeout && writer_) return false;
        }
        writer_ = true;
        while (readers_) {
            if (gate2_.wait_until(__lk, __tp) == cv_status::timeout && readers_) {
                writer_ = false;
                __lk.unlock();
                gate1_.notify_all();
                return false;
            }
        }
        return true;
    }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) {
        return try_lock_until(chrono::steady_clock::now() + __d);
    }

    void lock_shared() {
        unique_lock<mutex> __lk(m_);
        while (writer_) gate1_.wait(__lk);
        ++readers_;
    }
    bool try_lock_shared() {
        unique_lock<mutex> __lk(m_, try_to_lock);
        if (!__lk.owns_lock() || writer_) return false;
        ++readers_;
        return true;
    }
    void unlock_shared() {
        unique_lock<mutex> __lk(m_);
        if (--readers_ == 0) {
            __lk.unlock();
            gate2_.notify_one();
        }
    }
    template <class _Clock, class _Duration>
    bool try_lock_shared_until(const chrono::time_point<_Clock, _Duration>& __tp) {
        unique_lock<mutex> __lk(m_);
        while (writer_) {
            if (gate1_.wait_until(__lk, __tp) == cv_status::timeout && writer_) return false;
        }
        ++readers_;
        return true;
    }
    template <class _Rep, class _Period>
    bool try_lock_shared_for(const chrono::duration<_Rep, _Period>& __d) {
        return try_lock_shared_until(chrono::steady_clock::now() + __d);
    }
};

template<class M>
class shared_lock {
    M*   m_;
    bool owns_;
    shared_lock(const shared_lock&);
    shared_lock& operator=(const shared_lock&);
    typedef bool shared_lock::* _bool_type;
public:
    typedef M mutex_type;
    shared_lock() : m_(0), owns_(false) {}
    explicit shared_lock(M& m) : m_(&m), owns_(false) { lock(); }
    shared_lock(M& m, defer_lock_t)  : m_(&m), owns_(false) {}
    shared_lock(M& m, try_to_lock_t) : m_(&m), owns_(false) { owns_ = m_->try_lock_shared(); }
    shared_lock(M& m, adopt_lock_t)  : m_(&m), owns_(true) {}
    template <class _Rep, class _Period>
    shared_lock(M& m, const chrono::duration<_Rep, _Period>& __d) : m_(&m), owns_(false) { owns_ = m_->try_lock_shared_for(__d); }
    template <class _Clock, class _Duration>
    shared_lock(M& m, const chrono::time_point<_Clock, _Duration>& __tp) : m_(&m), owns_(false) { owns_ = m_->try_lock_shared_until(__tp); }
    ~shared_lock() { if (owns_ && m_) m_->unlock_shared(); }

    void lock()     { m_->lock_shared(); owns_ = true; }
    bool try_lock() { owns_ = m_->try_lock_shared(); return owns_; }
    void unlock()   { m_->unlock_shared(); owns_ = false; }
    bool owns_lock() const { return owns_; }
    M*   mutex() const { return m_; }
    M*   release() { M* r = m_; m_ = 0; owns_ = false; return r; }
    template <class _Rep, class _Period>
    bool try_lock_for(const chrono::duration<_Rep, _Period>& __d) { owns_ = m_->try_lock_shared_for(__d); return owns_; }
    template <class _Clock, class _Duration>
    bool try_lock_until(const chrono::time_point<_Clock, _Duration>& __tp) { owns_ = m_->try_lock_shared_until(__tp); return owns_; }
    void swap(shared_lock& __o) {
        M* __m = m_; m_ = __o.m_; __o.m_ = __m;
        bool __b = owns_; owns_ = __o.owns_; __o.owns_ = __b;
    }
    shared_lock(_CCW_RV_REF(shared_lock) __r) : m_(0), owns_(false) {
        shared_lock& __o = _CCW_MOVED_OBJ(__r);
        m_ = __o.m_; owns_ = __o.owns_; __o.m_ = 0; __o.owns_ = false;
    }
    shared_lock& operator=(_CCW_RV_REF(shared_lock) __r) {
        shared_lock& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) {
            if (owns_ && m_) m_->unlock_shared();
            m_ = __o.m_; owns_ = __o.owns_; __o.m_ = 0; __o.owns_ = false;
        }
        return *this;
    }
    operator _bool_type() const { return owns_ ? &shared_lock::owns_ : (_bool_type)0; }
};

template<class M>
inline void swap(shared_lock<M>& __a, shared_lock<M>& __b) { __a.swap(__b); }

}   // namespace std

#endif  // _CCW_DETAIL_SHARED_MUTEX_GENERIC_HPP
