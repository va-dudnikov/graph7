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
            dst[GRAPH7_MIDX(i, j, order)] = rand() % 2;
            dst[GRAPH7_MIDX(j, i, order)] = dst[GRAPH7_MIDX(i, j, order)];
        }
    }

    for(size_t i = 0; i < order; i++)
        dst[GRAPH7_MIDX(i, i, order)] = 0;
}

void rand_dirgraph(uint8_t *dst, size_t order)
{
    memset(dst, 0, order * order);
    for(size_t i = 0; i < order; i++)
    {
        for(size_t j = 0; j < order; j++)
        {
            dst[GRAPH7_MIDX(i, j, order)] = rand() % 2;
        }
    }
}

void complete_graph(uint8_t *dst, size_t order)
{
    memset(dst, 0, order * order);
    for(size_t i = 0; i < order; i++)
    {
        for(size_t j = 0; j < order; j++)
        {
            if(i == j)
                dst[GRAPH7_MIDX(i, j, order)] = 0;
            else
                dst[GRAPH7_MIDX(i, j, order)] = 1;
        }
    }
}

void path_graph(uint8_t *dst, size_t order)
{
    memset(dst, 0, order * order);
    for(size_t i = 0; i < order - 1; i++)
    {
        dst[GRAPH7_MIDX(i, i + 1, order)] = 1;
        dst[GRAPH7_MIDX(i + 1, i, order)] = 1;
    }
}

void weighted_graph(uint8_t *dst, size_t order, size_t width, size_t max_value, bool directed, bool loops)
{
    memset(dst, 0, order * order);
    for(size_t i = 0; i < order; i++)
    {
        for(size_t j = i; j < order; j++)
        {
            if(!loops && i == j)
                continue;

            size_t upper_idx = GRAPH7_WMIDX(i, j, order, width);
            size_t lower_idx = GRAPH7_WMIDX(j, i, order, width);

            for(size_t k = 0; k < width; k++)
            {
                dst[upper_idx + k] = (uint8_t)(rand() % max_value);

                if(i != j)
                {
                    if(directed)
                        dst[lower_idx + k] = (uint8_t)(rand() % max_value);
                    else
                        dst[lower_idx + k] = dst[upper_idx + k];
                }
            }
        }
    }
}
