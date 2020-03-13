#include <graph7/utils/misc.h>
#include <graph7/graph6.h>

#include <unity.h>
#include <string.h>

#include "utils.h"

#define BUFF_SIZE (1000 * 1000)

static uint8_t buff1[BUFF_SIZE];
static uint8_t buff2[BUFF_SIZE];
static uint8_t buff3[BUFF_SIZE];

void test_digraph6_encode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, digraph6_encode_from_matrix(NULL, buff2, 42, false));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, digraph6_encode_from_matrix(buff1, NULL, 42, false));

    // Null graph
    TEST_ASSERT_EQUAL(2, digraph6_encode_from_matrix(buff2, "", 0, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("&?", buff2, 2);

    // Trivial graph without loop
    buff1[0] = 0;
    TEST_ASSERT_EQUAL(3, digraph6_encode_from_matrix(buff2, buff1, 1, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("&@?", buff2, 3);

    // Trivial graph with loop
    buff1[0] = 1;
    TEST_ASSERT_EQUAL(3, digraph6_encode_from_matrix(buff2, buff1, 1, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("&@_", buff2, 3);

    // Small complete graphs
    complete_graph(buff1, 4);
    TEST_ASSERT_EQUAL(5, digraph6_encode_from_matrix(buff2, buff1, 4, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("&C]|w", buff2, 5);

    // Test optional header
    TEST_ASSERT_EQUAL(DIGRAPH6_HEADER_LEN + 5, digraph6_encode_from_matrix(buff2, buff1, 4, true));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(DIGRAPH6_HEADER "&C]|w", buff2, DIGRAPH6_HEADER_LEN + 5);
}

void test_digraph6_decode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, digraph6_decode_to_matrix(NULL, buff2, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, digraph6_decode_to_matrix(buff1, NULL, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, digraph6_decode_to_matrix(buff1, DIGRAPH6_HEADER, DIGRAPH6_HEADER_LEN));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_HEADER, digraph6_decode_to_matrix(buff1, ">>digraph6<!!", DIGRAPH6_HEADER_LEN + 1));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_HEADER, digraph6_decode_to_matrix(buff1, ">>digraph6<<aaaa", DIGRAPH6_HEADER_LEN + 4));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, digraph6_decode_to_matrix(buff1, "", 0));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, digraph6_decode_to_matrix(buff1, "&", 1));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, digraph6_decode_to_matrix(buff1, "&A", 2));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_DATA, digraph6_decode_to_matrix(buff1, "&A!", 3));

    // Small complete graphs
    complete_graph(buff2, 4);
    TEST_ASSERT_EQUAL(4, digraph6_decode_to_matrix(buff1, "&C]|w", 5));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, buff1, 4 * 4);

    // Test optional header
    TEST_ASSERT_EQUAL(4, digraph6_decode_to_matrix(buff1, ">>digraph6<<&C]|w", DIGRAPH6_HEADER_LEN + 5));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, buff1, 4 * 4);
}

void test_digraph6_encode_decode(void)
{
    // Graphs
    size_t orders[] =
    {
        // k = order * (order - 1) / 2
        6, 504, 996, // k % 6 == 0
        1, 503, 997, // k % 6 == 1
        3, 501, 999, // k % 6 == 3
        2, 500, 992  // k % 6 == 4
    };

    for(size_t i = 0; i < 16; i++)
    {
        size_t order = orders[i];
        size_t bytes = 1 + utils_ceiling_div(order * order, 6);

        if(order < 63)
            bytes += 1;
        else if(order > 63 && order < 258048)
            bytes += 4;
        else
            bytes += 8;

        rand_dirgraph(buff1, order);
        TEST_ASSERT_EQUAL(bytes, digraph6_encode_from_matrix(buff3, buff1, order, false));
        TEST_ASSERT_EQUAL(order, digraph6_decode_to_matrix(buff2, buff3, bytes));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff2, order * order);
    }
}
