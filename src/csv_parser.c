#include "def.h"
#include "csv_parser.h"

#include <string.h>

ssize_t csv_parser_field_pos(const char *buf, const char *sep, size_t field)
{
    if(!buf)
        return GRAPH7_INVALID_ARG;

    const char *_sep = (sep) ? sep : CSV_PARSER_STD_SEP;

    size_t buflen = strlen(buf);
    size_t seplen = strlen(_sep);

    if(buflen <= seplen)
        return -1;

    if(field == 0)
        return 0;

    size_t pos;
    size_t i;
    uint8_t is_quote = 0;

    for(i = 0, pos = 0; i < buflen - seplen && pos != field; i++)
    {
        if(buf[i] == CSV_PARSER_QUOTE)
        {
            is_quote = !is_quote;
            continue;
        }

        if(is_quote)
            continue;

        if(memcmp(&buf[i], _sep, seplen) == 0)
            ++pos;
    }

    return (pos == field) ? (ssize_t)(i + seplen - 1) : -GRAPH7_INVALID_ARG;
}

ssize_t csv_parser_fields_count(const char *buf, const char *sep)
{
//    size_t i;
//    ssize_t ret;
//    for(i = 0; (ret = csv_parser_field_pos(buf, sep, i)) >= 0; i++);
}
