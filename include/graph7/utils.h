/*!
 * \file
 * \brief Misc useful utils.
 */
#ifndef GRAPH7_UTILS_H
#define GRAPH7_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <graph7/def.h>

/*!
 * \brief Checking what processor byte order.
 * \return
 */
uint8_t graph7_utils_endianness(void);

/*!
 * \brief Reverse a buffer.
 * \param[in] src Buffer.
 * \param[in] length Buffer length.
 */
void graph7_utils_reverse(uint8_t *src, size_t length);

/*!
 * \brief Integer division and apply ceiling.
 * \param[in] numerator
 * \param[in] denominator
 * \return Number.
 */
size_t graph7_utils_ceiling_div(size_t numerator, size_t denominator);

/*!
 * \brief Counting the number of bits in a number.
 * \param[in] x Number.
 * \return The number of bits.
 */
size_t graph7_utils_count_bits(size_t x);

/*!
 * \brief graph7_utils_bytes_start_with
 * \param src
 * \param start
 * \param start_length
 * \return
 */
bool graph7_utils_bytes_start_with(const uint8_t *src, const uint8_t *start, size_t start_length);

#endif // GRAPH7_UTILS_H

#ifdef __cplusplus
}
#endif
