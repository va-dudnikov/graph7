/*!
 * \file
 */

#ifndef GRAPH7_DEF_H
#define GRAPH7_DEF_H

#include <graph7/errno.h>
#include <stdint.h>

#ifndef bool
#define bool uint8_t
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

// MSVC haven't ssize_t
#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

/*!
 * \name Types of graphs
 * Types of graphs for encoding/decoding.
 * \{
 */
#define GRAPH7_UNDIRECTED       0   //!< Undirected graph
#define GRAPH7_DIRECTED         1   //!< Directed graph
#define GRAPH7_UNDIRECTED_LOOPS 2   //!< Undirected graph with loops
#define GRAPH7_DIRECTED_LOOPS   3   //!< Directed graph with loops
/*! \} */

/*!
 * \name Other
 * Other constants
 * \{
 */
#define GRAPH7_COMPACT_ENCODING  0  //!< Encoding as 1 byte = 1 bit (passed to the width argument)
#define GRAPH7_LITTLE_ENDIAN 0
#define GRAPH7_BIG_ENDIAN 1
/*! \} */

/*!
 * \name Macroses
 * \{
 */

#define GRAPH7_MIDX(i, j, order)\
    ((i) * (order) + (j))  //!< Get array index

#define GRAPH7_WMIDX(i, j, order, width)\
    ((i) * (order) * (width) + (j) * (width))  //!< Get array index

/*! \} */

/*!
 * \brief The main format header
 */
struct graph7_header
{
    uint8_t weighed     :1; //!< For weighted graph
    uint8_t gtype       :2; //!< Type of graph
    uint8_t tail        :3; //!< Tail bits when grouping data by 6 bit
    uint8_t reserved    :2; //!< Reserved bits for encoding to ascii
};

/*!
 * \brief The header for weighted graphs.
 */
struct graph7_wheader
{
    uint8_t extended        :1; //!< For weighted graphs with large data type
    uint8_t width           :5; //!< Size of data type
    uint8_t reserved        :2; //!< Reserved bits for encoding to ascii
};

typedef uint8_t graph7_gtype_t;                 //!< Type of graph.
typedef struct graph7_header graph7_header_t;   //!< Main header
typedef struct graph7_wheader graph7_wheader_t; //!< Header for weighted graphs

#endif // GRAPH7_DEF_H
