/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include <test_framework.h>
#include <compressor_lzo.h>

int main(int argc, char** argv)
{
    return test_framework_run(compressor_lzo, compressor_lzo_size);
}
