/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <rewind.h>

int test_framework_run(const rewind_compressor_func_t comp_func, const rewind_compressor_size_func_t comp_size);

#ifdef __cplusplus
}
#endif
