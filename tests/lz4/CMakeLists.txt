cmake_minimum_required(VERSION 3.13)

project(test_lz4 LANGUAGES C)
add_executable(test_lz4 test_lz4.c)

target_link_libraries(test_lz4 PRIVATE
    rewind
    compressor_lz4
    test_framework
)

add_test(
    NAME test_lz4
    COMMAND test_lz4
)
