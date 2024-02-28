/**
 * Copyright 2024 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include "rewind.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct RewindBuffer
{
    void* data;
    size_t size;
};

struct Rewind
{
    rewind_compressor_func_t compressor;
    rewind_compressor_size_func_t compressor_size;

    struct RewindBuffer* frames; /* array of compressed frames. */
    struct RewindBuffer current_frame; /* current uncompressed frame. */
    struct RewindBuffer deflate_buffer; /* buffer that we compress into. */

    size_t index; /* which frame we are currently in. */
    size_t count; /* how many frames we have allocated. */
    size_t max; /* max frames. */
};

/* matching data gets xored to 0, which compresses really well. */
static void xor(void* _frame, const void* _keydata, const size_t size)
{
    char* frame = (char*)_frame;
    const char* keydata = (const char*)_keydata;
    size_t i;

    for (i = 0; i < size; i++)
    {
        frame[i] ^= keydata[i];
    }
}

static void rewindbuffer_free(struct RewindBuffer* rwf)
{
    if (rwf->data)
    {
        free(rwf->data);
    }
    memset(rwf, 0, sizeof(*rwf));
}

Rewind* rewind_init(
    const rewind_compressor_func_t compressor,
    const rewind_compressor_size_func_t compressor_size,
    size_t state_size,
    size_t frames_wanted
) {
    if (!compressor || !compressor_size || !state_size || !frames_wanted)
    {
        return NULL;
    }

    Rewind* rw = calloc(1, sizeof(*rw));
    if (!rw)
    {
        return NULL;
    }

    rw->compressor = compressor;
    rw->compressor_size = compressor_size;
    rw->max = frames_wanted;
    rw->frames = calloc(rw->max, sizeof(*rw->frames));

    rw->current_frame.size = state_size;
    rw->current_frame.data = malloc(rw->current_frame.size);
    rw->deflate_buffer.size = rw->compressor_size(state_size);
    rw->deflate_buffer.data = malloc(rw->deflate_buffer.size);

    if (!rw->frames || !rw->current_frame.data || !rw->deflate_buffer.data || !rw->deflate_buffer.size)
    {
        rewind_close(rw);
        return NULL;
    }

    return rw;
}

void rewind_close(Rewind* rw)
{
    if (!rw)
    {
        return;
    }

    if (rw->frames)
    {
        size_t i;
        for (i = 0; i < rw->count; i++)
        {
            rewindbuffer_free(&rw->frames[i]);
        }

        free(rw->frames);
    }

    rewindbuffer_free(&rw->current_frame);
    rewindbuffer_free(&rw->deflate_buffer);

    free(rw);
}

static void rewind_store_current_frame(Rewind* rw, const void* data, size_t size)
{
    assert(rw->current_frame.size == size);
    memcpy(rw->current_frame.data, data, size);
    rw->count = rw->count < rw->max ? rw->count + 1 : rw->max;
}

bool rewind_push(Rewind* rw, const void* data, size_t size)
{
    if (!rw || !data || rw->current_frame.size != size)
    {
        return false;
    }

    /* if we have no frames, store the current frame */
    if (rw->count == 0)
    {
        rewind_store_current_frame(rw, data, size);
    }
    else
    {
        /* we have a current frame. */
        xor(rw->current_frame.data, data, size);

        struct RewindBuffer new_frame;
        new_frame.size = rw->compressor(rw->deflate_buffer.data, rw->deflate_buffer.size, rw->current_frame.data, size, CompressMode_DEFLATE);

        if (new_frame.size == COMPRESS_ERROR)
        {
            assert(!"failed to compress");
            return false;
        }

        /* copy compressed data to buffer to be stored in the frame arary. */
        new_frame.data = malloc(new_frame.size);
        if (!new_frame.data)
        {
            assert(!"failed to malloc new frame data");
            return false;
        }

        memcpy(new_frame.data, rw->deflate_buffer.data, new_frame.size);

        /* remove old frame if data exists. */
        rewindbuffer_free(&rw->frames[rw->index]);

        rw->frames[rw->index] = new_frame;
        rw->index = (rw->index + 1) % rw->max;

        /* now store the new data as the current frame. */
        rewind_store_current_frame(rw, data, size);
    }

    return true;
}

bool rewind_pop(Rewind* rw, void* data, size_t size)
{
    if (!rw || !data || rw->current_frame.size != size)
    {
        return false;
    }

    if (rw->count == 0)
    {
        assert(!"rewind pop called with no frames stored!");
        return false;
    }

    memcpy(data, rw->current_frame.data, size);

    if (rw->count > 1)
    {
        rw->index = (rw->index - 1) % rw->max;
        const size_t result = rw->compressor(rw->current_frame.data, rw->current_frame.size, rw->frames[rw->index].data, rw->frames[rw->index].size, CompressMode_INFLATE);
        if (!result || result == COMPRESS_ERROR || result != rw->current_frame.size)
        {
            assert(!"failed to uncompress");
            return false;
        }

        /* store new buffer. */
        xor(rw->current_frame.data, data, rw->current_frame.size);
    }

    rw->count--;

    return true;
}

size_t rewind_get_frame_count(const Rewind* rw)
{
    return rw->count;
}
