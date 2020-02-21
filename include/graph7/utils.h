#ifndef GRAPH7_UTILS_H
#define GRAPH7_UTILS_H

#include <graph7/def.h>

uint8_t graph7_utils_endianness(void);

void graph7_utils_reverse(uint8_t *src, size_t length);

size_t graph7_utils_sextet_pack(uint8_t *dst, const uint8_t *src, size_t length, uint8_t *tail);

size_t graph7_utils_sextet_unpack(uint8_t *dst, const uint8_t *src, size_t length, uint8_t tail);

size_t graph7_utils_ceiling_div(size_t numerator, size_t denominator);

#endif // GRAPH7_UTILS_H
