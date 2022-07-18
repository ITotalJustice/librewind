// Copyright 2022 TotalJustice.
// SPDX-License-Identifier: Zlib

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifndef REWIND_FRAME_ENTRY_COUNT
    #define REWIND_FRAME_ENTRY_COUNT 120
#endif

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

typedef size_t (*rewind_compressor_size_func_t)(size_t src_size);
typedef size_t (*rewind_compressor_func_t)(void* dst_data, size_t dst_size, const void* src_data, size_t src_size, int mode);

struct RewindFrameEntry
{
    uint8_t* data;
    size_t size;
    size_t compressed_size;
    // padding 8
};

struct RewindFrame
{
    struct RewindFrameEntry keyframe;
    struct RewindFrameEntry data[REWIND_FRAME_ENTRY_COUNT]; // todo: make adjustable
    size_t count;
    bool keyframe_compressed;
};

struct Rewind
{
    rewind_compressor_func_t compressor;
    rewind_compressor_size_func_t compressor_size;
    struct RewindFrame* frames;
    size_t index; // which frame we are currently in
    size_t count; // how many frames we have allocated
    size_t max; // max frames
};

bool rewind_init(struct Rewind* rw,
    const rewind_compressor_func_t compressor,
    const rewind_compressor_size_func_t compressor_size,
    size_t seconds_wanted
);

void rewind_close(struct Rewind* rw);

bool rewind_pop(struct Rewind* rw, uint8_t* data, size_t size);
bool rewind_push(struct Rewind* rw, const uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif
