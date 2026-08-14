/**
 *  @file   detail/unordered_map.hpp
 *  @brief  std::unordered_map / unordered_multimap backport for pre-C++11 VC (vc8/9).
 *  @license Boost Software License Version 1.0
 *  @note   Thin wrappers over __ccw::hashtable (detail/hashtable.hpp). value_type is
 *          pair<const Key, T> (key immutable, mapped mutable). Value semantics.
 */
#ifndef _CCW_DETAIL_UNORDERED_MAP_HPP
#define _CCW_DETAIL_UNORDERED_MAP_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "hashtable.hpp"
#include <functional>
#include <utility>
#include <stdexcept>

#if _MSC_VER < 1600   // vc8/9: no native <unordered_map>

namespace std {

template<class Key, class T, class Hash = hash<Key>, class KeyEqual = equal_to<Key> >
class unordered_map {
    typedef std::pair<const Key, T> _value;
    struct _KOV { const Key& operator()(const _value& v) const { return v.first; } };
    typedef __ccw::hashtable<Key, _value, _KOV, Hash, KeyEqual, false> _ht;
    _ht ht_;
public:
    typedef Key         key_type;
    typedef T           mapped_type;
    typedef _value      value_type;
    typedef std::size_t size_type;
    typedef Hash        hasher;
    typedef KeyEqual    key_equal;
    typedef typename _ht::iterator       iterator;
    typedef typename _ht::const_iterator const_iterator;

    unordered_map() {}
    explicit unordered_map(size_type n) : ht_(n) {}
    template<class It> unordered_map(It f, It l) { ht_.insert(f, l); }

    iterator       begin()       { return ht_.begin(); }
    const_iterator begin() const { return ht_.begin(); }
    iterator       end()       { return ht_.end(); }
    const_iterator end() const { return ht_.end(); }
    const_iterator cbegin()const { return ht_.cbegin(); }
    const_iterator cend()  const { return ht_.cend(); }

    bool      empty()    const { return ht_.empty(); }
    size_type size()     const { return ht_.size(); }
    size_type max_size() const { return ht_.max_size(); }
    void      clear()          { ht_.clear(); }

    std::pair<iterator, bool> insert(const value_type& v) { return ht_.insert(v); }
    template<class It> void insert(It f, It l) { ht_.insert(f, l); }
#if _CCW_NO_VARIADIC_TEMPLATES
    std::pair<iterator, bool> emplace(const value_type& v) { return ht_.insert(v); }
#else
    template<class... A> std::pair<iterator, bool> emplace(A&&... a) { return ht_.insert(value_type(static_cast<A&&>(a)...)); }
#endif

    mapped_type& operator[](const key_type& k) {
        iterator it = ht_.find(k);
        if (it != ht_.end()) return it->second;
        return ht_.insert(value_type(k, mapped_type())).first->second;
    }
    mapped_type& at(const key_type& k) {
        iterator it = ht_.find(k);
        if (it == ht_.end()) throw std::out_of_range("unordered_map::at");
        return it->second;
    }
    const mapped_type& at(const key_type& k) const {
        const_iterator it = ht_.find(k);
        if (it == ht_.end()) throw std::out_of_range("unordered_map::at");
        return it->second;
    }

    size_type      erase(const key_type& k) { return ht_.erase(k); }
    iterator       erase(const_iterator pos) {
        typename _ht::iterator hit(pos.cur, pos.ht, pos.bkt);
        return ht_.erase(hit);
    }
    iterator       find(const key_type& k)       { return ht_.find(k); }
    const_iterator find(const key_type& k) const { return ht_.find(k); }
    size_type      count(const key_type& k) const { return ht_.count(k); }
    bool           contains(const key_type& k) const { return ht_.contains(k); }
    std::pair<iterator, iterator>             equal_range(const key_type& k)       { return ht_.equal_range(k); }
    std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const { return ht_.equal_range(k); }
    size_type max_bucket_count() const { return ht_.max_bucket_count(); }
    size_type bucket(const key_type& k) const { return ht_.bucket(k); }
    size_type bucket_size(size_type n) const { return ht_.bucket_size(n); }

    size_type bucket_count()    const { return ht_.bucket_count(); }
    float     load_factor()     const { return ht_.load_factor(); }
    float     max_load_factor() const { return ht_.max_load_factor(); }
    void      max_load_factor(float f){ ht_.max_load_factor(f); }
    void      rehash(size_type n)     { ht_.rehash(n); }
    void      reserve(size_type n)    { ht_.reserve(n); }
    hasher    hash_function()   const { return ht_.hash_function(); }
    key_equal key_eq()          const { return ht_.key_eq(); }
    void      swap(unordered_map& r)  { ht_.swap(r.ht_); }
};

template<class Key, class T, class Hash = hash<Key>, class KeyEqual = equal_to<Key> >
class unordered_multimap {
    typedef std::pair<const Key, T> _value;
    struct _KOV { const Key& operator()(const _value& v) const { return v.first; } };
    typedef __ccw::hashtable<Key, _value, _KOV, Hash, KeyEqual, true> _ht;
    _ht ht_;
public:
    typedef Key         key_type;
    typedef T           mapped_type;
    typedef _value      value_type;
    typedef std::size_t size_type;
    typedef Hash        hasher;
    typedef KeyEqual    key_equal;
    typedef typename _ht::iterator       iterator;
    typedef typename _ht::const_iterator const_iterator;

    unordered_multimap() {}
    explicit unordered_multimap(size_type n) : ht_(n) {}
    template<class It> unordered_multimap(It f, It l) { ht_.insert(f, l); }

    iterator       begin()       { return ht_.begin(); }
    const_iterator begin() const { return ht_.begin(); }
    iterator       end()       { return ht_.end(); }
    const_iterator end() const { return ht_.end(); }

    bool      empty() const { return ht_.empty(); }
    size_type size()  const { return ht_.size(); }
    void      clear()       { ht_.clear(); }

    iterator insert(const value_type& v) { return ht_.insert(v).first; }
    template<class It> void insert(It f, It l) { ht_.insert(f, l); }
    size_type      erase(const key_type& k) { return ht_.erase(k); }
    iterator       erase(const_iterator pos) {
        typename _ht::iterator hit(pos.cur, pos.ht, pos.bkt);
        return ht_.erase(hit);
    }
    iterator       find(const key_type& k)       { return ht_.find(k); }
    const_iterator find(const key_type& k) const { return ht_.find(k); }
    size_type      count(const key_type& k) const { return ht_.count(k); }
    bool           contains(const key_type& k) const { return ht_.contains(k); }
    std::pair<iterator, iterator>             equal_range(const key_type& k)       { return ht_.equal_range(k); }
    std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const { return ht_.equal_range(k); }
    size_type max_bucket_count() const { return ht_.max_bucket_count(); }
    size_type bucket(const key_type& k) const { return ht_.bucket(k); }
    size_type bucket_size(size_type n) const { return ht_.bucket_size(n); }
    size_type bucket_count() const { return ht_.bucket_count(); }
    void      swap(unordered_multimap& r) { ht_.swap(r.ht_); }
};

template<class K, class T, class H, class E> inline void swap(unordered_map<K,T,H,E>& a, unordered_map<K,T,H,E>& b) { a.swap(b); }
template<class K, class T, class H, class E> inline void swap(unordered_multimap<K,T,H,E>& a, unordered_multimap<K,T,H,E>& b) { a.swap(b); }

}   // namespace std

#endif  // _MSC_VER < 1600

#endif  // _CCW_DETAIL_UNORDERED_MAP_HPP
