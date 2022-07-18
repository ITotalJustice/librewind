// Copyright 2022 TotalJustice.
// SPDX-License-Identifier: Zlib

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

size_t compressor_zlib_size(size_t src_size);
size_t compressor_zlib(void* dst_data, size_t dst_size, const void* src_data, size_t src_size, int mode);

#ifdef __cplusplus
}
#endif
