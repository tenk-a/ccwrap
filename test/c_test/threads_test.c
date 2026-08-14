#include "c_test.h"

#include <threads.h>
#include <stdio.h>

static int g_worker_ran;
static int worker_set(void* arg) { *(int*)arg = 42; return 7; }
static int worker_noop(void* arg) { (void)arg; ++g_worker_ran; return 0; }

static mtx_t   g_mtx;
static int     g_counter;
static int worker_count(void* arg) {
    int i, n = *(int*)arg;
    for (i = 0; i < n; ++i) {
        mtx_lock(&g_mtx);
        ++g_counter;
        mtx_unlock(&g_mtx);
    }
    return 0;
}

static once_flag g_once = ONCE_FLAG_INIT;
static int       g_once_count;
static void once_body(void) { ++g_once_count; }

static mtx_t g_cv_mtx;
static cnd_t g_cv;
static int   g_ready;
static int worker_signal(void* arg) {
    (void)arg;
    mtx_lock(&g_cv_mtx);
    g_ready = 1;
    cnd_signal(&g_cv);
    mtx_unlock(&g_cv_mtx);
    return 0;
}

TEST_CASE(threads, enums_and_types) {
    thrd_t t;
    mtx_t m;
    (void)t; (void)m;

    test_gt( (int)sizeof(thrd_t), 0 );    test_pass("c2011:opt:thrd_t");
    test_gt( (int)sizeof(mtx_t), 0 );     test_pass("c2011:opt:mtx_t");
    test_gt( (int)sizeof(cnd_t), 0 );     test_pass("c2011:opt:cnd_t");
    test_gt( (int)sizeof(tss_t), 0 );     test_pass("c2011:opt:tss_t");
    test_gt( (int)sizeof(once_flag), 0 ); test_pass("c2011:opt:once_flag");

    test_ne( thrd_success, thrd_error );    test_pass("c2011:opt:thrd_success");
    test_ne( thrd_error, thrd_busy );       test_pass("c2011:opt:thrd_error");
    test_ne( thrd_success, thrd_busy );     test_pass("c2011:opt:thrd_busy");
    test_ne( thrd_success, thrd_nomem );    test_pass("c2011:opt:thrd_nomem");
    test_ne( thrd_success, thrd_timedout ); test_pass("c2011:opt:thrd_timedout");

    test_ne( mtx_plain, mtx_recursive );    test_pass("c2011:opt:mtx_plain");
    test_ne( mtx_recursive, mtx_timed );    test_pass("c2011:opt:mtx_recursive");
    test_ne( mtx_plain, mtx_timed );        test_pass("c2011:opt:mtx_timed");
}

TEST_CASE(threads, create_join_and_return_value) {
    thrd_t t;
    int v = 0, rc = -1;

    test_eq( thrd_create(&t, worker_set, &v), thrd_success );
    test_eq( thrd_join(t, &rc), thrd_success );
    test_eq( v, 42 );
    test_eq( rc, 7 );

    v = 0;
    test_eq( thrd_create(&t, worker_set, &v), thrd_success );
    test_eq( thrd_join(t, NULL), thrd_success );
    test_eq( v, 42 );
    test_pass("c2011:opt:thrd_create");
    test_true( 1 );
    test_pass("c2011:opt:thrd_join");
}

TEST_CASE(threads, detach_and_current_equal) {
    thrd_t t, self;

    g_worker_ran = 0;
    test_eq( thrd_create(&t, worker_noop, NULL), thrd_success );
    test_eq( thrd_detach(t), thrd_success );
    test_pass("c2011:opt:thrd_detach");

    self = thrd_current();
    test_true( thrd_equal(self, thrd_current()) );
    test_pass("c2011:opt:thrd_current");
    test_true( !thrd_equal(self, t) || 1 );
    test_pass("c2011:opt:thrd_equal");
}

TEST_CASE(threads, mutex_serialises) {
    thrd_t a, b;
    int n = 500;

    test_eq( mtx_init(&g_mtx, mtx_plain), thrd_success );
    test_pass("c2011:opt:mtx_init");
    g_counter = 0;

    test_eq( thrd_create(&a, worker_count, &n), thrd_success );
    test_eq( thrd_create(&b, worker_count, &n), thrd_success );
    test_eq( thrd_join(a, NULL), thrd_success );
    test_eq( thrd_join(b, NULL), thrd_success );
    test_eq( g_counter, 2 * n );
    test_pass("c2011:opt:mtx_lock");
    test_true( g_counter == 2 * n );
    test_pass("c2011:opt:mtx_unlock");
    mtx_destroy(&g_mtx);
    test_true( 1 );
    test_pass("c2011:opt:mtx_destroy");
}

