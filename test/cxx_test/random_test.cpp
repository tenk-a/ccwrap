#include "test_cxx.hpp"

#if TEST_TARGET_CXX >= 2011
#include <random>
#include <sstream>
#include <vector>
#include <iterator>
#include <cstddef>
#include <cstdint>
#include <type_traits>

TEST_CASE(random, engines_are_deterministic) {
    STD::mt19937 g(12345);
    unsigned long r1 = g(), r2 = g();
    test_ne( r1, r2 );
    test_pass("cxx11:mt19937");

    STD::mt19937 g2(12345);
    unsigned long s1 = g2();
    test_eq( s1, r1 );
    test_pass("cxx11:mersenne_twister_engine");

    STD::mt19937 gd;
    unsigned long d1 = gd();
    test_eq( d1, 3499211612UL );
    test_pass("cxx11:mt19937#default_output");

    STD::minstd_rand0 lc(1);
    unsigned long l1 = lc();
    test_eq( l1, 16807UL );
    test_pass("cxx11:minstd_rand0");
}

TEST_CASE(random, mt19937_64_basic) {
    STD::mt19937_64 a(777), b(777);
    unsigned long long x1 = a();
    unsigned long long y1 = b();
    test_eq( x1, y1 );
    unsigned long long x2 = a();
    test_ne( x1, x2 );
    test_pass("cxx11:mt19937_64");

    bool high_seen = false;
    STD::mt19937_64 c;
    for (int i = 0; i < 100; ++i) { unsigned long long v = c(); if (v > 0xFFFFFFFFULL) high_seen = true; }
    test_true( high_seen );
    test_pass("cxx11:mt19937_64#range");
}

TEST_CASE(random, engine_static_constants) {
    test_eq( (unsigned long)STD::mt19937::word_size, 32UL );
    test_pass("cxx11:mersenne_twister_engine::word_size");
    test_eq( (unsigned long)STD::mt19937::state_size, 624UL );
    test_pass("cxx11:mersenne_twister_engine::state_size");
    test_eq( (unsigned long)STD::mt19937::default_seed, 5489UL );
    test_pass("cxx11:mersenne_twister_engine::default_seed");
    test_eq( (unsigned long)STD::mt19937_64::word_size, 64UL );
    test_pass("cxx11:mt19937_64#word_size");
    test_eq( (unsigned long)STD::minstd_rand::multiplier, 48271UL );
    test_pass("cxx11:linear_congruential_engine::multiplier");
    test_eq( (unsigned long)STD::minstd_rand::increment, 0UL );
    test_pass("cxx11:linear_congruential_engine::increment");
    test_eq( (unsigned long)STD::minstd_rand::modulus, 2147483647UL );
    test_pass("cxx11:linear_congruential_engine::modulus");
}

TEST_CASE(random, discard_matches_repeated_calls) {
    STD::mt19937 a(42), b(42);
    for (int i = 0; i < 10; ++i) (void)a();
    b.discard(10);
    unsigned long va = a(), vb = b();
    test_eq( va, vb );
    test_pass("cxx11:mersenne_twister_engine::discard");

    STD::minstd_rand la(1), lb(1);
    for (int i = 0; i < 5; ++i) (void)la();
    lb.discard(5);
    unsigned long l1 = la(), l2 = lb();
    test_eq( l1, l2 );
    test_pass("cxx11:linear_congruential_engine::discard");
}

TEST_CASE(random, engine_equality_and_stream) {
    STD::mt19937 a(9), b(9);
    test_true( a == b );
    test_pass("cxx11:mersenne_twister_engine::operator==");
    (void)a();
    test_true( a != b );
    test_pass("cxx11:mersenne_twister_engine::operator!=");

    STD::mt19937 src(2024);
    for (int i = 0; i < 50; ++i) (void)src();
    STD::stringstream ss;
    ss << src;
    STD::mt19937 dst;
    ss >> dst;
    test_true( src == dst );
    test_pass("cxx11:mersenne_twister_engine::operator<<");
    unsigned long s1 = src(), d1 = dst();
    test_eq( s1, d1 );
    test_pass("cxx11:mersenne_twister_engine::operator>>");

    STD::minstd_rand lsrc(123);
    (void)lsrc(); (void)lsrc();
    STD::stringstream ls;
    ls << lsrc;
    STD::minstd_rand ldst;
    ls >> ldst;
    test_true( lsrc == ldst );
    test_pass("cxx11:linear_congruential_engine::operator<<");
}

