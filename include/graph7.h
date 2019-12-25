#ifndef GRAPH7_H
#define GRAPH7_H

#include "def.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
    Encoding adjacency matrix to graph7
    dst[out]: destination
    src[in]: source
    length[in]: number of cells in adjacency matrix
    gtype[in]: type of graph (see types of graphs)
    width[in]: number of bytes in cell (if need encoding as 1 byte = 1 bit, width must be set 0)

    return: number of characters if ok, < 0 otherwise
*/
int32_t graph7_encode(uint8_t *dst, const uint8_t *src, uint32_t length, int32_t gtype, uint32_t width);

/*
    Decoding graph7 to adjacency matrix
    dst[out]: destination
    src[in]: source
    length[in]: number of characters
    gtype[out]: pointer to variable, where will be stored type of graph
    width[out]: pointer to variable, where will be stored width of cells

    return: number of cells in adjacency matrix if ok, < 0 otherwise
*/
int32_t graph7_decode(uint8_t *dst, const uint8_t *src, uint32_t length, int32_t *gtype, uint32_t *width);

/*
    Convert number of cells in adjacency matrix to order of graph
    length[in]: number of cells in adjacency matrix
    gtype[in]: type of graph

    return: order of graph if ok, < 0 otherwise
*/
int32_t graph7_order(uint32_t length, int32_t gtype);

/*
    Get number of bytes for encoding
    length[in]: number of cells in adjacency matrix
    width[in]: number of bytes in cell

    return: number of bytes if ok, < 0 otherwise
*/
int32_t graph7_encoding_length(uint32_t length, uint32_t width);

/*
    Get metadata
    src[in]: source
    length[in]: number of characters
    gtype[out]: type of graph
    width[out]: number of bytes in cell

    return: decoding length if ok, < 0 otherwise
*/
int32_t graph7_metadata(const uint8_t *src, uint32_t length, int32_t *gtype, uint32_t *width);

/*
    Encoding graph7 from matrix
    dst[out]: destination
    src[in]: source (matrix order * order in 1D array)
    order[in]: order of graph
    gtype[in]: type of graph
    width[in]: number of bytes in cell

    return: the same as graph7_encode
*/
int32_t graph7_encode_from_matrix(uint8_t *dst, const uint8_t *src, uint32_t order, int32_t gtype, uint32_t width);

/*
    Decoding graph7 to matrix
    dst[out]: destination (matrix order * order in 1D array)
    src[in]: source
    length[in]: number of characters

    return: the same as graph7_decode
*/
int32_t graph7_decode_to_matrix(uint8_t *dst, const uint8_t *src, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif // GRAPH7_H
