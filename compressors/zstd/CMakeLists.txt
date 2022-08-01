cmake_minimum_required(VERSION 3.13)

project(compressor_zstd LANGUAGES C)
add_library(compressor_zstd compressor_zstd.c)
target_include_directories(compressor_zstd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(compressor_zstd PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)

set(FOUND_ZSTD FALSE)

if (VCPKG_TOOLCHAIN)
    find_package(zstd CONFIG)
    if (DEFINED zstd_FOUND)
        target_link_libraries(compressor_zstd PRIVATE $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>)

        set(FOUND_ZSTD TRUE)
        message(STATUS "using vcpkg zstd")
    endif()
endif()

if (NOT FOUND_ZSTD)
    find_package(zstd CONFIG QUIET)
    if (zstd_FOUND)
        target_link_libraries(compressor_zstd PRIVATE zstd::zstd)

        set(FOUND_ZSTD TRUE)
        message(STATUS "using system zstd with find package")
    endif()
endif()

if (NOT FOUND_ZSTD)
    find_library(zstd_lib zstd)
    find_path(zstd_inc zstd)

    if (zstd_lib AND zstd_inc)
        target_link_libraries(compressor_zstd PRIVATE ${zstd_lib})
        target_include_directories(compressor_zstd PRIVATE ${zstd_inc})

        set(FOUND_ZSTD TRUE)
        message(STATUS "using system zstd")
    endif()
endif()

if (NOT FOUND_ZSTD)
    message(FATAL_ERROR "could not find zstd")
endif()
