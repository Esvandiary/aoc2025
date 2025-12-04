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

    char* const data = datastore + MAXSIZE;

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

        for (int y = 0; y < width; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (AT(x,y) != '@')
                    continue;

                int near = 0;

                near += AT(x-1, y-1) >> 6;
                near += AT(x, y-1) >> 6;
                near += AT(x+1, y-1) >> 6;

                near += AT(x-1, y) >> 6;
                near += AT(x+1, y) >> 6;

                near += AT(x-1, y+1) >> 6;
                near += AT(x, y+1) >> 6;
                near += AT(x+1, y+1) >> 6;

                if (near < 4)
                    removeidx[toremove++] = (x << 16) | y;
            }
        }

        DEBUGLOG("removing %" PRIu64 "\n", toremove);
        if (answer1 == 0)
            answer1 = toremove;
        answer2 += toremove;

        for (size_t i = 0; i < toremove; ++i)
            AT(removeidx[i] >> 16, removeidx[i] & 0xFFFF) = ',';
    } while (toremove);

    // DEBUGLOG("%.*s\n", fileSize, file.data);

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
