#include <application/core/FleetComposition.h>
#include <gtest/gtest.h>

#include <map>

namespace cpp_warships::core {
    TEST(FleetCompositionTests, DefaultsToNoShips) {
        const FleetComposition fleet;

        EXPECT_TRUE(fleet.isEmpty());
        EXPECT_EQ(fleet.totalShips(), 0);
        EXPECT_EQ(fleet.totalCells(), 0);
    }

    TEST(FleetCompositionTests, ReportsTheCountsItWasBuiltFrom) {
        const FleetComposition fleet{std::map<int, int>{{1, 4}, {3, 2}}};

        EXPECT_FALSE(fleet.isEmpty());
        EXPECT_EQ(fleet.countOf(1), 4);
        EXPECT_EQ(fleet.countOf(3), 2);
        EXPECT_EQ(fleet.countOf(2), 0);
    }

    TEST(FleetCompositionTests, SumsShipsAndOccupiedCells) {
        const FleetComposition fleet{std::map<int, int>{{1, 4}, {3, 2}}};

        EXPECT_EQ(fleet.totalShips(), 6);
        EXPECT_EQ(fleet.totalCells(), 4 * 1 + 2 * 3);
    }

    TEST(FleetCompositionTests, ExposesTheWholeTable) {
        const std::map<int, int> counts{{2, 3}};
        const FleetComposition fleet{counts};

        EXPECT_EQ(fleet.countsByLength(), counts);
    }

    TEST(FleetCompositionTests, ScalesTheReferenceFleetForATenByTenBoard) {
        const FleetComposition fleet = FleetComposition::forBoardSize(10);

        EXPECT_EQ(fleet.countOf(4), 1);
        EXPECT_EQ(fleet.countOf(3), 2);
        EXPECT_EQ(fleet.countOf(2), 3);
        EXPECT_EQ(fleet.countOf(1), 4);
        EXPECT_EQ(fleet.totalShips(), 10);
    }

    TEST(FleetCompositionTests, KeepsTheFleetInsideItsShareOfTheBoard) {
        for (const int boardSize : {8, 10, 12, 15, 20}) {
            const FleetComposition fleet = FleetComposition::forBoardSize(boardSize);
            const int boardCells = boardSize * boardSize;

            EXPECT_LE(fleet.totalCells(), boardCells / 5) << "board size " << boardSize;
        }
    }

    TEST(FleetCompositionTests, GrowsTheFleetWithTheBoard) {
        const FleetComposition small = FleetComposition::forBoardSize(8);
        const FleetComposition large = FleetComposition::forBoardSize(20);

        EXPECT_LT(small.totalCells(), large.totalCells());
    }

    TEST(FleetCompositionTests, GivesNoFleetForANonPositiveBoard) {
        EXPECT_TRUE(FleetComposition::forBoardSize(0).isEmpty());
        EXPECT_TRUE(FleetComposition::forBoardSize(-7).isEmpty());
    }

    TEST(FleetCompositionTests, NeverAsksForANegativeNumberOfShips) {
        for (int boardSize = 1; boardSize <= 20; ++boardSize) {
            const FleetComposition fleet = FleetComposition::forBoardSize(boardSize);

            for (const auto& [length, count] : fleet.countsByLength()) {
                EXPECT_GE(count, 0) << "board size " << boardSize << ", length " << length;
            }
        }
    }
}  // namespace cpp_warships::core
