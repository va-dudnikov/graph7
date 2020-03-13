/*
 * Example for weighted graph.
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
        float mat[] =
        {
            0.f,    3.14f,  2.71f,  0.f,
            3.14f,  0.f,    0.f,    0.f,
            2.71f,  0.f,    0.f,    0.f,
            0.f,    0.f,    0.f,    0.f,
        };

        size_t order = 4;

        ssize_t retval = graph7_encode_from_matrix(
                    ibuff,
                    (uint8_t *)mat,
                    order,
                    GRAPH7_UNDIRECTED,
                    sizeof(float)
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

        float *mat = (float *)obuff;

        for(int i = 0; i < order; i++)
        {
            for(int j = 0; j < order; j++)
            {
                printf("%.2f ", mat[i * order + j]);
            }
            printf("\n");
        }
    }

    return 0;
}
