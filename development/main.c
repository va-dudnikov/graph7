#include <stdio.h>
#include <graph7.h>
#include <csv_parser.h>

static const char *matrix = "0, 1, 0, 0, \n"
                            "1, 0, 1, 1, \n"
                            "0, 1, 0, 1, \n"
                            "0, 1, 1, 0, \n";

static char buff[1024];

int main()
{
    const char *str = "hello;world;\"mother;fucker\";125";
    ssize_t ret = csv_parser_field_pos(str, ";", 3);
    if(ret >= 0)
        printf("%s", &str[ret]);
}
