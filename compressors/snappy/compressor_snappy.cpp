// Copyright 2022 TotalJustice.
// SPDX-License-Identifier: Zlib

#include <snappy.h>
#include <stddef.h>

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

extern "C" size_t compressor_snappy_size(const size_t src_size)
{
    return snappy::MaxCompressedLength(src_size);
}

extern "C" size_t compressor_snappy(void* dst_data, const size_t dst_size, const void* src_data, const size_t src_size, int mode)
{
    static_cast<void>(dst_size); // unused param

    if (mode == CompressMode_DEFLATE)
    {
        size_t output_size = 0;
        snappy::RawCompress(static_cast<const char*>(src_data), src_size, static_cast<char*>(dst_data), &output_size);

        if (!output_size)
        {
            return COMPRESS_ERROR;
        }

        return output_size;
    }
    else
    {
        size_t output_size = 0;
        if (!snappy::GetUncompressedLength(static_cast<const char*>(src_data), src_size, &output_size))
        {
            return COMPRESS_ERROR;
        }

        if (!snappy::RawUncompress(static_cast<const char*>(src_data), src_size, static_cast<char*>(dst_data)))
        {
            return COMPRESS_ERROR;
        }

        return output_size;
    }
}