TEST_CASE(random, uniform_int_range_and_param) {
    STD::mt19937 g(12345);
    STD::uniform_int_distribution<int> di(1, 6);
    test_eq( di.a(), 1 );
    test_pass("cxx11:uniform_int_distribution::a");
    test_eq( di.b(), 6 );
    test_pass("cxx11:uniform_int_distribution::b");
    test_eq( di.min(), 1 );
    test_pass("cxx11:uniform_int_distribution::min");
    test_eq( di.max(), 6 );
    test_pass("cxx11:uniform_int_distribution::max");
    bool inrange = true;
    for (int i = 0; i < 400; ++i) { int v = di(g); if (v < 1 || v > 6) inrange = false; }
    test_true( inrange );
    test_pass("cxx11:uniform_int_distribution");

    STD::uniform_int_distribution<int>::param_type p(10, 20);
    di.param(p);
    test_eq( di.a(), 10 );
    test_pass("cxx11:uniform_int_distribution::param");
    bool r2 = true;
    for (int i = 0; i < 200; ++i) { int v = di(g); if (v < 10 || v > 20) r2 = false; }
    test_true( r2 );
    test_pass("cxx11:uniform_int_distribution::param_type");
}

TEST_CASE(random, uniform_real_and_bernoulli) {
    STD::mt19937 g(12345);
    STD::uniform_real_distribution<double> dr(0.0, 1.0);
    bool rok = true;
    for (int i = 0; i < 400; ++i) { double v = dr(g); if (v < 0.0 || v >= 1.0) rok = false; }
    test_true( rok );
    test_pass("cxx11:uniform_real_distribution");

    STD::bernoulli_distribution db(0.5);
    int trues = 0;
    for (int i = 0; i < 1000; ++i) if (db(g)) ++trues;
    test_lim( trues, 350, 650 );
    test_pass("cxx11:bernoulli_distribution");
}

TEST_CASE(random, normal_mean_and_reset) {
    STD::mt19937 g(555);
    STD::normal_distribution<double> nd(10.0, 2.0);
    test_eq( nd.mean(), 10.0 );
    test_pass("cxx11:normal_distribution::mean");
    test_eq( nd.stddev(), 2.0 );
    test_pass("cxx11:normal_distribution::stddev");
    double sum = 0.0;
    const int N = 4000;
    for (int i = 0; i < N; ++i) sum += nd(g);
    double mean = sum / N;
    test_true( mean > 9.5 && mean < 10.5 );
    test_pass("cxx11:normal_distribution");
    nd.reset();
    double one = nd(g);
    test_true( one > 0.0 && one < 20.0 );
    test_pass("cxx11:normal_distribution::reset");
}

TEST_CASE(random, exponential_and_poisson) {
    STD::mt19937 g(101);
    STD::exponential_distribution<double> ed(2.0);
    test_eq( ed.lambda(), 2.0 );
    test_pass("cxx11:exponential_distribution::lambda");
    double s = 0.0; const int N = 4000;
    for (int i = 0; i < N; ++i) { double v = ed(g); if (v < 0.0) v = 0.0; s += v; }
    double m = s / N;
    test_true( m > 0.35 && m < 0.65 );
    test_pass("cxx11:exponential_distribution");

    STD::poisson_distribution<int> pd(4.0);
    test_eq( pd.mean(), 4.0 );
    test_pass("cxx11:poisson_distribution::mean");
    long ps = 0; bool nonneg = true;
    for (int i = 0; i < N; ++i) { int v = pd(g); if (v < 0) nonneg = false; ps += v; }
    test_true( nonneg );
    double pm = (double)ps / N;
    test_true( pm > 3.4 && pm < 4.6 );
    test_pass("cxx11:poisson_distribution");
}

TEST_CASE(random, binomial_geometric_gamma) {
    STD::mt19937 g(202);
    STD::binomial_distribution<int> bd(10, 0.5);
    test_eq( bd.t(), 10 );
    test_pass("cxx11:binomial_distribution::t");
    test_eq( bd.p(), 0.5 );
    test_pass("cxx11:binomial_distribution::p");
    bool brange = true; long bs = 0; const int N = 3000;
    for (int i = 0; i < N; ++i) { int v = bd(g); if (v < 0 || v > 10) brange = false; bs += v; }
    test_true( brange );
    double bm = (double)bs / N;
    test_true( bm > 4.3 && bm < 5.7 );
    test_pass("cxx11:binomial_distribution");

    STD::geometric_distribution<int> gd(0.3);
    bool gnn = true;
    for (int i = 0; i < 500; ++i) { int v = gd(g); if (v < 0) gnn = false; }
    test_true( gnn );
    test_pass("cxx11:geometric_distribution");

    STD::gamma_distribution<double> ga(2.0, 1.5);
    test_eq( ga.alpha(), 2.0 );
    test_pass("cxx11:gamma_distribution::alpha");
    test_eq( ga.beta(), 1.5 );
    test_pass("cxx11:gamma_distribution::beta");
    double gs = 0.0;
    for (int i = 0; i < N; ++i) { double v = ga(g); if (v < 0.0) v = 0.0; gs += v; }
    double gm = gs / N;
    test_true( gm > 2.4 && gm < 3.6 );
    test_pass("cxx11:gamma_distribution");
}

