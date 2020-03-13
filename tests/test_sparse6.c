#include <graph7/graph6.h>

#include <unity.h>
#include <string.h>

#include "utils.h"

#define BUFF_SIZE (1000 * 1000)

static uint8_t buff1[BUFF_SIZE];
static uint8_t buff2[BUFF_SIZE];
static uint8_t buff3[BUFF_SIZE];

void test_sparse6_encode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, sparse6_encode_from_matrix(NULL, buff1, 42, false));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, sparse6_encode_from_matrix(buff1, NULL, 42, false));

    // Null graph
    TEST_ASSERT_EQUAL(2, sparse6_encode_from_matrix(buff1, "", 0, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(":?", buff1, 2);

    // Trivial graph
    TEST_ASSERT_EQUAL(2, sparse6_encode_from_matrix(buff1, "", 1, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(":@", buff1, 2);

    // Empty graph
    memset((void *)buff1, 0, BUFF_SIZE);
    TEST_ASSERT_EQUAL(2, sparse6_encode_from_matrix(buff2, buff1, 5, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(":D", buff2, 2);

    TEST_ASSERT_EQUAL(5, sparse6_encode_from_matrix(buff2, buff1, 68, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(":~?@C", buff2, 2);

    // Test padding
    const char* codes[] = {":Cdv", ":DaYn", ":EaYnN", ":FaYnL", ":GaYnLz"};

    for(size_t i = 0; i < 5; i++)
    {
        size_t order = 4 + i;
        path_graph(buff1, order);
        size_t length = strlen(codes[i]);
        TEST_ASSERT_EQUAL(length, sparse6_encode_from_matrix(buff2, buff1, order, false));
        TEST_ASSERT_EQUAL_CHAR_ARRAY(codes[i], buff2, length);
    }
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
    for(size_t order = 4; order < 100; order++)
    {
        path_graph(buff1, order);
        ssize_t bytes = sparse6_encode_from_matrix(buff3, buff1, order, false);
        TEST_ASSERT_GREATER_THAN(0, bytes);
        TEST_ASSERT_EQUAL(order, sparse6_decode_to_matrix(buff2, buff3, bytes));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff2, order * order);
    }
}
