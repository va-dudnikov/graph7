#include <graph7/graph7.h>
#include <graph7/utils.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//==============================================================================
// PRIVATE

#define BITMASK_1 0x01
#define BITMASK_2 0x03
#define BITMASK_3 0x07
#define BITMASK_4 0x0F
#define BITMASK_5 0x1F
#define BITMASK_6 0x3F
#define BITMASK_7 0x7F

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

static inline uint8_t width_pack(uint8_t *dst, size_t width)
{
    GRAPH7_ERROR(width >= 1 << 4 * 6, 0);

    size_t new_width = width;
    uint8_t c = 0;

    for(uint8_t i = 1; i < 5; i++)
    {
        if(new_width < (1 << i * 6))
        {
            c = i;
            break;
        }
    }

    if(graph7_utils_endianness())
        graph7_utils_reverse((uint8_t *)&new_width, sizeof(size_t));

    for(uint8_t i = 0; i < c; i++)
        dst[i] = (new_width >> i * 6) & BITMASK_6;

    return c;
}

static inline size_t width_unpack(const uint8_t *src, uint8_t c)
{
    size_t new_width = 0;

    for(uint8_t i = 0; i < c; i++)
        new_width |= (size_t)(src[i] << i * 6);

    if(graph7_utils_endianness() == GRAPH7_BIG_ENDIAN)
        graph7_utils_reverse((uint8_t *)&new_width, sizeof(size_t));

    return new_width;
}

static inline size_t bytearray_encode(uint8_t *dst, const uint8_t *src, size_t length)
{
    size_t i;

    for(i = 0; i < length; i++)
        dst[i] = encoding_table[src[i] & BITMASK_6];

    return length;
}

static inline size_t bytearray_decode(uint8_t *dst, const uint8_t *src, size_t length)
{
    size_t i;

    for(i = 0; i < length; i++)
    {
        dst[i] = decoding_table[src[i] & BITMASK_7]; // Decoding table have only 128 cells
        // Wrong symbol
        if(dst[i] == 0xff)
            return 0;
    }

    return length;
}

static inline size_t bytearray_pack(uint8_t *dst, const uint8_t *src, size_t length)
{
    size_t i, j;
    uint8_t tail;

    for(i = j = 0; i < length; i++)
    {
        tail = i % 3;

        if(tail == 0)
        {
            dst[j++] = (src[i] >> 2) & BITMASK_6;
        }
        else if(tail == 1)
        {
            dst[j++] = ((src[i - 1] & BITMASK_2) << 4) | ((src[i] >> 4) & BITMASK_4);
        }
        else
        {
            dst[j++] = ((src[i - 1] & BITMASK_4) << 2) | ((src[i] >> 6) & BITMASK_2);
            dst[j++] = src[i] & BITMASK_6;
        }
    }

    --i;

    if(i % 3 == 0) // tail = 2
        dst[j++] = (src[i] & BITMASK_2) << 4;
    else if(i % 3 == 1) // tail = 1
        dst[j++] = (src[i] & BITMASK_4) << 2;
    // else tail = 0

    return j;
}

static inline size_t bytearray_unpack(uint8_t *dst, const uint8_t *src, size_t length, uint8_t in_tail)
{
    size_t i, j;
    uint8_t tail, byte;

    for(i = j = 0; i < length; i++)
    {
        tail = i % 4;
        byte = src[i] & BITMASK_7;

        if(byte == 0xff)
            return 0;

        if(tail == 0)
        {
            dst[j] = (uint8_t)(byte << 2);
        }
        else if(tail == 1)
        {
            dst[j++] |= (byte >> 4);

            if(i < length - 1 || !in_tail)
                dst[j] = (uint8_t)(byte << 4);
        }
        else if(tail == 2)
        {
            dst[j++] |= byte >> 2;

            if(i < length - 1 || !in_tail)
                dst[j] = (byte << 6) & 0xc0;
        }
        else
        {
            dst[j++] |= byte;
        }
    }

    return j;
}

static inline size_t sextet_pack(uint8_t *dst, const uint8_t *src, size_t length)
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

    return (t ? c + 1 : c);
}

static inline size_t sextet_unpack(uint8_t *dst, const uint8_t *src, size_t length, uint8_t tail)
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

//==============================================================================
// PUBLIC

