#include <gtest/gtest.h>


TEST(Simple, Test)
{
    int res = 2 + 3;
    EXPECT_EQ(res, 5);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
