cmake_minimum_required(VERSION 3.13)

project(compressor_lzo LANGUAGES C)
add_library(compressor_lzo compressor_lzo.c)
target_include_directories(compressor_lzo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(compressor_lzo PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)

set(FOUND_lzo FALSE)

if (NOT FOUND_lzo)
    find_library(lzo_lib lzo2)
    find_path(lzo_inc lzo)

    if (lzo_lib AND lzo_inc)
        target_link_libraries(compressor_lzo PRIVATE ${lzo_lib})
        target_include_directories(compressor_lzo PRIVATE ${lzo_inc})

        set(FOUND_lzo TRUE)
        message(STATUS "using system lzo")
    endif()
endif()

if (NOT FOUND_lzo)
    message(FATAL_ERROR "could not find lzo")
endif()
