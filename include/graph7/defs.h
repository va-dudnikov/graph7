#ifndef _GRAPH7_DEFS_H
#define _GRAPH7_DEFS_H

#define GRAPH7
#define GRAPH7_VERSION_MAJOR 0
#define GRAPH7_VERSION_MINOR 2
#define GRAPH7_VERSION_BUILD 0
#define GRAPH7_VERSION ((GRAPH7_VERSION_MAJOR << 16) | (GRAPH7_VERSION_MINOR << 8) | GRAPH7_VERSION_BUILD)

#include <stdint.h>

// MSVC haven't ssize_t so for it.
#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <unistd.h>
#endif // _MSC_VER

// graph7 graphs formats supported
#define GRAPH7_UNDIRECTED       0   //!< Undirected graph
#define GRAPH7_DIRECTED         1   //!< Directed graph
#define GRAPH7_UNDIRECTED_LOOPS 2   //!< Undirected graph with loops
#define GRAPH7_DIRECTED_LOOPS   3   //!< Directed graph with loops

#define GRAPH7_COMPACT_ENCODING  0  //!< Encoding as 1 byte = 1 bit (passed to the width argument).

// graph6 headers
#define GRAPH6_HEADER       ">>graph6<<"
#define GRAPH6_HEADER_LEN   10
#define DIGRAPH6_HEADER     ">>digraph6<<"
#define DIGRAPH6_HEADER_LEN 12
#define SPARSE6_HEADER      ">>sparse6<<"
#define SPARSE6_HEADER_LEN  11

typedef uint8_t graph7_gtype_t; //!< Type of graph.

// If there is no bool type
#ifndef bool
#define bool uint8_t    //!< Boolean type
#endif

#ifndef true
#define true 1          //!< True value
#endif

#ifndef false
#define false 0         //!< False value
#endif

#endif // _GRAPH7_DEFS_H
