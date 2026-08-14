#include "c_test.h"

#if defined(__WATCOMC__)
# define TST_STDATOMIC_UNAVAILABLE 1
#else
# include <stdatomic.h>
#endif

TEST_CASE(stdatomic, flag) {
#ifdef TST_STDATOMIC_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    atomic_flag f = ATOMIC_FLAG_INIT;

    test_gt( (int)sizeof(f), 0 );
    test_pass("c2011:opt:atomic_flag");

    test_true( !atomic_flag_test_and_set(&f) );
    test_pass("c2011:opt:ATOMIC_FLAG_INIT");
    test_true(  atomic_flag_test_and_set(&f) );
    test_pass("c2011:opt:atomic_flag_test_and_set");
    atomic_flag_clear(&f);
    test_true( !atomic_flag_test_and_set(&f) );
    atomic_flag_clear(&f);
    test_pass("c2011:opt:atomic_flag_clear");
#endif
}

TEST_CASE(stdatomic, load_store_exchange) {
#ifdef TST_STDATOMIC_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    atomic_int a;

    test_gt( (int)sizeof(a), 0 );
    TEST_NOTE("atomic_int is the representative of the atomic_* typedef family");
    test_pass("c2011:opt:atomic_int");
    atomic_init(&a, 5);
    test_eq( (int)atomic_load(&a), 5 );
    test_pass("c2011:opt:atomic_init");
    atomic_store(&a, 9);
    test_eq( (int)atomic_load(&a), 9 );
    test_pass("c2011:opt:atomic_store");
    test_eq( (int)atomic_load(&a), 9 );
    test_pass("c2011:opt:atomic_load");
    test_eq( (int)atomic_exchange(&a, 11), 9 );
    test_eq( (int)atomic_load(&a), 11 );
    test_pass("c2011:opt:atomic_exchange");
#endif
}

TEST_CASE(stdatomic, fetch_ops) {
#ifdef TST_STDATOMIC_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    atomic_int a;

    atomic_init(&a, 10);
    test_eq( (int)atomic_fetch_add(&a, 5), 10 );
    test_eq( (int)atomic_load(&a), 15 );
    test_pass("c2011:opt:atomic_fetch_add");
    test_eq( (int)atomic_fetch_sub(&a, 3), 15 );
    test_eq( (int)atomic_load(&a), 12 );
    test_pass("c2011:opt:atomic_fetch_sub");

    atomic_store(&a, 0x0F);
    test_eq( (int)atomic_fetch_or(&a, 0xF0), 0x0F );
    test_eq( (int)atomic_load(&a), 0xFF );
    test_pass("c2011:opt:atomic_fetch_or");
    test_eq( (int)atomic_fetch_and(&a, 0x3C), 0xFF );
    test_eq( (int)atomic_load(&a), 0x3C );
    test_pass("c2011:opt:atomic_fetch_and");
    test_eq( (int)atomic_fetch_xor(&a, 0xFF), 0x3C );
    test_eq( (int)atomic_load(&a), 0xC3 );
    test_pass("c2011:opt:atomic_fetch_xor");
#endif
}

TEST_CASE(stdatomic, compare_exchange) {
#ifdef TST_STDATOMIC_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    atomic_int a;
    int expected;

    atomic_init(&a, 7);

    expected = 99;
    test_true( !atomic_compare_exchange_strong(&a, &expected, 123) );
    test_eq( expected, 7 );
    test_eq( (int)atomic_load(&a), 7 );

    expected = 7;
    test_true( atomic_compare_exchange_strong(&a, &expected, 123) );
    test_eq( (int)atomic_load(&a), 123 );
    test_pass("c2011:opt:atomic_compare_exchange_strong");

    expected = 123;
    while (!atomic_compare_exchange_weak(&a, &expected, 456))
        ;
    test_eq( (int)atomic_load(&a), 456 );
    test_pass("c2011:opt:atomic_compare_exchange_weak");
#endif
}

