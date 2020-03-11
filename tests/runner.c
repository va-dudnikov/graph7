#include <unity.h>
#include <stdlib.h>

void setUp (void) {} /* Is run before every test, put unit init calls here. */
void tearDown (void) {} /* Is run after every test, put unit clean-up calls here. */

// graph6
void test_graph6_order_encode(void);
void test_graph6_order_decode(void);
void test_graph6_encode(void);
void test_graph6_decode(void);
void test_graph6_encode_decode(void);

// digraph6
void test_digraph6_encode(void);
void test_digraph6_decode(void);
void test_digraph6_encode_decode(void);

// sparse6
void test_sparse6_encode(void);
void test_sparse6_decode(void);
void test_sparse_encode_decode(void);

// graph7
void test_graph7_encode_decode(void);
void test_graph7_decode_header(void);
void test_graph7_encode_header(void);

int main(void)
{
    srand(42);
    UNITY_BEGIN();

    // graph6
    RUN_TEST(test_graph6_order_encode);
    RUN_TEST(test_graph6_order_decode);
    RUN_TEST(test_graph6_encode);
    RUN_TEST(test_graph6_decode);
    RUN_TEST(test_graph6_encode_decode);

    // digraph6
    RUN_TEST(test_digraph6_encode);
    RUN_TEST(test_digraph6_decode);
    RUN_TEST(test_digraph6_encode_decode);

    // sparse6
    RUN_TEST(test_sparse6_encode);
    RUN_TEST(test_sparse6_decode);
    RUN_TEST(test_sparse_encode_decode);

    // graph7
    RUN_TEST(test_graph7_encode_header);
    RUN_TEST(test_graph7_decode_header);
    RUN_TEST(test_graph7_encode_decode);

    return UNITY_END();
}
