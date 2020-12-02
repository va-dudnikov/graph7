/*
 * graph7 encoding and decoding simple graph.
 */

#include <stdio.h>
#include <string.h>
#include <graph7/graph7.h>

int main()
{
    uint8_t ibuff[1024]; // For encoded graph
    uint8_t obuff[1024]; // For decoded graph

    /* Encoding */
    {
        uint8_t mat[] =
        {
            0, 1, 0, 0,
            1, 0, 1, 1,
            0, 1, 0, 0,
            0, 1, 0, 0,
        };

        size_t order = 4;

        ssize_t retval = graph7_encode_from_matrix(
                    ibuff,
                    mat,
                    order,
                    GRAPH7_UNDIRECTED,
                    GRAPH7_COMPACT_ENCODING // Replace it on 1 and check result
                );

        if(retval < 0)
        {
            printf("Error: %zd\n", retval);
            return -1;
        }

        ibuff[retval] = 0;

        printf("Encoded graph: %s\n", ibuff);
    }

    /* Decoding */
    {
        ssize_t order = graph7_decode_to_matrix(obuff, ibuff, strlen(ibuff));

        if(order < 0)
        {
            printf("Error: %zd\n", order);
            return -1;
        }

        printf("Decoded graph:\n");

        for(int i = 0; i < order; i++)
        {
            for(int j = 0; j < order; j++)
            {
                printf("%u ", obuff[i * order + j]);
            }
            printf("\n");
        }
    }

    return 0;
}
