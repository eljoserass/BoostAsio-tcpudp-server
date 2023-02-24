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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/boost)

if (tcud_BUILD_CLIENT)
    message(STATUS "Building tcud's Client module.")
    add_subdirectory(client)
endif()

if (tcud_BUILD_SERVER)
    message(STATUS "Building tcud's Server module.")
    add_subdirectory(server)
endif()
