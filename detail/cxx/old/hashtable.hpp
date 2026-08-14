/**
 *  @file   detail/hashtable.hpp
 *  @brief  Shared chained hash table + std::hash backport for the unordered
 *          containers on pre-C++11 VC (vc8/9).
 *  @license Boost Software License Version 1.0
 *  @note
 *      __ccw::hashtable<Key, Value, KeyOfValue, Hash, KeyEqual, Multi> is a bucket
 *      (separate chaining) table used by unordered_set / unordered_map. Value
 *      semantics only. A minimal std::hash is provided for the common key types.
 */
#ifndef _CCW_DETAIL_HASHTABLE_HPP
#define _CCW_DETAIL_HASHTABLE_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <vector>
#include <utility>
#include <string>

#if _MSC_VER < 1600   // vc8/9: no native <unordered_*> / std::hash

// -------- std::hash (minimal) -----------------------------------------------
namespace std {

template<class T> struct hash;

#define _CCW_HASH_INT(T)  template<> struct hash<T> { std::size_t operator()(T v) const { return std::size_t(v); } };
_CCW_HASH_INT(bool)
_CCW_HASH_INT(char)
_CCW_HASH_INT(signed char)
_CCW_HASH_INT(unsigned char)
_CCW_HASH_INT(wchar_t)
_CCW_HASH_INT(short)
_CCW_HASH_INT(unsigned short)
_CCW_HASH_INT(int)
_CCW_HASH_INT(unsigned int)
_CCW_HASH_INT(long)
_CCW_HASH_INT(unsigned long)
_CCW_HASH_INT(_ccw_llong)
_CCW_HASH_INT(_ccw_ullong)
#undef _CCW_HASH_INT

template<> struct hash<float>  { std::size_t operator()(float v)  const { return v == 0 ? 0 : *(const unsigned*)&v; } };
template<> struct hash<double> { std::size_t operator()(double v) const { if (v == 0) return 0; const unsigned* p = (const unsigned*)&v; return p[0] ^ p[1]; } };

template<class T> struct hash<T*> { std::size_t operator()(T* p) const { return reinterpret_cast<std::size_t>(p); } };

// FNV-1a for the string types.
namespace __ccw_h {
template<class Ch> inline std::size_t fnv(const Ch* p, std::size_t n) {
    std::size_t h = std::size_t(2166136261u);
    for (std::size_t i = 0; i < n; ++i) { h ^= std::size_t((unsigned)p[i]); h *= std::size_t(16777619u); }
    return h;
}
}
template<> struct hash<std::string>  { std::size_t operator()(const std::string& s)  const { return __ccw_h::fnv(s.data(), s.size()); } };
template<> struct hash<std::wstring> { std::size_t operator()(const std::wstring& s) const { return __ccw_h::fnv(s.data(), s.size()); } };

}   // namespace std

// -------- the hash table -----------------------------------------------------
namespace __ccw {

inline std::size_t ht_next_prime(std::size_t n) {
    static const std::size_t primes[] = {
        11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437,
        102877, 205759, 411527, 823117, 1646237, 3292489, 6584983, 13169977,
        26339969, 52679969, 105359939, 210719881, 421439783, 842879579, 1685759167u
    };
    const std::size_t cnt = sizeof(primes) / sizeof(primes[0]);
    for (std::size_t i = 0; i < cnt; ++i) if (primes[i] >= n) return primes[i];
    return primes[cnt - 1];
}

template<class Value>
struct ht_node { ht_node* next; Value value; ht_node(const Value& v) : next(0), value(v) {} };

template<class Key, class Value, class KeyOfValue, class Hash, class KeyEqual, bool Multi>
class hashtable {
public:
    typedef Key          key_type;
    typedef Value        value_type;
    typedef std::size_t  size_type;
    typedef Hash         hasher;
    typedef KeyEqual     key_equal;
    typedef ht_node<Value> node;

private:
    std::vector<node*> buckets_;
    size_type          size_;
    Hash               hash_;
    KeyEqual           eq_;
    float              max_load_;