#ifdef __cplusplus
extern "C" {
#endif

ssize_t graph7_encode_header(uint8_t *dst, size_t order, graph7_gtype_t gtype, size_t width)
{
    GRAPH7_ERROR(!dst || !order, -GRAPH7_INVALID_ARG);

    size_t hsize = 1;
    graph7_header_t *header = (graph7_header_t *)&dst[0];
    dst[0] = 0; // Clear header
    header->gtype = gtype;

    size_t ncells = graph7_ncells(order, gtype);

    if(width)
    {
        hsize += 1;
        header->weighed = 1;
        graph7_wheader_t *wheader = (graph7_wheader_t *)&dst[1];
        dst[1] = 0; // Clear header

        if(width > 32)
        {
            wheader->extended = 1;
            wheader->width = width_pack(&dst[2], width - 1);
            GRAPH7_ERROR(!wheader->width, -GRAPH7_UNSUPPORTED);

            hsize += wheader->width;
        }
        else
        {
            wheader->width = (uint8_t)(width - 1);
        }

        if(ncells)
            header->tail = 2 - ((ncells * width - 1) % 3);
    }
    else
    {
        header->tail = ncells % 6;
    }

    bytearray_encode(dst, dst, hsize);

    return hsize;
}

ssize_t graph7_decode_header(const uint8_t *src, size_t length, graph7_gtype_t *gtype, size_t *width, uint8_t *tail)
{
    GRAPH7_ERROR(!src, -GRAPH7_INVALID_ARG);
    GRAPH7_ERROR(!length, -GRAPH7_INVALID_LENGTH);

    graph7_header_t header;

    GRAPH7_ERROR(
        !bytearray_decode((uint8_t *)&header, src, 1),
        -GRAPH7_INVALID_HEADER
    );

    size_t hsize = 1;
    size_t _width;

    if(header.weighed)
    {
        GRAPH7_ERROR(header.tail > 2, -GRAPH7_INVALID_HEADER);
        hsize += 1;
        graph7_wheader_t wheader;

        GRAPH7_ERROR(length < hsize, -GRAPH7_INVALID_LENGTH);
        GRAPH7_ERROR(
            !bytearray_decode((uint8_t *)&wheader, &src[1], 1),
            -GRAPH7_INVALID_HEADER
        );

        _width = wheader.width + 1;

        if(wheader.extended)
        {
            GRAPH7_ERROR(wheader.width > 4, -GRAPH7_UNSUPPORTED);

            if(length < hsize)
                return -GRAPH7_INVALID_LENGTH;

            GRAPH7_ERROR(
                !bytearray_decode((uint8_t *)&_width, &src[hsize], wheader.width),
                -GRAPH7_INVALID_HEADER
            );

            hsize += wheader.width;

            _width = width_unpack((uint8_t *)&_width, wheader.width) + 1;
        }
    }
    else
    {
        GRAPH7_ERROR(header.tail > 5, -GRAPH7_INVALID_HEADER);
        _width = 0;
    }

    if(width)
        *width = _width;

    if(gtype)
        *gtype = (graph7_gtype_t)header.gtype;

    if(tail)
        *tail = (uint8_t)header.tail;

    return hsize;
}

ssize_t graph7_encode(uint8_t *dst, const uint8_t *src, size_t ncells, graph7_gtype_t gtype, size_t width)
{
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    ssize_t order = graph7_order(ncells, gtype);

    GRAPH7_ERROR(order < 0, order);

    ssize_t hsize = graph7_encode_header(dst, order, gtype, width);

    GRAPH7_ERROR(hsize < 0, hsize);

    size_t k = ncells * (width ? width : 1);
    size_t nbytes;

    if(width)
        nbytes = bytearray_pack(&dst[hsize], &src[0], k);
    else
        nbytes = sextet_pack(&dst[1], &src[0], k);


    bytearray_encode(&dst[hsize], &dst[hsize], nbytes);

    return (ssize_t)(nbytes + hsize);
}

ssize_t graph7_decode(uint8_t *dst, const uint8_t *src, size_t length, graph7_gtype_t *gtype, size_t *width)
{
    size_t _width;
    graph7_gtype_t _gtype;
    uint8_t _tail;

    ssize_t hsize = graph7_decode_header(src, length, &_gtype, &_width, &_tail);

    GRAPH7_ERROR(hsize < 0, hsize);

    uint8_t *bytearray = (uint8_t *)malloc(length - hsize);

    GRAPH7_ERROR(!bytearray, -GRAPH7_ALLOC_ERROR);

    bytearray_decode(bytearray, &src[hsize], length - hsize);

    size_t data_size;
    if(_width)
        data_size = bytearray_unpack(dst, bytearray, length - hsize, _tail);
    else
        data_size = sextet_unpack(dst, bytearray, length - hsize, _tail);

    free(bytearray);

    if(gtype)
        *gtype = _gtype;

    if(width)
        *width = _width;

    size_t ncells = data_size / (_width ? _width : 1);
    return graph7_order(ncells, _gtype) > 0 ? ncells : -GRAPH7_INVALID_ARG;
}

size_t graph7_ncells(size_t order, graph7_gtype_t gtype)
{
    GRAPH7_ERROR(!order, 0);

    size_t ncells;
    switch(gtype)
    {
    case GRAPH7_UNDIRECTED:
        ncells = order * (order - 1) / 2;
        break;
    case GRAPH7_UNDIRECTED_LOOPS:
        ncells = order * (order + 1) / 2;
        break;
    case GRAPH7_DIRECTED:
        ncells = order * (order - 1);
        break;
    default:
        ncells = order * order;
        break;
    }

    return ncells;
}

ssize_t graph7_order(size_t ncells, graph7_gtype_t gtype)
{
    GRAPH7_ERROR(
        gtype > GRAPH7_DIRECTED_LOOPS,
        -GRAPH7_INVALID_ARG
    );

    size_t order = 0;
    size_t test = 0;

    /* Решения простых квадратных уравнений для вычисления порядка графа */
    switch(gtype)
    {
    case GRAPH7_UNDIRECTED:
        order = (size_t)(sqrt(0.25 + 2. * (double)ncells) + 0.5);
        test = order * (order - 1) / 2;
        break;
    case GRAPH7_UNDIRECTED_LOOPS:
        order = (size_t)(sqrt(0.25 + 2. * (double)ncells) - 0.5);
        test = order * (order + 1) / 2;
        break;
    case GRAPH7_DIRECTED:
        order = (size_t)((sqrt(1. + 4. * (double)ncells) + 1.) / 2.);
        test = order * (order - 1);
        break;
    case GRAPH7_DIRECTED_LOOPS:
        order = (size_t)(sqrt((double)ncells));
        test = order * order;
        break;
    }

    return (test == ncells) ? (ssize_t)order : -GRAPH7_INVALID_LENGTH;
}

ssize_t graph7_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, graph7_gtype_t gtype, size_t width)
{
    GRAPH7_ERROR(!dst || !src, -GRAPH7_INVALID_ARG);

    size_t ncells = graph7_ncells(order, gtype);
    size_t _width = (width) ? width : 1;

    uint8_t *bytearray = (uint8_t *)malloc(ncells * _width);

    GRAPH7_ERROR(!bytearray, -GRAPH7_ALLOC_ERROR);

    if(gtype == GRAPH7_UNDIRECTED)
    {
        for(size_t i = 0, c = 0; i < order - 1; i++)
        {
            for(size_t j = i + 1; j < order; j++, c++)
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
        }
    }
    else if(gtype == GRAPH7_UNDIRECTED_LOOPS)
    {
        for(size_t i = 0, c = 0; i < order; i++)
        {
            for(size_t j = i; j < order; j++, c++)
            {
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
            }
        }
    }
    else if(gtype == GRAPH7_DIRECTED)
    {
        for(size_t i = 0, c = 0; i < order; i++)
        {
            for(size_t j = 0; j < order; j++)
            {
                if(i == j) continue;
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
                ++c;
            }
        }
    }
    else // GRAPH7_DIRECTED_LOOPS
    {
        for(size_t i = 0, c = 0; i < order; i++)
        {
            for(size_t j = 0; j < order; j++, c++)
                memmove(&bytearray[c * _width], &src[i * order * _width + j * _width], _width);
        }
    }

    // This option need when we save only standard data types
    // as width-data (int, float etc). But if we want to save other
    // types (img, text etc), then you need keep track of in
    // what order the data.
    // In principle, if the option is enabled, then any data can be
    // stored, they will be correctly decoded, but extra work
    // will be done. It would be possible to shift the obligation on
    // the data format to the user, but I need this option, what to
    // enable it for the python module.
    #ifdef GRAPH7_LITTLE_ENDIAN_WIDTH_DATA
    if(width > 1 && graph7_utils_endianness() == GRAPH7_BIG_ENDIAN)
    {
        // If the byte order is big-endian, then we reverse data over,
        // since it is always stored in little-endian.
        for(size_t i = 0; i < ncells; i++)
            graph7_utils_reverse(&bytearray[i * _width], _width);
    }
    #endif

    ssize_t ret = graph7_encode(dst, bytearray, ncells, gtype, width);

    free(bytearray);

    return ret;
}