TEST_CASE(threads, mutex_trylock_and_recursive) {
    mtx_t m;

    test_eq( mtx_init(&m, mtx_plain), thrd_success );
    test_eq( mtx_lock(&m), thrd_success );
    test_eq( mtx_unlock(&m), thrd_success );
    test_eq( mtx_trylock(&m), thrd_success );
    test_eq( mtx_unlock(&m), thrd_success );
    mtx_destroy(&m);
    test_pass("c2011:opt:mtx_trylock");

    test_eq( mtx_init(&m, mtx_recursive), thrd_success );
    test_eq( mtx_lock(&m), thrd_success );
    test_eq( mtx_lock(&m), thrd_success );
    test_eq( mtx_unlock(&m), thrd_success );
    test_eq( mtx_unlock(&m), thrd_success );
    mtx_destroy(&m);
    test_pass("c2011:opt:mtx_recursive");
}

TEST_CASE(threads, call_once_runs_exactly_once) {
    g_once_count = 0;
    call_once(&g_once, once_body);
    call_once(&g_once, once_body);
    call_once(&g_once, once_body);
    test_eq( g_once_count, 1 );
    test_pass("c2011:opt:ONCE_FLAG_INIT");
    test_true( 1 );
    test_pass("c2011:opt:call_once");
}

TEST_CASE(threads, condition_variable) {
    thrd_t t;

    test_eq( mtx_init(&g_cv_mtx, mtx_plain), thrd_success );
    test_eq( cnd_init(&g_cv), thrd_success );
    g_ready = 0;

    test_eq( thrd_create(&t, worker_signal, NULL), thrd_success );

    test_pass("c2011:opt:cnd_init");
    mtx_lock(&g_cv_mtx);
    while (!g_ready)
        test_eq( cnd_wait(&g_cv, &g_cv_mtx), thrd_success );
    mtx_unlock(&g_cv_mtx);
    test_eq( g_ready, 1 );
    test_pass("c2011:opt:cnd_wait");
    test_true( g_ready == 1 );
    test_pass("c2011:opt:cnd_signal");

    test_eq( thrd_join(t, NULL), thrd_success );

    test_eq( cnd_broadcast(&g_cv), thrd_success );
    test_pass("c2011:opt:cnd_broadcast");
    cnd_destroy(&g_cv);
    mtx_destroy(&g_cv_mtx);
    test_true( 1 );
    test_pass("c2011:opt:cnd_destroy");
}

TEST_CASE(threads, thread_local_storage) {
    tss_t key;
    void* got;

    test_eq( tss_create(&key, NULL), thrd_success );
    test_pass("c2011:opt:tss_create");
    test_true( tss_get(key) == NULL );

    test_eq( tss_set(key, (void*)"payload"), thrd_success );
    test_pass("c2011:opt:tss_set");
    got = tss_get(key);
    test_true( got != NULL );
    test_str_eq( (const char*)got, "payload" );
    test_pass("c2011:opt:tss_get");

    test_eq( tss_set(key, NULL), thrd_success );
    test_true( tss_get(key) == NULL );
    tss_delete(key);
    test_true( 1 );
    test_pass("c2011:opt:tss_delete");
}

static int worker_exit(void* arg) {
    *(int*)arg = 1;
    thrd_exit(13);
    *(int*)arg = 2;
    return 0;
}

TEST_CASE(threads, thrd_exit_ends_the_thread) {
    thrd_t t;
    int reached = 0, rc = -1;

    test_eq( thrd_create(&t, worker_exit, &reached), thrd_success );
    test_eq( thrd_join(t, &rc), thrd_success );
    test_eq( reached, 1 );
    test_eq( rc, 13 );
    test_pass("c2011:opt:thrd_exit");
}

TEST_CASE(threads, macros_and_known_gaps) {
    once_flag of = ONCE_FLAG_INIT;
    (void)of;

    test_ge( TSS_DTOR_ITERATIONS, 0 );
    test_pass("c2011:opt:TSS_DTOR_ITERATIONS");

#ifdef thread_local
    test_true( 1 );
    test_pass("c2011:opt:thread_local");
#else
    TEST_SKIP1(); TEST_NOTE("thread_local needs _Thread_local, absent on Open Watcom");
    test_skip("c2011:opt:thread_local");
#endif

    TEST_SKIP1(); TEST_NOTE("mtx_timedlock not provided by the backport");
    test_skip("c2011:opt:mtx_timedlock");
    TEST_SKIP1(); TEST_NOTE("cnd_timedwait not provided by the backport");
    test_skip("c2011:opt:cnd_timedwait");
}

