#include <test_framework.h>
#include <compressor_lz4.h>

int main(int argc, char** argv)
{
    return test_framework_run(compressor_lz4, compressor_lz4_size);
}
