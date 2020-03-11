#include <graph7/bitstream.h>

void bitstream_init(bitstream_t *self, uint8_t *memory)
{
    self->memory = memory;
    self->bitp = 0;
    self->bytep = 0;
}

void bitstream_deinit(bitstream_t *self)
{
    self->bitp = 0;
    self->bytep = 0;
}

void bitstream_write(bitstream_t *self, bool value)
{
    // Reset currrent byte, but only when we need to write
    if(self->bitp == 0)
        self->memory[self->bytep] = 0;

    self->memory[self->bytep] |= !!value << (5 - self->bitp);

    if(self->bitp + 1 == 6)
    {
        ++self->bytep;
        self->bitp = 0;
    }
    else
    {
        ++self->bitp;
    }
}

bool bitstream_read(bitstream_t *self)
{
    bool value =  self->memory[self->bytep] >> (5 - self->bitp) & 1;

    if(self->bitp + 1 == 6)
    {
        ++self->bytep;
        self->bitp = 0;
    }
    else
    {
        ++self->bitp;
    }

    return value;
}

size_t bitstream_bytes_count(bitstream_t *self)
{
    return self->bitp ? self->bytep + 1 : self->bytep;
}

void bitstream_encode_number(bitstream_t *stream, size_t x, size_t k)
{
    for(size_t i = 0; i < k; i++)
        bitstream_write(stream, !!(x & 1 << (k - 1 - i)));
}

size_t bitstream_decode_number(bitstream_t *stream, size_t k)
{
    size_t value = 0;
    for(size_t i = 0; i < k; i++)
        value |= (1 & bitstream_read(stream)) << (k - 1 - i);

    return value;
}
