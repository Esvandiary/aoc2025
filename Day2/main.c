// #define ENABLE_DEBUGLOG
#include "../common/mmap.h"
#include "../common/basicfile.h"
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

    int idx = 0;
    while (idx < fileSize - 2)
    {
        uint64_t num1 = 0, num2 = 0;

        int num1start = idx;
        PARSEINT(num1, file.data, idx);
        int num1len = (idx - num1start);
        ++idx; // '-'

        int num2start = idx;
        PARSEINT(num2, file.data, idx);
        int num2len = (idx - num2start);
        ++idx; // ','

        char test[33] = {0};
        memcpy(test + 32 - num2len, file.data + num2start, num2len);
        int testlen = num2len;
        for (uint64_t ntest = num2; ntest >= num1; --ntest)
        {
            if ((testlen % 2) == 0 && memcmp(test + 32 - testlen, test + 32 - testlen / 2, testlen / 2) == 0)
            {
                answer1 += ntest;
                DEBUGLOG("invalid: %" PRIu64 " (%d)\n", ntest, testlen);
            }

            // you are now decrementing manually
            const bool dropnext = (--test[31] < '0');
            for (int c = 31; dropnext && c >= 32 - testlen; --c)
            {
                if (c == 32 - testlen && test[c] == '1')
                {
                    --testlen;
                    break;
                }
                else if (--test[c] < '0')
                {
                    test[c] = '9';
                }
                else
                {
                    break;
                }
            }
        }
    }

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
