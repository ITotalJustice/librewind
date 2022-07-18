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
static void xor(void* _frame, const void* _keydata, const size_t size)
{
    char* frame = (char*)_frame;
    const char* keydata = (const char*)_keydata;

    for (size_t i = 0; i < size; i++)
    {
        frame[i] ^= keydata[i];
    }
}

// allocates new data for keyframe
static void rewindframe_init(struct RewindFrame* rwf, const void* keydata, const size_t keysize)
{
    memset(rwf, 0, sizeof(struct RewindFrame));

    rwf->keyframe.data = malloc(keysize);
    rwf->keyframe.size = keysize;
    memcpy(rwf->keyframe.data, keydata, keysize);
}

static void rewindframe_close(struct RewindFrame* rwf)
{
    if (rwf->keyframe.data)
    {
        free(rwf->keyframe.data);
    }

    for (size_t i = 0; i < rwf->count; i++)
    {
        assert(rwf->data[i].data);
        free(rwf->data[i].data);
    }

    memset(rwf, 0, sizeof(struct RewindFrame));
}

static bool rewindframe_is_empty(const struct RewindFrame* rwf)
{
    return rwf->count == 0;
}

static bool rewindframe_is_full(const struct RewindFrame* rwf)
{
    return rwf->count == REWIND_FRAME_ENTRY_COUNT;
}

static bool rewindframe_push(struct RewindFrame* rwf, const void* framedata, const size_t framesize, const rewind_compressor_func_t compressor, const rewind_compressor_size_func_t compressor_size)
{
    // we can't push any more frames!
    assert(rewindframe_is_full(rwf) == false && "[RWF] tried to push frame when full");

    void* compressed_data = malloc(framesize);
    size_t compressed_size = framesize;
    memcpy(compressed_data, framedata, framesize);

    xor(compressed_data, rwf->keyframe.data, framesize);

    compressed_size = compressor_size(framesize);
    void* buf = malloc(compressed_size);

    compressed_size = compressor(buf, compressed_size, compressed_data, framesize, CompressMode_DEFLATE);

    if (compressed_size == COMPRESS_ERROR)
    {
        assert(compressed_size && "failed to compress");
        free(compressed_data);
        free(buf);
        return false;
    }

    free(compressed_data);
    compressed_data = realloc(buf, compressed_size);

    rwf->data[rwf->count].data = compressed_data;
    rwf->data[rwf->count].size = framesize;
    rwf->data[rwf->count].compressed_size = compressed_size;
    assert(rwf->data[rwf->count].size && rwf->data[rwf->count].compressed_size);
    rwf->count++;

    return true;
}

static bool rewindframe_pop(struct RewindFrame* rwf, void* outdata, const size_t outsize, const rewind_compressor_func_t compressor)
{
    // can't pop any more frames!
    assert(rewindframe_is_empty(rwf) == false && "[RWF] tried to pop frame when empty");

    struct RewindFrameEntry* entry = &rwf->data[rwf->count - 1];

    if (entry->size != outsize)
    {
        assert(entry->size == outsize);
        return false;
    }

    const size_t result = compressor(outdata, outsize, entry->data, entry->compressed_size, CompressMode_INFLATE);
    assert(result && result != COMPRESS_ERROR && "failed to decompress");
    if (!result || result == COMPRESS_ERROR)
    {
        return false;
    }

    xor(outdata, rwf->keyframe.data, outsize);

    free(entry->data);
    memset(entry, 0, sizeof(struct RewindFrameEntry));

    rwf->count--;
    return true;
}

// returns how many slots to allocate per keyframe
static size_t rewind_calculate_keyframe_slots(const size_t seconds_wanted)
{
    // if the user wants less seconds than the number of entries
    // per keyframe, then default to 1 slot
    // todo: maybe this should error instead?
    if (seconds_wanted < REWIND_FRAME_ENTRY_COUNT)
    {
        return 1;
    }

    return seconds_wanted / REWIND_FRAME_ENTRY_COUNT;
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
    rw->max = rewind_calculate_keyframe_slots(seconds_wanted);
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

    memset(rw, 0, sizeof(*rw));
}

bool rewind_push(struct Rewind* rw, const void* data, const size_t size)
{
    assert(rw);
    assert(data);
    assert(size);

    bool new_keyframe = false;

    // we are at the start
    if (rewindframe_is_empty(&rw->frames[rw->index]))
    {
        new_keyframe = true;
    }
    // we are at the end
    else if (rewindframe_is_full(&rw->frames[rw->index]))
    {
        rw->index = (rw->index + 1) % (rw->max);
        new_keyframe = true;
    }

    // push new keyframe
    if (new_keyframe)
    {
        rewindframe_close(&rw->frames[rw->index]);
        rewindframe_init(&rw->frames[rw->index], data, size);
        rw->count = rw->count < rw->max ? rw->count + 1 : rw->max;
    }

    // push new entry
    return rewindframe_push(&rw->frames[rw->index], data, size, rw->compressor, rw->compressor_size);
}

bool rewind_pop(struct Rewind* rw, void* data, const size_t size)
{
    assert(rw);
    assert(data);
    assert(size);

check_again:
    if (rw->count == 0)
    {
        return false;
    }

    if (rw->frames[rw->index].count == 0)
    {
        rewindframe_close(&rw->frames[rw->index]);
        rw->index = rw->index ? rw->index - 1 : rw->max - 1;
        rw->count--;
        goto check_again;
    }

    const bool result = rewindframe_pop(&rw->frames[rw->index], data, size, rw->compressor);

    if (rw->frames[rw->index].count == 0)
    {
        rewindframe_close(&rw->frames[rw->index]);
        rw->index = rw->index ? rw->index - 1 : rw->max - 1;
        rw->count--;
    }

    return result;
}

size_t rewind_get_frame_count(const struct Rewind* rw)
{
    if (rw->count == 0)
    {
        return 0;
    }

    const size_t count = (rw->count-1) * REWIND_FRAME_ENTRY_COUNT;

    return count + rw->frames[rw->index].count;
}
