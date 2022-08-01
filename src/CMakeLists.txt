cmake_minimum_required(VERSION 3.13)

add_library(rewind rewind.c)

target_include_directories(rewind PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(rewind PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)
