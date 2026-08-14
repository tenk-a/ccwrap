/**
 *  @file   _ccwrap_detail/flat_map.hpp
 *  @brief  std::flat_map backport (key-sorted vector-of-pairs adaptor).
 *  @license Boost Software License Version 1.0
 *  @note
 *      Practical, same-usage backport for the common map interface (operator[],
 *      at, find, insert, count, contains, lower/upper_bound, erase, iteration).
 *      Divergences from the standard, documented here: entries live in a single
 *      std::vector<pair<Key,T> > (not separate key/value containers), so
 *      value_type is pair<Key,T> with a *mutable* key and keys()/values() views
 *      are not provided. Only Key/T/Compare template parameters are exposed.
 */
#ifndef _CCW_DETAIL_FLAT_MAP_HPP
#define _CCW_DETAIL_FLAT_MAP_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <iterator>
#include <stdexcept>

namespace _CCW_STD23 {
using namespace ::std;

template<class Key, class T, class Compare = std::less<Key> >
class flat_map {
public:
    typedef Key                        key_type;
    typedef T                          mapped_type;
    typedef std::pair<Key, T>          value_type;
    typedef Compare                    key_compare;
    typedef std::vector<value_type>    container_type;
    typedef typename container_type::iterator               iterator;
    typedef typename container_type::const_iterator         const_iterator;
    typedef typename container_type::reverse_iterator       reverse_iterator;
    typedef typename container_type::const_reverse_iterator const_reverse_iterator;
    typedef typename container_type::size_type              size_type;
    typedef typename container_type::difference_type        difference_type;
    typedef value_type&                                     reference;
    typedef const value_type&                               const_reference;

private:
    struct vk_less {
        Compare c;
        vk_less() {}
        vk_less(const Compare& cc) : c(cc) {}
        bool operator()(const value_type& p, const Key& k) const { return c(p.first, k); }
        bool operator()(const Key& k, const value_type& p) const { return c(k, p.first); }
    };
    container_type c_;
    key_compare    comp_;

    iterator lb_(const Key& k) { return std::lower_bound(c_.begin(), c_.end(), k, vk_less(comp_)); }
    const_iterator lb_(const Key& k) const { return std::lower_bound(c_.begin(), c_.end(), k, vk_less(comp_)); }

public:
    flat_map() {}
    explicit flat_map(const Compare& comp) : comp_(comp) {}
    template<class InputIt>
    flat_map(InputIt first, InputIt last, const Compare& comp = Compare()) : comp_(comp) { insert(first, last); }

    iterator       begin()        { return c_.begin(); }
    const_iterator begin()  const { return c_.begin(); }
    iterator       end()          { return c_.end(); }
    const_iterator end()    const { return c_.end(); }
    const_iterator cbegin() const { return c_.begin(); }
    const_iterator cend()   const { return c_.end(); }
    reverse_iterator       rbegin()       { return c_.rbegin(); }
    const_reverse_iterator rbegin() const { return c_.rbegin(); }
    reverse_iterator       rend()         { return c_.rend(); }
    const_reverse_iterator rend()   const { return c_.rend(); }

    bool      empty()    const { return c_.empty(); }
    size_type size()     const { return c_.size(); }
    size_type max_size() const { return c_.max_size(); }
    void      clear()          { c_.clear(); }

    T& operator[](const Key& k) {
        iterator it = lb_(k);
        if (it != c_.end() && !comp_(k, it->first))
            return it->second;
        it = c_.insert(it, value_type(k, T()));
        return it->second;
    }
    T& at(const Key& k) {
        iterator it = lb_(k);
        if (it == c_.end() || comp_(k, it->first))
            throw std::out_of_range("flat_map::at");
        return it->second;
    }
    const T& at(const Key& k) const {
        const_iterator it = lb_(k);
        if (it == c_.end() || comp_(k, it->first))
            throw std::out_of_range("flat_map::at");
        return it->second;
    }

    iterator       find(const Key& k)       { iterator it = lb_(k); return (it != c_.end() && !comp_(k, it->first)) ? it : c_.end(); }
    const_iterator find(const Key& k) const { const_iterator it = lb_(k); return (it != c_.end() && !comp_(k, it->first)) ? it : c_.end(); }
    size_type count(const Key& k)    const { return find(k) != c_.end() ? 1 : 0; }
    bool      contains(const Key& k) const { return find(k) != c_.end(); }
    iterator       lower_bound(const Key& k)       { return lb_(k); }
    const_iterator lower_bound(const Key& k) const { return lb_(k); }
    iterator       upper_bound(const Key& k)       { return std::upper_bound(c_.begin(), c_.end(), k, vk_less(comp_)); }
    const_iterator upper_bound(const Key& k) const { return std::upper_bound(c_.begin(), c_.end(), k, vk_less(comp_)); }
    std::pair<iterator, iterator> equal_range(const Key& k) { return std::make_pair(lower_bound(k), upper_bound(k)); }
    std::pair<const_iterator, const_iterator> equal_range(const Key& k) const { return std::make_pair(lower_bound(k), upper_bound(k)); }

    std::pair<iterator, bool> insert(const value_type& v) {
        iterator it = lb_(v.first);
        if (it != c_.end() && !comp_(v.first, it->first))
            return std::make_pair(it, false);
        it = c_.insert(it, v);
        return std::make_pair(it, true);
    }
    iterator insert(const_iterator, const value_type& v) { return insert(v).first; }
    template<class InputIt>
    void insert(InputIt first, InputIt last) { for (; first != last; ++first) insert(*first); }

    // C++03 has no perfect forwarding, so try_emplace takes the mapped value the way
    // insert does; the point of the member is the "do not overwrite / do not construct
    // when present" semantics, and those are preserved.
    std::pair<iterator, bool> try_emplace(const Key& k, const T& v) {
        iterator it = lb_(k);
        if (it != c_.end() && !comp_(k, it->first))
            return std::make_pair(it, false);
        it = c_.insert(it, value_type(k, v));
        return std::make_pair(it, true);
    }
    iterator try_emplace(const_iterator, const Key& k, const T& v) { return try_emplace(k, v).first; }

    std::pair<iterator, bool> insert_or_assign(const Key& k, const T& v) {
        iterator it = lb_(k);
        if (it != c_.end() && !comp_(k, it->first)) { it->second = v; return std::make_pair(it, false); }
        it = c_.insert(it, value_type(k, v));
        return std::make_pair(it, true);
    }
    iterator insert_or_assign(const_iterator, const Key& k, const T& v) { return insert_or_assign(k, v).first; }

    size_type erase(const Key& k) {
        iterator it = lb_(k);
        if (it != c_.end() && !comp_(k, it->first)) { c_.erase(it); return 1; }
        return 0;
    }
    iterator erase(iterator pos) { return c_.erase(pos); }

    void        swap(flat_map& o) { c_.swap(o.c_); std::swap(comp_, o.comp_); }
    key_compare key_comp() const { return comp_; }
};

template<class K, class T, class C>
inline void swap(flat_map<K, T, C>& a, flat_map<K, T, C>& b) { a.swap(b); }

template<class K, class T, class C>
inline bool operator==(const flat_map<K, T, C>& a, const flat_map<K, T, C>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}
template<class K, class T, class C>
inline bool operator!=(const flat_map<K, T, C>& a, const flat_map<K, T, C>& b) { return !(a == b); }

}   // namespace _CCW_STD23

namespace _ccw { using _CCW_STD23::flat_map; }

#endif  /* _CCW_DETAIL_FLAT_MAP_HPP */
