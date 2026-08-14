# Orange C/C++ for Windows.  CMake already has native OrangeC compiler and
# platform modules, so this file only locates the installation.
# Override with -DORANGEC_ROOT=... or the ORANGEC_DIR environment variable.
if(DEFINED ENV{ORANGEC_DIR} AND NOT "$ENV{ORANGEC_DIR}" STREQUAL "")
    file(TO_CMAKE_PATH "$ENV{ORANGEC_DIR}" _orangec_root_default)
else()
    set(_orangec_root_default "C:/Program Files (x86)/OrangeC")
endif()
set(ORANGEC_ROOT "${_orangec_root_default}" CACHE PATH
    "Orange C/C++ installation root")

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER "${ORANGEC_ROOT}/bin/occ.exe" CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER "${ORANGEC_ROOT}/bin/occ.exe" CACHE FILEPATH "")
