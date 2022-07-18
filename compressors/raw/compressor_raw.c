/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <stddef.h>
#include <string.h>

#define COMPRESS_ERROR ((size_t)-1)

size_t compressor_raw_size(const size_t src_size)
{
    return src_size;
}

size_t compressor_raw(void* dst_data, const size_t dst_size, const void* src_data, const size_t src_size, int mode)
{
    (void)mode;

    if (!dst_data || !dst_size || !src_data || !src_size)
    {
        return COMPRESS_ERROR;
    }

    if (dst_size != src_size)
    {
        return COMPRESS_ERROR;
    }

    memcpy(dst_data, src_data, dst_size);
    return dst_size;
}
