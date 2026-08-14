//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: the ordered tree backing <set>/<map> for Open Watcom.
//  A RED-BLACK tree (CLRS): O(log n) worst case for find/insert/erase, as the
//  standard's complexity requirements demand. Node-based with parent pointers for
//  in-order iteration. A KeyOfValue functor lets set store the value itself and map
//  store a pair while keying on .first; both unique and multi insertion are here.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TREE_TREE_H
#define _CCW_LIBCPP___TREE_TREE_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
#include "../__memory/allocator.h"
#include "../__utility/move.h"
#include "../../cxx/ccw_check.hpp"
#include <cstddef>
#include <new>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct __tree_node_base {
    __tree_node_base* __parent_;
    __tree_node_base* __left_;
    __tree_node_base* __right_;
    bool              __black_;   // red-black colour; a null child counts as black
};

_CCW_LIBCPP_HIDE_FROM_ABI inline bool __tree_is_black(__tree_node_base* __x) { return !__x || __x->__black_; }
template <class _Vp> struct __tree_node : public __tree_node_base {
    _Vp __value_;
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_node(const _Vp& __v) : __value_(__v) {}
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_node(_CCW_RV_REF(_Vp) __v) : __value_(__v) {}
#endif
};

_CCW_LIBCPP_HIDE_FROM_ABI inline __tree_node_base* __tree_min(__tree_node_base* __x) { while (__x->__left_)  __x = __x->__left_;  return __x; }
_CCW_LIBCPP_HIDE_FROM_ABI inline __tree_node_base* __tree_max(__tree_node_base* __x) { while (__x->__right_) __x = __x->__right_; return __x; }

_CCW_LIBCPP_HIDE_FROM_ABI inline __tree_node_base* __tree_next(__tree_node_base* __x) {
    if (__x->__right_) return __tree_min(__x->__right_);
    while (__x->__parent_ && __x == __x->__parent_->__right_) __x = __x->__parent_;
    return __x->__parent_;
}
_CCW_LIBCPP_HIDE_FROM_ABI inline __tree_node_base* __tree_prev(__tree_node_base* __x) {
    if (__x->__left_) return __tree_max(__x->__left_);
    while (__x->__parent_ && __x == __x->__parent_->__left_) __x = __x->__parent_;
    return __x->__parent_;
}

template <class _Vp, class _Ref, class _Ptr>
class __tree_iterator {
    __tree_node_base* __ptr_;
public:
    typedef bidirectional_iterator_tag iterator_category;
    typedef _Vp                        value_type;
    typedef _CCW_STD::ptrdiff_t             difference_type;
    typedef _Ptr                       pointer;
    typedef _Ref                       reference;

