# Digital Mars C/C++ toolchain for CMake. (ninja)

if(DEFINED ENV{DMC_ROOT})
    file(TO_CMAKE_PATH "$ENV{DMC_ROOT}" _dmc_root)
    if(NOT IS_DIRECTORY "${_dmc_root}")
        message(FATAL_ERROR "DMC_ROOT does not point to a directory: ${_dmc_root}")
    endif()
elseif(IS_DIRECTORY "C:/dm")
    set(_dmc_root "C:/dm")
elseif(IS_DIRECTORY "C:/dmc")
    set(_dmc_root "C:/dmc")
else()
    message(FATAL_ERROR "DMC was not found. Set the DMC_ROOT environment variable, or install it in C:/dm or C:/dmc.")
endif()

set(DMC_ROOT "${_dmc_root}" CACHE PATH "Digital Mars C/C++ installation directory" FORCE)
unset(_dmc_root)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER   "${DMC_ROOT}/bin/dmc.exe"     CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER "${DMC_ROOT}/bin/dmc.exe"     CACHE FILEPATH "")
set(CMAKE_AR           "${DMC_ROOT}/bin/lib.exe"     CACHE FILEPATH "")
set(CMAKE_LINKER       "${DMC_ROOT}/bin/optlink.exe" CACHE FILEPATH "")

set(CMAKE_C_OUTPUT_EXTENSION    ".obj")
set(CMAKE_CXX_OUTPUT_EXTENSION  ".obj")
set(CMAKE_EXECUTABLE_SUFFIX     ".exe")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")

set(CMAKE_C_COMPILE_OBJECT          "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -c -o<OBJECT> <SOURCE>")
set(CMAKE_CXX_COMPILE_OBJECT        "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -c -o<OBJECT> <SOURCE>")

set(CMAKE_C_LINK_EXECUTABLE         "<CMAKE_C_COMPILER> <FLAGS> <LINK_FLAGS> <OBJECTS> -o<TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE       "<CMAKE_CXX_COMPILER> <FLAGS> <LINK_FLAGS> <OBJECTS> -o<TARGET> <LINK_LIBRARIES>")

set(CMAKE_C_CREATE_STATIC_LIBRARY   "<CMAKE_AR> -c <TARGET> <OBJECTS>")
set(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> -c <TARGET> <OBJECTS>")

set(CMAKE_C_FLAGS_DEBUG_INIT     "-g -o+none")
set(CMAKE_CXX_FLAGS_DEBUG_INIT   "-g -o+none")
set(CMAKE_C_FLAGS_RELEASE_INIT   "-o -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-o -DNDEBUG")

get_filename_component(_dmc_toolchain_file "${CMAKE_CURRENT_LIST_FILE}" ABSOLUTE)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_C   "${_dmc_toolchain_file}" CACHE FILEPATH "")
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX "${_dmc_toolchain_file}" CACHE FILEPATH "")
