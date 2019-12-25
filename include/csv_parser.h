#ifndef GRAPH7_CSV_PARSER_H
#define GRAPH7_CSV_PARSER_H

#include <stdint.h>

ssize_t csv_parser_field_pos(const char *buf, const char *sep, size_t field);

ssize_t csv_parser_fields_count(const char *buf, const char *sep);

#endif // GRAPH7_CSV_PARSER_H