TEST_CASE(stdatomic, explicit_orders_and_fences) {
#ifdef TST_STDATOMIC_UNAVAILABLE
    TEST_SKIP_CASE();
#else
    atomic_int a;

    test_ne( (int)memory_order_relaxed, (int)memory_order_acquire );
    test_ne( (int)memory_order_release, (int)memory_order_acq_rel );
    test_ne( (int)memory_order_seq_cst, (int)memory_order_relaxed );
    test_pass("c2011:opt:memory_order enumerators");

    atomic_init(&a, 1);
    atomic_store_explicit(&a, 2, memory_order_release);
    test_pass("c2011:opt:atomic_store_explicit");
    test_eq( (int)atomic_load_explicit(&a, memory_order_acquire), 2 );
    test_pass("c2011:opt:atomic_load_explicit");
    test_eq( (int)atomic_fetch_add_explicit(&a, 1, memory_order_relaxed), 2 );
    test_eq( (int)atomic_load(&a), 3 );
    test_pass("c2011:opt:atomic_fetch_add_explicit");

    atomic_thread_fence(memory_order_seq_cst);
    test_true( 1 );
    test_pass("c2011:opt:atomic_thread_fence");
    atomic_signal_fence(memory_order_seq_cst);
    test_true( 1 );
    test_pass("c2011:opt:atomic_signal_fence");
#endif
}

