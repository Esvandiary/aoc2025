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

    int32_t removeidx[4096];
    uint64_t toremove;

    for (const chartype* cd = data; cd != end; ++cd)
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

        nearcounts[cd - data] = near;
    }

    do
    {
        toremove = 0;

        const chartype* nc = nearcounts;
        for (const chartype* cd = data; cd != end; ++cd, ++nc)
        {
            if (*cd != '@')
                continue;
            if (*nc < 4)
                removeidx[toremove++] = cd - data;
        }

        DEBUGLOG("removing %" PRIu64 "\n", toremove);
        if (answer1 == 0)
            answer1 = toremove;
        answer2 += toremove;

        for (size_t i = 0; i < toremove; ++i)
        {
            data[removeidx[i]] = ',';
            uint8_t* const nccur = nearcounts + removeidx[i];
            --nccur[-linewidth-1];
            --nccur[-linewidth];
            --nccur[-linewidth+1];
            --nccur[-1];
            nccur[0] = 0xFF;
            --nccur[1];
            --nccur[linewidth-1];
            --nccur[linewidth];
            --nccur[linewidth+1];
        }
    } while (toremove);

    // DEBUGLOG("%.*s\n", fileSize, file.data);

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
