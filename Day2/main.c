// #define ENABLE_DEBUGLOG
#include "../common/mmap.h"
#include "../common/basicfile.h"
#include "../common/print.h"
#include "../common/view.h"
#include "../common/intparse.h"

// #define USE_NUMBERS

#define ARRLEN 32

static const uint64_t mods[] = {
    1ULL, 10ULL, 100ULL, 1000ULL,
    10000ULL, 100000ULL, 1000000ULL, 10000000ULL,
    100000000ULL, 1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL, 1000000000000000ULL
};


int main(int argc, char** argv)
{
    mmap_file file = mmap_file_open_ro("input.txt");
    const int fileSize = (int)(file.size);

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

#ifdef USE_NUMBERS
        int testlen = num2len;
        for (uint64_t ntest = num2; ntest >= num1; --ntest)
        {
            if (ntest / mods[testlen-1] == 0)
                --testlen;

            DEBUGLOG("===== %" PRIu64 " (%d) =====\n", ntest, testlen);
            for (int ilen = testlen / 2; ilen >= 1; --ilen)
            {
                if ((testlen % ilen) == 0)
                {
                    uint64_t ntestcur = ntest / mods[ilen];
                    const int iend = testlen / ilen;
                    for (int itest = 1; itest < iend; ++itest, ntestcur /= mods[ilen])
                    {
                        const uint64_t ntest1 = ntest % mods[ilen];
                        const uint64_t ntest2 = ntestcur % mods[ilen];
                        // DEBUGLOG("testing: %" PRIu64 " vs %" PRIu64 " (%d, %d, %" PRIu64 ", %" PRIu64 ")\n", ntest1, ntest2, ilen, itest, mods[itest], mods[ilen]);
                        if (ntest1 != ntest2)
                            goto nextlen;
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
                }
nextlen:
                (void)0;
            }
        }
#else
        char test[ARRLEN];
        memcpy(test + ARRLEN - num2len, file.data + num2start, num2len);
        int testlen = num2len;
        for (uint64_t ntest = num2; ntest >= num1; --ntest)
        {
            for (int ilen = testlen / 2; ilen >= 1; --ilen)
            {
                if ((testlen % ilen) == 0)
                {
                    const int iend = testlen / ilen;
                    for (int itest = 0; itest < iend; ++itest)
                    {
                        if (memcmp(test + ARRLEN - testlen, test + ARRLEN - testlen + ilen*itest, ilen) != 0)
                            goto nextlen;
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
            if (--test[ARRLEN-1] < '0')
            {
                for (int c = ARRLEN-1; c >= ARRLEN - testlen; --c)
                {
                    if (c == ARRLEN - testlen && test[c] == '1')
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
#endif
    }

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
