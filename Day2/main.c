// #define ENABLE_DEBUGLOG
#include "../common/mmap.h"
#include "../common/basicfile.h"
#include "../common/print.h"
#include "../common/view.h"
#include "../common/intparse.h"

#define ARRLEN 16

static inline FORCEINLINE int min_possible(const char* arr, int testlen)
{
    for (int i = 1; i < testlen; ++i)
    {
        if (arr[0] == arr[i])
            return i;
    }
    return testlen;
}

int main(int argc, char** argv)
{
    mmap_file file = mmap_file_open_ro("input.txt");
    const int fileSize = (int)(file.size);

    uint64_t answer1 = 0, answer2 = 0;

    uint64_t ilens = 0;

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

        char test[ARRLEN];
        memcpy(test + ARRLEN - num2len, file.data + num2start, num2len);
        int testlen = num2len;
        int imin = min_possible(test + ARRLEN - testlen, testlen);

        div_t ilendiv[ARRLEN];
        for (int i = 1; i <= testlen / 2; ++i)
            ilendiv[i] = div(testlen, i);

        int ilenkills[ARRLEN] = {0};

        for (uint64_t ntest = num2; ntest >= num1; --ntest)
        {
            //DEBUGLOG("===== %" PRIu64 " =====\n", ntest);
            for (int ilen = testlen / 2; ilen >= imin; --ilen)
            {
                if (ilenkills[ilen] == 0 && ilendiv[ilen].rem == 0)
                {
                    DEBUGDO(++ilens);
                    const int iend = ilendiv[ilen].quot;
                    for (int itest = 1; itest < iend; ++itest)
                    {
                        for (int i = 0; i < ilen; ++i)
                        {
                            if (test[ARRLEN - testlen + i] != test[ARRLEN - testlen + ilen*itest + i])
                            {
                                ilenkills[ARRLEN - testlen + ilen] = ARRLEN - testlen + ilen*itest + i;
                                goto nextlen;
                            }
                        }
                    }

                    answer2 += ntest;
                    if (ilen * 2 == testlen)
                    {
                        answer1 += ntest;
                        DEBUGLOG("invalid (1,2): %" PRIu64 " (%d)\n", ntest, ilen);
                    }
                    else
                    {
                        DEBUGLOG("invalid (2): %" PRIu64 " (%d)\n", ntest, ilen);
                    }
                    break;
                nextlen:
                    (void)0;
                }
            }

            // you are now decrementing manually
            int minchange = testlen;
            if (--test[ARRLEN-1] < '0')
            {
                int c;
                for (c = ARRLEN-1; c >= ARRLEN - testlen; --c)
                {
                    if (c == ARRLEN - testlen && test[c] == '1')
                    {
                        --testlen;
                        for (int i = 1; i <= testlen / 2; ++i)
                            ilendiv[i] = div(testlen, i);
                        break;
                    }

                    ilenkills[c] = 0;
                    if (--test[c] < '0')
                        test[c] = '9';
                    else
                        break;
                }
                minchange = (c - (ARRLEN - testlen));
            }
            if (minchange <= imin)
                imin = min_possible(test + ARRLEN - testlen, testlen);
        }
    }

    print_uint64(answer1);
    print_uint64(answer2);

    DEBUGLOG("checked %" PRIu64 " ilens\n", ilens);

    return 0;
}
