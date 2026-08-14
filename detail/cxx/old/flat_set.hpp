/**
 *  @file   _ccwrap_detail/flat_set.hpp
 *  @brief  std::flat_set backport (sorted unique vector adaptor).
 *  @license Boost Software License Version 1.0
 *  @note
 *      Keys are kept sorted and unique in a std::vector. Iterators are const
 *      (keys are immutable), matching the standard. sorted_unique construction,
 *      node handles, and heterogeneous (templated) lookup are not provided.
 */
#ifndef _CCW_DETAIL_FLAT_SET_HPP
#define _CCW_DETAIL_FLAT_SET_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <iterator>

namespace _CCW_STD23 {
using namespace ::std;

template<class Key, class Compare = std::less<Key>, class KeyContainer = std::vector<Key> >
class flat_set {
public:
    typedef Key            key_type;
    typedef Key            value_type;
    typedef KeyContainer   container_type;
    typedef Compare        key_compare;
    typedef Compare        value_compare;
    typedef typename KeyContainer::const_iterator          iterator;
    typedef typename KeyContainer::const_iterator          const_iterator;
    typedef typename KeyContainer::const_reverse_iterator  reverse_iterator;
    typedef typename KeyContainer::const_reverse_iterator  const_reverse_iterator;
    typedef typename KeyContainer::size_type               size_type;
    typedef typename KeyContainer::difference_type         difference_type;
    typedef const value_type&                              reference;
    typedef const value_type&                              const_reference;

private:
    container_type c_;
    key_compare    comp_;

    typename container_type::iterator lb_(const Key& k) {
        return std::lower_bound(c_.begin(), c_.end(), k, comp_);
    }

public:
    flat_set() {}
    explicit flat_set(const Compare& comp) : comp_(comp) {}
    template<class InputIt>
    flat_set(InputIt first, InputIt last, const Compare& comp = Compare()) : comp_(comp) { insert(first, last); }

    const_iterator begin()  const { return c_.begin(); }
    const_iterator end()    const { return c_.end(); }
    const_iterator cbegin() const { return c_.begin(); }
    const_iterator cend()   const { return c_.end(); }
    const_reverse_iterator rbegin() const { return c_.rbegin(); }
    const_reverse_iterator rend()   const { return c_.rend(); }

    bool      empty()    const { return c_.empty(); }
    size_type size()     const { return c_.size(); }
    size_type max_size() const { return c_.max_size(); }
    void      clear()          { c_.clear(); }

    const_iterator lower_bound(const Key& k) const { return std::lower_bound(c_.begin(), c_.end(), k, comp_); }
    const_iterator upper_bound(const Key& k) const { return std::upper_bound(c_.begin(), c_.end(), k, comp_); }
    const_iterator find(const Key& k) const {
        const_iterator it = lower_bound(k);
        return (it != c_.end() && !comp_(k, *it)) ? it : c_.end();
    }
    size_type count(const Key& k)    const { return find(k) != c_.end() ? 1 : 0; }
    bool      contains(const Key& k) const { return find(k) != c_.end(); }
    std::pair<const_iterator, const_iterator> equal_range(const Key& k) const {
        return std::make_pair(lower_bound(k), upper_bound(k));
    }

    std::pair<const_iterator, bool> insert(const value_type& v) {
        typename container_type::iterator it = lb_(v);
        if (it != c_.end() && !comp_(v, *it))
            return std::make_pair(const_iterator(it), false);
        it = c_.insert(it, v);
        return std::make_pair(const_iterator(it), true);
    }
    const_iterator insert(const_iterator, const value_type& v) { return insert(v).first; }
    template<class InputIt>
    void insert(InputIt first, InputIt last) { for (; first != last; ++first) insert(*first); }

    size_type erase(const Key& k) {
        typename container_type::iterator it = lb_(k);
        if (it != c_.end() && !comp_(k, *it)) { c_.erase(it); return 1; }
        return 0;
    }
    const_iterator erase(const_iterator pos) {
        difference_type d = std::distance<const_iterator>(c_.begin(), pos);
        typename container_type::iterator it = c_.begin();
        std::advance(it, d);
        return c_.erase(it);
    }

    void swap(flat_set& o) { c_.swap(o.c_); std::swap(comp_, o.comp_); }
    key_compare   key_comp()   const { return comp_; }
    value_compare value_comp() const { return comp_; }
};

template<class K, class C, class KC>
inline void swap(flat_set<K, C, KC>& a, flat_set<K, C, KC>& b) { a.swap(b); }

template<class K, class C, class KC>
inline bool operator==(const flat_set<K, C, KC>& a, const flat_set<K, C, KC>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}
template<class K, class C, class KC>
inline bool operator!=(const flat_set<K, C, KC>& a, const flat_set<K, C, KC>& b) { return !(a == b); }

}   // namespace _CCW_STD23

namespace _ccw { using _CCW_STD23::flat_set; }

#endif  /* _CCW_DETAIL_FLAT_SET_HPP */
