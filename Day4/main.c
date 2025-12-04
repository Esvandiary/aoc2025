// #define ENABLE_DEBUGLOG
#include "../common/basicfile.h"
#include "../common/print.h"
#include "../common/view.h"

#define MAXSIZE 192

#define AT(x, y) (data[((x) + (y)*linewidth)])

static chartype datastore[MAXSIZE*MAXSIZE] = {0};
static uint8_t nearcountstore[MAXSIZE*MAXSIZE] = {0};

int main(int argc, char** argv)
{
    const size_t fileSize = basic_file_read_inplace("input.txt", datastore + MAXSIZE, sizeof(chartype) * (sizeof(datastore) - MAXSIZE*2));

    chartype* const data = datastore + MAXSIZE;
    const chartype* const end = data + fileSize;

    uint8_t* const nearcounts = nearcountstore + MAXSIZE;

    //
    // content
    //

    int idx = 0;

    uint64_t answer1 = 0, answer2 = 0;

    while (data[idx++] != '\n');
    const int linewidth = idx;

    DEBUGLOG("linewidth = %d\n", linewidth);

    int16_t removeidx[2][8192];
    int ri = 0;
    uint64_t toremove = 0;
    uint64_t removed = 0;

    uint8_t* nc = nearcounts;
    for (chartype* cd = data; cd != end; ++cd, ++nc)
    {
        if (*cd != '@')
            continue;

        uint8_t near = 0;

        near += cd[-linewidth-1] >> 6;
        near += cd[-linewidth] >> 6;
        near += cd[-linewidth+1] >> 6;

        near += cd[-1] >> 6;
        near += cd[1] >> 6;

        near += cd[linewidth-1] >> 6;
        near += cd[linewidth] >> 6;
        near += cd[linewidth+1] >> 6;

        *nc = near;
        if (near < 4)
            removeidx[0][toremove++] = cd - data;
    }

    for (size_t i = 0; i < toremove; ++i)
    {
        data[removeidx[0][i]] = ',';
        uint8_t* const nccur = nearcounts + removeidx[0][i];
        --nccur[-linewidth-1];
        --nccur[-linewidth];
        --nccur[-linewidth+1];
        --nccur[-1];
        --nccur[1];
        --nccur[linewidth-1];
        --nccur[linewidth];
        --nccur[linewidth+1];
    }

    answer1 = toremove;
    answer2 = toremove;

    DEBUGLOG("removed = %" PRIu64 "\n", toremove);

    ri = 1;

    do
    {
        const uint64_t oldtoremove = toremove;
        toremove = 0;

        for (size_t i = 0; i < oldtoremove; ++i)
        {
            const int16_t ridx = removeidx[(ri+1)&1][i];
            const chartype* const cd = data + ridx;
            const uint8_t* const cnc = nearcounts + ridx;

            if (*cd != ',')
                continue;

            DEBUGLOG("[%zu] ri = %d, ori = %d, ridx = %d, ch = %c\n", i, ri, (ri+1)&1, ridx, *cd);
                
            if (cd[-linewidth-1] == '@' && cnc[-linewidth-1] < 4)
                removeidx[ri][toremove++] = ridx - linewidth - 1;
            if (cd[-linewidth] == '@' && cnc[-linewidth] < 4)
                removeidx[ri][toremove++] = ridx - linewidth;
            if (cd[-linewidth+1] == '@' && cnc[-linewidth+1] < 4)
                removeidx[ri][toremove++] = ridx - linewidth + 1;
            if (cd[-1] == '@' && cnc[-1] < 4)
                removeidx[ri][toremove++] = ridx - 1;
            if (cd[1] == '@' && cnc[1] < 4)
                removeidx[ri][toremove++] = ridx + 1;
            if (cd[linewidth-1] == '@' && cnc[linewidth-1] < 4)
                removeidx[ri][toremove++] = ridx + linewidth - 1;
            if (cd[linewidth] == '@' && cnc[linewidth] < 4)
                removeidx[ri][toremove++] = ridx + linewidth;
            if (cd[linewidth+1] == '@' && cnc[linewidth+1] < 4)
                removeidx[ri][toremove++] = ridx + linewidth + 1;
        }

        removed = 0;
        for (size_t i = 0; i < toremove; ++i)
        {
            if (data[removeidx[ri][i]] != '@')
            {
                removeidx[ri][i] = -1;
                continue;
            }
            ++removed;
            data[removeidx[ri][i]] = ',';
            DEBUGLOG("setting ridx %u\n", removeidx[ri][i]);
            uint8_t* const nccur = nearcounts + removeidx[ri][i];
            --nccur[-linewidth-1];
            --nccur[-linewidth];
            --nccur[-linewidth+1];
            --nccur[-1];
            --nccur[1];
            --nccur[linewidth-1];
            --nccur[linewidth];
            --nccur[linewidth+1];
        }

        answer2 += removed;
        DEBUGLOG("removed %" PRIu64 "\n", removed);
        DEBUGLOG("%.*s\n", (int)fileSize, data);

        ri = (ri+1)&1;
    } while (toremove);

    DEBUGLOG("%.*s\n", (int)fileSize, data);

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
