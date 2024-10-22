# librewind

a simple rewind lib for emulators, written in c89

---

## compressors

a compressor is needed to use librewind. this compressor will be called to defalte / inflate the data on calls to push / pop respectively.

a selection of compressor wrappers have been implemented, (see the [compressors folder](compressors/)
for the full list)

- lz4
- raw (no compression)
- zlib
- zstd

---

## using

add this to your `CMakeLists.txt`

```cmake
# SET(COMPRESSOR_LZ4 ON)
# SET(COMPRESSOR_RAW ON)
# SET(COMPRESSOR_ZLIB ON)
# SET(COMPRESSOR_ZSTD ON)
# SET(COMPRESSOR_SNAPPY ON)
# SET(COMPRESSOR_FASTLZ ON)
# SET(COMPRESSOR_LZO ON)

include(FetchContent)

FetchContent_Declare(rewind
    GIT_REPOSITORY https://github.com/ITotalJustice/librewind.git
    GIT_TAG        v2.0.0
)

FetchContent_MakeAvailable(rewind)

target_link_libraries(your_exe PRIVATE
    rewind
    # compressor_lz4
    # compressor_raw
    # compressor_zlib
    # compressor_zstd
    # compressor_snappy
    # compressor_fastlz
    # compressor_lzo
)
```

an example of how it would look in your emulator.

```c
void run(void)
{
    if (!rewinding)
    {
        emulator_run_frame();
        struct State state = emulator_save_state();
        if (!rewind_push(&rw, &state, sizeof(state)))
        {
            // handle error here
        }
    }
    else
    {
        struct State state;
        if (!rewind_pop(&rw, &state, sizeof(state)))
        {
            // handle error here
        }
        else
        {
            emulator_load_state(&state);

            // check if there's any more frames left
            if (!rewind_get_frame_count(&rw))
            {
                rewinding = false;
            }
        }
    }
}
```

---

## testing

features ctest support. build and run with:

```sh
cmake -B build \
-DCOMPRESSOR_LZ4=ON -DTEST_LZ4=ON \
-DCOMPRESSOR_RAW=ON -DTEST_RAW=ON \
-DCOMPRESSOR_ZLIB=ON -DTEST_ZLIB=ON \
-DCOMPRESSOR_ZSTD=ON -DTEST_ZSTD=ON \
-DCOMPRESSOR_SNAPPY=ON -DTEST_SNAPPY=ON \
-DCOMPRESSOR_FASTLZ=ON -DTEST_FASTLZ=ON \
-DCOMPRESSOR_LZO=ON -DTEST_LZO=ON

cmake --build build

ctest --test-dir build
```

---

## thanks

- dillon for his algorithm (i simply implemented it)
- lz4 https://github.com/lz4/lz4
- zlib https://github.com/madler/zlib
- zstd https://github.com/facebook/zstd
- snappy https://github.com/google/snappy
- fastlz https://github.com/ariya/FastLZ
- lzo http://www.oberhumer.com/opensource/lzo/
