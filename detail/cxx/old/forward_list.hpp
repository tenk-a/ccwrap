/**
 *  @file   detail/forward_list.hpp
 *  @brief  std::forward_list backport for pre-C++11 VC (vc8/9).
 *  @license Boost Software License Version 1.0
 *  @note
 *      Singly-linked list with a before-begin sentinel, in namespace std.
 *      Value semantics only (copy; no move on C++03). emplace_front / emplace_after
 *      use the fnctmpl variadic emulation. sort() gathers into a std::vector, sorts,
 *      and writes the values back (stable-ish, value semantics); merge splices nodes.
 */
#ifndef _CCW_DETAIL_FORWARD_LIST_HPP
#define _CCW_DETAIL_FORWARD_LIST_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <iterator>
#include <functional>
#include <algorithm>
#include <vector>
#include <type_traits>
#include "fnctmpl.hpp"

#if _MSC_VER < 1600   // vc8/9: no native <forward_list>

namespace std {

template<class T>
class forward_list {
    struct node_base { node_base* next; node_base() : next(0) {} };
    struct node : node_base { T value; node(const T& v) : value(v) {} };

    node_base head_;   // before_begin() sentinel; head_.next is the first element

    static node* as_node(node_base* p) { return static_cast<node*>(p); }

    void _clear_from(node_base* p) {
        node_base* c = p->next;
        while (c) { node_base* n = c->next; delete as_node(c); c = n; }
        p->next = 0;
    }
    void _copy_from(const forward_list& r) {
        node_base* tail = &head_;
        for (const node_base* s = r.head_.next; s; s = s->next) {
            node* nn = new node(static_cast<const node*>(s)->value);
            tail->next = nn; tail = nn;
        }
    }

public:
    typedef T           value_type;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    class const_iterator;
    class iterator {
        node_base* p_;
        friend class forward_list;
        friend class const_iterator;
    public:
        typedef std::forward_iterator_tag  iterator_category;
        typedef T                          value_type;
        typedef std::ptrdiff_t             difference_type;
        typedef T*                         pointer;
        typedef T&                         reference;
        iterator(node_base* p = 0) : p_(p) {}
        reference operator*()  const { return static_cast<node*>(p_)->value; }
        pointer   operator->() const { return &static_cast<node*>(p_)->value; }
        iterator& operator++()       { p_ = p_->next; return *this; }
        iterator  operator++(int)    { iterator t(*this); p_ = p_->next; return t; }
        bool operator==(const iterator& r) const { return p_ == r.p_; }
        bool operator!=(const iterator& r) const { return p_ != r.p_; }
    };
    class const_iterator {
        const node_base* p_;
        friend class forward_list;
    public:
        typedef std::forward_iterator_tag  iterator_category;
        typedef T                          value_type;
        typedef std::ptrdiff_t             difference_type;
        typedef const T*                   pointer;
        typedef const T&                   reference;
        const_iterator(const node_base* p = 0) : p_(p) {}
        const_iterator(const iterator& it) : p_(it.p_) {}
        const_reference operator*()  const { return static_cast<const node*>(p_)->value; }
        const T*        operator->() const { return &static_cast<const node*>(p_)->value; }
        const_iterator& operator++()    { p_ = p_->next; return *this; }
        const_iterator  operator++(int) { const_iterator t(*this); p_ = p_->next; return t; }
        bool operator==(const const_iterator& r) const { return p_ == r.p_; }
        bool operator!=(const const_iterator& r) const { return p_ != r.p_; }
    };

    forward_list() {}
    explicit forward_list(size_type n)             { for (size_type i = 0; i < n; ++i) push_front(T()); }
    forward_list(size_type n, const T& v)          { for (size_type i = 0; i < n; ++i) push_front(v); }
    template<class It> forward_list(It f, It l,
            typename std::enable_if<!std::is_integral<It>::value>::type* = 0) { _assign_it(f, l); }
    forward_list(const forward_list& r)            { _copy_from(r); }
    ~forward_list()                                { _clear_from(&head_); }

