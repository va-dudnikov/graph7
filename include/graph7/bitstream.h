#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <graph7/def.h>

typedef struct bitstream
{
    uint8_t *memory;    //!< Pointer to memory where we will write
    uint8_t bitp;       //!< Pointer to current bit where we will write
    size_t bytep;       //!< Pointer to current byte where we will write
} bitstream_t;

void bitstream_init(bitstream_t *self, uint8_t *memory);

void bitstream_deinit(bitstream_t *self);

void bitstream_write(bitstream_t *self, bool value);

bool bitstream_read(bitstream_t *self);

size_t bitstream_bytes_count(bitstream_t *self);

void bitstream_encode_number(bitstream_t *stream, size_t x, size_t k);

size_t bitstream_decode_number(bitstream_t *stream, size_t k);

#endif // BITSTREAM_H
