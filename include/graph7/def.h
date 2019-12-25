#ifndef GRAPH7_DEF_H
#define GRAPH7_DEF_H

#include "errno.h"

/* CSV PARSER */
#define CSV_PARSER_QUOTE    '"'
#define CSV_PARSER_STD_SEP  ","

/*
    Types of graphs
    Formulas for calculate of count of cells of adjacency matrix:
    Undirected:             n * (n - 1) / 2
    Undirected with loops:  n * (n + 1) / 2
    Directed:               n * (n - 1)
    Directed with loops:    n^2
    Where n: order of graph
*/
#define GRAPH7_UNDIRECTED       0   // Undirected graph
#define GRAPH7_DIRECTED         1   // Directed graph
#define GRAPH7_UNDIRECTED_LOOPS 2   // Undirected graph with loops
#define GRAPH7_DIRECTED_LOOPS   3   // Directed graph with loops

#define GRAPH7_COMPACT_ENCODING  0   // Encoding as 1 byte = 1 bit (transmitted to the width argument)

#endif // GRAPH7_DEF_H
