// #define ENABLE_DEBUGLOG
#include "../common/mmap.h"
#include "../common/print.h"
#include "../common/view.h"
#include "../common/intparse.h"

typedef enum op
{
    OP_MUL = 1,
    OP_ADD = 2,
    OP_SKIP = 3,
} op;

static uint64_t p1totals[2048] = {0};
static uint32_t p2nums[8192] = {0};
static uint8_t p1ops[2048] = {0};
static uint8_t p2opsstore[8192] = {0};
static uint8_t* p2ops = p2opsstore + 16;

int main(int argc, char** argv)
{
    mmap_file file = mmap_file_open_ro("input.txt");
    const int fileSize = (int)(file.size);

    //
    // content
    //

    int idx = fileSize-1;
    int end = idx;
    if (file.data[idx] != '\n')
        ++end;
    while (file.data[--idx] != '\n');
    ++idx; // '\n'

    const int width = end - idx;
    const int opstart = idx;

    int totalcount = 0;
    int opcount = 0;

    while (idx < end)
    {
        switch (file.data[idx])
        {
            case '*': p1ops[opcount++] = OP_MUL; p2ops[idx-opstart] = OP_MUL; p2ops[idx-opstart-1] = OP_SKIP; break;
            case '+': p1ops[opcount++] = OP_ADD; p2ops[idx-opstart] = OP_ADD; p2ops[idx-opstart-1] = OP_SKIP; break;
            default: break;
        }
        ++idx;
    }

    for (int i = 0; i < width; ++i)
    {
        switch (file.data[i])
        {
            case ' ': break;
            default:
            {
                uint64_t num = 0;
                while (isdigit(file.data[i]))
                {
                    const uint8_t digit = file.data[i] & 0xF;
                    num = (num * 10) + digit;
                    p2nums[i] = (p2nums[i] * 10) + digit;
                    ++i;
                }
                p1totals[totalcount++] = num;
                break;
            }
        }
    }

    DEBUGDO(for (int i = 0; i < totalcount; ++i) { DEBUGLOG("[%10" PRIu64 "] ", p1totals[i]); } DEBUGLOG("\n"));

    int start = width + 1;
    const int nend = end - width;
    while (start < nend)
    {
        int ctcount = 0;
        for (int i = start; i < start + width; ++i)
        {
            switch (file.data[i])
            {
                case ' ': continue;
                default:
                {
                    uint64_t num = 0;
                    while (isdigit(file.data[i]))
                    {
                        const uint8_t digit = file.data[i] & 0xF;
                        num = (num * 10) + digit;
                        p2nums[i-start] = (p2nums[i-start] * 10) + digit;
                        ++i;
                    }
                    switch (p1ops[ctcount])
                    {
                        case OP_MUL: p1totals[ctcount++] *= num; break;
                        case OP_ADD: p1totals[ctcount++] += num; break;
                    }
                    break;
                }
            }
        }
        start += width + 1;
        DEBUGDO(for (int i = 0; i < totalcount; ++i) { DEBUGLOG("[%10" PRIu64 "] ", p1totals[i]); } DEBUGLOG("\n"));
    }

    DEBUGLOG("totals = %d, ops = %d, width = %d\n", totalcount, opcount, width);

    uint64_t answer1 = 0, answer2 = 0;

    for (int i = 0; i < totalcount; ++i)
        answer1 += p1totals[i];
    
    int opidx = 0;
    for (int i = 0; i < width; ++i)
    {
        const uint8_t op = p2ops[i];

        uint64_t p2ct = p2nums[i++];
        DEBUGLOG("[%10" PRIu64 "] ", p2ct);
        while (i < width && p2ops[i] != OP_SKIP)
        {
            DEBUGLOG("[%10u] ", p2nums[i]);
            switch (op)
            {
                case OP_MUL: p2ct *= p2nums[i]; break;
                case OP_ADD: p2ct += p2nums[i]; break;
            }
            ++i;
        }
        answer2 += p2ct;
        DEBUGLOG("{%c} = %" PRIu64 "\n", (op == OP_MUL ? '*' : '+'), p2ct);
    }

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
