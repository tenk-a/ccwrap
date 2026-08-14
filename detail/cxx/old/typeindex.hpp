/**
 *  @file   detail/typeindex.hpp
 *  @brief  std::type_index backport for pre-C++11 VC (vc8/9).
 *  @license Boost Software License Version 1.0
 *  @note   Wraps a type_info reference. hash_code() hashes name() (FNV-1a) because
 *          type_info::hash_code() itself is C++11 and absent here; equal types share
 *          a name, so the contract (equal -> equal hash) holds.
 */
#ifndef _CCW_DETAIL_TYPEINDEX_HPP
#define _CCW_DETAIL_TYPEINDEX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <typeinfo>
#include <cstddef>

#if _MSC_VER < 1600   // vc8/9: no native <typeindex>

namespace std {

class type_index {
    const type_info* p_;
public:
    type_index(const type_info& t) : p_(&t) {}

    bool operator==(const type_index& r) const { return (*p_ == *r.p_) != 0; }
    bool operator!=(const type_index& r) const { return (*p_ != *r.p_) != 0; }
    bool operator< (const type_index& r) const { return p_->before(*r.p_) != 0; }
    bool operator> (const type_index& r) const { return r.p_->before(*p_) != 0; }
    bool operator<=(const type_index& r) const { return !(r < *this); }
    bool operator>=(const type_index& r) const { return !(*this < r); }

    const char* name() const { return p_->name(); }
    std::size_t hash_code() const {
        const char* s = p_->name();
        std::size_t h = std::size_t(2166136261u);
        for (; *s; ++s) { h ^= std::size_t((unsigned char)*s); h *= std::size_t(16777619u); }
        return h;
    }
};

template<class T> struct hash;   // primary (also declared by detail/hashtable.hpp)
template<> struct hash<type_index> {
    std::size_t operator()(const type_index& t) const { return t.hash_code(); }
};

}   // namespace std

#endif  // _MSC_VER < 1600

#endif  // _CCW_DETAIL_TYPEINDEX_HPP
