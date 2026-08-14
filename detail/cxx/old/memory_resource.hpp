/**
 *  @file   _ccwrap_detail/memory_resource.hpp
 *  @brief  std::pmr backport for pre-C++17 (C++03..C++14).
 *  @license Boost Software License Version 1.0
 *  @note
 *      polymorphic_allocator exposes the classic (C++03) allocator interface
 *      (pointer/rebind/construct/destroy/address/max_size) so it works as an
 *      allocator for the old MSVC containers. construct() uses the fnctmpl
 *      variadic emulation (0..6 args). monotonic_buffer_resource is a real bump
 *      allocator with geometric upstream growth; the pool resources delegate to
 *      their upstream (a functional approximation, not pooled).
 */
#ifndef _CCW_DETAIL_MEMORY_RESOURCE_HPP
#define _CCW_DETAIL_MEMORY_RESOURCE_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <new>
#include "../fnctmpl.hpp"

namespace _CCW_STD17 {
using namespace ::std;
namespace pmr {

namespace __ccw_mr {
    // default alignment (~ alignof(max_align_t)): 16 on LLP64/x64, 8 on 32-bit.
    const std::size_t max_align = (sizeof(void*) >= 8) ? 16 : 8;

    inline std::size_t align_up(std::size_t n, std::size_t a) {
        return (n + (a - 1)) & ~(a - 1);
    }
}

// --- memory_resource ---------------------------------------------------------
class memory_resource {
public:
    virtual ~memory_resource() {}
    void* allocate(std::size_t bytes, std::size_t align = __ccw_mr::max_align) {
        return do_allocate(bytes, align);
    }
    void deallocate(void* p, std::size_t bytes, std::size_t align = __ccw_mr::max_align) {
        do_deallocate(p, bytes, align);
    }
    bool is_equal(const memory_resource& other) const { return do_is_equal(other); }
protected:
    virtual void* do_allocate(std::size_t bytes, std::size_t align) = 0;
    virtual void  do_deallocate(void* p, std::size_t bytes, std::size_t align) = 0;
    virtual bool  do_is_equal(const memory_resource& other) const = 0;
};

inline bool operator==(const memory_resource& a, const memory_resource& b) {
    return &a == &b || a.is_equal(b);
}
inline bool operator!=(const memory_resource& a, const memory_resource& b) {
    return !(a == b);
}

// --- new_delete_resource / null_memory_resource ------------------------------
namespace __ccw_mr {
    class new_delete_res : public memory_resource {
    protected:
        virtual void* do_allocate(std::size_t bytes, std::size_t) {
            return ::operator new(bytes ? bytes : 1);
        }
        virtual void do_deallocate(void* p, std::size_t, std::size_t) {
            ::operator delete(p);
        }
        virtual bool do_is_equal(const memory_resource& o) const { return this == &o; }
    };
    class null_res : public memory_resource {
    protected:
        virtual void* do_allocate(std::size_t, std::size_t) { throw std::bad_alloc(); }
        virtual void do_deallocate(void*, std::size_t, std::size_t) {}
        virtual bool do_is_equal(const memory_resource& o) const { return this == &o; }
    };
}

inline memory_resource* new_delete_resource() {
    static __ccw_mr::new_delete_res r;
    return &r;
}
inline memory_resource* null_memory_resource() {
    static __ccw_mr::null_res r;
    return &r;
}

namespace __ccw_mr {
    inline memory_resource*& default_slot() {
        static memory_resource* p = new_delete_resource();
        return p;
    }
}
inline memory_resource* get_default_resource() { return __ccw_mr::default_slot(); }
inline memory_resource* set_default_resource(memory_resource* r) {
    memory_resource*& slot = __ccw_mr::default_slot();
    memory_resource* old = slot;
    slot = r ? r : new_delete_resource();
    return old;
}

// --- monotonic_buffer_resource -----------------------------------------------
class monotonic_buffer_resource : public memory_resource {
    struct chunk { chunk* prev; std::size_t size; };   /* header; data follows */
    memory_resource* upstream_;
    char*            cur_;
    char*            end_;
    chunk*           head_;      /* upstream-allocated chunk list */
    void*            init_buf_;
    std::size_t      init_size_;
    std::size_t      next_size_;

    monotonic_buffer_resource(const monotonic_buffer_resource&);
    monotonic_buffer_resource& operator=(const monotonic_buffer_resource&);

    void new_chunk(std::size_t need, std::size_t align) {
        std::size_t want = next_size_;
        std::size_t hdr  = __ccw_mr::align_up(sizeof(chunk), __ccw_mr::max_align);
        if (want < need + align) want = need + align;
        std::size_t total = hdr + want;
        chunk* c = static_cast<chunk*>(upstream_->allocate(total, __ccw_mr::max_align));
        c->prev = head_;
        c->size = total;
        head_ = c;
        cur_  = reinterpret_cast<char*>(c) + hdr;
        end_  = reinterpret_cast<char*>(c) + total;
        if (next_size_ < (std::size_t(-1) / 2)) next_size_ *= 2;
    }

public:
    explicit monotonic_buffer_resource(memory_resource* up = get_default_resource())
        : upstream_(up), cur_(0), end_(0), head_(0),
          init_buf_(0), init_size_(0), next_size_(1024 > 0 ? 1024 : 1024) {}
    explicit monotonic_buffer_resource(std::size_t initial_size, memory_resource* up = get_default_resource())
        : upstream_(up), cur_(0), end_(0), head_(0),
          init_buf_(0), init_size_(0), next_size_(initial_size ? initial_size : 1) {}
    monotonic_buffer_resource(void* buffer, std::size_t size, memory_resource* up = get_default_resource())
        : upstream_(up), cur_(static_cast<char*>(buffer)), end_(static_cast<char*>(buffer) + size),
          head_(0), init_buf_(buffer), init_size_(size), next_size_(size ? size : 1) {}
    ~monotonic_buffer_resource() { release(); }