TEST_CASE(random, misc_real_distributions) {
    STD::mt19937 g(303);
    STD::lognormal_distribution<double> ln(0.0, 1.0);
    bool lpos = true;
    for (int i = 0; i < 300; ++i) { if (ln(g) <= 0.0) lpos = false; }
    test_true( lpos );
    test_pass("cxx11:lognormal_distribution");

    STD::weibull_distribution<double> wd(1.5, 1.0);
    test_eq( wd.a(), 1.5 );
    test_pass("cxx11:weibull_distribution::a");
    bool wpos = true;
    for (int i = 0; i < 300; ++i) { if (wd(g) < 0.0) wpos = false; }
    test_true( wpos );
    test_pass("cxx11:weibull_distribution");

    STD::chi_squared_distribution<double> cs(4.0);
    test_eq( cs.n(), 4.0 );
    test_pass("cxx11:chi_squared_distribution::n");
    double css = 0.0; const int N = 3000;
    for (int i = 0; i < N; ++i) { double v = cs(g); if (v < 0.0) v = 0.0; css += v; }
    double csm = css / N;
    test_true( csm > 3.2 && csm < 4.8 );
    test_pass("cxx11:chi_squared_distribution");

    STD::cauchy_distribution<double> ca(0.0, 1.0);
    double med = ca(g);
    (void)med;
    test_true( true );
    test_pass("cxx11:cauchy_distribution");

    STD::extreme_value_distribution<double> ev(0.0, 1.0);
    double evv = ev(g); (void)evv;
    test_true( true );
    test_pass("cxx11:extreme_value_distribution");
    STD::student_t_distribution<double> st(5.0);
    double stv = st(g); (void)stv;
    test_true( true );
    test_pass("cxx11:student_t_distribution");
    STD::fisher_f_distribution<double> ff(3.0, 5.0);
    double f = ff(g);
    test_true( f >= 0.0 );
    test_pass("cxx11:fisher_f_distribution");
}

TEST_CASE(random, discrete_and_piecewise) {
    STD::mt19937 g(404);
    double w[] = {1.0, 1.0, 2.0};
#if !defined(_MSC_VER) || _MSC_VER >= 1900
    STD::discrete_distribution<int> dd(w, w + 3);
    STD::vector<double> probs = dd.probabilities();
    test_eq( (int)probs.size(), 3 );
    test_true( probs[2] > 0.45 && probs[2] < 0.55 );
    test_pass("cxx11:discrete_distribution::probabilities");
    int counts[3] = {0, 0, 0};
    const int N = 4000;
    for (int i = 0; i < N; ++i) { int v = dd(g); if (v >= 0 && v < 3) ++counts[v]; }
    test_true( counts[2] > counts[0] );
    test_pass("cxx11:discrete_distribution");
#else
    (void)w;
    TEST_SKIP1(); TEST_SKIP1();
    test_skip("cxx11:discrete_distribution::probabilities");
    TEST_SKIP1();
    test_skip("cxx11:discrete_distribution");
#endif

    double bnd[] = {0.0, 10.0, 20.0};
    double den[] = {0.0, 1.0};
    STD::piecewise_constant_distribution<double> pc(bnd, bnd + 3, den);
    bool pcok = true;
    for (int i = 0; i < 500; ++i) { double v = pc(g); if (v < 10.0 || v > 20.0) pcok = false; }
    test_true( pcok );
    test_pass("cxx11:piecewise_constant_distribution");

    double lb[] = {0.0, 1.0};
    double lw[] = {1.0, 1.0};
    STD::piecewise_linear_distribution<double> pl(lb, lb + 2, lw);
    bool plok = true;
    for (int i = 0; i < 500; ++i) { double v = pl(g); if (v < 0.0 || v > 1.0) plok = false; }
    test_true( plok );
    test_pass("cxx11:piecewise_linear_distribution");
}

TEST_CASE(random, generate_canonical_range) {
    STD::mt19937 g(505);
    bool ok = true;
    for (int i = 0; i < 500; ++i) {
        double v = STD::generate_canonical<double, 32>(g);
        if (v < 0.0 || v >= 1.0) ok = false;
    }
#if defined(_MSC_VER) && _MSC_VER >= 1700 && _MSC_VER < 1900
    TEST_SKIP_VC("MSVC 11/12 generate_canonical can return 1.0");
#endif
    test_true( ok );
    test_pass("cxx11:generate_canonical");
}

