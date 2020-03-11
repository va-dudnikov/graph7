#ifndef GRAPH7_UTILS_H
#define GRAPH7_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <graph7/def.h>

uint8_t graph7_utils_endianness(void);

void graph7_utils_reverse(uint8_t *src, size_t length);

size_t graph7_utils_ceiling_div(size_t numerator, size_t denominator);

size_t graph7_utils_count_bits(size_t x);

bool graph7_utils_bytes_start_with(const uint8_t *src, const uint8_t *start, size_t start_length);

#endif // GRAPH7_UTILS_H

#ifdef __cplusplus
}
#endif
