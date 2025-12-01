#pragma once

#define isdigit(c) ((c) >= '0' && (c) <= '9')

#define PARSEINT(num, data, idx) \
        while (isdigit((data)[idx])) \
            (num) = ((num) * 10) + ((data)[(idx)++] & 0xF);
