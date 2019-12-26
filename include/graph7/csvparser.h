/*!
 * \file
 */

#ifndef GRAPH7_CSVPARSER_H
#define GRAPH7_CSVPARSER_H

#include <stdint.h>

/*!
 * \brief csv_parser_field_pos
 * \param buf
 * \param sep
 * \param field
 * \return
 */
ssize_t csv_parser_field_pos(const char *buf, const char *sep, size_t field);

/*!
 * \brief csv_parser_fields_count
 * \param buf
 * \param sep
 * \return
 */
ssize_t csv_parser_fields_count(const char *buf, const char *sep);

#endif // GRAPH7_CSVPARSER_H
