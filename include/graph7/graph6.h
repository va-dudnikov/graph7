#ifndef GRAPH7_GRAPH6_H
#define GRAPH7_GRAPH6_H

#include <graph7/utils.h>

ssize_t graph7_graph6_size_encode(uint8_t *dst, size_t size);

ssize_t graph7_graph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order);

ssize_t graph7_graph6_decode_to_matrix(uint8_t *dst, const uint8_t *src);

#endif // GRAPH7_GRAPH6_H
