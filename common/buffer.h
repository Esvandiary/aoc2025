#pragma once

#include "common.h"
#include "view.h"
#include <stdio.h>
#include <string.h>

typedef view buffer;

static buffer buffer_create(size_t sz)
{
    buffer buf;
    buf.data = (chartype*)malloc(sz);
    buf.size = sz;
    return buf;
}

static void buffer_destroy(buffer* buf)
{
    if (!buf)
        return;
    free(buf->data);
    buf->size = 0;
}