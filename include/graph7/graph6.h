/*!
 * \file
 * \brief The file contains definition of API functions for working with graph6, sparse6 and digraph6 encoding formats.
 */

#ifndef GRAPH6_H
#define GRAPH6_H

#include <graph7/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

ssize_t graph6_order_encode(uint8_t *dst, size_t order);

ssize_t graph6_order_decode(size_t *dst, const uint8_t *src, size_t length);

ssize_t graph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, bool header);

ssize_t graph6_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length);

ssize_t digraph6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, bool header);

ssize_t digraph6_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length);

ssize_t sparse6_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, bool header);

ssize_t sparse6_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length);

#ifdef __cplusplus
}
#endif

#endif // GRAPH6_H
