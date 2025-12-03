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
        chartype nums2[COUNT2+1];
        memcpy(nums2, file.data + idx, sizeof(chartype) * COUNT2);
        nums2[COUNT2] = 0;
        idx += COUNT2;

        int sortedidx = 1;

        while (isdigit(file.data[idx]))
        {
            const chartype c = file.data[idx++];

            for (int i = sortedidx; i < COUNT2; ++i)
            {
                if (nums2[i] > nums2[i-1])
                {
                    memmove(nums2 + i - 1, nums2 + i, sizeof(chartype) * (COUNT2 - i));
                    sortedidx = MAX(1, i-1);
                    goto setnum;
                }
            }

            if (c > nums2[COUNT2-1])
            {
            setnum:
                nums2[COUNT2-1] = c;
            }
        }
        ++idx; // '\n'

        chartype nums1[COUNT1+1] = { nums2[0], nums2[1] };
        for (int i2 = 2; i2 < COUNT2; ++i2)
        {
            const chartype c = nums2[i2];
            if (nums1[1] > nums1[0])
            {
                nums1[0] = nums1[1];
                nums1[1] = c;
            }
            else if (c > nums1[1])
            {
                nums1[1] = c;
            }
        }
        
        int tidx1 = 0, tidx2 = 0;
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
