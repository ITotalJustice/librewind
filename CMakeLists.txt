cmake_minimum_required(VERSION 3.0.2)

project(rewind
    VERSION 1.0.0
    LANGUAGES C
)

option(COMPRESSOR_LZ4 "" OFF)
option(COMPRESSOR_RAW "" OFF)
option(COMPRESSOR_ZLIB "" OFF)
option(COMPRESSOR_ZSTD "" OFF)

option(TEST_LZ4 "build LZ4 test" OFF)
option(TEST_RAW "build RAW test" OFF)
option(TEST_ZLIB "build ZLIB test" OFF)
option(TEST_ZSTD "build ZSTD test" OFF)

    # -DCOMPRESSOR_LZ4=ON -DCOMPRESSOR_RAW=ON -DCOMPRESSOR_ZLIB=ON -DCOMPRESSOR_ZSTD=ON -DTEST_LZ4=ON -DTEST_RAW=ON -DTEST_ZLIB=ON -DTEST_ZSTD=ON
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
