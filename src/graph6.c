#include <graph7/graph6.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

ssize_t graph7_graph6_order_encode(uint8_t *dst, size_t order)
{
    if(!dst || order < 2)
        return -GRAPH7_INVALID_ARG;

    // Case 1
    if(order < 63)
    {
        dst[0] = (uint8_t)(order + 63);
        return 1;
    }

    // Case 2 (default values)
    uint8_t padding = 1;
    uint8_t nsextet = 3;
    dst[0] = 126;

    // Case 3
    if(order > 258047 && order < 68719476736)
    {
        padding = 2;
        nsextet = 6;
        dst[1] = 126;
    }

    if(order > 68719476735)
        return -GRAPH7_UNSUPPORTED;

    for(uint8_t i = 0; i < nsextet; i++)
    {
        uint8_t offset = (nsextet - (i + 1)) * 6;
        dst[i + padding] = (uint8_t)(63 + ((order & (63 << offset)) >> offset));
    }

    return padding + nsextet;
}

ssize_t graph7_graph6_order_decode(size_t *dst, const uint8_t *src)
{
    if(!dst || !src)
        return -GRAPH7_INVALID_ARG;

    // Case 1
    if(src[0] != 126)
    {
        *dst = src[0] - 63;
        return 1;
    }

    // Case 2 (default values)
    uint8_t padding = 1;
    uint8_t nsextet = 3;

    // Case 3
    if(src[1] == 126)
    {
        padding = 2;
        nsextet = 6;
    }

    if(sizeof(size_t) < nsextet)
        return -GRAPH7_UNSUPPORTED;

    *dst = 0;

    for(uint8_t i = 0; i < nsextet; i++)
    {
        uint8_t offset = (nsextet - (i + 1)) * 6;
        *dst |= ((size_t)(src[i + padding] - 63) << offset);
    }

    return padding + nsextet;
}

ssize_t graph7_graph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order)
{
    if(!src)
        return -GRAPH7_INVALID_ARG;

    ssize_t offset = graph7_graph6_order_encode(dst, order);

    if(offset < 0)
        return offset;

    size_t bits = 0;
    size_t bytes = offset;

    // Clear dst
    memset((void *)&dst[offset], 0, graph7_utils_ceiling_div(order * (order - 1) / 2, 6));

    for(size_t i = 1; i < order; i++)
    {
        for(size_t j = 0; j < i; j++)
        {
            dst[bytes] |= (!!src[GRAPH7_M_INDEX(j, i, order)]) << (5 - (bits % 6)); // Get only upper triangle
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
    if(!dst)
        return -GRAPH7_INVALID_ARG;

    size_t order;
    ssize_t offset = graph7_graph6_order_decode(&order, src);

    if(offset < 0)
        return offset;

    size_t bits = 0;
    size_t bytes = offset;

    for(size_t i = 1; i < order; i++)
    {
        for(size_t j = 0; j < i; j++)
        {
            uint8_t value = ((src[bytes] - 63) >> (5 - (bits % 6))) & 1;
            dst[GRAPH7_M_INDEX(j, i, order)] = value; // Upper triangle
            dst[GRAPH7_M_INDEX(i, j, order)] = value; // Lower triangle

            ++bits;

            if(bits % 6 == 0)
                bytes += 1;
        }
    }

    // Main diagonal must be 0
    for(size_t i = 0; i < order; i++)
        dst[GRAPH7_M_INDEX(i, i, order)] = 0;

    return order;
}

#ifdef __cplusplus
}
#endif
