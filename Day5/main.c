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

    int64_t ranges[1024][2];
    int rangecount = 0;

    int idx = 0;
    while (file.data[idx] != '\n')
    {
        uint64_t num0 = 0, num1 = 0;
        PARSEINT(num0, file.data, idx);
        ++idx; // '-'
        PARSEINT(num1, file.data, idx);
        ++idx; // '\n'

        DEBUGLOG("found range: %" PRIu64 " - %" PRIu64 "\n", num0, num1);
        for (int i = 0; i < rangecount; ++i)
        {
            if (num0 <= ranges[i][0] && num1 >= ranges[i][1])
            {
                DEBUGLOG("replacing range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1], num0, num1);
                ranges[i][0] = num0;
                ranges[i][1] = num1;
                goto nextnum;
            }
            else if (num0 >= ranges[i][0] && num0 <= ranges[i][1])
            {
                DEBUGLOG("consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1], ranges[i][0], MAX(ranges[i][1], num1));
                ranges[i][1] = MAX(ranges[i][1], num1);
                goto nextnum;
            }
            else if (num1 >= ranges[i][0] && num1 <= ranges[i][1])
            {
                DEBUGLOG("consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1], MIN(ranges[i][0], num0), ranges[i][1]);
                ranges[i][0] = MIN(ranges[i][0], num0);
                goto nextnum;
            }
            else if (ranges[i][0] >= num0 && ranges[i][0] <= num1)
            {
                DEBUGLOG("consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1], num0, MAX(ranges[i][1], num1));
                ranges[i][0] = num0;
                ranges[i][1] = MAX(ranges[i][1], num1);
                goto nextnum;
            }
            else if (ranges[i][1] >= num0 && ranges[i][1] <= num1)
            {
                DEBUGLOG("consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1], MIN(ranges[i][0], num0), num1);
                ranges[i][0] = MIN(ranges[i][0], num0);
                ranges[i][1] = num1;
                goto nextnum;
            }
        }
        DEBUGLOG("adding new range\n");
        ranges[rangecount][0] = num0;
        ranges[rangecount][1] = num1;
        ++rangecount;
    nextnum:
        (void)0;
    }
    ++idx; // '\n'
    while (idx < fileSize - 2)
    {
        uint64_t num = 0;
        PARSEINT(num, file.data, idx);
        ++idx; // '\n'

        for (int i = 0; i < rangecount; ++i)
        {
            if (num >= ranges[i][0] && num <= ranges[i][1])
            {
                ++answer1;
                break;
            }
        }
    }

    for (int i = 0; i < rangecount - 1; ++i)
    {
        if (ranges[i][0] < 0)
            continue;

        for (int j = i+1; j < rangecount; ++j)
        {
            if (ranges[j][0] < 0)
                continue;

            DEBUGLOG("considering ranges %" PRId64 " - %" PRId64 " and %" PRId64 " - %" PRId64 "\n",
                ranges[i][0], ranges[i][1], ranges[j][0], ranges[j][1]);

            if (ranges[j][0] <= ranges[i][0] && ranges[j][1] >= ranges[i][1])
            {
                DEBUGLOG("removing superceded range %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1]);
                ranges[i][0] = ranges[j][0];
                ranges[i][1] = ranges[j][1];
                ranges[j][0] = -1;
                ranges[j][1] = -2;
                j = i; // go again
            }
            else if (ranges[j][0] >= ranges[i][0] && ranges[j][0] <= ranges[i][1])
            {
                DEBUGLOG("late consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1], ranges[i][0], MAX(ranges[i][1], ranges[j][1]));
                ranges[i][1] = MAX(ranges[i][1], ranges[j][1]);
                ranges[j][0] = -1;
                ranges[j][1] = -2;
                j = i; // go again
            }
            else if (ranges[j][1] >= ranges[i][0] && ranges[j][1] <= ranges[i][1])
            {
                DEBUGLOG("late consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[i][0], ranges[i][1], MIN(ranges[i][0], ranges[j][0]), ranges[i][1]);
                ranges[i][0] = MIN(ranges[i][0], ranges[j][0]);
                ranges[j][0] = -1;
                ranges[j][1] = -2;
                j = i; // go again
            }
            else if (ranges[i][0] >= ranges[j][0] && ranges[i][0] <= ranges[j][1])
            {
                DEBUGLOG("late consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[j][0], ranges[j][1], ranges[j][0], MAX(ranges[j][1], ranges[i][1]));
                ranges[i][0] = ranges[j][0];
                ranges[i][1] = MAX(ranges[j][1], ranges[i][1]);
                ranges[j][0] = -1;
                ranges[j][1] = -2;
                j = i; // go again
            }
            else if (ranges[i][1] >= ranges[j][0] && ranges[i][1] <= ranges[j][1])
            {
                DEBUGLOG("late consolidating range %" PRId64 " - %" PRId64 " to %" PRId64 " - %" PRId64 "\n",
                    ranges[j][0], ranges[j][1], MIN(ranges[j][0], ranges[i][0]), ranges[j][1]);
                ranges[i][0] = MIN(ranges[j][0], ranges[i][0]);
                ranges[i][1] = ranges[j][1];
                ranges[j][0] = -1;
                ranges[j][1] = -2;
                j = i; // go again
            }
        }

    nexti:
        (void)0;
    }

    DEBUGDO(for (int i = 0; i < rangecount; ++i) if (ranges[i][0] >= 0) DEBUGLOG("range: %" PRId64 " - %" PRId64 "\n", ranges[i][0], ranges[i][1]));
    DEBUGDO(for (int i = 0; i < rangecount - 1; ++i)
    {
        if (ranges[i][0] < 0)
            continue;

        for (int j = i+1; j < rangecount; ++j)
        {
            if (ranges[j][0] < 0)
                continue;
            
            if (ranges[i][0] >= ranges[j][0] && ranges[i][0] <= ranges[j][1])
                DEBUGLOG("OH NO: %d vs %d, %" PRId64 " - %" PRId64 " vs %" PRId64 " - %" PRId64 "\n",
                    i, j, ranges[i][0], ranges[i][1], ranges[j][0], ranges[j][1]);
            if (ranges[i][1] >= ranges[j][0] && ranges[i][1] <= ranges[j][1])
                DEBUGLOG("OH NO: %d vs %d, %" PRId64 " - %" PRId64 " vs %" PRId64 " - %" PRId64 "\n",
                    i, j, ranges[i][0], ranges[i][1], ranges[j][0], ranges[j][1]);
        }
    });

    for (int i = 0; i < rangecount; ++i)
        answer2 += 1 + ranges[i][1] - ranges[i][0];

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
