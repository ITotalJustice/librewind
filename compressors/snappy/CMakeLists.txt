cmake_minimum_required(VERSION 3.0.2)

project(compressor_snappy LANGUAGES CXX)
add_library(compressor_snappy compressor_snappy.cpp)
target_include_directories(compressor_snappy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(compressor_snappy PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
    >
)

find_package(Snappy CONFIG REQUIRED)
target_link_libraries(compressor_snappy PRIVATE Snappy::snappy)
