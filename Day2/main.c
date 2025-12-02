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
        char* const tend = test + ARRLEN;
        memcpy(tend - num2len, file.data + num2start, num2len);
        int testlen = num2len;
        int testlendiv2 = testlen >> 1;
        int imin = min_possible(tend - testlen, testlendiv2 + 1);

        int ilenmod[ARRLEN];
        for (int i = 1; i <= testlendiv2; ++i)
            ilenmod[i] = testlen % i;

        int ilenkills[ARRLEN] = {0};

        for (uint64_t ntest = num2; ntest >= num1; --ntest)
        {
            //DEBUGLOG("===== %" PRIu64 " =====\n", ntest);
            for (int ilen = testlendiv2; ilen >= imin; --ilen)
            {
                if (ilenkills[ilen] == 0 && ilenmod[ilen] == 0)
                {
                    DEBUGDO(++ilens);
                    const char* const t1 = tend - testlen;
                    const char* t2 = t1 + ilen;

                    for (int i = 0; t2 < tend; ++i, ++t2)
                    {
                        if (i == ilen) i = 0;
                        if (*(t1 + i) != *t2)
                        {
                            ilenkills[ilen] = t2 - test;
                            goto nextlen;
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
            int minchange = ARRLEN-1;
            if (--test[ARRLEN-1] < '0')
            {
                int c;
                for (c = ARRLEN-1; c >= ARRLEN - testlen; --c)
                {
                    if (c == ARRLEN - testlen && test[c] == '1')
                    {
                        --testlen;
                        testlendiv2 = testlen >> 1;
                        for (int i = 1; i <= testlendiv2; ++i)
                            ilenmod[i] = testlen % i;
                        break;
                    }

                    if (--test[c] < '0')
                        test[c] = '9';
                    else
                        break;
                }
                minchange = c;
            }
            for (int i = 1; i <= testlendiv2; ++i)
                if (ilenkills[i] >= minchange)
                    ilenkills[i] = 0;
            if (minchange - (ARRLEN - testlen) <= imin)
                imin = min_possible(tend - testlen, testlendiv2 + 1);
        }
    }

    print_uint64(answer1);
    print_uint64(answer2);

    DEBUGLOG("checked %" PRIu64 " ilens\n", ilens);

    return 0;
}
