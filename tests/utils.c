#include <stdlib.h>
#include <string.h>

#include "utils.h"

void rand_graph(uint8_t *dst, size_t order)
{
    memset(dst, 0, order * order);
    for(size_t i = 0; i < order - 1; i++)
    {
        for(size_t j = i + 1; j < order; j++)
        {
            dst[GRAPH7_M_INDEX(i, j, order)] = rand() % 2;
            dst[GRAPH7_M_INDEX(j, i, order)] = dst[GRAPH7_M_INDEX(i, j, order)];
        }
    }

    for(size_t i = 0; i < order; i++)
        dst[GRAPH7_M_INDEX(i, i, order)] = 0;
}

void complete_graph(uint8_t *dst, size_t order)
{
    memset(dst, 0, order * order);
    for(size_t i = 0; i < order; i++)
    {
        for(size_t j = 0; j < order; j++)
        {
            if(i == j)
                dst[GRAPH7_M_INDEX(i, j, order)] = 0;
            else
                dst[GRAPH7_M_INDEX(i, j, order)] = 1;
        }
    }
}

void path_graph(uint8_t *dst, size_t order)
{
    memset(dst, 0, order * order);
    for(size_t i = 0; i < order - 1; i++)
    {
        dst[GRAPH7_M_INDEX(i, i + 1, order)] = 1;
        dst[GRAPH7_M_INDEX(i + 1, i, order)] = 1;
    }
}
