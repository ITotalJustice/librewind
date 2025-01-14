/**
 * Copyright 2022 TotalJustice.
 * SPDX-License-Identifier: Zlib
 */

#include "test_framework.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <rewind.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define TEST(result) if (!(result)) { assert(0); return false; }
#define TEST2(result, num) if (!(result)) { assert(0); return num; }

static const uint8_t data_to_compress[3][10] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {6, 99, 45, 101, 222, 1, 1, 2, 8, 8},
};

uint8_t output[3][10] = {0};


static void test_init(void)
{
    memset(output, 0xFF, sizeof(output));
}

static bool compare(void)
{
    return memcmp(data_to_compress, output, sizeof(data_to_compress));
}

// push 1, pop 1
static bool test_1(Rewind* rw)
{
    test_init();

    TEST(rewind_push(rw, data_to_compress[0], sizeof(data_to_compress[0])))
    TEST(rewind_get_frame_count(rw) == 1);
    TEST(rewind_pop(rw, output[0], sizeof(output[0])));
    TEST(rewind_get_frame_count(rw) == 0);
    TEST(compare());

    return true;
}

// push 3, pop 3
static bool test_2(Rewind* rw)
{
    test_init();
    for (size_t i = 0; i < ARRAY_SIZE(data_to_compress); i++)
    {
        TEST(rewind_push(rw, data_to_compress[i], sizeof(data_to_compress[i])))
        TEST(rewind_get_frame_count(rw) == (i+1));
    }

    TEST(rewind_get_frame_count(rw) == 3);

    for (size_t i = 0; i < ARRAY_SIZE(data_to_compress); i++)
    {
        TEST(rewind_pop(rw, output[i], sizeof(output[i])))
    }

    TEST(compare());
    TEST(rewind_get_frame_count(rw) == 0);

    return true;
}

// push 120, pop 120
static bool test_3(Rewind* rw)
{
    test_init();

    for (size_t j = 0; j < 120 / ARRAY_SIZE(data_to_compress); j++)
    {
        for (size_t i = 0; i < ARRAY_SIZE(data_to_compress); i++)
        {
            TEST(rewind_push(rw, data_to_compress[i], sizeof(data_to_compress[i])))
        }
    }

    TEST(rewind_get_frame_count(rw) == 120);

    for (size_t j = 0; j < 120 / ARRAY_SIZE(data_to_compress); j++)
    {
        for (size_t i = 0; i < ARRAY_SIZE(data_to_compress); i++)
        {
            TEST(rewind_pop(rw, output[i], sizeof(output[i])))
        }

        TEST(compare());
    }

    TEST(rewind_get_frame_count(rw) == 0)

    return true;
}

// push 123, pop 123
static bool test_4(Rewind* rw)
{
    test_init();

    for (size_t j = 0; j < 123 / ARRAY_SIZE(data_to_compress); j++)
    {
        for (size_t i = 0; i < ARRAY_SIZE(data_to_compress); i++)
        {
            TEST(rewind_push(rw, data_to_compress[i], sizeof(data_to_compress[i])))
        }
    }

    TEST(rewind_get_frame_count(rw) == 120);

    for (size_t j = 0; j < 120 / ARRAY_SIZE(data_to_compress); j++)
    {
        for (size_t i = 0; i < ARRAY_SIZE(data_to_compress); i++)
        {
            TEST(rewind_pop(rw, output[i], sizeof(output[i])))
        }
    }

    TEST(compare());
    TEST(rewind_get_frame_count(rw) == 0)

    return true;
}

int test_framework_run(const rewind_compressor_func_t comp_func, const rewind_compressor_size_func_t comp_size)
{
    Rewind* rw = rewind_init(comp_func, comp_size, sizeof(data_to_compress[0]), 120);
    if (!rw)
    {
        return false;
    }

    TEST2(test_1(rw), 1);
    TEST2(test_2(rw), 2);
    TEST2(test_3(rw), 3);
    TEST2(test_4(rw), 4);

    rewind_close(rw);

    return 0;
}
