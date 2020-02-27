#ifndef GRAPH7_bitstream_H
#define GRAPH7_bitstream_H

#include <graph7/def.h>

typedef struct bitstream
{
    uint8_t *memory;    //!< Pointer to memory where we will write
    uint8_t group_size; //!< Number from 1 to 8
    uint8_t bitp;       //!< Pointer to current bit where we will write
    size_t bytep;       //!< Pointer to current byte where we will write
} bitstream_t;

void bitstream_init(bitstream_t *self, uint8_t *memory, uint8_t group_size);

void bitstream_deinit(bitstream_t *self);

void bitstream_write(bitstream_t *self, bool value);

void bitstream_write_bytes(bitstream_t *self, uint8_t *src, size_t length);

bool bitstream_read(bitstream_t *self);

size_t bitstream_bytes_count(bitstream_t *self);

void bitstream_encode_number(bitstream_t *stream, size_t x, size_t k);

size_t bitstream_decode_number(bitstream_t *stream, size_t k);

#endif // GRAPH7_bitstream_H
