// #define ENABLE_DEBUGLOG
#include "../common/mmap.h"
#include "../common/print.h"
#include "../common/view.h"

#define MAXSIZE 192

#define AT(x, y) (file.data[((x) + (y)*linewidth)])

int main(int argc, char** argv)
{
    mmap_file file = mmap_file_open_ro("input.txt");
    const int fileSize = (int)(file.size);

    //
    // content
    //

    int idx = 0;

    uint64_t answer1 = 0, answer2 = 0;

    while (file.data[idx++] != '\n');
    int linewidth = idx;
    int width = linewidth-1;

    uint64_t toremove;

    do
    {
        toremove = 0;

        for (int i = 0; i < fileSize; ++i)
        {
            if (file.data[i] & 1)
                file.data[i] = ',';
        }

        for (int y = 0; y < width; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (AT(x,y) != '@')
                    continue;

                int near = 0;
                if (y > 0)
                {
                    if (x > 0)
                        near += AT(x-1, y-1) >> 6;
                    near += AT(x, y-1) >> 6;
                    if (x+1 < width)
                        near += AT(x+1, y-1) >> 6;
                }
                if (x > 0)
                    near += AT(x-1, y) >> 6;
                if (x+1 < width)
                    near += AT(x+1, y) >> 6;
                if (y+1 < width)
                {
                    if (x > 0)
                        near += AT(x-1, y+1) >> 6;
                    near += AT(x, y+1) >> 6;
                    if (x+1 < width)
                        near += AT(x+1, y+1) >> 6;
                }

                if (near < 4)
                {
                    AT(x,y) |= 1;
                    ++toremove;
                }
            }
        }

        DEBUGLOG("removing %" PRIu64 "\n", toremove);
        if (answer1 == 0)
            answer1 = toremove;
        answer2 += toremove;
    } while (toremove);

    // DEBUGLOG("%.*s\n", fileSize, file.data);

    print_uint64(answer1);
    print_uint64(answer2);

    return 0;
}
