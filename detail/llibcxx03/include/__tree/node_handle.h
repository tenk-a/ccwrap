// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: node handles for the tree/hash containers (C++17).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TREE_NODE_HANDLE_H
#define _CCW_LIBCPP___TREE_NODE_HANDLE_H
#include "../__config"
#include "../__utility/move.h"
#include "../__type_traits/is_class.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _T, bool = is_class<_T>::value> struct __ccw_is_class_gate {};   // non-class: no ::type
template <class _T> struct __ccw_is_class_gate<_T, true> { typedef void type; };
template <class _Vt, class _VT> struct __vins_gate {};                           // _Vt != value_type: no ::type
template <class _VT> struct __vins_gate<_VT, _VT> { typedef typename __ccw_is_class_gate<_VT>::type type; };

template <class _Node, class _NodeAlloc>
class __node_handle_base {
protected:
    struct __ptr_na : _NodeAlloc {
        mutable _Node* __ptr_;   // mutable: a const handle transfers on copy (see note)
        _CCW_LIBCPP_HIDE_FROM_ABI __ptr_na() : __ptr_(0) {}
        _CCW_LIBCPP_HIDE_FROM_ABI __ptr_na(_Node* __p, const _NodeAlloc& __a) : _NodeAlloc(__a), __ptr_(__p) {}
    };
    __ptr_na __pa_;
    _CCW_LIBCPP_HIDE_FROM_ABI _NodeAlloc&       __alloc()       { return __pa_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const _NodeAlloc& __alloc() const { return __pa_; }

    _CCW_LIBCPP_HIDE_FROM_ABI __node_handle_base() {}
    _CCW_LIBCPP_HIDE_FROM_ABI __node_handle_base(_Node* __p, const _NodeAlloc& __a) : __pa_(__p, __a) {}
    _CCW_LIBCPP_HIDE_FROM_ABI __node_handle_base(const __node_handle_base& __o) : __pa_(__o.__pa_.__ptr_, __o.__alloc()) { __o.__pa_.__ptr_ = 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI __node_handle_base& operator=(const __node_handle_base& __o) {
        if (this != &__o) { __free(); __alloc() = __o.__alloc(); __pa_.__ptr_ = __o.__pa_.__ptr_; __o.__pa_.__ptr_ = 0; }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __free() {
        if (__pa_.__ptr_) { __pa_.__ptr_->~_Node(); __alloc().deallocate(__pa_.__ptr_, 1); __pa_.__ptr_ = 0; }
    }
    ~__node_handle_base() { __free(); }

    typedef void (__node_handle_base::*__bool_type)() const;
    _CCW_LIBCPP_HIDE_FROM_ABI void __true_bool() const {}
public:
    _CCW_LIBCPP_HIDE_FROM_ABI bool empty() const { return __pa_.__ptr_ == 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator __bool_type() const { return __pa_.__ptr_ ? &__node_handle_base::__true_bool : 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI _NodeAlloc get_allocator() const { return __alloc(); }

    _CCW_LIBCPP_HIDE_FROM_ABI _Node* __release() const { _Node* __p = __pa_.__ptr_; __pa_.__ptr_ = 0; return __p; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Node* __get() const { return __pa_.__ptr_; }
};

template <class _Node, class _NodeAlloc, class _Value>
class __set_node_handle : public __node_handle_base<_Node, _NodeAlloc> {
    typedef __node_handle_base<_Node, _NodeAlloc> __base;
public:
    typedef _Value value_type;
    _CCW_LIBCPP_HIDE_FROM_ABI __set_node_handle() {}
    _CCW_LIBCPP_HIDE_FROM_ABI __set_node_handle(_Node* __p, const _NodeAlloc& __a) : __base(__p, __a) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Value& value() const { return this->__pa_.__ptr_->__value_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(__set_node_handle& __o) { _Node* __t = this->__pa_.__ptr_; this->__pa_.__ptr_ = __o.__pa_.__ptr_; __o.__pa_.__ptr_ = __t; }
};

template <class _Node, class _NodeAlloc, class _Key, class _Mapped>
class __map_node_handle : public __node_handle_base<_Node, _NodeAlloc> {
    typedef __node_handle_base<_Node, _NodeAlloc> __base;
public:
    typedef _Key    key_type;
    typedef _Mapped mapped_type;
    _CCW_LIBCPP_HIDE_FROM_ABI __map_node_handle() {}
    _CCW_LIBCPP_HIDE_FROM_ABI __map_node_handle(_Node* __p, const _NodeAlloc& __a) : __base(__p, __a) {}
    _CCW_LIBCPP_HIDE_FROM_ABI _Key&    key() const    { return const_cast<_Key&>(this->__pa_.__ptr_->__value_.first); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Mapped& mapped() const { return this->__pa_.__ptr_->__value_.second; }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(__map_node_handle& __o) { _Node* __t = this->__pa_.__ptr_; this->__pa_.__ptr_ = __o.__pa_.__ptr_; __o.__pa_.__ptr_ = __t; }
};

template <class _Iter, class _NodeType>
struct __insert_return_type {
    _Iter    position;
    bool     inserted;
    _NodeType node;
    _CCW_LIBCPP_HIDE_FROM_ABI __insert_return_type() : position(), inserted(false), node() {}
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___TREE_NODE_HANDLE_H
