#pragma once

#include "common.h"
#include "buffer.h"
#include <stdio.h>

static buffer basic_file_read(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer b = buffer_create(fsize);
    b.size = fread(b.data, sizeof(chartype), b.size, f);

    fclose(f);

    return b;
}

static size_t basic_file_read_inplace(const char* filename, chartype* buf, size_t bufsize)
{
    FILE* f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    size_t readsize = fread(buf, sizeof(chartype), bufsize, f);

    fclose(f);

    return readsize;
}