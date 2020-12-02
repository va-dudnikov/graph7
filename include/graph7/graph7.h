/*
 * The file contains definition of API functions for working with graph7 encoding format.
 */

#ifndef _GRAPH7_H
#define _GRAPH7_H

#ifdef __cplusplus
extern "C" {
#endif

#include <graph7/defs.h>

/*!
 * \brief The function for encoding graph7 header.
 * \param[out] dst A buffer to write encoding result.
 * \param[in] order Order of graph.
 * \param[in] gtype Type of graph.
 * \param[in] width Size of data type.
 * \return Number of characters if ok, < 0 otherwise.
 */
ssize_t graph7_encode_header(uint8_t *dst, size_t order, graph7_gtype_t gtype, size_t width);

/*!
 * \brief The function for decoding graph7 header.
 * \param[in] src A string containing an encoded graph7 header.
 * \param[in] length The string length. The length does not have to indicate the exact size,
 *  but the length should not be less than the size of the header.
 * \param[out] gtype Type of graph.
 * \param[out] width Size of data type.
 * \param[out] tail Specific information related to the graph7 format (see the format description).
 * \return Size of header if ok, < 0 otherwise.
 */
ssize_t graph7_decode_header(const uint8_t *src, size_t length, graph7_gtype_t *gtype, size_t *width, uint8_t *tail);

/*!
 * \brief
 * \param[out] dst A buffer to write encoding result.
 * \param[in] src Cells of adjacency matrix (only payload data).
 * \param[in] length Number of cells in adjacency matrix.
 * \param[in] gtype Type of graph.
 * \param[in] width Size of data type.
 * \return Number of characters if ok, < 0 otherwise.
 */
ssize_t graph7_encode(uint8_t *dst, const uint8_t *src, size_t ncells, graph7_gtype_t gtype, size_t width);

/*!
 * \brief Decoding graph7 to cells of adjacency matrix (only payload data).
 * \param[out] dst A buffer to write decoding result.
 * \param[in] src A string containing an encoded graph7 adjacency matrix.
 * \param[in] length Number of characters.
 * \param[out] gtype Type of graph.
 * \param[out] width Size of data type.
 * \return Number of cells in adjacency matrix if ok, < 0 otherwise
 */
ssize_t graph7_decode(uint8_t *dst, const uint8_t *src, size_t length, graph7_gtype_t *gtype, size_t *width);

/*!
 * \brief Calculate a number of cells with payload data in adjacency matrix.
 * \param[in] order Order of graph.
 * \param[in] gtype Type of graph.
 * \return Number of cells.
 */
size_t graph7_ncells(size_t order, graph7_gtype_t gtype);

/*!
 * \brief Convert number of cells in adjacency matrix to order of graph.
 * \param[in] ncells Number of cells in adjacency matrix.
 * \param[in] gtype Type of graph.
 * \return Order of graph if ok, < 0 otherwise.
 */
ssize_t graph7_order(size_t ncells, graph7_gtype_t gtype);

/*!
 * \brief Encoding adjacency matrix to graph7.
 * \param[out] dst Destination
 * \param[in] src Source (matrix order * order in 1D array)
 * \param[in] order Order of graph
 * \param[in] gtype Type of graph
 * \param[in] width Number of bytes in cell
 * \return The same as graph7_encode
 */
ssize_t graph7_encode_from_matrix(uint8_t *dst, const uint8_t *src, size_t order, graph7_gtype_t gtype, size_t width);

/*!
 * \brief Decoding graph7 to adjacency matrix.
 * \param[out] dst Destination (matrix order * order in 1D array)
 * \param[in] src Source
 * \param[in] length Number of characters
 * \return order of graph
 */
ssize_t graph7_decode_to_matrix(uint8_t *dst, const uint8_t *src, size_t length);

#ifdef __cplusplus
}
#endif

#endif // _GRAPH7_H