    void release() {
        while (head_) {
            chunk* p = head_->prev;
            upstream_->deallocate(head_, head_->size, __ccw_mr::max_align);
            head_ = p;
        }
        cur_ = static_cast<char*>(init_buf_);
        end_ = cur_ ? (cur_ + init_size_) : 0;
    }
    memory_resource* upstream_resource() const { return upstream_; }

protected:
    virtual void* do_allocate(std::size_t bytes, std::size_t align) {
        if (align == 0) align = 1;
        char* p = reinterpret_cast<char*>(__ccw_mr::align_up(reinterpret_cast<std::size_t>(cur_), align));
        if (p + bytes > end_ || cur_ == 0) {
            new_chunk(bytes, align);
            p = reinterpret_cast<char*>(__ccw_mr::align_up(reinterpret_cast<std::size_t>(cur_), align));
        }
        cur_ = p + bytes;
        return p;
    }
    virtual void do_deallocate(void*, std::size_t, std::size_t) {}   /* no-op (freed at release) */
    virtual bool do_is_equal(const memory_resource& o) const { return this == &o; }
};

// --- pool resources (approximation: delegate to upstream) --------------------
struct pool_options {
    std::size_t max_blocks_per_chunk;
    std::size_t largest_required_pool_block;
    pool_options() : max_blocks_per_chunk(0), largest_required_pool_block(0) {}
};

class unsynchronized_pool_resource : public memory_resource {
    memory_resource* upstream_;
    pool_options     opts_;
public:
    explicit unsynchronized_pool_resource(memory_resource* up = get_default_resource()) : upstream_(up) {}
    unsynchronized_pool_resource(const pool_options& o, memory_resource* up = get_default_resource())
        : upstream_(up), opts_(o) {}
    ~unsynchronized_pool_resource() {}
    void release() {}
    memory_resource* upstream_resource() const { return upstream_; }
    pool_options options() const { return opts_; }
protected:
    virtual void* do_allocate(std::size_t bytes, std::size_t align) { return upstream_->allocate(bytes, align); }
    virtual void do_deallocate(void* p, std::size_t bytes, std::size_t align) { upstream_->deallocate(p, bytes, align); }
    virtual bool do_is_equal(const memory_resource& o) const { return this == &o; }
};

typedef unsynchronized_pool_resource synchronized_pool_resource;   /* single-threaded approximation */

// --- polymorphic_allocator ---------------------------------------------------
template<class T>
class polymorphic_allocator {
    memory_resource* mr_;
public:
    typedef T                 value_type;
    typedef T*                pointer;
    typedef const T*          const_pointer;
    typedef T&                reference;
    typedef const T&          const_reference;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;
    template<class U> struct rebind { typedef polymorphic_allocator<U> other; };

    polymorphic_allocator() : mr_(get_default_resource()) {}
    polymorphic_allocator(memory_resource* r) : mr_(r ? r : get_default_resource()) {}
    polymorphic_allocator(const polymorphic_allocator& o) : mr_(o.mr_) {}
    template<class U> polymorphic_allocator(const polymorphic_allocator<U>& o) : mr_(o.resource()) {}

    pointer       address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }
    size_type     max_size() const { return size_type(-1) / (sizeof(T) ? sizeof(T) : 1); }

    T* allocate(size_type n) {
        return static_cast<T*>(mr_->allocate(n * sizeof(T), __alignof(T)));
    }
    void deallocate(T* p, size_type n) {
        mr_->deallocate(p, n * sizeof(T), __alignof(T));
    }

    void construct(pointer p, const T& v) { ::new(static_cast<void*>(p)) T(v); }
    template<class U> void destroy(U* p) { p->~U(); (void)p; }

    // construct(p, args...) : placement-new T(args...) via fnctmpl emulation.
    #define _CCW_PMR_CONSTRUCT(N) \
        template<class U _CCW_FNCTMPL_CM_TMPLARG(N)> \
        void construct(U* p _CCW_FNCTMPL_CM_DECLARG(N)) { \
            ::new(static_cast<void*>(p)) U(_CCW_FNCTMPL_FNCARG(N)); \
        }
    _CCW_FNCTMPL_GENERATE(_CCW_PMR_CONSTRUCT)
    #undef _CCW_PMR_CONSTRUCT

    memory_resource* resource() const { return mr_; }
    polymorphic_allocator select_on_container_copy_construction() const {
        return polymorphic_allocator();
    }
};

template<class T, class U>
inline bool operator==(const polymorphic_allocator<T>& a, const polymorphic_allocator<U>& b) {
    return *a.resource() == *b.resource();
}
template<class T, class U>
inline bool operator!=(const polymorphic_allocator<T>& a, const polymorphic_allocator<U>& b) {
    return !(a == b);
}

}   // namespace pmr
}   // namespace _CCW_STD17

// When _CCW_TARGET_CXX < 2017, _CCW_STD17 IS _ccw, so pmr already lives in _ccw;
// a self-referential namespace-alias would be an error -- only alias when std.
#if _CCW_TARGET_CXX >= 2017
namespace _ccw { namespace pmr = _CCW_STD17::pmr; }
#endif

#endif  /* _CCW_DETAIL_MEMORY_RESOURCE_HPP */
