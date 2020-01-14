/*!
 * \file
 */

#ifndef GRAPH7_DEF_H
#define GRAPH7_DEF_H

#include <graph7/errno.h>
#include <stdint.h>

/*!
 * \name Types of graphs
 * Formulas for calculate of count of cells for adjacency matrix:\n
 * Undirected:             n * (n - 1) / 2\n
 * Undirected with loops:  n * (n + 1) / 2\n
 * Directed:               n * (n - 1)\n
 * Directed with loops:    n^2\n
 * where n - order of graph.
 * \{
 */
#define GRAPH7_UNDIRECTED       0   //!< Undirected graph
#define GRAPH7_DIRECTED         1   //!< Directed graph
#define GRAPH7_UNDIRECTED_LOOPS 2   //!< Undirected graph with loops
#define GRAPH7_DIRECTED_LOOPS   3   //!< Directed graph with loops
//!\}

/*!
 * \name Other
 * \{
 */
#define GRAPH7_COMPACT_ENCODING  0   //!< Encoding as 1 byte = 1 bit (transmitted to the width argument)
//!\}

typedef uint8_t graph7_gtype_t; //!< Type of graph.

typedef union graph7_header
{
    struct
    {
        uint8_t weighed     :1;
        uint8_t gtype       :2;
        uint8_t tail        :3;
        uint8_t reserved    :2;
    };

    uint8_t byte;
} graph7_header_t;

typedef union graph7_wheader
{
    struct
    {
        uint8_t extended        :1;
        uint8_t width           :5;
        uint8_t reserved        :2;
    };

    uint8_t byte;
} graph7_wheader_t;

#define GRAPH7_BUFFER_SIZE 4096 //!< Stack buffer size for

#endif // GRAPH7_DEF_H
