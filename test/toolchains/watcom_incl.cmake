# ccwrap: shared tail of the Open Watcom toolchain files.
#
# TOOLCHAIN_USE_CCWRAP is for OTHER projects that vendor ccwrap under thirdparty/ and want
# the toolchain to wire it up. Inside ccwrap's own tree it must stay OFF: the suites get
# their include path and forced header from test/bin/ccwrap_test.cmake, and doing it
# here as well would pass both twice.
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".exe")
set(CMAKE_EXECUTABLE_SUFFIX_C ".exe")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".exe")

set(ADD_OPTS "")
if(TOOLCHAIN_USE_CCWRAP)
    set(CCWRAP_DIR "${CMAKE_CURRENT_LIST_DIR}/../thirdparty/ccwrap")
    if(EXISTS "${CCWRAP_DIR}" AND IS_DIRECTORY "${CCWRAP_DIR}")
      # watcom/std is the whole include path (watcom/ccwstd for the relocated build), and
      # the forced header is watcom's own -- not vc's.
      set(ADD_OPTS "-i=${CCWRAP_DIR}/watcom/std -fi=${CCWRAP_DIR}/watcom/std/ccwrap_header.h ${ADD_OPTS}")
    else()
      message(WARNING "thirdparty/ccwrap is missing; building without it")
      unset(TOOLCHAIN_USE_CCWRAP)
    endif()
endif()

if(NOT TOOLCHAIN_USE_CCWRAP)
    # Without ccwrap the C++03 front end has none of these keywords. Blanking them lets
    # ordinary C++11-flavoured sources through; ccwrap defines them properly instead.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dnoexcept=")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dconstexpr=const")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Doverride=")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dfinal=")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dnullptr=0")
endif()

set(CMAKE_C_FLAGS   "${ADD_OPTS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${ADD_OPTS} ${CMAKE_CXX_FLAGS}")
