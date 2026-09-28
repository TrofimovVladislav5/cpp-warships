#include <application/core/Coordinate.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <functional>
#include <unordered_set>

namespace cpp_warships::core {
    TEST(CoordinateTests, DefaultsToOrigin) {
        const Coordinate coordinate;

        EXPECT_EQ(coordinate.x, 0);
        EXPECT_EQ(coordinate.y, 0);
    }

    TEST(CoordinateTests, ComparesMemberwise) {
        EXPECT_EQ((Coordinate{3, 4}), (Coordinate{3, 4}));
        EXPECT_NE((Coordinate{3, 4}), (Coordinate{4, 3}));
    }

    TEST(CoordinateTests, FlattensRowByRow) {
        EXPECT_EQ(flattenToSingleAxis({0, 0}), 0U);
        EXPECT_EQ(flattenToSingleAxis({7, 0}), 7U);
        EXPECT_EQ(flattenToSingleAxis({0, 1}), COORDINATE_ROW_SPAN);
        EXPECT_EQ(flattenToSingleAxis({2, 3}), 3 * COORDINATE_ROW_SPAN + 2);
    }

    TEST(CoordinateTests, GivesEveryCellOfASupportedBoardItsOwnNumber) {
        constexpr int LARGEST_SUPPORTED_BOARD_SIZE = 20;
        std::unordered_set<std::size_t> seenNumbers;

        for (int row = 0; row < LARGEST_SUPPORTED_BOARD_SIZE; ++row) {
            for (int column = 0; column < LARGEST_SUPPORTED_BOARD_SIZE; ++column) {
                seenNumbers.insert(flattenToSingleAxis({column, row}));
            }
        }

        EXPECT_EQ(seenNumbers.size(), LARGEST_SUPPORTED_BOARD_SIZE * LARGEST_SUPPORTED_BOARD_SIZE);
    }

    TEST(CoordinateTests, HashesThroughTheStandardHashSpecialisation) {
        const std::hash<Coordinate> hasher;

        EXPECT_EQ(hasher({5, 6}), flattenToSingleAxis({5, 6}));
        EXPECT_EQ(hasher({5, 6}), hasher({5, 6}));
    }

    TEST(CoordinateTests, WorksAsAnUnorderedSetKey) {
        std::unordered_set<Coordinate> cells;
        cells.insert({1, 2});
        cells.insert({1, 2});
        cells.insert({2, 1});

        EXPECT_EQ(cells.size(), 2U);
        EXPECT_TRUE(cells.contains({1, 2}));
        EXPECT_FALSE(cells.contains({9, 9}));
    }
}  // namespace cpp_warships::core
