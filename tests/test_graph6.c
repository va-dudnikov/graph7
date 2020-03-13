#include <graph7/utils/misc.h>
#include <graph7/graph6.h>

#include <unity.h>
#include <string.h>

#include "utils.h"

#define BUFF_SIZE (1000 * 1000)

// Encoded complete graph with 67 vertices
# define EC_67_K 373
static const uint8_t * ec67 =   "~?@B~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                "~~~~~~~~~~~~w";

static uint8_t buff1[BUFF_SIZE];
static uint8_t buff2[BUFF_SIZE];
static uint8_t buff3[BUFF_SIZE];

void test_graph6_order_encode(void)
{
    uint8_t buff[8];

    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph6_order_encode(NULL, 62));

    // Null graph
    TEST_ASSERT_EQUAL(1, graph6_order_encode(buff, 0));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("?", buff, 1);

    // Trivial graph
    TEST_ASSERT_EQUAL(1, graph6_order_encode(buff, 1));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("@", buff, 1);

    // 3 cases
    // Case 1
    TEST_ASSERT_EQUAL(1, graph6_order_encode(buff, 62));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("}", buff, 1);

    // Case 2
    TEST_ASSERT_EQUAL(4, graph6_order_encode(buff, 63));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("~??~", buff, 4);

    TEST_ASSERT_EQUAL(4, graph6_order_encode(buff, 258047));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("~}~~", buff, 4);

    if(sizeof(size_t) > 4) // For 64-bit systems
    {
        // Case 3
        TEST_ASSERT_EQUAL(8, graph6_order_encode(buff, 258048));
        TEST_ASSERT_EQUAL_CHAR_ARRAY("~~???~??", buff, 8);

        TEST_ASSERT_EQUAL(8, graph6_order_encode(buff, 68719476735));
        TEST_ASSERT_EQUAL_CHAR_ARRAY("~~~~~~~~", buff, 8);

        // Unsupported case
        TEST_ASSERT_EQUAL(-GRAPH7_UNSUPPORTED, graph6_order_encode(buff, 68719476736));
    }
}

void test_graph6_order_decode(void)
{
    size_t order;

    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph6_order_decode(NULL, "}", 1));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph6_order_decode(&order, NULL, 1));

    // Null graph
    TEST_ASSERT_EQUAL(1, graph6_order_decode(&order, "?", 1));
    TEST_ASSERT_EQUAL_size_t(0, order);

    // Trivial graph
    TEST_ASSERT_EQUAL(1, graph6_order_decode(&order, "@", 1));
    TEST_ASSERT_EQUAL_size_t(1, order);

    // 3 cases
    // Case 1
    TEST_ASSERT_EQUAL(1, graph6_order_decode(&order, "}", 1));
    TEST_ASSERT_EQUAL_size_t(62, order);

    // Case 2
    TEST_ASSERT_EQUAL(4, graph6_order_decode(&order, "~??~", 4));
    TEST_ASSERT_EQUAL_size_t(63, order);

    TEST_ASSERT_EQUAL(4, graph6_order_decode(&order, "~}~~", 4));
    TEST_ASSERT_EQUAL_size_t(258047, order);

    if(sizeof(size_t) > 4) // For 64-bit systems
    {
        // Case 3
        TEST_ASSERT_EQUAL(8, graph6_order_decode(&order, "~~???~??", 8));
        TEST_ASSERT_EQUAL_size_t(258048, order);

        // Unsupported case
        TEST_ASSERT_EQUAL(8, graph6_order_decode(&order, "~~~~~~~~", 8));
        TEST_ASSERT_EQUAL_size_t(68719476735, order);
    }
}

void test_graph6_encode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph6_encode_from_matrix(NULL, buff2, 42, false));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph6_encode_from_matrix(buff1, NULL, 42, false));

    // Null graph and trivial graph
    TEST_ASSERT_EQUAL(1, graph6_encode_from_matrix(buff2, "", 0, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("?", buff2, 1);

    TEST_ASSERT_EQUAL(1, graph6_encode_from_matrix(buff2, "", 1, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("@", buff2, 1);

    // Small complete graphs
    complete_graph(buff1, 4);
    TEST_ASSERT_EQUAL(2, graph6_encode_from_matrix(buff2, buff1, 4, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("C~", buff2, 2);

    // Test optional header
    TEST_ASSERT_EQUAL(GRAPH6_HEADER_LEN + 2, graph6_encode_from_matrix(buff2, buff1, 4, true));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(GRAPH6_HEADER "C~", buff2, GRAPH6_HEADER_LEN + 2);

    // Large complete graph
    complete_graph(buff1, 67);

    TEST_ASSERT_EQUAL(EC_67_K, graph6_encode_from_matrix(buff2, buff1, 67, false));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(ec67, buff2, EC_67_K);
}

void test_graph6_decode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph6_decode_to_matrix(NULL, buff2, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph6_decode_to_matrix(buff1, NULL, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, graph6_decode_to_matrix(buff1, GRAPH6_HEADER, GRAPH6_HEADER_LEN));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_HEADER, graph6_decode_to_matrix(buff1, ">>graph6<!!", GRAPH6_HEADER_LEN + 1));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, graph6_decode_to_matrix(buff1, "", 0));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, graph6_decode_to_matrix(buff1, "A", 1));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_DATA, graph6_decode_to_matrix(buff1, "A!", 2));

    // Null graph and trivial graph
    TEST_ASSERT_EQUAL(0, graph6_decode_to_matrix(buff1, "?", 1));
    TEST_ASSERT_EQUAL(1, graph6_decode_to_matrix(buff1, "@", 1));

    // Small complete graphs
    complete_graph(buff2, 4);
    TEST_ASSERT_EQUAL(4, graph6_decode_to_matrix(buff1, "C~", 2));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, buff1, 4 * 4);

    // Test optional header
    TEST_ASSERT_EQUAL(4, graph6_decode_to_matrix(buff1, ">>graph6<<C~", 12));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, buff1, 4 * 4);

    // Large complete graph
    complete_graph(buff2, 67);
    TEST_ASSERT_EQUAL(67, graph6_decode_to_matrix(buff1, ec67, EC_67_K));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, buff1, 4 * 4);

}

void test_graph6_encode_decode(void)
{
    // Graphs
    size_t orders[] =
    {
        // k = order * (order - 1) / 2
        4, 504, 997, // k % 6 == 0
        2, 503, 998, // k % 6 == 1
        3, 499, 999, // k % 6 == 3
        5, 500, 992  // k % 6 == 4
    };

    for(size_t i = 0; i < 16; i++)
    {
        size_t order = orders[i];
        size_t bytes = utils_ceiling_div(order * (order - 1) / 2, 6);

        if(order < 63)
            bytes += 1;
        else if(order > 63 && order < 258048)
            bytes += 4;
        else
            bytes += 8;

        rand_graph(buff1, order);
        TEST_ASSERT_EQUAL(bytes, graph6_encode_from_matrix(buff3, buff1, order, false));
        TEST_ASSERT_EQUAL(order, graph6_decode_to_matrix(buff2, buff3, bytes));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff2, order * order);
    }
}
