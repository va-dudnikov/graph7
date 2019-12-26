/*!
 * \file
 */

#ifndef GRAPH7_DEF_H
#define GRAPH7_DEF_H

#include <graph7/errno.h>

/*!
 * \name CSV parser settings
 * \{
 */

#define CSV_PARSER_QUOTE    '"' //!< Quote-symbol
#define CSV_PARSER_STD_SEP  "," //!< Standard separator

//!\}

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

#endif // GRAPH7_DEF_H