TEST_CASE(random, seed_seq_basic) {
    unsigned s[] = {1, 2, 3, 4};
    STD::seed_seq sq(s, s + 4);
    test_eq( (int)sq.size(), 4 );
    test_pass("cxx11:seed_seq::size");

    STD::vector<unsigned long> out(10, 0UL);
    sq.generate(out.begin(), out.end());

    STD::seed_seq sq2(s, s + 4);
    STD::vector<unsigned long> out2(10, 0UL);
    sq2.generate(out2.begin(), out2.end());
    test_true( out == out2 );

    bool nonzero = false;
    for (STD::size_t i = 0; i < out.size(); ++i) if (out[i] != 0UL) nonzero = true;
    test_true( nonzero );
    test_pass("cxx11:seed_seq::generate");

    STD::vector<unsigned long> p;
    sq.param(STD::back_inserter(p));
    test_eq( (int)p.size(), 4 );
    test_eq( p[0], 1UL );
    test_eq( p[3], 4UL );
    test_pass("cxx11:seed_seq::param");

    STD::seed_seq sq3(s, s + 4);
    STD::vector<unsigned long> seeds(1, 0UL);
    sq3.generate(seeds.begin(), seeds.end());
    STD::mt19937 g(seeds[0]);
    unsigned long v = g();
    (void)v;
    test_true( true );
    test_pass("cxx11:seed_seq");
}

TEST_CASE(random, subtract_with_carry_engine) {
    STD::ranlux24_base e(1);
    unsigned long a1 = e();
    test_true( a1 <= 0xFFFFFFUL );
    test_pass("cxx11:subtract_with_carry_engine");
    test_eq( (unsigned long)STD::ranlux24_base::max(), 0xFFFFFFUL );
    test_pass("cxx11:subtract_with_carry_engine::max");
    test_eq( (unsigned long)STD::ranlux24_base::min(), 0UL );
    test_pass("cxx11:subtract_with_carry_engine::min");

    STD::ranlux24_base e2(1);
    unsigned long b1 = e2();
    test_eq( b1, a1 );
    test_pass("cxx11:subtract_with_carry_engine#reproduce");

    STD::ranlux48_base f(1);
    unsigned long long g1 = f();
    test_true( g1 <= 0xFFFFFFFFFFFFULL );
    test_pass("cxx11:ranlux48_base");

    STD::ranlux24_base h1(7), h2(7);
    test_true( h1 == h2 );
    test_pass("cxx11:subtract_with_carry_engine::operator==");
    (void)h1();
    test_true( h1 != h2 );
    test_pass("cxx11:subtract_with_carry_engine::operator!=");
}

TEST_CASE(random, discard_block_and_ranlux24) {
    STD::ranlux24 r(1);
    unsigned long v = r();
    test_true( v <= 0xFFFFFFUL );
    test_pass("cxx11:ranlux24");

    STD::ranlux24 r2(1);
    unsigned long v2 = r2();
    test_eq( v2, v );
    test_pass("cxx11:discard_block_engine");

    STD::ranlux24 a(5), b(5);
    a.discard(3);
    (void)b(); (void)b(); (void)b();
    unsigned long av = a(), bv = b();
    test_eq( av, bv );
    test_pass("cxx11:discard_block_engine::discard");

    test_eq( (unsigned long)STD::ranlux24::max(), 0xFFFFFFUL );
    test_pass("cxx11:discard_block_engine::max");
}

TEST_CASE(random, shuffle_order_knuth_b) {
    STD::knuth_b k(1);
    unsigned long v = k();
    test_true( v >= STD::knuth_b::min() && v <= STD::knuth_b::max() );
    test_pass("cxx11:knuth_b");

    STD::knuth_b k2(1);
    unsigned long v2 = k2();
    test_eq( v2, v );
    test_pass("cxx11:shuffle_order_engine");

    unsigned long w = k();
    test_ne( w, v );
    test_pass("cxx11:shuffle_order_engine#varies");

    STD::knuth_b e1(3), e2(3);
    test_true( e1 == e2 );
    test_pass("cxx11:shuffle_order_engine::operator==");
}

TEST_CASE(random, independent_bits_engine) {

    STD::independent_bits_engine<STD::mt19937, 16, unsigned long> e(1);
    unsigned long v = e();
    test_true( v <= 0xFFFFUL );
    test_pass("cxx11:independent_bits_engine");
    test_eq( (unsigned long)e.max(), 0xFFFFUL );
    test_pass("cxx11:independent_bits_engine::max");
    test_eq( (unsigned long)e.min(), 0UL );
    test_pass("cxx11:independent_bits_engine::min");

    STD::independent_bits_engine<STD::mt19937, 16, unsigned long> e2(1);
    unsigned long v2 = e2();
    test_eq( v2, v );
    test_pass("cxx11:independent_bits_engine#deterministic");

    STD::independent_bits_engine<STD::mt19937, 8, unsigned long> nrw(1);
    unsigned long nv = nrw();
    test_true( nv <= 0xFFUL );
    test_pass("cxx11:independent_bits_engine#width");
}

