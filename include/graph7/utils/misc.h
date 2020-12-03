#ifndef _GRAPH7_MISC_H
#define _GRAPH7_MISC_H

#include <graph7/defs.h>

#define GRAPH7_LITTLE_ENDIAN 0
#define GRAPH7_BIG_ENDIAN 1

#ifdef __cplusplus
extern "C" {
#endif

#define GRAPH7_MIDX(i, j, order)\
    ((i) * (order) + (j))

#define GRAPH7_WMIDX(i, j, order, width)\
    ((i) * (order) * (width) + (j) * (width))

static inline uint8_t utils_endianness(void)
{
    uint32_t x = 1;
    return (((uint8_t *)&x)[0]) ? 0 : 1;
}

static inline void utils_reverse(uint8_t *src, size_t length)
{
    for(size_t i = 0; i < length / 2; i++)
    {
        uint8_t tmp = src[i];
        src[i] = src[length - i - 1];
        src[length - i - 1] = tmp;
    }
}

static inline size_t utils_ceiling_div(size_t numerator, size_t denominator)
{
    return !!(numerator % denominator) + numerator / denominator;
}

static inline size_t utils_count_bits(size_t x)
{
    size_t k = 1;
    while(1 << k <= x)
        ++k;
    return k;
}

static inline bool utils_bytes_start_with(const uint8_t *src, const uint8_t *start, size_t start_length)
{
    for(size_t i = 0; i < start_length; i++)
    {
        if(src[i] != start[i])
            return false;
    }

    return true;
}

#endif // GRAPH7_UTILS_H

#ifdef __cplusplus
}

#endif // _GRAPH7_MISC_H
