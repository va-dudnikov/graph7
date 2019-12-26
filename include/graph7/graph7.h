/*!
 * \file
 */

#ifndef GRAPH7_H
#define GRAPH7_H

#include <graph7/def.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Encoding adjacency matrix to graph7
 * \param dst[out] Destination
 * \param src[in] Source
 * \param length[in] Number of cells in adjacency matrix
 * \param gtype[in] Type of graph (see Types of graphs)
 * \param width[in] Number of bytes in cell (if need encoding as 1 byte = 1 bit, width must be set 0)
 * \return Number of characters if ok, < 0 otherwise
 */
ssize_t graph7_encode(uint8_t *dst, const uint8_t *src, size_t length, int32_t gtype, size_t width);

/*!
 * \brief Decoding graph7 to adjacency matrix
 * \param dst[out] Destination
 * \param src[in] Source
 * \param length[in] Number of characters
 * \param gtype[out] Pointer to variable, where will be stored type of graph
 * \param width[out] Pointer to variable, where will be stored width of cells
 * \return Number of cells in adjacency matrix if ok, < 0 otherwise
 */
ssize_t graph7_decode(uint8_t *dst, const uint8_t *src, size_t length, int32_t *gtype, size_t *width);

/*!
 * \brief Convert number of cells in adjacency matrix to order of graph
 * \param length[in] Number of cells in adjacency matrix
 * \param gtype[in] Type of graph
 * \return Order of graph if ok, < 0 otherwise
 */
ssize_t graph7_order(size_t length, int32_t gtype);

/*!
 * \brief Get number of bytes for encoding
 * \param length[in] Number of cells in adjacency matrix
 * \param width[in] Number of bytes in cell
 * \return Number of bytes if ok, < 0 otherwise
 */
ssize_t graph7_encoding_length(size_t length, size_t width);

/*!
 * \brief Get metadata
 * \param src[in] Source
 * \param length[in] Number of characters
 * \param gtype[out] Type of graph
 * \param width[out] Number of bytes in cell
 * \return Decoding length if ok, < 0 otherwise
 */
ssize_t graph7_metadata(const uint8_t *src, size_t length, int32_t *gtype, size_t *width);

/*!
 * \brief Encoding graph7 from matrix
 * \param dst[out] Destination
 * \param src[in] Source (matrix order * order in 1D array)
 * \param order[in] Order of graph
 * \param gtype[in] Type of graph
 * \param width[in] Number of bytes in cell
 * \return The same as graph7_encode
 */
ssize_t graph7_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, int32_t gtype, size_t width);

/*!
 * \brief Decoding graph7 to matrix
 * \param dst[out] Destination (matrix order * order in 1D array)
 * \param src[in] Source
 * \param length[in] Number of characters
 * \return the same as graph7_decode
 */
ssize_t graph7_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length);

#ifdef __cplusplus
}
#endif

#endif // GRAPH7_H