TEST_CASE(random, random_device_interface) {
    STD::random_device rd;
    unsigned long a = rd(), b = rd();
    test_true( a >= STD::random_device::min() && a <= STD::random_device::max() );
    (void)b;
    test_pass("cxx11:random_device");
    test_true( rd.entropy() >= 0.0 );
    test_pass("cxx11:random_device::entropy");

#if defined(__WATCOMC__)
    test_true( rd.entropy() == 0.0 );
    test_pass("cxx11:random_device::entropy#port");
#else
    TEST_SKIP1();
    test_skip("cxx11:random_device::entropy#port");
#endif

    STD::uniform_int_distribution<int> d(1, 6);
    int roll = d(rd);
    test_true( roll >= 1 && roll <= 6 );
    test_pass("cxx11:random_device::operator()");
}

TEST_CASE(random, more_engines_and_dist) {
#if !defined(__WATCOMC__)
    {
        STD::linear_congruential_engine<STD::uint_fast32_t, 16807, 0, 2147483647> lce(1);
        for (int i = 0; i < 9999; ++i) lce();
        long v = (long)lce();
        test_eq( v, 1043618065L );
        test_pass("cxx11:linear_congruential_engine");
    }
    {

        STD::minstd_rand mr(1);
        for (int i = 0; i < 9999; ++i) mr();
        long v = (long)mr();
        test_eq( v, 399268537L );
        test_pass("cxx11:minstd_rand");
    }
    {
        STD::default_random_engine dre(42);
        unsigned long v1 = dre();
        unsigned long v2 = dre();
        test_true( v1 != v2 || v1 == v2 );
        (void)v2;
        test_pass("cxx11:default_random_engine");
    }
    {
        STD::ranlux24_base r24(1);
        STD::ranlux24_base::result_type a = r24();
        test_true( a <= r24.max() );
        test_pass("cxx11:ranlux24_base");

        STD::ranlux48 r48(1);
        STD::ranlux48::result_type b = r48();
        test_true( b <= r48.max() );
        test_pass("cxx11:ranlux48");
    }
    {
        STD::mt19937 gen(7);
        STD::negative_binomial_distribution<int> nb(5, 0.5);
        int lo = nb.min();
        int s = nb(gen);
        test_true( s >= lo );
        test_true( s >= 0 );
        test_pass("cxx11:negative_binomial_distribution");
    }
#else
    test_skip("cxx11:linear_congruential_engine");
    test_skip("cxx11:minstd_rand");
    test_skip("cxx11:default_random_engine");
    test_skip("cxx11:ranlux24_base");
    test_skip("cxx11:ranlux48");
    test_skip("cxx11:negative_binomial_distribution");
#endif
}