ssize_t graph7_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length)
{
    GRAPH7_ERROR(!dst, -GRAPH7_INVALID_ARG);

    size_t width;
    graph7_gtype_t gtype;
    uint8_t tail;
    size_t ncells;

    ssize_t hsize = graph7_decode_header(src, length, &gtype, &width, &tail);

    GRAPH7_ERROR(hsize < 0, hsize);

    // (6 * number of bytes) / 8 = (3 * number of bytes) / 4.
    if(width)
        ncells = 3 * (length - hsize) / 4 / width;
    else
        ncells = ((length - hsize) * 6 - 6 + tail) + (tail ? 0 : 6);

    size_t _width = (width) ? width : 1;
    size_t data_size = ncells * _width;

    uint8_t *bytearray = (uint8_t *)malloc(data_size);

    GRAPH7_ERROR(!bytearray, -GRAPH7_ALLOC_ERROR);

    size_t order = 0;
    ssize_t check = graph7_decode(bytearray, src, length, NULL, NULL);

    if(check < 0)
        goto _exit;

    if((size_t)check * _width != data_size)
    {
        check = -GRAPH7_INVALID_ARG;
        goto _exit;
    }

    // We do not check the value of the variable,
    // since the value will always be> 0 at this stage.
    order = (size_t)graph7_order((size_t)check, gtype);


    // This option need when we save only standard data types
    // as width-data (int, float etc). But if we want to save other
    // types (img, text etc), then you need keep track of in
    // what order the data.
    // In principle, if the option is enabled, then any data can be
    // stored, they will be correctly decoded, but extra work
    // will be done. It would be possible to shift the obligation on
    // the data format to the user, but I need this option, what to
    // enable it for the python module.
    #ifdef GRAPH7_LITTLE_ENDIAN_WIDTH_DATA
    if(width > 1 && graph7_utils_endianness() == GRAPH7_BIG_ENDIAN)
    {
        // If the byte order is big-endian, then we reverse data over,
        // since it is always stored in little-endian.
        for(size_t i = 0; i < data_size / _width; i++)
            graph7_utils_reverse(&bytearray[i * _width], _width);
    }
    #endif

    if(gtype == GRAPH7_UNDIRECTED)
    {
        for(size_t i = 0, c = 0; i < order - 1; i++)
        {
            for(size_t j = i + 1; j < order; j++, c++)
            {

                memmove(&dst[GRAPH7_WMIDX(i, j, order, _width)], &bytearray[c * _width], _width);
                memmove(&dst[GRAPH7_WMIDX(j, i, order, _width)], &bytearray[c * _width], _width);
            }
        }
    }
    else if(gtype == GRAPH7_UNDIRECTED_LOOPS)
    {
        for(size_t i = 0, c = 0; i < order; i++)
        {
            for(size_t j = i; j < order; j++, c++)
            {
                memmove(&dst[GRAPH7_WMIDX(i, j, order, _width)], &bytearray[c * _width], _width);
                if(i != j)
                    memmove(&dst[GRAPH7_WMIDX(j, i, order, _width)], &bytearray[c * _width], _width);
            }
        }
    }
    else if(gtype == GRAPH7_DIRECTED)
    {
        for(size_t i = 0, c = 0; i < order; i++)
        {
            for(size_t j = 0; j < order; j++)
            {
                if(i == j) continue;
                memmove(&dst[GRAPH7_WMIDX(i, j, order, _width)], &bytearray[c * _width], _width);
                ++c;
            }
        }
    }
    else // GRAPH7_DIRECTED_LOOPS
    {
        for(size_t i = 0, c = 0; i < order; i++)
        {
            for(size_t j = 0; j < order; j++, c++)
                memmove(&dst[GRAPH7_WMIDX(i, j, order, _width)], &bytearray[c * _width], _width);
        }
    }

    if(gtype == GRAPH7_UNDIRECTED || gtype == GRAPH7_DIRECTED)
    {
        for(size_t i = 0; i < order; i++)
            memset(&dst[GRAPH7_WMIDX(i, i, order, _width)], 0, _width);
    }

_exit:
    free(bytearray);
    return check > 0 ? (ssize_t)order : check;
}

#ifdef __cplusplus
}
#endif
