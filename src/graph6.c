#include <graph7/graph6.h>

ssize_t graph7_graph6_size_encode(uint8_t *dst, size_t size)
{
    // Case 1
    if(size < 63)
    {
        dst[0] = (uint8_t)(size + 63);
        return 1;
    }

    // Case 2 (default values)
    uint8_t padding = 1;
    uint8_t nsextet = 3;

    // Case 3
    if(size > 258047 && size < 68719476736)
    {
        padding = 2;
        nsextet = 6;
    }

    if(size > 68719476735)
        return -GRAPH7_UNSUPPORTED;

    for(uint8_t i = 0; i < padding; i++)
        dst[i] = 126;

    for(uint8_t i = 0; i < nsextet; i++)
    {
        uint8_t offset = (nsextet - 1 - i) * 6;
        dst[i + padding] = (uint8_t)(63 + ((size & (63 << offset)) >> offset));
    }

    return padding * 4;
}

ssize_t graph7_graph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order)
{
    ssize_t offset = graph7_graph6_size_encode(dst, order);

    if(offset < 0)
        return offset;

    size_t bits = 0;
    size_t bytes = offset;

    for(size_t i = 1; i < order; i++)
    {
        for(size_t j = 1; j < i + 1; j++)
        {
            dst[bytes] |= (!!src[GRAPH7_M_INDEX(j - 1, i, order)]) << (5 - (bits % 6));
            ++bits;

            if(bits % 6 == 0)
            {
                dst[bytes] += 63;
                bytes += 1;
            }
        }
    }

    if(bits % 6 != 0)
    {
        dst[bytes] += 63;
        bytes += 1;
    }

    return bytes;
}

ssize_t graph7_graph6_decode_to_matrix(uint8_t *dst, const uint8_t *src)
{

}
