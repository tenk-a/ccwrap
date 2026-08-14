# ccwrap: CMake toolchain for Open Watcom, 32-bit DOS (DOS/4GW).
#
# Only the TARGET is set here. The ccwrap wiring (include path, forced header,
# _CCW_TARGET_*) belongs to test/bin/ccwrap_test.cmake, which every suite includes --
# putting it in a toolchain as well would apply it twice and with the wrong paths.
#
# `-bt=<target>` is what actually retargets the compiler. Defining __DOS__ by hand does
# NOT: the objects still come out for the host target and the link then fails on Win32
# imports (measured). CMake's Watcom support turns CMAKE_SYSTEM_NAME "DOS" into
# `wlink ... system dos4g` on its own, so the linker side needs nothing here.

set(CMAKE_SYSTEM_NAME "DOS")
set(CMAKE_SYSTEM_PROCESSOR "X86")
set(CMAKE_C_COMPILER "wcl386")
set(CMAKE_CXX_COMPILER "wcl386")

# DOS has no threads, so there is no multi-threaded runtime to pick.
set(CMAKE_WATCOM_RUNTIME_LIBRARY "SingleThreaded")

set(CMAKE_C_FLAGS   "-bt=dos ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-bt=dos ${CMAKE_CXX_FLAGS}")

include(${CMAKE_CURRENT_LIST_DIR}/watcom_incl.cmake)
