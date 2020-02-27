#include <unity.h>

#include <string.h>
#include <graph7/graph6.h>

#include "utils.h"

#define BUFF_SIZE (1000 * 1000)

static uint8_t buff1[BUFF_SIZE];
static uint8_t buff2[BUFF_SIZE];
static uint8_t buff3[BUFF_SIZE];

void test_sparse6_encode(void)
{
}

void test_sparse6_decode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, sparse6_decode_to_matrix(NULL, buff1, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, sparse6_decode_to_matrix(buff1, NULL, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, sparse6_decode_to_matrix(buff1, ">>sparse6<<", 11));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_HEADER, sparse6_decode_to_matrix(buff1, ">>sarse6<<:xxx", 14));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_HEADER, sparse6_decode_to_matrix(buff1, ">>sparse6<<yyy", 14));

    // Test padding
    const char* codes[] = {":Cdv", ":DaYn", ":EaYnN", ":FaYnL", ":GaYnLz"};

    for(size_t i = 0; i < 5; i++)
    {
        size_t order = 4 + i;
        path_graph(buff1, order);
        TEST_ASSERT_EQUAL(order, sparse6_decode_to_matrix(buff2, codes[i], strlen(codes[i])));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff2, order * order);
    }

}

void test_sparse_encode_decode(void)
{
}
