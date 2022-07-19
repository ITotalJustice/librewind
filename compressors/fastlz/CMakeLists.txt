cmake_minimum_required(VERSION 3.0.2)

project(compressor_fastlz LANGUAGES C)
add_library(compressor_fastlz compressor_fastlz.c)
target_include_directories(compressor_fastlz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(compressor_fastlz PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)

set(FOUND_fastlz FALSE)

if (NOT FOUND_fastlz)
    find_library(fastlz_lib fastlz)
    find_path(fastlz_inc fastlz.h)

    if (fastlz_lib AND fastlz_inc)
        target_link_libraries(compressor_fastlz PRIVATE ${fastlz_lib})
        target_include_directories(compressor_fastlz PRIVATE ${fastlz_inc})

        set(FOUND_fastlz TRUE)
        message(STATUS "using system fastlz")
    endif()
endif()

if (NOT FOUND_fastlz)
    message(FATAL_ERROR "could not find fastlz")
endif()
