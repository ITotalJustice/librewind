/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <lzo/lzo1x.h>
#include <stddef.h>

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

size_t compressor_lzo_size(const size_t src_size)
{
    // from examples/simple.c in lzo-2.10 src
    return src_size + src_size / 16 + 64 + 3;
}

size_t compressor_lzo(void* dst_data, const size_t dst_size, const void* src_data, const size_t src_size, int mode)
{
    if (mode == CompressMode_DEFLATE)
    {
        static char mem[LZO1X_MEM_COMPRESS];
        lzo_uint output_size = dst_size;

        const int result = lzo1x_1_compress(src_data, src_size, dst_data, &output_size, mem);

        if (result != LZO_E_OK)
        {
            return COMPRESS_ERROR;
        }

        return output_size;
    }
    else
    {
        #if LZO1X_MEM_DECOMPRESS > 0
            static char mem[LZO1X_MEM_DECOMPRESS];
        #else
            char* mem = NULL;
        #endif
        lzo_uint output_size = dst_size;

        const int result = lzo1x_decompress(src_data, src_size, dst_data, &output_size, mem);

        if (result != LZO_E_OK)
        {
            return COMPRESS_ERROR;
        }

        return output_size;
    }
}
