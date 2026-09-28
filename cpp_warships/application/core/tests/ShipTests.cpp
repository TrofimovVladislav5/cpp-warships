#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/core/Segment.h>
#include <application/core/Ship.h>
#include <gtest/gtest.h>

#include <optional>
#include <stdexcept>
#include <vector>

namespace cpp_warships::core {
    TEST(ShipTests, LaysSegmentsOutAlongItsDirection) {
        const Ship horizontal{{2, 5}, Direction::Horizontal, 3};
        const Ship vertical{{2, 5}, Direction::Vertical, 3};

        EXPECT_EQ(horizontal.coordinateAt(0), (Coordinate{2, 5}));
        EXPECT_EQ(horizontal.coordinateAt(2), (Coordinate{4, 5}));
        EXPECT_EQ(vertical.coordinateAt(0), (Coordinate{2, 5}));
        EXPECT_EQ(vertical.coordinateAt(2), (Coordinate{2, 7}));
    }

    TEST(ShipTests, ReportsItsOriginDirectionAndLength) {
        const Ship ship{{1, 1}, Direction::Vertical, 4};

        EXPECT_EQ(ship.origin(), (Coordinate{1, 1}));
        EXPECT_EQ(ship.direction(), Direction::Vertical);
        EXPECT_EQ(ship.length(), 4);
        EXPECT_EQ(ship.segments().size(), 4U);
    }

    TEST(ShipTests, ListsEveryCoordinateItCovers) {
        const Ship ship{{0, 0}, Direction::Horizontal, 3};

        const std::vector<Coordinate> expected{{0, 0}, {1, 0}, {2, 0}};
        EXPECT_EQ(ship.coordinates(), expected);
    }

    TEST(ShipTests, RejectsAnOutOfRangeCoordinateIndex) {
        const Ship ship{{0, 0}, Direction::Horizontal, 2};

        EXPECT_THROW((void)ship.coordinateAt(-1), std::out_of_range);
        EXPECT_THROW((void)ship.coordinateAt(2), std::out_of_range);
        EXPECT_THROW((void)ship.segmentHealth(2), std::out_of_range);
    }

    TEST(ShipTests, FindsTheSegmentCoveringACoordinate) {
        const Ship ship{{3, 3}, Direction::Horizontal, 3};

        EXPECT_EQ(ship.segmentIndexAt({3, 3}), std::optional<int>{0});
        EXPECT_EQ(ship.segmentIndexAt({5, 3}), std::optional<int>{2});
        EXPECT_EQ(ship.segmentIndexAt({6, 3}), std::nullopt);
        EXPECT_EQ(ship.segmentIndexAt({2, 3}), std::nullopt);
        EXPECT_EQ(ship.segmentIndexAt({3, 4}), std::nullopt);
    }

    TEST(ShipTests, OccupiesOnlyItsOwnCells) {
        const Ship ship{{3, 3}, Direction::Vertical, 2};

        EXPECT_TRUE(ship.occupies({3, 3}));
        EXPECT_TRUE(ship.occupies({3, 4}));
        EXPECT_FALSE(ship.occupies({3, 5}));
        EXPECT_FALSE(ship.occupies({4, 3}));
    }

    TEST(ShipTests, DamagesTheAddressedSegmentAlone) {
        Ship ship{{0, 0}, Direction::Horizontal, 2, 2};

        EXPECT_TRUE(ship.damageSegment(0, 1));
        EXPECT_EQ(ship.segmentHealth(0), 1);
        EXPECT_EQ(ship.segmentHealth(1), 2);
    }

    TEST(ShipTests, RefusesToDamageASegmentOutOfRange) {
        Ship ship{{0, 0}, Direction::Horizontal, 2};

        EXPECT_FALSE(ship.damageSegment(-1, 1));
        EXPECT_FALSE(ship.damageSegment(2, 1));
    }

    TEST(ShipTests, SinksOnlyWhenEverySegmentIsDestroyed) {
        Ship ship{{0, 0}, Direction::Horizontal, 2, 1};

        EXPECT_FALSE(ship.isSunk());

        ship.damageSegment(0, 1);
        EXPECT_FALSE(ship.isSunk());

        ship.damageSegment(1, 1);
        EXPECT_TRUE(ship.isSunk());
    }

    TEST(ShipTests, RestoresPartlyDamagedSegments) {
        std::vector<Segment> segments;
        segments.emplace_back(2, 0);
        segments.emplace_back(2, 1);
        const Ship ship{{4, 4}, Direction::Vertical, std::move(segments)};

        EXPECT_EQ(ship.length(), 2);
        EXPECT_EQ(ship.segmentHealth(0), 0);
        EXPECT_EQ(ship.segmentHealth(1), 1);
        EXPECT_FALSE(ship.isSunk());
    }

    TEST(ShipTests, UsesTheDefaultSegmentHealthWhenNoneIsGiven) {
        const Ship ship{{0, 0}, Direction::Horizontal, 1};

        EXPECT_EQ(ship.segmentHealth(0), DEFAULT_SEGMENT_HEALTH);
    }
}  // namespace cpp_warships::core
