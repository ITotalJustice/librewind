/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <fastlz.h>
#include <stddef.h>

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

size_t compressor_fastlz_size(const size_t src_size)
{
    /*
        From fastlz.h

        The output buffer must be at least 5% larger than the input buffer
        and can not be smaller than 66 bytes.
    */
    const size_t size = src_size + (src_size / 5);
    return size < 66 ? 66 : size;
}

size_t compressor_fastlz(void* dst_data, const size_t dst_size, const void* src_data, const size_t src_size, int mode)
{
    int result;

    if (mode == CompressMode_DEFLATE)
    {
        result = fastlz_compress_level(1, src_data, src_size, dst_data);
    }
    else
    {
        result = fastlz_decompress(src_data, src_size, dst_data, dst_size);
    }

    if (result <= 0)
    {
        return COMPRESS_ERROR;
    }

    return result;
}
