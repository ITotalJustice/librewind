cmake_minimum_required(VERSION 3.13)

project(compressor_lz4 LANGUAGES C)
add_library(compressor_lz4 compressor_lz4.c)
target_include_directories(compressor_lz4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(compressor_lz4 PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)

set(FOUND_LZ4 FALSE)

if (VCPKG_TOOLCHAIN)
    find_package(lz4 CONFIG)
    if (DEFINED lz4_FOUND)
        target_link_libraries(compressor_lz4 PRIVATE lz4::lz4)

        set(FOUND_LZ4 TRUE)
        message(STATUS "using vcpkg lz4")
    endif()
endif()

if (NOT FOUND_LZ4)
    find_package(lz4 CONFIG QUIET)
    if (lz4_FOUND)
        target_link_libraries(compressor_lz4 PRIVATE lz4::lz4)

        set(FOUND_LZ4 TRUE)
        message(STATUS "using system lz4")
    endif()
endif()

if (NOT FOUND_LZ4)
    find_library(lz4_lib lz4)
    find_path(lz4_inc lz4)

    if (lz4_lib)
        target_link_libraries(compressor_lz4 PRIVATE ${lz4_lib})
        if (lz4_inc)
            target_include_directories(compressor_lz4 PRIVATE ${lz4_inc})
        endif()

        set(FOUND_LZ4 TRUE)
        message(STATUS "using system lz4")
    endif()
endif()

if (NOT FOUND_LZ4)
    message(FATAL_ERROR "could not find lz4")
endif()
