// #define ENABLE_DEBUGLOG
#include "../common/mmap.h"
#include "../common/print.h"
#include "../common/view.h"
#include "../common/intparse.h"

int main(int argc, char** argv)
{
    mmap_file file = mmap_file_open_ro("input.txt");
    const int fileSize = (int)(file.size);

    //
    // content
    //

    uint64_t answer1 = 0, answer2 = 0;

    int64_t curValue = 4611686018427387900LL + 50;

    int idx = 0;
    while (idx < fileSize - 2)
    {
        // L --> -1, R --> 1
        int mul = ((file.data[idx++] & 0b100) >> 1) - 1;

        int num1 = 0;
        PARSEINT(num1, file.data, idx);
        ++idx; // '\n'

        const int64_t diff = mul * num1;
        const int64_t provValue = curValue + diff;

        const int64_t oldValue = curValue % 100;
        int a2count = (mul > 0)
            ? (diff + oldValue) / 100
            : (-diff + (oldValue ? 100 - oldValue : 0)) / 100;

        curValue = provValue;

        // DEBUGLOG("mul = %d, num1 = %d, oldValue = %" PRId64 ", curValue = %" PRId64 ", a2count = %d\n", mul, num1, oldValue, curValue % 100, a2count);

        if ((curValue % 100) == 0)
            ++answer1;
        answer2 += a2count;
    }

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