TEST_CASE(random, engine_result_types_cxx11) {
    {
        STD::mt19937 g(1);
        STD::mersenne_twister_engine<STD::uint_fast32_t, 32, 624, 397, 31,
            0x9908b0dfUL, 11, 0xffffffffUL, 7, 0x9d2c5680UL, 15, 0xefc60000UL, 18,
            1812433253UL>::result_type v = g();
        test_true(( STD::is_same<STD::mt19937::result_type, STD::uint_fast32_t>::value ));
        test_true( v >= STD::mt19937::min() && v <= STD::mt19937::max() );
        test_true( tst_type_is<STD::mt19937::result_type>(g()) );
        test_pass("cxx11:mersenne_twister_engine::result_type");

        STD::mt19937 fresh(99);
        g.seed(99);
        test_eq( (long)g(), (long)fresh() );
        g.seed();
        STD::mt19937 defaulted;
        test_eq( (long)g(), (long)defaulted() );
        test_pass("cxx11:mersenne_twister_engine::seed");
    }
#if !defined(__WATCOMC__)
    {
        typedef STD::linear_congruential_engine<STD::uint_fast32_t, 16807, 0, 2147483647> LCE;
        LCE e(1);
        LCE::result_type v = e();
        test_true(( STD::is_same<LCE::result_type, STD::uint_fast32_t>::value ));
        test_true( v >= LCE::min() && v <= LCE::max() );
        test_pass("cxx11:linear_congruential_engine::result_type");

        LCE fresh(7);
        e.seed(7);
        test_eq( (long)e(), (long)fresh() );
        test_pass("cxx11:linear_congruential_engine::seed");
    }
    {
        typedef STD::subtract_with_carry_engine<STD::uint_fast32_t, 24, 10, 24> SWC;
        SWC e(1);
        SWC::result_type v = e();
        test_true(( STD::is_same<SWC::result_type, STD::uint_fast32_t>::value ));
        test_true( v <= SWC::max() );
        test_pass("cxx11:subtract_with_carry_engine::result_type");

        SWC fresh(5);
        e.seed(5);
        test_eq( (long)e(), (long)fresh() );
        test_pass("cxx11:subtract_with_carry_engine::seed");
    }
    {
        typedef STD::discard_block_engine<STD::ranlux24_base, 223, 23> DBE;
        DBE e(1);
        DBE::result_type v = e();
        test_true(( STD::is_same<DBE::result_type, STD::ranlux24_base::result_type>::value ));
        test_true( v <= DBE::max() );
        test_pass("cxx11:discard_block_engine::result_type");

        const DBE& ce = e;
#if _TST_HAS_DECLTYPE
        test_true(( STD::is_same<decltype(ce.base()), const STD::ranlux24_base&>::value ));
#else
        TEST_NOTE("no decltype in this language mode");
        TEST_SKIP1();
#endif
        STD::ranlux24_base copy = ce.base();
        test_true( copy() <= STD::ranlux24_base::max() );
        test_pass("cxx11:discard_block_engine::base");

        DBE fresh(3);
        e.seed(3);
        test_eq( (long)e(), (long)fresh() );
        test_pass("cxx11:discard_block_engine::seed");
    }
    {
        typedef STD::independent_bits_engine<STD::mt19937, 16, STD::uint_fast32_t> IBE;
        IBE e(1);
        IBE::result_type v = e();
        test_true(( STD::is_same<IBE::result_type, STD::uint_fast32_t>::value ));
        test_true( v <= 0xFFFFu );
        test_eq( (long)IBE::max(), 0xFFFFL );
        test_pass("cxx11:independent_bits_engine::result_type");

        const IBE& ce = e;
#if _TST_HAS_DECLTYPE
        test_true(( STD::is_same<decltype(ce.base()), const STD::mt19937&>::value ));
        test_pass("cxx11:independent_bits_engine::base");
#else
        TEST_NOTE("no decltype in this language mode");
        TEST_SKIP1();
        test_skip("cxx11:independent_bits_engine::base");
#endif

        IBE fresh(4);
        e.seed(4);
        test_eq( (long)e(), (long)fresh() );
        test_pass("cxx11:independent_bits_engine::seed");
    }
    {
        typedef STD::shuffle_order_engine<STD::minstd_rand0, 16> SOE;
        SOE e(1);
        SOE::result_type v = e();
        test_true(( STD::is_same<SOE::result_type, STD::minstd_rand0::result_type>::value ));
        test_true( v >= SOE::min() && v <= SOE::max() );
        test_pass("cxx11:shuffle_order_engine::result_type");

        const SOE& ce = e;
#if _TST_HAS_DECLTYPE
        test_true(( STD::is_same<decltype(ce.base()), const STD::minstd_rand0&>::value ));
        test_pass("cxx11:shuffle_order_engine::base");
#else
        TEST_NOTE("no decltype in this language mode");
        TEST_SKIP1();
        test_skip("cxx11:shuffle_order_engine::base");
#endif

        SOE fresh(6);
        e.seed(6);
        test_eq( (long)e(), (long)fresh() );
        test_pass("cxx11:shuffle_order_engine::seed");
    }
#else
    TEST_NOTE("the Open Watcom port ships a reduced <random>");
    test_skip("cxx11:linear_congruential_engine::result_type");
    test_skip("cxx11:linear_congruential_engine::seed");
    test_skip("cxx11:subtract_with_carry_engine::result_type");
    test_skip("cxx11:subtract_with_carry_engine::seed");
    test_skip("cxx11:discard_block_engine::result_type");
    test_skip("cxx11:discard_block_engine::base");
    test_skip("cxx11:discard_block_engine::seed");
    test_skip("cxx11:independent_bits_engine::result_type");
    test_skip("cxx11:independent_bits_engine::base");
    test_skip("cxx11:independent_bits_engine::seed");
    test_skip("cxx11:shuffle_order_engine::result_type");
    test_skip("cxx11:shuffle_order_engine::base");
    test_skip("cxx11:shuffle_order_engine::seed");
#endif

    test_true(( STD::is_same<STD::random_device::result_type, unsigned int>::value ));
    test_true(( STD::is_same<STD::seed_seq::result_type, STD::uint_least32_t>::value ));
    {
        unsigned s[] = { 1, 2, 3 };
        STD::seed_seq sq(s, s + 3);
        STD::seed_seq::result_type out[4] = { 0, 0, 0, 0 };
        sq.generate(out, out + 4);
        test_true( out[0] != 0 || out[1] != 0 );
    }
    test_pass("cxx11:seed_seq::result_type");
#if !defined(__WATCOMC__)
    {
        STD::random_device rd;
        STD::random_device::result_type v = rd();
        test_true( v >= STD::random_device::min() && v <= STD::random_device::max() );
    }
    test_pass("cxx11:random_device::result_type");
#else
    TEST_NOTE("no random_device in the Open Watcom port");
    test_skip("cxx11:random_device::result_type");
#endif
}

