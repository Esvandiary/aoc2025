// #define ENABLE_DEBUGLOG
#include "../common/basicfile.h"
#include "../common/print.h"
#include "../common/view.h"

#define MAXSIZE 192

#define AT(x, y) (data[((x) + (y)*linewidth)])

static chartype datastore[MAXSIZE*MAXSIZE] = {0};

int main(int argc, char** argv)
{
    const size_t fileSize = basic_file_read_inplace("input.txt", datastore + MAXSIZE, sizeof(chartype) * (sizeof(datastore) - MAXSIZE*2));

    chartype* const data = datastore + MAXSIZE;
    const chartype* const end = data + fileSize;

    //
    // content
    //

    int idx = 0;

    uint64_t answer1 = 0, answer2 = 0;

    while (data[idx++] != '\n');
    int linewidth = idx;
    int width = linewidth-1;

    uint32_t removeidx[4096];
    uint64_t toremove;

    do
    {
        toremove = 0;

        const chartype* cd = data;
        while (cd != end)
        {
            if (*cd != '@')
                goto skip;

            int near = 0;

            near += cd[-linewidth-1] >> 6;
            near += cd[-linewidth] >> 6;
            near += cd[-linewidth+1] >> 6;

            near += cd[-1] >> 6;
            near += cd[1] >> 6;

            near += cd[linewidth-1] >> 6;
            near += cd[linewidth] >> 6;
            near += cd[linewidth+1] >> 6;

            if (near < 4)
                removeidx[toremove++] = cd - data;

        skip:
            ++cd;
        }

        DEBUGLOG("removing %" PRIu64 "\n", toremove);
        if (answer1 == 0)
            answer1 = toremove;
        answer2 += toremove;

        for (size_t i = 0; i < toremove; ++i)
            data[removeidx[i]] = ',';
    } while (toremove);

    // DEBUGLOG("%.*s\n", fileSize, file.data);

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
