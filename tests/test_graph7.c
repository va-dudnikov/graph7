#include <graph7/graph7.h>
#include <graph7/errno.h>
#include <graph7/utils/misc.h>

#include <unity.h>
#include <string.h>

#include "utils.h"

#define MAX_ORDER 25
#define MAX_WIDTH 33
#define BUFF_SIZE (MAX_ORDER * MAX_ORDER * MAX_WIDTH)

static uint8_t buff1[BUFF_SIZE];
static uint8_t buff2[BUFF_SIZE];
static uint8_t buff3[BUFF_SIZE];

void test_graph7_encode_header(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(
        -GRAPH7_INVALID_ARG,
        graph7_encode_header(NULL, 1, GRAPH7_UNDIRECTED, GRAPH7_COMPACT_ENCODING)
    );
    TEST_ASSERT_EQUAL(
        -GRAPH7_INVALID_ARG,
        graph7_encode_header(buff1, 0, GRAPH7_UNDIRECTED, GRAPH7_COMPACT_ENCODING)
    );
    TEST_ASSERT_EQUAL(
        -GRAPH7_UNSUPPORTED,
        graph7_encode_header(buff1, 1, GRAPH7_UNDIRECTED, (1 << 4 * 6) + 1)
    );

    // Trivial graph | weigted = false, extended = false
    TEST_ASSERT_EQUAL(1, graph7_encode_header(buff1, 1, GRAPH7_UNDIRECTED, GRAPH7_COMPACT_ENCODING));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("A", buff1, 1);
    TEST_ASSERT_EQUAL(1, graph7_encode_header(buff1, 1, GRAPH7_DIRECTED, GRAPH7_COMPACT_ENCODING));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("C", buff1, 1);
    TEST_ASSERT_EQUAL(1, graph7_encode_header(buff1, 1, GRAPH7_UNDIRECTED_LOOPS, GRAPH7_COMPACT_ENCODING));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("M", buff1, 1);
    TEST_ASSERT_EQUAL(1, graph7_encode_header(buff1, 1, GRAPH7_DIRECTED_LOOPS, GRAPH7_COMPACT_ENCODING));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("O", buff1, 1);

    // Weight = 1 byte | weigted = true, extended = false
    size_t order = 1;
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED, 1));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("BA", buff1, 2);
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_DIRECTED, 1));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("DA", buff1, 2);
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED_LOOPS, 1));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("VA", buff1, 2);
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_DIRECTED_LOOPS, 1));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("XA", buff1, 2);

    // Weight = 32 bytes | weigted = true, extended = false
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED, 32));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("B+", buff1, 2);
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_DIRECTED, 32));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("D+", buff1, 2);
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED_LOOPS, 32));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("N+", buff1, 2);
    TEST_ASSERT_EQUAL(2, graph7_encode_header(buff1, order, GRAPH7_DIRECTED_LOOPS, 32));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("P+", buff1, 2);

    // Weight = 33 bytes | weighted = true, extended = true, 1 sextet
    TEST_ASSERT_EQUAL(3, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED, 33));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("BDg", buff1, 3);
    TEST_ASSERT_EQUAL(3, graph7_encode_header(buff1, order, GRAPH7_DIRECTED, 33));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("DDg", buff1, 3);
    TEST_ASSERT_EQUAL(3, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED_LOOPS, 33));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("FDg", buff1, 2);
    TEST_ASSERT_EQUAL(3, graph7_encode_header(buff1, order, GRAPH7_DIRECTED_LOOPS, 33));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("HDg", buff1, 2);

    // Weight = 65 bytes | weighted = true, extended = true, 2 sextets
    TEST_ASSERT_EQUAL(4, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED, 65));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("BFAB", buff1, 4);
    TEST_ASSERT_EQUAL(4, graph7_encode_header(buff1, order, GRAPH7_DIRECTED, 65));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("DFAB", buff1, 4);
    TEST_ASSERT_EQUAL(4, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED_LOOPS, 65));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("NFAB", buff1, 4);
    TEST_ASSERT_EQUAL(4, graph7_encode_header(buff1, order, GRAPH7_DIRECTED_LOOPS, 65));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("PFAB", buff1, 4);

    // Weight = 4097 bytes | weighted = true, extended = true, 3 sextets
    TEST_ASSERT_EQUAL(5, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED, 4097));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("BHAAB", buff1, 5);
    TEST_ASSERT_EQUAL(5, graph7_encode_header(buff1, order, GRAPH7_DIRECTED, 4097));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("DHAAB", buff1, 5);
    TEST_ASSERT_EQUAL(5, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED_LOOPS, 4097));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("NHAAB", buff1, 5);
    TEST_ASSERT_EQUAL(5, graph7_encode_header(buff1, order, GRAPH7_DIRECTED_LOOPS, 4097));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("PHAAB", buff1, 5);

    // Weight = 262145 bytes | weighted = true, extended = true, 4 sextets
    TEST_ASSERT_EQUAL(6, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED, 262145));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("BJAAAB", buff1, 6);
    TEST_ASSERT_EQUAL(6, graph7_encode_header(buff1, order, GRAPH7_DIRECTED, 262145));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("DJAAAB", buff1, 6);
    TEST_ASSERT_EQUAL(6, graph7_encode_header(buff1, order, GRAPH7_UNDIRECTED_LOOPS, 262145));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("NJAAAB", buff1, 6);
    TEST_ASSERT_EQUAL(6, graph7_encode_header(buff1, order, GRAPH7_DIRECTED_LOOPS, 262145));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("PJAAAB", buff1, 6);
}