TEST_CASE(stdatomic, atomic_typedefs_a_c2011) {
#if !defined(__STDC_NO_ATOMICS__) && defined(ATOMIC_INT_LOCK_FREE)
    { atomic_bool v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_bool");
    { atomic_char v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_char");
    { atomic_char16_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_char16_t");
    { atomic_char32_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_char32_t");
    { atomic_int_fast16_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_fast16_t");
    { atomic_int_fast32_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_fast32_t");
    { atomic_int_fast64_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_fast64_t");
    { atomic_int_fast8_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_fast8_t");
    { atomic_int_least16_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_least16_t");
    { atomic_int_least32_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_least32_t");
    { atomic_int_least64_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_least64_t");
    { atomic_int_least8_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_int_least8_t");
    { atomic_intmax_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_intmax_t");
    { atomic_intptr_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_intptr_t");
    { atomic_llong v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_llong");
    { atomic_long v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_long");
    { atomic_ptrdiff_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_ptrdiff_t");
    { atomic_schar v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_schar");
    { atomic_short v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_short");
    { atomic_size_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_size_t");
    { atomic_uchar v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uchar");
    { atomic_uint v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint");
    { atomic_uint_fast16_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_fast16_t");
    { atomic_uint_fast32_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_fast32_t");
#else
    TEST_NOTE("atomics are optional and absent here");
    test_skip("c2011:opt:atomic_bool");
    test_skip("c2011:opt:atomic_char");
    test_skip("c2011:opt:atomic_char16_t");
    test_skip("c2011:opt:atomic_char32_t");
    test_skip("c2011:opt:atomic_int_fast16_t");
    test_skip("c2011:opt:atomic_int_fast32_t");
    test_skip("c2011:opt:atomic_int_fast64_t");
    test_skip("c2011:opt:atomic_int_fast8_t");
    test_skip("c2011:opt:atomic_int_least16_t");
    test_skip("c2011:opt:atomic_int_least32_t");
    test_skip("c2011:opt:atomic_int_least64_t");
    test_skip("c2011:opt:atomic_int_least8_t");
    test_skip("c2011:opt:atomic_intmax_t");
    test_skip("c2011:opt:atomic_intptr_t");
    test_skip("c2011:opt:atomic_llong");
    test_skip("c2011:opt:atomic_long");
    test_skip("c2011:opt:atomic_ptrdiff_t");
    test_skip("c2011:opt:atomic_schar");
    test_skip("c2011:opt:atomic_short");
    test_skip("c2011:opt:atomic_size_t");
    test_skip("c2011:opt:atomic_uchar");
    test_skip("c2011:opt:atomic_uint");
    test_skip("c2011:opt:atomic_uint_fast16_t");
    test_skip("c2011:opt:atomic_uint_fast32_t");
#endif
}

TEST_CASE(stdatomic, atomic_typedefs_b_c2011) {
#if !defined(__STDC_NO_ATOMICS__) && defined(ATOMIC_INT_LOCK_FREE)
    { atomic_uint_fast64_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_fast64_t");
    { atomic_uint_fast8_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_fast8_t");
    { atomic_uint_least16_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_least16_t");
    { atomic_uint_least32_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_least32_t");
    { atomic_uint_least64_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_least64_t");
    { atomic_uint_least8_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uint_least8_t");
    { atomic_uintmax_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uintmax_t");
    { atomic_uintptr_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_uintptr_t");
    { atomic_ullong v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_ullong");
    { atomic_ulong v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_ulong");
    { atomic_ushort v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_ushort");
    { atomic_wchar_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2011:opt:atomic_wchar_t");
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    { atomic_char8_t v_; atomic_init(&v_, 0); test_true( atomic_load(&v_) == 0 ); }
    test_pass("c2023:opt:atomic_char8_t");
#else
    test_skip("c2023:opt:atomic_char8_t");
#endif
#else
    TEST_NOTE("atomics are optional and absent here");
    test_skip("c2011:opt:atomic_uint_fast64_t");
    test_skip("c2011:opt:atomic_uint_fast8_t");
    test_skip("c2011:opt:atomic_uint_least16_t");
    test_skip("c2011:opt:atomic_uint_least32_t");
    test_skip("c2011:opt:atomic_uint_least64_t");
    test_skip("c2011:opt:atomic_uint_least8_t");
    test_skip("c2011:opt:atomic_uintmax_t");
    test_skip("c2011:opt:atomic_uintptr_t");
    test_skip("c2011:opt:atomic_ullong");
    test_skip("c2011:opt:atomic_ulong");
    test_skip("c2011:opt:atomic_ushort");
    test_skip("c2011:opt:atomic_wchar_t");
    test_skip("c2023:opt:atomic_char8_t");
#endif
}

TEST_CASE(stdatomic, lock_free_macros_c2011) {
#if !defined(__STDC_NO_ATOMICS__) && defined(ATOMIC_INT_LOCK_FREE)
    test_true( (ATOMIC_BOOL_LOCK_FREE) >= 0 && (ATOMIC_BOOL_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_BOOL_LOCK_FREE");
    test_true( (ATOMIC_CHAR16_T_LOCK_FREE) >= 0 && (ATOMIC_CHAR16_T_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_CHAR16_T_LOCK_FREE");
    test_true( (ATOMIC_CHAR32_T_LOCK_FREE) >= 0 && (ATOMIC_CHAR32_T_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_CHAR32_T_LOCK_FREE");
    test_true( (ATOMIC_CHAR_LOCK_FREE) >= 0 && (ATOMIC_CHAR_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_CHAR_LOCK_FREE");
    test_true( (ATOMIC_INT_LOCK_FREE) >= 0 && (ATOMIC_INT_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_INT_LOCK_FREE");
    test_true( (ATOMIC_LLONG_LOCK_FREE) >= 0 && (ATOMIC_LLONG_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_LLONG_LOCK_FREE");
    test_true( (ATOMIC_LONG_LOCK_FREE) >= 0 && (ATOMIC_LONG_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_LONG_LOCK_FREE");
    test_true( (ATOMIC_POINTER_LOCK_FREE) >= 0 && (ATOMIC_POINTER_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_POINTER_LOCK_FREE");
    test_true( (ATOMIC_SHORT_LOCK_FREE) >= 0 && (ATOMIC_SHORT_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_SHORT_LOCK_FREE");
#if defined(ATOMIC_VAR_INIT)
    { atomic_int v_ = ATOMIC_VAR_INIT(7); test_true( atomic_load(&v_) == 7 ); }
    test_pass("c2011:opt:ATOMIC_VAR_INIT");
#else
    TEST_SKIP1();
    test_skip("c2011:opt:ATOMIC_VAR_INIT");
#endif
    test_true( (ATOMIC_WCHAR_T_LOCK_FREE) >= 0 && (ATOMIC_WCHAR_T_LOCK_FREE) <= 2 );
    test_pass("c2011:opt:ATOMIC_WCHAR_T_LOCK_FREE");
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    test_true( (ATOMIC_CHAR8_T_LOCK_FREE) >= 0 && (ATOMIC_CHAR8_T_LOCK_FREE) <= 2 );
    test_pass("c2023:opt:ATOMIC_CHAR8_T_LOCK_FREE");
#else
    test_skip("c2023:opt:ATOMIC_CHAR8_T_LOCK_FREE");
#endif
#else
    TEST_NOTE("atomics are optional and absent here");
    test_skip("c2011:opt:ATOMIC_BOOL_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_CHAR16_T_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_CHAR32_T_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_CHAR_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_INT_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_LLONG_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_LONG_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_POINTER_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_SHORT_LOCK_FREE");
    test_skip("c2011:opt:ATOMIC_VAR_INIT");
    test_skip("c2011:opt:ATOMIC_WCHAR_T_LOCK_FREE");
    test_skip("c2023:opt:ATOMIC_CHAR8_T_LOCK_FREE");
#endif
}

TEST_CASE(stdatomic, explicit_forms_c2011) {
#if !defined(__STDC_NO_ATOMICS__) && defined(ATOMIC_INT_LOCK_FREE)
    atomic_int a_;
    atomic_flag f_ = ATOMIC_FLAG_INIT;
    int exp_;
    atomic_init(&a_, 5);
    atomic_store(&a_, 6); exp_ = 6;
    test_true( atomic_compare_exchange_strong_explicit(&a_, &exp_, 9, memory_order_seq_cst, memory_order_seq_cst) );
    test_true( atomic_load(&a_) == 9 );
    atomic_store(&a_, 6);
    test_pass("c2011:opt:atomic_compare_exchange_strong_explicit");
    atomic_store(&a_, 6); exp_ = 6;
    while (!atomic_compare_exchange_weak_explicit(&a_, &exp_, 9, memory_order_seq_cst, memory_order_seq_cst)) exp_ = 6;
    test_true( atomic_load(&a_) == 9 );
    atomic_store(&a_, 6);
    test_pass("c2011:opt:atomic_compare_exchange_weak_explicit");
    atomic_store(&a_, 5);
    test_true( atomic_exchange_explicit(&a_, 6, memory_order_seq_cst) == 5 );
    test_pass("c2011:opt:atomic_exchange_explicit");
    atomic_store(&a_, 6);
    test_true( atomic_fetch_and_explicit(&a_, 3, memory_order_seq_cst) == 6 );
    test_pass("c2011:opt:atomic_fetch_and_explicit");
    atomic_store(&a_, 6);
    test_true( atomic_fetch_or_explicit(&a_, 1, memory_order_seq_cst) == 6 );
    test_pass("c2011:opt:atomic_fetch_or_explicit");
    atomic_store(&a_, 6);
    test_true( atomic_fetch_sub_explicit(&a_, 1, memory_order_seq_cst) == 6 );
    test_pass("c2011:opt:atomic_fetch_sub_explicit");
    atomic_store(&a_, 6);
    test_true( atomic_fetch_xor_explicit(&a_, 2, memory_order_seq_cst) == 6 );
    test_pass("c2011:opt:atomic_fetch_xor_explicit");
    atomic_flag_clear_explicit(&f_, memory_order_seq_cst);
    test_true( atomic_flag_test_and_set_explicit(&f_, memory_order_seq_cst) == 0 );
    test_pass("c2011:opt:atomic_flag_clear_explicit");
    atomic_flag_clear(&f_);
    test_true( atomic_flag_test_and_set_explicit(&f_, memory_order_seq_cst) == 0 );
    test_true( atomic_flag_test_and_set_explicit(&f_, memory_order_seq_cst) == 1 );
    test_pass("c2011:opt:atomic_flag_test_and_set_explicit");
    { int lf_ = atomic_is_lock_free(&a_); test_true( lf_ == 0 || lf_ == 1 ); }
    test_pass("c2011:opt:atomic_is_lock_free");
#else
    TEST_NOTE("atomics are optional and absent here");
    test_skip("c2011:opt:atomic_compare_exchange_strong_explicit");
    test_skip("c2011:opt:atomic_compare_exchange_weak_explicit");
    test_skip("c2011:opt:atomic_exchange_explicit");
    test_skip("c2011:opt:atomic_fetch_and_explicit");
    test_skip("c2011:opt:atomic_fetch_or_explicit");
    test_skip("c2011:opt:atomic_fetch_sub_explicit");
    test_skip("c2011:opt:atomic_fetch_xor_explicit");
    test_skip("c2011:opt:atomic_flag_clear_explicit");
    test_skip("c2011:opt:atomic_flag_test_and_set_explicit");
    test_skip("c2011:opt:atomic_is_lock_free");
#endif
}

TEST_CASE(stdatomic, header_gate_c2011) {
#if !defined(__STDC_NO_ATOMICS__) && defined(ATOMIC_INT_LOCK_FREE)
    TEST_NOTE("reaching this case means __STDC_NO_ATOMICS__ is undefined");
    test_true( (long)ATOMIC_INT_LOCK_FREE >= 0L && (long)ATOMIC_INT_LOCK_FREE <= 2L );
    test_pass("c2011:opt:__STDC_NO_ATOMICS__");

    {   int v = 7;
        int w = kill_dependency(v);
        test_eq( w, 7 );
        test_eq( kill_dependency(v + 1), 8 ); }
    test_pass("c2011:opt:kill_dependency");

    {   memory_order o = memory_order_relaxed;
        test_true( o == memory_order_relaxed );
        test_true( memory_order_relaxed != memory_order_seq_cst ); }
    test_pass("c2011:opt:memory_order_relaxed");
    test_true( memory_order_consume != memory_order_relaxed );
    test_pass("c2011:opt:memory_order_consume");
    test_true( memory_order_acquire != memory_order_relaxed );
    test_true( memory_order_acquire != memory_order_release );
    test_pass("c2011:opt:memory_order_acquire");
    test_true( memory_order_release != memory_order_acquire );
    test_pass("c2011:opt:memory_order_release");
    test_true( memory_order_acq_rel != memory_order_acquire );
    test_true( memory_order_acq_rel != memory_order_release );
    test_pass("c2011:opt:memory_order_acq_rel");
    test_true( memory_order_seq_cst != memory_order_acq_rel );
    test_pass("c2011:opt:memory_order_seq_cst");

#ifdef __STDC_VERSION_STDATOMIC_H__
    test_true( (long)__STDC_VERSION_STDATOMIC_H__ >= 202311L );
#else
    TEST_NOTE("C23 __STDC_VERSION_STDATOMIC_H__ not defined");
    TEST_SKIP1();
#endif
    test_pass("c2023:opt:__STDC_VERSION_STDATOMIC_H__");
#else
    TEST_NOTE("atomics are optional and absent here");
    test_skip("c2011:opt:__STDC_NO_ATOMICS__");
    test_skip("c2011:opt:kill_dependency");
    test_skip("c2011:opt:memory_order_acq_rel");
    test_skip("c2011:opt:memory_order_acquire");
    test_skip("c2011:opt:memory_order_consume");
    test_skip("c2011:opt:memory_order_relaxed");
    test_skip("c2011:opt:memory_order_release");
    test_skip("c2011:opt:memory_order_seq_cst");
    test_skip("c2023:opt:__STDC_VERSION_STDATOMIC_H__");
#endif
}

TEST_SUITE(stdatomic)
    TEST_ADD(flag)
    TEST_ADD(load_store_exchange)
    TEST_ADD(fetch_ops)
    TEST_ADD(compare_exchange)
    TEST_ADD(explicit_orders_and_fences)
    TEST_ADD(atomic_typedefs_a_c2011)
    TEST_ADD(atomic_typedefs_b_c2011)
    TEST_ADD(lock_free_macros_c2011)
    TEST_ADD(explicit_forms_c2011)
    TEST_ADD(header_gate_c2011)
TEST_SUITE_END(stdatomic)
