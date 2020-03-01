#include "gtest/gtest.h"

extern "C"
{
#include "../Inc/sum.h"
}

TEST(TestMySum, Test_MySumBasic)
{
    int rv = intAdd(3, 4);
    ASSERT_EQ(7, rv);
}
