cmake_minimum_required(VERSION 3.0.2)

if (COMPRESSOR_LZ4)
    add_subdirectory(lz4)
endif()

if (COMPRESSOR_RAW)
    add_subdirectory(raw)
endif()

if (COMPRESSOR_ZLIB)
    add_subdirectory(zlib)
endif()

if (COMPRESSOR_ZSTD)
    add_subdirectory(zstd)
endif()

if (COMPRESSOR_SNAPPY)
    add_subdirectory(snappy)
endif()

if (COMPRESSOR_FASTLZ)
    add_subdirectory(fastlz)
endif()
