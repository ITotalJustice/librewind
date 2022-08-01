cmake_minimum_required(VERSION 3.13)

project(compressor_zlib LANGUAGES C)
add_library(compressor_zlib compressor_zlib.c)
target_include_directories(compressor_zlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(compressor_zlib PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)

find_package(ZLIB REQUIRED)
target_link_libraries(compressor_zlib PRIVATE ZLIB::ZLIB)
