#include <graph7/graph6.h>
#include <graph7/utils/misc.h>
#include <graph7/utils/bitstream.h>
#include <stdlib.h>
#include <string.h>

//==============================================================================
// PRIVATE
static inline ssize_t sextet_decode(uint8_t *dst, const uint8_t *src, size_t length)
{
    for(size_t i = 0; i < length; i++)
    {
        dst[i] = src[i] - 63;
        GRAPH7_ERROR(dst[i] > 63, -GRAPH7_INVALID_DATA);
    }

    return 0;
}

//==============================================================================
// PUBLIC
#ifdef __cplusplus
extern "C" {
#endif

ssize_t graph6_order_encode(uint8_t *dst, size_t order)
{
    GRAPH7_ERROR(!dst, -GRAPH7_INVALID_ARG);

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

    GRAPH7_ERROR(order > 68719476735, -GRAPH7_UNSUPPORTED);

    for(uint8_t i = 0; i < nsextet; i++)
    {
        uint8_t offset = (nsextet - (i + 1)) * 6;
        dst[i + padding] = (uint8_t)(63 + ((order & (63 << offset)) >> offset));
    }

    return padding + nsextet;
}

ssize_t graph6_order_decode(size_t *dst, const uint8_t *src, size_t length)
{
    GRAPH7_ERROR(!length, -GRAPH7_INVALID_LENGTH);
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    // Case 1
    if(src[0] != 126)
    {
        *dst = src[0] - 63;
        return 1;
    }

    // Case 2 (default values)
    GRAPH7_ERROR(length < 4, -GRAPH7_INVALID_LENGTH);
    uint8_t padding = 1;
    uint8_t nsextet = 3;

    // Case 3
    if(src[1] == 126)
    {
        GRAPH7_ERROR(length < 8, -GRAPH7_INVALID_LENGTH);
        padding = 2;
        nsextet = 6;
    }

    GRAPH7_ERROR(sizeof(size_t) < nsextet, -GRAPH7_UNSUPPORTED);

    *dst = 0;

    for(uint8_t i = 0; i < nsextet; i++)
    {
        uint8_t offset = (nsextet - (i + 1)) * 6;
        *dst |= ((size_t)(src[i + padding] - 63) << offset);
    }

    return padding + nsextet;
}

ssize_t graph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, bool header)
{
    GRAPH7_ERROR(!dst || !src,  -GRAPH7_INVALID_ARG);

    size_t bytes = 0;

    if(header)
    {
        memmove(dst, GRAPH6_HEADER, GRAPH6_HEADER_LEN);
        bytes += GRAPH6_HEADER_LEN;
    }

    ssize_t offset = graph6_order_encode(&dst[bytes], order);

    if(offset < 0)
        return offset;

    bytes += offset;

    bitstream_t stream;
    bitstream_init(&stream, &dst[bytes]);

    for(size_t i = 1; i < order; i++)
    {
        for(size_t j = 0; j < i; j++)
            bitstream_write(&stream, src[GRAPH7_MIDX(j, i, order)]);
    }

    size_t encoded = (size_t)bitstream_bytes_count(&stream);

    for(size_t i = bytes; i < bytes + encoded; i++)
        dst[i] += 63;

    return bytes + encoded;
}

