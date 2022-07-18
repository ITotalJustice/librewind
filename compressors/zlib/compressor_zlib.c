/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <zlib.h>
#include <stddef.h>

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

size_t compressor_zlib_size(size_t src_size)
{
    return compressBound(src_size);
}

size_t compressor_zlib(void* dst_data, const size_t dst_size, const void* src_data, const size_t src_size, int mode)
{
    int result = 0;
    uLongf output_size = dst_size;

    if (mode == CompressMode_DEFLATE)
    {
        result = compress(dst_data, &output_size, src_data, src_size);
    }
    else
    {
        result = uncompress(dst_data, &output_size, src_data, src_size);
    }

    if (result != Z_OK)
    {
        return COMPRESS_ERROR;
    }

    return output_size;
}