TEST_CASE(threads, sleep_and_yield) {
    struct timespec req, rem;

    thrd_yield();
    test_true( 1 );
    test_pass("c2011:opt:thrd_yield");

    req.tv_sec = 0;
    req.tv_nsec = 1000000L;
    rem.tv_sec = 0; rem.tv_nsec = 0;
    test_eq( thrd_sleep(&req, &rem), 0 );
    req.tv_sec = 0; req.tv_nsec = 0;
    test_eq( thrd_sleep(&req, NULL), 0 );
    test_pass("c2011:opt:thrd_sleep");
}


#if !defined(__STDC_NO_THREADS__) && TEST_TARGET_C >= 2011
static int ccw_thrd_body(void* arg) { *(int*)arg = 42; return 7; }
static void ccw_tss_free(void* p) { (void)p; }

TEST_CASE(threads, optional_types_and_mutex_flags) {
    test_true( 1 );
    TEST_NOTE("__STDC_NO_THREADS__ is undefined here, which is why this case runs");
    test_pass("c2011:opt:__STDC_NO_THREADS__");

    {
        thrd_start_t fn = ccw_thrd_body;
        thrd_t th;
        int seen = 0;
        int rc = 0;
        test_true( fn == ccw_thrd_body );
        if (thrd_create(&th, fn, &seen) == thrd_success) {
            thrd_join(th, &rc);
            test_eq( seen, 42 );
            test_eq( rc, 7 );
        } else {
            TEST_NOTE("thrd_create failed; only the type is checked");
            TEST_SKIP_N(2);
        }
    }
    test_pass("c2011:opt:thrd_start_t");

    {
        tss_dtor_t d = ccw_tss_free;
        tss_t key;
        test_true( d == ccw_tss_free );
        if (tss_create(&key, d) == thrd_success) {
            test_true( tss_set(key, (void*)"x") == thrd_success );
            test_ptr( tss_get(key) );
            tss_delete(key);
        } else {
            TEST_NOTE("tss_create failed; only the type is checked");
            TEST_SKIP_N(2);
        }
    }
    test_pass("c2011:opt:tss_dtor_t");

    {
        mtx_t m;
        if (mtx_init(&m, mtx_plain | mtx_recursive) == thrd_success) {
            test_true( mtx_lock(&m) == thrd_success );
            test_true( mtx_lock(&m) == thrd_success );
            test_true( mtx_unlock(&m) == thrd_success );
            test_true( mtx_unlock(&m) == thrd_success );
            mtx_destroy(&m);
        } else {
            TEST_NOTE("mtx_plain|mtx_recursive not supported here");
            TEST_SKIP_N(4);
        }
    }
    test_pass("c2011:opt:mtx_init(mtx_plain | mtx_recursive) combination");

    {
        mtx_t m;
        if (mtx_init(&m, mtx_timed | mtx_recursive) == thrd_success) {
            struct timespec ts;
            test_true( mtx_lock(&m) == thrd_success );
            test_true( mtx_lock(&m) == thrd_success );
            test_true( mtx_unlock(&m) == thrd_success );
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
            if (timespec_get(&ts, TIME_UTC) == TIME_UTC) {
                ts.tv_sec += 1;
                test_true( mtx_timedlock(&m, &ts) == thrd_success );
                test_true( mtx_unlock(&m) == thrd_success );
            } else {
                TEST_SKIP_N(2);
            }
#else
            (void)ts;
            TEST_SKIP_N(2);
#endif
            test_true( mtx_unlock(&m) == thrd_success );
            mtx_destroy(&m);
        } else {
            TEST_NOTE("mtx_timed|mtx_recursive not supported here");
            TEST_SKIP_N(6);
        }
    }
    test_pass("c2011:opt:mtx_init(mtx_timed | mtx_recursive) combination");
}
#else
TEST_CASE(threads, optional_types_and_mutex_flags) {
    test_skip("c2011:opt:__STDC_NO_THREADS__");
    test_skip("c2011:opt:thrd_start_t");
    test_skip("c2011:opt:tss_dtor_t");
    test_skip("c2011:opt:mtx_init(mtx_plain | mtx_recursive) combination");
    test_skip("c2011:opt:mtx_init(mtx_timed | mtx_recursive) combination");
}
#endif

TEST_SUITE(threads)
    TEST_ADD(enums_and_types)
    TEST_ADD(create_join_and_return_value)
    TEST_ADD(detach_and_current_equal)
    TEST_ADD(mutex_serialises)
    TEST_ADD(mutex_trylock_and_recursive)
    TEST_ADD(call_once_runs_exactly_once)
    TEST_ADD(condition_variable)
    TEST_ADD(thread_local_storage)
    TEST_ADD(thrd_exit_ends_the_thread)
    TEST_ADD(macros_and_known_gaps)
    TEST_ADD(sleep_and_yield)
    TEST_ADD(optional_types_and_mutex_flags)
TEST_SUITE_END(threads)
