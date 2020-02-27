#include <unity.h>

#include <string.h>
#include <graph7/graph6.h>

#include "utils.h"

#define BUFF12_SIZE (1000 * 1000)
#define BUFF3_SIZE 166671 // 4 + ceiling(1000 * 1000 / 6)

static uint8_t buff1[BUFF12_SIZE];
static uint8_t buff2[BUFF12_SIZE];
static uint8_t buff3[BUFF3_SIZE];

void test_digraph6_encode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, digraph6_encode_from_matrix(NULL, buff2, 42, false));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, digraph6_encode_from_matrix(buff1, NULL, 42, false));

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
}
