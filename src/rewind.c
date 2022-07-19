/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include "rewind.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// matching data gets xored to 0, which compresses really well
// todo (velocity): do 64bit xor. byte xor's until on 64 aligned data
// boundry. i assume i should do 32bit xor on 32bit systems.
static void xor(void* _frame, const void* _keydata, const size_t size)
{
    char* frame = (char*)_frame;
    const char* keydata = (const char*)_keydata;

    for (size_t i = 0; i < size; i++)
    {
        frame[i] ^= keydata[i];
    }
}

static void rewindframe_close(struct RewindFrame* rwf)
{
    if (rwf->data)
    {
        free(rwf->data);
    }
    memset(rwf, 0, sizeof(*rwf));
}

bool rewind_init(struct Rewind* rw,
    const rewind_compressor_func_t compressor,
    const rewind_compressor_size_func_t compressor_size,
    size_t seconds_wanted
) {
    assert(rw);
    assert(compressor);
    assert(compressor_size);
    assert(seconds_wanted);

    if (!rw || !compressor || !compressor_size || !seconds_wanted)
    {
        return false;
    }

    memset(rw, 0, sizeof(struct Rewind));

    rw->compressor = compressor;
    rw->compressor_size = compressor_size;
    rw->max = seconds_wanted;
    rw->frames = calloc(rw->max, sizeof(struct RewindFrame));

    return true;
}

void rewind_close(struct Rewind* rw)
{
    if (rw->frames)
    {
        for (size_t i = 0; i < rw->count; i++)
        {
            rewindframe_close(&rw->frames[i]);
        }

        free(rw->frames);
    }

    if (rw->count)
    {
        rewindframe_close(&rw->current_frame);
    }

    memset(rw, 0, sizeof(*rw));
}

static void rewind_store_current_frame(struct Rewind* rw, const void* data, size_t size)
{
    rw->current_frame.data = malloc(size);
    rw->current_frame.size = size;
    rw->current_frame.compressed_size = 0;
    memcpy(rw->current_frame.data, data, size);

    rw->count = rw->count < rw->max ? rw->count + 1 : rw->max;
}

bool rewind_push(struct Rewind* rw, const void* data, size_t size)
{
    // if we have no frames, store the current frame
    if (rw->count == 0)
    {
        assert(rw->current_frame.data == NULL);
        rewind_store_current_frame(rw, data, size);
    }
    else
    {
        // we have a current frame,
        // xor and compressit against the new frame and stash it
        assert(rw->current_frame.data);

        // rw->frames[rw->index] = rw->current_frame;
        xor(rw->current_frame.data, data, size);

        rw->current_frame.compressed_size = rw->compressor_size(rw->current_frame.size);
        if (!rw->current_frame.compressed_size)
        {
            assert(rw->current_frame.compressed_size);
            return false;
        }

        void* buf = malloc(rw->current_frame.compressed_size);
        rw->current_frame.compressed_size = rw->compressor(buf, rw->current_frame.compressed_size, rw->current_frame.data, rw->current_frame.size, CompressMode_DEFLATE);

        if (rw->current_frame.compressed_size == COMPRESS_ERROR)
        {
            assert(!"failed to compress");
            free(buf);
            return false;
        }

        // "shrink" the data compressed data with the actual size.
        free(rw->current_frame.data);
        rw->current_frame.data = realloc(buf, rw->current_frame.compressed_size);
        assert(rw->current_frame.data);

        // finally, stash the new data!
        if (rw->frames[rw->index].data)
        {
            rewindframe_close(&rw->frames[rw->index]);
        }

        rw->frames[rw->index] = rw->current_frame;
        rw->index = (rw->index + 1) % rw->max;

        // now store the new data as the current frame
        rewind_store_current_frame(rw, data, size);
    }

    return true;
}

bool rewind_pop(struct Rewind* rw, void* data, size_t size)
{
    if (rw->count == 0)
    {
        return false;
    }

    assert(rw->current_frame.data);
    assert(rw->current_frame.size == size);

    memcpy(data, rw->current_frame.data, rw->current_frame.size);
    rewindframe_close(&rw->current_frame);

    if (rw->count > 1)
    {
        rw->index = rw->index == 0 ? rw->max - 1 : rw->index - 1;

        rw->current_frame.data = malloc(rw->frames[rw->index].size);
        rw->current_frame.size = rw->frames[rw->index].size;

        const size_t result = rw->compressor(rw->current_frame.data, rw->current_frame.size, rw->frames[rw->index].data, rw->frames[rw->index].compressed_size, CompressMode_INFLATE);
        if (!result || result == COMPRESS_ERROR)
        {
            assert(!"failed to uncompress");
            return false;
        }

        xor(rw->current_frame.data, data, rw->current_frame.size);
    }

    rw->count--;

    return true;
}

size_t rewind_get_frame_count(const struct Rewind* rw)
{
    return rw->count;
}
