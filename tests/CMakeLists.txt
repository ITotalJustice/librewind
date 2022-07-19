cmake_minimum_required(VERSION 3.0.2)

project(test_framework LANGUAGES C)
add_library(test_framework test_framework.c)
target_link_libraries(test_framework PRIVATE rewind)
target_include_directories(test_framework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (TEST_LZ4)
    add_subdirectory(lz4)
endif()

if (TEST_RAW)
    add_subdirectory(raw)
endif()

if (TEST_ZLIB)
    add_subdirectory(zlib)
endif()

if (TEST_ZSTD)
    add_subdirectory(zstd)
endif()

if (TEST_SNAPPY)
    add_subdirectory(snappy)
endif()

if (TEST_FASTLZ)
    add_subdirectory(fastlz)
endif()