    forward_list& operator=(const forward_list& r) {
        if (this != &r) { _clear_from(&head_); _copy_from(r); }
        return *this;
    }

    void assign(size_type n, const T& v) { clear(); for (size_type i = 0; i < n; ++i) push_front(v); }
    template<class It> void assign(It f, It l,
            typename std::enable_if<!std::is_integral<It>::value>::type* = 0) { clear(); _assign_it(f, l); }

    reference       front()       { return as_node(head_.next)->value; }
    const_reference front() const { return static_cast<const node*>(head_.next)->value; }

    iterator       before_begin()       { return iterator(&head_); }
    const_iterator before_begin() const { return const_iterator(&head_); }
    const_iterator cbefore_begin() const{ return const_iterator(&head_); }
    iterator       begin()       { return iterator(head_.next); }
    const_iterator begin() const { return const_iterator(head_.next); }
    const_iterator cbegin() const{ return const_iterator(head_.next); }
    iterator       end()       { return iterator(0); }
    const_iterator end() const { return const_iterator(0); }
    const_iterator cend() const{ return const_iterator(0); }

    bool      empty()    const { return head_.next == 0; }
    size_type max_size() const { return size_type(-1) / sizeof(node); }
    void      clear()          { _clear_from(&head_); }

    void push_front(const T& v) { node* nn = new node(v); nn->next = head_.next; head_.next = nn; }
    void pop_front()            { node_base* f = head_.next; head_.next = f->next; delete as_node(f); }

    iterator insert_after(const_iterator pos, const T& v) {
        node_base* p = const_cast<node_base*>(pos.p_);
        node* nn = new node(v); nn->next = p->next; p->next = nn;
        return iterator(nn);
    }
    iterator insert_after(const_iterator pos, size_type n, const T& v) {
        node_base* p = const_cast<node_base*>(pos.p_);
        for (size_type i = 0; i < n; ++i) { node* nn = new node(v); nn->next = p->next; p->next = nn; p = nn; }
        return iterator(p);
    }
    template<class It> iterator insert_after(const_iterator pos, It f, It l,
            typename std::enable_if<!std::is_integral<It>::value>::type* = 0) {
        node_base* p = const_cast<node_base*>(pos.p_);
        for (; f != l; ++f) { node* nn = new node(*f); nn->next = p->next; p->next = nn; p = nn; }
        return iterator(p);
    }

    iterator erase_after(const_iterator pos) {
        node_base* p = const_cast<node_base*>(pos.p_);
        node_base* d = p->next;
        if (d) { p->next = d->next; delete as_node(d); }
        return iterator(p->next);
    }
    iterator erase_after(const_iterator first, const_iterator last) {
        node_base* p = const_cast<node_base*>(first.p_);
        node_base* stop = const_cast<node_base*>(last.p_);
        while (p->next != stop) { node_base* d = p->next; p->next = d->next; delete as_node(d); }
        return iterator(stop);
    }

    void resize(size_type n) { resize(n, T()); }
    void resize(size_type n, const T& v) {
        node_base* p = &head_; size_type i = 0;
        while (p->next && i < n) { p = p->next; ++i; }
        if (i == n) { _clear_from(p); }
        else { for (; i < n; ++i) { node* nn = new node(v); nn->next = 0; p->next = nn; p = nn; } }
    }

    void swap(forward_list& r) { node_base* t = head_.next; head_.next = r.head_.next; r.head_.next = t; }

    void remove(const T& v) {
        node_base* p = &head_;
        while (p->next) {
            if (as_node(p->next)->value == v) { node_base* d = p->next; p->next = d->next; delete as_node(d); }
            else p = p->next;
        }
    }
    template<class Pred> void remove_if(Pred pred) {
        node_base* p = &head_;
        while (p->next) {
            if (pred(as_node(p->next)->value)) { node_base* d = p->next; p->next = d->next; delete as_node(d); }
            else p = p->next;
        }
    }

