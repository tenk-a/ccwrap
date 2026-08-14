/* The C++ side of watcom_win32_coexist_test.c: every ccwrap header that declares a
   Win32 API has to agree with <windows.h>, or the two cannot appear in one program.
   extern "C" functions do not overload, so a disagreement is a hard error rather
   than a silently wrong call. Both include orders are covered here. */
#include "watcom_win32_decls.h"

#include <mutex>
#include <condition_variable>
#include <shared_mutex>
#include <thread>
#include <future>
#include <latch>
#include <barrier>
#include <semaphore>
#include <stacktrace>
#include <debugging>

#include <ctime>
#include <cstdio>

int main()
{
    FILETIME ft;
    ft.dwLowDateTime = 0;
    ft.dwHighDateTime = 0;
    GetSystemTimeAsFileTime(&ft);
    if (ft.dwLowDateTime == 0 && ft.dwHighDateTime == 0) {
        std::printf("win32_coexist_cxx: GetSystemTimeAsFileTime gave 0\n");
        return 1;
    }

    std::timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = -1;
    if (std::timespec_get(&ts, TIME_UTC) != TIME_UTC || ts.tv_nsec < 0) {
        std::printf("win32_coexist_cxx: timespec_get failed\n");
        return 1;
    }

    std::mutex m;
    m.lock();
    m.unlock();
    if (!m.try_lock()) {
        std::printf("win32_coexist_cxx: try_lock failed\n");
        return 1;
    }
    m.unlock();

    std::shared_mutex sm;
    sm.lock_shared();
    sm.unlock_shared();

    std::condition_variable cv;
    cv.notify_all();

    std::printf("win32_coexist_cxx: ok\n");
    return 0;
}
