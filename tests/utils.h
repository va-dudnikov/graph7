#ifndef GRAPH7_TESTS_UTILS_H
#define GRAPH7_TESTS_UTILS_H

#include <graph7/def.h>

void rand_graph(uint8_t *dst, size_t order);

void rand_dirgraph(uint8_t *dst, size_t order);

void complete_graph(uint8_t *dst, size_t order);

void path_graph(uint8_t *dst, size_t order);

void weighted_graph(uint8_t *dst, size_t order, size_t width, size_t max_value, bool directed, bool loops);

#endif // GRAPH7_TESTS_UTILS_H
