/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <test_framework.h>
#include <compressor_raw.h>

int main(int argc, char** argv)
{
    return test_framework_run(compressor_raw, compressor_raw_size);
}