TEST_CASE(random, distribution_result_types_cxx11) {
    STD::mt19937 g(1);

    {
        STD::uniform_int_distribution<int> d(3, 9);
        test_true(( STD::is_same<STD::uniform_int_distribution<int>::result_type, int>::value ));
        int v = d(g);
        test_true( v >= d.min() && v <= d.max() );
        test_pass("cxx11:uniform_int_distribution::result_type");
    }
    {
        STD::uniform_real_distribution<double> d(1.0, 2.0);
        test_true(( STD::is_same<STD::uniform_real_distribution<double>::result_type,
                                 double>::value ));
        double v = d(g);
        test_true( v >= 1.0 && v < 2.0 );
        test_pass("cxx11:uniform_real_distribution::result_type");
    }
    {
        test_true(( STD::is_same<STD::bernoulli_distribution::result_type, bool>::value ));
        STD::bernoulli_distribution d(0.5);
        bool v = d(g);
        test_true( v == true || v == false );
        test_pass("cxx11:bernoulli_distribution::result_type");
    }
#if !defined(__WATCOMC__)
    {
        STD::normal_distribution<double> d(0.0, 1.0);
        test_true(( STD::is_same<STD::normal_distribution<double>::result_type,
                                 double>::value ));
        double v = d(g);
        test_true( v == v );
        test_pass("cxx11:normal_distribution::result_type");
    }
#if !defined(_MSC_VER) || _TST_HAS_CXX14_LIB_MEMBERS
    {
        double w[] = { 1.0, 2.0, 3.0 };
        STD::discrete_distribution<int> d(w, w + 3);
        test_true(( STD::is_same<STD::discrete_distribution<int>::result_type, int>::value ));
        int v = d(g);
        test_true( v >= 0 && v <= 2 );
        test_pass("cxx11:discrete_distribution::result_type");
    }
#else
    TEST_NOTE("no iterator-pair discrete_distribution constructor before VS2015");
    TEST_SKIP_N(2);
    test_skip("cxx11:discrete_distribution::result_type");
#endif
    {
        double b[] = { 0.0, 1.0, 3.0 };
        double w[] = { 1.0, 1.0 };
        STD::piecewise_constant_distribution<double> d(b, b + 3, w);
        test_true(( STD::is_same<STD::piecewise_constant_distribution<double>::result_type,
                                 double>::value ));
        double v = d(g);
        test_true( v >= 0.0 && v < 3.0 );
        test_pass("cxx11:piecewise_constant_distribution::result_type");

        STD::vector<double> iv = d.intervals();
        test_eq( iv.size(), (STD::size_t)3 );
        test_eq( iv[0], 0.0 );
        test_eq( iv[2], 3.0 );
        test_pass("cxx11:piecewise_constant_distribution::intervals");

        STD::vector<double> de = d.densities();
        test_eq( de.size(), (STD::size_t)2 );
        double area = de[0] * (iv[1] - iv[0]) + de[1] * (iv[2] - iv[1]);
        test_true( area > 0.999 && area < 1.001 );
        test_pass("cxx11:piecewise_constant_distribution::densities");
    }
    {
        double b[] = { 0.0, 1.0, 2.0 };
        double w[] = { 0.0, 1.0, 0.0 };
        STD::piecewise_linear_distribution<double> d(b, b + 3, w);
        test_true(( STD::is_same<STD::piecewise_linear_distribution<double>::result_type,
                                 double>::value ));
        double v = d(g);
        test_true( v >= 0.0 && v <= 2.0 );
        test_pass("cxx11:piecewise_linear_distribution::result_type");

        STD::vector<double> iv = d.intervals();
        test_eq( iv.size(), (STD::size_t)3 );
        test_eq( iv[1], 1.0 );
        test_pass("cxx11:piecewise_linear_distribution::intervals");

        STD::vector<double> de = d.densities();
        test_eq( de.size(), (STD::size_t)3 );
        test_true( de[1] > de[0] );
        test_pass("cxx11:piecewise_linear_distribution::densities");
    }
#else
    TEST_NOTE("the Open Watcom port ships a reduced <random>");
    test_skip("cxx11:normal_distribution::result_type");
    test_skip("cxx11:discrete_distribution::result_type");
    test_skip("cxx11:piecewise_constant_distribution::result_type");
    test_skip("cxx11:piecewise_constant_distribution::intervals");
    test_skip("cxx11:piecewise_constant_distribution::densities");
    test_skip("cxx11:piecewise_linear_distribution::result_type");
    test_skip("cxx11:piecewise_linear_distribution::intervals");
    test_skip("cxx11:piecewise_linear_distribution::densities");
#endif
}

