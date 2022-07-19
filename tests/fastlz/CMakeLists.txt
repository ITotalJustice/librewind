cmake_minimum_required(VERSION 3.0.2)

project(test_fastlz LANGUAGES C)
add_executable(test_fastlz test_fastlz.c)

target_link_libraries(test_fastlz PRIVATE
    rewind
    compressor_fastlz
    test_framework
)

add_test(
    NAME test_fastlz
    COMMAND test_fastlz
)
