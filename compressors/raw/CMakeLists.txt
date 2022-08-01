cmake_minimum_required(VERSION 3.13)

project(compressor_raw LANGUAGES C)
add_library(compressor_raw compressor_raw.c)
target_include_directories(compressor_raw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(compressor_raw PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)
