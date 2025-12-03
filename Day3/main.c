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
        int found = 0;
        chartype nums1[COUNT1+2] = {0xFF};
        chartype nums2[COUNT2+2] = {0xFF};
        while (isdigit(file.data[idx]))
        {
            const char c = file.data[idx++];
        p1:
            for (int i = 1; i <= COUNT1; ++i)
            {
                if (nums1[i] > nums1[i-1])
                {
                    memmove(nums1 + i - 1, nums1 + i, COUNT1 + 1 - i);
                    nums1[COUNT1] = c;
                    goto p2;
                }
            }
            if (c > nums1[COUNT1])
                nums1[COUNT1] = c;
            
        p2:
            for (int i = 1; i <= COUNT2; ++i)
            {
                if (nums2[i] > nums2[i-1])
                {
                    memmove(nums2 + i - 1, nums2 + i, COUNT2 + 1 - i);
                    nums2[COUNT2] = c;
                    goto nextnum;
                }
            }
            if (c > nums2[COUNT2])
                nums2[COUNT2] = c;
        nextnum:
            (void)0;
        }
        ++idx; // '\n'
        
        int tidx1 = 1, tidx2 = 1;
        uint64_t num1 = 0, num2 = 0;
        PARSEINT(num1, nums1, tidx1);
        PARSEINT(num2, nums2, tidx2);
        DEBUGLOG("line: P1 %" PRIu64 ", P2 %" PRIu64 "\n", num1, num2);
        answer1 += num1;
        answer2 += num2;
    }

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
