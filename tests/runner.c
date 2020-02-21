#include <unity.h>

void setUp (void) {} /* Is run before every test, put unit init calls here. */
void tearDown (void) {} /* Is run after every test, put unit clean-up calls here. */

// graph6 submodule
void test_graph7_graph6_order_encode(void);
void test_graph7_graph6_order_decode(void);
void test_graph7_graph6_encode_decode(void);

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_graph7_graph6_order_encode);
    RUN_TEST(test_graph7_graph6_order_decode);
    RUN_TEST(test_graph7_graph6_encode_decode);

    return UNITY_END();
}