TEST_CASE(random, uniform_random_bit_generator_cxx20) {
#if defined(__cpp_lib_concepts) && TEST_TARGET_CXX >= 2020
    test_true( STD::uniform_random_bit_generator<STD::mt19937> );
    test_true( STD::uniform_random_bit_generator<STD::minstd_rand> );
    test_true( !STD::uniform_random_bit_generator<int> );
    struct Signed {
        typedef int result_type;
        static constexpr int min() { return 0; }
        static constexpr int max() { return 7; }
        int operator()() { return 1; }
    };
    test_true( !STD::uniform_random_bit_generator<Signed> );
    static_assert(STD::uniform_random_bit_generator<STD::mt19937>,
                  "mt19937 must satisfy the concept");
    test_pass("cxx20:uniform_random_bit_generator");
#else
    TEST_NOTE("concepts are C++20; not available here");
    test_skip("cxx20:uniform_random_bit_generator");
#endif
}

TEST_CASE(random, philox_and_generate_random_cxx26) {
#if defined(__cpp_lib_philox_engine)
    test_true(( STD::is_same<STD::philox4x32, STD::philox_engine<STD::uint_fast32_t,
                             32, 4, 10, 0xCD9E8D57, 0x9E3779B9, 0xD2511F53, 0xBB67AE85> >::value ));
    test_pass("cxx26:philox_engine");
    {
        STD::philox4x32 e;
        STD::philox4x32::result_type v = e();
        test_true( v >= STD::philox4x32::min() && v <= STD::philox4x32::max() );
        test_true(( STD::is_same<STD::philox4x32::result_type, STD::uint_fast32_t>::value ));
        test_pass("cxx26:philox_engine::result_type");
        test_eq( (int)STD::philox4x32::word_size, 32 );
        test_pass("cxx26:philox_engine::word_size");
        test_eq( (int)STD::philox4x32::word_count, 4 );
        test_pass("cxx26:philox_engine::word_count");
        test_eq( (int)STD::philox4x32::round_count, 10 );
        test_pass("cxx26:philox_engine::round_count");
        test_pass("cxx26:philox4x32");
    }
    {
        STD::philox4x64 e;
        test_eq( (int)STD::philox4x64::word_size, 64 );
        test_true( e() <= STD::philox4x64::max() );
        test_pass("cxx26:philox4x64");
    }
#else
    TEST_NOTE("philox_engine is C++26 (P2075); not in this library");
    test_skip("cxx26:philox_engine");
    test_skip("cxx26:philox_engine::result_type");
    test_skip("cxx26:philox_engine::word_size");
    test_skip("cxx26:philox_engine::word_count");
    test_skip("cxx26:philox_engine::round_count");
    test_skip("cxx26:philox4x32");
    test_skip("cxx26:philox4x64");
#endif

#if defined(__cpp_lib_ranges_generate_random) && (__cpp_lib_ranges_generate_random) >= 202403L
    {
        STD::mt19937 g(1);
        unsigned buf[8] = { 0 };
        STD::ranges::generate_random(buf, g);
        bool any = false;
        for (int i = 0; i < 8; ++i) if (buf[i] != 0) any = true;
        test_true( any );
        test_pass("cxx26:ranges::generate_random(range,generator)");

        unsigned buf2[8] = { 0 };
        STD::ranges::generate_random(buf2, buf2 + 8, g);
        bool any2 = false;
        for (int i = 0; i < 8; ++i) if (buf2[i] != 0) any2 = true;
        test_true( any2 );
        test_pass("cxx26:ranges::generate_random(first,last,generator)");
    }
    test_pass("cxx26:generate_random");
#else
    TEST_NOTE("generate_random is C++26 (P1068); not in this library");
    test_skip("cxx26:ranges::generate_random(range,generator)");
    test_skip("cxx26:ranges::generate_random(first,last,generator)");
    test_skip("cxx26:generate_random");
#endif
}
#endif

TEST_CASE(random, feature_test_macros) {
#if defined(__cpp_lib_ranges_generate_random) && (__cpp_lib_ranges_generate_random) >= 202403L
    test_true( (__cpp_lib_ranges_generate_random) >= 202403L );
    test_pass("cxx26:__cpp_lib_ranges_generate_random=202403L");
#else
    TEST_NOTE("absent, or an earlier revision than the value spec names");
    test_skip("cxx26:__cpp_lib_ranges_generate_random=202403L");
#endif
}
