/**
 * Copyright 2024 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#ifndef _LIBREWIND_H_
#define _LIBREWIND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

enum CompressMode
{
    CompressMode_DEFLATE,
    CompressMode_INFLATE,
};

#define COMPRESS_ERROR ((size_t)-1)

typedef size_t (*rewind_compressor_size_func_t)(size_t src_size);
typedef size_t (*rewind_compressor_func_t)(void* dst_data, size_t dst_size, const void* src_data, size_t src_size, int mode);

typedef struct Rewind Rewind;

/*
* inits the rewind struct, allocating the number of frames needed.
* the function pointers must NOT be null.
* call rewind_close() before exiting to free allocted memory!
*/
Rewind* rewind_init(
    const rewind_compressor_func_t compressor,
    const rewind_compressor_size_func_t compressor_size,
    size_t state_size, // size of the state, cannot change between calls
    size_t frames_wanted
);

/*
* frees all allocated data.
* to re-use, call rewind_init() again.
*/
void rewind_close(Rewind* rw);

/*
* clears all the frames and resets count to 0.
* call this when loading or restting a game.
*/
void rewind_reset(Rewind* rw);

/*
* push a new savestate.
* this should be called once each frame.
* the size between each call CANNOT be change!
*/
bool rewind_push(Rewind* rw, const void* data, size_t size);

/*
* pops a savestate.
* this should be called once per frame in which you are rewinding.
* the [data] needs to be allocted, and will be filled with the state.
*/
bool rewind_pop(Rewind* rw, void* data, size_t size);

/*
* return the number of frames avaliable.
*/
size_t rewind_get_frame_count(const Rewind* rw);

#ifdef __cplusplus
}
#endif

#endif /* _LIBREWIND_H_ */
