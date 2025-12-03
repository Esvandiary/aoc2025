// #define ENABLE_DEBUGLOG
#include "../common/mmap.h"
#include "../common/print.h"
#include "../common/view.h"
#include "../common/intparse.h"

#define COUNT1 2
#define COUNT2 12

int main(int argc, char** argv)
{
    mmap_file file = mmap_file_open_ro("input.txt");
    const int fileSize = (int)(file.size);

    //
    // content
    //

    uint64_t answer1 = 0, answer2 = 0;

    int idx = 0;
    while (idx < fileSize - 2)
    {
        const int start = idx;

        chartype p1n0 = 0, p1n1 = 0;
        while (isdigit(file.data[idx]))
        {
            const chartype c = file.data[idx];
            if (p1n1 > p1n0)
            {
                p1n0 = p1n1;
                p1n1 = c;
            }
            else if (c > p1n1)
            {
                p1n1 = c;
            }

            ++idx;
        }

        uint64_t num1 = (p1n0 & 0xF) * 10 + (p1n1 & 0xF);
        uint64_t num2 = 0;

        int p2start = start;
        int endidx = idx - COUNT2;
        for (int nidx = 0; nidx < COUNT2; ++nidx, ++endidx)
        {
            chartype best = 0;
            int bestpos;
            for (int i = p2start; i <= endidx; ++i)
            {
                const chartype c = file.data[i];
                if (c > best)
                {
                    best = c;
                    bestpos = i;
                }
            }

            num2 = num2 * 10 + (best & 0xF);
            p2start = bestpos + 1;
            DEBUGLOG("nidx %d --> %c @ %d\n", nidx, best, bestpos - start);
        }

        DEBUGLOG("line: P1 %" PRIu64 ", P2 %" PRIu64 "\n", num1, num2);
        answer1 += num1;
        answer2 += num2;

        ++idx; // '\n'
    }

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
