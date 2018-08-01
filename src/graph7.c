#include "graph7.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

//==============================================================================
// PRIVATE

#define BUFFER_SIZE 256

typedef union graph7_header
{
    struct
    {
        uint8_t weighed     :1;
        uint8_t gtype       :2;
        uint8_t tail        :3;
        uint8_t reserved    :2;
    };

    uint8_t byte;
} graph7_header_t;

typedef union graph7_wheader
{
    struct
    {
        uint8_t extended        :1;
        uint8_t width           :5;
        uint8_t reserved        :2;
    };

    uint8_t byte;
} graph7_wheader_t;

static uint8_t encoding_table[64] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};

static uint8_t decoding_table[128] =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
    0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
    0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff,
};

/*
    Check endianess
    return: 0 - little-endian, 1 - big-endian
*/
static inline uint8_t endianness()
{
    uint32_t x = 1;
    return (((uint8_t *)&x)[0]) ? 0 : 1;
}

/*
    Reverse bytearray
*/
static inline void reverse(uint8_t *src, uint32_t length)
{
    uint32_t i;

    for(i = 0; i < length / 2; i++)
    {
        uint8_t tmp = src[i];
        src[i] = src[length - i - 1];
        src[length - i - 1] = tmp;
    }
}

static inline uint32_t sextet_pack(uint8_t *dst, const uint8_t *src, uint32_t length, uint8_t *tail)
{
    uint8_t t = length % 6;
    uint32_t c = length / 6;
    uint32_t i, j, k;

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

static inline uint32_t sextet_unpack(uint8_t *dst, const uint8_t *src, uint32_t length, uint8_t tail)
{
    uint32_t c = (tail) ? length - 1 : length;
    uint32_t i, j, k;

    for(i = 0, k = 0; i < c; i++)
    {
        for(j = 0; j < 6; j++)
            dst[k++] = (src[i] >> (5 - j)) & 1;
    }

    for(j = 0; j < tail; j++)
        dst[k++] = (src[i] >> (5 - j)) & 1;

    return k;
}

static inline uint8_t width_pack(uint8_t *dst, uint32_t width)
{
    uint32_t new_width = width;
    uint8_t c;

    if(new_width < 64) c = 1;
    else if(new_width < 4096) c = 2;
    else if(new_width < 262144) c = 3;
    else if(new_width < 16777216) c = 4;
    else return 0;

    if(endianness())
        reverse((uint8_t *)&new_width, sizeof(uint32_t));

    if(c > 0) dst[0] = new_width & 0b00111111;
    if(c > 1) dst[1] = (new_width >> 6) & 0b00111111;
    if(c > 2) dst[2] = (new_width >> 12) & 0b00111111;
    if(c > 3) dst[3] = (new_width >> 18) & 0b00111111;

    return c;
}

static inline uint32_t width_unpack(const uint8_t *src, uint8_t c)
{
    uint32_t new_width = 0;

    if(c > 0)
        new_width |= src[0];
    if(c > 1)
        new_width |= src[1] << 6;
    if(c > 2)
        new_width |= src[2] << 12;
    if(c > 3)
        new_width |= src[3] << 18;

    if(endianness())
        reverse((uint8_t *)&new_width, sizeof(uint32_t));

    return new_width;
}

static inline uint32_t sextet_encode(uint8_t *dst, const uint8_t *src, uint32_t length)
{
    uint32_t i;

    for(i = 0; i < length; i++)
        dst[i] = encoding_table[src[i] & 0b00111111];

    return length;
}

static inline uint32_t sextet_decode(uint8_t *dst, const uint8_t *src, uint32_t length)
{
    uint32_t i;

    for(i = 0; i < length; i++)
    {
        dst[i] = decoding_table[src[i] & 0b01111111];
        if(dst[i] == 0xff)
            return 0;
    }

    return length;
}

static inline uint32_t bytearray_encode(uint8_t *dst, const uint8_t *src, uint32_t length, uint8_t *out_tail)
{
    uint32_t i, j;
    uint8_t tail;

    for(i = j = 0; i < length; i++)
    {
        tail = i % 3;

        if(tail == 0)
        {
            dst[j++] = encoding_table[(src[i] >> 2) & 0b00111111];
        }
        else if(tail == 1)
        {
            dst[j++] = encoding_table[((src[i - 1] & 0b0000011) << 4) | ((src[i] >> 4) & 0b00001111)];
        }
        else
        {
            dst[j++] = encoding_table[((src[i - 1] & 0b00001111) << 2) | ((src[i] >> 6) & 0b00000011)];
            dst[j++] = encoding_table[src[i] & 0b00111111];
        }
    }

    --i;

    if(i % 3 == 0)
    {
        dst[j++] = encoding_table[(src[i] & 0b00000011) << 4];
        *out_tail = 2;
    }
    else if(i % 3 == 1)
    {
        dst[j++] = encoding_table[(src[i] & 0b00001111) << 2];
        *out_tail = 1;
    }
    else
    {
        *out_tail = 0;
    }

    return j;
}

static inline uint32_t bytearray_decode(uint8_t *dst, const uint8_t *src, uint32_t length, uint8_t in_tail)
{
    uint32_t i, j;
    uint8_t tail, byte;

    for(i = j = 0; i < length; i++)
    {
        tail = i % 4;
        byte = decoding_table[src[i] & 0b01111111];

        if(byte == 0xff)
            return 0;

        if(tail == 0)
        {
            dst[j] = byte << 2;
        }
        else if(tail == 1)
        {
            dst[j++] |= (byte >> 4);

            if(i < length - 1 || !in_tail)
                dst[j] = byte << 4;
        }
        else if(tail == 2)
        {
            dst[j++] |= byte >> 2;

            if(i < length - 1 || !in_tail)
                dst[j] = (byte << 6) & 0b11000000;
        }
        else
        {
            dst[j++] |= byte;
        }
    }

    return j;
}

//==============================================================================
// PUBLIC

#ifdef __cplusplus
extern "C" {
#endif

int32_t graph7_encode(uint8_t *dst, const uint8_t *src, uint32_t length, int32_t gtype, uint32_t width)
{
    if(!dst || !src || graph7_order(length, gtype) <= 0)
        return -GRAPH7_INVALID_ARG;

    uint32_t k = length * (width ? width : 1);
    uint32_t hsize = 1;
    uint32_t out_length;
    uint8_t tail;

    graph7_header_t *header = (graph7_header_t *)&dst[0];
    header->byte = 0;
    header->gtype = gtype;

    if(width)
    {
        hsize += 1;
        header->weighed = 1;
        graph7_wheader_t *wheader = (graph7_wheader_t *)&dst[1];
        wheader->byte = 0;

        if(width > 32)
        {
            wheader->extended = 1;
            wheader->width = width_pack(&dst[2], width - 1);

            if(!wheader->width)
                return -GRAPH7_INVALID_ARG;

            hsize += wheader->width;
        }
        else
        {
            wheader->width = width - 1;
        }

        out_length = bytearray_encode(&dst[hsize], &src[0], k, &tail);
    }
    else
    {
        out_length = sextet_pack(&dst[1], &src[0], k, &tail);
        sextet_encode(&dst[1], &dst[1], out_length);
    }

    header->tail = tail;
    sextet_encode(&dst[0], &dst[0], hsize);

    return out_length + hsize;
}

int32_t graph7_decode(uint8_t *dst, const uint8_t *src, uint32_t length, int32_t *gtype, uint32_t *width)
{
    if(!dst || !src || length < 2)
        return -GRAPH7_INVALID_ARG;

    uint32_t out_width = 1;
    uint32_t out_length = 0;
    graph7_header_t header;

    if(!sextet_decode(&header.byte, &src[0], 1))
        return -GRAPH7_INVALID_HEADER;

    if(header.weighed)
    {
        uint32_t hsize = 2;
        graph7_wheader_t wheader;

        if(header.tail > 2)
            return -GRAPH7_INVALID_HEADER;

        if(!sextet_decode(&wheader.byte, &src[1], 1))
            return -GRAPH7_INVALID_HEADER;

        if(wheader.extended)
        {
            uint8_t buff[4];

            if(!wheader.width || wheader.width > 4 || length < hsize + wheader.width + 1)
                return -GRAPH7_INVALID_HEADER;

            if(sextet_decode(buff, &src[2], wheader.width) != wheader.width)
                return -GRAPH7_INVALID_HEADER;

            out_width = width_unpack(buff, wheader.width) + 1;
            hsize += wheader.width;
        }
        else
        {
            out_width = wheader.width + 1;
        }

        if(!(out_length = bytearray_decode(&dst[0], &src[hsize], length - hsize, header.tail)))
            return -GRAPH7_INVALID_DATA;
    }
    else
    {
        if(header.tail > 5)
            return -GRAPH7_INVALID_HEADER;

        const uint8_t *new_src = &src[1];
        uint32_t new_length = length - 1;
        uint32_t c = new_length / BUFFER_SIZE;
        uint32_t t = new_length % BUFFER_SIZE;
        uint8_t buffer[BUFFER_SIZE];

        if(!t)
        {
            c -= 1;
            t = BUFFER_SIZE;
        }

        uint32_t i;

        for(i = 0; i < c; i++)
        {
            if(!sextet_decode(&buffer[0], &new_src[i * BUFFER_SIZE], BUFFER_SIZE))
                return -GRAPH7_INVALID_DATA;

            out_length += sextet_unpack(&dst[i * 6 * BUFFER_SIZE], &buffer[0], BUFFER_SIZE, 0);
        }

        if(!sextet_decode(&buffer[0], &new_src[i * BUFFER_SIZE], t))
            return -GRAPH7_INVALID_DATA;

        out_length += sextet_unpack(&dst[i * 6 * BUFFER_SIZE], &buffer[0], t, header.tail);
    }

    if(gtype)
        *gtype = header.gtype;

    if(width)
        *width = out_width;

    /*
       Дополнительная проверка, основанная на том, что если распоковалось
       неправильное количество байт, то graph7_order вернет значение < 0
    */
    return graph7_order(out_length / out_width, header.gtype) > 0
            ? out_length / out_width : -GRAPH7_INVALID_LENGTH;
}

int32_t graph7_order(uint32_t length, int32_t gtype)
{
    if(!length || gtype < 0 || gtype > GRAPH7_DIRECTED_LOOPS)
        return -GRAPH7_INVALID_ARG;

    uint32_t order;
    uint32_t test;

    /* Решения простых квадратных уравнений для вычисления порядка графа */
    switch(gtype)
    {
    case GRAPH7_UNDIRECTED:
        order = (uint32_t)(sqrt(0.25 + 2. * (double)length) + 0.5);
        test = order * (order - 1) / 2;
        break;
    case GRAPH7_UNDIRECTED_LOOPS:
        order = (uint32_t)(sqrt(0.25 + 2. * (double)length) - 0.5);
        test = order * (order + 1) / 2;
        break;
    case GRAPH7_DIRECTED:
        order = (uint32_t)((sqrt(1. + 4. * (double)length) + 1.) / 2.);
        test = order * (order - 1);
        break;
    case GRAPH7_DIRECTED_LOOPS:
        order = (uint32_t)sqrt((double)length);
        test = order * order;
        break;
    }

    return (test == length) ? order : -GRAPH7_INVALID_LENGTH;
}

int32_t graph7_encoding_length(uint32_t length, uint32_t width)
{
    if(!length)
        return -GRAPH7_INVALID_ARG;

    uint32_t k = length * (width ? width : 1);
    uint32_t out_lenght = 1;

    if(width)
    {
        out_lenght += 1;
        uint32_t new_width = width - 1;

        if(new_width >= 32)
        {
            if(new_width < 64) out_lenght += 1;
            else if(new_width < 4096) out_lenght += 2;
            else if(new_width < 262144) out_lenght += 3;
            else if(new_width < 16777216) out_lenght += 4;
            else return -GRAPH7_INVALID_ARG;
        }

        out_lenght += k * 4 / 3 + (k % 3 ? 1 : 0);
    }
    else
    {
        out_lenght += k / 6;
        out_lenght += (k % 6 != 0 ? 1 : 0);
    }

    return out_lenght;
}

int32_t graph7_metadata(const uint8_t *src, uint32_t length, int32_t *gtype, uint32_t *width)
{
    if(!src || length < 2)
        return -GRAPH7_INVALID_ARG;

    graph7_header_t header;
    uint32_t out_length;
    int32_t _gtype;
    uint32_t _width = 1;

    if(!sextet_decode(&header.byte, &src[0], 1))
        return -GRAPH7_INVALID_HEADER;

    _gtype = (int32_t)header.gtype;

    if(header.weighed)
    {
        uint32_t hsize = 2;
        graph7_wheader_t wheader;

        if(header.tail > 2)
            return -GRAPH7_INVALID_HEADER;

        if(!sextet_decode(&wheader.byte, &src[1], 1))
            return -GRAPH7_INVALID_HEADER;

        _width = wheader.width + 1;

        if(wheader.extended)
        {
            if(wheader.width > 4)
                return -GRAPH7_INVALID_HEADER;

            sextet_decode((uint8_t *)&_width, &src[hsize], wheader.width);
            _width = width_unpack((uint8_t *)&_width, wheader.width) + 1;
            hsize += wheader.width;
        }

        if(length <= hsize)
            return -GRAPH7_INVALID_ARG;

        out_length = 3 * (length - hsize) / 4;
    }
    else
    {
        if(header.tail > 5)
            return -GRAPH7_INVALID_HEADER;

        out_length = (length - 1) * 6 - 6 + header.tail;
        out_length += (header.tail) ? 0 : 6;
    }

    if(gtype)
        *gtype = _gtype;

    if(width)
        *width = _width;

    return out_length;
}

int32_t graph7_encode_from_matrix(uint8_t *dst, const uint8_t *src, uint32_t order, int32_t gtype, uint32_t width)
{
    if(!dst || !src || order < 2)
        return -GRAPH7_INVALID_ARG;

    uint32_t length;
    uint32_t _width = (width) ? width : 1;

    switch(gtype)
    {
    case GRAPH7_UNDIRECTED:
        length = order * (order - 1) / 2;
        break;
    case GRAPH7_UNDIRECTED_LOOPS:
        length = order * (order + 1) / 2;
        break;
    case GRAPH7_DIRECTED:
        length = order * (order - 1);
        break;
    case GRAPH7_DIRECTED_LOOPS:
        length = order * order;
        break;
    }

    uint8_t *bytearray = (uint8_t *)malloc(length * _width);

    if(!bytearray)
        return -GRAPH7_ALLOC_ERROR;

    if(gtype == GRAPH7_UNDIRECTED)
    {
        for(uint32_t i = 0, c = 0; i < order - 1; i++)
        {
            for(uint32_t j = i + 1; j < order; j++, c++)
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
        }
    }
    else if(gtype == GRAPH7_UNDIRECTED_LOOPS)
    {
        for(uint32_t i = 0, c = 0; i < order; i++)
        {
            for(uint32_t j = i; j < order; j++, c++)
            {
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
            }
        }
    }
    else if(gtype == GRAPH7_DIRECTED)
    {
        for(uint32_t i = 0, c = 0; i < order; i++)
        {
            for(uint32_t j = 0; j < order; j++)
            {
                if(i == j) continue;
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
                ++c;
            }
        }
    }
    else // GRAPH7_DIRECTED_LOOPS
    {
        for(uint32_t i = 0, c = 0; i < order; i++)
        {
            for(uint32_t j = 0; j < order; j++, c++)
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
        }
    }

    if(width > 1 && endianness())
    {
        /*
            Если порядок байт big-endian, то переворачиваем, так как сохраняется
            всегда в little-endian.
        */
        for(uint32_t i = 0; i < length; i++)
            reverse(&bytearray[i * _width], _width);
    }

    int32_t ret = graph7_encode(dst, bytearray, length, gtype, width);

    free(bytearray);

    return ret;
}

int32_t graph7_decode_to_matrix(uint8_t *dst, const uint8_t *src, uint32_t length)
{
    if(!dst)
        return -GRAPH7_INVALID_ARG;

    int32_t gtype;
    uint32_t width;
    int32_t decoding_length = graph7_metadata(src, length, &gtype, &width);

    if(decoding_length < 0)
        return decoding_length;

    uint8_t *bytearray = (uint8_t *)malloc(decoding_length);

    if(!bytearray)
        return -GRAPH7_ALLOC_ERROR;

    int32_t ret = graph7_decode(bytearray, src, length, NULL, NULL);

    if(ret * width != decoding_length)
        goto _exit;

    uint32_t order = graph7_order(ret, gtype);

    if(width > 1 && endianness())
    {
        /*
            Если порядок байт big-endian, то переворачиваем, так как сохраняется
            всегда в little-endian.
        */
        for(uint32_t i = 0; i < decoding_length / width; i++)
            reverse(&bytearray[i * width], width);
    }

    if(gtype == GRAPH7_UNDIRECTED)
    {
        for(uint32_t i = 0, c = 0; i < order - 1; i++)
        {
            for(uint32_t j = i + 1; j < order; j++, c++)
            {
                memmove(&dst[i * order * width + j * width], &bytearray[c * width], width);
                memmove(&dst[j * order * width + i * width], &bytearray[c * width], width);
            }
        }
    }
    else if(gtype == GRAPH7_UNDIRECTED_LOOPS)
    {
        for(uint32_t i = 0, c = 0; i < order; i++)
        {
            for(uint32_t j = i; j < order; j++, c++)
            {
                memmove(&dst[i * order * width + j * width], &bytearray[c * width], width);
                if(i != j)
                    memmove(&dst[j * order * width + i * width], &bytearray[c * width], width);
            }
        }
    }
    else if(gtype == GRAPH7_DIRECTED)
    {
        for(uint32_t i = 0, c = 0; i < order; i++)
        {
            for(uint32_t j = 0; j < order; j++)
            {
                if(i == j) continue;
                memmove(&dst[i * order * width + j * width], &bytearray[c * width], width);
                ++c;
            }
        }
    }
    else // GRAPH7_DIRECTED_LOOPS
    {
        for(uint32_t i = 0, c = 0; i < order; i++)
        {
            for(uint32_t j = 0; j < order; j++, c++)
                memmove(&dst[i * order * width + j * width], &bytearray[c * width], width);
        }
    }

    if(gtype == GRAPH7_UNDIRECTED || gtype == GRAPH7_DIRECTED)
    {
        for(uint32_t i = 0; i < order; i++)
            memset(&dst[i * order * width + i * width], 0, width);
    }

_exit:
    free(bytearray);
    return ret;
}

#ifdef __cplusplus
}
#endif
