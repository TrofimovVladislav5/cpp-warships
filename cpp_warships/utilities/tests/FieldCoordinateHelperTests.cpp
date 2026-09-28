#include <gtest/gtest.h>
#include <utilities/FieldCoordinateHelper.h>

#include <vector>

namespace {
    const std::vector<FieldCoordinate> COORDINATES{{1, 9}, {5, 2}, {3, 7}};
}  // namespace

TEST(FieldCoordinateHelperTests, FindsTheLargestFirstValue) {
    EXPECT_EQ(FieldCoordinateHelper::findMaxPairValue(COORDINATES, true), 5);
}

TEST(FieldCoordinateHelperTests, FindsTheLargestSecondValue) {
    EXPECT_EQ(FieldCoordinateHelper::findMaxPairValue(COORDINATES, false), 9);
}

TEST(FieldCoordinateHelperTests, FindsTheSmallestFirstValue) {
    EXPECT_EQ(FieldCoordinateHelper::findMinPairValue(COORDINATES, true), 1);
}

TEST(FieldCoordinateHelperTests, FindsTheSmallestSecondValue) {
    EXPECT_EQ(FieldCoordinateHelper::findMinPairValue(COORDINATES, false), 2);
}

TEST(FieldCoordinateHelperTests, AnEmptyListHasNoLargestOrSmallest) {
    const std::vector<FieldCoordinate> empty;

    EXPECT_EQ(FieldCoordinateHelper::findMaxPairValue(empty, true), -1);
    EXPECT_EQ(FieldCoordinateHelper::findMinPairValue(empty, false), -1);
}

TEST(FieldCoordinateHelperTests, ASingleCoordinateIsBothLargestAndSmallest) {
    const std::vector<FieldCoordinate> single{{4, 6}};

    EXPECT_EQ(FieldCoordinateHelper::findMaxPairValue(single, true), 4);
    EXPECT_EQ(FieldCoordinateHelper::findMinPairValue(single, true), 4);
    EXPECT_EQ(FieldCoordinateHelper::findMaxPairValue(single, false), 6);
    EXPECT_EQ(FieldCoordinateHelper::findMinPairValue(single, false), 6);
}

TEST(FieldCoordinateHelperTests, CopesWithNegativeCoordinates) {
    const std::vector<FieldCoordinate> negatives{{-5, -1}, {-2, -8}};

    EXPECT_EQ(FieldCoordinateHelper::findMaxPairValue(negatives, true), -2);
    EXPECT_EQ(FieldCoordinateHelper::findMinPairValue(negatives, true), -5);
    EXPECT_EQ(FieldCoordinateHelper::findMaxPairValue(negatives, false), -1);
    EXPECT_EQ(FieldCoordinateHelper::findMinPairValue(negatives, false), -8);
}
