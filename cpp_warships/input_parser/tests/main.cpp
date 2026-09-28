#include <gtest/gtest.h>

#include <string>

TEST(Group1, Test1) {
    EXPECT_EQ("Input input_parser::model::Parser Tests", "Input input_parser::model::Parser Tests");
}

TEST(Group1, Test2) {
    ASSERT_FLOAT_EQ(3.0F, 1.0F + 2.0F);
}