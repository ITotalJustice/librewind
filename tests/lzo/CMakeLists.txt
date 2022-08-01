cmake_minimum_required(VERSION 3.13)

project(test_lzo LANGUAGES C)
add_executable(test_lzo test_lzo.c)

target_link_libraries(test_lzo PRIVATE
    rewind
    compressor_lzo
    test_framework
)

add_test(
    NAME test_lzo
    COMMAND test_lzo
)
