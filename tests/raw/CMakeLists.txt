cmake_minimum_required(VERSION 3.0.2)

project(test_raw LANGUAGES C)
add_executable(test_raw test_raw.c)

target_link_libraries(test_raw PRIVATE
    rewind
    compressor_raw
    test_framework
)

add_test(
    NAME test_raw
    COMMAND test_raw
)
