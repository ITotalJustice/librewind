cmake_minimum_required(VERSION 3.0.2)

project(test_zlib LANGUAGES C)
add_executable(test_zlib test_zlib.c)

target_link_libraries(test_zlib PRIVATE
    rewind
    compressor_zlib
    test_framework
)

add_test(
    NAME test_zlib
    COMMAND test_zlib
)
