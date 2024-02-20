#include <Averager.h>
#include "unity.h"

void testAverager()
{
    Averager<int, long, 10> TestAverager;
    TEST_ASSERT_EQUAL(0, TestAverager.Average());
    for (int i = 0; i < 20; ++i)
    {
        TEST_ASSERT_EQUAL(5, TestAverager.push(5));
        TEST_ASSERT_EQUAL(5, TestAverager.Average());
    }
    TEST_ASSERT_EQUAL(50, TestAverager.Sum());
}

void TestAverager(void)
{
    RUN_TEST(testAverager);
}