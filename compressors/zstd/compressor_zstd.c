/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <zstd.h>
#include <stddef.h>

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

size_t compressor_zstd_size(const size_t src_size)
{
    return ZSTD_compressBound(src_size);
}

size_t compressor_zstd(void* dst_data, const size_t dst_size, const void* src_data, const size_t src_size, int mode)
{
    size_t result = 0;

    if (mode == CompressMode_DEFLATE)
    {
        result = ZSTD_compress(dst_data, dst_size, src_data, src_size, ZSTD_CLEVEL_DEFAULT);
    }
    else
    {
        result = ZSTD_decompress(dst_data, dst_size, src_data, src_size);
    }

    if (ZSTD_isError(result))
    {
        return COMPRESS_ERROR;
    }

    return result;
}
