/**
 *  @file   detail/unordered_set.hpp
 *  @brief  std::unordered_set / unordered_multiset backport for pre-C++11 VC (vc8/9).
 *  @license Boost Software License Version 1.0
 *  @note   Thin wrappers over __ccw::hashtable (detail/hashtable.hpp). Value
 *          semantics; keys are immutable (iterator == const_iterator).
 */
#ifndef _CCW_DETAIL_UNORDERED_SET_HPP
#define _CCW_DETAIL_UNORDERED_SET_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "hashtable.hpp"
#include <functional>
#include <utility>

#if _MSC_VER < 1600   // vc8/9: no native <unordered_set>

namespace std {

template<class Key, class Hash = hash<Key>, class KeyEqual = equal_to<Key> >
class unordered_set {
    struct _KOV { const Key& operator()(const Key& v) const { return v; } };
    typedef __ccw::hashtable<Key, Key, _KOV, Hash, KeyEqual, false> _ht;
    _ht ht_;
public:
    typedef Key         key_type;
    typedef Key         value_type;
    typedef std::size_t size_type;
    typedef Hash        hasher;
    typedef KeyEqual    key_equal;
    typedef typename _ht::const_iterator iterator;
    typedef typename _ht::const_iterator const_iterator;

    unordered_set() {}
    explicit unordered_set(size_type n) : ht_(n) {}
    template<class It> unordered_set(It f, It l) { ht_.insert(f, l); }

    iterator       begin() const { return ht_.begin(); }
    iterator       end()   const { return ht_.end(); }
    const_iterator cbegin()const { return ht_.cbegin(); }
    const_iterator cend()  const { return ht_.cend(); }

    bool      empty()    const { return ht_.empty(); }
    size_type size()     const { return ht_.size(); }
    size_type max_size() const { return ht_.max_size(); }
    void      clear()          { ht_.clear(); }

    std::pair<iterator, bool> insert(const value_type& v) {
        std::pair<typename _ht::iterator, bool> r = ht_.insert(v);
        return std::pair<iterator, bool>(const_iterator(r.first), r.second);
    }
    template<class It> void insert(It f, It l) { ht_.insert(f, l); }
#if _CCW_NO_VARIADIC_TEMPLATES
    std::pair<iterator, bool> emplace(const value_type& v) { return insert(v); }
#else
    template<class... A> std::pair<iterator, bool> emplace(A&&... a) { return insert(value_type(static_cast<A&&>(a)...)); }
#endif

    size_type erase(const key_type& k) { return ht_.erase(k); }
    iterator  erase(const_iterator pos) {
        typename _ht::iterator hit(pos.cur, pos.ht, pos.bkt);
        return const_iterator(ht_.erase(hit));
    }
    iterator  find(const key_type& k)  const { return ht_.find(k); }
    size_type count(const key_type& k) const { return ht_.count(k); }
    bool      contains(const key_type& k) const { return ht_.contains(k); }
    std::pair<iterator, iterator> equal_range(const key_type& k) const { return ht_.equal_range(k); }
    size_type max_bucket_count() const { return ht_.max_bucket_count(); }
    size_type bucket(const key_type& k) const { return ht_.bucket(k); }
    size_type bucket_size(size_type n) const { return ht_.bucket_size(n); }

    size_type bucket_count()     const { return ht_.bucket_count(); }
    float     load_factor()      const { return ht_.load_factor(); }
    float     max_load_factor()  const { return ht_.max_load_factor(); }
    void      max_load_factor(float f) { ht_.max_load_factor(f); }
    void      rehash(size_type n)      { ht_.rehash(n); }
    void      reserve(size_type n)     { ht_.reserve(n); }
    hasher    hash_function()    const { return ht_.hash_function(); }
    key_equal key_eq()           const { return ht_.key_eq(); }
    void      swap(unordered_set& r)   { ht_.swap(r.ht_); }
};

template<class Key, class Hash = hash<Key>, class KeyEqual = equal_to<Key> >
class unordered_multiset {
    struct _KOV { const Key& operator()(const Key& v) const { return v; } };
    typedef __ccw::hashtable<Key, Key, _KOV, Hash, KeyEqual, true> _ht;
    _ht ht_;
public:
    typedef Key         key_type;
    typedef Key         value_type;
    typedef std::size_t size_type;
    typedef Hash        hasher;
    typedef KeyEqual    key_equal;
    typedef typename _ht::const_iterator iterator;
    typedef typename _ht::const_iterator const_iterator;

    unordered_multiset() {}
    explicit unordered_multiset(size_type n) : ht_(n) {}
    template<class It> unordered_multiset(It f, It l) { ht_.insert(f, l); }

    iterator begin() const { return ht_.begin(); }
    iterator end()   const { return ht_.end(); }
    bool      empty() const { return ht_.empty(); }
    size_type size()  const { return ht_.size(); }
    void      clear()       { ht_.clear(); }

    iterator insert(const value_type& v) { return const_iterator(ht_.insert(v).first); }
    template<class It> void insert(It f, It l) { ht_.insert(f, l); }
    size_type erase(const key_type& k) { return ht_.erase(k); }
    iterator  erase(const_iterator pos) {
        typename _ht::iterator hit(pos.cur, pos.ht, pos.bkt);
        return const_iterator(ht_.erase(hit));
    }
    iterator  find(const key_type& k)  const { return ht_.find(k); }
    size_type count(const key_type& k) const { return ht_.count(k); }
    bool      contains(const key_type& k) const { return ht_.contains(k); }
    std::pair<iterator, iterator> equal_range(const key_type& k) const { return ht_.equal_range(k); }
    size_type max_bucket_count() const { return ht_.max_bucket_count(); }
    size_type bucket(const key_type& k) const { return ht_.bucket(k); }
    size_type bucket_size(size_type n) const { return ht_.bucket_size(n); }
    size_type bucket_count() const { return ht_.bucket_count(); }
    void      swap(unordered_multiset& r) { ht_.swap(r.ht_); }
};

template<class K, class H, class E> inline void swap(unordered_set<K,H,E>& a, unordered_set<K,H,E>& b) { a.swap(b); }
template<class K, class H, class E> inline void swap(unordered_multiset<K,H,E>& a, unordered_multiset<K,H,E>& b) { a.swap(b); }

}   // namespace std

#endif  // _MSC_VER < 1600

#endif  // _CCW_DETAIL_UNORDERED_SET_HPP