ssize_t graph6_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length)
{
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    size_t bytes = 0;

    // Checking optional header
    if(src[0] == '>')
    {
        GRAPH7_ERROR(length <= GRAPH6_HEADER_LEN, -GRAPH7_INVALID_LENGTH);
        GRAPH7_ERROR(
            !graph7_utils_bytes_start_with(src, GRAPH6_HEADER, GRAPH6_HEADER_LEN),
            -GRAPH7_INVALID_HEADER
        );

        bytes = GRAPH6_HEADER_LEN;
    }

    // At least one byte of information must occupy the graph
    GRAPH7_ERROR(length < bytes + 1, -GRAPH7_INVALID_LENGTH);

    size_t order;
    ssize_t offset = graph6_order_decode(&order, &src[bytes], length);

    GRAPH7_ERROR(offset < 0, offset);

    bytes += offset;

    size_t data_length = graph7_utils_ceiling_div(order * (order - 1) / 2, 6);

    // Checking that the length is not less than the need
    GRAPH7_ERROR(length < bytes + data_length, -GRAPH7_INVALID_LENGTH);

    uint8_t *bytearray = (uint8_t *)malloc(data_length);

    GRAPH7_ERROR(!bytearray, -GRAPH7_ALLOC_ERROR);

    // Decoding and copy to other array
    ssize_t status = sextet_decode(bytearray, &src[bytes], data_length);

    if(status < 0)
        goto _exit;

    bitstream_t stream;
    bitstream_init(&stream, bytearray);

    for(size_t i = 1; i < order; i++)
    {
        for(size_t j = 0; j < i; j++)
        {
            bool value = bitstream_read(&stream);

            dst[GRAPH7_MIDX(j, i, order)] = value; // Upper triangle
            dst[GRAPH7_MIDX(i, j, order)] = value; // Lower triangle
        }
    }

    // Main diagonal must be 0
    for(size_t i = 0; i < order; i++)
        dst[GRAPH7_MIDX(i, i, order)] = 0;

_exit:
    free((void *)bytearray);
    return status < 0 ? status : order;
}

ssize_t digraph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, bool header)
{
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    size_t bytes = 0;

    if(header)
    {
        memmove(dst, DIGRAPH6_HEADER, DIGRAPH6_HEADER_LEN);
        bytes += DIGRAPH6_HEADER_LEN;
    }

    ssize_t offset = graph6_order_encode(&dst[bytes + 1], order);

    GRAPH7_ERROR(offset < 0,  offset);

    dst[bytes] = '&';
    bytes += offset + 1;

    bitstream_t stream;
    bitstream_init(&stream, &dst[bytes]);

    for(size_t i = 0; i < order * order; i++)
        bitstream_write(&stream, src[i]);

    size_t encoded = (size_t)bitstream_bytes_count(&stream);

    for(size_t i = bytes; i < bytes + encoded; i++)
        dst[i] += 63;

    return bytes + encoded;
}

ssize_t digraph6_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length)
{
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    size_t bytes = 0;

    // Checking optional header
    if(src[0] == '>')
    {
        GRAPH7_ERROR(length <= DIGRAPH6_HEADER_LEN, -GRAPH7_INVALID_LENGTH);
        GRAPH7_ERROR(
            !graph7_utils_bytes_start_with(src, DIGRAPH6_HEADER, DIGRAPH6_HEADER_LEN),
            -GRAPH7_INVALID_HEADER;
        );


        bytes = DIGRAPH6_HEADER_LEN;
    }

    // At least two bytes (required header and order of graph) of information must occupy the graph
    GRAPH7_ERROR(length < bytes + 2, -GRAPH7_INVALID_LENGTH);
    // Checking required header
    GRAPH7_ERROR(src[bytes] != '&', -GRAPH7_INVALID_HEADER);

    size_t order;
    ssize_t offset = graph6_order_decode(&order, &src[bytes + 1], length);

    GRAPH7_ERROR(offset < 0, offset);

    bytes += offset + 1;

    size_t data_length = graph7_utils_ceiling_div(order * order, 6);

    // Checking that the length is not less than the need
    GRAPH7_ERROR(length != bytes + data_length, -GRAPH7_INVALID_LENGTH);

    uint8_t *bytearray = (uint8_t *)malloc(data_length);

    GRAPH7_ERROR(!bytearray, -GRAPH7_ALLOC_ERROR);

    // Decoding and copy to other array
    ssize_t status = sextet_decode(bytearray, &src[bytes], data_length);

    if(status < 0)
        goto _exit;

    bitstream_t stream;
    bitstream_init(&stream, bytearray);

    for(size_t i = 0; i < order * order; i++)
        dst[i] = bitstream_read(&stream);

_exit:
    free((void *)bytearray);
    return status < 0 ? status : order;
}

