#include <application/core/Board.h>
#include <application/core/Direction.h>
#include <application/core/FleetComposition.h>
#include <application/core/Segment.h>
#include <application/core/Ship.h>
#include <application/flow/PlacementPlan.h>
#include <application/flow/RandomEngine.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <map>

namespace cpp_warships::flow {
    namespace {
        constexpr unsigned int FIXED_SEED = 4242U;

        core::FleetComposition twoSinglesAndOnePair() {
            return core::FleetComposition{std::map<int, int>{{1, 2}, {2, 1}}};
        }
    }  // namespace

    TEST(PlacementPlanTests, AnEmptyBoardStillOwesTheWholeFleet) {
        const core::Board board{10, 10};
        const PlacementPlan plan{twoSinglesAndOnePair(), board};

        EXPECT_EQ(plan.remainingOf(1), 2);
        EXPECT_EQ(plan.remainingOf(2), 1);
        EXPECT_EQ(plan.remainingShipCount(), 3);
        EXPECT_FALSE(plan.isComplete());
    }

    TEST(PlacementPlanTests, PlacedShipsCountAgainstWhatIsOwed) {
        core::Board board{10, 10};
        board.place({0, 0}, core::Direction::Horizontal, 1);
        const PlacementPlan plan{twoSinglesAndOnePair(), board};

        EXPECT_EQ(plan.remainingOf(1), 1);
        EXPECT_EQ(plan.remainingOf(2), 1);
        EXPECT_EQ(plan.remainingShipCount(), 2);
    }

    TEST(PlacementPlanTests, AFullyPlacedFleetIsComplete) {
        core::Board board{10, 10};
        board.place({0, 0}, core::Direction::Horizontal, 1);
        board.place({2, 0}, core::Direction::Horizontal, 1);
        board.place({4, 0}, core::Direction::Horizontal, 2);
        const PlacementPlan plan{twoSinglesAndOnePair(), board};

        EXPECT_TRUE(plan.isComplete());
        EXPECT_EQ(plan.remainingShipCount(), 0);
    }

    TEST(PlacementPlanTests, NeverOwesLessThanNothing) {
        core::Board board{10, 10};
        board.place({0, 0}, core::Direction::Horizontal, 1);
        board.place({2, 0}, core::Direction::Horizontal, 1);
        board.place({4, 0}, core::Direction::Horizontal, 1);
        const PlacementPlan plan{core::FleetComposition{std::map<int, int>{{1, 2}}}, board};

        EXPECT_EQ(plan.remainingOf(1), 0);
        EXPECT_EQ(plan.remainingShipCount(), 0);
    }

    TEST(PlacementPlanTests, IgnoresLengthsTheFleetNeverAskedFor) {
        core::Board board{10, 10};
        board.place({0, 0}, core::Direction::Horizontal, 4);
        const PlacementPlan plan{core::FleetComposition{std::map<int, int>{{1, 1}}}, board};

        EXPECT_EQ(plan.remainingOf(1), 1);
        EXPECT_EQ(plan.remainingOf(4), 0);
    }

    TEST(PlacementPlanTests, ExposesTheWholeTableOfWhatIsLeft) {
        const core::Board board{10, 10};
        const PlacementPlan plan{twoSinglesAndOnePair(), board};

        const std::map<int, int> expected{{1, 2}, {2, 1}};
        EXPECT_EQ(plan.remaining(), expected);
    }

    TEST(PlacementPlanTests, AnEmptyFleetIsAlreadyComplete) {
        const core::Board board{10, 10};
        const PlacementPlan plan{core::FleetComposition{}, board};

        EXPECT_TRUE(plan.isComplete());
    }

    TEST(PlaceFleetRandomlyTests, LaysOutTheWholeFleetLegally) {
        core::Board board{10, 10};
        RandomEngine randomEngine{FIXED_SEED};
        const core::FleetComposition fleet = core::FleetComposition::forBoardSize(10);

        ASSERT_TRUE(placeFleetRandomly(board, fleet, randomEngine, core::DEFAULT_SEGMENT_HEALTH));

        const PlacementPlan plan{fleet, board};
        EXPECT_TRUE(plan.isComplete());
        EXPECT_EQ(static_cast<int>(board.ships().size()), fleet.totalShips());
    }

    TEST(PlaceFleetRandomlyTests, GivesEverySegmentTheHealthAskedFor) {
        core::Board board{10, 10};
        RandomEngine randomEngine{FIXED_SEED};

        ASSERT_TRUE(placeFleetRandomly(board, twoSinglesAndOnePair(), randomEngine, 5));

        for (const core::Ship& ship : board.ships()) {
            for (int index = 0; index < ship.length(); ++index) {
                EXPECT_EQ(ship.segmentHealth(index), 5);
            }
        }
    }

    TEST(PlaceFleetRandomlyTests, ClearsWhateverWasThereBefore) {
        core::Board board{10, 10};
        board.place({0, 0}, core::Direction::Horizontal, 3);
        board.attack({9, 9}, 1);
        RandomEngine randomEngine{FIXED_SEED};

        ASSERT_TRUE(placeFleetRandomly(board, twoSinglesAndOnePair(), randomEngine, 1));

        EXPECT_EQ(board.ships().size(), 3U);
        EXPECT_TRUE(board.attackedCells().empty());
    }

    TEST(PlaceFleetRandomlyTests, LeavesTheBoardEmptyWhenTheFleetCannotFit) {
        core::Board board{2, 2};
        RandomEngine randomEngine{FIXED_SEED};
        const core::FleetComposition tooLarge{std::map<int, int>{{4, 3}}};

        EXPECT_FALSE(placeFleetRandomly(board, tooLarge, randomEngine, 1));
        EXPECT_FALSE(board.hasShips());
    }

    TEST(PlaceFleetRandomlyTests, TheSameSeedLaysOutTheSameFleet) {
        core::Board firstBoard{10, 10};
        core::Board secondBoard{10, 10};
        RandomEngine firstEngine{FIXED_SEED};
        RandomEngine secondEngine{FIXED_SEED};

        placeFleetRandomly(firstBoard, twoSinglesAndOnePair(), firstEngine, 1);
        placeFleetRandomly(secondBoard, twoSinglesAndOnePair(), secondEngine, 1);

        ASSERT_EQ(firstBoard.ships().size(), secondBoard.ships().size());
        for (std::size_t index = 0; index < firstBoard.ships().size(); ++index) {
            EXPECT_EQ(firstBoard.ships()[index].origin(), secondBoard.ships()[index].origin());
            EXPECT_EQ(
                firstBoard.ships()[index].direction(),
                secondBoard.ships()[index].direction()
            );
        }
    }
}  // namespace cpp_warships::flow
