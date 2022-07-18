cmake_minimum_required(VERSION 3.0.2)

project(test_zstd LANGUAGES C)
add_executable(test_zstd test_zstd.c)

target_link_libraries(test_zstd PRIVATE
    rewind
    compressor_zstd
    test_framework
)

add_test(
    NAME test_zstd
    COMMAND test_zstd
)