ssize_t sparse6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, uint8_t header)
{
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    size_t bytes = 0;

    if(header)
    {
        memmove(dst, SPARSE6_HEADER, SPARSE6_HEADER_LEN);
        bytes += SPARSE6_HEADER_LEN;
    }

    ssize_t offset = graph6_order_encode(&dst[bytes + 1], order);

    GRAPH7_ERROR(offset < 0, offset);

    dst[bytes] = ':';
    bytes += offset + 1;

    if(order < 2)
        return bytes;

    bitstream_t stream;
    bitstream_init(&stream, &dst[bytes]);

    size_t k = graph7_utils_count_bits(order - 1);
    size_t current = 0;

    for(size_t v = 1; v < order; v++)
    {
        for(size_t u = 0; u < v; u++)
        {
            for(size_t p = 0; p < src[GRAPH7_MIDX(u, v, order)]; p++)
            {
                if(v == current)
                {
                    bitstream_write(&stream, 0);
                    bitstream_encode_number(&stream, u, k);
                }
                else if(v == current + 1)
                {
                    ++current;
                    bitstream_write(&stream, 1);
                    bitstream_encode_number(&stream, u, k);
                }
                else
                {
                    current = v;
                    bitstream_write(&stream, 1);
                    bitstream_encode_number(&stream, v, k);
                    bitstream_write(&stream, 0);
                    bitstream_encode_number(&stream, u, k);
                }
            }
        }
    }

    uint8_t tail = stream.bitp ? 6 - (stream.bitp) : 0;

    if(k < 6 && order == (1 << k) && tail >= k && current < (order - 1))
        bitstream_write(&stream, 0);

    for(size_t i = 0; i < tail; i++)
        bitstream_write(&stream, 1);

    size_t encoded = bitstream_bytes_count(&stream);

    for(size_t i = bytes; i < bytes + encoded; i++)
        dst[i] += 63;

    return bytes + encoded;
}

ssize_t sparse6_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length)
{
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    size_t bytes = 0;

    // Checking optional header
    if(src[0] == '>')
    {
        GRAPH7_ERROR(length <= SPARSE6_HEADER_LEN, -GRAPH7_INVALID_LENGTH);
        GRAPH7_ERROR(
            !graph7_utils_bytes_start_with(src, SPARSE6_HEADER, SPARSE6_HEADER_LEN),
            -GRAPH7_INVALID_HEADER
        );

        bytes = SPARSE6_HEADER_LEN;
    }

    // Checking required header
    GRAPH7_ERROR(src[bytes] != ':', -GRAPH7_INVALID_HEADER);

    size_t order;
    ssize_t offset = graph6_order_decode(&order, &src[bytes + 1], length);

    GRAPH7_ERROR(offset < 0,  offset);

    bytes += offset + 1;

    // TODO: Make checking about correct length
    size_t data_length = length - bytes;

    if(!data_length)
        return order;

    uint8_t *bytearray = (uint8_t *)malloc(data_length);

    GRAPH7_ERROR(!bytearray, -GRAPH7_ALLOC_ERROR);

    // Decoding and copy to other array
    ssize_t status = sextet_decode(bytearray, &src[bytes], data_length);

    if(status < 0)
        goto _exit;

    memset((void *)dst, 0, order * order); // For ++ action

    bitstream_t stream;
    bitstream_init(&stream, bytearray);

    size_t k = graph7_utils_count_bits(order - 1);
    size_t bx_size = (data_length * 6) / (k + 1);

    size_t v = 0;
    for(size_t i = 0; i < bx_size; i++)
    {
        bool b = bitstream_read(&stream);
        size_t x = bitstream_decode_number(&stream, k);

        if(b)
            ++v;

        if(x >= order || v >= order)
        {
            break;
        }
        else if(x > v)
        {
            v = x;
        }
        else
        {
            ++dst[GRAPH7_MIDX(x, v, order)];
            ++dst[GRAPH7_MIDX(v, x, order)];
        }
    }

_exit:
    free(bytearray);
    return status < 0 ? status : order;
}

#ifdef __cplusplus
}
#endif