    void unique() { unique(equal_to<T>()); }
    template<class Pred> void unique(Pred pred) {
        node_base* p = head_.next;
        while (p && p->next) {
            if (pred(as_node(p)->value, as_node(p->next)->value)) { node_base* d = p->next; p->next = d->next; delete as_node(d); }
            else p = p->next;
        }
    }

    void merge(forward_list& other) { merge(other, less<T>()); }
    template<class Comp> void merge(forward_list& other, Comp comp) {
        if (this == &other) return;
        node_base* p = &head_;
        while (p->next && other.head_.next) {
            if (comp(as_node(other.head_.next)->value, as_node(p->next)->value)) {
                node_base* b = other.head_.next; other.head_.next = b->next;
                b->next = p->next; p->next = b; p = b;
            } else { p = p->next; }
        }
        if (other.head_.next) { p->next = other.head_.next; other.head_.next = 0; }
    }

    void reverse() {
        node_base* prev = 0; node_base* cur = head_.next;
        while (cur) { node_base* nx = cur->next; cur->next = prev; prev = cur; cur = nx; }
        head_.next = prev;
    }

    void sort() { sort(less<T>()); }
    template<class Comp> void sort(Comp comp) {
        std::vector<T> v(begin(), end());
        std::sort(v.begin(), v.end(), comp);
        iterator it = begin();
        for (std::size_t i = 0; i < v.size(); ++i, ++it) *it = v[i];
    }

#if _CCW_NO_VARIADIC_TEMPLATES
    void emplace_front() { push_front(T()); }
#define _CCW_GEN_FL_EMPLACE(N)                                                            \
    template<_CCW_FNCTMPL_TMPLARG(N)> void emplace_front(_CCW_FNCTMPL_DECLARG(N))         \
        { push_front(T(_CCW_FNCTMPL_FNCARG(N))); }                                        \
    template<_CCW_FNCTMPL_TMPLARG(N)> iterator emplace_after(const_iterator pos, _CCW_FNCTMPL_DECLARG(N)) \
        { return insert_after(pos, T(_CCW_FNCTMPL_FNCARG(N))); }
    _CCW_GEN_FL_EMPLACE(1) _CCW_GEN_FL_EMPLACE(2) _CCW_GEN_FL_EMPLACE(3)
#undef _CCW_GEN_FL_EMPLACE
    iterator emplace_after(const_iterator pos) { return insert_after(pos, T()); }
#else
    template<class... A> void emplace_front(A&&... a) { push_front(T(static_cast<A&&>(a)...)); }
    template<class... A> iterator emplace_after(const_iterator pos, A&&... a) { return insert_after(pos, T(static_cast<A&&>(a)...)); }
#endif

private:
    template<class It> void _assign_it(It f, It l) {
        node_base* tail = &head_;
        for (; f != l; ++f) { node* nn = new node(*f); tail->next = nn; tail = nn; }
    }
};

template<class T> inline bool operator==(const forward_list<T>& a, const forward_list<T>& b) {
    typename forward_list<T>::const_iterator ia = a.begin(), ib = b.begin();
    for (; ia != a.end() && ib != b.end(); ++ia, ++ib) if (!(*ia == *ib)) return false;
    return ia == a.end() && ib == b.end();
}
template<class T> inline bool operator!=(const forward_list<T>& a, const forward_list<T>& b) { return !(a == b); }
template<class T> inline bool operator<(const forward_list<T>& a, const forward_list<T>& b) {
    typename forward_list<T>::const_iterator ia = a.begin(), ib = b.begin();
    for (; ia != a.end() && ib != b.end(); ++ia, ++ib) {
        if (*ia < *ib) return true;
        if (*ib < *ia) return false;
    }
    return (ia == a.end()) && (ib != b.end());
}
template<class T> inline void swap(forward_list<T>& a, forward_list<T>& b) { a.swap(b); }

}   // namespace std

#endif  // _MSC_VER < 1600

#endif  // _CCW_DETAIL_FORWARD_LIST_HPP
