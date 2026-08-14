/**
 *  @file   _ccwrap_detail/random.hpp
 *  @brief  std::random backport (practical subset) for pre-C++11 VC (vc8/9).
 *  @license Boost Software License Version 1.0
 *  @note   Engines: mt19937, linear_congruential_engine (minstd_rand0/minstd_rand).
 *          Distributions: uniform_int_distribution, uniform_real_distribution,
 *          bernoulli_distribution. random_device (best effort via rand_s/rand).
 *          uniform_int uses modulo scaling (slightly biased) -- a light backport.
 */
#ifndef _CCW_DETAIL_RANDOM_HPP
#define _CCW_DETAIL_RANDOM_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <cstdlib>
#include <string>

namespace std {

class mt19937 {
    unsigned int mt_[624];
    int          idx_;
    void _gen() {
        for (int i = 0; i < 624; ++i) {
            unsigned int y = (mt_[i] & 0x80000000u) | (mt_[(i + 1) % 624] & 0x7fffffffu);
            mt_[i] = mt_[(i + 397) % 624] ^ (y >> 1);
            if (y & 1u) mt_[i] ^= 0x9908b0dfu;
        }
        idx_ = 0;
    }
public:
    typedef unsigned int result_type;
    static result_type (min)() { return 0u; }
    static result_type (max)() { return 0xffffffffu; }
    static const result_type default_seed = 5489u;

    explicit mt19937(result_type s = default_seed) { seed(s); }
    void seed(result_type s) {
        mt_[0] = s;
        for (int i = 1; i < 624; ++i)
            mt_[i] = 1812433253u * (mt_[i - 1] ^ (mt_[i - 1] >> 30)) + (unsigned int)i;
        idx_ = 624;
    }
    result_type operator()() {
        if (idx_ >= 624) _gen();
        unsigned int y = mt_[idx_++];
        y ^= y >> 11;
        y ^= (y << 7) & 0x9d2c5680u;
        y ^= (y << 15) & 0xefc60000u;
        y ^= y >> 18;
        return y;
    }
    void discard(unsigned long long n) { while (n--) (*this)(); }
};

template<class UIntType, UIntType a, UIntType c, UIntType m>
class linear_congruential_engine {
    UIntType x_;
public:
    typedef UIntType result_type;
    static result_type (min)() { return c == 0u ? 1u : 0u; }
    static result_type (max)() { return m - 1u; }
    static const result_type default_seed = 1u;

    explicit linear_congruential_engine(result_type s = default_seed) { seed(s); }
    void seed(result_type s) { x_ = (s % m == 0u && c == 0u) ? 1u : s % m; }
    result_type operator()() { x_ = (result_type)(((unsigned long long)a * x_ + c) % m); return x_; }
    void discard(unsigned long long n) { while (n--) (*this)(); }
};
typedef linear_congruential_engine<unsigned int, 16807u, 0u, 2147483647u> minstd_rand0;
typedef linear_congruential_engine<unsigned int, 48271u, 0u, 2147483647u> minstd_rand;

template<class IntType>
class uniform_int_distribution {
    IntType a_, b_;
public:
    typedef IntType result_type;
    explicit uniform_int_distribution(IntType a = 0, IntType b = 2147483647) : a_(a), b_(b) {}
    IntType a() const { return a_; }
    IntType b() const { return b_; }
    void reset() {}
    template<class G> IntType operator()(G& g) {
        unsigned int span = (unsigned int)(b_ - a_) + 1u;
        if (span == 0u) return (IntType)g();      /* full range */
        unsigned int r = (unsigned int)(g() - (G::min)());
        return (IntType)(a_ + (IntType)(r % span));
    }
};

template<class RealType>
class uniform_real_distribution {
    RealType a_, b_;
public:
    typedef RealType result_type;
    explicit uniform_real_distribution(RealType a = RealType(0), RealType b = RealType(1)) : a_(a), b_(b) {}
    RealType a() const { return a_; }
    RealType b() const { return b_; }
    void reset() {}
    template<class G> RealType operator()(G& g) {
        double range = (double)((G::max)() - (G::min)()) + 1.0;
        double u = (double)(g() - (G::min)()) / range;   /* [0,1) */
        return (RealType)(a_ + u * (b_ - a_));
    }
};

class bernoulli_distribution {
    double p_;
public:
    typedef bool result_type;
    explicit bernoulli_distribution(double p = 0.5) : p_(p) {}
    double p() const { return p_; }
    void reset() {}
    template<class G> bool operator()(G& g) {
        double range = (double)((G::max)() - (G::min)()) + 1.0;
        double u = (double)(g() - (G::min)()) / range;
        return u < p_;
    }
};

class random_device {
public:
    typedef unsigned int result_type;
    random_device() {}
    explicit random_device(const std::string&) {}
    static result_type (min)() { return 0u; }
    static result_type (max)() { return 0xffffffffu; }
    double entropy() const { return 0.0; }
    result_type operator()() {
        /* not cryptographically strong; a light backport over the CRT rand() */
        return ((unsigned int)rand() << 17) ^ ((unsigned int)rand() << 6) ^ (unsigned int)rand();
    }
};

}   // namespace std

#endif  // _CCW_DETAIL_RANDOM_HPP
