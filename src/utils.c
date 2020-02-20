#include <graph7/utils.h>
#include <string.h>

uint8_t graph7_utils_endianness(void)
{
    uint32_t x = 1;
    return (((uint8_t *)&x)[0]) ? 0 : 1;
}

void graph7_utils_reverse(uint8_t *src, size_t length)
{
    if(!src)
        return;

    for(size_t i = 0; i < length / 2; i++)
    {
        uint8_t tmp = src[i];
        src[i] = src[length - i - 1];
        src[length - i - 1] = tmp;
    }
}

size_t graph7_utils_sextet_pack(uint8_t *dst, const uint8_t *src, size_t length, uint8_t *tail)
{
    if(!dst || !src || !length)
        return 0;

    uint8_t t = length % 6;
    size_t c = length / 6;
    size_t i, j, k;

    memset(dst, 0, (t ? c + 1 : c));

    for(i = 0, k = 0; i < c; i++)
    {
        for(j = 0; j < 6; j++)
            dst[i] |= (!!src[k++]) << (5 - j);
    }

    for(j = 0; j < t; j++)
        dst[i] |= (!!src[k++]) << (5 - j);

    if(tail)
        *tail = t;

    return (t ? c + 1 : c);
}

size_t graph7_utils_sextet_unpack(uint8_t *dst, const uint8_t *src, size_t length, uint8_t tail)
{
    if(!dst || !src || !length)
        return 0;

    size_t c = (tail) ? length - 1 : length;
    size_t i, j, k;

    for(i = 0, k = 0; i < c; i++)
    {
        for(j = 0; j < 6; j++)
            dst[k++] = (src[i] >> (5 - j)) & 1;
    }

    for(j = 0; j < tail; j++)
        dst[k++] = (src[i] >> (5 - j)) & 1;

    return k;
}
