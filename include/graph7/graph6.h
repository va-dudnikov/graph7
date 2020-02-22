#ifndef GRAPH7_GRAPH6_H
#define GRAPH7_GRAPH6_H

#include <graph7/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

ssize_t graph7_graph6_order_encode(uint8_t *dst, size_t order);

ssize_t graph7_graph6_order_decode(size_t *dst, const uint8_t *src);

ssize_t graph7_graph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order);

ssize_t graph7_graph6_decode_to_matrix(uint8_t *dst, const uint8_t *src);

ssize_t graph7_digraph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order);

ssize_t graph7_digraph6_decode_to_matrix(uint8_t *dst, const uint8_t *src);

ssize_t graph7_sparse6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order);

ssize_t graph7_sparse6_decode_to_matrix(uint8_t *dst, const uint8_t *src);

#ifdef __cplusplus
}
#endif

#endif // GRAPH7_GRAPH6_H