    _CCW_LIBCPP_HIDE_FROM_ABI __tree_iterator() : __ptr_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit __tree_iterator(__tree_node_base* __p) : __ptr_(__p) {}
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_iterator(const __tree_iterator<_Vp, _Vp&, _Vp*>& __o) : __ptr_(__o.__node()) {}
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_node_base* __node() const { return __ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference operator*()  const { return static_cast<__tree_node<_Vp>*>(__ptr_)->__value_; }
    _CCW_LIBCPP_HIDE_FROM_ABI pointer   operator->() const { return &static_cast<__tree_node<_Vp>*>(__ptr_)->__value_; }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_iterator& operator++()    { __ptr_ = _CCW_STD::__tree_next(__ptr_); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_iterator  operator++(int) { __tree_iterator __t = *this; ++*this; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_iterator& operator--()    { __ptr_ = _CCW_STD::__tree_prev(__ptr_); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_iterator  operator--(int) { __tree_iterator __t = *this; --*this; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const __tree_iterator& __o) const { return __ptr_ == __o.__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const __tree_iterator& __o) const { return __ptr_ != __o.__ptr_; }
};

template <class _Key, class _Vp, class _KoV, class _Compare, class _Alloc = allocator<_Vp> >
class __tree {
public:
    typedef _Vp                                            value_type;
    typedef _CCW_STD::size_t                                    size_type;
    typedef _Alloc                                         allocator_type;
    typedef __tree_iterator<_Vp, _Vp&, _Vp*>               iterator;
    typedef __tree_iterator<_Vp, const _Vp&, const _Vp*>   const_iterator;
private:
    typedef __tree_node<_Vp>                                        __node;
    typedef typename _Alloc::template rebind<__node>::other         __node_allocator;

    __tree_node_base __end_;   // sentinel; __end_.__left_ is the root; begin = leftmost
    _Compare         __comp_;
    _KoV             __kov_;
    struct __na_sz : __node_allocator {
        size_type __size_;
        _CCW_LIBCPP_HIDE_FROM_ABI __na_sz() : __size_(0) {}
        _CCW_LIBCPP_HIDE_FROM_ABI explicit __na_sz(const __node_allocator& __a) : __node_allocator(__a), __size_(0) {}
    };
    __na_sz __nas_;

    _CCW_LIBCPP_HIDE_FROM_ABI __node* __new_node(const _Vp& __v) {
        __node* __p = __node_alloc().allocate(1);
        _CCW_TRY {
            ::new (static_cast<void*>(__p)) __node(__v);
        } _CCW_CATCH_ALL {
            __node_alloc().deallocate(__p, 1);
            _CCW_RETHROW;
        }
        return __p;
    }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI __node* __new_node(_CCW_RV_REF(_Vp) __v) {
        __node* __p = __node_alloc().allocate(1);
        _CCW_TRY {
            ::new (static_cast<void*>(__p)) __node(__v);
        } _CCW_CATCH_ALL {
            __node_alloc().deallocate(__p, 1);
            _CCW_RETHROW;
        }
        return __p;
    }
#endif
    _CCW_LIBCPP_HIDE_FROM_ABI void __delete_node(__tree_node_base* __n) {
        __node* __p = static_cast<__node*>(__n);
        __p->~__node();
        __node_alloc().deallocate(__p, 1);
    }

    _CCW_LIBCPP_HIDE_FROM_ABI __tree_node_base* __root() const { return __end_.__left_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __init() { __end_.__parent_ = 0; __end_.__left_ = 0; __end_.__right_ = 0; __end_.__black_ = true; __nas_.__size_ = 0; }

    _CCW_LIBCPP_HIDE_FROM_ABI void __steal(__tree& __o) {
        __end_.__left_ = __o.__end_.__left_;
        if (__end_.__left_) __end_.__left_->__parent_ = &__end_;
        __comp_ = __o.__comp_;
        __kov_  = __o.__kov_;
        __node_alloc() = __o.__node_alloc();   // clobbers __nas_.__size_ low byte...
        __nas_.__size_ = __o.__nas_.__size_;   // ...repaired here
        __o.__init();
    }

    _CCW_LIBCPP_HIDE_FROM_ABI void __rotate_left(__tree_node_base* __x) {
        __tree_node_base* __y = __x->__right_;
        __x->__right_ = __y->__left_;
        if (__y->__left_) __y->__left_->__parent_ = __x;
        __y->__parent_ = __x->__parent_;
        if (__x->__parent_ == &__end_)               __end_.__left_ = __y;
        else if (__x == __x->__parent_->__left_)     __x->__parent_->__left_ = __y;
        else                                         __x->__parent_->__right_ = __y;
        __y->__left_ = __x;
        __x->__parent_ = __y;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __rotate_right(__tree_node_base* __x) {
        __tree_node_base* __y = __x->__left_;
        __x->__left_ = __y->__right_;
        if (__y->__right_) __y->__right_->__parent_ = __x;
        __y->__parent_ = __x->__parent_;
        if (__x->__parent_ == &__end_)               __end_.__left_ = __y;
        else if (__x == __x->__parent_->__right_)    __x->__parent_->__right_ = __y;
        else                                         __x->__parent_->__left_ = __y;
        __y->__right_ = __x;
        __x->__parent_ = __y;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI void __insert_fixup(__tree_node_base* __z) {
        while (__z->__parent_ != &__end_ && !__z->__parent_->__black_) {
            __tree_node_base* __p = __z->__parent_;
            __tree_node_base* __g = __p->__parent_;      // exists: a red parent is never the root
            if (__p == __g->__left_) {
                __tree_node_base* __u = __g->__right_;
                if (!__tree_is_black(__u)) {             // red uncle: recolour and move up
                    __p->__black_ = true; __u->__black_ = true; __g->__black_ = false;
                    __z = __g;
                } else {
                    if (__z == __p->__right_) { __z = __p; __rotate_left(__z); __p = __z->__parent_; }
                    __p->__black_ = true; __g->__black_ = false;
                    __rotate_right(__g);
                }
            } else {
                __tree_node_base* __u = __g->__left_;
                if (!__tree_is_black(__u)) {
                    __p->__black_ = true; __u->__black_ = true; __g->__black_ = false;
                    __z = __g;
                } else {
                    if (__z == __p->__left_) { __z = __p; __rotate_right(__z); __p = __z->__parent_; }
                    __p->__black_ = true; __g->__black_ = false;
                    __rotate_left(__g);
                }
            }
        }
        if (__root()) __root()->__black_ = true;         // the root is always black
    }

    _CCW_LIBCPP_HIDE_FROM_ABI void __erase_fixup(__tree_node_base* __x, __tree_node_base* __xp) {
        while (__x != __root() && __tree_is_black(__x)) {
            if (__x == __xp->__left_) {
                __tree_node_base* __w = __xp->__right_;
                if (!__tree_is_black(__w)) {             // red sibling: rotate it away
                    __w->__black_ = true; __xp->__black_ = false;
                    __rotate_left(__xp);
                    __w = __xp->__right_;
                }
                if (__tree_is_black(__w->__left_) && __tree_is_black(__w->__right_)) {
                    __w->__black_ = false;               // push the deficit up
                    __x = __xp; __xp = __x->__parent_;
                } else {
                    if (__tree_is_black(__w->__right_)) {
                        if (__w->__left_) __w->__left_->__black_ = true;
                        __w->__black_ = false;
                        __rotate_right(__w);
                        __w = __xp->__right_;
                    }
                    __w->__black_ = __xp->__black_;
                    __xp->__black_ = true;
                    if (__w->__right_) __w->__right_->__black_ = true;
                    __rotate_left(__xp);
                    __x = __root();
                    break;
                }
            } else {
                __tree_node_base* __w = __xp->__left_;
                if (!__tree_is_black(__w)) {
                    __w->__black_ = true; __xp->__black_ = false;
                    __rotate_right(__xp);
                    __w = __xp->__left_;
                }
                if (__tree_is_black(__w->__right_) && __tree_is_black(__w->__left_)) {
                    __w->__black_ = false;
                    __x = __xp; __xp = __x->__parent_;
                } else {
                    if (__tree_is_black(__w->__left_)) {
                        if (__w->__right_) __w->__right_->__black_ = true;
                        __w->__black_ = false;
                        __rotate_left(__w);
                        __w = __xp->__left_;
                    }
                    __w->__black_ = __xp->__black_;
                    __xp->__black_ = true;
                    if (__w->__left_) __w->__left_->__black_ = true;
                    __rotate_right(__xp);
                    __x = __root();
                    break;
                }
            }
        }
        if (__x) __x->__black_ = true;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator __attach(__tree_node<_Vp>* __n, __tree_node_base* __parent, bool __left) {
        __n->__parent_ = __parent; __n->__left_ = 0; __n->__right_ = 0;
        __n->__black_ = false;                            // new nodes start red
        if (__parent == &__end_)  __end_.__left_ = __n;   // root
        else if (__left)          __parent->__left_ = __n;
        else                      __parent->__right_ = __n;
        ++__nas_.__size_;
        __insert_fixup(__n);
        return iterator(__n);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __destroy(__tree_node_base* __n) {
        if (!__n) return;
        __destroy(__n->__left_); __destroy(__n->__right_);
        __delete_node(__n);
    }
private:
    _CCW_LIBCPP_HIDE_FROM_ABI void __copy_from(const __tree& __o) {
        for (const_iterator __i = __o.begin(); __i != __o.end(); ++__i) __insert_multi(*__i);
    }
public:
    _CCW_LIBCPP_HIDE_FROM_ABI __tree() { __init(); }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit __tree(const _Compare& __c) : __comp_(__c) { __init(); }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit __tree(const _Alloc& __a) : __nas_(__a) { __init(); }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree(const _Compare& __c, const _Alloc& __a) : __comp_(__c), __nas_(__a) { __init(); }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree(const __tree& __o) : __comp_(__o.__comp_), __nas_(__o.__node_alloc()) { __init(); __copy_from(__o); }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree(const __tree& __o, const _Alloc& __a) : __comp_(__o.__comp_), __nas_(__a) { __init(); __copy_from(__o); }
    _CCW_LIBCPP_HIDE_FROM_ABI const _Compare& __comp() const { return __comp_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Alloc __alloc() const { return _Alloc(__node_alloc()); }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree& operator=(const __tree& __o) {
        if (this != &__o) {
            clear();                       // frees through the OLD allocator, before it changes
            __comp_ = __o.__comp_;
            __copy_from(__o);
        }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI ~__tree() { clear(); }

#if _CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI __tree(__tree&& __o) { __init(); __steal(__o); }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree& operator=(__tree&& __o) {
        if (this != &__o) { clear(); __steal(__o); }
        return *this;
    }
#else // !_CCW_HAS_REAL_RVALUE_REF -- the same through the C++03 emulation (detail/move03.hpp)
    _CCW_LIBCPP_HIDE_FROM_ABI __tree(_CCW_RV_REF(__tree) __r) { __init(); __steal(_CCW_MOVED_OBJ(__r)); }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree& operator=(_CCW_RV_REF(__tree) __r) {
        __tree& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) { clear(); __steal(__o); }
        return *this;
    }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI iterator       begin()       { return iterator(__root() ? _CCW_STD::__tree_min(__root()) : &__end_); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator begin() const { return const_iterator(__end_.__left_ ? _CCW_STD::__tree_min(const_cast<__tree_node_base*>(__end_.__left_)) : const_cast<__tree_node_base*>(&__end_)); }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator       end()         { return iterator(&__end_); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator end() const   { return const_iterator(const_cast<__tree_node_base*>(&__end_)); }

    _CCW_LIBCPP_HIDE_FROM_ABI bool      empty() const { return __nas_.__size_ == 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type size() const  { return __nas_.__size_; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type max_size() const { return size_type(-1) / sizeof(__tree_node<_Vp>); }
    _CCW_LIBCPP_HIDE_FROM_ABI void clear() { __destroy(__root()); __init(); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(__tree& __o) {
        __tree_node_base* __t = __end_.__left_; __end_.__left_ = __o.__end_.__left_; __o.__end_.__left_ = __t;
        _Compare __tc = __comp_; __comp_ = __o.__comp_; __o.__comp_ = __tc;
        __na_sz __tt = __nas_; __nas_ = __o.__nas_; __o.__nas_ = __tt;   // size + allocator together
        if (__end_.__left_)   __end_.__left_->__parent_ = &__end_;
        if (__o.__end_.__left_) __o.__end_.__left_->__parent_ = &__o.__end_;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator find(const _Key& __k) {
        __tree_node_base* __x = __root();
        _CCW_CHK_INIT(__steps);
        while (__x) {
            _CCW_CHK_STEP(__steps, __nas_.__size_, "tree find: cycle or bad node", this, __x);
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk)) __x = __x->__left_;
            else if (__comp_(__xk, __k)) __x = __x->__right_;
            else return iterator(__x);
        }
        return end();
    }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator find(const _Key& __k) const {
        __tree_node_base* __x = const_cast<__tree_node_base*>(__end_.__left_);
        _CCW_CHK_INIT(__steps);
        while (__x) {
            _CCW_CHK_STEP(__steps, __nas_.__size_, "tree find: cycle or bad node", this, __x);
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk)) __x = __x->__left_;
            else if (__comp_(__xk, __k)) __x = __x->__right_;
            else return const_iterator(__x);
        }
        return end();
    }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type count(const _Key& __k) const { return find(__k) == end() ? 0 : 1; }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_unique(const _Vp& __v, bool& __inserted) {
        const _Key& __k = __kov_(__v);
        __tree_node_base* __parent = &__end_;
        __tree_node_base* __x = __root();
        bool __left = true;
        while (__x) {
            __parent = __x;
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk))      { __x = __x->__left_;  __left = true; }
            else if (__comp_(__xk, __k)) { __x = __x->__right_; __left = false; }
            else { __inserted = false; return iterator(__x); }
        }
        __inserted = true;
        return __attach(__new_node(__v), __parent, __left);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(const _Vp& __v) { bool __b; return __insert_unique(__v, __b); }

#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_unique(_CCW_RV_REF(_Vp) __v, bool& __inserted) {
        const _Key& __k = __kov_(__v);
        __tree_node_base* __parent = &__end_;
        __tree_node_base* __x = __root();
        bool __left = true;
        while (__x) {
            __parent = __x;
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk))      { __x = __x->__left_;  __left = true; }
            else if (__comp_(__xk, __k)) { __x = __x->__right_; __left = false; }
            else { __inserted = false; return iterator(__x); }   // __v is NOT consumed
        }
        __inserted = true;
        return __attach(__new_node(__v), __parent, __left);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(_CCW_RV_REF(_Vp) __v) { bool __b; return __insert_unique(__v, __b); }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_multi(const _Vp& __v) {
        const _Key& __k = __kov_(__v);
        __tree_node_base* __parent = &__end_;
        __tree_node_base* __x = __root();
        bool __left = true;
        while (__x) {
            __parent = __x;
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk)) { __x = __x->__left_; __left = true; }
            else                    { __x = __x->__right_; __left = false; }
        }
        return __attach(__new_node(__v), __parent, __left);
    }
#if !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_multi(_CCW_RV_REF(_Vp) __v) {
        const _Key& __k = __kov_(__v);
        __tree_node_base* __parent = &__end_;
        __tree_node_base* __x = __root();
        bool __left = true;
        while (__x) {
            __parent = __x;
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk)) { __x = __x->__left_; __left = true; }
            else                    { __x = __x->__right_; __left = false; }
        }
        return __attach(__new_node(__v), __parent, __left);
    }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI __tree_node_base* __lower(const _Key& __k) const {
        __tree_node_base* __x = const_cast<__tree_node_base*>(__end_.__left_);
        __tree_node_base* __r = const_cast<__tree_node_base*>(&__end_);
        while (__x) {
            if (!__comp_(__kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_), __k)) { __r = __x; __x = __x->__left_; }
            else __x = __x->__right_;
        }
        return __r;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI __tree_node_base* __upper(const _Key& __k) const {
        __tree_node_base* __x = const_cast<__tree_node_base*>(__end_.__left_);
        __tree_node_base* __r = const_cast<__tree_node_base*>(&__end_);
        while (__x) {
            if (__comp_(__k, __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_))) { __r = __x; __x = __x->__left_; }
            else __x = __x->__right_;
        }
        return __r;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator       lower_bound(const _Key& __k)       { return iterator(__lower(__k)); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator lower_bound(const _Key& __k) const { return const_iterator(__lower(__k)); }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator       upper_bound(const _Key& __k)       { return iterator(__upper(__k)); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator upper_bound(const _Key& __k) const { return const_iterator(__upper(__k)); }

    _CCW_LIBCPP_HIDE_FROM_ABI size_type __count_multi(const _Key& __k) const {
        size_type __n = 0;
        for (__tree_node_base* __x = __lower(__k); __x != &__end_ && __x != __upper(__k); __x = _CCW_STD::__tree_next(__x)) ++__n;
        return __n;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI size_type __height() const { return __height_of(__root()); }

    _CCW_LIBCPP_HIDE_FROM_ABI bool __rb_valid() const {
        if (!__root()) return __nas_.__size_ == 0;
        if (!__root()->__black_) return false;                       // the root must be black
        if (__root()->__parent_ != &__end_) return false;
        int __bh = -1;
        size_type __n = 0;
        return __check(__root(), 0, __bh, __n) && __n == __nas_.__size_;
    }

private:
    _CCW_LIBCPP_HIDE_FROM_ABI size_type __height_of(__tree_node_base* __x) const {
        if (!__x) return 0;
        size_type __l = __height_of(__x->__left_), __r = __height_of(__x->__right_);
        return 1 + (__l > __r ? __l : __r);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool __check(__tree_node_base* __x, int __blacks, int& __bh, size_type& __n) const {
        if (!__x) {                                                  // null = black leaf
            if (__bh < 0) __bh = __blacks + 1;
            return __bh == __blacks + 1;
        }
        ++__n;
        if (!__x->__black_) {                                        // red: children must be black
            if (!__tree_is_black(__x->__left_) || !__tree_is_black(__x->__right_)) return false;
        }
        const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
        if (__x->__left_) {
            if (__x->__left_->__parent_ != __x) return false;
            if (__comp_(__xk, __kov_(static_cast<__tree_node<_Vp>*>(__x->__left_)->__value_))) return false;
        }
        if (__x->__right_) {
            if (__x->__right_->__parent_ != __x) return false;
            if (__comp_(__kov_(static_cast<__tree_node<_Vp>*>(__x->__right_)->__value_), __xk)) return false;
        }
        const int __b = __blacks + (__x->__black_ ? 1 : 0);
        return __check(__x->__left_, __b, __bh, __n) && __check(__x->__right_, __b, __bh, __n);
    }
public:

    _CCW_LIBCPP_HIDE_FROM_ABI size_type erase(const _Key& __k) {
        iterator __it = find(__k);
        if (__it == end()) return 0;
        __erase_node(__it.__node());
        return 1;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type __erase_multi(const _Key& __k) {
        size_type __n = 0;
        iterator __it = lower_bound(__k);
        while (__it != end() && !__comp_(__k, __kov_(*__it))) {
            __tree_node_base* __z = __it.__node(); ++__it; __erase_node(__z); ++__n;
        }
        return __n;
    }
    template <class _It>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator erase(_It __pos) {
        __tree_node_base* __z = __pos.__node();
        __tree_node_base* __nx = _CCW_STD::__tree_next(__z);
        __erase_node(__z);
        return iterator(__nx);
    }

private:
    _CCW_LIBCPP_HIDE_FROM_ABI void __replace(__tree_node_base* __u, __tree_node_base* __v) {
        if (__u->__parent_ == &__end_)           __end_.__left_ = __v;
        else if (__u == __u->__parent_->__left_) __u->__parent_->__left_ = __v;
        else                                     __u->__parent_->__right_ = __v;
        if (__v) __v->__parent_ = __u->__parent_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __erase_node(__tree_node_base* __z) {
        __detach_node(__z);
        __delete_node(__z);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __detach_node(__tree_node_base* __z) {
        __tree_node_base* __x  = 0;
        __tree_node_base* __xp = 0;
        bool __removed_black;

        if (!__z->__left_) {
            __removed_black = __z->__black_;
            __x  = __z->__right_;
            __xp = __z->__parent_;
            __replace(__z, __z->__right_);
        } else if (!__z->__right_) {
            __removed_black = __z->__black_;
            __x  = __z->__left_;
            __xp = __z->__parent_;
            __replace(__z, __z->__left_);
        } else {
            __tree_node_base* __y = _CCW_STD::__tree_min(__z->__right_);   // successor
            __removed_black = __y->__black_;
            __x = __y->__right_;
            if (__y->__parent_ == __z) {
                __xp = __y;                                           // __x hangs off __y
            } else {
                __xp = __y->__parent_;
                __replace(__y, __y->__right_);
                __y->__right_ = __z->__right_;
                __y->__right_->__parent_ = __y;
            }
            __replace(__z, __y);
            __y->__left_ = __z->__left_;
            __y->__left_->__parent_ = __y;
            __y->__black_ = __z->__black_;                            // __y wears __z's colour
        }

        --__nas_.__size_;

        if (__removed_black) __erase_fixup(__x, __xp);
    }

public:
    typedef __node           __node_t;
    typedef __node_allocator __node_alloc_t;
    _CCW_LIBCPP_HIDE_FROM_ABI __node_allocator&       __node_alloc()       { return __nas_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const __node_allocator& __node_alloc() const { return __nas_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __free_node(__node* __n) { __delete_node(__n); }

    _CCW_LIBCPP_HIDE_FROM_ABI __node* __extract_key(const _Key& __k) {
        iterator __it = find(__k);
        if (__it == end()) return 0;
        __tree_node_base* __z = __it.__node();
        __detach_node(__z);
        return static_cast<__node*>(__z);
    }
    template <class _It>
    _CCW_LIBCPP_HIDE_FROM_ABI __node* __extract_iter(_It __pos) {
        __tree_node_base* __z = __pos.__node();
        __detach_node(__z);
        return static_cast<__node*>(__z);
    }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_unique_node(__node* __n, bool& __inserted, __tree_node_base*& __existing) {
        const _Key& __k = __kov_(__n->__value_);
        __tree_node_base* __parent = &__end_;
        __tree_node_base* __x = __root();
        bool __left = true;
        while (__x) {
            __parent = __x;
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk))      { __x = __x->__left_;  __left = true; }
            else if (__comp_(__xk, __k)) { __x = __x->__right_; __left = false; }
            else { __inserted = false; __existing = __x; return iterator(__x); }
        }
        __inserted = true; __existing = 0;
        return __attach(__n, __parent, __left);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_multi_node(__node* __n) {
        const _Key& __k = __kov_(__n->__value_);
        __tree_node_base* __parent = &__end_;
        __tree_node_base* __x = __root();
        bool __left = true;
        while (__x) {
            __parent = __x;
            const _Key& __xk = __kov_(static_cast<__tree_node<_Vp>*>(__x)->__value_);
            if (__comp_(__k, __xk)) { __x = __x->__left_; __left = true; }
            else                    { __x = __x->__right_; __left = false; }
        }
        return __attach(__n, __parent, __left);
    }
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___TREE_TREE_H
