#include <unity.h>

#include <stdlib.h>
#include <string.h>
#include <graph7/graph6.h>

#define BUFF12_SIZE (1000 * 1000)
#define BUFF3_SIZE 83258 // 8 + ceiling(1000 * 999 / (2 * 6))

static uint8_t buff1[BUFF12_SIZE];
static uint8_t buff2[BUFF12_SIZE];
static uint8_t buff3[BUFF3_SIZE];

static void rand_graph(uint8_t *dst, size_t order)
{
    for(size_t i = 0; i < order - 1; i++)
    {
        for(size_t j = i + 1; j < order; j++)
        {
            dst[GRAPH7_M_INDEX(i, j, order)] = rand() % 2;
            dst[GRAPH7_M_INDEX(j, i, order)] = dst[GRAPH7_M_INDEX(i, j, order)];
        }
    }

    for(size_t i = 0; i < order; i++)
        dst[GRAPH7_M_INDEX(i, i, order)] = 0;
}

void test_graph7_graph6_order_encode(void)
{
    uint8_t buff[8];

    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_order_encode(NULL, 62));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_order_encode(buff, 0));

    // 3 cases
    // Case 1
    TEST_ASSERT_EQUAL(1, graph7_graph6_order_encode(buff, 62));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("}", buff, 1);

    // Case 2
    TEST_ASSERT_EQUAL(4, graph7_graph6_order_encode(buff, 63));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("~??~", buff, 4);

    TEST_ASSERT_EQUAL(4, graph7_graph6_order_encode(buff, 258047));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("~}~~", buff, 4);

    // Case 3
    TEST_ASSERT_EQUAL(8, graph7_graph6_order_encode(buff, 258048));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("~~???~??", buff, 8);

    TEST_ASSERT_EQUAL(8, graph7_graph6_order_encode(buff, 68719476735));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("~~~~~~~~", buff, 8);

    // Unsupported case
    TEST_ASSERT_EQUAL(-GRAPH7_UNSUPPORTED, graph7_graph6_order_encode(buff, 68719476736));

}

void test_graph7_graph6_order_decode(void)
{
    size_t order;

    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_order_decode(NULL, "}"));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_order_decode(&order, NULL));

    // 3 cases
    // Case 1
    TEST_ASSERT_EQUAL(1, graph7_graph6_order_decode(&order, "}"));
    TEST_ASSERT_EQUAL_size_t(62, order);

    // Case 2
    TEST_ASSERT_EQUAL(4, graph7_graph6_order_decode(&order, "~??~"));
    TEST_ASSERT_EQUAL_size_t(63, order);

    TEST_ASSERT_EQUAL(4, graph7_graph6_order_decode(&order, "~}~~"));
    TEST_ASSERT_EQUAL_size_t(258047, order);

    // Case 3
    TEST_ASSERT_EQUAL(8, graph7_graph6_order_decode(&order, "~~???~??"));
    TEST_ASSERT_EQUAL_size_t(258048, order);

    TEST_ASSERT_EQUAL(8, graph7_graph6_order_decode(&order, "~~~~~~~~"));
    TEST_ASSERT_EQUAL_size_t(68719476735, order);
}

void test_graph7_graph6_encode_decode(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_encode_from_matrix(NULL, buff2, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_encode_from_matrix(buff1, NULL, 42));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_encode_from_matrix(buff1, buff2, 1));

    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_decode_to_matrix(NULL, buff2));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_graph6_decode_to_matrix(buff1, NULL));

    // Graphs
    size_t orders[] = {4, 504, 997, 2, 503, 998, 3, 499, 999, 5, 500, 992};

    for(size_t i = 0; i < 16; i++)
    {
        size_t order = orders[i];
        size_t bytes = graph7_utils_ceiling_div(order * (order - 1) / 2, 6);

        if(order < 63)
            bytes += 1;
        else if(order > 63 && order < 258048)
            bytes += 4;
        else
            bytes += 8;

        rand_graph(buff1, order);
        TEST_ASSERT_EQUAL(bytes, graph7_graph6_encode_from_matrix(buff3, buff1, order));
        TEST_ASSERT_EQUAL(order, graph7_graph6_decode_to_matrix(buff2, buff3));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff2, order * order);
    }
}
