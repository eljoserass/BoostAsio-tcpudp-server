cmake_minimum_required(VERSION 3.0)

project(p)

macro(tcud_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# set default modules to build
tcud_set_option(tcud_BUILD_CLIENT TRUE BOOL "TRUE to build tcud's Client module.")
tcud_set_option(tcud_BUILD_SERVER TRUE BOOL "TRUE to build tcud's Server module.")

include(FetchContent)

# BOOST

# set boost include and library directory
Set(BOOST_ENABLE_CMAKE ON)
set(BOOST_ROOT ${CMAKE_BINARY_DIR}/_deps/boost-src)
set(Boost_INCLUDE_DIR ${BOOST_ROOT})
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_DEBUG ON)

# set(BOOST_REQD_SUBMODULES
#     "tools/cmake;libs/integer;libs/move;libs/concept_check;libs/exception;libs/"
#     "libs/any"
#     "libs/asio;libs/align;libs/bind;libs/chrono;libs/config;libs/coroutine;libs/date_time;libs/function;libs/regex;libs/smart_ptr;libs/type_traits;libs/utility"
#     "libs/array;libs/assert;libs/config;libs/core;libs/static_assert;libs/throw_exception"
#     "libs/algorithm"
#     "libs/system"
#     "libs/endian;libs/phoenix;libs/proto"
#     )

Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    boost
    GIT_REPOSITORY "https://github.com/boostorg/boost.git"
    GIT_TAG master
    GIT_PROGRESS TRUE
    CONFIGURE_COMMAND ""  # tell CMake it's not a cmake project
)

FetchContent_GetProperties(boost)
FetchContent_MakeAvailable(boost)
if (NOT boost_POPULATED)
    FetchContent_Populate(boost)
endif()
# find_package(Boost)

if (tcud_BUILD_CLIENT)
    message(STATUS "Building tcud's Client module.")
    add_subdirectory(client)
endif()

if (tcud_BUILD_SERVER)
    message(STATUS "Building tcud's Server module.")
    add_subdirectory(server)
endif()
