cmake_minimum_required(VERSION 3.0.2)

project(rewind
    VERSION 1.1.0
    LANGUAGES C
)

option(COMPRESSOR_LZ4 "" OFF)
option(COMPRESSOR_RAW "" OFF)
option(COMPRESSOR_ZLIB "" OFF)
option(COMPRESSOR_ZSTD "" OFF)
option(COMPRESSOR_SNAPPY "" OFF)
option(COMPRESSOR_FASTLZ "" OFF)
option(COMPRESSOR_LZO "" OFF)

option(TEST_LZ4 "build LZ4 test" OFF)
option(TEST_RAW "build RAW test" OFF)
option(TEST_ZLIB "build ZLIB test" OFF)
option(TEST_ZSTD "build ZSTD test" OFF)
option(TEST_SNAPPY "build SNAPPY test" OFF)
option(TEST_FASTLZ "build FASTLZ test" OFF)
option(TEST_LZO "build LZO test" OFF)
option(TEST_ALL "builds all tests" OFF)

if (TEST_ALL)
    set(COMPRESSOR_LZ4 ON)
    set(TEST_LZ4 ON)

    set(COMPRESSOR_RAW ON)
    set(TEST_RAW ON)

    set(COMPRESSOR_ZLIB ON)
    set(TEST_ZLIB ON)

    set(COMPRESSOR_ZSTD ON)
    set(TEST_ZSTD ON)

    set(COMPRESSOR_SNAPPY ON)
    set(TEST_SNAPPY ON)

    set(COMPRESSOR_FASTLZ ON)
    set(TEST_FASTLZ ON)

    set(COMPRESSOR_LZO ON)
    set(TEST_LZO ON)
endif()

# export compile commands
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# enable testing
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

add_subdirectory(src)
add_subdirectory(compressors)