    size_type bkt_of_key(const Key& k) const { return hash_(k) % buckets_.size(); }

public:
    struct const_iterator;
    struct iterator {
        node*      cur;
        const hashtable* ht;
        size_type  bkt;
        iterator(node* c = 0, const hashtable* h = 0, size_type b = 0) : cur(c), ht(h), bkt(b) {}
        value_type& operator*()  const { return cur->value; }
        value_type* operator->() const { return &cur->value; }
        iterator& operator++() { _adv(); return *this; }
        iterator  operator++(int) { iterator t = *this; _adv(); return t; }
        bool operator==(const iterator& r) const { return cur == r.cur; }
        bool operator!=(const iterator& r) const { return cur != r.cur; }
        void _adv() {
            if (cur->next) { cur = cur->next; return; }
            for (size_type b = bkt + 1; b < ht->buckets_.size(); ++b)
                if (ht->buckets_[b]) { cur = ht->buckets_[b]; bkt = b; return; }
            cur = 0;
        }
        friend struct const_iterator;
    };
    struct const_iterator {
        node*      cur;
        const hashtable* ht;
        size_type  bkt;
        const_iterator(node* c = 0, const hashtable* h = 0, size_type b = 0) : cur(c), ht(h), bkt(b) {}
        const_iterator(const iterator& it) : cur(it.cur), ht(it.ht), bkt(it.bkt) {}
        const value_type& operator*()  const { return cur->value; }
        const value_type* operator->() const { return &cur->value; }
        const_iterator& operator++() { _adv(); return *this; }
        const_iterator  operator++(int) { const_iterator t = *this; _adv(); return t; }
        bool operator==(const const_iterator& r) const { return cur == r.cur; }
        bool operator!=(const const_iterator& r) const { return cur != r.cur; }
        void _adv() {
            if (cur->next) { cur = cur->next; return; }
            for (size_type b = bkt + 1; b < ht->buckets_.size(); ++b)
                if (ht->buckets_[b]) { cur = ht->buckets_[b]; bkt = b; return; }
            cur = 0;
        }
    };

    explicit hashtable(size_type nb = 11, const Hash& h = Hash(), const KeyEqual& e = KeyEqual())
        : buckets_(ht_next_prime(nb), (node*)0), size_(0), hash_(h), eq_(e), max_load_(1.0f) {}
    hashtable(const hashtable& r)
        : buckets_(r.buckets_.size(), (node*)0), size_(0), hash_(r.hash_), eq_(r.eq_), max_load_(r.max_load_) {
        for (const_iterator it = r.begin(); it != r.end(); ++it) _insert_noresize(*it);
    }
    ~hashtable() { clear(); }
    hashtable& operator=(const hashtable& r) {
        if (this != &r) { clear(); buckets_.assign(r.buckets_.size(), (node*)0); max_load_ = r.max_load_;
            hash_ = r.hash_; eq_ = r.eq_;
            for (const_iterator it = r.begin(); it != r.end(); ++it) _insert_noresize(*it); }
        return *this;
    }

    size_type size()  const { return size_; }
    bool      empty() const { return size_ == 0; }
    size_type max_size() const { return size_type(-1) / sizeof(node); }
    hasher    hash_function() const { return hash_; }
    key_equal key_eq() const { return eq_; }
    size_type bucket_count() const { return buckets_.size(); }
    size_type max_bucket_count() const { return buckets_.max_size(); }
    size_type bucket(const Key& k) const { return bkt_of_key(k); }
    size_type bucket_size(size_type n) const {
        size_type c = 0; for (node* p = buckets_[n]; p; p = p->next) ++c; return c;
    }
    float     load_factor() const { return buckets_.empty() ? 0.f : float(size_) / float(buckets_.size()); }
    float     max_load_factor() const { return max_load_; }
    void      max_load_factor(float f) { max_load_ = f; }

    iterator       begin()       { return _first<iterator>(); }
    const_iterator begin() const { return _first<const_iterator>(); }
    const_iterator cbegin() const{ return _first<const_iterator>(); }
    iterator       end()       { return iterator(0, this, buckets_.size()); }
    const_iterator end() const { return const_iterator(0, this, buckets_.size()); }
    const_iterator cend() const{ return const_iterator(0, this, buckets_.size()); }

    void clear() {
        for (size_type b = 0; b < buckets_.size(); ++b) {
            node* n = buckets_[b];
            while (n) { node* nx = n->next; delete n; n = nx; }
            buckets_[b] = 0;
        }
        size_ = 0;
    }

    iterator find(const Key& k) {
        size_type b = bkt_of_key(k);
        for (node* n = buckets_[b]; n; n = n->next) if (eq_(KeyOfValue()(n->value), k)) return iterator(n, this, b);
        return end();
    }
    const_iterator find(const Key& k) const {
        size_type b = bkt_of_key(k);
        for (node* n = buckets_[b]; n; n = n->next) if (eq_(KeyOfValue()(n->value), k)) return const_iterator(n, this, b);
        return end();
    }
    size_type count(const Key& k) const {
        size_type b = bkt_of_key(k), c = 0;
        for (node* n = buckets_[b]; n; n = n->next) if (eq_(KeyOfValue()(n->value), k)) { ++c; if (!Multi) break; }
        return c;
    }
    bool contains(const Key& k) const { return count(k) != 0; }