void test_graph7_decode_header(void)
{
    // Invalid arguments
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_ARG, graph7_decode_header(NULL, 1, NULL, NULL, NULL));
    TEST_ASSERT_EQUAL(-GRAPH7_INVALID_LENGTH, graph7_decode_header("!", 0, NULL, NULL, NULL));

    size_t width;
    graph7_gtype_t gtype;
    uint8_t tail;

    // Trivial graph | weigted = false, extended = false
    // Undirected
    TEST_ASSERT_EQUAL(1, graph7_decode_header("A", 1, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(GRAPH7_COMPACT_ENCODING, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed
    TEST_ASSERT_EQUAL(1, graph7_decode_header("C", 1, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(GRAPH7_COMPACT_ENCODING, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Undirected with loops
    TEST_ASSERT_EQUAL(1, graph7_decode_header("M", 1, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(GRAPH7_COMPACT_ENCODING, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);
    // Directed with loops
    TEST_ASSERT_EQUAL(1, graph7_decode_header("O", 1, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(GRAPH7_COMPACT_ENCODING, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);

    // Weight = 1 byte | weigted = true, extended = false
    // Undirected
    TEST_ASSERT_EQUAL(2, graph7_decode_header("BA", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(1, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed
    TEST_ASSERT_EQUAL(2, graph7_decode_header("DA", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(1, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Undirected with loops
    TEST_ASSERT_EQUAL(2, graph7_decode_header("VA", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(1, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(2, tail);
    // Directed with loops
    TEST_ASSERT_EQUAL(2, graph7_decode_header("XA", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(1, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(2, tail);

    // Weight = 32 bytes | weigted = true, extended = false
    // Undirected
    TEST_ASSERT_EQUAL(2, graph7_decode_header("B+", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(32, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed
    TEST_ASSERT_EQUAL(2, graph7_decode_header("D+", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(32, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Undirected with loops
    TEST_ASSERT_EQUAL(2, graph7_decode_header("N+", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(32, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);
    // Directed with loops
    TEST_ASSERT_EQUAL(2, graph7_decode_header("P+", 2, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(32, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);

    // Weight = 33 bytes | weighted = true, extended = true, 1 sextet
    // Undirected
    TEST_ASSERT_EQUAL(3, graph7_decode_header("BDg", 3, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(33, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed
    TEST_ASSERT_EQUAL(3, graph7_decode_header("DDg", 3, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(33, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Undirected with loops
    TEST_ASSERT_EQUAL(3, graph7_decode_header("FDg", 3, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(33, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed with loops
    TEST_ASSERT_EQUAL(3, graph7_decode_header("HDg", 3, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(33, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(0, tail);

    // Weight = 65 bytes | weighted = true, extended = true, 2 sextets
    // Undirected
    TEST_ASSERT_EQUAL(4, graph7_decode_header("BFAB", 4, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(65, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed
    TEST_ASSERT_EQUAL(4, graph7_decode_header("DFAB", 4, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(65, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Undirected with loops
    TEST_ASSERT_EQUAL(4, graph7_decode_header("NFAB", 4, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(65, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);
    // Directed with loops
    TEST_ASSERT_EQUAL(4, graph7_decode_header("PFAB", 4, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(65, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);

    // Weight = 4097 bytes | weighted = true, extended = true, 3 sextets
    // Undirected
    TEST_ASSERT_EQUAL(5, graph7_decode_header("BHAAB", 5, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(4097, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed
    TEST_ASSERT_EQUAL(5, graph7_decode_header("DHAAB", 5, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(4097, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Undirected with loops
    TEST_ASSERT_EQUAL(5, graph7_decode_header("NHAAB", 5, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(4097, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);
    // Directed with loops
    TEST_ASSERT_EQUAL(5, graph7_decode_header("PHAAB", 5, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(4097, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);

    // Weight = 262145 bytes | weighted = true, extended = true, 4 sextets
    // Undirected
    TEST_ASSERT_EQUAL(6, graph7_decode_header("BJAAAB", 6, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(262145, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Directed
    TEST_ASSERT_EQUAL(6, graph7_decode_header("DJAAAB", 6, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(262145, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED, gtype);
    TEST_ASSERT_EQUAL(0, tail);
    // Undirected with loops
    TEST_ASSERT_EQUAL(6, graph7_decode_header("NJAAAB", 6, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(262145, width);
    TEST_ASSERT_EQUAL(GRAPH7_UNDIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);
    // Directed with loops
    TEST_ASSERT_EQUAL(6, graph7_decode_header("PJAAAB", 6, &gtype, &width, &tail));
    TEST_ASSERT_EQUAL(262145, width);
    TEST_ASSERT_EQUAL(GRAPH7_DIRECTED_LOOPS, gtype);
    TEST_ASSERT_EQUAL(1, tail);
}

void test_graph7_encode_decode(void)
{
    //==============================================================================
    // Compact
    for(size_t order = 2; order < MAX_ORDER + 1; order++)
    {
        size_t bytes;

        // Undirected
        weighted_graph(buff1, order, 1, 2, false, false);
        bytes = 1 + utils_ceiling_div(order * (order - 1) / 2, 6);

        TEST_ASSERT_EQUAL(
            bytes,
            graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_UNDIRECTED, GRAPH7_COMPACT_ENCODING)
        );
        TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);

        // Directed
        weighted_graph(buff1, order, 1, 2, true, false);
        bytes = 1 + utils_ceiling_div(order * (order - 1), 6);

        TEST_ASSERT_EQUAL(
            bytes,
            graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_DIRECTED, GRAPH7_COMPACT_ENCODING)
        );
        TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);

        // Undirected with loops
        weighted_graph(buff1, order, 1, 2, false, true);
        bytes = 1 + utils_ceiling_div(order * (order + 1) / 2, 6);

        TEST_ASSERT_EQUAL(
            bytes,
            graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_UNDIRECTED_LOOPS, GRAPH7_COMPACT_ENCODING)
        );
        TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);

        // Directed with loops
        weighted_graph(buff1, order, 1, 2, true, true);
        bytes = 1 + utils_ceiling_div(order * order, 6);

        TEST_ASSERT_EQUAL(
            bytes,
            graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_DIRECTED_LOOPS, GRAPH7_COMPACT_ENCODING)
        );
        TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);
    }
    //==============================================================================
    // Weighted
    for(size_t width = 1; width < MAX_WIDTH + 1; width++)
    {
        size_t offset = 2;
        if(width > 32)
            offset += 1;
        if(width > 64)
            offset += 1;

        for(size_t order = 2; order < MAX_ORDER + 1; order++)
        {
            size_t bytes;

            // Undirected
            bytes = offset + utils_ceiling_div(width * 8 * (order * (order - 1) / 2), 6);
            weighted_graph(buff1, order, width, 256, false, false);
            TEST_ASSERT_EQUAL(bytes, graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_UNDIRECTED, width));
            TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);

            // Directed
            bytes = offset + utils_ceiling_div(width * 8 * (order * (order - 1)), 6);
            weighted_graph(buff1, order, width, 256, true, false);
            TEST_ASSERT_EQUAL(bytes, graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_DIRECTED, width));
            TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);

            // Undirected with loops
            bytes = offset + utils_ceiling_div(width * 8 * (order * (order + 1) / 2), 6);
            weighted_graph(buff1, order, width, 256, false, true);
            TEST_ASSERT_EQUAL(bytes, graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_UNDIRECTED_LOOPS, width));
            TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);

            // Directed with loops
            bytes = offset + utils_ceiling_div(width * 8 * (order * order), 6);
            weighted_graph(buff1, order, width, 256, false, true);
            TEST_ASSERT_EQUAL(bytes, graph7_encode_from_matrix(buff2, buff1, order, GRAPH7_DIRECTED_LOOPS, width));
            TEST_ASSERT_EQUAL(order, graph7_decode_to_matrix(buff3, buff2, bytes));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(buff1, buff3, order * order);
        }
    }
}
