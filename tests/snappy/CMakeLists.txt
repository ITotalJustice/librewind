cmake_minimum_required(VERSION 3.13)

project(test_snappy LANGUAGES C CXX)
add_executable(test_snappy test_snappy.c)

target_link_libraries(test_snappy PRIVATE
    rewind
    compressor_snappy
    test_framework
)

add_test(
    NAME test_snappy
    COMMAND test_snappy
)