    std::pair<iterator, iterator> equal_range(const Key& k) {
        iterator f = find(k);
        if (f == end()) return std::pair<iterator, iterator>(end(), end());
        iterator l = f; ++l;
        while (l != end() && eq_(KeyOfValue()(*l), k)) ++l;   // equal keys are adjacent
        return std::pair<iterator, iterator>(f, l);
    }
    std::pair<const_iterator, const_iterator> equal_range(const Key& k) const {
        const_iterator f = find(k);
        if (f == end()) return std::pair<const_iterator, const_iterator>(end(), end());
        const_iterator l = f; ++l;
        while (l != end() && eq_(KeyOfValue()(*l), k)) ++l;
        return std::pair<const_iterator, const_iterator>(f, l);
    }

    std::pair<iterator, bool> insert(const Value& v) {
        if (!Multi) { iterator f = find(KeyOfValue()(v)); if (f != end()) return std::pair<iterator, bool>(f, false); }
        _maybe_rehash();
        iterator it = _insert_noresize(v);
        return std::pair<iterator, bool>(it, true);
    }
    template<class It> void insert(It first, It last) { for (; first != last; ++first) insert(*first); }

    size_type erase(const Key& k) {
        size_type b = bkt_of_key(k), erased = 0;
        node* prev = 0; node* n = buckets_[b];
        while (n) {
            if (eq_(KeyOfValue()(n->value), k)) {
                node* d = n; n = n->next;
                if (prev) prev->next = n; else buckets_[b] = n;
                delete d; --size_; ++erased;
                if (!Multi) break;
            } else { prev = n; n = n->next; }
        }
        return erased;
    }
    iterator erase(iterator pos) {
        iterator nx = pos; ++nx;
        size_type b = pos.bkt;
        node* prev = 0; node* n = buckets_[b];
        while (n && n != pos.cur) { prev = n; n = n->next; }
        if (n) { if (prev) prev->next = n->next; else buckets_[b] = n->next; delete n; --size_; }
        return nx;
    }

    void rehash(size_type nb) {
        size_type want = ht_next_prime(nb);
        size_type need = size_type(float(size_) / max_load_) + 1;
        if (want < need) want = ht_next_prime(need);
        if (want == buckets_.size()) return;
        std::vector<node*> nbk(want, (node*)0);
        for (size_type b = 0; b < buckets_.size(); ++b) {
            node* n = buckets_[b];
            while (n) {
                node* nx = n->next;
                size_type t = hash_(KeyOfValue()(n->value)) % want;
                n->next = nbk[t]; nbk[t] = n;
                n = nx;
            }
        }
        buckets_.swap(nbk);
    }
    void reserve(size_type cnt) { rehash(size_type(float(cnt) / max_load_) + 1); }

    void swap(hashtable& r) { buckets_.swap(r.buckets_); std::swap(size_, r.size_); std::swap(max_load_, r.max_load_); }

    // exposed for the map wrapper's operator[] / at
    iterator _insert_noresize(const Value& v) {
        size_type b = hash_(KeyOfValue()(v)) % buckets_.size();
        node* nn = new node(v);
        // keep equal keys adjacent (so equal_range returns a contiguous range):
        // insert just after the first node with the same key, else at the head.
        for (node* n = buckets_[b]; n; n = n->next) {
            if (eq_(KeyOfValue()(n->value), KeyOfValue()(v))) {
                nn->next = n->next; n->next = nn; ++size_; return iterator(nn, this, b);
            }
        }
        nn->next = buckets_[b]; buckets_[b] = nn; ++size_;
        return iterator(nn, this, b);
    }

private:
    void _maybe_rehash() {
        if (float(size_ + 1) > max_load_ * float(buckets_.size())) rehash(buckets_.size() * 2 + 1);
    }
    template<class Iter> Iter _first() const {
        for (size_type b = 0; b < buckets_.size(); ++b) if (buckets_[b]) return Iter(buckets_[b], this, b);
        return Iter(0, this, buckets_.size());
    }
};

}   // namespace __ccw

#endif  // _MSC_VER < 1600

#endif  // _CCW_DETAIL_HASHTABLE_HPP
