/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <lz4.h>
#include <stddef.h>

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

size_t compressor_lz4_size(const size_t src_size)
{
    return LZ4_compressBound(src_size);
}

size_t compressor_lz4(void* dst_data, const size_t dst_size, const void* src_data, const size_t src_size, int mode)
{
    if (mode == CompressMode_DEFLATE)
    {
        const size_t result = LZ4_compress_default(src_data, dst_data, src_size, dst_size);

        if (!result)
        {
            return COMPRESS_ERROR;
        }

        return result;
    }
    else
    {
        // returns -1 on error, so, same as COMPRESS_ERROR
        return LZ4_decompress_safe(src_data, dst_data, src_size, dst_size);
    }
}